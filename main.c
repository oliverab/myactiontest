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

// enumerated type strings
// these replace simple on-off options with a range of named options

#define enumcount  39
#define enumlength 11 //remember +1 length for a string

extern const char enumtext[enumcount][enumlength];

// dvenum entries are a more advanced substitute for 
// on/off options. 
// since the same field also stores integers these will be stored as "int"
// not a dedicated "enum" type.
// selections are picked from a list using low byte as the option count
// and high byte as the offset into the string table
// note since space is not a problem this may simply be split up into an
// extra field for the offset
// Special constants, first digit is offset into table and
// second digit is number of options minus one
#define enumonoff 0x0001
#define enumextend 0x0002
#define enumfeature 0x0301
#define enumdetfeat 0x0302
#define enumbright 0x0603
#define enumtimeunits 0x0a02
#define enumshowhide 0x0d01
#define enumdecimals 0x0f07
#ifdef _USE_MCC
#define enumpulsrc 0x1707
#else
#define enumpulsrc 0x1706
#endif
// enumpulsrc should be 0x1507, increased by 3 for test purposes
//#define enumpulsrc 0x150a
#define enumalarmout 0x1f03
#define enumstepsize 0x2303
// Action values for "immed" (act on key press) entries
// Action values should be split out into another field so that 
// number and option fields can trigger an "action"
#define immed_return 0
#define immed_store_user 1
#define immed_store_sys 2

enum dvar {
  dvimmediate=0,
  dvnumber   =1,       // 2 byte value that will be stored to eeprom
  dvenum     =7,       // 1 byte value stored to eeprom
  dvstop    =13  // end of menu indicator record     
};


uint16_t  v_forward;
uint16_t  v_reflected;
uint8_t   v_timer_enable @0x0a;
uint8_t volatile flag @0x0b;
uint16_t  v_timer_value;
uint8_t   v_timer_range;


typedef uint8_t  *byteptr;
typedef uint16_t *wordptr;
typedef int16_t *intptr;

/*
typedef struct {
int number;
int *ptr;
} record_t;
 
int i;
const struct {
int number;
int *ptr;
} record = { 0x55, &i };

struct { char x;
         char y; } example;
*/


typedef struct {
  char text[11];
  void * ptr;
  uint16_t maxvalue;
  enum dvar type;
  uint16_t defvalue;
  uint8_t flagbit;
  uint8_t hidebit; // non zero value means hide
} optionstruct;
// by making optioncount a typed constant it can be defined in a module
// and shared using extern with the important caveat that "sizeof" appears to
// only be valid after the array is initialised
const optionstruct opts[]= {
// Menu option table
// Name      Variable name limits           param type  default value
//                                                          flags
"Main view ",&v_forward,   immed_return,    dvimmediate,0,  0,  0,
"Store CFG ",&v_reflected, immed_store_user,dvimmediate,0,  0,  0,
"Timer    :",&v_timer_enable,enumextend,    dvenum,      0, 0,  0,
"Time Set :",&v_timer_value, 100,           dvnumber,   50, 0,  0,
"Time Range",&v_timer_range, enumtimeunits, dvenum,      0, 0,  0,
""          ,NULL,         0,               dvstop,      0, 0,  0 //always stop
};

const uint8_t optioncount=sizeof(opts) / sizeof(optionstruct); //45

//size of the opts array fixed in header as attempting to infer it using 
//sizeof does not work for externals and would break serial

//2 test lines added
//#define optioncount 45
//optioncount is initialised using sizeof in fpaoptions.c
extern const uint8_t optioncount;


//size of the opts array using sizeof (broken))
//#define optioncount (sizeof(opts)/sizeof(optionstruct)) 
//number of "system" entries
#define optionsystem 0
//number of "user" entries also start of system
#define optionuser (optioncount-optionsystem)
//location of the power parameter
#define optionpower 6
//location of the system parameters in EE
#define sys_ee_start 0x40



void default_config(void) {
  uint8_t aa;
  for( aa=0; aa < optionuser; aa++) {
    switch (opts[aa].type) {
        case dvnumber:
        *((wordptr)opts[aa].ptr) = opts[aa].defvalue;
        break;
        case dvenum:
        *((byteptr)opts[aa].ptr) = opts[aa].defvalue;
        break;

    };
  };
};




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
