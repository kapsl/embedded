#include "tools.h"
#include "usart.h"
#include "floorDetection.h"
#include "drivecontrol.h"
#include "outOfCourseController.h"

uint8_t leftSensorOverLine = 0;
uint8_t rightSensorOverLine = 0;

/**
 * TODO
 */
void handleOutOfCourse(detectedType activeSensorSide) {
	// TODO special case both at one time
	// We are currently driving in
	if (leftSensorOverLine && rightSensorOverLine) {
		driveInContinued(activeSensorSide);
	} 
	// First sensor drives over a line
	else if (activeSensorSide == BORDER_LEFT && !rightSensorOverLine) {
		leftSensorOverLine = 1;
	} else if (activeSensorSide == BORDER_RIGHT && !leftSensorOverLine) {
		rightSensorOverLine = 1;
	} else if (activeSensorSide == BORDER_LEFT && rightSensorOverLine) {
		leftSensorOverLine = 1;
		driveIn(RIGHT_WHEEL);
	} else if (activeSensorSide == BORDER_RIGHT && leftSensorOverLine) {
		rightSensorOverLine = 1;
		driveIn(LEFT_WHEEL);
	}
}

void driveIn(uint8_t wheel) {
	// Stop roomba
	drive_stop();
		
	// Turn roomba by 180 degree
	drive_turn(180);
	
	// Turn the roomba till sensor reaches line
	if (wheel == RIGHT_WHEEL) {
		drive_direction(0, DRIVE_STRAIGHT_SPEED);
	} else {
		drive_direction(DRIVE_STRAIGHT_SPEED, 0);
	}
}

void driveInContinued(detectedType activeSensorSide) {
	// Stop when sensor readed line
	drive_stop();
	
	// Drive into middle of course 30 cm
	drive_roomba_exact(300, DRIVE_IN_SPEED);
	
	// Turn to driving direction
	if (activeSensorSide == BORDER_RIGHT) {
		drive_turn(90);
	} else {
		drive_turn(-90);
	}
}
