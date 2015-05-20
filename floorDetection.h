#ifndef FLOORDETECTION_H_INCLUDED
#define FLOORDETECTION_H_INCLUDED

#include <avr/io.h>

#define POWER_UP_CLIFF_THRESHOLD 6400
#define BORDER_CLIFF_THRESHOLD 2500

enum DetectedType{POWER_UP, BORDER_LEFT, BORDER_RIGHT};  
typedef enum DetectedType detectedType;

void floorDetection(detectedType *type);

#endif
