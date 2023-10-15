/** 
  Granular Effect / Pitch shift for Hacked Orchestra

  Pot 1 - A1 -> Input gain
  Pot 2 - A2 -> Granular pitchShift grainLength
  Pot 3 - A3 -> Granular speed ratio
  Pot 4 - A4 -> Reverb time
  Btn 1 - D0 -> Toggle - effects on/off
  Btn 2 - D1 -> Momentary switch - reverb on/off

**/

#include <Audio.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s2;             //xy=160.0999755859375,234
AudioMixer4 mixer1;             //xy=334.0999755859375,313
AudioEffectGranular granular1;  //xy=506.0999755859375,301
AudioEffectReverb reverb1;      //xy=652.0999755859375,424.1000061035156
AudioMixer4 mixer2;             //xy=753.0999755859375,242.10000610351562
AudioOutputI2S i2s1;            //xy=930.0999755859375,236
AudioConnection patchCord1(i2s2, 0, mixer1, 0);
AudioConnection patchCord2(mixer1, granular1);
AudioConnection patchCord3(granular1, reverb1);
AudioConnection patchCord4(granular1, 0, mixer2, 0);
AudioConnection patchCord5(reverb1, 0, mixer2, 1);
AudioConnection patchCord6(mixer2, 0, i2s1, 0);
AudioConnection patchCord7(mixer2, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=238.0999755859375,394
// GUItool: end automatically generated code


int button1 = 0;
Bounce button2 = Bounce(1, 15);
bool isReverb = false;
float reverbTime;

bool button2_pressed = false;

#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

#define LED_PIN 13        // built-in led
unsigned long timer = 0;  // timer for blink led
bool ledState = false;

void setup() {
  Serial.begin(9600);
  AudioMemory(300);

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(15);  //sensitivity of the line in (0-15?)

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);

  mixer1.gain(0, 1);
  mixer1.gain(1, 0);

  // the Granular effect requires memory to operate
  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);

  reverb1.reverbTime(10);

  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250);
  }

  delay(1000);
  timer = millis();
}

void loop() {

  // read pushbuttons
  button2.update();

  // read knobs, scale to 0-1.0 numbers
  float knobA1 = (float)analogRead(A1) / 1023.0;  // sets lineInLevel
  float knobA2 = (float)analogRead(A2) / 1023.0;  // sets grainLength
  float knobA3 = (float)analogRead(A3) / 1023.0;  // sets speedRatio
  float knobA4 = (float)analogRead(A4) / 1023.0;  // set reverb room size

  // control the gain on line in w knob
  int lineInLevel = map(knobA1, 0, 1, 0, 15);
  sgtl5000_1.lineInLevel(lineInLevel);

  // calculate speed from knob 2
  float msec = 25.0 + (knobA2 * 75.0);
  // float ratio = powf(2.0, knobA3 * 2.0 - 1.0);  // 0.5 to 2.0 -- beastie boys
  float ratio = powf(2.0, knobA3 * 6.0 - 3.0);  // 0.125 to 8.0 -- daftpunky
  granular1.setSpeed(ratio);


  if (digitalRead(button1) == 0) {
    // if low, start pitch shift
    granular1.beginPitchShift(msec);
    digitalWrite(LED_PIN, HIGH);
    Serial.print("lineInLevel =  ");
    Serial.print(lineInLevel);
    Serial.print(" |  grainLength  = ");
    Serial.print(msec);
    Serial.print(" |  speed ratio ");
    Serial.print(ratio);
    Serial.print(" |  reverbTime =  ");
    if (isReverb == 1) {
      Serial.println(reverbTime);
    } else {
      Serial.println(isReverb);
    }
  }

  if (digitalRead(button1) == 1) {
    // if high, bypass filter (feedthrough line in)
    granular1.stop();
    digitalWrite(LED_PIN, LOW);
    Serial.print("lineInLevel =  ");
    Serial.print(lineInLevel);
    Serial.print(" |  Pitchshifting OFF ");
    Serial.print(" |  reverbTime =  ");
    if (isReverb == 1) {
      Serial.println(reverbTime);
    } else {
      Serial.println(isReverb);
    }
  }

  if (button2.fallingEdge()) { button2_pressed = true; }
  if (button2.risingEdge()) { button2_pressed = false; }

  reverbTime = knobA4 * 20;
  if (button2_pressed) {
    isReverb = true;
    // Serial.print("with reverbTime ");
    // Serial.println(reverbTime);
    reverb1.reverbTime(reverbTime);
    mixer2.gain(0, 0);
    mixer2.gain(1, 1);
  } else {
    isReverb = false;
    mixer2.gain(0, 1);
    mixer2.gain(1, 0);
    reverb1.reverbTime(0);
  }

  // if (millis() > timer + 500) {
  //   digitalWrite(LED_PIN, ledState);
  //   ledState = !ledState;
  //   timer = millis();
  // }
}
