#include "roomba.h"
#include "tools.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>

void initializeRoomba(void) {
	// Start command
	send_byte_roomba(128);
		
	// Save mode
	send_byte_roomba(132);
	
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

void clear_Display() {
	char result[4] = {' ', ' ', ' ', ' '};
	set_Display(result);
}

void set_Display_raw(uint8_t *data_bytes) {
	send_byte_roomba(163);
	
	for (uint8_t i = 0; i < 4; i++) {
		send_byte_roomba(data_bytes[i]);
	}
}

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

void read_values(uint8_t packetid, uint8_t * data, uint8_t length) {
	send_byte_roomba(142);
	send_byte_roomba(packetid);
	
	for (uint8_t a = 0; a < length; a++) {
		data[a] = receive_byte_roomba();
	}
}

uint8_t receiveRemote() {
	uint8_t data[2];
	read_values(17, data, 1);
	
	//showRemoteSignalInDisplay(data[0]);
	
	return data[0];
}

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

void intToHex(uint16_t mint, char *result) {
	sprintf(result, "%4x", mint);
}

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


void playSong(uint8_t songNr) {
	send_byte_roomba(141);
	send_byte_roomba(songNr);
}

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
		
	
	// Song 2 -- power-up and game over
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
	
	
	// Song 3 -- Shooting
	// Send optcode
	send_byte_roomba(140);
	
	// Song nr.
	send_byte_roomba(2);
	
	// Length
	send_byte_roomba(4);
	
	// Note 1, duration 1	
	send_byte_roomba(80);
	send_byte_roomba(10);
	
	send_byte_roomba(80);
	send_byte_roomba(10);
	
	send_byte_roomba(80);
	send_byte_roomba(10);
	
	send_byte_roomba(80);
	send_byte_roomba(10);

		
	
	// Song 4 -- No hit, because of mushroom
	// Send optcode
	send_byte_roomba(140);
	
	// Song nr.
	send_byte_roomba(3);
	
	// Length
	send_byte_roomba(4);
	
	// Note 1, duration 1	
	send_byte_roomba(80);
	send_byte_roomba(8);
	
	
	// Note 2, duration 2
	send_byte_roomba(75);
	send_byte_roomba(8);
	
	// Note 3, duration 3
	send_byte_roomba(70);
	send_byte_roomba(8);
	
	// Note 4, duration 4
	send_byte_roomba(65);
	send_byte_roomba(8);
}

uint8_t getRoombaNrFromRemote() {
	while (1) {
		uint8_t result = read_user_input();
	
		//TODO for testing
		//uint16_t result = 1;
	
		// Error, when not 1 or 2
		if (result != 1 && result != 2) {
			char result[4] = {'1', 'O', 'R', '2'};
			set_Display(result);
			
			my_msleep(2000);
		} else {
			return result;
		}
	}
}
