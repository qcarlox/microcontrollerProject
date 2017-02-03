#include "FullLibrary.h"
#define LCD_DDR DDRD
#define LCD_PRT PORTD
#define LCD_RS 0 //LCD RS
#define LCD_EN 1 //LCD EN
#define LCD_DT 2 //Shift Data
#define LCD_ST 3 //Shift Clock
#define POWER_PRT PORTB //PB7
#define POWER_PIN 7
#define POWER_DDR DDRB //PB7
#define START 0
#define BUTTONPRESS 1


//****************************************************************
//****************************************************************
void lcdCommand (uint8_t cmnd)
{
	for(int i = 0;i<8;i++)
	{
		if((cmnd&0x80) == 0)
		{
			LCD_PRT &= ~(1<<LCD_DT);
		}
		else
		{
			LCD_PRT |= (1<<LCD_DT);
		}
		cmnd <<= 1;
		LCD_PRT |= (1<<LCD_ST);
		_delay_us(1);
		LCD_PRT &= ~(1<<LCD_ST);
		LCD_PRT &= ~(1<<LCD_DT);
	}
	LCD_PRT &= ~(1<<LCD_RS); //RS = 0 for command
	LCD_PRT |= (1<<LCD_EN); //EN = 1 for H-to-L pulse
	_delay_us(1); //wait to make enable wide
	LCD_PRT &= ~(1<<LCD_EN); //EN = 0 for H-to_L pulse
	_delay_us(100); //wait to make enable wide
	LCD_PRT &= ~(1<<LCD_ST);
	LCD_PRT &= ~(1<<LCD_DT);
	LCD_PRT &= ~(1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_EN);
}
void lcdData(uint8_t data)
{
	for(int i = 0;i<8;i++)
	{
		if((data&0x80) == 0)
		{
			LCD_PRT &= ~(1<<LCD_DT);
		}
		else
		{
			LCD_PRT |= (1<<LCD_DT);
		}
		data <<= 1;
		LCD_PRT |= (1<<LCD_ST);
		_delay_us(1);
		LCD_PRT &= ~(1<<LCD_ST);
	}
	LCD_PRT |= (1<<LCD_RS); //RS = 1 for data
	LCD_PRT |= (1<<LCD_EN); //EN = 1 for H-to-L pulse
	_delay_us(1); //wait to make enable wide
	LCD_PRT &= ~(1<<LCD_EN); //EN = 0 for H-to_L pulse
	_delay_us(100); //wait to make enable wide
	LCD_PRT &= ~(1<<LCD_ST);
	LCD_PRT &= ~(1<<LCD_DT);
	LCD_PRT &= ~(1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_EN);
}
void lcdInit()
{
	_delay_ms(25);
	LCD_DDR |= 0x0F;
	LCD_PRT &= ~(1<<LCD_EN); //LCD_EN = 0
	_delay_ms(5);
	lcdCommand(0x38); //turn on second line, and 8 bit data size
	_delay_us(5000);
	lcdCommand(0x0C); //display on
	lcdCommand(0x01); //clear LCD
	_delay_us(2000); //wait
	//lcdCommand(0x06); //shift cursor right
}

//****************************************************************************
void lcdGoToY(uint8_t y)
{
	unsigned char firstCharAdr[] = {0x80, 0xC0, 0x94, 0xD4};
	lcdCommand(firstCharAdr[y-1] + 0);
	_delay_us(100);
}
void lcdPrint(uint8_t * str, uint8_t length)
{
	
	for(uint8_t i=0;i<16;i++)
	{
		if(i<length){
			lcdData(str[i]);
		}
		else{
			lcdData(' ');
		}
		
	}
		
}
/*void lcdScroll(uint8_t * message, uint8_t length){
	while(lastCommand == 0)
	{
		uint8_t messageIndex;
		lastCommand = 0;
		uint8_t buffer[16];
		for(int i = 1; i < 16; i++){
			buffer[i-1] = buffer[i];
		}
		buffer[15] = message[cardIndex++];
		if(messageIndex == length){
			messageIndex = 0;
		}
		lcdGoToY(1);
		lcdPrint((uint8_t*)"Card Data",9);
		lcdGoToY(2);
		lcdPrint(buffer,16);
		_delay_ms(500);
	}
}*/


void lcdUpdate(){
	uint8_t buffer[16];
	uint8_t dollars;
	uint8_t cents;
	uint8_t hours;
	uint8_t minutes;
	if(uiState==SET_POSTPAY){
		lcdGoToY(1);
		lcdPrint((uint8_t*)"SWIPE CARD",10);
		lcdGoToY(2);
		sprintf((char*)buffer,"PRICE %d.%02d",priceDollars,priceCents);
		lcdPrint((uint8_t*)buffer,strlen((char*)buffer));
	}
	else if(uiState==SET_PREPAY){
		lcdGoToY(1);
		getTotalPrice(&dollars,&cents,(uint16_t)prepayTime*5);
		sprintf((char*)buffer,"PRICE %d.%02d",dollars,cents);
		lcdPrint(buffer,strlen((char*)buffer));
		lcdGoToY(2);
		getTotalTime(&hours,&minutes);
		sprintf((char*)buffer,"TIME %d:%02d",hours,minutes);
		lcdPrint(buffer,strlen((char*)buffer));
	}
	else if(uiState == FINAL_PREPAY){
		lcdGoToY(1);
		lcdPrint((uint8_t*)"SWIPE CARD",10);
		lcdGoToY(2);
		lcdPrint((uint8_t*)"PRESS TO CANCEL",15);
	}
	else if(uiState == PREPAID){
		lcdGoToY(1);
		sprintf((char*)buffer,"TIME %d:%02d",timeHours,timeMinutes);
		lcdPrint(buffer,strlen((char*)buffer));
		lcdGoToY(2);
		lcdPrint((uint8_t*)"",0);
	}
	else if(uiState == POSTPAID){
		lcdGoToY(1);
		sprintf((char*)buffer,"TIME %d:%02d",timeHours,timeMinutes);
		lcdPrint(buffer,strlen((char*)buffer));
		lcdGoToY(2);
		getTotalPrice(&dollars,&cents,(uint16_t)timeHours*60+timeMinutes);
		sprintf((char*)buffer,"PRICE %d:%02d",dollars,cents);
		lcdPrint(buffer,strlen((char*)buffer));
		
	}
	else if(uiState == PREPAY_ERROR){
		lcdGoToY(1);
		lcdPrint((uint8_t*)"Card Declined",13);
		lcdGoToY(2);
		lcdPrint((uint8_t*)"Press to retry",14);
	}
	else if(uiState == POSTPAY_ERROR){
		lcdGoToY(1);
		lcdPrint((uint8_t*)"Card Declined",13);
		lcdGoToY(2);
		lcdPrint((uint8_t*)"Press to retry",14);
	}
	else if(uiState == WAITING_POSTPAY || 
			uiState == WAITING_PREPAY || 
			uiState == WAITING_TO_SEND_CARD_POSTPAY || 
			uiState == WAITING_TO_SEND_CARD_PREPAY){
		lcdGoToY(1);
		lcdPrint((uint8_t*)"Waiting for hub",15);
		lcdGoToY(2);
		lcdPrint((uint8_t*)"",0);
	}
	
}

void getTotalPrice(uint8_t*dollars,uint8_t*cents,uint16_t time ){
	uint32_t tempDollars;
	uint32_t tempCents;
	tempCents = ((uint32_t)priceCents + (uint32_t)priceDollars*100)*time/60;
	tempDollars = tempCents/100;
	tempCents = tempCents%100;
	*dollars = (uint8_t)tempDollars;
	*cents = (uint8_t)tempCents;
}

void getTotalTime(uint8_t*hours,uint8_t*minutes){
	uint16_t tempMinutes;
	tempMinutes = prepayTime*5;
	*hours = tempMinutes/60;
	*minutes = tempMinutes%60;

}

