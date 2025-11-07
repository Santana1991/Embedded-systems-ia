/*
 * File:   ADC-1.c
 * Author: SANTA
 *
 * Created on 7 de noviembre de 2025, 10:50 AM
 */

#include <xc.h>
#define _XTAL_FREQ 20000000 //20MHz
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define RS PORTBbits.RB6
#define EN PORTBbits.RB7

void lcdCommand(unsigned char cmd){
    RS = 0;
    PORTD = cmd;
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);
    
}

void lcdCommand(unsigned char data){
    RS = 0;
    PORTD = data;
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);
    
}
void lcd_init(){
    lcdCommand(0x38); //configura el display: 2 lineas matris x5x7
    lcdCommand(0x06); //muve el cursos a la derecha automaticamente 
    lcdCommand(0x06); //display on y blinking
    lcdCommand(0x01); //borra pantalla 
    lcdCommand(0x80); //muve el cursor a la primera linea 
    __delay_ms(1);
 
}
void adc_init(void){
    TRISA = 0b00000001; //pin AN0
    
    ADCON1 = 0b11001110;
/*/    
     bit 0 ADCON = 1 - enciende el ADC
     bit 2 godone = 0
     bit 3-5  canal 0
     bit 6-7 110
     */
    ADCON0 = 0b10000001;
    
    __delay_us(20);
}
void lcdSendString(char *addr){
    while(*addr){
        lcSedDataByted(*addr);
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
    trisb = 0b00000000;
    
    adc_init();
    acd_init();
    char buffer[16];
    uint16_t  prev =0xff;
    while (1){
        uint16_t raw = adc_read_an0();
        uint16_t mV = (uint16_t)raw*500L/1023U;
        
        if(mV!=prev_mv){
            lcdCommand(0x80);
            sprintf(buffer,"RAW:%4u",raw);
            lcdSendString(buffer);
            
            prev_mv = mV;
        }
        __delay_ms(100);
    }
}