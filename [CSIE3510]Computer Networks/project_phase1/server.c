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
    char hostname[512];     // server's host name
    unsigned short port;    // port to listen
    int listen_fd;          // fd to wait for new connection
} server;

typedef struct {
    char hostname[512];         // client's host name
    int conn_fd;                // fd to talk with client
    char buf[512];              // data sent to/by client
    size_t buf_len;
    bool close;
} request;

server svr;
request* request_arr = NULL;
int maxfd; // size of open file descriptor table, size of request list         

/* initialize request instance */
static void init_request(request* reqP) {
    reqP->conn_fd = -1;
    reqP->buf_len = 0;
    memset(reqP->buf, 0, sizeof(reqP->buf));
    reqP->close = 0;
}
static void free_request(request* reqP) {
    /*if (reqP->filename != NULL) {
        free(reqP->filename);
        reqP->filename = NULL;
    }*/
    init_request(reqP);
}

/* initialize a server, exit for error */
static void init_server(unsigned short port) {
    struct sockaddr_in server_addr;
    int tmp;

    gethostname(svr.hostname, sizeof(svr.hostname));
    printf("Server's hostname: %s\n", svr.hostname);

    svr.port = port;
    svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);       // AF_INET: IPv4 ; SOCK_STREAM: TCP ; 
    if(svr.listen_fd < 0) ERR_EXIT("socket");

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    tmp = 1;
    if(setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if(bind(svr.listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        ERR_EXIT("bind");
    }
    if(listen(svr.listen_fd, 1024) < 0) {
        ERR_EXIT("listen");
    }

    printf("Server listening on: %d\n", (int)port);

    maxfd = getdtablesize();

    request_arr = (request*) malloc(sizeof(request) * maxfd);
    if(request_arr == NULL) {
        ERR_EXIT("out of memory allocating all requests");
    }
    for(int i = 0; i < maxfd; ++i) {
        init_request(&(request_arr[i]));
    }
    request_arr[svr.listen_fd].conn_fd = svr.listen_fd;
    strcpy(request_arr[svr.listen_fd].hostname, svr.hostname);
}

char tmp_buff[128];

void check_connection(struct pollfd* fdarray, int* i, nfds_t* nfds) {
    if(read(fdarray[*i].fd, tmp_buff, 0) < 0 && write(fdarray[*i].fd, "", 0) < 0) {
        close(fdarray[*i].fd);
        free_request(&request_arr[fdarray[*i].fd]);
        printf("Connection from %s closed\n", request_arr[fdarray[*i].fd].hostname);
        fdarray[*i] = fdarray[--(*nfds)];
        --(*i);
    }
}

int handle_read(request* reqP) {
    int r;
    char buf[512] = "";

    // Read in request from client
    r = read(reqP->conn_fd, buf, sizeof(buf));
    if (r < 0) return -1;
    if (r == 0) return 0;

    int len = strlen(buf);
    buf[len-2] = '\0';
    sprintf(reqP->buf, "%s", buf);
    reqP->buf[len-2] = '\0';
    reqP->buf_len = len-1;

    return 1;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        exit(1);
    }
    struct sockaddr_in client_adrr;
    int client_len;

    int conn_fd;
    char buf[512];
    int buf_len;

    /* Initailize server */
    init_server((unsigned int)atoi(argv[1]));
    struct pollfd fdarray[maxfd];

    fdarray[0].fd = svr.listen_fd;
    fdarray[0].events = POLLIN;
    nfds_t nfds = 1;
    int totalfds = 0;

    while(1) {
        if((totalfds = poll(fdarray, nfds, -1)) < 0) {
            if(errno == EINTR) continue;
        }
        for(int i = 1; i < nfds; ++i) {
            if(fdarray[i].revents & POLLIN) {
                int ret = handle_read(&request_arr[fdarray[i].fd]);
                fprintf(stderr, "ret = %d\n", ret);

                if(ret < 0) {
                    fprintf(stderr, "bad request from %s\n", request_arr[fdarray[i].fd].hostname);
                    continue;
                }
                printf("Client %s said: %s\n", request_arr[fdarray[i].fd].hostname, request_arr[fdarray[i].fd].buf);
                if(strstr(request_arr[fdarray[i].fd].buf, "GET / HTTP") != NULL) {
                    int html_fd = open("profile.html", O_RDONLY);

                    int w = read(html_fd, tmp_buff, 128);
                    if(w < 0) ERR_EXIT("read html file");

                    sprintf(request_arr[fdarray[i].fd].buf, "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n");
                    strcat(request_arr[fdarray[i].fd].buf, tmp_buff);
                    request_arr[fdarray[i].fd].close = 1;
                }
                else {
                    sprintf(tmp_buff, "Server echo: %s\n", request_arr[fdarray[i].fd].buf);
                    sprintf(request_arr[fdarray[i].fd].buf, "%s", tmp_buff);
                }

                fdarray[i].events = POLLOUT;
                memset(tmp_buff, 0, sizeof(tmp_buff));
                check_connection(fdarray, &i, &nfds);
            }
            else if(fdarray[i].revents & POLLOUT) {
                write(fdarray[i].fd, request_arr[fdarray[i].fd].buf, strlen(request_arr[fdarray[i].fd].buf));

                if(request_arr[fdarray[i].fd].close) close(fdarray[i].fd);

                fdarray[i].events = POLLIN;
                check_connection(fdarray, &i, &nfds);
            }
        }
        if(fdarray[0].revents & POLLIN) {
            client_len = sizeof(client_adrr);
            conn_fd = accept(svr.listen_fd, (struct sockaddr*)&client_adrr, (socklen_t*)&client_len);

            if(conn_fd < 0) {
                if (errno == EINTR || errno == EAGAIN) continue;  // try again
                if (errno == ENFILE) {
                    (void) fprintf(stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd);
                    continue;
                }
                ERR_EXIT("accept");
            }

            init_request(&(request_arr[conn_fd]));
            request_arr[conn_fd].conn_fd = conn_fd;

            strcpy(request_arr[conn_fd].hostname, inet_ntoa(client_adrr.sin_addr));
            printf("getting a new request... fd %d from %s\n", conn_fd, request_arr[conn_fd].hostname);

            fdarray[nfds].fd = conn_fd;
            fdarray[nfds++].events = POLLIN;
        }
    }
}