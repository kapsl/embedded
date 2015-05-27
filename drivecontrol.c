#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "remotecontrol.h"
#include "drivecontrol.h"

uint8_t left_just_pressed = 0x0; 
uint8_t right_just_pressed = 0x0;
/**
 * \brief TODO
 */
void getCommand(remoteSignal type, int16_t * actVel_right,int16_t * actVel_left) {
	
	if (type == RACCELERATE) {
			if(*actVel_left>*actVel_right){
				*actVel_right=*actVel_left;
			} 
			else if (*actVel_left<*actVel_right){
				*actVel_left=*actVel_right; 
			}
			else {
				*actVel_right=*actVel_right+ACC_BRAKE_CONSTANT; 
				*actVel_left=*actVel_left+ACC_BRAKE_CONSTANT; 
			}
	} 
	else if (type == RBRAKE) {
				if(*actVel_left>*actVel_right){
				*actVel_right=*actVel_left;
			} 
			else if (*actVel_left<*actVel_right){
				*actVel_left=*actVel_right; 
			}
			else {
				*actVel_right=*actVel_right-ACC_BRAKE_CONSTANT; 
				*actVel_left=*actVel_left-ACC_BRAKE_CONSTANT; 
	} 
	else if (type == RLEFT && left_just_pressed==0x0) {
		left_just_pressed=0x1; 
		right_just_pressed=0x0;
		*actVel_right=*actVel_left;
		*actVel_left=0.5*(*actVel_left);  
	} 
	else if (type == RRIGHT && right_just_pressed==0x0) {
		left_just_pressed=0x0; 
		right_just_pressed=0x1; 
		*actVel_left=*actVel_right;
		*actVel_right=0.5*(*actVel_right);
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
