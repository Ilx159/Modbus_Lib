#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


int serial_port, len;
char text[255]; //texto

struct termios tty; //Configurações da porta serial

void settings(){
    if(!isatty(serial_port)) {printf("Is not a tty");}

    //configuring the settings

    tty.c_cflag &= B9600 | CS8 | CLOCAL | CLOCAL | ~(CSTOPB | PARENB);

    tty.c_iflag &= IGNPAR | ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    tty.c_oflag &= ~(OPOST | ONLCR);
    tty.c_lflag &= ~(ICANON | ISIG);
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    cfsetspeed(&tty, B9600);

    //apply settings

    tcflush(serial_port, TCIFLUSH);
    if(tcsetattr(serial_port, TCSANOW, &tty) != 0){
        tcsetattr(serial_port, TCSANOW, &tty);
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        
    }
    

}

int open_serial(char device[64]){
    serial_port = open(device, O_RDWR | O_NDELAY | O_NOCTTY);
    if (serial_port < 0) {
        printf("Error %i when opening %s\n", errno, strerror(errno));
        return -1;
    }

    
}

int close_serial(){
    close(serial_port);
}

int read_serial(){

    char read_buf[255];

    int n = read(serial_port, &read_buf, sizeof(read_buf));

    printf("%s\n", read_buf);

}

int write_serial(char mensage[255]){

    strcpy(text, mensage);
    len = strlen(text);
    len = write(serial_port, text, len);
    printf("wrote %d bytes in UART", len);

}
