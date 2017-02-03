/*
 * USARTLibrary.h
 *
 * Created: 1/24/2014 12:37:08 PM
 *  Author: hillp8
 */ 


#ifndef USARTLIBRARY_H_
#define USARTLIBRARY_H_


uint8_t didSerialTimeout();

uint8_t serialCheckRxComplete(void);

uint8_t serialRead();

uint8_t serialReadTimeout();

void usartSend( uint8_t ch);

void usartSendString(uint8_t* str,  uint8_t strLength);

void usartSendNullTerminatedString(uint8_t* str);

void usartInit();



#endif /* USARTLIBRARY_H_ */