
#define RS_PIN 0  // PB0
#define E_PIN  1  // PB1

// Variables globales
volatile uint16_t adcValue = 0;
volatile bool adcReady = false;

void setup() {
  
  DDRD = 0xFF;  // PORTD como salida (datos LCD D0-D7)
  DDRB |= (1 << RS_PIN) | (1 << E_PIN);  // PB0 y PB1 como salida
  
  // Inicializar LCD
  delay(50);
  lcdInit();
  
 
  ADMUX = (1 << REFS0);  // AVCC como referencia
  
  
  ADCSRA = (1 << ADEN) |   // Habilitar ADC
           (1 << ADIE) |   
           (1 << ADPS2) |  // Prescaler 128
           (1 << ADPS1) | 
           (1 << ADPS0);
  
  
  sei();
  
  
  ADCSRA |= (1 << ADSC);
}

void loop() {
  if (adcReady) {
    adcReady = false;
    
    
    uint32_t millivolts = ((uint32_t)adcValue * 5000UL) / 1023UL;
    
    
    lcdClear();
    
    
    lcdSetCursor(0, 0);
    lcdPrint("RAW: ");
    lcdPrintNumber(adcValue);
    
    
    lcdSetCursor(0, 1);
    lcdPrint("mV: ");
    lcdPrintNumber(millivolts);
    
    
    delay(200);
    
    // Iniciar nueva conversión
    ADCSRA |= (1 << ADSC);
  }
}


ISR(ADC_vect) {
  adcValue = ADC; )
  adcReady = true;
}


void lcdPulseEnable() {
  PORTB |= (1 << E_PIN);   // E = 1
  delayMicroseconds(1);
  PORTB &= ~(1 << E_PIN);  // E = 0
  delayMicroseconds(50);
}

void lcdSendCommand(uint8_t cmd) (
  PORTB &= ~(1 << RS_PIN);  // RS = 0 
  PORTD = cmd;              
  lcdPulseEnable();
  delayMicroseconds(2000);
}

void lcdSendData(uint8_t data) {
  PORTB |= (1 << RS_PIN);   // RS = 1
  PORTD = data;             // Enviar dato
  lcdPulseEnable();
  delayMicroseconds(50);
}

void lcdInit() {
  
  delay(15);
  lcdSendCommand(0x38);  // ......l....
  delay(5);
  lcdSendCommand(0x38);
  delay(1);
  lcdSendCommand(0x38);
  
  lcdSendCommand(0x0C);  // Display ON
  lcdSendCommand(0x01);  // ....
  delay(2);
  lcdSendCommand(0x06);  // ,,,
}

void lcdClear() {
  lcdSendCommand(0x01);
  delay(2);
}

void lcdSetCursor(uint8_t col, uint8_t row) {
  uint8_t address = (row == 0) ? 0x00 : 0x40;
  address += col;
  lcdSendCommand(0x80 | address);
}

void lcdPrint(const char* str) {
  while (*str) {
    lcdSendData(*str++);
  }
}

void lcdPrintNumber(uint32_t num) {
  char buffer[11];
  ultoa(num, buffer, 10);
  lcdPrint(buffer);
}