// ****************************************************************************
//       Sketch: WAV Trigger Pro Qwiic Library Example 3
//               Using MIDI
// Date Created: 1/3/2025
//
//     Comments: This example uses MIDI to play tracks.
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

#define TEMPO_PERIOD 10

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80

#define NOTE_01 50
#define NOTE_02 54
#define NOTE_03 57

#define NOTE_11 49
#define NOTE_12 52
#define NOTE_13 56

#define NOTE_21 115
#define NOTE_22 112


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

      // State 0: Play the first chord
      case 0:
        if (gDelayCount > 0)
          gDelayCount--;
        else {
          Serial.println("Playing chord 1");
          wtp.sendMidiMsg(MIDI_NOTE_OFF, NOTE_11, 0);
          wtp.sendMidiMsg(MIDI_NOTE_OFF, NOTE_12, 0);
          wtp.sendMidiMsg(MIDI_NOTE_OFF, NOTE_13, 0);
          wtp.sendMidiMsg(MIDI_NOTE_ON, NOTE_01, 100);
          delay(50);
          wtp.sendMidiMsg(MIDI_NOTE_ON, NOTE_02, 100);
          delay(50);
          wtp.sendMidiMsg(MIDI_NOTE_ON, NOTE_03, 100);
          gState++;
          gDelayCount = TEMPO_PERIOD;  // Delay 3
        }
        break;

      case 1:
        if (gDelayCount > 0)
          gDelayCount--;
        else {
          Serial.println("Playing perc 1");
          wtp.sendMidiMsg(MIDI_NOTE_OFF, NOTE_21, 0);
          wtp.sendMidiMsg(MIDI_NOTE_OFF, NOTE_22, 0);
          wtp.sendMidiMsg(MIDI_NOTE_ON, NOTE_21, 100);
          gState++;
          gDelayCount = TEMPO_PERIOD;  // Delay 3
        }
        break;

      // State 1: Play the second chord
      case 2:
        if (gDelayCount > 0)
          gDelayCount--;
        else {
          Serial.println("Playing chord 2");
          wtp.sendMidiMsg(MIDI_NOTE_OFF, NOTE_01, 0);
          wtp.sendMidiMsg(MIDI_NOTE_OFF, NOTE_02, 0);
          wtp.sendMidiMsg(MIDI_NOTE_OFF, NOTE_03, 0);
          wtp.sendMidiMsg(MIDI_NOTE_ON, NOTE_11, 85);
          wtp.sendMidiMsg(MIDI_NOTE_ON, NOTE_12, 85);
          wtp.sendMidiMsg(MIDI_NOTE_ON, NOTE_13, 85);
          gState++;
          gDelayCount = TEMPO_PERIOD;  // Delay
        }
        break;

      case 3:
        if (gDelayCount > 0)
          gDelayCount--;
        else {
          Serial.println("Playing perc 2");
          wtp.sendMidiMsg(MIDI_NOTE_ON, NOTE_22, 100);
          gState = 0;
          gDelayCount = TEMPO_PERIOD;  // Delay
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
