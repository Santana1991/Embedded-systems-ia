/*
 * File:   counter.c
 * Author: SANTA
 *
 * Created on 21 de noviembre de 2025, 11:37 AM
 */



#define _XTAL_FREQ 20000000  // Frecuencia del cristal a 20 MHz

#include <xc.h>


#pragma config FOSC = HS        
#pragma config WDTE = OFF       
#pragma config PWRTE = ON       
#pragma config BOREN = ON       
#pragma config LVP = OFF        
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF


const unsigned char NUMBERS[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

void main(void) {

  
    ADCON1 = 0x06;   // Desactivar entradas analógicas (RA0 digital)
    CMCON  = 0x07;   // Apagar comparadores (SimulIDE lo necesita)

    TRISA = 0x01;    // RA0 como entrada (botón)
    TRISB = 0x00;    // Puerto B como salida (display)

    PORTB = NUMBERS[0];  // Mostrar 0 al inicio

    unsigned char counter = 0;

    while(1) {

        if(PORTAbits.RA0 == 1) {   // Detectar pulsación

            __delay_ms(10);        // Antirrebote

            if(PORTAbits.RA0 == 1) {  // Confirmar que sigue presionado

                // Incrementar contador
                counter++;
                if(counter >= 10)
                    counter = 0;

                // Mostrar número en display
                PORTB = NUMBERS[counter];

                // Esperar que se suelte el botón
                while(PORTAbits.RA0 == 1);
            }
        }
    }
}
