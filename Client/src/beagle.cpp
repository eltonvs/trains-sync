#include "beagle.h"

Beagle::Beagle() {
    // Add func to vector
    colors.push_back(red_light);
    colors.push_back(green_light);
    colors.push_back(blue_light);
    colors.push_back(white_light);
    colors.push_back(yellow_light);
    colors.push_back(pink_light);
    colors.push_back(cyan_light);
}

Beagle::~Beagle() {
    turnLedOff();
    turnOffRGB();
}

void Beagle::turnLedOn() {
    statusLed.TURN_ON;
}

void Beagle::turnLedOff() {
    statusLed.TURN_OFF;
}

void Beagle::setColorRGB(Colors c) {
    colors.at(c)(rgbLed);
}

void Beagle::turnOffRGB() {
    turn_off(rgbLed);
}

int Beagle::getPotentiometerValue() {
    return readAnalog(potentiometerID);
}

int Beagle::getButtonValue() {
    return button.getNumericValue();
}
