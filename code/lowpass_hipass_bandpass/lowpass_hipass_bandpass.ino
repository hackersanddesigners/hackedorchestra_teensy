#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


Bounce button0 = Bounce(0, 15); // debounce time 15 ms
int knob_A1;
int counter = 0;
int prevCounter = 0;
float freq;
const int myInput = AUDIO_INPUT_LINEIN;   // alternatively: AUDIO_INPUT_MIC;
String filter = "no filter";

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=257,241
AudioFilterBiquad        biquad1;        //xy=415,241
AudioMixer4              mixer1;         //xy=568,223
AudioOutputI2S           i2s2;           //xy=735,252
AudioConnection          patchCord1(i2s1, 0, biquad1, 0);
AudioConnection          patchCord2(i2s1, 0, mixer1, 0);
AudioConnection          patchCord3(biquad1, 0, mixer1, 1);
AudioConnection          patchCord4(mixer1, 0, i2s2, 0);
AudioConnection          patchCord5(mixer1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=463,313
// GUItool: end automatically generated code

void setup() {

  Serial.begin(9600);
  delay(3000);
  Serial.println("Started.");
  Serial.println("no filter");

  pinMode(0, INPUT_PULLUP);
  AudioMemory(200);

  sgtl5000_1.enable();  // Enable the audio shield
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.lineInLevel(30);  //sensitivity of the line in (0-15?)
  sgtl5000_1.volume(0.5);

  mixer1.gain(0, 0.8);   // direct from line in, default
  mixer1.gain(1, 0);    //filter in, default off

  delay(1000);

}


void loop() {

  button0.update();

  if (button0.fallingEdge()) {
    prevCounter = counter;
    counter ++;
  }


  if (counter == 0) {
    // feedthrough line in, filter off
    mixer1.gain(0, 0.8);   //feedthrough gain
    mixer1.gain(1, 0);    //filter gain
    String filter = "no filter";
    if (counter != prevCounter) {
      Serial.println(filter);
    }
    prevCounter = 0;
  }

  if (counter == 1) {
    mixer1.gain(0, 0);   //feedthrough gain
    mixer1.gain(1, 0.8);    //filter gain
    int knob = analogRead(A1);  // read the knob and adjust the filter frequency
    freq =  expf((float)knob / 150.0) * 10.0 + 80.0;  // quick and dirty equation for exp scale frequency adjust
    biquad1.setLowpass(0, freq, 0.25); // setLowpass(stage 0-3, corner frequency 90-10k, Q shape < 0.70)
    String filter = "lowpass";
    if (counter != prevCounter) {
      Serial.println(filter);
    }
    prevCounter = 1;
  }

  if (counter == 2) {
    mixer1.gain(0, 0);   //feedthrough gain
    mixer1.gain(1, 0.8);    //filter gain
    int knob = analogRead(A1);  // read the knob and adjust the filter frequency
    freq =  expf((float)knob / 150.0) * 10.0 + 80.0; // quick and dirty equation for exp scale frequency adjust
    biquad1.setHighpass(0, freq, 0.25); // setHighpass(stage 0-3, corner frequency 90-10k, Q shape < 0.70)
    String filter = "highpass";
    if (counter != prevCounter) {
      Serial.println(filter);
    }
    prevCounter = 2;
  }

  if (counter == 3) {
    mixer1.gain(0, 0);   //feedthrough gain
    mixer1.gain(1, 0.8);    //filter gain
    int knob = analogRead(A1);  // read the knob and adjust the filter frequency
    freq =  expf((float)knob / 150.0) * 10.0 + 80.0; // quick and dirty equation for exp scale frequency adjust
    biquad1.setBandpass(0, freq, 0.54); // setBandpass(stage 0-3, corner frequency 90-10k, bandwidth 10-5000
    String filter = "bandpass";
    if (counter != prevCounter) {
      Serial.println(filter);
    }
    prevCounter = 3;
  }

  if (counter > 3) {
    counter = 0;
  }
}
