#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "power_up.h"

powerUp_type getPowerUp(uint16_t tickCountRand) {
	char result[4] = {'P', 'O', 'W', 'R'};
	set_Display(result);
	
	my_msleep(500);
	
	// Show randomize signal on Display
	//char result[4];
	
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
    
    // TODO Display what power up we have
    
    return powerup_type;
}
