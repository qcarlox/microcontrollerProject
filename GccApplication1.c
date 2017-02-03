
#include "FullLibrary.h"


int main(void)
{
	
	
	
	PORTD |= (1<<5);	//mode select based to determine if hub or meter
	
	if(1)
	{
		maintenanceInit();
		if(isMaintenance()){
			maintenanceMode();
		}

		ultrasonicInit();
		ultrasonicInterruptInit();
		buttonInit();
		rfInit();
		setAddressFromEEPROM();
		ledInit();
		ledOn();
		ledSetDutyCycle(20);
		turnOnUIPower();
		sei();
		
	}
	else
	{
		usartInit();
		rfInit();
		usartSendNullTerminatedString((uint8_t*)"Start");
	}
	
	
	while(1)
	{
		
		if(1)
		{
			uint8_t cardWritten = 0;
			uint8_t inBufferSize = 20;
			uint8_t outBufferSize = 100;
			uint8_t inBuffer[inBufferSize];
			uint8_t outBuffer[outBufferSize];
			uint8_t messageLengthIn = inBufferSize;
			uint8_t messageLengthOut = outBufferSize;
			uint8_t rfResult;
			for(uint8_t i=0;i<inBufferSize;i++)
			{
				inBuffer[i] = 0;
			}
			outBuffer[0] = isCarParked<<7;
			outBuffer[0] |= (isNewCard<<6);
			outBuffer[0] |= (isPrepay<<5);
			outBuffer[0] |= (isBroken<<4);
			outBuffer[1] = priceDollars;
			outBuffer[2] = priceCents;
			if(isNewCard){
				cardWritten = 1;
				for(uint8_t i=0;i<80;i++)
				{
					outBuffer[i+3] = cardBuffer[i];
				}
				if(isPrepay){
					messageLengthOut = 84;
					outBuffer[83] = prepayTime;
				}else{
					messageLengthOut = 83;
				}
			}else{
				messageLengthOut = 3;
			}
			switch(rfResult = rfRead(inBuffer,&messageLengthIn)){
				case RF_SUCCESS:{
					rfResult = rfSend((uint8_t*)outBuffer,messageLengthOut);
					if(rfResult==RF_SUCCESS){
						if(cardWritten == 1){
							isNewCard = 0;
							for(uint8_t i=0;i<80;i++)
							{
								cardBuffer[i] = 0;
								outBuffer[i+4] = 0;
							}
							
						}
					}
					ledSetDutyCycle(inBuffer[2]);
					meterStatus = inBuffer[3];
					timeHours = inBuffer[4];
					timeMinutes = inBuffer[5];
					priceDollarsNext =  inBuffer[0];
					priceCentsNext = inBuffer[1];
					switch(uiState){
						case WAITING_TO_SEND_CARD_POSTPAY:
						case WAITING_TO_SEND_CARD_PREPAY:
							setUIStateRF();
						break;
						case WAITING_POSTPAY:
						case WAITING_PREPAY:
							setUIStateRF();
							turnOnUIPower();
							break;
						case PREPAID:
						case POSTPAID:
							setUIStateRF();
							if(isPower()){
								turnOnUIPower();
							}
							break;
					}
					
				}break;
				case RF_TIMEOUT:{
					
				}break;
				default:{
					sprintf((char*)outBuffer,"%d",rfResult);
					lcdGoToY(1);
					lcdPrint((uint8_t*)"Error",5);
					lcdGoToY(2);
					lcdPrint(outBuffer,1);
				}break;
			}
			
			/*
			ledOn();
			_delay_ms(2000);
			ledSetDutyCycle(25);
			_delay_ms(2000);
			ledSetDutyCycle(230);
			_delay_ms(2000);
			ledOff();
			_delay_ms(2000);*/
			/*uint8_t buffer[512];
			uint8_t* dst = buffer;
			uint8_t* lut = (uint8_t*)"abcdefghijklmnopqrstuvwxyz0123456789\n";
			for(int i = 0; i < 10; i++){
				(*dst++) = lut[i]-32;
				for(int j = 0; j < 37; j++){
					(*dst++) = lut[j];
				}
			}
			rfSend(buffer,380);
			_delay_ms(1000);*/
		}
		else
		{
			
			uint8_t bufferSize = 255;
			uint8_t message[bufferSize];
			uint8_t length;
			length = serialRead();
			if(length == 0){
				continue;
			}
			
			uint8_t address[5];
			for(uint8_t i = 0; i < 5; i++){
				address[i] = serialRead();
			}
				
				/*
			for(uint8_t i=0; i< length;i++){
				message[i+1] = serialRead();
			}
			message[0] = length;
			usartSendString(message,length+1);*/
			
			
			for(uint8_t i=0; i < length; i++){
				message[i] = serialRead();
			}
			
			setAddress(address);
			if(rfSend(message,length)==RF_SUCCESS){
				//usartSendNullTerminatedString("Sent");
				uint8_t messageSize = bufferSize-1;
				if(rfRead(message+1,&messageSize)==RF_SUCCESS)
				{
					//usartSendNullTerminatedString("Rec");
					message[0] = messageSize;
					usartSendString(message,messageSize+1);
				}else{
					usartSend(4);
					usartSendNullTerminatedString((uint8_t*)"FAIL");
				}
			}else{
				usartSend(5);
				usartSendNullTerminatedString((uint8_t*)"FAIL2");
			}
			
			
				
			/*
			uint16_t bufferSize = 512;
			uint8_t buffer[bufferSize];
			
			uint16_t messageSize = bufferSize;
			uint8_t rfResult = rfRead(buffer,&messageSize);
			switch(rfResult){
				case RF_SUCCESS:{
					usartSendString(buffer,messageSize);
				}break;
				default:{
					sprintf((char*)buffer,"RF Error:  %d\n",rfResult);
					usartSendNullTerminatedString(buffer);
				}break;
			}
			*/
		}
		
	}
}