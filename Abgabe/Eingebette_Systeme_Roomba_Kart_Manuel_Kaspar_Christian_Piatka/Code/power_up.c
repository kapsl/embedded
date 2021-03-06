#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "power_up.h"
#include "radio.h"
#include "outOfCourseController.h"
#include "drivecontrol.h"

uint8_t bigRoombaActive = 0;
uint8_t mushroomActive = 0;
int16_t powerUpDisplayCounter = POWCONST;
uint64_t powerUpTimer = 0;

/**
 * Hold the tick count of a wheel to calculate a random number
 */
uint16_t tickCountRandGlobal = 1;

void getPowerUp(uint16_t tickCountRand) {
	if (currentPowerUp != NO_POWERUP || powerUpDisplayCounter != POWCONST || bigRoombaActive || mushroomActive) {
		return;
	}
	
	tickCountRandGlobal = tickCountRand;
	
	char result[4] = {'P', 'O', 'W', 'R'};
	set_Display(result);
	
	my_msleep(500);
	
	powerUpDisplayCounter = 0;
	showRandomizeSign();
}

void shootPowerUp() {
	if (currentPowerUp == NO_POWERUP) {
		return;
	}
	
	//sendString("Shoot...\r\n");
	
	// Make shooting sound
	playSong(0);
	
	// If we have a red tank --> send shooting over radio
	if (currentPowerUp == RED_TANK) {
		sendRadio(RED_TANK_SHOT, 3);
	} else {	
		// If Big roomba or mushroom is active --> set global variable	
		if (currentPowerUp == MUSHROOM) {
			mushroomActive = 1;
		} else if (currentPowerUp == BIG_DADY) {
			bigRoombaActive = 1;
		}
		
		// Initialize timer variable so we can use the power up for a nr. of seconds
		powerUpTimer = 1;
	}
	
	// Delete display
	clear_Display();
	
	currentPowerUp = NO_POWERUP;
}

void powerUpIsOver() {
	//sendString("Power up is over...");
	
	// When we had a mushroom, check if we are outside the course
	if (mushroomActive == 1 && outsideCourse == 1) {
		// Game over
		playSong(3);
		
		drive_stop();
		
		char result[4] = {'O', 'V', 'E', 'R'};
		set_Display(result);
		
		while(1);
	}
	
	bigRoombaActive = 0;
	mushroomActive = 0;
	outsideCourse = 0;
}

void showRandomizeSign() {
	if (powerUpDisplayCounter == POWCONST) {
		return;
	}
	
	my_msleep(30);
	
	// Infinity sign in 7 segments
	uint8_t infinitySign[16][4] = {{0x10, 0x0, 0x0, 0x0}, {0x30, 0x0, 0x0, 0x0}, 
		{0x31, 0x0, 0x0, 0x0}, {0x31, 0x1, 0x0, 0x0}, {0x31, 0x3, 0x0, 0x0},
		{0x31, 0x7, 0x0, 0x0}, {0x31, 0x7, 0x8, 0x0}, {0x31, 0x7, 0x8, 0x8},
		{0x31, 0x7, 0x8, 0xC}, {0x31, 0x7, 0x8, 0xE}, {0x31, 0x7, 0x8, 0xF},
		{0x31, 0x7, 0x9, 0xF}, {0x31, 0x7, 0x9, 0xF}, {0x31, 0xF, 0x9, 0xF},
		{0x39, 0xF, 0x9, 0xF}, {0x0, 0x0, 0x0, 0x0}};
	set_Display_raw(infinitySign[powerUpDisplayCounter % 16]);
	
	powerUpDisplayCounter++;
	
	// Stop when we reached array length
	if (powerUpDisplayCounter > 16*2) {
		// Get random nr.
		srand((unsigned int) tickCountRandGlobal);  
		
		powerUp_type powerup_type = rand() % 3;
		
		// For testing
		//powerup_type = MUSHROOM;
		
		// Display what power up we have
		uint8_t tank[4] = {0x00, 0x5c, 0x40, 0x00};
		uint8_t mushroom[4] = {0x61, 0x1D, 0x43, 0x00};
		uint8_t bigDaddy[4] = {0x5c, 0xff, 0x40, 0x00};
		 
		uint8_t *pUSymbols[3] = {tank, mushroom, bigDaddy};

		set_Display_raw(pUSymbols[powerup_type]);
		
		currentPowerUp = powerup_type;
		
		powerUpDisplayCounter = POWCONST;
	}
}

void handleTimerVariable() {
	if (powerUpTimer == 0)
		return;
		
	if (powerUpTimer == POWER_UP_TIME) {
		powerUpIsOver();
		
		powerUpTimer = 0;
		
		return;
	}
	
	powerUpTimer++;
}
