/*
 * File:   cotrol-led.c
 * Author: SANTA
 *
 * Created on 19 de noviembre de 2025, 12:18 PM
 */


#include "PIC16F877ACONFIG.H"
#include <xc.h>
#define  __XTAL_FREQ 8000000

void main(void) {
    TRISC0 = 1;
    TRISD0 = 0;
    RD0 = 0;
    RC0 = 0;
    while(1)
    {
        if(RC0 ==1)
        {
            RD0 = 1;
        }
        else
        {
          RD0 = 0;  
        }    
    }
    
}
    
