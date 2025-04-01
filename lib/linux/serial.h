#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>

typedef struct
{
    char data_types;
    char parity_bits;
    char stop_bits;
    char IX;
    char flow_control;
    int boundrate;
} SerialConfig;

int open_serial(const char *device, SerialConfig config);

void close_serial();

int read_serial();

int write_serial(const char *mensage);


#endif