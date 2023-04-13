#include "opencv2/opencv.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    int sock;
    char* server_ip;
    int serverPort;

    if (argc != 3) {
           std::cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << std::endl;
           exit(-1);
    }

    server_ip = argv[1];
    serverPort = atoi(argv[2]);

    struct  sockaddr_in server_addr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket() failed" << std::endl;
        exit(-1);
    }

    struct timeval timeout_video_is_over;
    timeout_video_is_over.tv_sec = 1;
    timeout_video_is_over.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_video_is_over, sizeof timeout_video_is_over);

    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(serverPort);

    while (1) {
        if (connect(sock, (sockaddr*)&server_addr, addrLen) < 0) {
            std::cerr << "connect() failed! retry after 1 sec" << std::endl;
            sleep(1);
            continue;
        } else {
            std::cerr << "connected successfully" << std::endl;
        }
        break;
    }

    //Receive the frame size first
    int size_buffer[3];

    if(recv(sock, size_buffer, sizeof(size_buffer), 0) < 0){
        std::cerr << "Unknown frame size!" << std::endl;
    }
    int height = size_buffer[0]; 
    int width = size_buffer[1];
    cout << height << " " << width << "\n";

    cv::Mat frame = cv::Mat::zeros(height , width, CV_8UC3);
    int frame_length = frame.total() * frame.elemSize();
    uchar *data_ptr = frame.data;

    // make frame continuous
    if (!frame.isContinuous()) {
          frame = frame.clone();
    }

    cv::namedWindow("CV Video Client", 1);

    int bytes = 0;
    while (1) {
        if ((bytes = recv(sock, data_ptr, frame_length , MSG_WAITALL)) == -1) {
            std::cerr << "received bytes = " << bytes << ", recv didn't receive anything for 1 sec. Video seems to be over" << std::endl;
            break;
        }

        cv::imshow("CV Video Client", frame);
        if (char key = (char)cv::waitKey(25)) {
            if (key == 27 || key == 'q')
                break;
        }
    }
    cv::destroyAllWindows();

    close(sock);

    return 0;
}