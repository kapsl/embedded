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
void set_Display_raw(uint8_t *data_bytes);
void clear_Display(void);

uint8_t read_button(uint8_t button);
void read_values(uint8_t packetid, uint8_t * data, uint8_t length);
void drive(int16_t velocity);
void stop(void);
void drive_roomba(uint16_t distance, int16_t velocity);
void drive_roomba_exact(uint16_t distance, int16_t velocity);
uint16_t getTicks(void);
uint8_t receiveRemote(void);
void showRemoteSignalInDisplay(char *data);
void intToHex(uint16_t hex, char *result);
void driveWithRadius(uint16_t velocity, uint16_t radius);
uint16_t read_user_input(void);
void drive_circle(uint16_t radius, uint16_t velocity);
void turn(int16_t degree);
void drive_autonom(void);
void set_back(void);
void drive_with_p_regler(void);
void getSensorQueryList(uint8_t nrPackets, uint8_t* packet_ids, uint8_t* packet_length, uint16_t* data);
void bump_handling(uint8_t bump);
void storeSongs(void);
void playSong(uint8_t songNr);

#endif
