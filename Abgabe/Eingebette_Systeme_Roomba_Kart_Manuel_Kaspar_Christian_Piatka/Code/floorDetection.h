#ifndef FLOORDETECTION_H_INCLUDED
#define FLOORDETECTION_H_INCLUDED

#include <avr/io.h>

//#define POWER_UP_CLIFF_THRESHOLD 6400
//#define POWER_UP_CLIFF_THRESHOLD 6000
/**
 * Threshold for yellow crep
 */
//#define BORDER_CLIFF_THRESHOLD 2500
#define BORDER_CLIFF_THRESHOLD 1800

//#define BORDER_CLIFF_BLACK_THRESHOLD 2040
/*
 * Threshold when two sensors detect yellow crep
 */
#define BORDER_BOTH_THRESHOLD 3500
#define WALL_SIGNAL_THRESHOLD 26

/**
 * Possible types that the roomba can detect
 * 
 * Wall sensor power-up, cliff sensor left, right, both front cliff sensors, 
 * both side cliff sensors, front left cliff sensor, right, nothing detected
 */
enum DetectedType{POWER_UP, BORDER_LEFT, BORDER_RIGHT, BORDER_BOTH, BORDER_SIDE_BOTH, BORDER_FRONT_LEFT, BORDER_FRONT_RIGHT, NO_TYPE};  
typedef enum DetectedType detectedType;

/**
 *  \brief This method can detect different things on floor or a
 * 		power up (wall sensor)
 * 
 * 	\param type array to write the detected type in
 *  \param sensorData the sensorData Array retrieved in the main function
 */
void floorDetection(detectedType *type, uint16_t *sensorData);

#endif
