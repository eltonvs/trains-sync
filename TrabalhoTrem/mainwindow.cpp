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
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Deleta semáforos quando fecha a janela
    for (auto i = 0u; i < semaforos.size(); i++) {
        delete semaforos[i];
    }
    event->accept();
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
