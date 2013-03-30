/*-------|---------|---------|---------|---------|---------|---------|---------|
grativator.c	

a sine wave generator 

by chaz miller for ATMEGAxx8 

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3 or any later
version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
******************************************************************/
//todo: add GFCI?

#include <avr/io.h>

void delay(uint16_t);
void blink (uint8_t);
void adc_init(void);
uint16_t adc_read(uint8_t);

int main(){
    //8 bit Timer 0 is used by delay().
    TCCR0A = 0;                //standard timer mode (page 103)
    TCCR0B = 2;                //fcpu / 1
    blink(3);

    //16-bit Timer 1 used as output PWM on OC1B PB2 (Arduino pin 10) p.115
    //fast, CTC-PWM hybrid mode p135 
    TCCR1A = (1<<COM1B1)|(1<<WGM11)|(1<<WGM10)|(1<<WGM13)|(1<<WGM12); 
    OCR1A = 0xFF;             //sets pwm TOP 
    OCR1B = 0;
    TCCR1B = (1<<WGM13)|(1<<CS10);                   //  clk/1; 60kHz @

    //CLKPR = 0x80; CLKPR = 0;                       //  clk*8; 2kHz
    //8 bit Timer 2 for compressor delay; 1 may work?
    TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);                //CPU/1024; 2Hz
    PORTB = 0xFF;
    DDRB = 0b000100100;              //LED on PB5; OC1B is PB2
    PORTB|=1<<5;                     //turn on LED
    adc_init();
    uint8_t index;
    static uint8_t sinewave[64]= {0,13,25,37,50,62,74,86,98,109,120,131,
    142,152,162,171,180,189,197,205,212,219,225,231,236,240,244,247,250,252,
    254,255,255,255,254,252,250,247,244,240,236,231,225,219,212,205,197,189,
    180,171,162,152,142,131,120,109,98,86,74,62,50,37,25,13};
    /****************************************
    *****main loop***************************
    ****************************************/
    for(;;){  
	index++;
	OCR1B = sinewave[index&0x3F];
	//delay ticks at pwm frequency. delay N = N cycles per pwm step.
	delay(8);
    }
}//main
void adc_init(void){
    //ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0); //125kHz @ 1MHz clock, page 264
    ADCSRA = (1<<ADEN)|(1<<ADPS2); //62kHz @ 1MHz clock, page 264
    ADMUX |= (1<<REFS0);           //AREF=Avcc (5v)
    //ADMUX |= (1<<ADLAR);           //left align for 8-bit operation
    ADCSRA |= (1<<ADEN); 
}
uint16_t adc_read(uint8_t me){    //expects ADMUX register value
    uint16_t ad_bucket=0;
    ADMUX &= 0xF0;
    ADMUX |= me;
    for (int i=0; i<16; i++){
	ADCSRA |= (1<<ADSC); 
	while(ADCSRA & (1<<ADSC)); 
	ad_bucket += ADCW;
    }
    return (ad_bucket>>2); //12 bits oversampled
}
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
	delay(800);
	PORTB &= ~(1<<5);
	delay(800);
    }
    delay(1500);
}
Set a bit
 bit_fld |= (1 << n)

Clear a bit
 bit_fld &= ~(1 << n)

Toggle a bit
 bit_fld ^= (1 << n)

Test a bit
 bit_fld & (1 << n)
*/ 
