/*
 * File:   main.c
 * Author: Oliver
 *
 * Created on 12 August 2023, 16:31
 */


#include <xc.h>
#include "fpaoptions.h"

uint8_t volatile flag;
void main(void) {
    flag=1;
    default_config();

    return;
}
