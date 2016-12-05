#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Setup trens
    trens.push_back(new Trem(1, 160, 30));
    trens.push_back(new Trem(2, 130, 170));
    trens.push_back(new Trem(3, 410, 170));
    trens.push_back(new Trem(4, 270, 270));

    for (auto i = 0u; i < trens.size(); i++) {
        connect(trens.at(i), SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));
        connect(trens.at(i), SIGNAL(updateNumerosSignal()), SLOT(updateNumeros()));
        trens.at(i)->start();
    }

    // Cria Semáforos
    semaforos.push_back(new Semaforo(1231, 1, IPC_CREAT|0600));  // Região Crítica 1
    semaforos.push_back(new Semaforo(1232, 1, IPC_CREAT|0600));  // Região Crítica 2
    semaforos.push_back(new Semaforo(1233, 1, IPC_CREAT|0600));  // Região Crítica 3

    // Associa semáforos com os trens
    // trem1 - apenas área crítica 1
    trens.at(0)->addSemaforo(semaforos.at(0));
    // trem2 - áreas críticas 1 e 2
    trens.at(1)->addSemaforo(semaforos.at(0));
    trens.at(1)->addSemaforo(semaforos.at(1));
    // trem3 - áreas críticas 2 e 3
    trens.at(2)->addSemaforo(semaforos.at(1));
    trens.at(2)->addSemaforo(semaforos.at(2));
    // trem4 - apenas área crítica 3
    trens.at(3)->addSemaforo(semaforos.at(2));

    server = std::thread(&MainWindow::watchServer, this);
}

MainWindow::~MainWindow() {
    server.join();
    delete ui;
}

void MainWindow::socketHandler(MainWindow *window, int socketDescriptor, Data data) {
    int byteslidos;

    //Verificando erros
    if (socketDescriptor == -1) {
        printf("Falha ao executar accept()");
        exit(EXIT_FAILURE);
    }

    // receber uma msg do cliente
    byteslidos = recv(socketDescriptor, &data, sizeof(data), 0);

    if (byteslidos == -1) {
        printf("Falha ao executar recv()");
        exit(EXIT_FAILURE);
    } else if (byteslidos == 0) {
        printf("Cliente finalizou a conexão\n");
        exit(EXIT_SUCCESS);
    }

    if (data.function == 1) {
        // Define a velocidade de todos os trens como "val1"
        window->setVelocity(data.val1);
    } else if (data.function == 2) {
        // Define a velocidade do trem "val1" como "val2"
        window->setVelocity(data.val1, data.val2);
    } else if (data.function == 3) {
        // Habilita ou desabilita todos os trens com "val1"
        window->setTrainEnable((bool)data.val1);
    } else if (data.function == 4) {
        // Habilita ou desabilita o trem "val1" com "val2"
        window->setTrainEnable(data.val1, (bool)data.val2);
    }

    ::close(socketDescriptor);
}

void MainWindow::watchServer() {
    struct sockaddr_in endereco;
    int socketId;

    // variáveis relacionadas com as conexões clientes
    struct sockaddr_in enderecoCliente;
    socklen_t tamanhoEnderecoCliente = sizeof(struct sockaddr);
    int conexaoClienteId;

    //mensagem enviada pelo cliente
    Data data;

    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    // endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_addr.s_addr = inet_addr("192.168.7.1");

    /*
     * Criando o Socket
     *
     * PARAM1: AF_INET ou AF_INET6 (IPV4 ou IPV6)
     * PARAM2: SOCK_STREAM ou SOCK_DGRAM
     * PARAM3: protocolo (IP, UDP, TCP, etc). Valor 0 escolhe automaticamente
     */
    socketId = socket(AF_INET, SOCK_STREAM, 0);

    // Verificar erros
    if (socketId == -1) {
        printf("Falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }

    // Conectando o socket a uma porta. Executado apenas no lado servidor
    if (bind(socketId, (struct sockaddr *) &endereco, sizeof(struct sockaddr)) == -1) {
        printf("Falha ao executar bind()\n");
        exit(EXIT_FAILURE);
    }

    // Habilitando o servidor a receber conexoes do cliente
    if (listen(socketId, 10) == -1) {
        printf("Falha ao executar listen()\n");
        exit(EXIT_FAILURE);
    }

    while (true) {
        // Servidor fica bloqueado esperando uma conexão do cliente
        conexaoClienteId = accept(socketId, (struct sockaddr *) &enderecoCliente, &tamanhoEnderecoCliente);

        //disparar a thread
        std::thread t(&MainWindow::socketHandler, this, conexaoClienteId, data);
        t.detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Deleta semáforos quando fecha a janela
    for (auto i = 0u; i < semaforos.size(); i++) {
        delete semaforos[i];
    }
    event->accept();
}

void MainWindow::updateNumeros() {
    ui->nbSem01->display(semaforos.at(0)->getContador());
    ui->nbSem02->display(semaforos.at(1)->getContador());
    ui->nbSem03->display(semaforos.at(2)->getContador());
}

void MainWindow::updateInterface(int id, int x, int y) {
    switch(id){
        case 1:
            ui->labelTrem01->setGeometry(x, y, 20, 20);
            break;
        case 2:
            ui->labelTrem02->setGeometry(x, y, 20, 20);
            break;
        case 3:
            ui->labelTrem03->setGeometry(x, y, 20, 20);
            break;
        case 4:
            ui->labelTrem04->setGeometry(x, y, 20, 20);
            break;
        default:
            break;
    }
}

void MainWindow::setVelocity(int velocity) {
    for (auto i = 0u; i < trens.size(); i++) {
        trens.at(i)->setVelocidade(velocity);
    }
}

void MainWindow::setVelocity(int train, int velocity) {
    trens.at(train)->setVelocidade(velocity);
}

void MainWindow::setTrainEnable(bool enable) {
    for (auto i = 0u; i < trens.size(); i++) {
        trens.at(i)->setEnable(enable);
    }
}

void MainWindow::setTrainEnable(int train, bool enable) {
    trens.at(train)->setEnable(enable);
}
