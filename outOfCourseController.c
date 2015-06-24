#include "tools.h"
#include "usart.h"
#include "floorDetection.h"
#include "drivecontrol.h"
#include "power_up.h"
#include "outOfCourseController.h"
#include "roomba.h"

/**
 * Variable to check at which step of driving back in we are
 */
uint8_t step = 0;

uint8_t drive_in = 0;

/**
 * On which side of the road did we drive out
 */
uint8_t side = 0;
uint8_t outsideCourse = 0;

void handleOutOfCourse(detectedType activeSensorSide) {
	// Detect side on which we drive out
	if (activeSensorSide == BORDER_FRONT_LEFT && !drive_in) {
		side = 0;
	} else if (activeSensorSide == BORDER_FRONT_RIGHT && !drive_in) {
		side = 1;
	}
	
	// Drive back to line
	else if (activeSensorSide == BORDER_BOTH && !drive_in && step == 0) {
		// When mushroom is active, we are allowed to drive out
		if (mushroomActive && outsideCourse == 0) {
			// We drove outside
			outsideCourse = 1;
			my_msleep(1000);
		} else if (mushroomActive && outsideCourse == 1) {
			// We are driving back inside
			outsideCourse = 0;
			my_msleep(1000);
		} else {
			driveIn(RIGHT_WHEEL);
		}
	} else if ((activeSensorSide == BORDER_RIGHT || activeSensorSide == BORDER_SIDE_BOTH) && step == 0 && drive_in) {
		step = 1;
		driveIn(LEFT_WHEEL);
	} else if ((activeSensorSide == BORDER_LEFT || activeSensorSide == BORDER_SIDE_BOTH) && drive_in && step == 1) {
		step = 2;
		driveIn(RIGHT_WHEEL);
	} 
	
	// Drive back to middle of the road
	else if (activeSensorSide == BORDER_SIDE_BOTH && step == 2 && drive_in) {
		driveInContinued(activeSensorSide);
	}
}

void driveIn(uint8_t wheel) {
	drive_in = 1;

	// Stop roomba
	drive_stop();
	
	// Turn the roomba till sensor reaches line
	if (wheel == LEFT_WHEEL) {
		drive_direction(-DRIVE_STRAIGHT_SPEED, 0);
	} else {
		drive_direction(0, -DRIVE_STRAIGHT_SPEED);
	}
	
	my_msleep(350);
}

void driveInContinued(detectedType activeSensorSide) {
	// Stop when sensor readed line
	drive_stop();
	
	// Drive into middle of course 30 cm
	drive_roomba_exact(300, DRIVE_IN_SPEED);
	
	// Turn to driving direction
	if (side == 0) {
		drive_turn(90);
	} else if (side == 1) {
		drive_turn(-90);
	}
	
	drive_in = 0;
	step = 0;
	side = 0;
}
