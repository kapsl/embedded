#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>

RemoteSignal getRemoteSingnal(){ 

	uint8_t data [1]; 

	read_values(17, data[], 1); 
	uint8_t rem = data[0];

	if (rem == FORWARD){
		return ACCELERATE; 
	}
	else if (rem == BACKWARDS){
		return BRAKE;
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
