// Granular Effect Example - Pitch shift or freeze sound
//
// This example is meant to be used with 3 buttons (pin 0,
// 1, 2) and 2 knobs (pins 16/A2, 17/A3), which are present
// on the audio tutorial kit.
//   https://www.pjrc.com/store/audio_tutorial_kit.html
//
// Data files to put on your SD card can be downloaded here:
//   http://www.pjrc.com/teensy/td_libs_AudioDataFiles.html
//
// This example code is in the public domain.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=898.75,251.25
AudioMixer4              mixer1;         //xy=1072.75,330.75
AudioEffectGranular      granular1;      //xy=1244.7500190734863,318.75000381469727
AudioOutputI2S           i2s1;           //xy=1404.75,348.75
AudioConnection          patchCord1(i2s2, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, granular1);
AudioConnection          patchCord3(granular1, 0, i2s1, 0);
AudioConnection          patchCord4(granular1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=976.75,411.75
// GUItool: end automatically generated code


Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);
Bounce button2 = Bounce(2, 15);

#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7   // Teensy 4 ignores this, uses pin 11
#define SDCARD_SCK_PIN   14  // Teensy 4 ignores this, uses pin 13

void setup() {
  Serial.begin(9600);
  AudioMemory(10);

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(30);     //sensitivity of the line in (0-15?)

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);

  // the Granular effect requires memory to operate
  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);
}

void loop() {

  // read pushbuttons
  button0.update();
  button1.update();
  button2.update();

  // read knobs, scale to 0-1.0 numbers
  float knobA1 = (float)analogRead(A1) / 1023.0;
  float knobA2 = (float)analogRead(A2) / 1023.0;
  float knobA3 = (float)analogRead(A3) / 1023.0;


  // Button 0 starts Freeze effect
  if (button0.fallingEdge()) {
    float msec = 100.0 + (knobA3 * 190.0);  // range 100-200.000
    granular1.beginFreeze(msec);
    Serial.print("Begin granular freeze using ");
    Serial.print(msec);
    Serial.println(" grains");
  }
  if (button0.risingEdge()) {
    granular1.stop();
  }

  // Button 1 starts Pitch Shift effect
  if (button1.fallingEdge()) {
    float msec = 25.0 + (knobA3 * 75.0);
    granular1.beginPitchShift(msec);
    Serial.print("Begin granular pitch phift using ");
    Serial.print(msec);
    Serial.println(" grains");
  }
  if (button1.risingEdge()) {
    granular1.stop();
  }

  // Continuously adjust the speed, based on the A3 pot
  float ratio;
  ratio = powf(2.0, knobA2 * 2.0 - 1.0); // 0.5 to 2.0 -- beastie boys
//  ratio = powf(2.0, knobA2 * 6.0 - 3.0); // 0.125 to 8.0 -- daftpunky
  granular1.setSpeed(ratio);
//  Serial.print("Ratio is ");
//  Serial.println(ratio);
}
