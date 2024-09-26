#include <avr/io.h>
#include <avr/interrupt.h>

unsigned contador = 0;
//funcion con la configuracion de puertos
void fun_puertos() {
    DDRB |= 0x0F; //xxxx xx11 Salida
    DDRD = (DDRD & 0xF0) | (DDRD | 0xF0); //1111 0000
    
}
void fun_init(){
    PORTB &= ~(1<<0); //PB0 inicializado en 0.
    PORTD |= (1 << PD3);
    PORTD |= (1 << PD2);
}
void fun_interrupsiones() {
    EICRA = 0b00001010; //Interrupción externa por flanco de bajada en INT0
    EIMSK = 0b00000011; //Habilita interrupción externa a INT0
    // EIMSK = (1<<INT0); //Oytra forma de habilitar INT0.
    EIFR = 0b00000000; //Bajamos Bandera de interrupció a cero.
    sei(); //Habilitar Interrupciones globales
    /* Replace with your application code */
}
//Interrupción por flanco de bajada en INT0 
ISR(INT1_vect){
    contador--;
    PORTD |= contador & 0x0F; 
    PORTB |= (contador & 0xF0) >> 4; ; 
}
//Interrupción por flanco de bajada en INT0 
ISR(INT0_vect){
    contador++;
    PORTD |= contador & 0x0F; 
    PORTB |= (contador & 0xF0) >> 4; ; 
}
//-------------------- Main Program -------------------------
int main(void){
    //contador = 0;
    fun_puertos();
    fun_init();
    fun_interrupsiones();
    while(1){
        //PORTD |= contador & 0x0F; 
        //PORTB |= (contador & 0xF0) >> 4; ; 
    }
}