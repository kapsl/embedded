#ifndef DRIVECONTROL_H_INCLUDED
#define DRIVECONTROL_H_INCLUDED

#define ACC_BRAKE_CONSTANT 0x50
#define CONTROL_CONSTANT 0x30

#include <avr/io.h>
#include "remotecontrol.h"

/**
 * \brief Gets a command from remote control
 * 
 * \param type
 */
void getCommand(uint8_t type, int16_t * actVel_right, int16_t * actVel_left);
void drive_direction(int16_t velocity_right, int16_t velocity_left);

#endif
