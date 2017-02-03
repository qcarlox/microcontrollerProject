/*
 * Power.c
 *
 * Created: 2/6/2014 1:38:01 PM
 *  Author: lopeztel
 */ 
#include "FullLibrary.h"
void powerInit(){
	DDRB |= (1<<7);
}
void turnOnUIPower(){
	if(isPower()==FALSE){
		uiPowerState = 1;
		powerInit();
		//PORTB |= (1<<7);
		PORTB &= ~(1<<7);
		uiPowerTicks = 0;
		lcdInit();
		cardReaderInit();
		
	}
	lcdUpdate();
}
void turnOffUIPower(){
	uiPowerState = 0;
	
	if(meterStatus != LEGAL_PARK){
		priceDollars = priceDollarsNext;
		priceCents = priceCentsNext;
	}
	
	//powerInit();
	PORTB |= (1<<7);
	cardReaderOff();
}
int isPower(){
	if((PINB&(1<<7)) == 0){
		return 1;
	}
	return 0;
}
void ultrasonicTick(){
	
	ledPowerTicks++;
	if(ledPowerTicks>5){
		ledPowerTicks = 254;
		ledOff();
	}else{
		if((ledPowerTicks&1)==1){
			ledOff();
		}else{
			ledOn();
		}
	}
	uiPowerTicks++;
	if(uiPowerTicks>5){
		turnOffUIPower();
		uiPowerTicks = 254;
	}
}