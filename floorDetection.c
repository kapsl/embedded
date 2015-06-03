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
void floorDetection(detectedType *type, uint16_t *sensorData) {
	int16_t cliff_front_left = sensorData[0];
	int16_t cliff_front_right = sensorData[1];
	
	*type = NO_TYPE;
	
	// Power up
	if (cliff_front_left + cliff_front_right > POWER_UP_CLIFF_THRESHOLD) {
		*type = POWER_UP;
		
		return;
	}
	
	int16_t cliff_left = sensorData[2];
	int16_t cliff_right = sensorData[3];
	
	// Gelb ca. 2757
	// Teppich ca. 592
	// Schwarz ca. 860 --> evtl. schwarz unterscheiden
	// Braun ca. 2664
	
	char result[4];
	intToHex(cliff_left, result);
	set_Display(result);
	my_msleep(1000);
	
	// Left border
	if (cliff_left > BORDER_CLIFF_THRESHOLD) {
		*type = BORDER_LEFT;
	} 
	
	// Right border
	if (cliff_right > BORDER_CLIFF_THRESHOLD) {
		// Both borders
		if (*type == BORDER_LEFT) {
			*type = BORDER_BOTH;	
		} else {
			*type = BORDER_RIGHT;
		}
	}
}
