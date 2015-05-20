#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "remotecontrol.h"
#include "drivecontrol.h"

/**
 * \brief TODO
 */
void getCommand(remoteSignal type, int16_t * actVel_right,int16_t * actVel_left) {
	if (type == RACCELERATE) {
		*actVel_right=*actVel_right + ACC_BRAKE_CONSTANT;
		*actVel_left=*actVel_left + ACC_BRAKE_CONSTANT; 
	} else if (type == RBRAKE) {
		*actVel_right=*actVel_right - ACC_BRAKE_CONSTANT;
		*actVel_left=*actVel_left - ACC_BRAKE_CONSTANT; 
	} else if (type == RLEFT) {
		*actVel_right=*actVel_right + ACC_BRAKE_CONSTANT;
		*actVel_left=*actVel_left - ACC_BRAKE_CONSTANT;  
	} else if (type == RRIGHT) {
		*actVel_right=*actVel_right - ACC_BRAKE_CONSTANT;
		*actVel_left=*actVel_left + ACC_BRAKE_CONSTANT; 
	}
	
	drive_direction(*actVel_right, *actVel_left);
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
