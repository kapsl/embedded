#include <avr/io.h>
#include "tools.h"
#include "button.h"
#include "usart.h"
#include <ctype.h>
#include "roomba.h"
#include "drivecontrol.h"
#include "remotecontrol.h"
#include "floorDetection.h"

int main(int argc, const char* argv[]) {
	usart_init_roomba();
	initializeRoomba();
	
	int16_t velocity_right = 0;
	int16_t velocity_left = 0;

		
	while (1) {	
		remoteSignal signal = getRemoteSignal();
		
		detectedType dType;
		floorDetection(&dType);
		
		// Detect floor stuff
		if (dType == POWER_UP) {
			char result[4] = {'P', 'O', 'W', 'R'};
			set_Display(result);
		} else if (dType == BORDER_LEFT) {
			char result[4] = {'L', 'E', 'F', 'T'};
			set_Display(result);
		} else if (dType == BORDER_RIGHT) {
			char result[4] = {'R', 'I', 'G', 'T'};
			set_Display(result);
		}
		
		if (signal != RNOTHINGPRESSED) {
			getCommand(signal, &velocity_right, &velocity_left);
		}
		my_msleep(200);
		
	} 
    
    return 0;
}
