/*
 * File:   fpaoptions.c
 * Author: Oliver
 *
 * Created on 11 April 2015, 09:12
 */

#include "fpaoptions.h"

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
""          ,NULL,         0,               dvstop,      0, 0,  hb_unimp //always stop
};
const uint8_t optioncount=sizeof(opts) / sizeof(optionstruct); //45

void default_config(void) {
  uint8_t aa;
  for( aa=0; aa < optionuser; aa++) {
    switch (opts[aa].type) {
        case dvnumber:
        case dvrealtime:
        case dvpassword:
        case dvsigned:
        *((wordptr)opts[aa].ptr) = opts[aa].defvalue;
        break;
        case dvenum:
        *((byteptr)opts[aa].ptr) = opts[aa].defvalue;
        break;
        case dvbitfield:
            if (opts[aa].maxvalue & 0xff00)
                // if maxvalue is 16 bit perform a 16 bit initialise otherwise
                // assume 8 bit
                // in the case of bitfield maxvalue is a bitfield indicating which
                // bits are in use
                *((wordptr)opts[aa].ptr) = opts[aa].defvalue;
            else
                *((byteptr)opts[aa].ptr) = opts[aa].defvalue;
        break;

    };
    flagbits|=opts[aa].flagbit;
  };
};

void default_config_sys(void) {
  uint8_t aa;
  for( aa=optionuser; aa < optioncount; aa++) {
    switch (opts[aa].type) {
        case dvnumber:
        case dvpassword:
        case dvsigned:
        *((wordptr)opts[aa].ptr) = opts[aa].defvalue;
        break;
        case dvenum:
        *((byteptr)opts[aa].ptr) = opts[aa].defvalue;
        break;
    };
    flagbits|=opts[aa].flagbit;
  };
};

