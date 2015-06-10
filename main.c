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

// TODO Kurs bauen
// TODO Testen
// TODO Code aufräumen
// TODO comments in header files

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
	
	//TODO for testing
	//uint16_t result = 1;
	
	// TODO error, when not 1 or 2
	initializeRadio((uint8_t) result);
	
	// Wait a bit so the power button is not pressed immediatley afterwards
	my_msleep(200);
	// TODO set remote control by this

	while (1) {			
		// Get sensor data
		// Cliff front left, front right, left, right, remotecontrol, tick_count, bumper, wall sensor
		uint8_t packet_ids[8] = {29, 30, 28, 31, 17, 43, 7, 27};
		uint8_t packet_length[8] = {2, 2, 2, 2, 1, 2, 1, 2};
		uint16_t qdata[8];
	
		getSensorQueryList(8, packet_ids, packet_length, qdata);
		remoteSignal signal = getRemoteSignal(qdata[4]);

		bump_handling((uint8_t) qdata[6]); 

		my_msleep(20);

		detectedType dType;
		floorDetection(&dType, qdata);
		
		// Detect floor stuff
		if (dType != NO_TYPE) {
			if (dType == POWER_UP) {
				getPowerUp(qdata[5]);
			} else {
				handleOutOfCourse(dType);
			}
		}
		
		if (signal != RNOTHINGPRESSED && !drive_in) {
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
				playSong(1);
				sendString("Hit with mushroom - no damage...");
			} else {
				playSong(2);
				drive_hit();
				sendString("Hit...");
			}
		}
	/*playSong(0);
	my_msleep (5000);
	playSong(1);
	my_msleep (5000);
	playSong(2);
	my_msleep (5000);
	playSong(3);
	my_msleep (5000);*/
	} 
    
    return 0;
}
