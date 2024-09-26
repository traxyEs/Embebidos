#define F_CPU 16000000  //frecuencias del Atmega328p
// Librerias
#include <avr/io.h>
#include "util/delay.h"

void fun_puertos() {
    DDRB |= 0x2A;   // xx1x 1x1x PB1,PB3,PB5 configurados como salida (leds)
    DDRB &= ~(1 << PB0); // xx1x 1x10 PB0 como entrada (boton)
}

//Main
int main(void) {
    fun_puertos();  // Se llama a la funcion que contiene la configuracion de puertos
    while (1) {
        if (PINB & (1 << PB0)){     // Verificar si se pulso el boton (pull up)   
            PORTB &= 0xD5;          // Salida de los leds 000
        }
        else{
             PORTB |= 0x2A;         // Salida de los leds 111
        }
    }
}


