# Computer-Network-FinalProject
The final project of Computer Network 2022 (@NTU) (Project phase 2)

### Team Number
```
r10625016 許致銓
b08902149 徐晨祐
```

### Deploy IP & Port
```
# For web page
http://linux15.csie.ntu.edu.tw:8999

# For video(In your local side)
./client 140.112.30.46:8787 -video

# For audio(In your local side)
./client 140.112.30.46:8788 -audio
```

### The Function of this Server
- Allow multiple users comment on the board 
- Users can register / login / log out on this website
- Both frontend and backend are developed in C++

### Other Features
- Used `poll` to perform multiplexing
- Error Detection: 
    - When users try to login, the server will check if the password is correct
    - When users try to register a new account, the server will check if the account has been used
    - When users enter data, the server will ensure that no fields are left blank

### Usage
```shell
# at your server side
make
# initialize the database and run the server
./server [PORT_NUMBER] --init
# if you don't want to initialize the database, just run the following command
./server [PROT_NUMBER]
# Send the video/audio
./server [PORT_NUMBER] -video/-audio

# at your client side
make
./client [BACKEND_IP]:[BACKEND_PORTNUMBER] [FRONTEND_PORTNUMBER]
# receive video/audio
./client [BACKEND_IP]:[BACKEND_PORTNUMBER] -video/-audio
```
After running all these commands, you can use `http://[FRONTEND_IP]:[FRONTEND_PORTNUMBER]` to connect the website.
For example, if you want to set up both frontend and backend at `linux15.csie.ntu.edu.tw`, you can run the following commands on that machine.

```shell
make
./server 8999

# use another shell
make
./client 127.0.0.1:8999 8989
```
Now you can use `http://linux15.csie.ntu.edu.tw:8989` to browse the website.