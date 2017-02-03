/*
 * Maintenace.c
 *
 * Created: 2/7/2014 3:05:13 PM
 *  Author: lopeztel
 */ 
#include "FullLibrary.h"



void maintenanceInit(){
	PORTB |= (1<<6);
}

#define ULTRASONIC 0
#define CARDREADER 1
#define ASSIGN_ID  2
#define DISPLAY_ID 3

void maintenanceMode(){
	buttonInit();
	turnOnUIPower();
	lcdInit();			//turn on LCD
	sei();
	int modeSelect = 0;
	lastCommand = 0;
	while(1)
	{
		
		if(lastCommand == UP)
		{
			lastCommand = 0;
			modeSelect += 1;
			modeSelect %= 4;
		}
		else if(lastCommand == DOWN)
		{
			lastCommand = 0;
			modeSelect -= 1;
			if(modeSelect == -1){
				modeSelect = 3;
			}
		}
		else if(lastCommand == ENTER)
		{
			lastCommand = 0;
			if(modeSelect == ULTRASONIC)
			{
				testUltrasonic();
			}
			else if(modeSelect == CARDREADER)
			{
				testCardReader();
			}
			else if(modeSelect == ASSIGN_ID)
			{
				assignID();
			}
			else if(modeSelect == DISPLAY_ID)
			{
				displayID();
			}
			
		}
		else if(modeSelect == ULTRASONIC)
		{
			lcdGoToY(1);
			lcdPrint((uint8_t*)"Choose Mode",11);
			lcdGoToY(2);
			lcdPrint((uint8_t*)"Ultrasonic Test",15);
		}
		else if(modeSelect == CARDREADER)
		{
			lcdGoToY(1);
			lcdPrint((uint8_t*)"Choose Mode",11);
			lcdGoToY(2);
			lcdPrint((uint8_t*)"Card Reader Test",16);
		}
		else if(modeSelect == ASSIGN_ID)
		{
			lcdGoToY(1);
			lcdPrint((uint8_t*)"Choose Mode",11);
			lcdGoToY(2);
			lcdPrint((uint8_t*)"Assign ID",9);
		}
		else if(modeSelect == DISPLAY_ID)
		{
			lcdGoToY(1);
			lcdPrint((uint8_t*)"Choose Mode",11);
			lcdGoToY(2);
			lcdPrint((uint8_t*)"Display ID",10);
		}
	}
}

int isMaintenance(){
	if((PINB&(1<<6)) == 0){
		return 0;
	}
	return 1;
}

void testUltrasonic(){
	ultrasonicInit();
	uint8_t buffer[16];
	while(lastCommand != ENTER)
	{
		lastCommand = 0;
		ultrasonicUpdateDistance();
		lcdGoToY(1);
		lcdPrint((uint8_t*)"Distance, Min",13);
		sprintf((char*)buffer,"%d, %d",distanceHistory[ultrasonicCounter],ultrasonicMinDistance());
		lcdGoToY(2);
		lcdPrint(buffer,strlen((char*)buffer));
		_delay_ms(1000);
	}
	lastCommand = 0;
}


void displayID(){
	uint8_t buffer[16];
	eeprom_read_block((void *)buffer,(void*)eeAddress,5);
	uint8_t asciiBuffer[10];
	hexToAscii(buffer,5,asciiBuffer);
	while(lastCommand != ENTER)
	{
		lastCommand = 0;
		lcdGoToY(1);
		lcdPrint((uint8_t*)"ID",2);
		lcdGoToY(2);
		lcdPrint(asciiBuffer,10);
		_delay_ms(1000);
	}
	lastCommand = 0;
}


void testCardReader(){
	cardReaderInit();
	//rfInit();
	uint8_t buffer[16];
	uint8_t cardIndex = 0;
	
	for(int i = 0; i < 16; i++){
		buffer[i] = ' ';
	}
	
	for(int i = 0; i < 79; i++){
		cardBuffer[i] = ' ';
	}
	
	while(lastCommand != ENTER)
	{
		lastCommand = 0;
		if(isNewCard){
			isNewCard = FALSE;
			cardIndex = 0;
		}
		for(int i = 1; i < 16; i++){
			buffer[i-1] = buffer[i];
		}
		buffer[15] = cardBuffer[cardIndex++];
		if(cardIndex == 79){
			cardIndex = 0;
		}
		lcdGoToY(1);
		lcdPrint((uint8_t*)"Card Data",9);
		lcdGoToY(2);
		lcdPrint(buffer,16);
		_delay_ms(500);
	}
	lastCommand = 0;
}

void testRf(){
	
}

void assignID(){
	rfInit();
	uint8_t bufferSize = 255;
	uint8_t buffer[bufferSize];
	uint8_t messageSize;
	
	lastCommand = 0;
	while(1){
		buffer[0] = 0xE7;
		buffer[1] = 0xE7;
		buffer[2] = 0xE7;
		buffer[3] = 0xE7;
		buffer[4] = 0xE7;
		setAddress(buffer);
		lcdGoToY(1);
		lcdPrint((uint8_t*)"Getting ID",10);
		messageSize = bufferSize;
		uint8_t rfResult = rfRead(buffer,&messageSize);
		switch(rfResult){
			case RF_SUCCESS:{
				lcdGoToY(2);
				uint8_t buffer1[messageSize*2];
				hexToAscii(buffer,messageSize,buffer1);
				lcdPrint(buffer1,messageSize*2);
				setAddress(buffer);
				lcdGoToY(1);
				lcdPrint((uint8_t*)"Verifying ID",12);
				while(lastCommand == 0){
					uint8_t rfResult = rfRead(buffer,&messageSize);
					switch(rfResult){
						case RF_SUCCESS:{
							lcdGoToY(1);
							lcdPrint((uint8_t*)"ID Verified",11);
							eeprom_update_block((const void*)buffer,(void*)eeAddress,5);
							while(lastCommand != ENTER);
							lastCommand = 0;
						}goto loop;
					}
				}
				lastCommand = 0;
			}break;
		}
	}
	loop:;
}

void hexToAscii(uint8_t * hex,uint8_t hexLength,uint8_t* ascii){
	int j=0;
	for(int i=0;i<hexLength;i++)
	{
		int temp = (hex[i]>>4)&0x0F;
		
		if(temp>9){
			temp = temp+'A'-10;
		}
		else{
			temp = temp + '0';
		}
		ascii[j] = temp;
		j++;
		temp = hex[i]&0x0F;
		if(temp>9){
			temp = temp+'A'-10;
		}
		else{
			temp = temp + '0';
		}
		ascii[j] = temp;
		j++;
	}
}