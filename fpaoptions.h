/* 
 * File:   fpaoptions.h
 * Author: Oliver
 *
 * Created on 12 April 2015, 13:33
 */

#ifndef FPAOPTIONS_H
#define	FPAOPTIONS_H


#include <xc.h>
#include <stddef.h>
#include <stdint.h>


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
  dvreadonly =4,       // 2 byte value that is only displayed
  dvpassword =6,       // 2 byte special value
  dvenum     =7,       // 1 byte value stored to eeprom
  dvenumro   =8, // read only version
  dvrealtime =9, // read only number constantly updating, not fully implemented
                 // a "realtime" updates display every cycle regardless of "linechange"
  dvsigned  =10, // a signed 16 bit value, doesn't generate a bargraph
  dvsignedro=11, // as above but read only
  dvbitfield=12, // displays value as bits
  dvstop    =13  // end of menu indicator record     
};


uint16_t  v_forward;
uint16_t  v_reflected;
uint8_t   v_timer_enable @0x0a;
uint8_t volatile flag @0x0b;
uint16_t  v_timer_value;
uint8_t   v_timer_range;

/* Flagbits can be set by an option and indicate that an action is required
 * for example send the adjusted value to a device
 * 
 * by default an option with no special actions will have no flag bit
 */
uint8_t  flagbits;
#define fb_switch 0x01     //indicates a menu option controlling an output
#define fb_pulse  0x02     //indicates a menu option setting the pulse gen
#define fb_p_freq 0x08     //indicates pulse frequency adjusted
#define fb_trim   0x04     //indicates the "trim" option adjusted 

uint8_t  hidebits;
#define hb_unimp  0x80     //always set, allows for permanent hiding
#define hb_oldfp  0x01     //set for legacy front panel
#define hb_nopulse 0x02    //pulse gen not equipped
#define hb_notrim  0x04
#define hb_system  0x08    //set to allow system menu note this bit acts backwards
                           // by hiding the end of menu record
#define hb_noextend  0x10    // hides the experimental overload board
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


void default_config(void);
void default_config_sys(void);
int get_config(void); //boolean
int get_config_sys(void); //boolean
void write_config();
void write_config_sys();


#endif	/* FPAOPTIONS_H */

