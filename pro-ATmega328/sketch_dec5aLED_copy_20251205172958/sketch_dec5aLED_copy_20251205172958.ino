#include <avr/io.h>
#include <util/delay.h>

// Definiciones
#define LED_PIN     PB5    // Pin del LED (Puerto B, bit 5) = Arduino D13
#define BUTTON_PIN  PD2    // Pin del pulsador (Puerto D, bit 2) = Arduino D2

// Variables
volatile uint8_t estado_led = 0;        // Estado actual del LED (0=apagado, 1=encendido)
volatile uint8_t boton_anterior = 0;    // Estado anterior del botón

// Función
void init_ports(void) {
    // DDRB: Data Direction Register B
    // 1 = salida, 0 = entrada
    DDRB |= (1 << LED_PIN);    // Establecer bit 5 como salida
    
    // Configurar PD2 como entrada
    DDRD &= ~(1 << BUTTON_PIN); // Limpiar bit 2 (entrada)
    
    // Inicializar LED apagado
    PORTB &= ~(1 << LED_PIN);   // Limpiar bit 5 de PORTB
}

//leer el estado del pulsador
uint8_t leer_boton(void) {
    // PIND: Port Input Register   D
    // Lee el estado actual del pin PD2
    return (PIND & (1 << BUTTON_PIN)) ? 1 : 0;
}

//encender el LED
void led_on(void) {
    PORTB |= (1 << LED_PIN);    // Establecer bit 5 de PORTB
}

//apagar el LED
void led_off(void) {
    PORTB &= ~(1 << LED_PIN);   // Limpiar bit 5 de PORTB
}

// alternar el LED
void toggle_led(void) {
    PORTB ^= (1 << LED_PIN);    // XOR para alternar el bit 5
}

//detección de flanco ascendente
void detectar_pulso(void) {
    uint8_t boton_actual = leer_boton();
    
    // Detectar flanco ascendente (de 0 a 1)
    if (boton_actual == 1 && boton_anterior == 0) {
        // Se detectó una pulsación nueva
        _delay_ms(50);  // Antirrebote
        
        // Verificar que el botón sigue presionado
        if (leer_boton() == 1) {
            // Alternar estado del LED
            estado_led = !estado_led;
            
            if (estado_led) {
                led_on();
            } else {
                led_off();
            }
            
            // Esperar a que se suelte el botón
            while (leer_boton() == 1) {
                _delay_ms(10);
            }
            _delay_ms(50);  // Antirrebote al soltar
        }
    }
    
    boton_anterior = boton_actual;
}

void setup(void) {
    // Inicializar los puertos
    init_ports();
}

void loop(void) {
    // Detectar y procesar pulsaciones
    detectar_pulso();
    
    // Pequeño retardo para estabilidad
    _delay_ms(10);
}