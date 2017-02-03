/*
 * Globals.h
 *
 * Created: 2/7/2014 4:09:29 PM
 *  Author: lopeztel
 */ 


#ifndef GLOBALS_H_
#define GLOBALS_H_

#define NO_CAR	0
#define LEGAL_PARK 2
#define CARD_DECLINED 1

#define SET_POSTPAY 0
#define SET_PREPAY 1
#define FINAL_PREPAY 2
#define PREPAID 3
#define POSTPAID 4
#define WAITING_TO_SEND_CARD_PREPAY 5
#define WAITING_TO_SEND_CARD_POSTPAY 6
#define PREPAY_ERROR 7
#define POSTPAY_ERROR 8
#define WAITING_PREPAY 9
#define WAITING_POSTPAY 10

extern uint32_t parkingMeterID;
extern uint8_t parkingMeterKey[16];
extern volatile int lastCommand;
extern volatile uint8_t cardBuffer[80];
extern volatile bool isNewCard;
extern volatile uint8_t brightness;
extern volatile uint8_t meterStatus;
extern volatile uint8_t isCarParked;
extern volatile uint8_t isPrepay;
extern volatile uint8_t prePayAmount;
extern volatile uint8_t priceDollars;
extern volatile uint8_t priceCents;
extern volatile uint8_t priceDollarsNext;
extern volatile uint8_t priceCentsNext;
extern volatile uint8_t timeHours;
extern volatile uint8_t timeMinutes;
extern volatile uint8_t uiPowerTicks;
extern volatile uint8_t ledPowerTicks;
extern volatile uint8_t uiState;
extern volatile uint8_t uiPowerState;
extern volatile EEMEM uint8_t eeAddress[5];
extern volatile uint8_t prepayTime;
extern volatile uint8_t isBroken;
extern volatile uint8_t distanceHistory[3];
extern volatile uint8_t ultrasonicCounter;
#endif /* GLOBALS_H_ */