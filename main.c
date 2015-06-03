#include <avr/io.h>
#include "tools.h"
#include "button.h"
#include "usart.h"
#include <ctype.h>
#include "roomba.h"
#include "drivecontrol.h"
#include "remotecontrol.h"
#include "floorDetection.h"
#include "power_up.h"
#include "radio.h"
#include "outOfCourseController.h"

// TODO comments in header files

// TODO Funk gut testen. Powerups hinkriegen. - rausfahren und zurücksetzen

int main(int argc, const char* argv[]) {
	usart_init_roomba();
	usart_init();
	initializeRoomba();
	
	my_msleep(20);
	
	int16_t velocity_right = 0;
	int16_t velocity_left = 0;
	currentPowerUp = NO_POWERUP;
	
	// Get nr. of roomba from remote control
	uint16_t result = read_user_input();
	// TODO error, when not 1 or 2
	initializeRadio((uint8_t) result);
	
	// Wait a bit so the power button is not pressed immediatley afterwards
	my_msleep(200);
	// TODO set remote control by this

	while (1) {			
		// Get sensor data
		// Cliff front left, front right, left, right, remotecontrol, tick_count
		uint8_t packet_ids[7] = {29, 30, 28, 31, 17, 43, 7};
		uint8_t packet_length[7] = {2, 2, 2, 2, 1, 2, 1};
		uint16_t qdata[7];
	
		getSensorQueryList(7, packet_ids, packet_length, qdata);
		remoteSignal signal = getRemoteSignal(qdata[4]);
		
		bump_handling((uint8_t)qdata[6]); 

		my_msleep(20);

		detectedType dType;
		floorDetection(&dType, qdata);
		
		// Detect floor stuff
		if (dType == POWER_UP) {
			getPowerUp(qdata[5]);
		} else if (dType == BORDER_LEFT) {
			//char result[4] = {'L', 'E', 'F', 'T'};
			//set_Display(result);
			handleOutOfCourse(BORDER_LEFT);
		} else if (dType == BORDER_RIGHT) {
			//char result[4] = {'R', 'I', 'G', 'T'};
			//set_Display(result);
			handleOutOfCourse(BORDER_RIGHT);
		} else {
			//char result[4] = {' ', ' ', ' ', ' '};
			//set_Display(result);
		}
		
		if (signal != RNOTHINGPRESSED) {
			// OK Pressed for power ups
			if (signal == RSHOOT) {
				// Shoot power up
				shootPowerUp();
			} else {
				getCommand(signal, &velocity_right, &velocity_left);
			}
		}
		
		// Receive radio
		// Hit by red tank
		if (receiveRadio() == RED_TANK_SHOT) {
			// Hit did not work
			if (bigRoombaActive) {
				playSong(0); // TODO another song
				sendString("mushhit");
			} else {
				drive_hit();
				playSong(0);
				sendString("Hit..");
			}
		}
	playSong(0);
	my_msleep (5000);
	playSong(1);
	my_msleep (5000);
	playSong(2);
	my_msleep (5000);
	playSong(3);
	my_msleep (5000);
	} 
    
    return 0;
}
