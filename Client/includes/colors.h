#ifndef _colors_h_
#define _colors_h_

#include "BlackGPIO.h"

void red_light(BlackLib::BlackGPIO (&rgb_led)[3]);

void green_light(BlackLib::BlackGPIO (&rgb_led)[3]);

void blue_light(BlackLib::BlackGPIO (&rgb_led)[3]);

void white_light(BlackLib::BlackGPIO (&rgb_led)[3]);

void yellow_light(BlackLib::BlackGPIO (&rgb_led)[3]);

void pink_light(BlackLib::BlackGPIO (&rgb_led)[3]);

void cyan_light(BlackLib::BlackGPIO (&rgb_led)[3]);

void turn_off(BlackLib::BlackGPIO (&rgb_led)[3]);

#endif
