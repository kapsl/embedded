#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "power_up.h"
#include "radio.h"

powerUp_type getPowerUp(uint16_t tickCountRand) {
	char result[4] = {'P', 'O', 'W', 'R'};
	set_Display(result);
	
	my_msleep(500);
	
	// TODO nicer symbols and not blocking
	// Show randomize signal on Display
	for (int a = 0; a < 12; a++) {
		result[0] = ' ';
		result[1] = ' ';
		result[2] = ' ';
		result[3] = ' ';
		
		result[a % 4] = 'O';
		
		set_Display(result);
		
		my_msleep(250);
	}
	
	// Get random nr.
    srand((unsigned int) tickCountRand);  
    
    powerUp_type powerup_type = rand() % 3 + 1;
    
    // Display what power up we have
    uint8_t tank[4] = {0x00, 0x5c, 0x40, 0x00};
    uint8_t bigTank[4] = {0x5c, 0xff, 0x40, 0x00};
    uint8_t mushroom[4] = {0x61, 0x1D, 0x43, 0x00};
     
    uint8_t *pUSymbols[3] = {tank, bigTank, mushroom};

    set_Display_raw(pUSymbols[powerup_type - 1]);
    
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
	
	// Make shooting sound
	playSong(0);
	
	// If we have a red tank --> send shooting over radio
	if (currentPowerUp == RED_TANK) {
		sendRadio(RED_TANK_SHOT);
	}
	
	// TODO boost set global variable
	// TODO safe for red tank set global var
	
	// Delete display
	clear_Display();
}
