// ****************************************************************************
//       Sketch: WAV Trigger Pro Qwiic Library Example 2
//               Playing tracks
// Date Created: 1/3/2025
//
//     Comments: This example continuously plays the eight spoken number
//               tracks in order, to alternating left right output channels.
//
//     Requires: The WAV Trigger Pro Arduino example tracks should be
//               installed on the microSD card:
//
//        Tracks  1 - 8: Spoken numbers
//       Tracks 48 - 84: Piano notes
//            Track 100: Stereo A440 Sine Wave, 8 secs
//            Track 101: Mono 1K Sine Wave, 8 secs
//            Track 102: Stereo ambient music
//            Track 103: Stereo acoustic guitar
//            Track 104: Dialog 1
//            Track 105: Dialog 2
//     Tracks 200 - 208: EDM percussion
//
// ****************************************************************************

#include "WAVTriggerPro.h"

#define LED 13  // Our LED

#define HEARTBEAT_PERIOD_MS 2000
#define POLLING_PERIOD_MS 500

// ****************************************************************************
// Global variables

WAVTriggerPro wtp;  // Our WAV Trigger Pro object

unsigned long gMsTick;
unsigned long gLastFlashMs;
unsigned long gLastPollMs;
int gLedTimeout = 0;
int gNumTracks = 0;
int gNumActiveVoices = 0;
int gTrackNum;
int gBalance;

// Allocate a buffer for the version string
char gVersion[VERSION_STRING_LEN + 2];

// ****************************************************************************
// setup
// ****************************************************************************
void setup() {

  // Initialize the LED pin
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);

  Wire.begin();
  Serial.begin(9600);

  // Provide time for the WAV Trigger Pro to finish reset before trying to send
  //  any commands.
  delay(10);

  // WAV Trigger Pro initialization
  if (!wtp.begin()) {
    Serial.print("Could not start the WAV Trigger Pro\n");
  } else {
    Serial.print("\n*******************\n");
    Serial.print("Qwiic device found\n");
    Serial.print("Address: ");
    Serial.print(wtp.getAddress());
    Serial.print("\n");
  }

  // Issue a stop all tracks
  wtp.stopAll();
  delay(5);

  // Get the version string and number of available tracks
  if (wtp.getVersion(gVersion)) {
    Serial.print("Version: ");
    Serial.print(gVersion);
    Serial.print("\n");
    //delay(5);
    gNumTracks = wtp.getNumTracks();
    Serial.print("Number of tracks: ");
    Serial.print(gNumTracks);
    Serial.print("\n");
    //delay(5);
  } else
    Serial.print("WAV Trigger Pro response not available");

  gMsTick = millis();
  gLastFlashMs = gMsTick;
  gLastPollMs = gMsTick;
  gTrackNum = 0;
  gBalance = 0;
}

// ****************************************************************************
// loop
// ****************************************************************************
void loop() {

  // ================== MAIN LOOP TASK 1 ===================
  // Play next track when the previous is finished
  if ((gMsTick - gLastPollMs) > POLLING_PERIOD_MS) {
    gLastPollMs = gMsTick;

    // Wait for no tracks playing
    gNumActiveVoices = wtp.getNumActiveVoices();
    if (gNumActiveVoices == 0) {
      if (++gTrackNum > 8)
        gTrackNum = 1;

      Serial.print("Play track: ");
      Serial.print(gTrackNum);
      Serial.print("\n");

      wtp.trackPlayPoly(gTrackNum, 0, gBalance, 0, 0, 0);

      // Next track should be to the opposite channel
      if (gBalance > 0)
        gBalance = 0;
      else
        gBalance = 127;
    }
  }

  // ================== MAIN LOOP TASK 2 ===================
  // Flash the hearbeat LED
  if ((gMsTick - gLastFlashMs) > HEARTBEAT_PERIOD_MS) {
    gLastFlashMs = gMsTick;
    gLedTimeout = 25;
    digitalWrite(LED, 1);
  }


  // ================== MAIN LOOP TASK 3 ===================
  // Keep gMsTick up to date
  if (millis() > gMsTick) {
    gMsTick = millis();
    if (gLedTimeout > 0) {
      if (--gLedTimeout == 0) {
        digitalWrite(LED, 0);
      }
    }
  }
}
