#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <poll.h>
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
#include <unordered_map>
#include<opencv2/opencv.hpp>

#define HEADER_MAX 8192

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)

using namespace std;
using namespace cv;

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
    int filesize;
    string username;
} request;

typedef struct server {
    string hostname;
    unsigned short port;
    int listen_fd;
} server;

char process_buf[HEADER_MAX+10];

struct sockaddr_in frontendAddr, backendAddr, clientAddr;
int frontend_fd, connect2backend_fd, client_fd;
int client_len;
unordered_map<string, string> req_map;
package pkg;

void init_backend_socket(string backendIP, int backendPort){
    // tcp client connecting to server(backend)
    connect2backend_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(connect2backend_fd  < 0) {
        ERR_EXIT("socket");
    }
    if(inet_addr(backendIP.c_str()) < 0) {
        fprintf(stderr, "server ip should be a valid ip.\n");
        exit(1);
    }
    if(backendPort < 0 || backendPort > 65535) {
        fprintf(stderr, "server port should be a number between 0 to 65535.\n");
        exit(1);
    }
    bzero(&backendAddr, sizeof(backendAddr));

    backendAddr.sin_family = AF_INET;
    backendAddr.sin_addr.s_addr = inet_addr(backendIP.c_str());
    backendAddr.sin_port = htons((unsigned short)backendPort);
    int tmp = 1;
    if(setsockopt(connect2backend_fd , SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if(connect(connect2backend_fd , (struct sockaddr*)&backendAddr, sizeof(backendAddr)) < 0) {
        ERR_EXIT("connect");
    }

    cout << "connected to " + backendIP << '\n';
}

static void init_server(int port, string backendIP, int backendPort) {
    int tmp;

    gethostname(process_buf, sizeof(process_buf));
    frontend_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(frontend_fd < 0) {
        ERR_EXIT("socket");
    }
    bzero(&frontendAddr, sizeof(frontendAddr));
    frontendAddr.sin_family = AF_INET;
    frontendAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    frontendAddr.sin_port = htons((unsigned short)port);

    tmp = 1;
    if(setsockopt(frontend_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if(bind(frontend_fd, (struct sockaddr*)&frontendAddr, sizeof(frontendAddr))) {
        ERR_EXIT("bind");
    }
    if(listen(frontend_fd, 1024) < 0) {
        ERR_EXIT("listen");
    }

    cout << "HTTP server listen on port: " << port << ", fd: " << frontend_fd << '\n';
    init_backend_socket(backendIP, backendPort);
}

string set_200_header(string file_type, string extra_field = "", int body_len = 0) {
    string res_header;
    res_header.append("HTTP/1.1 200 OK\r\n");
    res_header.append("Content-Length: " + to_string(body_len) + "\r\n");
    res_header.append("Content-Type: " + file_type + "\r\n");
    res_header.append("Connection: keep-alive\r\n");
    res_header.append(extra_field + "\r\n");
    return res_header;
} 

int read_httpreq() {
    memset(process_buf, 0, sizeof(process_buf));
    req_map = {};
    ssize_t bytes = read(client_fd, process_buf, HEADER_MAX);
    if(bytes <= 0) return bytes;
    string req = (string)process_buf;

    // check if "\r\n\r\n" in req string
    int eoh;
    if((eoh = req.find("\r\n\r\n")) == string::npos) {
        fprintf(stderr, "did not read a complete http request\n");
        return -1;
    }

    // seperate header and body
    string header = req.substr(0, eoh), subheader = header, body = req.substr(eoh + 4), tmpstr;
    eoh += 4;
    int sz, sep;
    sz = subheader.find("\r\n");
    tmpstr = subheader.substr(0, sz);
    subheader = subheader.substr(sz + 2);

    // method
    sz = tmpstr.find(" ");
    req_map["method"] = tmpstr.substr(0, sz);
    tmpstr = tmpstr.substr(sz + 1);
    // reqpath
    sz = tmpstr.find(" ");
    req_map["reqpath"] = tmpstr.substr(0, sz);

    // by traversing the header, split each field out and store it into a map
    while((sz = subheader.find("\r\n")) != string::npos) {
        tmpstr = subheader.substr(0, sz);
        subheader = subheader.substr(sz + 2);
        sep = tmpstr.find(": ");
        req_map[tmpstr.substr(0, sep)] = tmpstr.substr(sep + 2);
    }

    // get the last field in request
    sep = subheader.find(": ");
    if(sep != string::npos) {
        req_map[subheader.substr(0, sep)] = subheader.substr(sep + 2);
    }

    string subbody = body;
    while((sz = subbody.find("\r\n")) != string::npos) {
        tmpstr = subbody.substr(0, sz);
        subbody = subbody.substr(sz + 2);
        sep = tmpstr.find("=");
        req_map[tmpstr.substr(0, sep)] = tmpstr.substr(sep + 1);
    }
    sep = subbody.find("=");
    if(sep != string::npos) {
        req_map[subbody.substr(0, sep)] = subbody.substr(sep + 1);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "usage: %s [BACKEND_IP]:[BACKEND_PORT] [FRONTEND_PORT]\n", argv[0]);
        fprintf(stderr, "Or\n");
        fprintf(stderr, "usage: %s [BACKEND_IP]:[BACKEND_PORT] -video(-audio)\n", argv[0]);
        exit(1);
    }//No need to change for video/audio

    string ip = (string)argv[1];
    if(ip.find(':') == string::npos) {
        fprintf(stderr, "usage: %s [BACKEND_IP]:[BACKEND_PORT] [FRONTEND_PORT]\n", argv[0]);
        exit(1);
    }
    string backendIP = ip.substr(0, ip.find(':')), backendPortStr = ip.substr(ip.find(':') + 1);
    int backendPort = stoi(backendPortStr);

    int type_media = 0;
    //this is for video
    if((string)argv[2] == "-video"){
        type_media = 1;
    }else if((string)argv[2] == "-audio"){
        type_media = 2;
    }else{
        string myPortStr = (string)argv[2];
        int myPort = stoi(myPortStr);
        init_server(myPort, backendIP, backendPort);
    }
    
    string username;
    string tmp_method = "";
    char file_data[512], buffer[512];
    while(1) {
        if(type_media >= 1){//request for video or audio
            //Connect here!
            init_backend_socket(backendIP, backendPort);
            //Video
            if(type_media == 1){
                //Receive the frame size first
                int size_buffer[3];

                if(recv(connect2backend_fd, size_buffer, sizeof(size_buffer), 0) < 0){
                    cerr << "Unknown frame size!\n";
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
                    if ((bytes = recv(connect2backend_fd, data_ptr, frame_length , MSG_WAITALL)) == -1) {
                        cerr << "received bytes = " << bytes << ", recv didn't receive anything for 1 sec. Video seems to be over\n";
                        break;
                    }
                    cout << bytes << "\n";
                    cv::imshow("CV Video Client", frame);
                    if (char key = (char)cv::waitKey(25)) {
                        if (key == 27 || key == 'q')
                            break;
                    }
                }
                cv::destroyAllWindows();

                close(connect2backend_fd);
            }else{//Audio
                FILE *fp2 = fopen("AudioReceived.mp3", "w"); // creating new file in write mode for receiving data
                int count = 0;
                while (1){
                    recv(connect2backend_fd, buffer, sizeof(buffer), 0);
                    if(count > 100){
                        break;
                    }
                    // if(read(connect2backend_fd, buffer, sizeof(buffer)) < 0){
                    //     cerr << "Can't read!\n";
                    //     break;
                    // }
                    fwrite(file_data, 512, 1, fp2);
                    count ++;
                }
                close(connect2backend_fd);
            }
        }else{
            client_len = sizeof(clientAddr);
            client_fd = -1;
            if((client_fd = accept(frontend_fd, (struct sockaddr*)&clientAddr, (socklen_t *)&client_len)) < 0) {
                if (errno == EINTR || errno == EAGAIN) continue;  // try again
                if (errno == ENFILE) {
                    fprintf(stderr, "out of file descriptor table\n");
                    continue;
                }
                ERR_EXIT("accept");
            }

            // read the fd when it is ready to read
            struct pollfd pfd[2];
            pfd[0].fd = client_fd;
            pfd[0].events = POLLIN;

            if(poll(pfd, (nfds_t)1, -1) < 0) {
                if(errno == EINTR || errno == EAGAIN) {
                    continue;
                }
            }
            
            if(read_httpreq() < 0) {
                fprintf(stderr, "read error\n");
            } 
            else {
                memset(&pkg, 0, sizeof(pkg));
                sprintf(pkg.reqpath, "%s", req_map["reqpath"].c_str());
                if(req_map["method"] == "GET") {
                    cout << "GET\n";
                    pkg.type = 0;
                    if(tmp_method != "POST")
                        username = "";
                }
                else if(req_map["method"] == "POST") {
                    if(!username.empty() && (string)pkg.reqpath != "/register") {
                        req_map["username"] = username;
                        cout << username << "\n";
                    }
                    pkg.type = 1;
                    if(req_map.find("username") != req_map.end()) {
                        sprintf(pkg.sender, "%s", req_map["username"].c_str());
                    }else{
                        cout << "haven't store req_map username\n";
                    }
                    if(req_map.find("password") != req_map.end()) {
                        sprintf(pkg.password, "%s", req_map["password"].c_str());
                    }else{
                        cout << "haven't store req_map pwd\n";
                    }
                    if(req_map.find("content") != req_map.end()) {
                        sprintf(pkg.message, "%s", req_map["content"].c_str());
                    }else{
                        cout << "haven't store req_map cnt\n";
                    }
                }
                
                // send request to backend 
                write(connect2backend_fd , &pkg, sizeof(package));

                struct pollfd svrpfd[2];
                svrpfd[0].fd = connect2backend_fd;
                svrpfd[0].events = POLLIN;
                // read response from backend 
                if(poll(svrpfd, (nfds_t)1, -1) < 0) {
                    if(errno == EINTR || errno == EAGAIN) {
                        continue;
                    }
                }
                memset(&pkg, 0, sizeof(pkg));
                read(connect2backend_fd, &pkg, sizeof(package));

                // record username
                // cout << tmp_method << " tmp\n";
                if(tmp_method != "POST")
                    username = (string)(pkg.sender);
                // cout << username << "\n";
                tmp_method = req_map["method"];

                string header = set_200_header("text/html", "", strlen(pkg.buf));
                string response = header + (string)(pkg.buf);
                write(client_fd, response.c_str(), response.length());
            }
            close(client_fd);
        }
    }
    return 0;
}