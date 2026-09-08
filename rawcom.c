#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/serial.h"

int main(int argc, char *argv[]) {
    char dev[128];
    int baud = 9600;
    int pin_flag = 0;
    if(argc < 2){
        printf("use: %s <port> [baud] [-pins]\n", argv[0]);
        printf("if windows: %s COM3 115200\n", argv[0]);
        printf("if linux: %s /dev/ttyUSB0 9600 -pins\n", argv[0]);
        return 1;}
    strcpy(dev, argv[1]);

    if(argc >= 3){
        if(strcmp(argv[2], "-pins")==0){
            pin_flag = 1;
        } else {
            baud = atoi(argv[2]);
        }
    }

    if(argc >= 4){
        if(strcmp(argv[3], "-pins")==0){
            pin_flag = 1;
        }
    }
    printf("opening port %s at %d...\n", dev, baud);
    port_t p = my_open_port(dev);
    if(p == BAD_PORT){
        printf("error: cant open %s\n", dev);
        return 1;
    }

    if(!set_baud(p, baud)){
        printf("error: cant setup baud\n");
        my_close_port(p);
        return 1;
    }
    //printf("port ready!\n");

    if(pin_flag){
        printf("pins status:\n");
        check_pins(p);
    }

    printf("listening (ctrl+c to stop)...\n");
    char temp[512];
    while(1){
        memset(temp, 0, sizeof(temp));
        int n = my_read_data(p, temp, sizeof(temp) - 1);
        if (n > 0) {
            temp[n] = '\0';
            printf("%s", temp);
            fflush(stdout);
        }
    }

    my_close_port(p);
    return 0;
}