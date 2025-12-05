/*
 * File:   cotrol-led.c
 * Author: SANTA
 *
 * Created on 19 de noviembre de 2025, 12:18 PM
 */


#include "PIC16F877ACONFIG.H"
#include <xc.h>
#include <stdio.h>
#define  __XTAL_FREQ 8000000


void main(void) {
    TRISC =0b00000000; 
    
    
    PORTCbits.RC0 = 0;// el led inicia apagado porque el pin  tiene un cero
    
  
}
    
