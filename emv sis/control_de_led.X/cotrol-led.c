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
    TRISC =0b00000001; //configura el puerto c como salida excepto el primer pin
    TRISD =0b00000000; // configura todos los pines D como salida
    
    PORTDbits.RD0 = 0;// el led inicia apagado porque el pin  tiene un cero
    
    while(1){
        if(PORTCbits.RC0 == 1){
            PORTDbits.RD0 = 1; // El led se enciende al mandarle un 1
        }
        else{
          PORTDbits.RD0 = 0;  // el led se vuelve a apagar
                              //cuando se deje de presionar el boton
                              // osea se le mande un cero
        }    
    }
}
    
