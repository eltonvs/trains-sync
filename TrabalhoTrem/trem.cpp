#include "trem.h"

Trem::Trem(int id, int x, int y) {
    this->id = id;
    this->x = x;
    this->y = y;
    this->velocidade = 250;
    this->enable = true;
    this->semaforos = std::vector<Semaforo *>();
}

Trem::~Trem() {
    threadTrem.join();
}

void Trem::setVelocidade(int velocidade) {
    this->velocidade = velocidade;
}

void Trem::setEnable(bool enable) {
    this->enable = enable;
}

void Trem::addSemaforo(Semaforo *semaforo) {
    this->semaforos.push_back(semaforo);
}

void Trem::start() {
    threadTrem = std::thread(&Trem::run, this);
}

void Trem::move(int type, int xi, int yi, int w, int h) {
    if (type == 1) {  // anti-horario
        if (y == yi && x > xi) // em cima
            x -= 10;
        else if (x == xi && y < (yi + h)) // lado esquerdo
            y += 10;
        else if (x < (xi + w) && y == (yi + h)) // embaixo
            x += 10;
        else // lado direito
            y -= 10;
    } else {  // horario
        if (y == yi && x < (xi + w))  // em cima
            x += 10;
        else if (x == (xi + w) && y < (yi + h))  // lado direito
            y += 10;
        else if (x > xi && y == (yi + h))  // embaixo
            x -= 10;
        else  // lado esquerdo
            y -= 10;
    }
}

void Trem::run() {
    while (true) {
        if (enable) {
            if (id == 1) {
                emit updateGUI(id, x, y);
                move(1, 130, 30);
            } else if (id == 2) {
                emit updateGUI(id, x, y);
                move(0, 130, 130);
            } else if (id == 3) {
                emit updateGUI(id, x, y);
                move(0, 270, 130);
            } else if (id == 4) {
                emit updateGUI(id, x, y);
                move(0, 270, 230);
            }
            verifyRegion();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(velocidade));
    }
}

void Trem::verifyRegion() {
    if (id == 1) {
        // verifica se chegou no inicio da primeira região crítica
        if (x == 130 && y == 120) {
            semaforos.at(0)->P();
        }
        // verifica se saiu da primeira região crítica
        if (x == 270 && y == 120) {
            semaforos.at(0)->V();
        }
    } else if (id == 2) {
        // verifica se chegou na primeira região crítica
        if (x == 130 && y == 140) {
            semaforos.at(0)->P();
            semaforos.at(1)->P();
        }
        // verifica se saiu da primeira região crítica
        if (x == 270 && y == 140) {
            semaforos.at(0)->V();
        }
        // verifica se saiu da segunda região crítica
        if (x == 260 && y == 230) {
            semaforos.at(1)->V();
        }
    } else if (id == 3) {
        // verifica se chegou na terceira região crítica
        if (x == 410 && y == 220) {
            semaforos.at(1)->P();
            semaforos.at(0)->P();
        }
        // verifica se saiu da terceira região crítica
        if (x == 270 && y == 220) {
            semaforos.at(1)->V();
        }
        // verifica se saiu da segunda região crítica
        if (x == 280 && y == 130) {
            semaforos.at(0)->V();
        }
    } else if (id == 4) {
        // verifica se chegou no inicio da terceira região crítica
        if (x == 270 && y == 240) {
            semaforos.at(0)->P();
        }
        // verifica se saiu da terceira região crítica
        if (x == 410 && y == 240) {
            semaforos.at(0)->V();
        }
    }
}
