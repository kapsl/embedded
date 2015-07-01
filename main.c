#include <avr/io.h>
#include "tools.h"
#include "usart.h"
#include <ctype.h>
#include "roomba.h"
#include "drivecontrol.h"
#include "remotecontrol.h"
#include "floorDetection.h"
#include "power_up.h"
#include "radio.h"
#include "outOfCourseController.h"
#include "timer.h"

int main(int argc, const char* argv[]) {
	initialization();

	while (1) {			
		// Get sensor data
		// Cliff front left, front right, left, right, remotecontrol, tick_count, bumper, wall sensor
		uint8_t packet_ids[8] = {29, 30, 28, 31, 17, 43, 7, 27};
		uint8_t packet_length[8] = {2, 2, 2, 2, 1, 2, 1, 2};
		uint16_t qdata[8];
	
		getSensorQueryList(8, packet_ids, packet_length, qdata);
		remoteSignal signal = getRemoteSignal(qdata[4]);

		if (!drive_in) {
			bump_handling((uint8_t) qdata[6]); 
		}
		
		// If we are getting a power up, we need to call this method 
		// To show the randomize sign at the display and not block
		// everything else
		showRandomizeSign();

		detectedType dType;
		floorDetection(&dType, qdata);
		
		// Detect course border and power up
		if (dType != NO_TYPE) {
			if (dType == POWER_UP) {
				getPowerUp(qdata[5]);
			} else {
				handleOutOfCourse(dType);
			}
		}
		
		if (!drive_in) {
			// OK Pressed for power ups
			if (signal == RSHOOT) {
				// Shoot power up
				shootPowerUp();
			} else {
				roomba_drive(signal);
			}
		}
		
		handleRadioSignal();
	} 
    
    return 0;
}

void initialization() {
	usart_init_roomba();
	usart_init();
	initializeRoomba();
	initializeTimers();
	
	my_msleep(20);
	
	currentPowerUp = NO_POWERUP;
	
	// Get nr. of roomba from remote control
	uint8_t rmResult = getRoombaNrFromRemote();

	initializeRadio((uint8_t) rmResult);
	
	// Wait a bit so the power button is not pressed immediatley afterwards
	my_msleep(200);
}

void handleRadioSignal() {
	// Receive radio
	// faster when hit by other roomba
	char radioSignal = receiveRadio();
	
	if (radioSignal == BUMP_SPEED && !drive_in){
		drive_bump_speed();  
	}
	// Receive radio
	// Hit by red tank
	else if (radioSignal == RED_TANK_SHOT && !drive_in) {
		// Hit did not work
		if (bigRoombaActive) {
			char result[4] = {'N', 'O', 'H', 'T'};
			set_Display(result);
			
			playSong(1);
			
			my_msleep(1500);
		} else {
			char result[4] = {'H', 'I', 'T', ' '};
			set_Display(result);
			
			playSong(3);
			drive_hit();
		}
		
		// Delete Power up
		currentPowerUp = NO_POWERUP;
		
		char result[4] = {' ', ' ', ' ', ' '};
		set_Display(result);
	}
}
