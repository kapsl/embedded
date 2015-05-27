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
 * TODO
 */
void clear_Display() {
	char result[4] = {' ', ' ', ' ', ' '};
	set_Display(result);
}

/**
 * TODO
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
 * \brief Let the roomba drive
 * 
 * \param velocity the velocity in mm/s. If negative --> backwards
 */
void drive(int16_t velocity) {
	send_byte_roomba(145);
	
	uint8_t low = velocity;
	uint8_t high = (velocity >> 8);
	
	send_byte_roomba(high);
	send_byte_roomba(low);
	
	send_byte_roomba(high);
	send_byte_roomba(low);
}



/**
 * Stop the roomba
 */
void stop() {
	drive(0);
}

/**
 * \brief Drive the roomba
 * 
 * \param distance so far
 * \param velocity with this velocity
 */
void drive_roomba(uint16_t distance, int16_t velocity) {
	// Start tick
	char buff[50];
	uint16_t start_value = getTicks();
	sprintf(buff, "Ticks-Start: %u\r\n", start_value);
	sendString(buff);
	
	drive(velocity);
	
	uint16_t sleep = distance / abs(velocity);
	my_msleep(sleep * 1000);
	
	stop();
	
	uint16_t end_value = getTicks();
	sprintf(buff, "Ticks-End: %u\r\n", end_value);
	sendString(buff);
	sprintf(buff, "Ticks: %u\r\n", (end_value - start_value));
	sendString(buff);
	
	// 1m = 2262 Ticks
}

/**
 * \brief Does the same like drive_roomba but more exact with counting ticks
 */
void drive_roomba_exact(uint16_t distance, int16_t velocity) {
	uint16_t start_value = getTicks();
	
	drive(velocity);
	
	char buff[50];
	sprintf(buff, "Ticks-Start: %u\r\n", start_value);
	sendString(buff);
	
	uint16_t maxTicks = (uint16_t) (distance * 2.262);
	sprintf(buff, "Max: %u\r\n", maxTicks);
	sendString(buff);
	
	my_msleep(200);
	
	while (1) {
		uint16_t ticks = getTicks();
		uint16_t distanceTicks;
		
		// Overflow
		if (ticks < start_value && velocity > 0) {
			distanceTicks = 65535 - start_value + ticks;
		} else if (ticks > start_value && velocity < 0) {
			distanceTicks = start_value + (65535 - ticks);
		} else {
			distanceTicks = abs(ticks - start_value);
		}
		
		if (distanceTicks > maxTicks) {
			break;
		}
	}
	
	stop();
}

/**
 * \brief Get the actual tick count for the left wheel
 * 			(at the moment enough, as long as we only drive straight)
 */
uint16_t getTicks() {
	uint8_t data[2];
	read_values(43, data, 2);
	
	return concat_bytes(data);
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
 */
void intToHex(uint16_t hex, char *result) {
	sprintf(result, "%4x", hex);
}

/**
 * \brief Drive roomba with
 * 
 * \param velocity this velocity
 * \param radius this radius (use DRIVE_STRAIGHT for straight, -1 for 
 * 			clockwise 1 for anticlockwise)
 */
void driveWithRadius(uint16_t velocity, uint16_t radius) {
	send_byte_roomba(137);
	
	uint8_t low = velocity;
	uint8_t high = (velocity >> 8);
	
	send_byte_roomba(high);
	send_byte_roomba(low);	
	
	uint8_t rlow = radius;
	uint8_t rhigh = (radius >> 8);
	
	send_byte_roomba(rhigh);
	send_byte_roomba(rlow);	
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
 * \brief Drive a circle 
 * 
 * \param radius the radius to drive (with bigger radiuses it gets much bigger)
 * \param velocity
 */
void drive_circle(uint16_t radius, uint16_t velocity) {
	driveWithRadius(velocity, radius);
}

/**
 * \brief Turn the robot by
 * 
 * \param degree 
 */
void turn(int16_t degree) {
	// In 13 seconds, 360°
	uint16_t delay = (13000 / 360) * abs(degree);
	
	if (degree > 0) {
		driveWithRadius(50, 1);
	} else {
		driveWithRadius(50, -1);
	}
	
	my_msleep(delay);
	
	driveWithRadius(0, 0);
}

/**
 * \brief drive straight, till a bumper sensor is active,
 * 		set-back and turn
 */
void drive_autonom() {
	while (1) {
		drive(150);
		
		// Bumpers
		uint8_t data[2];
		read_values(7, data, 1);
			
		bump_handling(data[0]);
	}
}

/**
 * \brief Set back and turn, if a bump is registered
 * 
 * \param bump the byte where the bump values are stored
 */
void bump_handling(uint8_t bump) {
	if ((bump & 0x02) == 0x02) {
		// Left bumper
		set_back();
		turn(-80);
	} else if ((bump & 0x01) == 0x01) {
		// Right bumper
		set_back();
		turn(45);
	} 
}

/**
 * \brief Drive a bit backwards
 */
void set_back() {
	drive(-100);
	my_msleep(1700);
	drive(0);
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
 * \brief Drive along aline with a p regler
 */
void drive_with_p_regler() {
	uint8_t speed = 250;
	uint8_t abtastzeit = 30;
	
	uint16_t v_right = speed;
	uint16_t v_left = speed;
	
	float kp = 0.1f;
	float kd = 0.08f;
	float ki = 0.00005f;
	
	// Cliff left, right / bumper 
	uint8_t packet_ids[3] = {29, 30, 7};
	uint8_t packet_length[3] = {2, 2, 1};
	uint16_t qdata[3];
	int32_t esum = 0;
	int16_t ealt = 0;
		
	while (1) {
		drive_direction(v_right, v_left);
		
		// Set speed to straight normal speed
		v_right = speed;
		v_left = speed;
		
		getSensorQueryList(3, packet_ids, packet_length, qdata);
		
		int16_t cliff_left = qdata[0];
		int16_t cliff_right = qdata[1];
		
		int16_t e = cliff_left - cliff_right;
		esum += e;
		int32_t y = kp * e + ki * abtastzeit * esum + kd * (e - ealt) / abtastzeit;
		ealt = e;
		
		// Output on display
		char result[4];
		intToHex(abs(y), result);
		set_Display(result);
		
		// Compensate direction
		v_right += y;
		v_left -= y;
		
		bump_handling(qdata[2]);
		
		my_msleep(abtastzeit);
	}	
		
}

/**
 * \brief Play a stored song with
 * 
 * \param songNr
 */
void playSong(uint8_t songNr) {
	send_byte_roomba(141);
	send_byte_roomba(0);
}

/**
 *  \brief Store songs for playing them later
 */
void storeSongs() {
	// Programm songs
	send_byte_roomba(140);
	
	// Song 1 -- shoot
	// Song nr.
	send_byte_roomba(0);
	
	// Length
	send_byte_roomba(3);
	
	// Note 1, duration 1
	send_byte_roomba(87);
	send_byte_roomba(16);
	
	send_byte_roomba(88);
	send_byte_roomba(16);
	
	send_byte_roomba(89);
	send_byte_roomba(16);
}
