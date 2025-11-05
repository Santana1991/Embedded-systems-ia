/*
 * File:   counter.c
 * Author: SANTA
 *
 * Created on 30 de octubre de 2025, 06:25 AM
 */

#define _XTAL_FREQ 20000000  
#include <xc.h>
#define DEBOUNCE_MS 20
unsigned char counter = 0;
 const unsigned char DISPLAY [10] ={
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D;
    0x7D,
    0x07,
    0x7F,
    0x6F,
    0x00,
};
unsigned char get_input_debounced(void){
    unsigned char a = PORTAbits.RA0;
__delay_ms(debounce_ms);
unsigned char b = PORTAbits.RA0;
if(a==b){ // Estabilidad en el pin 
    return b;
}else{
    return 0xff; //insetabilidad en el pin
};
}
void main(void) {
    TRISA = 0x01;  //boton
    TRISB = 0x00;  //display
    PORTB = DISPLAY[0];   
    
    unsigned char counter = 0;
   unsigned char prev = 0;
   
    while(1) {
        unsigned char stable = get_input_debounced();
        
        if (stable!=0xff){
            
            if(prev == 0 && stable == 1){
                counter++; //acumulador
                PORTB = DISPLAY[counter];
            }
            prev = stable;
            if(counter==10){
                counter=0;
                PORTB = DISPLAY[0];
            }
        }
       
        
    }
}