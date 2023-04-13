#include <arpa/inet.h>
// #include <dirent.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <memory.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include<opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <deque>

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)
#define MSGMAX 10000

using namespace std;
using namespace cv;

#define LOGINLEN 16
#define REGISTERLEN 24
#define BOARDLEN 7

char process_buf[MSGMAX + 10];

typedef struct package {
    int type, buf_size;
    char sender[256];
    char password[28];
    char reqpath[256];
    char message[256];
    char buf[2048];
    time_t Time;
} package;

typedef struct request {
    int conn_fd;
    package pkg;
} request;

// typedef struct server {
//     string hostname;
//     unsigned short port;
//     int listen_fd;
// } server;

// server tcpsvr;
struct sockaddr_in frontendAddr, backendAddr;
int backend_fd, maxfd;
int client_len;

request *request_array;

//Below are for video frames transferring
const size_t sending_threads_num = 2;
const string default_filename_video = "./sample.mov";
const string default_filename_audio = "./sample.m4a";
int max_queue_size = 5;

typedef deque< shared_ptr<Mat> > FrameQueue;
mutex frame_queues_guard;
vector<FrameQueue> frame_queues;

static void init_server(unsigned short port) {
    int tmp;

    gethostname(process_buf, sizeof(process_buf));
    // tcpsvr.hostname = (string)process_buf;
    // tcpsvr.port = port;
    backend_fd = socket(AF_INET, SOCK_STREAM, 0);
    // tcpsvr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    // backend_fd = httpsvr.listen_fd;
    if(backend_fd < 0) {
        ERR_EXIT("socket");
    }

    bzero(&backendAddr, sizeof(backendAddr));
    backendAddr.sin_family = AF_INET;
    backendAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    backendAddr.sin_port = htons((unsigned short)port);

    tmp = 1;//bool variable
    //setsockopt means the operation when use socket, SO_REUSEADDR means reuse address when close socket
    if(setsockopt(backend_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if(bind(backend_fd, (struct sockaddr*)&backendAddr, sizeof(backendAddr))) {
        ERR_EXIT("bind");
    }
    if(listen(backend_fd, 1024) < 0) {//allow 1024 clients
        ERR_EXIT("listen");
    }

    cout << "TCP server listen on port: " << port << ", fd: " << backend_fd << '\n';

    maxfd = getdtablesize();
    request_array = (request *)malloc(sizeof(request) * maxfd);
}

void init_database() {
    remove("database");
    mkdir("database", 0777);

    ofstream user_info("./database/user_passwd.txt");
    user_info << "";
    user_info.close();

    ofstream comments("./database/comments.txt");
    comments << "";
    comments.close();
}

static void init_request(request *reqP) {
    reqP->conn_fd = -1;
    memset(&(reqP->pkg), 0, sizeof(package));
}

static void free_request(request *reqP) {
    /*if (reqP->filename != NULL) {
        free(reqP->filename);
        reqP->filename = NULL;
    }*/
    init_request(reqP);
}

static int check_connection(struct pollfd *fdarray, int &i, nfds_t &nfds) {
    int error_code;
    int error_code_size = sizeof(error_code);
    getsockopt(fdarray[i].fd, SOL_SOCKET, SO_ERROR, &error_code, (socklen_t *)&error_code_size);
    return error_code;
}

static int read_package(int fd) {
    int bytes = 0, tmp;
    memset(&(request_array[fd].pkg), 0, sizeof(package));
    while(bytes < sizeof(package)) {
        tmp = read(fd, &(request_array[fd].pkg), sizeof(package));
        if(tmp <= 0) {
            if(errno == EAGAIN) {
                continue;
            }
            return 0;
        }
        bytes += tmp;
    }
    return bytes;
}

/* Homwpage Error Detection */
static void homepage_message(string &response, string error, int line_num) {
    string data;
    fstream data_file;
    data_file.open("./template/index.html");
    for(int i = 1; i <= line_num; ++i) {
        getline(data_file, data);
        response.append(data + "\n");
    }
    response.append("<h4>" + error + "<h4>\n");
    while(getline(data_file, data)) {
        response.append(data + "\n");
    }
}

static pair<bool, string> find_user(string &username) {
    string data;
    fstream data_file;
    data_file.open("./database/user_passwd.txt");
    while(getline(data_file, data)) {
        int sep = data.find(",");
        if(data.substr(0, sep) == username) {
            return {1, data.substr(sep + 1)};
        }
    }
    return {0, ""};
}

static void load_comments(string &response) {
    string data;
    fstream template_file, comments_file;
    template_file.open("./template/board.html");
    for(int i = 1; i <= BOARDLEN; ++i) {
        getline(template_file, data);
        response.append(data + "\n");
    }
    comments_file.open("./database/comments.txt");
    while(getline(comments_file, data)) {
        int sep = data.find(",");
        cout << data.substr(0, sep) << "\n";
        response.append("<p>[" + data.substr(0, sep) + "]: " + data.substr(sep + 1) + "\n");
    }
    while(getline(template_file, data)) {
        response.append(data + "\n");
    }
}

void read_file(const string& filename) {
    VideoCapture source(filename);
    assert(frame_queues.size() == sending_threads_num);

    if(!source.isOpened()) {
        cout << "Error opening video stream or file\n";
        exit(-1);
    }
    bool stop_now = false;

    uint64_t timestamp = 0;
    while(source.isOpened()) {
        size_t queue_id = timestamp % sending_threads_num;{
            lock_guard<mutex> lock(frame_queues_guard);
            if (frame_queues[queue_id].size() >= max_queue_size)
                continue;
        }

        shared_ptr<Mat> frame(new Mat());
        bool ret = source.read(*frame);
        
        if (!ret && frame->empty()) {//Video is over
            cerr << "Video is over\n";
            stop_now = true; // wait putting empty "deactivation frame" to the queue to stop other threads
        }else{
            lock_guard<mutex> lock(frame_queues_guard);
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
        lock_guard<mutex> lock(frame_queues_guard);
        if (frame_queues[debug_thread_id].empty())//No frames to send in this queue
            continue;

        shared_ptr<Mat> frame_to_send = frame_queues[debug_thread_id].front();
        frame_queues[debug_thread_id].pop_front();
        if (frame_to_send->empty()) // No more frames to send now!
            break;

        int frame_length = frame_to_send->total() * frame_to_send->elemSize();
        if ((bytes = send(socket, frame_to_send->data, frame_length, 0)) < 0) {
            cerr << "bytes = " << bytes << "\n";
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    //the variables for sending video
    string filename;
    int width, height;
    int size_buffer[3];
    bool size_sent;
    int type_media = 0;
    //the variables for sending audio
    char audio_buffer[512];

    //Specifying argv
    if(argc != 2 && argc != 3 && argc != 4) {
        fprintf(stderr, "usage: %s [BACKEND_PORT] [OPTIONAL]--init\n", argv[0]);
        fprintf(stderr, "Or\n");
        fprintf(stderr, "usage: %s [BACKEND_PORT] -video(-audio) [OPTIONAL][FILEPATH]\n", argv[0]);
        exit(1);
    }

    string arg1 = (string)argv[1];
    int port = stoi(arg1);
    init_server(port);

    if(argc == 3) {
        if((string)argv[2] == "--init"){
            init_database();
        }else if((string)argv[2] == "-video"){
            type_media = 1;
            filename = default_filename_video;
        }else if((string)argv[2] == "-audio"){
            type_media = 2;
            filename = default_filename_audio;
        }
    }

    if(argc == 4){
        type_media = 1;
        filename = argv[3];
    }

    if(type_media == 1){//If I need to send video
        VideoCapture cap(filename);
        width = cap.get(CAP_PROP_FRAME_WIDTH);
        height = cap.get(CAP_PROP_FRAME_HEIGHT);

        size_sent = false;
        size_buffer[0] = height; size_buffer[1] = width;
            
        cout << "height: " << size_buffer[0] << "\n";
        cout << "width: " << size_buffer[1] << "\n";
    }
    //deal with the video file transfer
    for (size_t i = 0; i < sending_threads_num; ++i) {
        lock_guard<mutex> lock(frame_queues_guard);
        frame_queues.push_back(FrameQueue());
    }
    vector<thread> sending_threads;
    

    struct pollfd fdarray[maxfd];
    fdarray[0].fd = backend_fd;
    fdarray[0].events = POLLIN;
    nfds_t nfds = 1;
    int ready_fds = 0, client_len;

    char frontend_host[256];

    while(1) {
        if((ready_fds = poll(fdarray, nfds, -1)) < 0) {//Poll failed(with some error)
            if(errno == EINTR || errno == EAGAIN) {
                continue;
            }
        }
        for(int i = 1; i < nfds; ++i) { // we can threading here
            check_connection(fdarray, i, nfds);//If error return > 0, return error bytes position
            if(fdarray[i].revents & POLLIN) {
                if(read_package(fdarray[i].fd) <= 0 || check_connection(fdarray, i, nfds) > 0) {//deal with error
                    close(fdarray[i].fd);
                    swap(fdarray[i--], fdarray[--nfds]);
                    continue;
                }

                string response;
                if(request_array[fdarray[i].fd].pkg.type == 0) {
                    homepage_message(response, "", LOGINLEN);//read the first part of index.html
                    sprintf(request_array[fdarray[i].fd].pkg.buf, "%s", response.c_str());
                }
                else {
                    if((string)request_array[fdarray[i].fd].pkg.reqpath == "/register") {
                        string username = (string)request_array[fdarray[i].fd].pkg.sender;
                        string password = (string)request_array[fdarray[i].fd].pkg.password;
                        memset(&(request_array[fdarray[i].fd].pkg), 0, sizeof(package));

                        pair<int, string> search_res;
                        if(username.empty() || password.empty()) {
                            homepage_message(response, "Username and Password cannot be empty.", REGISTERLEN);
                        }
                        else if((search_res = find_user(username)).first == 1) {
                            homepage_message(response, "The user name already exists.", REGISTERLEN);
                        }
                        else {
                            ofstream file;
                            file.open("./database/user_passwd.txt", ios_base::app);
                            file << username << "," << password << '\n';
                            file.close();
                            homepage_message(response, "Success.", REGISTERLEN);
                        }

                        sprintf(request_array[fdarray[i].fd].pkg.buf, "%s", response.c_str());
                    }
                    else if((string)request_array[fdarray[i].fd].pkg.reqpath == "/login") {
                        string username = (string)request_array[fdarray[i].fd].pkg.sender;
                        string password = (string)request_array[fdarray[i].fd].pkg.password;
                        memset(&(request_array[fdarray[i].fd].pkg), 0, sizeof(package));

                        // cout << username << " " << password << "\n";
                        pair<int, string> search_res;
                        if(username.empty() || password.empty()) {
                            homepage_message(response, "Username and Password cannot be empty.", LOGINLEN);
                        }
                        else if((search_res = find_user(username)).first == 0) {
                            homepage_message(response, "The username does not exist.", LOGINLEN);
                        }
                        else if(search_res.second != password) {
                            homepage_message(response, "The password is not correct.", LOGINLEN);
                        }
                        else {
                            sprintf(request_array[fdarray[i].fd].pkg.sender, "%s", username.c_str());
                            load_comments(response);
                        }
                        sprintf(request_array[fdarray[i].fd].pkg.buf, "%s", response.c_str());
                    }
                    else if((string)request_array[fdarray[i].fd].pkg.reqpath == "/comment") {
                        string username = (string)request_array[fdarray[i].fd].pkg.sender;
                        string message = (string)request_array[fdarray[i].fd].pkg.message;
                        memset(&(request_array[fdarray[i].fd].pkg), 0, sizeof(package));
                        if(username.empty()) {
                            cout << "username empty when comments\n";
                            homepage_message(response, "", LOGINLEN);
                        }
                        else {
                            if(message.empty() == false) {
                                cout << "message empty\n";
                                ofstream file;
                                file.open("./database/comments.txt", ios_base::app);
                                file << username << "," << message << '\n';
                                file.close();
                            }
                            load_comments(response);
                        }

                        sprintf(request_array[fdarray[i].fd].pkg.buf, "%s", response.c_str());
                        sprintf(request_array[fdarray[i].fd].pkg.sender, "%s", username.c_str());
                    }
                    else if((string)request_array[fdarray[i].fd].pkg.reqpath == "/logout") {
                        homepage_message(response, "", LOGINLEN);
                        sprintf(request_array[fdarray[i].fd].pkg.buf, "%s", response.c_str());
                    }
                }
                fdarray[i].events = POLLOUT;
            }
            else if(fdarray[i].revents & POLLOUT) {
                if(write(fdarray[i].fd, &(request_array[fdarray[i].fd].pkg), sizeof(package)) <= 0 || \
                    check_connection(fdarray, i, nfds) > 0) {

                    close(fdarray[i].fd);
                    swap(fdarray[i--], fdarray[--nfds]);
                    continue;
                }
                fdarray[i].events = POLLIN;
            }
        }
        
        if(fdarray[0].revents & POLLIN) {
            client_len = sizeof(frontendAddr);
            int conn_fd = accept(backend_fd, (struct sockaddr *)&frontendAddr, (socklen_t *)&client_len);

            if(conn_fd < 0) {
                if(errno == EINTR || errno == EAGAIN) {
                    continue;
                }
                if(errno == ENFILE) {
                    fprintf(stderr, "out of file descriptor table... (maxconn = %d)\n", maxfd);
                    continue;
                }
                ERR_EXIT("accept");
            }

            //if the type is video sending
            if(type_media == 1){
                //At first, send the frame size to client
                if(!size_sent){
                    if(send(conn_fd, size_buffer, sizeof(size_buffer), 0) < 0){
                        cerr << "Can't send frame size!" << "\n";
                    }else{
                        size_sent = true;
                    }
                }

                thread read_thread(read_file, filename);

                for (size_t i = 0; i < sending_threads_num; ++i) {
                    assert(frame_queues.size() > i);
                    assert(sending_threads.size() == i);
                    thread t(play_to_network, conn_fd, (int)i);
                    sending_threads.push_back(move(t));
                }

                for (size_t i = 0; i < sending_threads_num; ++i) {
                    sending_threads[i].join();
                }
                sending_threads.clear();

                read_thread.join();
                cerr << "Wait for new connections\n";
            }else if(type_media == 2){
                // File Pointer Declaration
                FILE *fp;
                fp = fopen("sample.m4a", "rb");
                int bytes_left = 0;
                while(1){
                    if((bytes_left = fread(audio_buffer, sizeof(audio_buffer), 1, fp)) < 0){
                        cerr << "read bytes = " << bytes_left << ", fread didn't read anything for 1 sec. Audio seems to be over\n";
                        break;
                    }
                    if(send(conn_fd, audio_buffer, sizeof(audio_buffer), 0) < 0){
                        cerr << "Can't send audio file!\n";
                    }
                }
                cerr << "Wait for new connections\n";
            }else{//else
                memset(frontend_host, 0, sizeof(frontend_host));
                strcpy(frontend_host, inet_ntoa(frontendAddr.sin_addr));
                printf("getting a new request... fd %d from %s\n", conn_fd, frontend_host);

                init_request(&(request_array[conn_fd]));
                request_array[conn_fd].conn_fd = conn_fd;
                fdarray[nfds].fd = conn_fd;
                fdarray[nfds++].events = POLLIN;
            }
        }
    }
    return 0;
}
