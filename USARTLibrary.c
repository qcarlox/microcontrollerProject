/*
 * USARTLibrary.h
 *
 * Created: 1/24/2014 12:37:08 PM
 *  Author: hillp8
 */ 

#include "FullLibrary.h"

int serialFlags;

#define SERIAL_FLAG_TIMEOUT 1

uint8_t didSerialTimeout(){
	return (serialFlags&SERIAL_FLAG_TIMEOUT)!=0;
}

uint8_t serialCheckRxComplete(void)
{
	return( UCSR0A & _BV(RXC0)) ;		// nonzero if serial data is available to read.
}

uint8_t serialRead(){
	while (serialCheckRxComplete() == 0)		// While data is NOT available to read
	{}
	return UDR0;
}

uint8_t serialReadTimeout()
{
	
	TCNT0 = 0;
	TCCR0A = 0;
	TCCR0B = 0x5;
	
	
	while (serialCheckRxComplete() == 0)		// While data is NOT available to read
	{
		if(TCNT0 > 40){
			serialFlags |= SERIAL_FLAG_TIMEOUT;
			TCCR0B = 0;
			return 0;
		}
	}
	TCCR0B = 0;
	serialFlags &= ~SERIAL_FLAG_TIMEOUT;
	return UDR0;
}

void usartSend( uint8_t ch)
{
	while( !(UCSR0A&0x20) );		//poll UDRE0, when set begin new transmission
	UDR0 = ch;
}

void usartSendString(uint8_t* str,  uint8_t strLength){
	for (uint8_t i=0; i<strLength; i++)
	{
		usartSend(str[i]);
	}
}

void usartSendNullTerminatedString(uint8_t* str){
	int i = 0;
	while(str[i] != '\0'){
		usartSend(str[i++]);
	}
}

void usartInit(){
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); ;
	UCSR0C = 0x06;
	UCSR0A |= 2;
	UBRR0 = 0;
}
