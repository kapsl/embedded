#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "remotecontrol.h"

/**
 * TODO
 */
remoteSignal getRemoteSignal(uint16_t remoteSignalData){ 
	uint8_t rem = remoteSignalData;

	if (rem == FORWARD) {
		return RACCELERATE; 
	} else if (rem == BACKWARDS) {
		return RBRAKE;
	} else if (rem == LEFT) {
		return RLEFT; 
	} else if (rem == RIGHT) {
		return RRIGHT; 
	} else if (rem == LEFT) {
		return RLEFT; 
	} else if (rem == POWER) {
		return RSHOOT;
	} else {
		return RNOTHINGPRESSED; 
	}
}
