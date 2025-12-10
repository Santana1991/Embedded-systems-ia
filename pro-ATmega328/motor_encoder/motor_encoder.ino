#define encoder_clk_pin 2 //PD2
#define encoder_DT_pin 3  //PD3
#define encoder_SW_pin 4  //PD4 ==PULSAR ENCODER==

//MOTOR
#define motor_pwm_pin 9  // PB1 - ena
#define motor_IN1_pin 8  // PB0 - in1
#define motor_IN2_pin 7 // PD7 - in2

//variables
int PosicionEncoder = 0;
bool EstadoAnteriorCLK = 0;
bool BotonPresionado = false;

void setup(){
//PINES DEL ENCODER COMO ENTRADAS PULL-UP
  DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4));
  PORTD |= ((1 << PD2) | (1 << PD3) | (1 << PD4));

// PINES DEL MOTOR COMO SALIDAS LOW
  DDRB |=((1 << PB0) | (1 << PB1)); // PIN 8 Y PIN 9 DEL ARDUINO
  DDRD |= (1 << PD7); //PIN 7 DEL ARDUINO
  PORTB &=~ ((1 << PB0) | (1 << PB1));
  PORTD &=~ (1 << PD7);

// CONFUGURAR INTERRUPCIONES PARA EL ENCODER
  EICRA |= (1 << ISC00); //PONER ISC00 EN 1
  EICRA &= ~(1 << ISC01); //PONER ISC01 EN 0
  EIMSK |= (1 << INT0); //HABILITAR INT0
  sei(); //set enable interrupts (habilita interrupciones globales)

  TCCR1A |= (1 << WGM10); //MODO FAST DEL PWM DE 8-BITS
  TCCR1A |= (1 << COM1A1); // MAYOR VALOR = MAS VELOCIDAD

  TCCR1B |= (1 << WGM12); //MODO FAST DEL PWM DE 8-BITS
  TCCR1B |= (1 << CS10); //SIN PREESCALER(MAXIMA VELOCIDAD)

  OCR1A = 0; //MOTOR DETENIDO INICIALMENTE
}

ISR(INT0_vect){
  bool estadoCLK = (PIND &(1 << PD2));
  bool estadoDT = (PIND &(1 << PD3));
/*si CLK y DT son diferentes el giro es derecha
 * si son iguales el giro es izquierda
 */
  if(estadoCLK != estadoDT){
    PosicionEncoder++; //girar derecha
  }
  else {
    PosicionEncoder--; //girar izquierda
  }
}
void loop(){
  int velocidad = abs(PosicionEncoder); //valor absoluto

  //limitar velocidad a 255(max pwm)
  if(velocidad > 255){
    velocidad = 255;
  }
  //establecer direccion
  if(PosicionEncoder > 0){
    PORTB |= (1 << PB0); //IN1 = HIGH
    PORTD &=~ (1 << PD7); //IN2 = LOW
  }
  else if(PosicionEncoder < 0){
    PORTB &=~ (1 << PB0); //IN1 = LOW
    PORTD |=(1 << PD7); //IN2 = HIGH
  }
  else{
    PORTB &=~(1 << PB0); //IN1 = LOW
    PORTD &=~ (1 << PD7); //IN2 = LOW
    velocidad = 0; //motor detenido posicion = 0
    
  }
  OCR1A = velocidad;
}