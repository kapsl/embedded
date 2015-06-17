#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "remotecontrol.h"
#include "drivecontrol.h"
#include "roomba.h"
#include "radio.h"

/**
 * \brief This function receives a signal from the remote control which is used to stear the roomba. 
 *        Depending on the signal the roomba will drive forward, backwards, right or left 
 * 
 * \param type
 */

int16_t  actVel_right=0; 
int16_t  actVel_left=0;
int16_t  actVel_right_old=0;
int16_t  actVel_left_old=0;
uint8_t  bump_active=0;
char str [4]; 

void roomba_drive(remoteSignal type) {
	
		if(type == RACCELERATE && actVel_left<400 && actVel_right < 400) {
				
				bump_active=0; 
				drive_forward(); 
				actVel_left_old=actVel_left;
				actVel_right_old=actVel_right; 
		}
		else if (type == RBRAKE  && actVel_left>-400 && actVel_right>-400) {
				
				bump_active=0; 
				drive_break_backwards(); 
				actVel_left_old=actVel_left;
				actVel_right_old=actVel_right; 
	
		} 
		else if (type == RLEFT) {
				
				bump_active=0; 
				drive_left();
	
		}		 
		else if (type == RRIGHT) {
				
				bump_active=0; 
				drive_right();
			
		}
		else{
			
			if(bump_active==0){
			
				actVel_left=actVel_left_old;
				actVel_right=actVel_right_old; 
		
			}
		
		}		
	
		 
		 
		drive_direction(actVel_left, actVel_right);
		
		sprintf(str, "%d",
				bump_active); 
		set_Display(str); 
		
		my_msleep(200);
}

void drive_stop(){

		actVel_right=0; 
		actVel_left=0;
		drive_direction(actVel_left, actVel_right); 
			

}	

/**
 * \brief This Function changes actVel_right and actVel_left so roomba can drive forward
 * 
 * \param velocity_right
 * \param velocity_left
 */

void drive_forward (){
	
		if(actVel_left>actVel_right){
				
				actVel_right=actVel_left;
		} 
		else if (actVel_left<actVel_right){
				
				actVel_left=actVel_right; 
		}
		else if (actVel_right<400&&actVel_left<400) {
				
				actVel_right=actVel_right+SPEED_CONSTANT; 
				actVel_left=actVel_left+SPEED_CONSTANT; 
		}
}

/**
 * \brief This Function changes actVel_right and actVel_left so roomba can drive backwards
 * 
 * \param velocity_right
 * \param velocity_left
 */

void drive_break_backwards (){
	
		if(actVel_left>actVel_right){
				
				actVel_right=actVel_left;
				actVel_right=actVel_right-SPEED_CONSTANT; 
				actVel_left=actVel_left-SPEED_CONSTANT; 
		} 
		else if (actVel_left<actVel_right){
			
				actVel_left=actVel_right; 
				actVel_right=actVel_right-SPEED_CONSTANT; 
				actVel_left=actVel_left-SPEED_CONSTANT; 
		}
		else{
		
				actVel_right=actVel_right-SPEED_CONSTANT; 
				actVel_left=actVel_left-SPEED_CONSTANT; 
		}
}	

/**
 * \brief This Function changes actVel_right and actVel_left so roomba can turn left
 * 
 * \param velocity_right
 * \param velocity_left
 */

void drive_left (){
	
		if (actVel_right>=0){
			
				if (actVel_right<400){
							
						actVel_left=actVel_left-SPEED_CONSTANT;
						actVel_right=actVel_right+SPEED_CONSTANT;
				}
				else{
						actVel_left=actVel_left-(2*SPEED_CONSTANT);
				}
		}
		else if (actVel_right<0){
				
				if (actVel_right>-400){
							
							actVel_left=actVel_left-SPEED_CONSTANT;
							actVel_right=actVel_right+SPEED_CONSTANT;
				}
				else{
							actVel_left=actVel_left-(2*SPEED_CONSTANT);
				}
		}
}

/**
 * \brief This Function changes actVel_right and actVel_left so roomba can turn right
 * 
 * \param velocity_right
 * \param velocity_left
 */

void drive_right (){
	
		if (actVel_left>=0){
		
				if (actVel_left<400){
					
						actVel_left=actVel_left+SPEED_CONSTANT;
						actVel_right=actVel_right-SPEED_CONSTANT;
				}
				else{
						actVel_right=actVel_right-(2*SPEED_CONSTANT);
				}
		}
		else if (actVel_left<0){
	
				if (actVel_left>-400){
						actVel_left=actVel_left+SPEED_CONSTANT;
						actVel_right=actVel_right-SPEED_CONSTANT;
				}
				else{
						actVel_right=actVel_right-(2*SPEED_CONSTANT);
				}
		}	
}

/**
 * \brief Turn the robot by a certain degree
 * 
 * \param degree 
 */
void drive_turn(int16_t degree){
	// In 13 seconds, 360°
	uint16_t delay = (uint16_t)(abs(degree)*10.45f);
	
		if (degree > 0) {
			driveWithRadius(200, 1);
		}
		else{
			driveWithRadius(200, -1);
		}
	
	my_msleep(delay);
	
	drive_stop(); 
}

/**
 * \brief This Function receives drive commands and sends them to the roomba. The rooba can drive forwards, backwards, left or right. 
 * 
 * \param velocity_right
 * \param velocity_left
 */

void drive_direction(int16_t left_speed, int16_t right_speed) {
	send_byte_roomba(145);
	
	uint8_t low = actVel_right;
	uint8_t high = (actVel_right >> 8);
	
	send_byte_roomba(high);
	send_byte_roomba(low);
	
	low = actVel_left;
	high = (actVel_left >> 8);
	
	send_byte_roomba(high);
	send_byte_roomba(low);
}

/**
 * \brief Lets roomba drive forward for an exact distance
 * 
 * \param distance
 * \param velocity
 */
 
void drive_roomba_exact(uint16_t distance, int16_t velocity) {
	uint16_t start_value = getTicks();
	
	drive_direction(velocity, velocity);
	
	char buff[50];
	sprintf(buff, "Ticks-Start: %u\r\n", start_value);
	sendString(buff);
	
	uint16_t maxTicks = (uint16_t) (distance * 2.262);
	sprintf(buff, "Max: %u\r\n", maxTicks);
	sendString(buff);
	
	my_msleep(400);
	
	while (1) {
		uint16_t ticks = getTicks();
		uint16_t distanceTicks;
		
		// Overflow
		if (ticks < start_value && velocity > 0) {
		
				distanceTicks = 65535 - start_value + ticks;
		} 
		else if (ticks > start_value && velocity < 0) {
				
				distanceTicks = start_value + (65535 - ticks);
		} 
		else {
			
				distanceTicks = abs(ticks - start_value);
		}
		
		if (distanceTicks > maxTicks) {
		
				break;
		}
	}
	
	drive_stop();
}

/**
 * \brief This function return the actual number of ticks 
 * 
 * \return number of ticks
 */
 
uint16_t getTicks() {
	
	uint8_t data[2];
	read_values(43, data, 2);
	
	return concat_bytes(data);
}

/**
 * \brief This function stops the roomba in case drive hits an opstical to prevent damage
 * 
 * \param bump
 */
 
void bump_handling(uint8_t bump) {
		
		
		// Left bumper
		if ((bump & 0x02) == 0x02) {
				bump_active=1; 
				sendRadio(BUMP_SPEED); 
				drive_stop();
		
		// Right bumper
		} 
		else if ((bump & 0x01) == 0x01) {
		bump_active=1; 
		sendRadio(BUMP_SPEED); 
		drive_stop(); 
		}
}

/**
 * \brief This function makes the roomba drive an circle with a certain radius
 * 
 * \param velocity 
 * \param radius this radius (use DRIVE_STRAIGHT for straight, -1 for 
 * 			clockwise 1 for anticlockwise)
 */
 
void driveWithRadius(uint16_t velocity, uint16_t radius) {
	
	send_byte_roomba(137);
	
	uint8_t low = velocity;
	uint8_t high = (velocity >> 8);
	
	send_byte_roomba(high);
	send_byte_roomba(low);	
	
	uint8_t rlow = radius;
	uint8_t rhigh = (radius >> 8);
	
	send_byte_roomba(rhigh);
	send_byte_roomba(rlow);	
}

/**
 * \brief This function first stops the roomba and then turns it 3 times arround its own axis. Function is a consiquence for player if his roomba was hit by the power up "RED-TANK"
 */

void drive_hit(){
	
	drive_stop(); 
	drive_turn(3*360+10); 
} 

void drive_bump_speed(){
		
		bump_active=1; 
		int16_t turbo_speed_left=500;
		int16_t turbo_speed_right=500; 
		drive_direction(turbo_speed_left, turbo_speed_right);

}
