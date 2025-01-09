// **************************************************************
//     Filename: WAVTriggerPro.cpp
// Date Created: 10/15/2024
//
//     Comments: Robertsonics WAV Trigger Pro Qwiic library
//
// **************************************************************

#include "WAVTriggerPro.h"
#include <Arduino.h>


// **************************************************************
bool WAVTriggerPro::begin(uint8_t deviceAddress, TwoWire &wirePort) {
	_i2cPort = &wirePort;			// Chooses the wire port of the device
	_deviceAddress = deviceAddress; // Sets the address of the device

	//make sure the Tsunami will acknowledge over I2C
	_i2cPort->beginTransmission(_deviceAddress);
	if (_i2cPort->endTransmission() != 0)
	{
		return false;
	}
	return true;
}

// **************************************************************
uint8_t WAVTriggerPro::getAddress() {
	return _deviceAddress;
}

// **************************************************************
bool WAVTriggerPro::getVersion(char *pDst) {

uint8_t txbuf[2];
char * pStr = pDst;

	txbuf[0] = CMD_GET_VERSION;
	write(txbuf, 1);
	delay(RESPONSE_DELAY_MS);
	_i2cPort->requestFrom(_deviceAddress, uint8_t(VERSION_STRING_LEN));
	while (_i2cPort->available()) {
		*pDst++ = _i2cPort->read();
	}
	*pDst = 0;
	return true;
}

// **************************************************************
int WAVTriggerPro::getNumTracks(void) {

int i = 0;
uint8_t txbuf[2];
uint8_t rxbuf[MAX_MESSAGE_LEN];
uint16_t numTracks;

	txbuf[0] = CMD_GET_NUM_TRACKS;
	write(txbuf, 1);
	delay(RESPONSE_DELAY_MS);
	_i2cPort->requestFrom(_deviceAddress, uint8_t(2));
	while (_i2cPort->available()) {
		rxbuf[i++] = _i2cPort->read();
	}
	numTracks = rxbuf[1];
	numTracks = (numTracks << 8) + rxbuf[0];
	return (int)numTracks;
}

// **************************************************************
void WAVTriggerPro::trackPlayPoly(int track, int gainDb, int bal, int attackMs, int cents, uint8_t flags) {

uint8_t txbuf[12];
unsigned short tmp16;

	txbuf[0] = CMD_TRACK_PLAY_POLY;
    txbuf[1] = (uint8_t)track;
    txbuf[2] = (uint8_t)(track >> 8);
	tmp16 = (unsigned short)gainDb;
    txbuf[3] = (uint8_t)tmp16;
    txbuf[4] = (uint8_t)(tmp16 >> 8);
	txbuf[5] = (uint8_t)bal;
	txbuf[6] = (uint8_t)attackMs;
	txbuf[7] = (uint8_t)(attackMs >> 8);
	tmp16 = (unsigned short)cents;
	txbuf[8] = (uint8_t)tmp16;
	txbuf[9] = (uint8_t)(tmp16 >> 8);
	txbuf[10] = flags;
	write(txbuf, 11);
}

// **************************************************************
void WAVTriggerPro::trackFade(int track, int gainDb, int timeMs) {

uint8_t txbuf[8];
unsigned short tmp16;

	txbuf[0] = CMD_TRACK_FADE;
    txbuf[1] = (uint8_t)track;
    txbuf[2] = (uint8_t)(track >> 8);
	tmp16 = (unsigned short)gainDb;
    txbuf[3] = (uint8_t)tmp16;
    txbuf[4] = (uint8_t)(tmp16 >> 8);
	txbuf[5] = (uint8_t)timeMs;
	txbuf[6] = (uint8_t)(timeMs >> 8);
	write(txbuf, 7);
}

// **************************************************************
void WAVTriggerPro::trackSetLoop(int track, bool loop) {

uint8_t txbuf[6];

	txbuf[0] = CMD_TRACK_SET_LOOP;
    txbuf[1] = (uint8_t)track;
    txbuf[2] = (uint8_t)(track >> 8);
	if (loop) {
		txbuf[3] = 1;
	}
	else {
		txbuf[3] = 0;
	}
	write(txbuf, 4);
}

// **************************************************************
void WAVTriggerPro::trackSetLock(int track, bool lock) {

uint8_t txbuf[6];

	txbuf[0] = CMD_TRACK_SET_LOCK;
    txbuf[1] = (uint8_t)track;
    txbuf[2] = (uint8_t)(track >> 8);
	if (lock) {
		txbuf[3] = 1;
	}
	else {
		txbuf[3] = 0;
	}
	write(txbuf, 4);
}

// **************************************************************
void WAVTriggerPro::trackStop(int track, int releaseMs) {

uint8_t txbuf[6];

	txbuf[0] = CMD_TRACK_STOP;
    txbuf[1] = (uint8_t)track;
    txbuf[2] = (uint8_t)(track >> 8);
	txbuf[3] = (uint8_t)releaseMs;
	txbuf[4] = (uint8_t)(releaseMs >> 8);
	write(txbuf, 5);
}

// **************************************************************
int WAVTriggerPro::getNumActiveVoices(void) {

int i = 0;
uint8_t txbuf[2];
uint8_t rxbuf[MAX_MESSAGE_LEN];
uint16_t numVoices;

	txbuf[0] = CMD_GET_NUM_ACTIVE_VOICES;
	write(txbuf, 1);
	delay(RESPONSE_DELAY_MS);
	_i2cPort->requestFrom(_deviceAddress, uint8_t(1));
	while (_i2cPort->available()) {
		rxbuf[i++] = _i2cPort->read();
	}
	numVoices = rxbuf[0];
	return (int)numVoices;
}

// **************************************************************
int WAVTriggerPro::trackGetStatus(int track) {

int i = 0;
uint8_t txbuf[4];
uint8_t rxbuf[MAX_MESSAGE_LEN];
uint16_t trackStat;

	txbuf[0] = CMD_GET_TRACK_STATUS;
    txbuf[1] = (uint8_t)track;
    txbuf[2] = (uint8_t)(track >> 8);
	write(txbuf, 3);
	delay(RESPONSE_DELAY_MS);
	_i2cPort->requestFrom(_deviceAddress, uint8_t(1));
	while (_i2cPort->available()) {
		rxbuf[i++] = _i2cPort->read();
	}
	trackStat = rxbuf[0];
 	return (int)trackStat;
}

// **************************************************************
void WAVTriggerPro::stopAll(void) {

uint8_t txbuf[2];

    txbuf[0] = CMD_STOP_ALL;
    write(txbuf, 1);
}

// **************************************************************
void WAVTriggerPro::sendMidiMsg(uint8_t cmd, uint8_t dat1, uint8_t dat2) {

uint8_t txbuf[6];

	txbuf[0] = CMD_MIDI_MSG;
	txbuf[1] = cmd;
	txbuf[2] = dat1 & 0x7f;
	txbuf[3] = dat2 & 0x7f;
	write(txbuf, 4);
}

// **************************************************************
void WAVTriggerPro::loadPreset(int preset) {

uint8_t txbuf[4];

	txbuf[0] = CMD_LOAD_PRESET;
    txbuf[1] = (uint8_t)preset;
    txbuf[2] = (uint8_t)(preset >> 8);
	write(txbuf, 3);
}

// **************************************************************
void WAVTriggerPro::setOutputGain(int gainDb) {

uint8_t txbuf[4];
unsigned short tmp16;

	txbuf[0] = CMD_SET_OUTPUT_GAIN;
	tmp16 = (unsigned short)gainDb;
    txbuf[1] = (uint8_t)tmp16;
    txbuf[2] = (uint8_t)(tmp16 >> 8);
	write(txbuf, 3);
}

// **************************************************************
void WAVTriggerPro::write(uint8_t * buf, int cnt) {

int bCnt = 0;
int i;

	_i2cPort->beginTransmission(_deviceAddress);
	for (i = 0; i < cnt; i++)
		_i2cPort->write(buf[bCnt++]);
	_i2cPort->endTransmission();
}




