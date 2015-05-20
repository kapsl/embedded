#include <avr/io.h>
#include "tools.h"
#include "button.h"
#include "usart.h"
#include <ctype.h>
#include "roomba.h"
#include "drivecontrol.c"
#include "remotecontrol.c"

int main(int argc, const char* argv[]) {
	usart_init_roomba();
	initializeRoomba();
	
	int16_t velocity_right = 0;
	int16_t velocity_left = 0;
	
	while (1) {	
		RemoteSignal remoteSignal = getRemoteSignal();
		
		if (remoteSignal != NOTHINGPRESSED) {
			getCommand(remoteSignal, velocity_right, velocity_left);
		}
	} 
    
    return 0;
}
