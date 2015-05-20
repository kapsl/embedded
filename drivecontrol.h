#ifndef DRIVECONTROL_H_INCLUDED
#define DRIVECONTROL_H_INCLUDED

#define ACC_BRAKE_CONSTANT 10

#include <avr/io.h>

/**
 * \brief Gets a command from remote control
 * 
 * \param type
 */
void getCommand(Direction type, int16_t actVel_right, int16_t actVel_left);
void drive_direction(int16_t velocity_right, int16_t velocity_left);

#endif
