#include <avr/io.h>
#include "tools.h"
#include "button.h"
#include "usart.h"
#include <ctype.h>
#include "roomba.h"
#include "drivecontrol.h"
#include "remotecontrol.h"

int main(int argc, const char* argv[]) {
	usart_init_roomba();
	initializeRoomba();
	
	int16_t velocity_right = 0;
	int16_t velocity_left = 0;

		
	while (1) {	
		remoteSignal signal = getRemoteSignal();
		
		if (signal != RNOTHINGPRESSED) {
			getCommand(signal, &velocity_right, &velocity_left);
		}
		my_msleep(200);
		
	} 
    
    return 0;
}
