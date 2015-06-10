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
 * TODO
 */
powerUp_type getPowerUp(uint16_t tickCountRand) {
	if (currentPowerUp != NO_POWERUP) {
		return currentPowerUp;
	}
	
	char result[4] = {'P', 'O', 'W', 'R'};
	set_Display(result);
	
	my_msleep(500);
	
	// TODO nicer symbols and not blocking
	// Show randomize signal on Display
	/*for (int a = 0; a < 12; a++) {
		result[0] = ' ';
		result[1] = ' ';
		result[2] = ' ';
		result[3] = ' ';
		
		result[a % 4] = 'O';
		
		set_Display(result);
		
		my_msleep(250);
	}*/
	
	// Get random nr.
    srand((unsigned int) tickCountRand);  
    
    //powerUp_type powerup_type = rand() % 3;
    
    // For testing
    powerUp_type powerup_type = MUSHROOM;
    
    // Display what power up we have
    uint8_t tank[4] = {0x00, 0x5c, 0x40, 0x00};
    uint8_t mushroom[4] = {0x61, 0x1D, 0x43, 0x00};
    uint8_t bigDaddy[4] = {0x5c, 0xff, 0x40, 0x00};
     
    uint8_t *pUSymbols[3] = {tank, mushroom, bigDaddy};

    set_Display_raw(pUSymbols[powerup_type]);
    
    currentPowerUp = powerup_type;
    
    return powerup_type;
}

/**
 * TODO
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
			
			// TODO probably we will disable steering / accelerating / etc. 
			//so that we have a defined position when driving off the road
		} else if (currentPowerUp == BIG_DADY) {
			bigRoombaActive = 1;
		}
		
		// If Big roomba or mushroom is active --> set global variable
		// Initialize timer so we can use the power up for a nr. of seconds
		initializeTimer1(5);
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

