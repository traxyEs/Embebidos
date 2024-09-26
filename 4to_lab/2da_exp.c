#define F_CPU 16000000  //frecuencias del Atmega328p
// Librerias
#include <avr/io.h>
#include "util/delay.h"

#define pulsador PB4
#define interruptores (PINB & 0x0F)  // 0000 xxxx
//funcion con la configuracion de puertos
void fun_puertos() {
    DDRD = 0xFF;    // 1111 1111 PD0-PD7 configurados como salida (8 leds)
    DDRB &= 0xE0;   // xxx0 0000 PB0-PB4 configurados como entrada (0-3 interruptores 4 boton)
}
// Leer el estado del boton
uint8_t leer_pulsador() {
    if ((PINB & (1 << pulsador)) == 0) {        // Boton presionado cuando es 0
        _delay_ms(50);                          // Espera para evitar el rebote
        if ((PINB & (1 << pulsador)) == 0) {    // Verificar si sigue presionado después del retardo
            return 1;                           // Boton presionado
        }
    }
    return 0;  // Boton no presionado
}


//Main
int main(void) {
    fun_puertos();  // Se llama a la funcion que contiene la configuracion de puertos 
    uint8_t cont_1 = 0; // Variable de conteo ascendente del pulsador
    // bucle principal
    while (1) {
        if(leer_pulsador()){    // Verificar si se pulso el boton 
            cont_1++;           // Si se presiono aumentar cont_1 en 1
        }
        uint8_t est_interruptores = interruptores;  // Leer el valor de los interruptores
        PORTD = est_interruptores + cont_1;         // Suma del valor de los interruptores y las pulsaciones del boton
    }
}
