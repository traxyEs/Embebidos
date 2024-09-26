#define F_CPU 16000000  //frecuencias del Atmega328p
// Librerias
#include <avr/io.h>
#include "util/delay.h"
// decodificador para un display de 7 segmentos
const  uint8_t buff_7seg[10] = {
    0x3F,   //0b0011 1111    0
    0x06,   //0b0000 0110    1
    0x5B,   //0b0101 1011    2
    0x4F,   //0b0100 1111    3
    0x66,   //0b0110 0110    4
    0x6D,   //0b0110 1101    5
    0x7D,   //0b0111 1101    6
    0x07,   //0b0000 0111    7
    0x7F,   //0b0111 1111    8
    0x6F    //0b0110 1111    9
};
//funcion con la configuracion de puertos
void fun_puertos() {
    DDRD = 0xFF;    // 1111 1111 PD0-PD7 configurados como salida (8 leds)
    DDRB |= 0x3F;   // xx11 1111 PB0-PB5 configurados como salida (6 bit Disp_7seg)
    DDRC |= 0x01;   // xxxx xxx1 PC0 configurado como salida (1 bit Disp_7seg)
}

// Funcion para mostrar un numero en un display de 7 segmentos
void fun_7seg(uint8_t numero) {
    if (numero > 9) return;    // Verifica que el numero este entre 0 y 9
    uint8_t out_deco = buff_7seg[numero];   // numero decodificado
    PORTB = out_deco & 0x3F;    // Los 6 primeros bits (a-f) se envían al puerto B
    // El bit más significativo (g) se envía al pin C0
    if (out_deco & 0x40) {  // Verifica si el bit 6 de un elemento del arreglo buff_7seg es 1
        PORTC |= (1 << 0);  // Establece el bit C0 en 1
    } else {
        PORTC &= ~(1 << 0); // Establece el bit C0 en 0
    }
}

//Main
int main(void) {
    fun_puertos();  // Se llama a la funcion que contiene la configuracion de puertos 
    uint8_t i = 0;  // Varible de conteo ascendente de los leds
    uint8_t j = 6;  // Variable de conteo descendente de los leds
    uint8_t cont_1 = 0; // Variable de conteo ascendente del Display de 7 segmentos
    // bucle principal
    while (1) {
        // Recorrido descendente de los leds 
        if(i > 7){      // Si el contador ascendente llego al final i > 7
            if(j == 0){ // SI en contador descendente llego al final j = 0
                i = 0;  // Reiniciar el contador ascendente
                j = 6;  // Reiniciar el contador descendente
            }
            else{
                PORTD = (1 << j);   // Salida de los led en funcion de j
                _delay_ms(200);     // Retraso para su visualizacion
                j--;                // Reducir en 1 el valor de contador j
            }            
        }
        // Recorrido ascendente de los leds
        else{
            PORTD = (1 << i);   // Salida de los led en funcion de i
            _delay_ms(200);     // Retraso para su visualizacion
            i++;                // Incrementar en 1 el valor de contador i
        }
        // Recorrido del contador en el display de 7 segmentos 
        fun_7seg(cont_1);   // Llamar a la funcion que decodifica los numeros
        cont_1++;           // Incrementar en 1 el valor de contador cont_1
        if(cont_1 > 9){     // Si el contador cont_1 alcaza su valor maximo cont_1 > 9
            cont_1 = 0;     // Reiniciar el contador cont_1 = 0
        }
    }
}
