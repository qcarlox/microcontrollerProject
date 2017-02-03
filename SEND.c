/*
 * CFile1.c
 *
 * Created: 1/28/2014 12:36:23 PM
 *  Author: hillp8
 */ 
/*
#include "FullLibrary.h"
int main(void)
{
	rfInit();
	while(1)
	{
		if((PINB&(1<<6)) == (1<<6))
		{
			rfSend("Hello");
			_delay_ms(1000);
		}
		else
		{
			uint8_t buffer[256];
			rfRead(buffer);
			usartSendString(buffer+1,buffer[0]);
			_delay_ms(1000);
		}
		
	}
}*/