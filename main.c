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
#include "weapondamage.h"

int main(int argc, const char* argv[]) {
	usart_init_roomba();
	initializeRoomba();
	my_msleep(20);
	
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
		
		my_msleep(20);
		
		if (signal != RNOTHINGPRESSED) {
			getCommand(signal, &velocity_right, &velocity_left);
		}
		
	} 
    
    return 0;
}
