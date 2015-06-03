#ifndef DRIVECONTROL_H_INCLUDED
#define DRIVECONTROL_H_INCLUDED

#define ACC_BRAKE_CONSTANT 50
#define CONTROL_CONSTANT 50


#include <avr/io.h>
#include "remotecontrol.h"

/**
 * \brief Gets a command from remote control
 * 
 * \param type
 */
void getCommand(uint8_t type, int16_t * actVel_right, int16_t * actVel_left);
void drive_direction(int16_t velocity_right, int16_t velocity_left);
void drive_forward (int16_t * actVel_right, int16_t * actVel_left);
void drive_break_backwards (int16_t * actVel_right, int16_t * actVel_left);
void drive_right (int16_t * actVel_right, int16_t * actVel_left);
void drive_left (int16_t * actVel_right, int16_t * actVel_left);
void drive_stop(void); 
void drive_turn(int16_t degree);
void drive_roomba_exact(uint16_t distance, int16_t velocity);
void driveWithRadius(uint16_t velocity, uint16_t radius);
void bump_handling(uint8_t bump); 
void drive_hit(void); 
uint16_t getTicks(void);

#endif
