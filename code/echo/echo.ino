#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>


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

Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);
Bounce button2 = Bounce(2, 15);  // 15 = 15 ms debounce time

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7   // Teensy 4 ignores this, uses pin 11
#define SDCARD_SCK_PIN   14  // Teensy 4 ignores this, uses pin 13

void setup() {
  Serial.begin(9600);
  AudioMemory(160);     //working with delays requires more memory allocated!
  sgtl5000_1.enable();            // enable control panel
  sgtl5000_1.volume(0.5);          // set volume (max 0.8 is good)
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(30);     //sensitivity of the line in (0-15?)

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  mixer1.gain(0, 0.7);
  mixer1.gain(3, 0.7);
  delay1.delay(0, 200);   //length of delay in ms
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read pushbuttons
  button0.update();
  if (button0.fallingEdge()) {
    // do something
  }
  button1.update();   
  if (button1.fallingEdge()) {
    //do something
  }
  button2.update();
  if (button2.fallingEdge()) {
    //do something
  }
  // control the gain on line in w knob
  int knob1 = analogRead(A1);
  int gainLineIn = map(knob1, 0,1023,0,30);
  sgtl5000_1.lineInLevel(gainLineIn); // is level of line in! 
  Serial.print("gain Line In = ");
  Serial.println(gainLineIn);

  //control the decay of the feedback signal float 0-1 with 1 = no decay
  int knob2 = analogRead(A2);
  float feedback = (float)knob2 / 1050.0;
  mixer1.gain(3, feedback);
  Serial.print("feedback decay = ");
  Serial.println(feedback);

  //control the delay time (ms)
  int knob3 = analogRead(A3);
  int delaytime = map(knob3, 0, 1023, 0, 400);
  delay1.delay(0, delaytime);   //length of delay in ms
  Serial.print("delaytime = ");
  Serial.println(delaytime);

}
