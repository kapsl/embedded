#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>

void initializeRoomba(void) {
	// Start command
	send_byte_roomba(128);
		
	// Save mode
	send_byte_roomba(131);
	
	// Store songs
	storeSongs();
}

void set_Display(char *data) {
	send_byte_roomba(164);
	
	uint8_t a = 0;
	while (a < 4) {
		send_byte_roomba(data[a]);
		a++;
	}
}

/**
 * \brief Clear all entries on the display
 */
void clear_Display() {
	char result[4] = {' ', ' ', ' ', ' '};
	set_Display(result);
}

/**
 * \brief Set display by using the 7 segment mode
 * 
 * \param data_bytes array of 4 values, each value gives the combination
 * 			of 7 segments switched on or off
 */
void set_Display_raw(uint8_t *data_bytes) {
	send_byte_roomba(163);
	
	for (uint8_t i = 0; i < 4; i++) {
		send_byte_roomba(data_bytes[i]);
	}
}

/**
 * \brief Find out if a button on the roomba was pressed
 * 
 * \param button a constant fromm roomba.h e.g HOUR
 * \return 0 if button was not pressed, 1 instead
 */
uint8_t read_button(uint8_t button) {
	// Get sensor packets
	send_byte_roomba(142);
	
	// Buttons
	send_byte_roomba(18); 
	
	uint8_t result = receive_byte_roomba();
	
	if ((result & button) == button) {
		return 1;
	}
	
	return 0;
}

/**
 * \brief Read a sensor value
 * 
 * \param packetid the packetid corresponding to the sensor
 * \param data an array where the returned bytes are stored
 * \param length how many bytes should be retrieved
 */
void read_values(uint8_t packetid, uint8_t * data, uint8_t length) {
	send_byte_roomba(142);
	send_byte_roomba(packetid);
	
	for (uint8_t a = 0; a < length; a++) {
		data[a] = receive_byte_roomba();
	}
}

/**
 * \brief Receive a signal from the remote. Show the received
 * 			value as hex on Display and drive corresponding
 * 			to the steering buttons
 */
uint8_t receiveRemote() {
	uint8_t data[2];
	read_values(17, data, 1);
	
	//showRemoteSignalInDisplay(data[0]);
	
	return data[0];
}

/**
 * \brief Show the pressed remote button hex code on display
 * 
 * \param data the received button code
 */
void showRemoteSignalInDisplay(char *data) {
	if (data == 0) {
		char empty[4] = {' ', ' ', ' ', ' '};
		set_Display(empty);
		
		return;
	}
	
	char result[4];
	intToHex((uint16_t) data, result);
	
	set_Display(result);
}

/**
 * \brief Convert a integer to a hex value
 * 
 * \param mint the integer value to convert
 * \param result a pointer to store the hex value in 
 */
void intToHex(uint16_t mint, char *result) {
	sprintf(result, "%4x", mint);
}


/**
 * \brief Read 4 numbers from remote control
 * 
 * \return the entered number
 */
uint16_t read_user_input() {
	char awaitInput[4] = {'-', '-', '-', '-'};
	set_Display(awaitInput);
	
	// Set 99 as dummy value (we can't have bigger numbers than 9)
	uint16_t remoteVal[4] = {99, 99, 99, 99};
	
	// Holds display chars
	char toDisplay[4];
	
	// The actual received value from the remote
	uint8_t actVal;
	
	// Set display to - - - -
	for (uint8_t b = 0; b < 4; b++) {
		toDisplay[b] = '-';	
	}
	
	// 0 is 0x81 = 129
	uint8_t finished = 0;
	uint16_t counter = 0;
	while (counter < 4 && finished == 0) {
		uint8_t flag = 1;

		while(flag) {
			actVal = receiveRemote();
			
			if (actVal != 0) {
				// Check if arrow to right
				if (actVal == REMOTE_RIGHT && remoteVal[counter] != 99) {
					flag = 0;
					counter++;
				} 
				// Input finished
				else if (actVal == REMOTE_POWER && counter > 0) {
					finished = 1;
					break;
				}
				// Check if it is really a number
				else if (actVal != REMOTE_RIGHT && actVal != REMOTE_POWER && actVal >= ROOMBA_0 && actVal <= (ROOMBA_0 + 9)) {
					remoteVal[counter] = actVal - ROOMBA_0;
					toDisplay[counter] = (remoteVal[counter] + ASCI_0);
					
					set_Display(toDisplay);
				}
			}			
		}
	}
	
	// Calculate number
	uint16_t result = 0;
	
	for (uint8_t b = 0; b < counter; b++) {
		result = result * 10;
		result = result + remoteVal[b];
	}
	
	
	return result;
}

/**
 * \brief get a list of sensor data
 * 
 * \param nrPackets the number of packets we want to retrieve
 * \param packet_ids
 * \param packet_length are it 1 or 2 bytes
 * \param data a array to return the values
 */
void getSensorQueryList(uint8_t nrPackets, uint8_t* packet_ids, uint8_t* packet_length, uint16_t* data) {
	send_byte_roomba(149);
	send_byte_roomba(nrPackets);
	
	for (uint8_t a = 0; a < nrPackets; a++) {
		send_byte_roomba(packet_ids[a]);
	}
	
	for (uint8_t b = 0; b < nrPackets; b++) {
		if (packet_length[b] == 1) {
			data[b] = receive_byte_roomba();
		} else {
			uint8_t darray[2];
			darray[0] = receive_byte_roomba();
			darray[1] = receive_byte_roomba();
			
			data[b] = concat_bytes(darray);
		}
	}
}

/**
 * \brief Play a stored song with
 * 
 * \param songNr
 */
void playSong(uint8_t songNr) {
	send_byte_roomba(141);
	send_byte_roomba(songNr);
}

/**
 *  \brief Store songs for playing them later
 *  TODO change songs
 */
void storeSongs() {
	// Song 1 -- hit
	// Send optcode
	send_byte_roomba(140);
	
	// Song nr.
	send_byte_roomba(0);
	
	// Length
	send_byte_roomba(6);
	
	// Note 1, duration 1	
	send_byte_roomba(60);
	send_byte_roomba(16);
	
	// Note 2, duration 2	
	send_byte_roomba(65);
	send_byte_roomba(32);
	
	// Note 3, duration 3	
	send_byte_roomba(60);
	send_byte_roomba(16);
	
	// Note 4, duration 4	
	send_byte_roomba(65);
	send_byte_roomba(32);
	
	// Note 5, duration 5	
	send_byte_roomba(60);
	send_byte_roomba(16);
	
	// Note 6, duration 6	
	send_byte_roomba(65);
	send_byte_roomba(32);
		
	
	// Song 2 -- power-up
	// Send optcode
	send_byte_roomba(140);
	
	// Song nr.
	send_byte_roomba(1);
	
	// Length
	send_byte_roomba(4);
	
	// Note 1, duration 1	
	send_byte_roomba(65);
	send_byte_roomba(8);
	
	// Note 2, duration 2
	send_byte_roomba(70);
	send_byte_roomba(8);
	
	// Note 3, duration 3
	send_byte_roomba(75);
	send_byte_roomba(8);
	
	// Note 4, duration 4
	send_byte_roomba(80);
	send_byte_roomba(8);
	
	
	// Song 3 -- hit
	// Send optcode
	send_byte_roomba(140);
	
	// Song nr.
	send_byte_roomba(2);
	
	// Length
	send_byte_roomba(1);
	
	// Note 1, duration 1	
	send_byte_roomba(60);
	send_byte_roomba(128);

		
	
	// Song 4 -- power-up
	// Send optcode
	send_byte_roomba(140);
	
	// Song nr.
	send_byte_roomba(3);
	
	// Length
	send_byte_roomba(4);
	
	// Note 1, duration 1	
	send_byte_roomba(65);
	send_byte_roomba(8);
	
	// Note 2, duration 2
	send_byte_roomba(70);
	send_byte_roomba(8);
	
	// Note 3, duration 3
	send_byte_roomba(75);
	send_byte_roomba(8);
	
	// Note 4, duration 4
	send_byte_roomba(80);
	send_byte_roomba(8);
}
