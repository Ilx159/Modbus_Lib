#include <stdio.h>
#include "serial.h"

int main() {
    char porta[32];
    printf("Digite a localização da porta:\n");
    scanf("%s", porta);
    open_serial(porta);
    write_serial("oi");
    read_serial();
    close_serial();
    
    return 0;
}