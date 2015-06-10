#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "floorDetection.h"

int16_t cliff_front_both_past = 0;

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
	
	int16_t cliff_left = sensorData[2];
	int16_t cliff_right = sensorData[3];
	
	int16_t wall_distance = sensorData[7];
	
	/*char result[4];
	intToHex(wall_distance, result);
	set_Display(result);
	my_msleep(200);*/
	
	// Power up
	if (wall_distance > WALL_SIGNAL_THRESHOLD) {
		*type = POWER_UP;
		
		return;
	}
	
	// Gelb ca. 2757
	// Teppich ca. 592
	// Schwarz ca. 860 --> evtl. schwarz unterscheiden
	// Braun ca. 2664
	
	
	
	if (cliff_front_left + cliff_front_right > BORDER_BOTH_THRESHOLD) {
		*type = BORDER_BOTH;
	} else if (cliff_left + cliff_right > BORDER_BOTH_THRESHOLD) {
		*type = BORDER_SIDE_BOTH;
	} 
	
	else if (cliff_front_left > BORDER_CLIFF_THRESHOLD) {
		*type = BORDER_FRONT_LEFT;
	} else if (cliff_front_right > BORDER_CLIFF_THRESHOLD) {
		*type = BORDER_FRONT_RIGHT;
	}
	
	else if (cliff_left > BORDER_CLIFF_THRESHOLD) {
		*type = BORDER_LEFT;
	} else if (cliff_right > BORDER_CLIFF_THRESHOLD) {
		*type = BORDER_RIGHT;
	}
	
	cliff_front_both_past = cliff_front_left + cliff_front_right;
}
