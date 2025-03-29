#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "serial.h"
#include <stdlib.h>


int serial_port, len;
char read_buf[121]; //texto read
char write_buf[121]; //texto write

struct termios tty; //Configurações da porta serial

void settings(){
    if(!isatty(serial_port)) {printf("Is not a tty");}

    //configuring the settings

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);


    tty.c_cflag &= ~CRTSCTS;

    

    tty.c_oflag = 0;
    
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    tty.c_lflag &= ~ICANON; // Non-canonical mode
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    cfsetspeed(&tty, B115200);

    //apply settings

    tcflush(serial_port, TCIFLUSH);
    if(tcsetattr(serial_port, TCSANOW, &tty) != 0){
        tcsetattr(serial_port, TCSANOW, &tty);
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        
    }

    
}

coid open_serial(char *device){
    serial_port = open(device, O_RDWR | O_NDELAY | O_NOCTTY);
    if (serial_port < 0) {
        printf("Error %i when opening %s\n", errno, strerror(errno));
        return -1;
    }

    settings();

    return serial_port;
}

void close_serial(){
    close(serial_port);
}

void read_serial(){

    int num_bytes;

    while(1){
   // if(read(serial_port, &read_buf, sizeof(read_buf)) > 0)
        num_bytes = read(serial_port, read_buf, sizeof(read_buf) - 1);
        if(num_bytes > 0){
            read_buf[num_bytes] = '\0';
            printf("%s", read_buf);
            fflush(stdout);
        }
    }
}

void write_serial(char *menssage){

    strcpy(write_buf, menssage);
    len = strlen(write_buf);
    len = write(serial_port, write_buf, len);
    printf("wrote %d bytes in UART\n", len);
}
