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
  enum dvar type;
  uint16_t defvalue;
} optionstruct;
const optionstruct opts[]= {
//                                                          flags
"Timer    :",&v_timer_enable, dvenum,      0,
"Time Set :",&v_timer_value,  dvnumber,  500,
"Time Range",&v_timer_range,  dvenum,      0,
};

const uint8_t optioncount=sizeof(opts) / sizeof(optionstruct); //45


void main(void) {
    uint8_t aa;
    init_uart();
    printf("\n");
    printf("XC8 compiler version %.2f\n", __XC8_VERSION / 1000.0f);

    printf("Setting flag\n");
    flag=1;
    v_timer_value=0;
    printf("Flag is %i\n",flag);
    printf("Running suspect code\n");  
    for( aa=0; aa < optioncount; aa++) {
      switch (opts[aa].type) {
          case dvenum:
          *((byteptr)opts[aa].ptr) = opts[aa].defvalue;
          break;
          case dvnumber:
          *((wordptr)opts[aa].ptr) = opts[aa].defvalue;
          break;

    };
  };
    printf("Flag (expect 1) is %i\n",flag);
    printf("Timer value (expect 500) is %i\n",v_timer_value);

    printf("Setting flag\n");
    flag=1;
    v_timer_value=0;
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
    printf("Flag (expect 1) is %i\n",flag);
    printf("Timer value (expect 500) is %i\n",v_timer_value);

    _delay(1000);
    while(1);
    return;
}
