#ifndef DRIVECONTROL_H_INCLUDED
#define DRIVECONTROL_H_INCLUDED

#define SPEED_CONSTANT 5


#include <avr/io.h>
#include "remotecontrol.h"

/**
 * \brief Gets a command from remote control
 * 
 * \param type
 */
void roomba_drive(uint8_t type);
void drive_direction(int16_t left_speed, int16_t right_speed);
void drive_forward (void);
void drive_break_backwards (void);
void drive_right (void);
void drive_left (void);
void drive_stop(void); 
void drive_turn(int16_t degree);
void drive_roomba_exact(uint16_t distance, int16_t velocity);
void driveWithRadius(uint16_t velocity, uint16_t radius);
void bump_handling(uint8_t bump); 
void drive_hit(void); 
void drive_bump_speed(void); 
uint16_t getTicks(void);

#endif
