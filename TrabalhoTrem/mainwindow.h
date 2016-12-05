#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <vector>
#include "trem.h"
#include "semaforo.h"

namespace Ui {
class MainWindow;
}

struct Data {
    int function;
    int val1;
    int val2;
};

#define MAXNAME 100
#define PORTNUM 4325

class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setVelocity(int velocity);
    void setVelocity(int train, int velocity);
    void setTrainEnable(bool enable);
    void setTrainEnable(int train, bool enable);
    void watchServer();
    static void socketHandler(MainWindow *window, int socketDescriptor, Data data);

 public slots:
    void updateInterface(int, int, int);
    void updateNumeros();

 protected:
    void closeEvent(QCloseEvent *event);

 private:
    Ui::MainWindow *ui;
    std::vector<Trem *> trens;
    std::vector<Semaforo *> semaforos;
    std::thread server;
};

#endif // MAINWINDOW_H
