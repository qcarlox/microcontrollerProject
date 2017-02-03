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
#define MAX_DISTANCE	100
#define MIN_DISTANCE	10
#define DISTANCE_MEASUREMENTS 3
ISR(TIMER1_COMPA_vect)
{
	cli();
	ultrasonicTick();
	TCCR1B = 0x00;
	ultrasonicUpdateDistance();
	uint8_t distance = ultrasonicMinDistance();
	uint8_t previousIsCarParked = isCarParked;
	
	if(distance<MAX_DISTANCE && distance>MIN_DISTANCE)
	{
		isCarParked = 1;
		isBroken = 0;
	}
	else
	{
		isCarParked = 0;
		if(distance<MIN_DISTANCE){
			isBroken = 1;
			isCarParked = 1;
		}
		else{
			isBroken = 0;
		}
	}
	if(isCarParked != previousIsCarParked){
		ledPowerTicks = 0;
		ledOn();
		if(isCarParked){
			turnOnUIPower();
			
		}
	}
	//uint8_t buffer[128];
	//sprintf((char*)buffer,"Distance:  %u cm\n",distance);
	//usartSendString(buffer,strlen(buffer));
	//rfSend(buffer,strlen((char*)buffer));
	//lcdPrint(buffer,strlen((char*)buffer));
	TCNT1 = 0;
	TCCR1B = 0x04;
	TIFR1 |= 0x2;
	sei();
}

void ultrasonicInit(){
	SET_TRIG_OUTPUT;
	SET_ECHO_INPUT;
	
}
void ultrasonicInterruptInit(){
	TCNT1 = 0;
	OCR1A = 0x8000;
	TIMSK1 = 0x02;
	TCCR1B = 0x04;
	
}

void ultrasonicUpdateDistance()
{
	uint16_t time;
	SET_TRIG;
	_delay_us(10);
	CLEAR_TRIG;
	TCNT1 = 0x0000;
	TCCR1B |= (1<<CS11);	//start timer1 with 1/8 prescaler
	while(!(PIND&0x80) && TCNT1<7000)		//wait until PD7 is set
	{
	}
	TCCR1B &= ~(1<<CS11);	//stop timer1

	TCNT1 = 0x0000;
	TCCR1B |= (1<<CS11);	//start timer1 with 1/8 prescaler
	while((PIND&0x80) && TCNT1<7000)		//wait for PD7 to be low
	{
	}
	TCCR1B &= ~(1<<CS11);	//stop timer1
	time = TCNT1;
	ultrasonicCounter++;
	ultrasonicCounter %= 3;
	distanceHistory[ultrasonicCounter] = time>>6;//time/58;
}
uint8_t ultrasonicMinDistance(){
	uint8_t min = distanceHistory[0];
	for(uint8_t i = 1;i<DISTANCE_MEASUREMENTS;i++){
		if(min > distanceHistory[i]){
			min = distanceHistory[i];
		}
	}
	return min;
}
