/*
 * CardReaderLibrary.c
 *
 * Created: 1/31/2014 3:19:04 PM
 *  Author: hillp8
 */ 

#include "FullLibrary.h"


enum TrackReadingState{WaitingForLoaded, WaitingForFall, WaitingForRise, FinishedReading};

typedef struct{
	enum TrackReadingState readingState;
	char loadedPinMask;
	char clockPinMask;
	char dataPinShift;
	char* buffer;
	uint8_t bufferLength;
	uint8_t bufferIndex;
	uint8_t bitIndex;
	uint8_t bitsPerByte;
} TrackState;

void clearBuffer(char* buffer, char bufferLength){
	for(uint8_t i = 0; i < bufferLength; i++){
		buffer[i] = 0;
	}
}

void initTrackState(TrackState* trackState, char* buffer, char bufferLength){
	clearBuffer(buffer,200);
	trackState->bitIndex = 0;
	trackState->buffer = buffer;
	trackState->bufferLength = bufferLength;
	trackState->bufferIndex = 0;
	trackState->readingState = WaitingForLoaded;
}

void shiftTrack(char* track, char* shiftBuffer, uint8_t dir){
	if(dir == 1){
		uint8_t x = 0;
		for(uint8_t i = 0; i < 79+30; i++){
			shiftBuffer[i] = ((track[i]<<1)|x)&0x3F;
			x = (track[i]>>6) & 0x1;
		}
		}else{
		uint8_t x = 0;
		for(uint8_t i = 0; i < 79+30; i++){
			x = track[i+1] & 0x1;
			shiftBuffer[i] = ((track[i]>>1)|(x<<6));
		}
	}
	
	for(uint8_t i = 0; i < 79+30; i++){
		track[i] = shiftBuffer[i];
	}
}

uint8_t verifycard(char* track1) {
	uint8_t parityok = 1;
	
	// calculate parity for each byte
	for (uint8_t i = 0; i < 79; i++) {
		uint8_t p = 1;
		for (uint8_t j = 0; j < 6; j++) {
			if (track1[i] & (1<<j))
			p++;
		}
		p %= 2;
		if (p != track1[i] >> 6) {
			//Serial.print("Bad parity on ");
			//Serial.println(track1[i], HEX);
			parityok = 0;
		}
		if (track1[i] == 0x1F) break;
	}
	
	if ((track1[0] == 0x45) && (track1[1] == 0x62) && parityok) {
		return 1;
	}
	return 0;
}

void sendCardInformation(TrackState* trackState){
	volatile uint8_t* retBuffer = cardBuffer;
	char buffer[500];
	char* bufferPointer = buffer;
	
	//usartSendString("Start\n",6);
	for(uint8_t i = 0; i < trackState->bufferIndex; i++){
		char ch = trackState->buffer[i];
		
		//Check the parity
		char parity = (ch^(ch>>4))&0xF;
		parity = (parity^(parity>>2))&0x3;
		parity = (parity^(parity>>1))&0x1;
		
		/*if(parity == 0){
			usartSend('ÿ');
		}*/
		
		ch = (ch&0x3F)+0x20;
		(*bufferPointer++) = ch;
		
		/*
		usartSendString("TRAN\n",5);
		trans(ch);				//puts the NRF24L01 into TX and transmits current temperature
		_delay_ms(20);							//sleep for 1 sec
		
		if((commandret(NOP)&(1<<TX_DS)))	//check if ACK received
		{
			usartSendString("ACK\n",4);
			ch++;
		}*/
	}
	(*bufferPointer++) = '\n';
	memcpy((char*)retBuffer,buffer,79);	//information content in track 1 is 79 char
	//rfSend(buffer,bufferPointer-buffer);
	//usartSendString(buffer,bufferPointer-buffer);
	//usartSendString("\nEnd\n",5);
	
}

bool getCard(){
	char ts1Buffer[200];
	TrackState ts1;
	initTrackState(&ts1,ts1Buffer,200);
	ts1.loadedPinMask = 0x01;	//Pin 0
	ts1.clockPinMask = 0x02;	//Pin 1
	ts1.dataPinShift = 2;		//Pin 2
	ts1.bitsPerByte = 7;
	char scratch[200];
	
	
	if((PINC&ts1.loadedPinMask)){
		return FALSE;
	}
	
	//while((PINC&ts1.loadedPinMask));			//wait for card loaded pin PC0
	
	uint8_t zeros = 0;
	for(uint8_t t1 = 0; t1 < 79; t1++){
		ts1.buffer[t1] = 0;
		for(uint8_t b=0; b < 7; b++){
			while((PINC&ts1.clockPinMask) && !(PINC&ts1.loadedPinMask));
			
			uint8_t x = (PINC&4);
			if(!x){
				ts1.buffer[t1] |= 1<<b;
			}
			
			while(!(PINC&ts1.clockPinMask) && !(PINC&ts1.loadedPinMask));
		}
		
		if((t1==0) && ts1.buffer[t1]==0){
			zeros++;
			t1--;
			continue;
		}
		
		if(zeros<4){
			t1--;
			continue;
		}
		if((t1==1)&&ts1.buffer[t1]==0){
			t1 = -1;
			zeros = 1;
			continue;
		}
	}
	
	while((ts1.buffer[0]&0x1)==0){
		shiftTrack(ts1.buffer,scratch,0);
	}
	//0000000134000000
	
	if(!verifycard(ts1.buffer)){
		for(uint8_t i = 0; i < 79+30; i++)
		scratch[i] = 0;
		
		for(uint8_t i = 0; i < 79+30; i++){
			for(int8_t j=0; j < 7; j++){
				if(ts1.buffer[79+30-1-i] & (1<<(6-j)))
				scratch[i] |= (1<<j);
			}
		}
		
		for(uint8_t i = 0; i < 79+30; i++){
			ts1.buffer[i] = scratch[i];
		}
		
		while(((ts1.buffer[0]&0x1)==0)||ts1.buffer[1]==0)
		shiftTrack(ts1.buffer,scratch,0);
	}
	
	if(verifycard(ts1.buffer))
	{
		//usartSendNullTerminatedString("Swiped\n\0");
		ts1.bufferIndex = 79+30;
		sendCardInformation(&ts1);
		
		return TRUE;
		/*ts1Buffer[0]++;
		for(int i=0;i<1;i++)
		{
		trans(ts1.buffer[i]);
		}*/
	}
	return FALSE;
}





void cardReaderInterrupt(){
	bool result = getCard(cardBuffer);
	isNewCard |= result;
	if(!isMaintenance()){
		uiPowerTicks = 0;
		isNewCard &= (uiState == SET_POSTPAY || uiState == FINAL_PREPAY);
		if(isNewCard){
			setUIState();
			turnOnUIPower();
			
			cardBuffer[79] = 0;
			aes128_ctx_t ctx;
			aes128_init(parkingMeterKey, &ctx);
			for(uint8_t i = 0; i < 5; i++){
				aes128_enc(cardBuffer+(i*16), &ctx);
			}
		}
	}
}

volatile uint8_t previousC = 0xFF;
//volatile bool testState = 0;
ISR(PCINT1_vect)
{
	cli();
	//rfSend("ISR\n",4);
	
	//This is also for the button interrupt (since we can only have one interrupt).
	uint8_t pinC = PINC;
	uint8_t deltaC = previousC ^ pinC;
	
	/*if(testState){
		testState = FALSE;
		PORTD |= 1<<6;
	}else{
		testState = TRUE;
		PORTD &= ~(1<<6);
	}*/
	
	if(uiPowerState == 1){
		if((deltaC&1) != 0){
			if((pinC&1) == 0){
				cardReaderInterrupt();
			}
		}
	}
	
	if((deltaC&0x38) != 0){
		_delay_us(1000);
		uint8_t buttons = (PINC>>3)&7;
		uint8_t deltaButtons = ((previousC>>3)&7) ^ buttons;
		buttonInterrupt(buttons,deltaButtons);
	}
	
	previousC = pinC;
	
	sei();
}


void cardReaderInit(){
	//turnOnUIPower();
	
	PCMSK1 |= 0x01;
	PCICR |= (1<<PCIE1);
	PORTC |= 0x07; //Pull up resistor
}

void cardReaderOff(){
	PORTC &= ~(0x07);
}