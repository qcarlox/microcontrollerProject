/*
 * CardReaderLibrary.h
 *
 * Created: 1/31/2014 3:19:15 PM
 *  Author: hillp8
 */ 


#ifndef CARDREADERLIBRARY_H_
#define CARDREADERLIBRARY_H_



/*
 * CardReaderLibrary.c
 *
 * Created: 1/31/2014 3:19:04 PM
 *  Author: hillp8
 */ 

#include "FullLibrary.h"


bool getCard();

void cardReaderInterrupt();

void cardReaderInit();
void cardReaderOff();

#endif /* CARDREADERLIBRARY_H_ */