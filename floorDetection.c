#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "floorDetection.h"

/**
 * Possible return values: 
 * 	power up detected
 * 	left border detected with angle
 * 	right border detected with angle
 */
void floorDetection(detectedType *type) {
	// Cliff front left, front right, left right 
	uint8_t packet_ids[4] = {29, 30, 28, 31};
	uint8_t packet_length[4] = {2, 2, 2, 2};
	uint16_t qdata[4];
	
	getSensorQueryList(4, packet_ids, packet_length, qdata);
	
	int16_t cliff_front_left = qdata[0];
	int16_t cliff_front_right = qdata[1];
	
	// Power up
	if (cliff_front_left + cliff_front_right > POWER_UP_CLIFF_THRESHOLD) {
		*type = POWER_UP;
		
		return;
	}
	
	int16_t cliff_left = qdata[2];
	int16_t cliff_right = qdata[3];
	
	// Left border
	if (cliff_left > BORDER_CLIFF_THRESHOLD) {
		// Left sensor is on border
		//char val[4] = {'l', 'e', 'f', 't'};
		char result[4];
		intToHex(cliff_left, result);
		set_Display(result);
		
		*type = BORDER_LEFT;
		
		return;
	} else if (cliff_right > BORDER_CLIFF_THRESHOLD) {
		char result[4];
		intToHex(cliff_left, result);
		set_Display(result);
		
		*type = BORDER_RIGHT;
		
		return;
	}
}
