#include "tools.h"
#include "usart.h"
#include "floorDetection.h"
#include "drivecontrol.h"
#include "power_up.h"
#include "outOfCourseController.h"

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
	
	// We are currently driving in
	if (drive_in && leftSensorOverLine && rightSensorOverLine) {
		driveInContinued(activeSensorSide);
	} 
	
	// Both sensors at one time - means drive off vertical
	else if (activeSensorSide == BORDER_BOTH) {
		drive_turn(180);
		driveInContinued(activeSensorSide);
	}
	
	// First sensor drives over a line
	else if ((activeSensorSide == BORDER_LEFT && !rightSensorOverLine) || (activeSensorSide == BORDER_LEFT && drive_in)) {
		// We are driving back into the course
		// ??TODO doesn't work, because we can drive longer on the line (Really thin?)
		/*if (leftSensorOverLine == 1 && !drive_in) {
			leftSensorOverLine = 0;
			my_msleep(2000);
		} else {
			leftSensorOverLine = 1;
		}*/
		
		leftSensorOverLine = 1;
		
		// Wait a bit so that we drove over the line
		// TODO out
		//my_msleep(500);
	} else if ((activeSensorSide == BORDER_RIGHT && !leftSensorOverLine) || (activeSensorSide == BORDER_RIGHT && drive_in)) {
		/*if (rightSensorOverLine == 1 && !drive_in) {
			rightSensorOverLine = 0;
			my_msleep(200);
		} else {
			rightSensorOverLine = 1;
		}*/
		rightSensorOverLine = 1;
		
		// Wait a bit so that we drove over the line
		// TODO out
		//my_msleep(500);
	} else if (activeSensorSide == BORDER_LEFT && rightSensorOverLine) {
		leftSensorOverLine = 1;
		rightSensorOverLine = 0;
		
		driveIn(RIGHT_WHEEL);
	} else if (activeSensorSide == BORDER_RIGHT && leftSensorOverLine) {
		rightSensorOverLine = 1;
		leftSensorOverLine = 0;
		
		driveIn(LEFT_WHEEL);
	}
	
	send_byte(leftSensorOverLine);
}

void driveIn(uint8_t wheel) {
	if (drive_in) {
		return;
	}
	
	// TODO When mushroom is active, we are allowed to drive out
	if (mushroomActive) {
		return;
	}
	
	drive_in = 1;
	
	// Stop roomba
	drive_stop();
		
	// Turn the roomba till sensor reaches line
	if (wheel == RIGHT_WHEEL) {
		drive_direction(DRIVE_STRAIGHT_SPEED, 0);
	} else {
		drive_direction(0, DRIVE_STRAIGHT_SPEED);
	}
	
	my_msleep(300);
}

void driveInContinued(detectedType activeSensorSide) {
	// Wait a bit more, because sensor is not in center of roomba
	my_msleep(350);
	
	// Stop when sensor readed line
	drive_stop();
	
	// Drive into middle of course 30 cm
	drive_roomba_exact(300, DRIVE_IN_SPEED);
	
	// Turn to driving direction
	// No turn, when we drove off straight
	if (activeSensorSide == BORDER_RIGHT) {
		drive_turn(-90);
	} else if (activeSensorSide == BORDER_LEFT) {
		drive_turn(90);
	}
	
	drive_in = 0;
	leftSensorOverLine = 0;
	rightSensorOverLine = 0;
}
