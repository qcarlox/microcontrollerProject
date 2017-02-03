/*
 * LED.c
 *
 * Created: 2/15/2014 12:02:05 PM
 *  Author: lopeztel
 */ 
#include "FullLibrary.h"

void ledInit(){
	DDRD |= (1<<PORTD6);	
}

void ledOn(){
	TCCR0A = 0x83;
	TCCR0B = 0x04;
	OCR0A = brightness;
}

void ledOff(){
	TCCR0B = 0x00;
	TCCR0A = 0x00;
}
void ledSetDutyCycle(uint8_t duty){
	brightness = duty;
	OCR0A = duty;
}