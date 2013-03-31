/*-------|---------|---------|---------|---------|---------|---------|---------|
squar.c	

test internal interrupts

by chaz miller for ATMEGAxx8 

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3 or any later
version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
******************************************************************/
//it should take about 200us for wave to travel across bukkit

#include <avr/io.h>
#include <avr/interrupt.h>

void delay(uint16_t);
void blink (uint8_t);

uint16_t counts;

ISR(INT0_vect){
    TCNT1 = 0;;
}
ISR(INT1_vect){
    counts = TCNT1;
}
int main(){
    EIMSK |= (1<<INT0);  //Enable INT0
    EIMSK |= (1<<INT1);  //Enable INT1
    EICRA |= (1<<ISC01)|(1<<ISC00); //Trigger on rising edge of INT0
    EICRA |= (1<<ISC11)|(1<<ISC10); //Trigger on rising edge of INT1
    sei();

    //8 bit Timer 0 is used by delay().
    TCCR0A = 0;                //standard timer mode (page 103)
    TCCR0B = 2;                //fcpu / 1

    //TCCR1A = (1<<COM1B1)|(1<<WGM11)|(1<<WGM10)|(1<<WGM13)|(1<<WGM12); 
    TCCR1B = (1<<CS10);                   //  clk/1
    //CLKPR = 0x80; CLKPR = 0;                       //  clk*8; 2kHz

    DDRD = 0;
    PORTD = 0;

    blink(4);
    /****************************************
    *****main loop***************************
    ****************************************/
    for(;;){  
	blink(counts>>3);
	delay(4000);  //4=1ms
    }
}//main
void delay(uint16_t me){    //at 1MHz, each unit is 2.55us. 1ms is 4units. 
    while(me){
	while(TCNT0 < 128){}
	me--;
	while(TCNT0 > 128){}
    }
}
void blink (uint8_t me){
    for (int i=0; i<me; i++){
	PORTB |= (1<<5);
	delay(300);
	PORTB &= ~(1<<5);
	delay(300);
    }
    delay(1500);
}
/*
Set a bit
 bit_fld |= (1 << n)

Clear a bit
 bit_fld &= ~(1 << n)

Toggle a bit
 bit_fld ^= (1 << n)

Test a bit
 bit_fld & (1 << n)
*/ 
