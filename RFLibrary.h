/*
 * RFLibrary.h
 *
 * Created: 1/24/2014 12:38:17 PM
 *  Author: hillp8
 */ 


#ifndef RFLIBRARY_H_
#define RFLIBRARY_H_




#define MESSAGE_PAYLOAD_SIZE 29
#define RF_SUCCESS 0
#define RF_TIMEOUT 1
#define RF_BUFFER_OVERFLOW 2
#define RF_INCORRECT_PACKET_NUMBER 3
#define RF_NO_PACKETS 4
#define RF_ILLEGAL_MESSAGE_SIZE 5
#define RF_ILLEGAL_PADDING_SIZE 6


//
//this function takes a command and returns one byte of data from the NRF24L01
//can be used to read registers of the NRF24L01
uint8_t read_onebyte( uint8_t cmd);

void read_payload( uint8_t cmd, uint8_t * data);

//this function writes one byte into a register of the NRF24L01
void write_onebyte( uint8_t cmd, uint8_t data );

void write_payload(uint8_t cmd, uint8_t * data);

//this function executes command of the NRF24L01 that doesn't send back data
uint8_t commandret(uint8_t cmd);

//this function executes command of the NRF24L01 that doesn't send back data and returns the STATUS register
void command(uint8_t cmd);

//this function puts the NRF24L01 into TX mode and transmits the argument of the function
uint8_t rfSend(uint8_t* message, uint8_t messageLength);


//this function puts the NRF24L01 into RX mode scans for 1 sec
void rec();

void rfInit();

uint8_t rfRead(uint8_t* dst, uint8_t* dstLength);
void write_data(uint8_t cmd, uint8_t * data,uint8_t length);
void setAddress(uint8_t* address);
void setAddressFromEEPROM();

#endif /* RFLIBRARY_H_ */