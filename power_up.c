#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "power_up.h"
#include "radio.h"
#include "avr/interrupt.h"
#include "timer.h"
#include "outOfCourseController.h"
#include "drivecontrol.h"

uint8_t bigRoombaActive = 0;
uint8_t mushroomActive = 0;

/**
 * Used for counting which state we have when showing the randomize
 * symbol on the display
 */
int16_t powerUpDisplayCounter = -1;
uint16_t tickCountRandGlobal = 1;

/**
 * \brief Get an random power up and show corresponding information 
 * 			on display
 * 
 * \param tickCountRand the tickCount of a wheel, used for initializing
 * 			the random generator
 */
void getPowerUp(uint16_t tickCountRand) {
	if (currentPowerUp != NO_POWERUP) {
		return;
	}
	
	tickCountRandGlobal = tickCountRand;
	
	char result[4] = {'P', 'O', 'W', 'R'};
	set_Display(result);
	
	my_msleep(500);
	
	powerUpDisplayCounter = 0;
	showRandomizeSign();
}

/**
 * \brief Perform actions corresponding to shoot a specific power up
 */
void shootPowerUp() {
	if (currentPowerUp == NO_POWERUP) {
		return;
	}
	
	sendString("Shoot...\r\n");
	
	// Make shooting sound
	playSong(0);
	
	// If we have a red tank --> send shooting over radio
	if (currentPowerUp == RED_TANK) {
		sendRadio(RED_TANK_SHOT);
	} else {		
		if (currentPowerUp == MUSHROOM) {
			mushroomActive = 1;
		} else if (currentPowerUp == BIG_DADY) {
			bigRoombaActive = 1;
		}
		
		// If Big roomba or mushroom is active --> set global variable
		// Initialize timer so we can use the power up for a nr. of seconds
		startTimer1(5);
	}
	
	// Delete display
	clear_Display();
	
	currentPowerUp = NO_POWERUP;
}

/**
 * \brief Called from the timer, when 
 * 		the time for a power up to be active is over
 */
void powerUpIsOver() {
	sendString("Power up is over...");
	
	// When we had a mushroom, check if we are outside the course
	if (mushroomActive == 1 && outsideCourse == 1) {
		// Game over
		playSong(3);
		
		drive_stop();
		while(1);
	}
	
	bigRoombaActive = 0;
	mushroomActive = 0;
}

/**
 * \brief Used for setting the display,
 * 			when power up is generated and steering must not be blocked
 * 			and to finish the process of getting a power up
 */
void showRandomizeSign() {
	if (powerUpDisplayCounter == -1) {
		return;
	}
	
	my_msleep(20);
	
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
	if (powerUpDisplayCounter > 16*5) {
		// Get random nr.
		srand((unsigned int) tickCountRandGlobal);  
		
		powerUp_type powerup_type = rand() % 3;
		
		// For testing
		//powerUp_type powerup_type = MUSHROOM;
		
		// Display what power up we have
		uint8_t tank[4] = {0x00, 0x5c, 0x40, 0x00};
		uint8_t mushroom[4] = {0x61, 0x1D, 0x43, 0x00};
		uint8_t bigDaddy[4] = {0x5c, 0xff, 0x40, 0x00};
		 
		uint8_t *pUSymbols[3] = {tank, mushroom, bigDaddy};

		set_Display_raw(pUSymbols[powerup_type]);
		
		currentPowerUp = powerup_type;
		
		powerUpDisplayCounter = -1;
	}
}
