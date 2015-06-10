#include "tools.h"
#include "usart.h"
#include "floorDetection.h"
#include "drivecontrol.h"
#include "power_up.h"
#include "outOfCourseController.h"
#include "roomba.h"

uint8_t leftSensorOverLine = 0;
uint8_t rightSensorOverLine = 0;

uint8_t drive_in = 0;

/**
 * TODO
 */
void handleOutOfCourse(detectedType activeSensorSide) {
	// TODO Time window, after that the flags are set back, elseway 
	// it is possible, that we drive out with one wheel and then on 
	// the other side and then we have a problem
	
	// TODO eventually implement that he drives off the road backwards
	
	if (activeSensorSide == BORDER_BOTH && !drive_in && rightSensorOverLine == 0) {
		driveIn(RIGHT_WHEEL);
	} else if (activeSensorSide == BORDER_RIGHT && rightSensorOverLine == 0 && drive_in) {
		rightSensorOverLine = 1;
		driveIn(LEFT_WHEEL);
	} else if (activeSensorSide == BORDER_LEFT && drive_in && rightSensorOverLine == 1) {
		rightSensorOverLine = 2;
		driveIn(RIGHT_WHEEL);
	} else if (activeSensorSide == BORDER_SIDE_BOTH && rightSensorOverLine == 2 && drive_in) {
		driveInContinued(activeSensorSide);
	}
}

void driveIn(uint8_t wheel) {
	sendString("Drive in\r\n");
	
	/*if (drive_in) {
		return;
	}*/
	
	// TODO When mushroom is active, we are allowed to drive out
	if (mushroomActive) {
		return;
	}
	
	drive_in = 1;
	
	// Stop roomba
	drive_stop();
		
	// Turn the roomba till sensor reaches line
	if (wheel == RIGHT_WHEEL) {
		drive_direction(-DRIVE_STRAIGHT_SPEED, 0);
	} else {
		drive_direction(0, -DRIVE_STRAIGHT_SPEED);
	}
	
	my_msleep(350);
}

void driveInContinued(detectedType activeSensorSide) {
	sendString("Drive in Continued\r\n");
	
	// Wait a bit more, because sensor is not in center of roomba
	//my_msleep(350);
	
	// Stop when sensor readed line
	drive_stop();
	
	// Drive into middle of course 30 cm
	drive_roomba_exact(300, DRIVE_IN_SPEED);
	
	// Turn to driving direction
	// No turn, when we drove off straight
	/*if (activeSensorSide == BORDER_RIGHT) {
		drive_turn(-90);
	} else if (activeSensorSide == BORDER_LEFT) {
		drive_turn(90);
	}*/
	drive_turn(90);
	
	drive_in = 0;
	leftSensorOverLine = 0;
	rightSensorOverLine = 0;
}
