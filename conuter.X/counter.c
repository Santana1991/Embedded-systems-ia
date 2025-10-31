/*
 * File:   counter.c
 * Author: SANTA
 *
 * Created on 30 de octubre de 2025, 06:25 AM
 */

#define _XTAL_FREQ 20000000  // Frecuencia del cristal (ajústala a tu caso real)
#include <xc.h>

unsigned char counter = 0;
unsigned char NUMEROS [10] ={
    0x3F;
    0x06;
    0x5B;
    0x4F;
    0x66;
    0x6D;
    0x7D;
    0x07;
    0x7F;
    0x6F;
    0x00;
};

void main(void) {
    TRISA = 0x01;   // RA0 como entrada
    TRISB = 0x00;   // Puerto B como salida
    PORTB = 0x3F;   // Mostrar "0" inicialmente
    
    while(1) {
        unsigned char button = PORTAbits.RA0;
        
        if(button == 1) {
            __delay_ms(200);   // Antirrebote simple
            counter++;
            
            if(counter == 10) {
                counter = 0;
            }
            
            switch(counter) {
                case 0: PORTB = 0x3F; 
                  break; // 0
                case 1: PORTB = 0x06; 
                  break; // 1
                case 2: PORTB = 0x5B;
                  break; // 2
                case 3: PORTB = 0x4F;
                  break; // 3
                case 4: PORTB = 0x66; 
                  break; // 4
                case 5: PORTB = 0x6D; 
                  break; // 5
                case 6: PORTB = 0x7D; 
                  break; // 6
                case 7: PORTB = 0x07; 
                  break; // 7
                case 8: PORTB = 0x7F; 
                  break; // 8
                case 9: PORTB = 0x6F; 
                  break; // 9
                default: PORTB = 0x00; 
                  break;
            }
            
            // Esperar a que se suelte el botón
            while(PORTAbits.RA0 == 1);
        }
    }
}
