#include <cstdio>       //printf
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <iostream>

#define PORTNUM 4325
#define SERVERIP "192.168.7.1"

struct Data {
    int op, v1, v2;
};

void menu() {
    system("clear");
    std::cout << std::string(15, '-') << " MENU: " << std::string(15, '-') << "\n";
    std::cout << "1) Set speed for all trains\n";
    std::cout << "2) Set speed for a specific train\n";
    std::cout << "3) Enable/Disable all trains\n";
    std::cout << "4) Enable/Disable a specific train\n";
    std::cout << "0) Quit\n";
    std::cout << std::string(37, '-') << "\n";
}

bool sendData(Data &d) {
    struct sockaddr_in address;
    int socketId;
    int sentData;

    // Address Settings
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORTNUM);
    address.sin_addr.s_addr = inet_addr(SERVERIP);

    // Create Socket
    socketId = socket(AF_INET, SOCK_STREAM, 0);

    // Check Errors
    if (socketId == -1) {
        std::cerr << "Fail at socket()\n";
        return false;
    }

    // Conectando o socket cliente ao socket servidor
    if (connect(socketId, (struct sockaddr *) &address, sizeof(struct sockaddr)) == -1) {
        std::cerr << "Fail at connect()\n";
        return false;
    }

    sentData = send(socketId, &d, sizeof(d), 0);

    if (sentData == -1) {
        std::cerr << "Fail at send()";
        return false;
    }

    close(socketId);

    return true;
}

int main() {
    Data data;

    while (true) {
        do {
            menu();
            std::cout << "$ ";
            std::cin >> data.op;
        } while (data.op < 0 || data.op > 4);

        if (data.op == 0) {
            std::cout << "\n>>> Exiting...\n";
            return 0;
        } else if (data.op == 1) {
            std::cout << "\n>>> Set speed for all trains\n";
            std::cout << "New Speed: ";
            std::cin >> data.v1;
        } else if (data.op == 2) {
            std::cout << "\n>>> Set speed for a specific train\n";
            std::cout << "Train [1 - 4]: ";
            do {
                std::cin >> data.v1;
            } while (data.v1 < 1 || data.v1 > 4);
            data.v1--;
            std::cout << "New Speed: ";
            std::cin >> data.v2;
        } else if (data.op == 3) {
            std::cout << "\n>>> Enable/Disable all trains\n";
            std::cout << "What do you want? (1 - Enable | 0 - Disable) ";
            do {
                std::cin >> data.v1;
            } while (data.v1 && data.v1 != 1);
        } else {
            std::cout << "\n>>> Enable/Disable a specific train\n";
            std::cout << "Train [1 - 4]: ";
            do {
                std::cin >> data.v1;
            } while (data.v1 < 1 || data.v1 > 4);
            data.v1--;
            std::cout << "What do you want? (1 - Enable | 0 - Disable) ";
            do {
                std::cin >> data.v2;
            } while (data.v2 && data.v2 != 1);
        }

        sendData(data);
    }

    return 0;
}
