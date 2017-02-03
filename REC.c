/*
 *
 * Created: 1/28/2014 12:36:23 PM
 *  Author: hillp8
 */ 
#include "FullLibrary.h"
int main(void)
{
	while(1)
	{
		uint8_t buffer[256];
		rfRead(buffer);
		usartSendString(buffer+1,buffer[0]);
		_delay_ms(1000);
	}
}