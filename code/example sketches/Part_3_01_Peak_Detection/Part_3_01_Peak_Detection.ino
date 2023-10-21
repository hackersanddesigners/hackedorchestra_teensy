// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 3-1: Peak Detection


///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=170.83333333333331,289.99999999999994
AudioAnalyzePeak         peak2;          //xy=356.6666793823242,387.49997329711914
AudioAnalyzePeak         peak1;          //xy=367.5000228881836,342.49999618530273
AudioOutputI2S           i2s1;           //xy=437.2619171142578,291.47618865966797
AudioConnection          patchCord1(playSdWav1, 0, peak1, 0);
AudioConnection          patchCord2(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord3(playSdWav1, 1, peak2, 0);
AudioConnection          patchCord4(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=359.66665840148926,218.66668891906738
// GUItool: end automatically generated code



// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7   // Teensy 4 ignores this, uses pin 11
#define SDCARD_SCK_PIN   14  // Teensy 4 ignores this, uses pin 13

// Use these with the Teensy 3.5 & 3.6 & 4.1 SD card
//#define SDCARD_CS_PIN    BUILTIN_SDCARD
//#define SDCARD_MOSI_PIN  11  // not actually used
//#define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
//#define SDCARD_CS_PIN    4
//#define SDCARD_MOSI_PIN  11
//#define SDCARD_SCK_PIN   13

void setup() {
  Serial.begin(9600);
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);
}

// for best effect make your terminal/monitor a minimum of 62 chars wide and as high as you can.

elapsedMillis msecs;

void loop() {
  if (playSdWav1.isPlaying() == false) {
    Serial.println("Start playing");
    //playSdWav1.play("SDTEST1.WAV");
    playSdWav1.play("SDTEST2.WAV");
    //playSdWav1.play("SDTEST3.WAV");
    //playSdWav1.play("SDTEST4.WAV");
    delay(10); // wait for library to parse WAV info
  }
  
  if (msecs > 40) {
    if (peak1.available() && peak2.available()) {
      msecs = 0;
      float leftNumber = peak1.read();
      float rightNumber = peak2.read();
      int leftPeak = leftNumber * 30.0;
      int rightPeak = rightNumber * 30.0;
      int count;
      for (count=0; count < 30-leftPeak; count++) {
        Serial.print(" ");
      }
      while (count++ < 30) {
        Serial.print("<");
      }
      Serial.print("||");
      for (count=0; count < rightPeak; count++) {
        Serial.print(">");
      }
      while (count++ < 30) {
        Serial.print(" ");
      }
      Serial.print(leftNumber);
      Serial.print(", ");
      Serial.print(rightNumber);
      Serial.println();
    }
  }
}
