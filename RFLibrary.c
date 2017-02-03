/*
 * RFLibrary.h
 *
 * Created: 1/24/2014 12:38:17 PM
 *  Author: hillp8
 */ 


#include "FullLibrary.h"

#define MOSI	3					//PB3
#define SCK		5					//PB5
#define SS		2					//PB2,CSN
#define MISO	4					//PB4
#define CE		1					//PB1, if NRF24L01 is acting as receiver 1 makes 24L monitor air, low puts NRF24L01 in standby
//if NRF24L01 acting as transmitter, CE always low except when user wants to transmit a packet,
//to transmit load TXFIFO then toggle CE from low to high (leave high for at least 10us, then switch to low.



//NRF24L01 instructions////////////////////////////////
#define R_REG		0x00			//format R_REG|(ADDR&MASK)
#define W_REG		0x20			//format W_REG|(ADDR&MASK)
#define MASK		0x1F
#define R_RX		0x61
#define W_TX		0xA0
#define FLUSH_RX	0xE1
#define FLUSH_TX	0xE2
#define RE_TX		0xE3
#define NOP			0xFF


//NRF24L01 Registers////////////////////////////////
#define CONFIG			0x00
#define EN_AA			0x01
#define EN_RXADDR		0x02
#define SETUP_AW		0x03
#define SETUP_RETR		0x04
#define RF_CH			0x05
#define RF_SETUP		0x06
#define STATUS			0x07
#define OBSERVE_TX		0x08
#define CD				0x09
#define RX_ADDR_P0		0x0A
#define RX_ADDR_P1		0x0B
#define RX_ADDR_P2		0x0C
#define RX_ADDR_P3		0x0D
#define RX_ADDR_P4		0x0E
#define RX_ADDR_P5		0x0F
#define TX_ADDR			0x10
#define RX_PW_P0		0x11
#define RX_PW_P1		0x12
#define RX_PW_P2		0x13
#define RX_PW_P3		0x14
#define RX_PW_P4		0x15
#define RX_PW_P5		0x16
#define FIFO_STATUS		0x17


//NRF24L01 register bits
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0

#define CHANNEL     2		//transmission channel
#define TEMP		85		//threshold temperature


//
//this function takes a command and returns one byte of data from the NRF24L01
//can be used to read registers of the NRF24L01
uint8_t read_onebyte( uint8_t cmd)
{
	_delay_us(10);
	PORTB &= ~(1<<SS);								//initialize packet
	_delay_us(10);
	
	SPDR = cmd;										//send command
	while( !(SPSR & (1<<SPIF)));
	
	SPDR = 0xFF;									//send data
	while( !(SPSR & (1<<SPIF)));
	
	_delay_us(10);
	PORTB |= (1<<SS);								//terminate packet
	_delay_us(10);
	return SPDR;
}
void read_payload( uint8_t cmd, uint8_t * data)
{
	_delay_us(10);
	PORTB &= ~(1<<SS);								//initialize packet
	_delay_us(10);
	
	SPDR = cmd;										//send command
	while( !(SPSR & (1<<SPIF)));
	
	for(uint8_t i=0;i<32;i++)
	{
		SPDR = 0xFF;									//send data
		while( !(SPSR & (1<<SPIF)));
		data[i] = SPDR;
	}
	
	_delay_us(10);
	PORTB |= (1<<SS);								//terminate packet
	_delay_us(10);
	
	
}


//
//this function writes one byte into a register of the NRF24L01
void write_onebyte( uint8_t cmd, uint8_t data )
{
	_delay_us(10);
	PORTB &= ~(1<<SS);								//initialize packet
	_delay_us(10);
	
	SPDR = cmd;										//send command
	while( !(SPSR & (1<<SPIF)));
	
	SPDR = data;									//send data
	while( !(SPSR & (1<<SPIF)));
	
	_delay_us(10);
	PORTB |= (1<<SS);								//terminate packet
	_delay_us(10);									//terminate packet
}

void write_payload(uint8_t cmd, uint8_t * data)
{
	_delay_us(10);
	PORTB &= ~(1<<SS);								//initialize packet
	_delay_us(10);
	
	SPDR = cmd;										//send command
	while( !(SPSR & (1<<SPIF)));
	
	for(uint8_t i=0;i<32;i++)
	{
		SPDR = data[i];									//send data
		while( !(SPSR & (1<<SPIF)));
	}
	
	_delay_us(10);
	PORTB |= (1<<SS);								//terminate packet
	_delay_us(10);									//terminate packet
}

void write_data(uint8_t cmd, uint8_t* data, uint8_t length)
{
	_delay_us(10);
	PORTB &= ~(1<<SS);								//initialize packet
	_delay_us(10);
	
	SPDR = cmd;										//send command
	while( !(SPSR & (1<<SPIF)));
	
	for(uint8_t i=0; i<length; i++)
	{
		SPDR = data[i];									//send data
		while( !(SPSR & (1<<SPIF)));
	}
	
	_delay_us(10);
	PORTB |= (1<<SS);								//terminate packet
	_delay_us(10);									//terminate packet
}
void setAddress(uint8_t* address){
	write_data(W_REG|(MASK&RX_ADDR_P0),address,5);
	write_data(W_REG|(MASK&TX_ADDR),address,5);
}

//this function executes command of the NRF24L01 that doesn't send back data
uint8_t commandret(uint8_t cmd)
{
	_delay_us(10);
	PORTB &= ~(1<<SS);
	_delay_us(10);									//initialize packet
	
	SPDR = cmd;
	while( !(SPSR & (1<<SPIF)));
	
	_delay_us(10);
	PORTB |= (1<<SS);								//terminate packet
	_delay_us(10);
	return SPDR;
}

//this function executes command of the NRF24L01 that doesn't send back data and returns the STATUS register
void command(uint8_t cmd)
{
	_delay_us(10);
	PORTB &= ~(1<<SS);
	_delay_us(10);									//initialize packet
	
	SPDR = cmd;
	while( !(SPSR & (1<<SPIF)));
	
	_delay_us(10);
	PORTB |= (1<<SS);								//terminate packet
	_delay_us(10);
}



//this function puts the NRF24L01 into RX mode scans for 1 sec
void rec()
{
	PORTB &= ~(1<<CE);							//make sure CE is low
	write_onebyte(W_REG|(MASK&CONFIG),0x0F);	//NRF24L01 and set RX, power on, CRC 2 bytes, EN_CRC
	write_onebyte(W_REG|(MASK&STATUS),0x70);	//clear NRF24L01 interrupts
	command(FLUSH_RX);							//flush NRF24L01 RX pipeline
	command(FLUSH_TX);							//flush NRF24L01 TX pipeline
	_delay_us(150);
	
	PORTB |= (1<<CE);							//set CE to scan for messages
	_delay_ms(10);
	PORTB &= ~(1<<CE);							//clear CE to stop scanning
}


//===========================================================
//Public interface below
//===========================================================


//Initializes the RF module.  This requires PB1 to PB5 on the ATMEGA328p, so these
//pins must not be in use by something else in order to use the RF library.  This
//function only needs to be called once after the RF module is powered on.
void rfInit(){
	
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS)|(1<<CE);	//set MOSI, SCK, SS, CE, and OC1A to output
	PORTB |= (1<<SS);									//set SS
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);				//enable SPI as master
	_delay_ms(30);										//wait for NRF24L01 to power up
	write_onebyte(W_REG|(MASK&RF_CH),CHANNEL);			//set NRF24L01 transmission channel
	write_onebyte(W_REG|(MASK&SETUP_RETR),0x0F);		//set NRF24L01 auto retransmission max retries and delay between retries
	write_onebyte(W_REG|(MASK&RX_PW_P0),0x20);			//RX P0 payload length
}

//Sends the given message using the RF module.  The returned value denotes the
//RF result message (e.g. RF_SUCCESS, RF_TIMEOUT, ...)
uint8_t rfSend(uint8_t* message, uint8_t messageLength)
{
	
	//If there is no message, don't do anything on the RF.
	if(messageLength == 0){
		return RF_ILLEGAL_MESSAGE_SIZE;
	}
	
	uint8_t buffer[32];
	uint8_t numberOfPackets = (messageLength+MESSAGE_PAYLOAD_SIZE-1)/MESSAGE_PAYLOAD_SIZE; //Number of packets for the message
	buffer[1] = numberOfPackets;
	buffer[2] = MESSAGE_PAYLOAD_SIZE-messageLength%MESSAGE_PAYLOAD_SIZE; //Number of padding bytes in the last packet for the message
	
	uint8_t* messagePointer = message;
	for(uint8_t i = 0; i < numberOfPackets; i++)
	{
		//Copy next part of the message to the buffer.
		buffer[0] = i;
		for(uint8_t j = 0; j < 29; j++){
			buffer[j+3] = messagePointer[j];
		}
		//memcpy(buffer+3,messagePointer,29);
		messagePointer += 29;
		
		uint8_t status;
		uint8_t retryCount = 0;
		do{
			if(retryCount > 3){
				return RF_TIMEOUT;
			}
			PORTB &= ~(1<<CE);										//make sure CE is low
			write_onebyte(W_REG|(MASK&CONFIG),0x0E);				//NRF24L01 and set TX, power on, CRC 2 bytes, EN_CRC
			write_onebyte(W_REG|(MASK&STATUS),0x70);				//clear NRF24L01 interrupts
			command(FLUSH_TX);										//flush NRF24L01 TX pipeline
			command(FLUSH_RX);										//flush NRF24L01 RX pipeline
			_delay_us(150);
			
			
			write_payload(W_TX,buffer);								//write TX payload/message
			_delay_ms(10);
			PORTB |= (1<<CE);										//toggle CE to send TX payload/message
			_delay_us(20);
			PORTB &= ~(1<<CE);
			
			while(!((status = commandret(NOP))&((1<<TX_DS)|(1<<MAX_RT))));		//wait for ACK or MAX retry
			retryCount++;
		}while(!(status&(1<<TX_DS)));
		
	}
	return RF_SUCCESS;
}


//Reads a message from the RF module.  The length of the message will be written
//to dstLength.  The input value of dstLength should denote the size of the dst
//buffer.  If the message buffer is smaller than the message, an error will be
//returned.  The returned value denotes the RF result message.
uint8_t rfRead(uint8_t* dst, uint8_t* dstLength){		
	uint8_t buffer[32];
	uint8_t packetNumber = 0;
	uint8_t numberOfPackets = 0;
	uint8_t paddingBytes = 0;
	uint8_t state = 0;
	uint8_t i;
	
	while(1){
		for(i=0;i<3;i++){
			rec();
			if(commandret(NOP)&(1<<RX_DR)){
				read_payload(R_RX,buffer);
				break;
			}
		}
		if(i==3){
			return RF_TIMEOUT;
		}
		
		switch(state){
			//State for the first packet.
			case 0:{
				//Get header information.
				packetNumber = buffer[0];
				numberOfPackets = buffer[1];
				paddingBytes = buffer[2]&0x1F;
				
				//Check to make sure that the number of padding bytes is less
				//than 29.  If it is greater than 28, we have an entire packet
				//of only padding.
				if(paddingBytes > 28){
					return RF_ILLEGAL_PADDING_SIZE;
				}
				
				uint8_t paddedMessageSize = numberOfPackets*29;
				
				//Check to make sure that the message size is greater than 0.
				if(paddedMessageSize <= paddingBytes){
					return RF_ILLEGAL_MESSAGE_SIZE;
				}
				uint8_t messageSize = paddedMessageSize-paddingBytes;
				
				//Check for buffer overflow immediately.
				if(messageSize > *dstLength){
					return RF_BUFFER_OVERFLOW;
				}
				*dstLength = messageSize;
				
				//The first packet must be number 0.
				if(packetNumber != 0){
					return RF_INCORRECT_PACKET_NUMBER;
				}
				
				//There must be at least one packet (the one in the buffer).
				if(numberOfPackets == 0){
					return RF_NO_PACKETS;
				}
			
				//Move to the next state.
				state = 1;
			}break;
			
			//State for additional packets.
			case 1:{
				//Verify we found the next packet.
				if(packetNumber+1 != buffer[0]){
					return RF_INCORRECT_PACKET_NUMBER;
				}
				packetNumber = buffer[0];
			}break;
		}
		
		//This should never occur, since the packets must be in order and
		//we exit the loop before this condition should ever occur.  This
		//check is only here to make sure something didn't go completely
		//wrong in the previous steps.
		if(packetNumber >= numberOfPackets){
			return RF_INCORRECT_PACKET_NUMBER;
		}
		
		//Append the buffer to the message and check for the end of the
		//message.
		if(packetNumber+1 == numberOfPackets){
			uint8_t n = 29-paddingBytes;
			for(uint8_t j = 0; j < n; j++){
				dst[j] = buffer[j+3];
			}
			//memcpy(dst,buffer+3,29-paddingBytes);
			break;
		}else{
			for(uint8_t j = 0; j < 29; j++){
				dst[j] = buffer[j+3];
			}
			//memcpy(dst,buffer+3,29);
			dst += 29;
		}
	}
	return RF_SUCCESS;
	
}

void setAddressFromEEPROM(){
	uint8_t buffer[5];
	eeprom_read_block((void*)buffer,(void*)eeAddress,5);
	setAddress(buffer);
}
