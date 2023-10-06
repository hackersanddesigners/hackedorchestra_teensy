/** 
  Echo for Hacked Orchestra

  Pot 1 - A1 -> gain line in
  Pot 2 - A2 -> delay time
  Pot 3 - A3 -> decay
  Pot 4 - A4 -> Reverb time
  Btn 1 - D0 -> Toggle - effects on/off
  Btn 2 - D1 -> Toggle - reverbe on/off
  Btn 3 - D2 -> Momentary - Flange on

**/

#include <Audio.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s2;         //xy=300.066650390625,226
AudioMixer4 mixer1;         //xy=471.066650390625,256
AudioEffectDelay delay1;    //xy=471.066650390625,362
AudioEffectReverb reverb1;  //xy=630.066650390625,344.066650390625
AudioMixer4 mixer2;         //xy=779.066650390625,332.066650390625
AudioMixer4 mixer3;         //xy=948.066650390625,240.06666564941406
AudioOutputI2S i2s1;        //xy=1077.066650390625,238.00001525878906
AudioConnection patchCord1(i2s2, 0, mixer1, 0);
AudioConnection patchCord2(i2s2, 0, mixer3, 0);
AudioConnection patchCord3(mixer1, delay1);
AudioConnection patchCord4(delay1, 0, mixer1, 3);
AudioConnection patchCord5(delay1, 0, reverb1, 0);
AudioConnection patchCord6(delay1, 0, mixer2, 0);
AudioConnection patchCord7(reverb1, 0, mixer2, 1);
AudioConnection patchCord8(mixer2, 0, mixer3, 1);
AudioConnection patchCord9(mixer3, 0, i2s1, 0);
AudioConnection patchCord10(mixer3, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=1073.066650390625,287
// GUItool: end automatically generated code

Bounce button1 = Bounce(0, 15);
Bounce button2 = Bounce(1, 15);
Bounce button3 = Bounce(2, 15);  // 15 = 15 ms debounce time

bool button2_pressed = false;
unsigned long updateCounterMs;
int lastDelayTime = 0;

void setup() {
  Serial.begin(9600);
  AudioMemory(300);        //working with delays requires more memory allocated!
  sgtl5000_1.enable();     // enable control panel
  sgtl5000_1.volume(0.5);  // set volume (max 0.8 is good)
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(30);  //sensitivity of the line in (0-15?)

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  // mixer 1 for echo
  mixer1.gain(0, 0.7);
  mixer1.gain(3, 0.7);

  // mixer 2 for reverb
  mixer2.gain(0, 1);
  mixer2.gain(1, 0);

  // mixer 3 for bypass
  mixer3.gain(0, 1);
  mixer3.gain(1, 0);

  reverb1.reverbTime(10);

  delay1.delay(0, 200);  //length of delay in ms
  delay(1000);

  updateCounterMs = millis();
}

void loop() {
  // read pushbuttons
  button1.update();
  button2.update();
  button3.update();

  // read knobs, scale to 0-1.0 numbers
  float knobA1 = (float)analogRead(A1) / 1023.0;  // sets lineInLevel
  float knobA2 = (float)analogRead(A2) / 1023.0;  // sets delay time
  float knobA3 = (float)analogRead(A3) / 1023.0;  // sets decay
  float knobA4 = (float)analogRead(A4) / 1023.0;  // set reverb room size

  // control the gain on line in w knob
  // int gainLineIn = knobA1 * 30;
  // sgtl5000_1.lineInLevel(gainLineIn);  // is level of line in!

  //control the decay of the feedback signal float 0-1 with 1 = no decay
  float feedback = knobA2;
  mixer1.gain(3, feedback);
  Serial.print("feedback decay = ");
  Serial.println(feedback);

  //control the delay time (ms)
  int delaytime = knobA3 * 400;  //map(knob3, 0, 1023, 0, 400);
  // calling update in the loop create noise, so we only update
  // when changed, and account for some noise. 
  if (delaytime < lastDelayTime - 5 || delaytime > lastDelayTime + 5) {
    delay1.delay(0, delaytime);  //length of delay in ms
    lastDelayTime = delaytime;
  }

  Serial.print("delaytime = ");
  Serial.println(delaytime);


  if (button1.fallingEdge()) {
    // bypass all effects
    mixer3.gain(0, 0);
    mixer3.gain(1, 1);
  }
  if (button1.risingEdge()) {
    // mix in the effects
    mixer3.gain(0, 1);
    mixer3.gain(1, 0);
  }

  if (button2.fallingEdge()) { button2_pressed = true; }
  if (button2.risingEdge()) { button2_pressed = false; }

  float reverbTime = knobA1 * 10;
  if (button2_pressed) {
    Serial.print("with reverbTime ");
    Serial.println(reverbTime);
    reverb1.reverbTime(reverbTime);
    mixer2.gain(0, 0);
    mixer2.gain(1, 1);
  } else {
    mixer2.gain(0, 1);
    mixer2.gain(1, 0);
    reverb1.reverbTime(0);
  }

}