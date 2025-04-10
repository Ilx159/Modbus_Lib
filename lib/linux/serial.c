#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "serial.h"
#include <stdlib.h>


int serial_port;





static struct termios tty; //Configurações da porta serial

void settings(SerialConfig config){

    if(!isatty(serial_port)) {printf("Is not a tty");}

    //configuring the settings

    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_lflag &= ~ICANON; // Non-canonical mode
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_cflag &= ~CSIZE;
    tty.c_oflag = 0;
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    //user config
    
    //flow_control
    if(config.flow_control == 1)
        tty.c_cflag |= CRTSCTS;
    else if(config.flow_control == 0)
        tty.c_cflag &= ~CRTSCTS;

    //IXON, IXOFF, IXANY
    if(config.IX == 1)
        tty.c_iflag |= IXON | IXOFF | IXANY;
    else if(config.IX == 0)
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    //Parity_bits
    if(config.parity_bits == 1)
        tty.c_cflag |= PARENB;
    else if(config.parity_bits == 0)
        tty.c_cflag &= ~PARENB;

    //stop_bits
    if(config.stop_bits == 2)
        tty.c_cflag |= CSTOPB; 
    else if(config.stop_bits == 1)
        tty.c_cflag &= ~CSTOPB;

    //data_type
    if(config.data_types == 5)
        tty.c_cflag |= CS5;
    else if(config.data_types == 6)
        tty.c_cflag |= CS6;
    else if(config.data_types == 7)
        tty.c_cflag |= CS7;
    else if(config.data_types == 8)
        tty.c_cflag |= CS8;

    //boundrate
    switch (config.boundrate){
        case 1800:
            cfsetspeed(&tty, B1800);
            break;

        case 2400:
            cfsetspeed(&tty, B2400);
            break;
        
        case 4800:
            cfsetspeed(&tty, B4800);
            break;
        
        case 9600:
            cfsetspeed(&tty, B9600);
            break;

        case 19200:
            cfsetspeed(&tty, B19200);
            break;

        case 38400:
            cfsetspeed(&tty, B38400);
            break;
        case 115200:
            cfsetspeed(&tty, B115200);
            break;
        
        default:
            break;
    }

    //apply settings

    tcflush(serial_port, TCIFLUSH);
    if(tcsetattr(serial_port, TCSANOW, &tty) != 0){
        tcsetattr(serial_port, TCSANOW, &tty);
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        
    }

    
}

int open_serial(const char *device, SerialConfig config){
    serial_port = open(device, O_RDWR | O_NDELAY | O_NOCTTY);
    if (serial_port < 0) {
        printf("Error %i when opening %s\n", errno, strerror(errno));
        return -1;
    }

    settings(config);

    return serial_port;
}

void close_serial(){
    close(serial_port);
}

int read_serial(){
    static char read_buf[121]; //texto read
    
    int num_bytes = read(serial_port, read_buf, sizeof(read_buf) - 1);
        if(num_bytes > 0){
            printf("funcionando");
            read_buf[num_bytes] = '\0';
            printf("%d / %s", num_bytes, read_buf);
            fflush(stdout);
        } else if (num_bytes < 0)
        printf("erro na leitura");
    
    return num_bytes;
}

int write_serial(const char *menssage){
    char write_buf[256]; //texto write
    if(strlen(menssage) >= sizeof(write_buf)){
        fprintf(stderr, "Mensagem muito longa, max:255 caracteres.\n");
        return -1;
    } else {
    int len;
    strcpy(write_buf, menssage);
    len = strlen(write_buf);
    if(len <= 0){
        printf("Invalid text");
        return -1;
    }

    len = write(serial_port, write_buf, len);
    printf("wrote %d bytes in UART\n", len);
    tcdrain(serial_port);

    return len;
    }
}