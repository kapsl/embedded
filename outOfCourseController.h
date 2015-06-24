#ifndef OUTOFCOURSECONTROLLER_H_INCLUDED
#define OUTOFCOURSECONTROLLER_H_INCLUDED

#include <avr/io.h>
#include "floorDetection.h"

#define RIGHT_WHEEL 0
#define LEFT_WHEEL 1
#define DRIVE_STRAIGHT_SPEED 80
#define DRIVE_IN_SPEED 150

/**
 * Flag if we are inside or outside the course
 */
extern uint8_t outsideCourse;

/**
 * If we are currently driving back to the middle of the line
 */
extern uint8_t drive_in;

/**
 * \brief Determine if we are driving out of the course,
 * 		and if yes, drive the roomba back into the middle of thecourse
 * 
 * \param activeSensorSide which sensor detected a drive over the crep
 */
void handleOutOfCourse(detectedType activeSensorSide);

/**
 * \brief Drive back in to the course
 * 
 * \param wheel RIGHT_WHEEL or LEFT_WHEEL, with wich we drove out
 */
void driveIn(uint8_t wheel);

/**
 * \brief When we are 90 degrees to the border, continue here,
 * 			drive back in the middle of the course and turn 
 * 			roomba by 90 degrees into the right direction
 */
void driveInContinued(detectedType activeSensorSide);

#endif
