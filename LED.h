/*
 * LED.h
 *
 * Created: 2/15/2014 12:02:21 PM
 *  Author: lopeztel
 */ 


#ifndef LED_H_
#define LED_H_


void ledInit();

void ledOn();

void ledOff();
void ledSetDutyCycle(uint8_t duty);

#endif /* LED_H_ */