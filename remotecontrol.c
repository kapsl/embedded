#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "remotecontrol.h"

remoteSignal getRemoteSignal(){ 
	uint8_t data [2]; 
	read_values(17, data, 1); 
	
	uint8_t rem = data[0];

	if (rem == FORWARD){
		return RACCELERATE; 
	}
	else if (rem == BACKWARDS){
		return RBRAKE;
	}
	else if (rem == LEFT){
		return RLEFT; 
		}
	else if (rem == RIGHT){
		return RRIGHT; 
		}
	else if (rem == LEFT){
		return RLEFT; 
		}
	else {
		return RNOTHINGPRESSED; 
		}
	}
