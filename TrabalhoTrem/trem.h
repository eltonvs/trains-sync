#ifndef TREM_H
#define TREM_H

#include <QObject>
#include <thread>
#include <chrono>
#include <vector>
#include "semaforo.h"

class Trem : public QObject {
    Q_OBJECT
 public:
    Trem(int, int, int);
    ~Trem();
    void start();
    void move(int type, int xi, int yi, int w = 140, int h = 100);
    void run();
    void verifyRegion();
    void setVelocidade(int);
    void setEnable(bool);
    void addSemaforo(Semaforo *semaforo);

 signals:
    void updateGUI(int, int, int);

 private:
   std::thread threadTrem;
   int id;
   int x;
   int y;
   int velocidade;
   bool enable;
   std::vector<Semaforo *> semaforos;
};

#endif // TREM_H
