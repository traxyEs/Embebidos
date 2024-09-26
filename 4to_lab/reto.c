#define F_CPU 16000000  //frecuencias del Atmega328p
// Librerias
#include <avr/io.h>
#include "util/delay.h"
// decodificador para un display de 7 segmentos
const  uint8_t buff_7seg[10] = {
    0x3F,   //0b00111111    0
    0x06,   //0b00000110    1
    0x5B,   //0b01011011    2
    0x4F,   //0b01001111    3
    0x66,   //0b01100110    4
    0x6D,   //0b01101101    5
    0x7D,   //0b01111101    6
    0x07,   //0b00000111    7
    0x7F,   //0b01111111    8
    0x6F    //0b01101111    9
};
#define pulsador PC5
#define interruptores (((PINC & 0x1E) >> 1) & 0x0F)  // 0000 xxxx
//funcion con la configuracion de puertos
void fun_puertos() {
    DDRD = 0xFF;    // 1111 1111 PD0-PD7 configurados como salida
    DDRB |= 0x3F;   // xx11 1111 PB0-PB5 configurados como salida
    DDRC |= 0x01;   // xx00 0001 PC0 configurado como salida y PC1=PC5 como entrada 
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
// Leer el estado del botón

uint8_t leer_pulsador() {
    if ((PINC & (1 << pulsador)) == 0) {  // Botón presionado cuando es 0
        _delay_ms(50);  // Espera para evitar el rebote
        if ((PINC & (1 << pulsador)) == 0) {  // Verificar si sigue presionado después del retardo
            return 1;  // Botón presionado
        }
    }
    return 0;  // Botón no presionado
}


//Main
int main(void) {
    fun_puertos();
    uint8_t i = 0; 
    uint8_t j = 6; 
    uint8_t cont_1 = 0; 
    while (1) {
        
        if(i > 8){
            if(j == 0){
                i = 0;
                j = 6;
            }
            else{
                PORTD = (1 << j);
                _delay_ms(200);
                j--;
            }            
        }
        else{
            PORTD = (1 << i);
            _delay_ms(100);
            i++;
        }
        if(leer_pulsador()){
            cont_1++; 
        }
        uint8_t est_interruptores = interruptores;
        uint8_t out = est_interruptores + cont_1;
        fun_7seg(out);
    }
}
