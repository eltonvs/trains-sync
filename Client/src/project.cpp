#include <iostream>

#include "beagle.h"
#include "socket.h"

#define PORTNUM 4325
#define POTENTIOMETER_MAX_VAL 4096

struct Data {
    int op, v1, v2;
};

void menu() {
    system("clear");
    std::cout << std::string(15, '-') << " MENU: " << std::string(15, '-') << "\n";
    std::cout << "1) Set speed for all trains\n";
    std::cout << "2) Set speed for a specific train\n";
    std::cout << "3) Enable/Disable all trains\n";
    std::cout << "4) Enable/Disable a specific train\n";
    std::cout << "0) Quit\n";
    std::cout << std::string(37, '-') << "\n";
}

int getPotentiometerStep(Beagle *b) {
    int step = 0;
    while (!b->getButtonValue()) {
        int p_val = b->getPotentiometerValue();
        if (p_val < POTENTIOMETER_MAX_VAL/3) {
            b->setColorRGB(RED);
            step = 0;
        } else if (p_val < 2*(POTENTIOMETER_MAX_VAL/3)) {
            b->setColorRGB(GREEN);
            step = 1;
        } else {
            b->setColorRGB(BLUE);
            step = 2;
        }
    }
    return step;
}

bool getPotentiometerBinary(Beagle *b) {
    bool r = false;
    while (!b->getButtonValue()) {
        int p_val = b->getPotentiometerValue();
        if (p_val < POTENTIOMETER_MAX_VAL/2) {
            b->setColorRGB(RED);
            r = false;
        } else {
            b->setColorRGB(GREEN);
            r = true;
        }
    }
    return r;
}

int main() {
    Socket<Data> s(PORTNUM, "192.168.7.1");
    Beagle b;
    Data data;

    b.turnLedOn();

    while (true) {
        do {
            menu();
            std::cout << "$ ";
            std::cin >> data.op;
        } while (data.op < 0 || data.op > 4);

        if (data.op == 0) {
            std::cout << "\n>>> Exiting...\n";
            b.turnLedOff();
            b.turnOffRGB();
            return 0;
        } else if (data.op == 1) {
            std::cout << "\n>>> Set speed for all trains\n";
            std::cout << "Turn the potentiometer to change speed (Red - Fast | Green - Normal | Blue - Slow)\n";
            int selected = getPotentiometerStep(&b);
            data.v1 = selected == 0 ? 10 : selected == 1 ? 50 : 150;
        } else if (data.op == 2) {
            std::cout << "\n>>> Set speed for a specific train\n";
            std::cout << "Train [1 - 4]: ";
            do {
                std::cin >> data.v1;
            } while (data.v1 < 1 || data.v1 > 4);
            data.v1--;
            std::cout << "Turn the potentiometer to change speed (Red - Fast | Green - Normal | Blue - Slow)\n";
            int selected = getPotentiometerStep(&b);
            data.v2 = selected == 0 ? 10 : selected == 1 ? 50 : 150;
        } else if (data.op == 3) {
            std::cout << "\n>>> Enable/Disable all trains\n";
            std::cout << "What do you want? (Green - Enable | Red - Disable)\n";
            data.v1 = getPotentiometerBinary(&b);
        } else {
            std::cout << "\n>>> Enable/Disable a specific train\n";
            std::cout << "Train [1 - 4]: ";
            do {
                std::cin >> data.v1;
            } while (data.v1 < 1 || data.v1 > 4);
            data.v1--;
            std::cout << "What do you want? (Green - Enable | Red - Disable)\n";
            data.v2 = getPotentiometerBinary(&b);
        }

        b.turnOffRGB();

        s.setData(data);
        s.send();
    }

    b.turnLedOff();
    b.turnOffRGB();

    return 0;
}
