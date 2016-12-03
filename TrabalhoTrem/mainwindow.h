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

 public slots:
    void updateInterface(int, int, int);

 protected:
    void closeEvent(QCloseEvent *event);

 private:
    Ui::MainWindow *ui;
    std::vector<Trem *> trens;
    std::vector<Semaforo *> semaforos;
};

#endif // MAINWINDOW_H
