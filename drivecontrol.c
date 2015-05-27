#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "remotecontrol.h"
#include "drivecontrol.h"

uint8_t speed_set_left = 0x00; 
uint8_t speed_set_right = 0x00;
char str [5];
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
			else if (*actVel_right<500&&*actVel_left<500) {
				*actVel_right=*actVel_right+ACC_BRAKE_CONSTANT; 
				*actVel_left=*actVel_left+ACC_BRAKE_CONSTANT; 
			}
	} 
	else if (type == RBRAKE  && *actVel_left>0) {
		
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
	} 
	else if (type == RLEFT  && *actVel_left>0) {
		if (actVel_left<500&&actVel_right<500){
			*actVel_left=*actVel_left-CONTROL_CONSTANT;
			*actVel_right=*actVel_right+CONTROL_CONSTANT;
		}
		else{
			*actVel_right=*actVel_right+CONTROL_CONSTANT;
			*actVel_left=*actVel_left-(2*CONTROL_CONSTANT);
		}
	} 
	else if (type == RRIGHT && *actVel_right>0) {
		if (actVel_left<500&&actVel_right<500){
			*actVel_left=*actVel_left+CONTROL_CONSTANT;
			*actVel_right=*actVel_right-CONTROL_CONSTANT;
		}
		else{
			*actVel_left=*actVel_left+CONTROL_CONSTANT;
			*actVel_right=*actVel_right-(2*CONTROL_CONSTANT);
		}
		
	}
		 
	drive_direction(*actVel_right, *actVel_left);
		sprintf(str, "%x%x",
				*actVel_left,
				*actVel_right);
		my_msleep(20);		 
		set_Display(str);
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
