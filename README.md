## MULTICLIENT EPOOL BASED ECHO SERVER

### Enviornment
* ***Operating System:*** Pop!_OS (Ubuntu-based)
* ***Language:*** C++
* ***Build System:*** CMake
* ***Dependencies:*** Standard POSIX socket libraries (available by default on Linux)

### Project structure
```bash
MultiClientAsyncEpollEchoServer/
├── CMakeLists.txt
├── LICENSE
├── README.md
└── src
    └── server.cpp
```
### Instructions to run the project

#### Requirements
* gcc or g++ (minimum 12.3.0)
* Cmake (4.2.0)

#### 1.Clone the repository

```bash
https://github.com/Pram018/MultiClientAsyncEpollEchoServer.git
cd MultiClientAsyncEpollEchoServer/
```

#### 2.Build the project

```bash
mkdir build && cd build
cmake ..
make
```

#### 3.Run the server

```bash
./bin/threadpool_server
```

#### 4.Test the server

You can use `netcat` or a custom client:

```bash
telnet 127.0.0.1 54000
```


