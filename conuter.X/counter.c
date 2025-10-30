/*
 * File:   counter.c
 * Author: SANTA
 *
 * Created on 30 de octubre de 2025, 06:25 AM
 */

#define XTAL_FREQ 200000000
#include <xc.h>
unsigned char counetr = 0;

void main(void) {
 
    TRISA = 0x01;
    TRISB = 0X00
    PORTB = 0x3f;
    
    while(1){
      
        unsigned char button = PORTAbits.RA0;
        
        if(button == 1){
            counter++;
        }
        
        if(counter ==10){
           counter =0;
        }
        
        switch (counter)
                
                case 0:
                    PORTB = 0x3f;
                    break;
                    
               case 1:
                 PORTB = 0X06;
                 break;
                    
               case 2:
                   
                            
                    
        
        
    }
    return;
}
