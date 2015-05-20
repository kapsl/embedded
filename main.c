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

int main(int argc, const char* argv[]) {
	usart_init_roomba();
	initializeRoomba();
	
	int16_t velocity_right = 0;
	int16_t velocity_left = 0;

		
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
			powerUp_type powerup_type = getPowerUp(qdata[5]);
			
			// TODO Save current powerup, if OK is pressed --> handle powerup
			// TODO funkmodul
		} else if (dType == BORDER_LEFT) {
			char result[4] = {'L', 'E', 'F', 'T'};
			set_Display(result);
		} else if (dType == BORDER_RIGHT) {
			char result[4] = {'R', 'I', 'G', 'T'};
			set_Display(result);
		} else {
			char result[4] = {' ', ' ', ' ', ' '};
			set_Display(result);
		}
		
		if (signal != RNOTHINGPRESSED) {
			getCommand(signal, &velocity_right, &velocity_left);
		}
		my_msleep(200);
		
	} 
    
    return 0;
}
