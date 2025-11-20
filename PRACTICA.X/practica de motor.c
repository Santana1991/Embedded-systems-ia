/*
 * File:   practica de motor.c
 * Author: SANTA
 *
 * Created on 13 de noviembre de 2025, 07:24 AM
 */


 
#define _XTAL_FREQ 4000000
#include <xc.h>
/*
 * 
*/

void pwm_init(void){
        while(1){
    TRISC = 0b00000000;
   // Configurar RC2 como salida
    // Configurar PWM en CCP1
    CCP1CON = 0b00001100;  // Modo PWM
    // Frecuencia PWM = 1 kHz
    PR2 = 249;
    // Timer2 prescaler 4, encendido
    T2CON = 0b00000101;
        }
}

void set_percent(unsigned char percent){
    if(percent > 100) percent = 100;

    unsigned int max_counts = (unsigned int)(4 * (PR2 + 1));
    unsigned int dc = (unsigned int)((unsigned long)percent * max_counts / 100UL);

    CCPR1L = (unsigned char)(dc >> 2);
    CCP1CONbits.CCP1X = (dc >> 1) & 1;
    CCP1CONbits.CCP1Y = dc & 1;
}

void main (void){
  TRISB = 0b11111111
  TRISA = 0b00000000
            
    pwm_init();
    
    set_percent(50);  // PWM al 50%
    
    while(1){
        // Aquí puedes meter lógica adicional
    }
}