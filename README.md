Robertsonics **WAV Trigger Pro** Qwiic Arduino Library
==================================================

The WAV Trigger Pro is a high-peformance, low latency polyphonic audio player with extensive USB MIDI capabilities.

This Arduino Library is capable of controlling the WAV Trigger Pro via I2C commands using a Qwiic Cable. For connection details, please refer to the Hookup Guide on the SparkFun product page.

In all cases below, the range for **t** (track number) is 1 through 4096.

Usage:
======

#include "WAVTriggerPro.h"

WAVTriggerPro wtp;

Wire.begin()

**wtp.begin()** - you must call this method first to establish the I2C communications. Returns true if a WAV Trigger Pro was found at the default I2C address of 0x13. Returns false if no response from the WAV Trigger Pro.
  
**wtp.getAddress()** - returns the Qwiic address of the WAV Trigger Pro.

**wtp.getVersion(char * pDst)** - this function will return a null-terminated version string to the location specified by pointer **pDst**. The calling function should allocate a minimum of VERSION_STRING_LEN + 1 = 13 bytes to pDst to prevent buffer overflow.

**wtp.getNumTracks()** - Returns number of tracks found on the installed microSD card.

**wtp.trackPlayPoly(int track, int gainDb, int bal, int attackMs, int cents, uint8_t flags)** - This function starts track number **t** from the beginning, blending it with any other tracks that are currently playing, including potentially another copy of the same track.

    gainDb is the target gain attenuation for the track, from 0 (full volume) to -80.

    bal is the left/right balance for the track. 0 = full left, 64 = center, 127 = full right

    attackMs is the attack time in milliseconds

    cents specifies the pitch offset for the track instance, from -700 to +700. 100 cents is equal to one musical semi-tone.

    flags Any combination of LOOP_FLAG | LOCK_FLAG | PITCH_BEND_FLAG

        LOOP_FLAG: Track will loop over file length
        LOCK_FLAG: Track will not yield to voice stealing algorithm
        PITCH_BEND_FLAG: Track will follow MIDI PitchBend messages

**wtp.trackFade(int t, int gain, int time)** - this command initiates a hardware volume fade on all instances of track number **t** if it is currently playing. The track volume will transition smoothly from the current value to the target gain in the specified number of milliseconds. Use this command to smoothly change the volume of a playing track. To fade out and stop a track, use the trackStop() command below.

**wtp.trackStop(int track, int releaseMs)** - This command stops any instance of the specified track using the supplied release value in milliseconds. The track will fade and then stop.

**trackSetLoop(int track, bool loop)** - Sets or resets the track loop flag.

**trackSetLock(int track, bool loop)** - Sets or resets the track lock flag.

**trackGetStatus(int track)** - returns 1 if one or more instances of the specified track are currently playing, otherwise returns 0.

**stopAll()** - This command stops all audio.

**getNumActiveVoices()** - This command returns the number of voices currently active.

**sendMidiMsg(uint8_t cmd, uint8_t dat1, uint8_t dat2)** - This command sends a standard 3-byte MIDI message to the WAV Trigger Pro, just as if it had come from a MIDI Controller or Keyboard.

**loadPreset(int preset)** - This command tells the WAV Trigger Pro to load the specified preset from the microSD card.

**setOutputGain(int gainDb)** - This sets the overall output gain of the WAV Trigger Pro's stereo audio output in dB, from 0 to -80dB.

Examples
========

The libary includes a number of example sketches to demonstrate playing and controlling audio. These examples were developed using the SparkFun Redboard Qwiic, and do not require any other hardware. They do require a set of demo sounds on the microSD card. These sounds can be found here:

[microSD image for examples](https://drive.google.com/file/d/17SlXueDCNjg29a56VSZPSQ7Gt-WlDesp/view?usp=sharing)

Unzip the archive, copy all the .wav files to an otherwise empty microSD card and install in your WAV Trigger Pro before running the examples.


