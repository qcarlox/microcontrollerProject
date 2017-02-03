/*
 * ButtonLibrary.h
 *
 * Created: 2/4/2014 1:24:15 PM
 *  Author: hillp8
 */ 


#ifndef BUTTONLIBRARY_H_
#define BUTTONLIBRARY_H_

void buttonInterrupt(uint8_t deltaButton, uint8_t currentButton);

void buttonInit();
void setUIState();
void setUIStateRF();

#endif /* BUTTONLIBRARY_H_ */