#ifndef FLOORDETECTION_H_INCLUDED
#define FLOORDETECTION_H_INCLUDED

#include <avr/io.h>

#define POWER_UP_CLIFF_THRESHOLD 6400
//#define POWER_UP_CLIFF_THRESHOLD 2500
#define BORDER_CLIFF_THRESHOLD 2500

// Black crep
#define BORDER_CLIFF_BLACK_THRESHOLD 840


enum DetectedType{POWER_UP, BORDER_LEFT, BORDER_RIGHT, BORDER_LEFT_BLACK, BORDER_RIGHT_BLACK, BORDER_BOTH, BORDER_BOTH_BLACK, NO_TYPE};  
typedef enum DetectedType detectedType;

void floorDetection(detectedType *type, uint16_t *sensorData);

#endif
