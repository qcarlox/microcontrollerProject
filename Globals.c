/*
 * Globals.c
 *
 * Created: 2/7/2014 4:09:14 PM
 *  Author: lopeztel
 */ 
#include "FullLibrary.h"

uint32_t parkingMeterID = 1;
uint8_t parkingMeterKey[] = {39,30,87,56,70,41,87,6,35,33,89,53,27,29,31,97};

volatile int lastCommand = 0;
volatile uint8_t cardBuffer[80];
volatile bool isNewCard = 0;
volatile uint8_t brightness = 0;
volatile uint8_t meterStatus = 0;
volatile uint8_t isCarParked = 0;
volatile uint8_t isPrepay = 0;
volatile uint8_t prePayAmount = 0;
volatile uint8_t priceDollars = 0;
volatile uint8_t priceCents = 0;
volatile uint8_t priceDollarsNext = 0;
volatile uint8_t priceCentsNext = 0;
volatile uint8_t timeHours = 0;
volatile uint8_t timeMinutes = 0;
volatile uint8_t uiPowerTicks = 0;
volatile uint8_t ledPowerTicks = 0;
volatile uint8_t uiState = 0;
volatile uint8_t uiPowerState = 0;
volatile uint8_t prepayTime = 0;
volatile uint8_t isBroken = 0;
volatile uint8_t distanceHistory[3] = {110,110,110};
volatile EEMEM uint8_t eeAddress[5];
volatile uint8_t ultrasonicCounter = 0;
