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
	
	// Power up
	if (cliff_front_left + cliff_front_right > POWER_UP_CLIFF_THRESHOLD) {
		*type = POWER_UP;
		
		return;
	}
	
	int16_t cliff_left = sensorData[2];
	int16_t cliff_right = sensorData[3];
	
	// Left border
	if (cliff_left > BORDER_CLIFF_THRESHOLD) {
		// Left sensor is on border
		/*char result[4];
		intToHex(cliff_left, result);
		set_Display(result);*/
		
		*type = BORDER_LEFT;
		
		return;
	} else if (cliff_right > BORDER_CLIFF_THRESHOLD) {
		/*char result[4];
		intToHex(cliff_left, result);
		set_Display(result);*/
		
		*type = BORDER_RIGHT;
		
		return;
	}
	
	*type = NO_TYPE;
}
