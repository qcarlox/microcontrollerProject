/*
 * lcdLibrary.h
 *
 * Created: 2/6/2014 1:49:15 PM
 *  Author: lopeztel
 */ 


#ifndef LCDLIBRARY_H_
#define LCDLIBRARY_H_




void lcdCommand(uint8_t cmnd);
 
void lcdData(uint8_t data);

void lcdInit();

void lcdGoToY(uint8_t y);

void lcdPrint(uint8_t* str,  uint8_t strLength);

void lcdUpdate();
void getTotalPrice(uint8_t*dollars,uint8_t*cents,uint16_t time);

void getTotalTime(uint8_t*hours,uint8_t*minutes);

#endif /* LCDLIBRARY_H_ */