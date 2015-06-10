#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "floorDetection.h"

/**
 *  \brief Possible return values: 
 * 	power up detected
 * 	left border detected with angle
 * 	right border detected with angle
 * 
 * 	\param type TODO
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
	
	/*char result[4];
	intToHex(cliff_left, result);
	set_Display(result);*/
	//my_msleep(1000);
	
	// Yellow, black crep
	// Left border
	if (cliff_left > BORDER_CLIFF_THRESHOLD) {
		*type = BORDER_LEFT;
	} else if (cliff_left > BORDER_CLIFF_BLACK_THRESHOLD) {
		*type = BORDER_LEFT_BLACK;
	}
	
	// Right border
	if (cliff_right > BORDER_CLIFF_THRESHOLD) {
		// Both borders
		if (*type == BORDER_LEFT) {
			*type = BORDER_BOTH;	
		} else {
			*type = BORDER_RIGHT;
		}
	} else if (cliff_right > BORDER_CLIFF_BLACK_THRESHOLD) {
		// Both borders
		if (*type == BORDER_LEFT_BLACK) {
			*type = BORDER_BOTH_BLACK;	
		} else {
			*type = BORDER_RIGHT_BLACK;
		}
	}
}
