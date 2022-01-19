#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)

typedef struct {
    char hostname[512];  // server's hostname
    unsigned short port;  // port to listen
    int listen_fd;  // fd to wait for a new connection
} server;

typedef struct {
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[512];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    // you don't need to change this.
    int id;
    int wait_for_write;  // used by handle_read to know if the header is read or not.
} request;

server svr;  // server
request* requestP = NULL;  // point to a list of requests
int maxfd;  // size of open file descriptor table, size of request list

const char* accept_read_header = "ACCEPT_FROM_READ";
const char* accept_write_header = "ACCEPT_FROM_WRITE";

static void init_server(unsigned short port);
// initailize a server, exit for error

static void init_request(request* reqP);
// initailize a request instance

static void free_request(request* reqP);
// free resources used by a request instance

typedef struct {
    int id;          //902001-902020
    int AZ;          
    int BNT;         
    int Moderna;     
}registerRecord;

int handle_read(request* reqP) {
    int r;
    char buf[512];
    // Read in request from client
    r = read(reqP->conn_fd, buf, sizeof(buf));
    if (r < 0) return -1;
    if (r == 0) return 0;
    char* p1 = strstr(buf, "\015\012");
    int newline_len = 2;
    if (p1 == NULL) {
       p1 = strstr(buf, "\012");
        if (p1 == NULL) {
            ERR_EXIT("this really should not happen...");
        }
    }
    size_t len = p1 - buf + 1;
    memmove(reqP->buf, buf, len);
    reqP->buf[len - 1] = '\0';
    reqP->buf_len = len-1;
    return 1;
}

int main(int argc, char** argv) {

    // Parse args.
    if (argc != 2) {
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in cliaddr;  // used by accept()
    int clilen;

    int conn_fd;  // fd for a new connection with client
    int file_fd = open("registerRecord", O_RDWR);  // fd for file that we open for reading
    char buf[512];
    int buf_len;
    struct flock lock;
    // Initialize server
    init_server((unsigned short) atoi(argv[1]));

    // Loop for handling connections
    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);
    struct pollfd fdarray[maxfd];
    bool toClose[maxfd];
    int locked[20] = {};

    fdarray[0].fd = svr.listen_fd;
    fdarray[0].events = POLLIN;
    unsigned int nfds = 1;
    int totalFds;
    while (1) {
        // TODO: Add IO multiplexing
        if((totalFds = poll(fdarray, nfds, -1)) < 0){
            if(errno == EINTR) continue;
        }
        for(int i = 1; i < nfds; ++i){
            struct flock lock;
            if(fdarray[i].revents & POLLOUT){
                write(fdarray[i].fd, requestP[fdarray[i].fd].buf, strlen(requestP[fdarray[i].fd].buf));
                fdarray[i].events = POLLIN;
                if(toClose[i]){
                    close(fdarray[i].fd);
                    free_request(&requestP[fdarray[i].fd]);
                    fdarray[i] = fdarray[--nfds];
                    toClose[i] = toClose[nfds];
                    --i;
                }
            } 
            else if(fdarray[i].revents & POLLIN){
                fdarray[i].events = POLLOUT;
                int ret = handle_read(&requestP[fdarray[i].fd]); // parse data from client to requestP[conn_fd].buf
                fprintf(stderr, "ret = %d\n", ret);
                if (ret < 0) {
                    fprintf(stderr, "bad request from %s\n", requestP[fdarray[i].fd].host);
                    continue;
                }
    // TODO: handle requests from clients
#ifdef READ_SERVER
                //check if there is anyone writing this account
                int myId = atoi(requestP[fdarray[i].fd].buf) - 902001;
                if(strlen(requestP[fdarray[i].fd].buf) != 6) myId = -1;
                if(myId > 19 || myId < 0){
                    sprintf(requestP[fdarray[i].fd].buf, "[Error] Operation failed. Please try again.\n");
                }
                else{
                    lock.l_type = F_RDLCK;
                    lock.l_start = sizeof(registerRecord) * myId;
                    lock.l_whence = SEEK_SET;
                    lock.l_len = sizeof(registerRecord);
                    if(fcntl(file_fd, F_SETLK, &lock) == -1 || locked[myId] == 1){
                        sprintf(requestP[fdarray[i].fd].buf, "Locked.\n");
                    }
                    else{
                        registerRecord input;
                        lseek(file_fd, sizeof(registerRecord) * myId, SEEK_SET);
                        read(file_fd, &input, sizeof(registerRecord));
                        char *p[3];
                        p[input.AZ-1] = "AZ";
                        p[input.BNT-1] = "BNT";
                        p[input.Moderna-1] = "Moderna";
                        sprintf(requestP[fdarray[i].fd].buf, 
                            "Your preference order is %s > %s > %s.\n", p[0], p[1], p[2]);
                        lock.l_type = F_UNLCK;
                        fcntl(file_fd, F_SETLK, lock);
                    }
                }
                toClose[i] = 1;
                // fprintf(stderr, "%s", requestP[conn_fd].buf);
                // sprintf(buf,"%s : %s",accept_read_header,requestP[conn_fd].buf);
                // write(requestP[conn_fd].conn_fd, buf, strlen(buf));    
#elif WRITE_SERVER
                if(requestP[fdarray[i].fd].wait_for_write != 1){
                    int myId = atoi(requestP[fdarray[i].fd].buf) - 902001;
                    if(strlen(requestP[fdarray[i].fd].buf) != 6) myId = -1;
                    if(myId > 19 || myId < 0){
                        sprintf(requestP[fdarray[i].fd].buf, "[Error] Operation failed. Please try again.\n");
                        toClose[i] = 1;
                    }
                    else{
                        requestP[fdarray[i].fd].id = myId;
                        lock.l_type = F_WRLCK;
                        lock.l_start = sizeof(registerRecord) * myId;
                        lock.l_whence = SEEK_SET;
                        lock.l_len = sizeof(registerRecord);
                        if(fcntl(file_fd, F_SETLK, &lock) == -1 || locked[myId] == 1){
                            sprintf(requestP[fdarray[i].fd].buf, "Locked.\n");
                            toClose[i] = 1;
                        }
                        else{
                            locked[myId] = 1;
                            registerRecord input;
                            lseek(file_fd, sizeof(registerRecord) * myId, SEEK_SET);
                            read(file_fd, &input, sizeof(registerRecord));
                            char *p[3];
                            p[input.AZ-1] = "AZ";
                            p[input.BNT-1] = "BNT";
                            p[input.Moderna-1] = "Moderna"; 
                            requestP[fdarray[i].fd].wait_for_write = 1;
                            sprintf(requestP[fdarray[i].fd].buf, 
                                "Your preference order is %s > %s > %s.\nPlease input your preference order respectively(AZ,BNT,Moderna):\n", p[0], p[1], p[2]);
                        }
                    }
                }
                else if(requestP[fdarray[i].fd].wait_for_write == 1){
                    registerRecord output;
                    lseek(file_fd, sizeof(registerRecord) * requestP[fdarray[i].fd].id, SEEK_SET);
                    read(file_fd, &output, sizeof(registerRecord));
                    if(strcmp(requestP[fdarray[i].fd].buf, "1 2 3") != 0 &&
                        strcmp(requestP[fdarray[i].fd].buf, "1 3 2") != 0 &&
                        strcmp(requestP[fdarray[i].fd].buf, "2 1 3") != 0 &&
                        strcmp(requestP[fdarray[i].fd].buf, "2 3 1") != 0 &&
                        strcmp(requestP[fdarray[i].fd].buf, "3 1 2") != 0 &&
                        strcmp(requestP[fdarray[i].fd].buf, "3 2 1") != 0){
                            sprintf(requestP[fdarray[i].fd].buf, "[Error] Operation failed. Please try again.\n");
                        }
                    else{
                        output.AZ = requestP[fdarray[i].fd].buf[0] - 48;
                        output.BNT = requestP[fdarray[i].fd].buf[2] - 48;
                        output.Moderna = requestP[fdarray[i].fd].buf[4] - 48;
                        char *p[3];
                        p[output.AZ-1] = "AZ";
                        p[output.BNT-1] = "BNT";
                        p[output.Moderna-1] = "Moderna";
                        lseek(file_fd, sizeof(registerRecord) * requestP[fdarray[i].fd].id, SEEK_SET);
                        write(file_fd, &output, sizeof(registerRecord));
                        sprintf(requestP[fdarray[i].fd].buf, 
                            "Preference order for %d modified successed, new preference order is %s > %s > %s.\n",
                            output.id, p[0], p[1], p[2]);
                    }
                    lock.l_start = sizeof(registerRecord) * requestP[fdarray[i].fd].id;
                    lock.l_whence = SEEK_SET;
                    lock.l_type = F_UNLCK;
                    lock.l_len = sizeof(registerRecord);
                    locked[requestP[fdarray[i].fd].id] = 0;
                    fcntl(file_fd, F_SETLK, &lock);
                    toClose[i] = 1;
                }
                // fprintf(stderr, "%s", requestP[conn_fd].buf);
                // sprintf(buf,"%s : %s",accept_write_header,requestP[conn_fd].buf);
                // write(requestP[conn_fd].conn_fd, buf, strlen(buf));    
#endif
            }
        }
            

        // Check new connection
        if(fdarray[0].revents & POLLIN){
            clilen = sizeof(cliaddr);
            conn_fd = accept(svr.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
            if (conn_fd < 0) {
                if (errno == EINTR || errno == EAGAIN) continue;  // try again
                if (errno == ENFILE) {
                    (void) fprintf(stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd);
                    continue;
                }
                ERR_EXIT("accept");
            }
            requestP[conn_fd].conn_fd = conn_fd;
            strcpy(requestP[conn_fd].host, inet_ntoa(cliaddr.sin_addr));
            fprintf(stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host);
            fdarray[nfds].fd = conn_fd;
            fdarray[nfds].events = POLLOUT;
            requestP[conn_fd].wait_for_write = 0;
            toClose[nfds++] = 0;
            sprintf(requestP[conn_fd].buf, "Please enter your id (to check your preference order):\n");
            // write(conn_fd, requestP[conn_fd].buf, strlen(requestP[conn_fd].buf));
        }
    }
    close(file_fd);
    free(requestP);
    return 0;
}

// ======================================================================================================
// You don't need to know how the following codes are working
#include <fcntl.h>

static void init_request(request* reqP) {
    reqP->conn_fd = -1;
    reqP->buf_len = 0;
    reqP->id = 0;
}

static void free_request(request* reqP) {
    /*if (reqP->filename != NULL) {
        free(reqP->filename);
        reqP->filename = NULL;
    }*/
    init_request(reqP);
}

static void init_server(unsigned short port) {
    struct sockaddr_in servaddr;
    int tmp;

    gethostname(svr.hostname, sizeof(svr.hostname));
    svr.port = port;

    svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr.listen_fd < 0) ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    tmp = 1;
    if (setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if (bind(svr.listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        ERR_EXIT("bind");
    }
    if (listen(svr.listen_fd, 1024) < 0) {
        ERR_EXIT("listen");
    }

    // Get file descripter table size and initialize request table
    maxfd = getdtablesize();
    requestP = (request*) malloc(sizeof(request) * maxfd);
    if (requestP == NULL) {
        ERR_EXIT("out of memory allocating all requests");
    }
    for (int i = 0; i < maxfd; i++) {
        init_request(&requestP[i]);
    }
    requestP[svr.listen_fd].conn_fd = svr.listen_fd;
    strcpy(requestP[svr.listen_fd].host, svr.hostname);

    return;
}
