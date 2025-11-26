/*
 * File:   newmain.c
 * Author: SANTA
 *
 * Created on 25 de noviembre de 2025, 08:55 PM
 */

#define _XTAL_FREQ 20000000
#include <xc.h>
#include <stdio.h>
#include <string.h>


void adc_init(void){
    ADCON1 = 0b10000100;   // RA2 analógica, voltajes de referencia por defecto
    ADCON0 = 0b01000001;   // ADC encendido, canal 0 por defecto
}

unsigned int adc_read(unsigned char channel){
    ADCON0 &= 0b11000111;          // Limpia selección de canal
    ADCON0 |= (channel << 3);      // Selecciona canal
    __delay_us(20);                // Tiempo de adquisición

    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);  
    return ((ADRESH << 8) + ADRESL);
}


void pwm_init(void){
    TRISCbits.TRISC2 = 0;          // CCP1 como salida

    CCP1CON = 0b00001100;          // Modo PWM

    PR2 = 249;                     // Para 20 kHz
    T2CON = 0b00000100;            // Timer2 ON, prescaler 1:1
}

void set_pwm_from_adc(unsigned int adc_value){
    unsigned int max_counts = 4 * (PR2 + 1); 
    unsigned int dc = ((unsigned long)adc_value * max_counts) / 1023;

    CCPR1L = dc >> 2;
    CCP1CONbits.CCP1Y = dc & 1;
    CCP1CONbits.CCP1X = (dc >> 1) & 1;
}


void main(){
    TRISB = 0xFF;           // RB0 y RB1 entradas
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 1;   // Potenciómetro

    pwm_init();
    adc_init();

    unsigned int pot_value = 0;

    while(1){

        // Control de giro
        if(PORTBbits.RB0){
            PORTA = 0b00000001;   // Derecha
        }
        else if(PORTBbits.RB1){
            PORTA = 0b00000010;   // Izquierda
        }

        // Control de velocidad
        pot_value = adc_read(2);
        set_pwm_from_adc(pot_value);

        __delay_ms(10);
    }
}
