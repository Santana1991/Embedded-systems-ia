

#include <avr/io.h>
#include <util/delay.h>

// Segmentos a-g conectados a PORTD (pines 2-8 de Arduino)
// Pin 2 (PD2) = segmento a
// Pin 3 (PD3) = segmento b
// Pin 4 (PD4) = segmento c
// Pin 5 (PD5) = segmento d
// Pin 6 (PD6) = segmento e
// Pin 7 (PD7) = segmento f
// Pin 8 (PB0) = segmento g

// Pin 9 (PB1) = Pulsador

// Variables
volatile uint8_t contador = 0;
volatile uint8_t estadoBotonAnterior = 0;

// Los bits representan: g-f-e-d-c-b-a (bit 6 al bit 0)
const uint8_t tablaNumeros[10] = {
  0b00111111,  // 0: a,b,c,d,e,f encendidos
  0b00000110,  // 1: b,c encendidos
  0b01011011,  // 2: a,b,d,e,g encendidos
  0b01001111,  // 3: a,b,c,d,g encendidos
  0b01100110,  // 4: b,c,f,g encendidos
  0b01101101,  // 5: a,c,d,f,g encendidos
  0b01111101,  // 6: a,c,d,e,f,g encendidos
  0b00000111,  // 7: a,b,c encendidos
  0b01111111,  // 8: todos encendidos
  0b01101111   // 9: a,b,c,d,f,g encendidos
};

//  inicialización
void setup() {
  // PORTD pines 2-7 (PD2-PD7) como salida
  DDRD |= 0b11111100;  // Bits 7,6,5,4,3,2 como salida
  
  DDRB |= (1 << DDB0);   // PB0 como salida (segmento g)
  DDRB &= ~(1 << DDB1);  // PB1 como entrada (botón)
  
  PORTB |= (1 << PORTB1);
  
  // Inicializar el display en 0
  mostrarNumero(contador);
  
  // Inicializar UART para debug
  inicializarUART();
  transmitirCadena("Sistema Iniciado\r\n");
}

//principal
void loop() {
  // Con pull-up: botón presionado = LOW, botón suelto = HIGH
  uint8_t estadoBotonActual = (PINB & (1 << PINB1)) ? 0 : 1;
  
  // Detectar flanco de subida (transición de no presionado a presionado)
  if (estadoBotonActual && !estadoBotonAnterior) {
    // Antirrebote por software
    _delay_ms(50);
    
    // Verificar nuevamente el estado del botón
    estadoBotonActual = (PINB & (1 << PINB1)) ? 0 : 1;
    
    if (estadoBotonActual) {
      // Incrementar el contador
      contador++;
      
      // Reiniciar a 0 después de 9
      if (contador > 9) {
        contador = 0;
      }
      
      // Actualizar el display
      mostrarNumero(contador);
      
      // Debug UART
      transmitirCadena("Contador: ");
      transmitirNumero(contador);
      transmitirCadena("\r\n");
      
      // Esperar a que se suelte el botón
      while ((PINB & (1 << PINB1)) == 0) {
        _delay_ms(10);
      }
      _delay_ms(50); // Antirrebote al soltar
    }
  }
  
  // Actualizar el estado anterior del botón
  estadoBotonAnterior = estadoBotonActual;
  
  _delay_ms(10); // Pequeño delay para estabilidad
}

//mostrar un número en el display
void mostrarNumero(uint8_t num) {
  if (num > 9) {
    num = 0;
  }
  
  uint8_t patron = tablaNumeros[num];
  
  // Extraer bits individuales y escribir en los puertos
  // Segmentos a-f están en PORTD (bits 2-7)
  // Primero limpiamos los bits relevantes de PORTD (bits 2-7)
  PORTD &= 0b00000011;
  
  // Luego escribimos los nuevos valores
  // Desplazamos el patrón 2 posiciones a la izquierda porque 
  // los segmentos empiezan en PD2
  PORTD |= ((patron & 0b00111111) << 2);
  
  // Segmento g está en PORTB bit 0
  if (patron & 0b01000000) {
    PORTB |= (1 << PORTB0);  // Encender segmento g
  } else {
    PORTB &= ~(1 << PORTB0); // Apagar segmento g
  }
}

// Función para inicializar UART
void inicializarUART() {
  
  uint16_t ubrr = 103; // Para 16MHz y 9600 baud
  UBRR0H = (uint8_t)(ubrr >> 8);
  UBRR0L = (uint8_t)ubrr;
  
  // Habilitar transmisor
  UCSR0B = (1 << TXEN0);
  
  // Configurar formato: 8 bits de datos, 1 bit de stop
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Función para transmitir un byte por UART
void transmitirByte(uint8_t dato) {
  // Esperar a que el buffer esté vacío
  while (!(UCSR0A & (1 << UDRE0)));
  
  // Escribir el dato en el buffer
  UDR0 = dato;
}

// Función para transmitir una cadena por UART
void transmitirCadena(const char* cadena) {
  while (*cadena) {
    transmitirByte(*cadena++);
  }
}

// Función para transmitir un número (0-9) por UART
void transmitirNumero(uint8_t num) {
  transmitirByte('0' + num);
}

// Punto de entrada del programa
int main(void) {
  setup();
  
  while (1) {
    loop();
  }
  
  return 0;
}
