/*
 * File:   main.c
 * Author: Oliver
 *
 * Created on 12 August 2023, 16:31
 */


#include <xc.h>
#include "fpaoptions.h"
#include <stdio.h>

void putch(char data) {
    while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
        continue;
    TXREG = data;                     // send one character
}
void init_uart(void) {
    TXSTAbits.TXEN = 1;               // enable transmitter
    RCSTAbits.SPEN = 1;               // enable serial port
}


void main(void) {
    init_uart();
    printf("\n");
    printf("XC8 compiler version %.2f\n", __XC8_VERSION / 1000.0f);

    printf("Setting flag\n");
    flag=1;
    printf("Flag is %i\n",flag);
    printf("calling \"default_config()\"\n");  
    default_config();
    printf("Flag is %i\n",flag);
/*
    printf("Setting flag\n");
    flag=1;
    printf("Trying a char pointer write\n");
    volatile unsigned char * ptr;
    ptr=&v_timer_enable;
    *ptr=1;
    printf("Flag is %i\n",flag);
    flag=1;
    printf("Trying a void pointer write\n");
    volatile void * vptr;
    vptr=&v_timer_enable;
    *(byteptr)vptr=1;
    printf("Flag is %i\n",flag);
*/
    _delay(1000);
    while(1);
    return;
}
