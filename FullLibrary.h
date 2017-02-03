/*
 * FullLibrary.h
 *
 * Created: 1/24/2014 12:39:45 PM
 *  Author: hillp8
 */ 


#ifndef FULLLIBRARY_H_
#define FULLLIBRARY_H_

#define UP 1
#define DOWN 2
#define ENTER 3

#define bool uint8_t
#define TRUE 1
#define FALSE 0

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "USARTLibrary.h"
#include "RFLibrary.h"
#include "string.h"
#include "stdio.h"
#include "UltrasonicLibrary.h"
#include "CardReaderLibrary.h"
#include "ButtonLibrary.h"
#include "Power.h"
#include "LcdLibrary.h"
#include "Maintenance.h"
#include "Globals.h"
#include "LED.h"
#include "asm/aes.h"







#endif /* FULLLIBRARY_H_ */