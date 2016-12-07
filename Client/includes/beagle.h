#ifndef _beagle_h_
#define _beagle_h_

#include <vector>

#include "BlackGPIO.h"
#include "macros.h"
#include "analog.h"
#include "colors.h"

enum Colors {RED, GREEN, BLUE, WHITE, YELLOW, PINK, CYAN};

class Beagle {
 public:
    Beagle();
    ~Beagle();
    void turnLedOn();
    void turnLedOff();
    void setColorRGB(Colors);
    void turnOffRGB();
    int getPotentiometerValue();
    int getButtonValue();

 private:
    BL_GPIO button = BL_GPIO(BlackLib::GPIO_115, BL_IN, BL_SM);
    BL_GPIO statusLed = BL_GPIO(BlackLib::GPIO_50, BL_OUT, BL_SM);
    BL_GPIO rgbLed[3] = {
        BL_GPIO(BlackLib::GPIO_46, BL_OUT, BL_SM),
        BL_GPIO(BlackLib::GPIO_26, BL_OUT, BL_SM),
        BL_GPIO(BlackLib::GPIO_44, BL_OUT, BL_SM)
    };
    std::vector<void (*)(BL_GPIO (&)[3])> colors;
    int potentiometerID = 1;
};

#endif  // _beagle_h_
