/*
 * File:   LCD 16x2.c
 * Author: SANTA
 *
 * Created on 25 de noviembre de 2025, 10:23 AM
 */


#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000      // Frecuencia del oscilador (4 MHz)
#define RS PORTBbits.RB6
#define EN PORTBbits.RB7
#include<string.h>
#include<stdlib.h>



void lcdCommand(unsigned char cmd){
    RS = 0;
    PORTD = cmd;
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);
    
}

void lcSendDataByte(unsigned char data){
    RS = 1;
    PORTD = data;
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(0);
    
}

void lcd_init(){
    lcdCommand (0x38);  //configurar el display: dos lineas matrix 5x7
    lcdCommand (0x06);  // mover el cursor a la derecha automaticamente
    lcdCommand (0x0f);  //display on y blinking
    lcdCommand (0x01);  // borrar pantlla
    lcdCommand (0x80);  // mover el cursor a la primera linea
    __delay_ms(1);
}

void adc_init(void){
    TRISA = 0b00000001;
    ADCON1 = 0b11001110;
    ADCON0 = 0b10000001;
    
    __delay_us(20);
}

void lcdSendString(char *addr){
    while (*addr){
        lcSendDataByte(*addr);
        addr++;
        
    }
}

unsigned int adc_read_an0(void){
    __delay_us(20);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((unsigned int)ADRESH<<8|ADRESL);
    
}

void main(void){
    TRISD = 0b00000000;
    TRISB = 0b00000000;
    lcd_init();
    adc_init();
    
    char buffer[16];
    uint16_t prev_mv = 0xff;
    
    
    while (1){
        uint16_t raw = adc_read_an0();
        uint16_t mV = (uint16_t) raw*5000l/1023U;
        
        if(mV!=prev_mv){
            lcdCommand(0x80);
            sprintf(buffer,"RAW:%4u", raw);
            lcdSendString(buffer);
            
            lcdCommand(0xC0);
            sprintf(buffer,"mV:%4u", mV);
            lcdSendString(buffer);
            
            prev_mv = mV;
            
        }
        __delay_ms(20);
    }
}