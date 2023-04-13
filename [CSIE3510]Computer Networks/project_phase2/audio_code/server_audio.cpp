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
    int socket_desc, newSocket_desc, portnumber, n;
    char buffer[512];
    struct sockaddr_in server_address, client_address;
    socklen_t cli1;

    // File Pointer Declaration
    FILE *fp2;           // declaring new file pointer for received file
    char file_data[512]; // declaring buffer for writing new fileFILE *fp2;
    int storage;

    // if condition to make sure that port number is mentioned
    if (argc < 2)
    {
        fprintf(stderr, "Please enter port number\n");
        exit(1);
    }

    // Code to check socket connection
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0)
    {
        fault("Socket not found");
    }

    /*bzero eares the data in the bytes specidied by sizeof(server_address)*/
    bzero((char *)&server_address, sizeof(server_address));

    portnumber = atoi(argv[1]);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // defines the family
    server_address.sin_port = htons(portnumber); // declares port number

    if (bind(socket_desc, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        fault("Not bounded");

    listen(socket_desc, 5); // Listening to the connection and receiving files
    cli1 = sizeof(client_address);

    newSocket_desc = accept(socket_desc, (struct sockaddr *)&client_address, &cli1);

    if (newSocket_desc < 0)
        fault("Can't accpet the socket.");

    fp2 = fopen("AudioReceived.mp3", "w"); // creating new file in write mode for receiving data

    while (1)
    {
        storage = read(newSocket_desc, buffer, 512);
        fwrite(file_data, 512, 1, fp2);

        // If there is storage problem then break out of the loop 
        if (storage < 0)
        {
            fault("Error.");
        }
        
        // End of file
        if (storage == 0)
        {
            break;
        }
    }

    printf("\nConnection made.\nFile received.");

    close(newSocket_desc); // closing sockets
    close(socket_desc);
    return 0;
}