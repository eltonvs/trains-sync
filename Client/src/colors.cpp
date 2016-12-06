#include "BlackGPIO.h"
#include "colors.h"
#include "macros.h"

extern const int NUMBER_OF_COLORS = 6;

void red_light(BL_GPIO (&rgb_led)[3]) {
    rgb_led[0].TURN_ON;
    rgb_led[1].TURN_OFF;
    rgb_led[2].TURN_OFF;
}

void green_light(BL_GPIO (&rgb_led)[3]) {
    rgb_led[0].TURN_OFF;
    rgb_led[1].TURN_ON;
    rgb_led[2].TURN_OFF;
}

void blue_light(BL_GPIO (&rgb_led)[3]) {
    rgb_led[0].TURN_OFF;
    rgb_led[1].TURN_OFF;
    rgb_led[2].TURN_ON;
}

void white_light(BL_GPIO (&rgb_led)[3]) {
    rgb_led[0].TURN_ON;
    rgb_led[1].TURN_ON;
    rgb_led[2].TURN_ON;
}

void yellow_light(BL_GPIO (&rgb_led)[3]) {
    rgb_led[0].TURN_ON;
    rgb_led[1].TURN_ON;
    rgb_led[2].TURN_OFF;
}

void pink_light(BL_GPIO (&rgb_led)[3]) {
    rgb_led[0].TURN_ON;
    rgb_led[1].TURN_OFF;
    rgb_led[2].TURN_ON;
}

void cyan_light(BL_GPIO (&rgb_led)[3]) {
    rgb_led[0].TURN_OFF;
    rgb_led[1].TURN_ON;
    rgb_led[2].TURN_ON;
}

void turn_off(BL_GPIO (&rgb_led)[3]) {
    rgb_led[0].TURN_OFF;
    rgb_led[1].TURN_OFF;
    rgb_led[2].TURN_OFF;
}
