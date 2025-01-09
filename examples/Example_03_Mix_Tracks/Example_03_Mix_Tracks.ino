// ****************************************************************************
//       Sketch: WAV Trigger Pro Qwiic Library Example 3
//               Mixing tracks
// Date Created: 1/3/2025
//
//     Comments: This example combines music and dialog tracks.
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
#define POLLING_PERIOD_MS 100

// ****************************************************************************
// Global variables

WAVTriggerPro wtp;  // Our WAV Trigger Pro object

unsigned long gMsTick;
unsigned long gLastFlashMs;
unsigned long gLastPollMs;
int gLedTimeout = 0;
int gNumTracks = 0;
int gNumActiveVoices = 0;
int gDelayCount = 0;
int gState = 0;

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
  gDelayCount = 10;
  gState = 0;
}

// ****************************************************************************
// loop
// ****************************************************************************
void loop() {

  // ================== MAIN LOOP TASK 1 ===================
  // Proceed to the next state when the time comes
  if ((gMsTick - gLastPollMs) > POLLING_PERIOD_MS) {
    gLastPollMs = gMsTick;

    // Process according to state
    switch (gState) {

      // State 0: start the ambient music
      case 0:
        if (gDelayCount > 0)
          gDelayCount--;
        else {
          Serial.println("Starting music 1");
          wtp.trackPlayPoly(102, 0, BALANCE_MID, 0, 0, 0);
          gState++;
          gDelayCount = 60;  // Delay 6 seconds
        }
        break;

      // State 1: start dialog 2 track
      case 1:
        if (gDelayCount > 0)
          gDelayCount--;
        else {
          Serial.println("Starting dialog 1");
          wtp.trackPlayPoly(105, 0, BALANCE_MID, 0, 0, 0);
          gState++;
        }
        break;

      // State 2: wait for dialog track to finish
      case 2:
        if (wtp.trackGetStatus(105) == 0) {
          Serial.println("Cross-fading to music 2 ");
          wtp.trackPlayPoly(103, 0, BALANCE_MID, 4000, 0, 0);
          gState++;
          gDelayCount = 20;  // Delay 1 second
        }
        break;

      // State 3: crossfade to music 2
      case 3:
        if (gDelayCount > 0)
          gDelayCount--;
        else {
          wtp.trackStop(102, 4000);
          gState++;
          gDelayCount = 30;
        }
        break;

      // Do nothing
      default:
        break;
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
