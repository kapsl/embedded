#include "weapondamage.h"
#include "roomba.h"
#include <stdio.h>
#include <stdlib.h>

void weapondamage(uint8_t weapon){
	if (weapon==RED_TANK){
	drive(0x0); 
	my_msleep(200);
	turn(3*360); 
	}
}	
