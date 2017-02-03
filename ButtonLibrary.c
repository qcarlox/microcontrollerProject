/*
 * ButtonLibrary.c
 *
 * Created: 2/4/2014 1:23:51 PM
 *  Author: hillp8
 */ 
#include "FullLibrary.h"

void buttonInterrupt(uint8_t currentButton, uint8_t deltaButton){
	uint8_t fallingEdge = deltaButton & ~currentButton;
	
	if((fallingEdge&1)!=0){
		//rfSend((uint8_t*)"Down\n",5);
		lastCommand = DOWN;
	}
	if((fallingEdge&2)!=0){
		//rfSend((uint8_t*)"Enter\n",6);
		lastCommand = ENTER;
	}
	if((fallingEdge&4)!=0){
		//rfSend((uint8_t*)"Up\n",3);
		lastCommand = UP;
	}
	
	if(!isMaintenance()){
		uiPowerTicks = 0;
		if(!isPower()){
			lastCommand = 0;
		}
		
		setUIState();
		turnOnUIPower();
	}
}

void buttonInit(){
	PCMSK1 |= 0x38;
	PCICR |= (1<<PCIE1);
	PORTC |= 0x38;
}

void setUIStateRF(){
	if(uiState == WAITING_PREPAY){
		if(meterStatus == LEGAL_PARK){
			prepayTime = 0;
			uiState = PREPAID;
		}
		else if(meterStatus == CARD_DECLINED){
			uiState = PREPAY_ERROR;
		}
	}
	else if(uiState == WAITING_POSTPAY){
		if(meterStatus == LEGAL_PARK){
			prepayTime = 0;
			uiState = POSTPAID;
		}else if(meterStatus == CARD_DECLINED){
			uiState = POSTPAY_ERROR;
		}
	}
	else if(uiState == PREPAID){
		if(meterStatus != LEGAL_PARK){
			uiState = SET_POSTPAY;
		}
	}
	else if(uiState == POSTPAID){
		if(meterStatus != LEGAL_PARK){
			uiState = SET_POSTPAY;
		}
	}
	else if(uiState == WAITING_TO_SEND_CARD_POSTPAY){
		uiState = WAITING_POSTPAY;
	}
	else if(uiState == WAITING_TO_SEND_CARD_PREPAY){
		uiState = WAITING_PREPAY;
	}
}

void setUIState(){
	
	if(uiState==SET_POSTPAY){
		if(lastCommand==ENTER){
			uiState = SET_PREPAY;
		}
		else if(isNewCard){
			isPrepay = FALSE;
			uiState = WAITING_TO_SEND_CARD_POSTPAY;
		}
	}
	else if(uiState==SET_PREPAY){
		if(lastCommand == ENTER){
			uiState = FINAL_PREPAY;
		}
		else if(lastCommand == UP){
			prepayTime++;
		}
		else if(lastCommand == DOWN){
			prepayTime--;
		}
	}
	else if(uiState == FINAL_PREPAY){
		if(lastCommand == ENTER){
			uiState = SET_POSTPAY;
		}
		else if(isNewCard){
			isPrepay = TRUE;
			uiState = WAITING_TO_SEND_CARD_PREPAY;
		}
	}
	else if(uiState == PREPAY_ERROR){
		if(lastCommand != 0){
			uiState = FINAL_PREPAY;
		}
	}
	else if(uiState == POSTPAY_ERROR){
		if(lastCommand != 0){
			uiState = SET_POSTPAY;
		}
	}
	
	lastCommand = 0;
	
}
