#ifndef _socket_h_
#define _socket_h_

#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <iostream>

template <typename T>
class Socket {
 public:
    Socket(int, const char *);
    ~Socket();
    void setData(T&);
    bool send();

 private:
    int ID;
    int port;
    const char *ip;
    struct sockaddr_in address;
    T data;
};

#include "socket.inl"

#endif  // _socket_h_
