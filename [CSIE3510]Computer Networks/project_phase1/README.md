# CN2022 Project Phase1 README

#### 1. Compile and execute the server

```shell
gcc server.c -o server
./server $PORTNUMBER
```

**For example:**

```shell
gcc server.c -o server
./server 8989
```

#### 2. Get profile from server

Type `{IP_ADDRESS or HOST_NAME}:{PORT}` in your browser.
**For example:**
If you run your server on `linux15.csie.ntu.edu.tw` with port `8989`, you can type `linux15.csie.ntu.edu.tw:8989` to get the profile.

#### 3. Send text to server

Use `telnet {IP_ADDRESS or HOST_NAME} {PORT}` to connect with your server.
**For example:**
If you run the server on `linux15.csie.ntu.edu.tw` with port `8989`, you can use 
`telnet linux15.csie.ntu.edu.tw 8989` to send text to the server.

#### 4.

You can type `linux15.csie.ntu.edu.tw:8989` to get my profile from my server and use 
`telnet linux15.csie.ntu.edu.tw 8989` to send text to my server.

