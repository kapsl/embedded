#ifndef OUTOFCOURSECONTROLLER_H_INCLUDED
#define OUTOFCOURSECONTROLLER_H_INCLUDED

#include <avr/io.h>
#include "floorDetection.h"

#define RIGHT_WHEEL 0
#define LEFT_WHEEL 1
#define DRIVE_STRAIGHT_SPEED 80
#define DRIVE_IN_SPEED 30

extern uint8_t outsideCourse;

void handleOutOfCourse(detectedType activeSensorSide);
void driveIn(uint8_t wheel);
void driveInContinued(detectedType activeSensorSide);

#endif
