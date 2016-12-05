#include <cstdio>       //printf
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <iostream>     //close

#define MAXMSG 1024
#define MAXNAME 100
#define PORTNUM 4325

struct Data {
    int function;
    int val1;
    int val2;
};

int main(int argc, char *argv[]) {

    struct sockaddr_in endereco;
    int socketId;
    Data data;

    std::cout << "function to execute: [1 - 4]: ";
    std::cin >> data.function;

    std::cout << "val1 ";
    std::cin >> data.val1;

    if (data.function == 2 || data.function == 4) {
        std::cout << "val2 ";
        std::cin >> data.val2;
    }

    int bytesenviados;

    /*
     * Configurações do endereço
    */
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    endereco.sin_addr.s_addr = inet_addr("192.168.7.1");

    /*
     * Criando o Socket
     *
     * PARAM1: AF_INET ou AF_INET6 (IPV4 ou IPV6)
     * PARAM2: SOCK_STREAM ou SOCK_DGRAM
     * PARAM3: protocolo (IP, UDP, TCP, etc). Valor 0 escolhe automaticamente
    */
    socketId = socket(AF_INET, SOCK_STREAM, 0);

    //Verificar erros
    if (socketId == -1) {
        printf("Falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }

    //Conectando o socket cliente ao socket servidor
    if (connect(socketId, (struct sockaddr *) &endereco, sizeof(struct sockaddr)) == -1) {
        printf("Falha ao executar connect()\n");
        exit(EXIT_FAILURE);
    }

    printf("Cliente conectado ao servidor\n");

    //Enviar uma msg para o cliente que se conectou
    printf("Cliente vai enviar uma mensagem\n");
    bytesenviados = send(socketId,&data,sizeof(data),0);

    if (bytesenviados == -1) {
        printf("Falha ao executar send()");
        exit(EXIT_FAILURE);
    }
    printf("Cliente enviou a seguinte msg (%d bytes) para o servidor: %i \n",bytesenviados, data.function);

    close(socketId);

    return 0;
}
