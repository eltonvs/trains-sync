#include <sstream>
#include <fstream>
#include "analog.h"

#define PATH_ADC "/sys/bus/iio/devices/iio:device0/in_voltage"

int readAnalog(int number) {
    std::stringstream ss;
    ss << PATH_ADC << number << "_raw";
    std::fstream fs;
    fs.open(ss.str().c_str(), std::fstream::in);
    fs >> number;
    fs.close();

    return number;
}
