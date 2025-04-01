#include <stdio.h>
#include "serial.h"

int main() {

    SerialConfig config = {
        .data_types = 8,
        .parity_bits = 0,
        .stop_bits = 1,
        .IX = 0,
        .flow_control = 0,
        .boundrate = 115200,
    };

    char mensage[256];
    char porta[32];
    
    printf("Digite a localização da porta:\n");
    scanf("%s", porta);
    open_serial(porta, config);
    write_serial("Oi\r\n");
    while(1){
    read_serial();
    }
    close_serial();
    
    return 0;
}