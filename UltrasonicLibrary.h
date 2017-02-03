/*
 * UltrasonicLibrary.h
 *
 * Created: 1/31/2014 1:34:47 PM
 *  Author: hillp8
 */ 


#ifndef ULTRASONICLIBRARY_H_
#define ULTRASONICLIBRARY_H_


/*
 * UltrasonicLibrary.c
 *
 * Created: 1/31/2014 1:34:57 PM
 *  Author: hillp8
 */
 #include "FullLibrary.h"

#define TRIG_MASK (1<<PORTB0)
#define SET_TRIG_OUTPUT DDRB |= TRIG_MASK
#define SET_ECHO_INPUT PORTD |= (1<<PORTD7)
#define SET_TRIG PORTB |= TRIG_MASK
#define CLEAR_TRIG PORTB &= ~TRIG_MASK



void ultrasonicInit();

void ultrasonicInterruptInit();

void ultrasonicUpdateDistance();
uint8_t ultrasonicMinDistance();
#endif /* ULTRASONICLIBRARY_H_ */