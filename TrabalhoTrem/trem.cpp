#include "trem.h"

Trem::Trem(int id, int x, int y) {
    this->id = id;
    this->x = x;
    this->y = y;
    velocidade = 250;
    enable = true;
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

void Trem::start() {
    threadTrem = std::thread(&Trem::run, this);
}

void Trem::run() {
    while(true) {
        switch(id) {
        case 1:
            if (enable) {
                emit updateGUI(id, x, y);
                if (y == 30 && x > 130) // em cima
                    x -= 10;
                else if (x == 130 && y < 130) // lado esquerdo
                    y += 10;
                else if (x < 270 && y == 130) // em baixo
                    x += 10;
                else // lado direito
                    y -= 10;
            }
            break;
        case 2:
            if (enable) {
                emit updateGUI(id, x, y);
                if (y == 130 && x < 270) // em cima
                    x += 10;
                else if (x == 270 && y < 230) // lado direito
                    y += 10;
                else if (x > 130 && y == 230) // em baixo
                    x -= 10;
                else // lado esquerdo
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
                else if (x > 270 && y == 230) // em baixo
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
                else if (x > 270 && y == 330) // em baixo
                    x -= 10;
                else // lado esquerdo
                    y -= 10;
            }
            break;
        default:
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(velocidade));
    }
}
