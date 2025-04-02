#include <stdio.h>
#include "serial.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>

//init variables

volatile char running;
char porta[32];
char mensage[256] = "";
int serial_porta;

//threads functions

void* thread_write (void* vargp)
    {
        while(running){
            if(fgets(mensage, sizeof(mensage), stdin) != NULL){
                if (!(strcmp(mensage, "exit\n"))){
                    running = 0;
                    break;
                }
                
                mensage[strcspn(mensage, "\n")] = '\r';
                mensage[strcspn(mensage, "\n")+1] = '\n';
                if (!(strcmp(mensage, "exit\n"))){
                    running = 0;
                    break;
                }
                write_serial(mensage);
                tcdrain(serial_porta);
            }
        }
        return NULL;
    }

void* thread_read (void* vargp)
    {
        while(running){
            read_serial();
            tcdrain(serial_porta);
            usleep(500000);
        }
        return NULL;
    }


//                          //
//          MAIN            //
//                          //



int main() {

//User Settings of Serial

    SerialConfig config = {
        .data_types = 8,
        .parity_bits = 0,
        .stop_bits = 1,
        .IX = 0,
        .flow_control = 0,
        .boundrate = 115200,
    };
    

    


    printf("Digite a localização da porta:\n");//port init
    fgets(porta, sizeof(porta), stdin);
    porta[strcspn(porta, "\n")] = '\0';
    serial_porta = open_serial(porta, config);
    
    running = 1;

    pthread_t thread_id_read;
    pthread_t thread_id_write;

    pthread_create(&thread_id_read, NULL, thread_read, NULL);//threads init
    pthread_create(&thread_id_write, NULL, thread_write, NULL);
 
    pthread_join(thread_id_write, NULL);
    pthread_join(thread_id_read, NULL);
    

    close_serial();
    
    return 0;
}