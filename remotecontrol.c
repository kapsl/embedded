#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "remotecontrol.h"
#include "radio.h"

/**
 * TODO
 */
remoteSignal getRemoteSignal(uint16_t remoteSignalData){ 
	uint8_t rem = remoteSignalData;
	
	if(node_id==1){
		if (rem == FORWARD){
			return RACCELERATE; 
		} else if (rem == BACKWARDS){
			return RBRAKE;
		} else if (rem == LEFT){
			return RLEFT; 
		} else if (rem == RIGHT){
			return RRIGHT; 
		} else if (rem == LEFT){
			return RLEFT; 
		} else if (rem == POWER){
			return RSHOOT;
		}
	}	
	else if (node_id==2){
		if (rem == NR_2){
			return RACCELERATE; 
		} else if (rem == NR_8){
			return RBRAKE;
		} else if (rem == NR_4){
			return RLEFT; 
		} else if (rem == NR_6){
			return RRIGHT; 
		} else if (rem == NR_5){
			return RSHOOT;
		}
	}
	
	return RNOTHINGPRESSED;
}
