#include <stdio.h>
#include "libs/serial.h"

int main() {

    open_serial("/dev/ttyUSB0");
    write_serial("oi");
    read_serial();
    close_serial();
    return 0;
}