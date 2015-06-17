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
	
	currentPowerUp = NO_POWERUP;
	
	// Get nr. of roomba from remote control
	uint16_t result = read_user_input();
	
	//TODO for testing
	//uint16_t result = 1;
	
	// TODO error, when not 1 or 2
	initializeRadio((uint8_t) result);
	
	// Wait a bit so the power button is not pressed immediatley afterwards
	my_msleep(200);

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
		//signal != RNOTHINGPRESSED && 
		if (!drive_in) {
			// OK Pressed for power ups
			if (signal == RSHOOT) {
				// Shoot power up
				shootPowerUp();
			} else {
				roomba_drive(signal);
			}
		}
		
		// Receive radio
		// faster when hit by other roomba
		char radioSignal = receiveRadio();
		
		if (radioSignal == BUMP_SPEED){
			drive_bump_speed();  
		}
		// Receive radio
		// Hit by red tank
		else if (radioSignal == RED_TANK_SHOT) {
			// Hit did not work
			if (bigRoombaActive) {
				char result[4] = {'N', 'O', 'H', 'T'};
				set_Display(result);
				
				playSong(1);
			} else {
				
				char result[4] = {'H', 'I', 'T', ' '};
				set_Display(result);
				
				playSong(2);
				drive_hit();
			}
		}
	} 
    
    return 0;
}
