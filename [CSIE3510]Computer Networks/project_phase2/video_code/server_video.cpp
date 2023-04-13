#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <deque>

using namespace std;
using namespace cv;

const size_t sending_threads_num = 2;
const std::string default_filename = "./sing.mov";
int default_port = 8886;
int max_queue_size = 5;

typedef std::deque< std::shared_ptr<cv::Mat> > FrameQueue;

std::mutex frame_queues_guard;
std::vector<FrameQueue> frame_queues;


void read_file(const std::string& filename) {
    cv::VideoCapture source(filename);

    assert(frame_queues.size() == sending_threads_num);

    if(!source.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        exit(-1);
    }
    bool stop_now = false;

    uint64_t timestamp = 0;
    while(source.isOpened()) {
        size_t queue_id = timestamp % sending_threads_num;
        {
            std::lock_guard<std::mutex> lock(frame_queues_guard);
            if (frame_queues[queue_id].size() >= max_queue_size)
                continue;
        }

        std::shared_ptr<cv::Mat> frame(new cv::Mat());
        bool ret = source.read(*frame);
        
        if (!ret && frame->empty()) {//Video is over
            std::cerr << "Video is over\n";
            stop_now = true; // wait putting empty "deactivation frame" to the queue to stop other threads
        }else{
            std::lock_guard<std::mutex> lock(frame_queues_guard);
            if (!stop_now){
                frame_queues[queue_id].push_back(frame);
            }//else {
            //     for(int i = 0; i < sending_threads_num; ++i){
            //         frame_queues[i].push_back(frame);
            //     }
            // }
        }

        if (stop_now)
            break;
        ++timestamp;
    }
    source.release();
}


void play_to_network(int socket, int debug_thread_id) {
    int bytes = 0;
    while (1) {
        std::lock_guard<std::mutex> lock(frame_queues_guard);
        if (frame_queues[debug_thread_id].empty())//No frames to send in this queue
            continue;

        std::shared_ptr<cv::Mat> frame_to_send = frame_queues[debug_thread_id].front();
        frame_queues[debug_thread_id].pop_front();
        if (frame_to_send->empty()) // No more frames to send now!
            break;

        int frame_length = frame_to_send->total() * frame_to_send->elemSize();
        if ((bytes = send(socket, frame_to_send->data, frame_length, 0)) < 0) {
            std::cerr << "bytes = " << bytes << std::endl;
            break;
        }
    }
}


int main(int argc, char** argv)
{
    std::string filename = default_filename;

    int local_socket;
    int remote_socket;
    int port = default_port;

    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    int addr_len = sizeof(struct sockaddr_in);

    if ( (argc > 1) && (strcmp(argv[1],"-h") == 0) ) {
          std::cerr << "usage: ./server <port> <file or stream>\n" <<
                       "port           : socket port (4097 default)\n" <<
                       "file or stream : (0 default)\n" << std::endl;

          exit(1);
    }

    if (argc >= 2) port = atoi(argv[1]);
    if (argc == 3){
        filename = argv[2];
    }
    VideoCapture cap(filename);
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    bool size_sent = false;
    int size_buffer[3]; 
    size_buffer[0] = height; size_buffer[1] = width;
    cout << "height: " << size_buffer[0] << "\n";
    cout << "width: " << size_buffer[1] << "\n";

    local_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (local_socket == -1){
         perror("socket() call failed!!");
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons( port );

    if( bind(local_socket,(struct sockaddr *)&local_addr , sizeof(local_addr)) < 0) {
         perror("Can't bind() socket");
         exit(1);
    }

    listen(local_socket , 3);
    std::cout <<  "Waiting for connections...\n"
              <<  "Server Port:" << port << std::endl;

    for (size_t i = 0; i < sending_threads_num; ++i) {
        std::lock_guard<std::mutex> lock(frame_queues_guard);
        frame_queues.push_back(FrameQueue());
    }

    std::vector<std::thread> sending_threads;

    // support now only 1 client, don't assume simultaneous connections from multiple clients.
    while (1) {
        remote_socket = accept(local_socket, (struct sockaddr *)&remote_addr, (socklen_t*)&addr_len);
        if (remote_socket < 0) {
            perror("accept failed!");
            exit(1);
        }
        std::cout << "Connection accepted" << std::endl;

        //At first, send the frame size to client
        if(!size_sent){
            if(send(remote_socket, size_buffer, sizeof(size_buffer), 0) < 0){
                cerr << "Can't send frame size!" << "\n";
            }else{
                size_sent = true;
            }
        }

        std::thread read_thread(read_file, filename);

        for (size_t i = 0; i < sending_threads_num; ++i) {
            assert(frame_queues.size() > i);
            assert(sending_threads.size() == i);
            std::thread t(play_to_network, remote_socket, (int)i);
            sending_threads.push_back(std::move(t));
        }

        for (size_t i = 0; i < sending_threads_num; ++i) {
            sending_threads[i].join();
        }
        sending_threads.clear();

        read_thread.join();
        std::cerr << "Wait for new connections\n";
    }

    close(remote_socket);
    close(local_socket);

    return 0;
}
