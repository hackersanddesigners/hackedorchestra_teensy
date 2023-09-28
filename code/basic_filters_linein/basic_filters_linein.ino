// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
//
// Part 2-7: Filters

#include <Bounce.h>


///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=790,542.5
AudioFilterStateVariable filter1;        //xy=972.25,543
AudioFilterStateVariable filter2;        //xy=974.25,638
AudioMixer4              mixer2;         //xy=1108.25,657
AudioMixer4              mixer1;         //xy=1116.25,547
AudioOutputI2S           i2s1;           //xy=1246.25,592
AudioConnection          patchCord1(i2s2, 0, filter1, 0);
AudioConnection          patchCord2(i2s2, 1, filter2, 0);
AudioConnection          patchCord3(filter1, 0, mixer1, 0);
AudioConnection          patchCord4(filter1, 1, mixer1, 1);
AudioConnection          patchCord5(filter1, 2, mixer1, 2);
AudioConnection          patchCord6(filter2, 0, mixer2, 0);
AudioConnection          patchCord7(filter2, 1, mixer2, 1);
AudioConnection          patchCord8(filter2, 2, mixer2, 2);
AudioConnection          patchCord9(mixer2, 0, i2s1, 1);
AudioConnection          patchCord10(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=836.25,721
// GUItool: end automatically generated code


// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7   // Teensy 4 ignores this, uses pin 11
#define SDCARD_SCK_PIN   14  // Teensy 4 ignores this, uses pin 13

// Bounce objects to read pushbuttons
Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);  // 15 ms debounce time
Bounce button2 = Bounce(2, 15);

void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(30);     //sensitivity of the line in (0-15?)

  mixer1.gain(0, 0.0);
  mixer1.gain(1, 1.0);  // default to hearing band-pass signal
  mixer1.gain(2, 0.0);
  mixer1.gain(3, 0.0);
  mixer2.gain(0, 0.0);
  mixer2.gain(1, 1.0);
  mixer2.gain(2, 0.0);
  mixer2.gain(3, 0.0);
  delay(1000);
}

void loop() {

  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();

  if (button0.fallingEdge()) {
    Serial.println("Low Pass Signal");
    mixer1.gain(0, 1.0);  // hear low-pass signal
    mixer1.gain(1, 0.0);
    mixer1.gain(2, 0.0);
    mixer2.gain(0, 1.0);
    mixer2.gain(1, 0.0);
    mixer2.gain(2, 0.0);
  }
  if (button1.fallingEdge()) {
    Serial.println("Band Pass Signal");
    mixer1.gain(0, 0.0);
    mixer1.gain(1, 1.0);  // hear low-pass signal
    mixer1.gain(2, 0.0);
    mixer2.gain(0, 0.0);
    mixer2.gain(1, 1.0);
    mixer2.gain(2, 0.0);
  }
  if (button2.fallingEdge()) {
    Serial.println("High Pass Signal");
    mixer1.gain(0, 0.0);
    mixer1.gain(1, 0.0);
    mixer1.gain(2, 1.0);  // hear low-pass signal
    mixer2.gain(0, 0.0);
    mixer2.gain(1, 0.0);
    mixer2.gain(2, 1.0);
  }

  // read the knob and adjust the filter frequency
  int knob = analogRead(A3);
  // quick and dirty equation for exp scale frequency adjust
  float freq =  expf((float)knob / 150.0) * 10.0 + 80.0;
  filter1.frequency(freq);
  filter2.frequency(freq);
  Serial.print("frequency = ");
  Serial.println(freq);
  delay(200);
}
