#ifndef ROOMBA_H_INCLUDED
#define ROOMBA_H_INCLUDED

#include <avr/io.h>

#define CLEAN 0x01
#define SPOT 0x02
#define DOCK 0x04
#define MINUTE 0x08
#define HOUR 0x10
#define DAY 0x20
#define SCHEDULE 0x40
#define CLOCK 0x80

#define DRIVE_STRAIGHT 0x8000

#define ASCI_0 48
#define ROOMBA_0 129

#define REMOTE_POWER 138
#define REMOTE_FORWARD 139
#define REMOTE_BACKWARD 140
#define REMOTE_LEFT 141
#define REMOTE_RIGHT 143


void initializeRoomba(void);
void set_Display(char *data);

/**
 * \brief Set display by using the 7 segment mode
 * 
 * \param data_bytes array of 4 values, each value gives the combination
 * 			of 7 segments switched on or off
 */
void set_Display_raw(uint8_t *data_bytes);

/**
 * \brief Clear all entries on the display
 */
void clear_Display(void);

/**
 * \brief Find out if a button on the roomba was pressed
 * 
 * \param button a constant fromm roomba.h e.g HOUR
 * \return 0 if button was not pressed, 1 instead
 */
uint8_t read_button(uint8_t button);

/**
 * \brief Read a sensor value
 * 
 * \param packetid the packetid corresponding to the sensor
 * \param data an array where the returned bytes are stored
 * \param length how many bytes should be retrieved
 */
void read_values(uint8_t packetid, uint8_t * data, uint8_t length);

/**
 * \brief Receive a signal from the remote. Show the received
 * 			value as hex on Display and drive corresponding
 * 			to the steering buttons
 */
uint8_t receiveRemote(void);

/**
 * \brief Show the pressed remote button hex code on display
 * 
 * \param data the received button code
 */
void showRemoteSignalInDisplay(char *data);

/**
 * \brief Convert a integer to a hex value
 * 
 * \param mint the integer value to convert
 * \param result a pointer to store the hex value in 
 */
void intToHex(uint16_t hex, char *result);

/**
 * \brief Read 4 numbers from remote control
 * 
 * \return the entered number
 */
uint16_t read_user_input(void);

/**
 * \brief get a list of sensor data
 * 
 * \param nrPackets the number of packets we want to retrieve
 * \param packet_ids
 * \param packet_length are it 1 or 2 bytes
 * \param data a array to return the values
 */
void getSensorQueryList(uint8_t nrPackets, uint8_t* packet_ids, uint8_t* packet_length, uint16_t* data);

/**
 *  \brief Store songs for playing them later
 */
void storeSongs(void);

/**
 * \brief Play a stored song with
 * 
 * \param songNr
 */
void playSong(uint8_t songNr);

/**
 * \brief Get from remote control if roomba should be nr 1 or 2
 */
uint8_t getRoombaNrFromRemote(void);

#endif
