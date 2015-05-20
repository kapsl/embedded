#ifndef DRIVECONTROL_H_INCLUDED
#define DRIVECONTROL_H_INCLUDED

#include <avr/io.h>

enum Direction{ACCELERATE, BRAKE, LEFT, RIGHT};

/**
 * \brief Gets a command from remote control
 * 
 * \param type
 */
void getCommand(Direction type);

#endif
