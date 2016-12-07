#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBitmap>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->labelTrem01->setPixmap(QCoreApplication::applicationDirPath() + "/../img/train01.png");
    ui->labelTrem02->setPixmap(QCoreApplication::applicationDirPath() + "/../img/train02.png");
    ui->labelTrem03->setPixmap(QCoreApplication::applicationDirPath() + "/../img/train03.png");
    ui->labelTrem04->setPixmap(QCoreApplication::applicationDirPath() + "/../img/train04.png");

    // Create Semaphores
    sems = new std::vector<Semaphore *>();
    for (int i = 1; i <= 3; i++) {
        sems->push_back(new Semaphore(i, 1, IPC_CREAT|0600));  // Critical Section %i
    }

    // Setup trains
    trains = new std::vector<Train *>();
    trains->push_back(new Train(1, Shape(Position(130, 30)), Position(160, 30), sems, Direction::ANTICLOCKWISE));
    trains->push_back(new Train(2, Shape(Position(130, 130)), Position(130, 170), sems, Direction::CLOCKWISE));
    trains->push_back(new Train(3, Shape(Position(270, 130)), Position(410, 170), sems, Direction::CLOCKWISE));
    trains->push_back(new Train(4, Shape(Position(270, 230)), Position(270, 270), sems, Direction::CLOCKWISE));

    for (auto i = 0u; i < trains->size(); i++) {
        connect(trains->at(i), SIGNAL(updateGUI(int, int, int, bool)), SLOT(updateInterface(int, int, int, bool)));
        trains->at(i)->start();
    }

    // Start server
    server = std::thread(&MainWindow::watchServer, this);

    connect(this, SIGNAL(updateCounter()), SLOT(slotUpdateCounter()));
    counter = std::thread(&MainWindow::counterUpdate, this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Detach Threads
    server.detach();
    counter.detach();

    // Delete semaphores
    for (auto i = 0u; i < sems->size(); i++) {
        delete sems->at(i);
    }
    delete sems;

    // Delete Trains
    for (auto i = 0u; i < trains->size(); i++) {
        delete trains->at(i);
    }
    delete trains;

    event->accept();
}

void MainWindow::setSpeed(int speed) {
    for (auto i = 0u; i < trains->size(); i++) {
        trains->at(i)->setSpeed(speed);
    }
}

void MainWindow::setSpeed(int train, int speed) {
    trains->at(train)->setSpeed(speed);
}

void MainWindow::setTrainEnable(bool enable) {
    for (auto i = 0u; i < trains->size(); i++) {
        trains->at(i)->setEnable(enable);
    }
}

void MainWindow::setTrainEnable(int train, bool enable) {
    trains->at(train)->setEnable(enable);
}

void MainWindow::counterUpdate() {
    while (true) {
        emit(updateCounter());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void MainWindow::slotUpdateCounter() {
    if (sems != nullptr && sems->size() == 3) {
        ui->nbSem01->display(sems->at(0)->getCounter());
        ui->nbSem02->display(sems->at(1)->getCounter());
        ui->nbSem03->display(sems->at(2)->getCounter());
    }
}

void MainWindow::updateInterface(int id, int x, int y, bool critic) {
    switch (id) {
        case 1:
            ui->labelTrem01->setGeometry(x, y, 20, 20);
            ui->labelTrem01->setStyleSheet(critic ? "border: 1px solid red" : "border: none");
            break;
        case 2:
            ui->labelTrem02->setGeometry(x, y, 20, 20);
            ui->labelTrem02->setStyleSheet(critic ? "border: 1px solid red" : "border: none");
            break;
        case 3:
            ui->labelTrem03->setGeometry(x, y, 20, 20);
            ui->labelTrem03->setStyleSheet(critic ? "border: 1px solid red" : "border: none");
            break;
        case 4:
            ui->labelTrem04->setGeometry(x, y, 20, 20);
            ui->labelTrem04->setStyleSheet(critic ? "border: 1px solid red" : "border: none");
            break;
        default:
            break;
    }
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
    endereco.sin_addr.s_addr = inet_addr(IP_SERVER);

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
    }
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
    printf("Received Message: {%i, %i, %i}\n", data.op, data.v1, data.v2);

    if (byteslidos == -1) {
        printf("Falha ao executar recv()");
        exit(EXIT_FAILURE);
    } else if (byteslidos == 0) {
        printf("Cliente finalizou a conexão\n");
        exit(EXIT_SUCCESS);
    }

    if (data.op == 1) {
        // Define a velocidade de todos os trens como "v1"
        window->setSpeed(data.v1);
    } else if (data.op == 2) {
        // Define a velocidade do trem "v1" como "v2"
        window->setSpeed(data.v1, data.v2);
    } else if (data.op == 3) {
        // Habilita ou desabilita todos os trens com "v1"
        window->setTrainEnable((bool)data.v1);
    } else if (data.op == 4) {
        // Habilita ou desabilita o trem "v1" com "v2"
        window->setTrainEnable(data.v1, (bool)data.v2);
    }

    ::close(socketDescriptor);
}
