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
	
	// Power up
	if (cliff_front_left + cliff_front_right > POWER_UP_CLIFF_THRESHOLD) {
		*type = POWER_UP;
		
		return;
	}
	
	// Gelb ca. 2757
	// Teppich ca. 592
	// Schwarz ca. 860 --> evtl. schwarz unterscheiden
	// Braun ca. 2664
	
	/*char result[4];
	intToHex(cliff_left, result);
	set_Display(result);*/
	//my_msleep(1000);
	
	if (cliff_front_left + cliff_front_right > BORDER_BOTH_THRESHOLD && cliff_front_both_past > BORDER_BOTH_THRESHOLD && cliff_front_left + cliff_front_right < 5500) {
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
