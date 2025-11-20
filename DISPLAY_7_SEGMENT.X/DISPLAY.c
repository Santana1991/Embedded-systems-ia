/*
 * File:   DISPLAY.c
 * Author: SANTA
 *
 * Created on 19 de noviembre de 2025, 07:26 PM
 */
#include <xc.h>
#define _XTAL_FREQ 4000000


#pragma config FOSC = HS  
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF


const unsigned char tabla[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void main(void){
    // Configuración pines
    TRISB = 0x00;       // PORTB como salida (display)
    PORTB = 0x00;
    TRISDbits.TRISD0 = 1; // RD0 entrada para pulsador

    unsigned char contador = 0;
    PORTB = tabla[contador];

    while(1){
        if(PORTDbits.RD0 == 0){     // Pulsador presionado (porque usamos pull-up externo)
            __delay_ms(20);         // Antirrebote por software
            if(PORTDbits.RD0 == 0){
                contador++;
                if(contador > 9) contador = 0;
                PORTB = tabla[contador];
                while(PORTDbits.RD0 == 0); // Espera a soltar
                __delay_ms(20); // Antirrebote al soltar
            }
        }
    }
}
