// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
//
// Part 2-6: Feedback (Echo) Delay



///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=75,158
AudioMixer4              mixer1;         //xy=258,191
AudioEffectDelay         delay1;         //xy=267,405
AudioOutputI2S           i2s1;           //xy=446.4285659790039,198.14285469055176
AudioConnection          patchCord1(i2s2, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, delay1);
AudioConnection          patchCord3(delay1, 0, mixer1, 3);
AudioConnection          patchCord4(delay1, 0, i2s1, 0);
AudioConnection          patchCord5(delay1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=481.33332562446594,297.8333282470703
// GUItool: end automatically generated code


#include <Bounce.h>

Bounce button0 = Bounce(0, 5);

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
  AudioMemory(160);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.55);
  //sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  //sgtl5000_1.micGain(36);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(30);
  mixer1.gain(0, 0.7);
  mixer1.gain(3, 0.7);
  delay1.delay(0, 400);
  delay(1000);
}

void loop() {
  // A3 knob to control the feedback level

  int knob = analogRead(A3);
  float feedback = (float)knob / 1050.0;
  mixer1.gain(3, feedback);


  // pin 0 button to double the feedback (allowing unstable)

  button0.update();
  if (button0.read() == LOW) {
    Serial.println("button 0 pressed");
    mixer1.gain(3, feedback * 2.0);
  }

}
