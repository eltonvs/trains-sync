#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "trem.h"
#include "semaforo.h"

namespace Ui {
class MainWindow;
}

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
