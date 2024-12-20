// **************************************************************
//     Filename: WAVTriggerPro.h
// Date Created: 10/15/2024
//
//     Comments: Robertsonics WAV Trigger Pro Qwiic library
//
// **************************************************************

#ifndef _20240810_WAVTRIGGERPRO_H_
#define _20240810_WAVTRIGGERPRO_H_
#include "Wire.h"

#define WAV_TRIGGER_PRO_DEFAULT_ADDR 	0x13 // 7-bit unshifted default I2C Address

#define CMD_GET_VERSION				1
#define CMD_GET_NUM_TRACKS			2
#define CMD_TRACK_PLAY_POLY			3
#define CMD_GET_TRACK_STATUS        4
#define CMD_GET_NUM_ACTIVE_VOICES	5
#define CMD_TRACK_SET_LOOP			6
#define CMD_TRACK_SET_LOCK			7
#define CMD_STOP_ALL                8
#define CMD_TRACK_STOP				9
#define CMD_TRACK_FADE				10
#define CMD_MIDI_MSG				11
#define CMD_LOAD_PRESET				12
#define CMD_SET_OUTPUT_GAIN			13

#define MAX_MESSAGE_LEN				32
#define VERSION_STRING_LEN			12

#define RESPONSE_DELAY_MS			2

class WAVTriggerPro
{
public:
	WAVTriggerPro() {;}
	~WAVTriggerPro() {;}
	bool begin(uint8_t deviceAddress = WAV_TRIGGER_PRO_DEFAULT_ADDR, TwoWire &wirePort = Wire);
	uint8_t getAddress();
	bool getVersion(char *pDst);
	int getNumTracks(void);
	void trackPlayPoly(int track, int gainDb, int bal, int attackMs, int cents, uint8_t flags);
	void trackFade(int track, int gainDb, int timeMs);
	void trackStop(int track, int releaseMs);
	void trackSetLoop(int track, bool loop);
	void trackSetLock(int track, bool lock);
    int trackGetStatus(int track);
    void stopAll(void);
	int getNumActiveVoices(void);
	void sendMidiMsg(uint8_t cmd, uint8_t dat1, uint8_t dat2);
	void loadPreset(int preset);
	void setOutputGain(int gainDb);

private:
	void write(uint8_t * buf, int cnt);
	
	TwoWire *_i2cPort = NULL; // The generic connection to user's chosen I2C hardware
	uint8_t _deviceAddress;   // Address of Tsunami
	char version[VERSION_STRING_LEN];
};

#endif
