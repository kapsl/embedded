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

// TODO comments in header files

int main(int argc, const char* argv[]) {
	usart_init_roomba();
	usart_init();
	initializeRoomba();
	
	int16_t velocity_right = 0;
	int16_t velocity_left = 0;
	currentPowerUp = NO_POWERUP;
	
	// Get nr. of roomba from remote control
	uint16_t result = read_user_input();
	// TODO error, when not 1 or 0
	initializeRadio((uint8_t) result);
	
	// Wait a bit so the power button is not pressed immediatley afterwards
	my_msleep(200);
	// TODO set remote control by this

	while (1) {			
		// Get sensor data
		// Cliff front left, front right, left, right, remotecontrol, tick_count
		uint8_t packet_ids[6] = {29, 30, 28, 31, 17, 43};
		uint8_t packet_length[6] = {2, 2, 2, 2, 1, 2};
		uint16_t qdata[6];
	
		getSensorQueryList(6, packet_ids, packet_length, qdata);
		
		remoteSignal signal = getRemoteSignal(qdata[4]);
		
		detectedType dType;
		floorDetection(&dType, qdata);
		
		// Detect floor stuff
		if (dType == POWER_UP) {
			getPowerUp(qdata[5]);
			//my_msleep(1000);
			//while(1);
			// TODO funkmodul
		} else if (dType == BORDER_LEFT) {
			//char result[4] = {'L', 'E', 'F', 'T'};
			//set_Display(result);
		} else if (dType == BORDER_RIGHT) {
			//char result[4] = {'R', 'I', 'G', 'T'};
			//set_Display(result);
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
		if (receiveRadio() == RED_TANK_SHOT) {
			// Hit by red tank
			if (currentPowerUp == MUSHROOM) {
				// Hit did not work
				playSong(0); // TODO another song
				sendString("mushhit");
			} else {
				// Stop roomba and turn
				//weapondamage(1);
				playSong(0);
				sendString("hit");
			}
		}
		
		my_msleep(200);
	} 
    
    return 0;
}
