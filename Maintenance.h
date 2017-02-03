/*
 * Maintenance.h
 *
 * Created: 2/7/2014 3:05:27 PM
 *  Author: lopeztel
 */ 


#ifndef MAINTENANCE_H_
#define MAINTENANCE_H_
/*
 * Maintenace.c
 *
 * Created: 2/7/2014 3:05:13 PM
 *  Author: lopeztel
 */ 
#include "FullLibrary.h"



void maintenanceInit();


void maintenanceMode();

int isMaintenance();

void testUltrasonic();

void testCardReader();
void testRf();
void displayID();
void hexToAscii(uint8_t * hex,uint8_t hexLength,uint8_t* ascii);
void assignID();

#endif /* MAINTENANCE_H_ */