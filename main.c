/*
 * File:   main.c
 * Author: Oliver
 *
 * Created on 12 August 2023, 16:31
 */


#include <xc.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

void putch(char data) {
    while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
        continue;
    TXREG = data;                     // send one character
}
void init_uart(void) {
    TXSTAbits.TXEN = 1;               // enable transmitter
    RCSTAbits.SPEN = 1;               // enable serial port
}


enum dvar {
  dvnumber   =1,       // 2 byte value that will be stored to eeprom
  dvenum     =7,       // 1 byte value stored to eeprom
};


uint8_t   v_timer_enable @0x0a;
uint8_t volatile flag @0x0b;
uint16_t  v_timer_value;
uint8_t   v_timer_range;


typedef uint8_t  *byteptr;
typedef uint16_t *wordptr;
typedef int16_t *intptr;



typedef struct {
  char text[11];
  void * ptr;
  uint16_t maxvalue;
  enum dvar type;
  uint16_t defvalue;
} optionstruct;
// by making optioncount a typed constant it can be defined in a module
// and shared using extern with the important caveat that "sizeof" appears to
// only be valid after the array is initialised
const optionstruct opts[]= {
// Menu option table
// Name      Variable name limits           param type  default value
//                                                          flags
"Timer    :",&v_timer_enable,0x0002 , dvenum,      0,
"Time Set :",&v_timer_value, 100    , dvnumber,   50,
"Time Range",&v_timer_range, 0x0a02 , dvenum,      0,
};

const uint8_t optioncount=sizeof(opts) / sizeof(optionstruct); //45


void main(void) {
    uint8_t aa;
    init_uart();
    printf("\n");
    printf("XC8 compiler version %.2f\n", __XC8_VERSION / 1000.0f);

    printf("Setting flag\n");
    flag=1;
    printf("Flag is %i\n",flag);
    printf("Running suspect code\n");  
    for( aa=0; aa < optioncount; aa++) {
      switch (opts[aa].type) {
          case dvnumber:
          *((wordptr)opts[aa].ptr) = opts[aa].defvalue;
          break;
          case dvenum:
          *((byteptr)opts[aa].ptr) = opts[aa].defvalue;
          break;

    };
  };
    printf("Flag is %i\n",flag);

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

    _delay(1000);
    while(1);
    return;
}
