#include <Bounce.h>
#include <Audio.h>

#define B1 0
#define B2 1

AudioSynthWaveform waveform1;  //xy=235.0833282470703,232.08334350585938
AudioInputI2S i2s1;            //xy=238.0833282470703,154.08334350585938
AudioMixer4 mixer1;            //xy=409.0833282470703,230.0833282470703
AudioOutputI2S i2s2;           //xy=610.0833129882812,211.08334350585938
AudioConnection patchCord1(waveform1, 0, mixer1, 1);
AudioConnection patchCord2(i2s1, 0, mixer1, 0);
AudioConnection patchCord3(mixer1, 0, i2s2, 0);
AudioConnection patchCord4(mixer1, 0, i2s2, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=565.0833129882812,454.0833435058594


Bounce btn1 = Bounce(B1, 5);  // cycles the bitcrusher through all bitdepths
Bounce btn2 = Bounce(B2, 5);  //cycles the bitcrusher through some key samplerates

bool btn1_state, btn2_state, btn3_state;
int knob_A1, knob_A2, knob_A3, knob_A4;

unsigned long timer = 0;
bool playBeep = false;

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("Started.");

  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);

  AudioMemory(200);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(30);  //sensitivity of the line in (0-15?)

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);

  waveform1.begin(WAVEFORM_SINE);
  waveform1.frequency(440);
  waveform1.amplitude(0.9);

  delay(1000);
}

void loop() {
  knob_A1 = analogRead(A1);
  knob_A2 = analogRead(A2);
  knob_A3 = analogRead(A3);
  knob_A4 = analogRead(A4);

  Serial.print("A1=");
  Serial.print(knob_A1);
  Serial.print(", A2=");
  Serial.print(knob_A2);
  Serial.print(", A3=");
  Serial.print(knob_A3);
  Serial.print(", A4=");
  Serial.print(knob_A4);
  Serial.print(", B1=");
  Serial.print(digitalRead(B1));
  Serial.print(", B2=");
  Serial.print(digitalRead(B2));

  if (millis() > timer + 500) {
    timer = millis();
    playBeep = !playBeep;
    if (playBeep) {
      waveform1.amplitude(0.9);
    } else {
      waveform1.amplitude(0);
    }
  }
}
