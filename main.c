/*
 * File:   main.c
 * Author: Oliver
 *
 * Created on 12 August 2023, 16:31
 */

#include "test_tool.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>



enum dvar {
  dvnumber   =1,       // 2 byte value that will be stored to eeprom
  dvenum     =7,       // 1 byte value stored to eeprom
};

struct teststruct
{
uint8_t   timer_enable;
uint8_t   flag;
    
};
struct teststruct v;

//uint8_t   v_timer_enable;
//uint8_t volatile flag;
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
"Timer    :",&v.timer_enable, dvenum,      0,
"Time Set :",&v_timer_value,  dvnumber,  500,
"Time Range",&v_timer_range,  dvenum,      0,
};

const uint8_t optioncount=sizeof(opts) / sizeof(optionstruct); //45


void main(void) {
    uint8_t aa;
    init_test();
    printf("\n");
    report_compiler();
    printf("Setting flag\n");
    v.flag=1;
    v_timer_value=0;
    printf("Flag is %i\n",v.flag);
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
    printf("Flag (expect 1) is %i\n",v.flag);
    printf("Timer value (expect 500) is %i\n",v_timer_value);

    printf("Setting flag\n");
    v.flag=1;
    v_timer_value=0;
    printf("Flag is %i\n",v.flag);
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
    printf("Flag (expect 1) is %i\n",v.flag);
    printf("Timer value (expect 500) is %i\n",v_timer_value);
    end_test();
    return;
}
