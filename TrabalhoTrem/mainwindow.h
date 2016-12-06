#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <vector>
#include "train.h"
#include "semaphore.h"

namespace Ui {
class MainWindow;
}

struct Data {
    int op, v1, v2;
};

#define MAXNAME 100
#define PORTNUM 4325

class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setSpeed(int);
    void setSpeed(int, int);
    void setTrainEnable(bool);
    void setTrainEnable(int, bool);
    void counterUpdate();
    void watchServer();
    static void socketHandler(MainWindow *window, int socketDescriptor, Data data);

 public slots:
    void updateInterface(int, int, int);
    void slotUpdateCounter();

 signals:
    void updateCounter();

 protected:
    void closeEvent(QCloseEvent *event);

 private:
    Ui::MainWindow *ui;
    std::vector<Train *> *trains;
    std::vector<Semaphore *> *sems;
    std::thread server;
    std::thread counter;
};

#endif // MAINWINDOW_H
