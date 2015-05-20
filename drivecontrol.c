#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * \brief TODO
 */
void getCommand(Direction type, int16_t actVel_right, int16_t actVel_left) {
	if (type == ACCELERATE) {
		drive_direction(actVel_right + ACC_BRAKE_CONSTANT, actVel_left + ACC_BRAKE_CONSTANT);
	} else if (type == BRAKE) {
		drive_direction(actVel_right - ACC_BRAKE_CONSTANT, actVel_left - ACC_BRAKE_CONSTANT);
	} else if (type == LEFT) {
		drive_direction(actVel_right + ACC_BRAKE_CONSTANT, actVel_left - ACC_BRAKE_CONSTANT);
	} else if (type == RIGHT) {
		drive_direction(actVel_right - ACC_BRAKE_CONSTANT, actVel_left + ACC_BRAKE_CONSTANT);
	}
}

/**
 * \brief Drive and have the posibility to drive a corner
 * 
 * \param velocity_right
 * \param velocity_left
 */
void drive_direction(int16_t velocity_right, int16_t velocity_left) {
	send_byte_roomba(145);
	
	uint8_t low = velocity_right;
	uint8_t high = (velocity_right >> 8);
	
	send_byte_roomba(high);
	send_byte_roomba(low);
	
	low = velocity_left;
	high = (velocity_left >> 8);
	
	send_byte_roomba(high);
	send_byte_roomba(low);
}
