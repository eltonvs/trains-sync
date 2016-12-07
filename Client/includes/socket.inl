#include "socket.h"

template <typename T>
Socket<T>::Socket(int _port, const char *_ip) {
    port = _port;
    ip = _ip;

    // Address Settings
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
}

template <typename T>
Socket<T>::~Socket() {}

template <typename T>
void Socket<T>::setData(T &d) {
    this->data = d;
}

template <typename T>
bool Socket<T>::send() {
    // Create Socket
    ID = ::socket(AF_INET, SOCK_STREAM, 0);

    // Check Errors
    if (ID == -1) {
        std::cerr << "Fail at socket()\n";
        return false;
    }

    // Connect client socket with server
    if (::connect(ID, (struct sockaddr *) &address, sizeof(struct sockaddr)) == -1) {
        std::cerr << "Fail at connect()\n";
        return false;
    }

    if (::send(ID, &data, sizeof(data), 0) == -1) {
        std::cerr << "Fail at send()";
        return false;
    }

    ::close(ID);

    return true;
}