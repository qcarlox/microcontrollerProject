/*
 * Watchdog.c
 *
 * Created: 2/23/2014 12:07:44 AM
 *  Author: griever99999
 */ 
#include "FullLibrary.h"
void watchdogDisable(){
	cli();
	wdt_reset();
	MCUSR &= ~(1<<WDRF);
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = 0x00;
	sei();
}