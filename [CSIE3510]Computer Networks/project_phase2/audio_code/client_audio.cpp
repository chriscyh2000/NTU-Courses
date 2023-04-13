/*Libraires to add additional functionalities*/
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>

void fault(const char *txt) // Function in case of an error
{
    perror(txt);
    exit(1);
}

int main(int argc, char *argv[])
{
    // declaration of variables necessary for socket programming
    int socket_desc, portno, n;
    struct hostent *serv1;
    struct sockaddr_in server_address;
    char hold2[512];

    // File Pointer Declaration
    FILE *fp;
    fp = fopen("sample.m4a", "rb");

    int storage;

    /*************** Start of port number***************/
    // if condition to make sure that port number is mentioned
    if (argc < 3)
    {
        fprintf(stderr, "%s Please enter port number!\n", argv[0]);
        exit(1);
    }

    // This receives the port number
    portno = atol(argv[2]);
    /*************** End of port number***************/

    // Checks wheter or not a socket is found
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0)
        fault("Socket not found.");

    /*gethostbyname retreives host information from a host databse corresponding to hostname*/
    serv1 = gethostbyname(argv[1]);

    if (serv1 == NULL) /*if no hostname and port number is passed in terminal then this is NULL*/
    {
        fprintf(stderr, "Host not found!."); /*stderr is default whenever we pass anything to main*/
    }

    bzero((char *)&server_address, sizeof(server_address)); /*bzero eares the data in the bytes specidied by sizeof(server_address)*/

    server_address.sin_family = AF_INET; // Specifies the address of AF_INET

    /*bcopy copies the data from the server to the client or the receiver and serv1->h_lenght is the total space occupied by the server*/
    bcopy((char *)serv1->h_addr, (char *)&server_address.sin_addr.s_addr, serv1->h_length);

    server_address.sin_port = htons(portno); // Declares port specified by portno in terminal

    if (connect(socket_desc, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) /*if conditions to verify the connection between server and client*/
    {
        fault("Connection not made");
    }

    bzero(hold2, 512); // 512 bytes are made

    while (1)
    {

        storage = fread(hold2, 512, 1, fp); // reading from file
        write(socket_desc, hold2, 512);     // tranferring file

        if (storage == 0)
        {
            printf("\n End of file!!");
            break;
        } // breaking loop when end of file is reached

        if (storage < 0)
        {

            fault("ERROR!!! PROBLEM!");
        }
    }

    printf("\n Connection made.\nFile sent.");
    close(socket_desc);
    return 0;
}