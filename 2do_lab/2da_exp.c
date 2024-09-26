#include <avr/io.h>
#include <avr/interrupt.h>

unsigned contador = 0;
unsigned decenas = 0;
unsigned unidades = 0;
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
    DDRD = (DDRD & 0xF0) | 0x03; // xxxx 0011 PD2-PD3 conf. como entrada PD0-PD1 conf. como salida 
    DDRB |= 0x3F;   // xx11 1111 PB0-PB5 configurados como salida (6 bit Disp_7seg)
    DDRC |= 0x01;   // xxxx xxx1 PC0 configurado como salida (1 bit Disp_7seg)
}

void fun_init() {
    PORTB &= 0xC0; //PB inicializado en 0.
    PORTC &= 0xFE; //PC0 inicializado en 0.
    PORTD |= (1 << PD3); // Pull-up en PD3
    PORTD |= (1 << PD2); // Pull-up en PD2
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

void fun_interrupsiones() {
    EICRA = 0b00001010; //Interrupción externa por flanco de bajada en INT0 e INT1
    EIMSK = 0b00000011; //Habilita interrupción externa en INT0 y INT1
    EIFR = 0b00000000; //Bajamos Bandera de interrupción a cero.
    sei(); //Habilitar Interrupciones globales
}

//Interrupción por flanco de bajada en INT1
ISR(INT1_vect) {
    contador++;
}

//Interrupción por flanco de bajada en INT0
ISR(INT0_vect) {
    contador--;
}

//-------------------- Main Program -------------------------
int main(void) {
    fun_puertos();
    fun_init();
    fun_interrupsiones();
    while(1) {
        decenas = contador / 10;
        unidades = contador % 10;
        PORTD |= (1 << PD0);
        fun_7seg(unidades)
        PORTD &= ~(1 << PD0);
        PORTD |= (1 << PD1);
        fun_7seg(decenas)
        PORTD &= ~(1 << PD1);
    }
}
