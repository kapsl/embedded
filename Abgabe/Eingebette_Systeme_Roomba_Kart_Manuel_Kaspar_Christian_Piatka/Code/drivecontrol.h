#ifndef DRIVECONTROL_H_INCLUDED
#define DRIVECONTROL_H_INCLUDED

#define SPEED_CONSTANT 5

#include <avr/io.h>
#include "remotecontrol.h"
 
/**
 * \brief This function receives a signal from the remote control which is used to stear the roomba. 
 *        Depending on the signal the roomba will drive forward, backwards, right or left 
 * 
 * \param type (gives direction in which roomba should drive)
 */
void roomba_drive(remoteSignal type);

void drive_direction(int16_t left_speed, int16_t right_speed);

/**
 * \brief This Function makes roomba drive forward
 */
void drive_forward (void);

/**
 * \brief This Function stops roomba or makes it drive backwards
 */
void drive_break_backwards (void);

/**
 * \brief This Function makes roomba turn right
 */
void drive_right (void);

/**
 * \brief This Function makes roomba turn left
 */
void drive_left (void);

/**
 * \brief This Function stops roomba completely 
 */
void drive_stop(void); 

/**
 * \brief This Function turns roomba for a certain degree
 */
void drive_turn(int16_t degree);

/**
 * \brief This Function lets roomba drive for an exact distance at a certain speed
 * 
 * \param distance (distance roomba should drive)
 * \param velocity (speed at which roomba should drive) 
 */
void drive_roomba_exact(uint16_t distance, int16_t velocity);

/**
 * \brief This Function lets roomba drive a circle with a specific radius at a specific speed
 * 
 * \param velocity (speed at which roomba should drive)
 * \param radius (radius of the circle roomba should drive in mm)
 */
void driveWithRadius(uint16_t velocity, uint16_t radius);

/**
 * \brief If the roomba bumps something it stops and informs the other roomba via radiosignal
 * 
 * \param bump (Signal of the Bumpers from the roomba)   
 */
void bump_handling(uint8_t bump); 

/**
 * \brief If the roomba gets hit by the power up "RED_TANK" this function turns roomba 1080°
 */
void drive_hit(void); 

/**
 * \brief If oppenent roomba bumps something which it registers with its bumpers, the roomba will have a powerboost
 */
void drive_bump_speed(void); 

/**
 * \brief Get wheel ticks
 */
uint16_t getTicks(void);

#endif
