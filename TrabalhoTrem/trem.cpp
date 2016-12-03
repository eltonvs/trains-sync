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

void Trem::run() {
    while (true) {
        switch (id) {
        case 1:
            if (enable) {
                emit updateGUI(id, x, y);
                if (y == 30 && x > 130) // em cima
                    x -= 10;
                else if (x == 130 && y < 130) // lado esquerdo
                    y += 10;
                else if (x < 270 && y == 130) // embaixo
                    x += 10;
                else // lado direito
                    y -= 10;
            }
            break;
        case 2:
            if (enable) {
                emit updateGUI(id, x, y);
                if (y == 130 && x < 270)  // em cima
                    x += 10;
                else if (x == 270 && y < 230)  // lado direito
                    y += 10;
                else if (x > 130 && y == 230)  // embaixo
                    x -= 10;
                else  // lado esquerdo
                    y -= 10;
            }
            break;
        case 3:
            if (enable) {
                emit updateGUI(id, x, y);
                if (y == 130 && x < 410) // em cima
                    x += 10;
                else if (x == 410 && y < 230) // lado direito
                    y += 10;
                else if (x > 270 && y == 230) // embaixo
                    x -= 10;
                else // lado esquerdo
                    y -= 10;
            }
            break;
        case 4:
            if (enable) {
                emit updateGUI(id, x, y);
                if (y == 230 && x < 410) // em cima
                    x += 10;
                else if (x == 410 && y < 330) // lado direito
                    y += 10;
                else if (x > 270 && y == 330) // embaixo
                    x -= 10;
                else // lado esquerdo
                    y -= 10;
            }
            break;
        default:
            break;
        }

        verifyRegion();
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
        // verifica se chegou na segunda região crítica
        if (x == 260 && y == 130) {
            //
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
        // verifica se chegou na segunda região crítica
        if (x == 280 && y == 230) {
            //
        }
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
