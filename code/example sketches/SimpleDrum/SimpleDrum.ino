#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <synth_simple_drum.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum2;          //xy=399,244
AudioSynthSimpleDrum     drum3;          //xy=424,310
AudioSynthSimpleDrum     drum1;          //xy=431,197
AudioSynthSimpleDrum     drum4;          //xy=464,374
AudioMixer4              mixer1;         //xy=737,265
AudioOutputI2S           i2s1;           //xy=979,214
AudioConnection          patchCord1(drum2, 0, mixer1, 1);
AudioConnection          patchCord2(drum3, 0, mixer1, 2);
AudioConnection          patchCord3(drum1, 0, mixer1, 0);
AudioConnection          patchCord4(drum4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=930,518
// GUItool: end automatically generated code

static uint32_t next;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  // audio library init
  AudioMemory(15);

  next = millis() + 1000;

  AudioNoInterrupts();

  drum1.frequency(60);  // base frequency
  drum1.length(1500);   // length of envelope in ms
  drum1.secondMix(1.0); // adds second sine wave harmonized in fifth above base
  drum1.pitchMod(0.55); // default is 0.5, aove sweeps downwards, below sweeps up
  
  drum2.frequency(60);
  drum2.length(1000);
  drum2.secondMix(1.0);
  drum2.pitchMod(1.0);
  
  drum3.frequency(550);
  drum3.length(1000);
  drum3.secondMix(1.0);
  drum3.pitchMod(0.5);

  drum4.frequency(1200);
  drum4.length(1000);
  drum4.secondMix(1.0);
  drum4.pitchMod(0.0);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.35);

  AudioInterrupts();

}

void loop() {
  // put your main code here, to run repeatedly:

  static uint32_t num = 0;

  if(millis() == next)
  {
    next = millis() + 1000;

    switch(num % 4)
    {
      case 0:
        drum1.noteOn();
        break;
      case 1:
        drum2.noteOn();
        break;
      case 2:
        drum3.noteOn();
        break;
      case 3:
        drum4.noteOn();
        break;
    }
    num++;

    Serial.print("Diagnostics: ");
    Serial.print(AudioProcessorUsageMax());
    Serial.print(" ");
    Serial.println(AudioMemoryUsageMax());
    AudioProcessorUsageMaxReset();
  }
}
