/** 
  Granular Effect / Pitch shift for Hacked Orchestra

  Pot 1 - A1 -> Input gain
  Pot 2 - A2 -> Granular pitchShift grainLength
  Pot 3 - A3 -> Granular speed ratio
  Pot 4 - A4 -> Reverb time
  Btn 1 - D0 -> Toggle - effects on/off
  Btn 2 - D1 -> Toggle - reverbe on/off
  Btn 3 - D2 -> Momentary - Flange on

**/

#include <Audio.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s2;             //xy=898.75,251.25
AudioMixer4 mixer1;             //xy=1072.75,330.75
AudioEffectGranular granular1;  //xy=1244.7500190734863,318.75000381469727
AudioOutputI2S i2s1;            //xy=1404.75,348.75
AudioConnection patchCord1(i2s2, 0, mixer1, 0);
AudioConnection patchCord2(mixer1, granular1);
AudioConnection patchCord3(granular1, 0, i2s1, 0);
AudioConnection patchCord4(granular1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=976.75,411.75
// GUItool: end automatically generated code


Bounce button1 = Bounce(0, 15);
Bounce button2 = Bounce(1, 15);
Bounce button3 = Bounce(2, 15);

bool button1_pressed = false;

#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

void setup() {
  Serial.begin(9600);
  AudioMemory(200);

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(7);  //sensitivity of the line in (0-15?)

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);

  // the Granular effect requires memory to operate
  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);
}

void loop() {

  // read pushbuttons
  button1.update();
  button2.update();
  button3.update();

  // read knobs, scale to 0-1.0 numbers
  float knobA1 = (float)analogRead(A1) / 1023.0;  // sets lineInLevel
  float knobA2 = (float)analogRead(A2) / 1023.0;  // sets pitchShift grainLength
  float knobA3 = (float)analogRead(A3) / 1023.0;  // sets speed ratio

  int lineInLevel = map(knobA1, 0, 1, 0, 15);
  sgtl5000_1.lineInLevel(lineInLevel);

  float msec = 25.0 + (knobA2 * 75.0);

  // Continuously adjust the speed, based on the A3 pot
  float ratio;
  ratio = powf(2.0, knobA3 * 2.0 - 1.0);  // 0.5 to 2.0 -- beastie boys
  //  ratio = powf(2.0, knobA2 * 6.0 - 3.0); // 0.125 to 8.0 -- daftpunky
  granular1.setSpeed(ratio);

  // Button 1 starts Pitch Shift effect
  if (button1.fallingEdge()) {
    button1_pressed = true;
  }
  if (button1.risingEdge()) {
    button1_pressed = false;
  }

  if (button2.fallingEdge()) {
    Serial.print("2");
  }

  if (button3.fallingEdge()) {
    Serial.print("3");
  }

  if (button1_pressed) {
    Serial.print("Pitch phifting using ");
    Serial.print(msec);
    Serial.print(" grains and ratio ");
    Serial.println(ratio);
    granular1.beginPitchShift(msec);
  } else {
    granular1.stop();
  }
}
