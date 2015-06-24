#ifndef RADIO_H_INCLUDED
#define RADIO_H_INCLUDED

#include <avr/io.h>

#define NETWORKID    99  //the network ID we are on
#define ACK_TIME     50  // # of ms to wait for an ack
#define SERIAL_BAUD  115200

// Char to transmit over radio
#define RED_TANK_SHOT 'r'
#define BUMP_SPEED 's'

/**
 * Which node id does this roomba have
 */
uint8_t node_id;

/**
 * \brief Initialize the radio
 */
void initializeRadio(uint8_t nr);

/**
 * Send the payload over the radio to the other roomba
 * 
 * \param payload - array with only one! char
 */
void sendRadio(char payload);

/**
 * \brief Receive a radio signal
 * 
 * \return the received char, '0' otherwise
 */
char receiveRadio(void);

/**
 * \brief Wait for an ACK - not used at the moment
 */
uint8_t waitForAck(void);

#endif
