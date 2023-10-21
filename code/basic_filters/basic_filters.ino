// check out this vid for explanation of EQ filters
// pass filters = filtering out certain frequencies
// frequency (90-10000), stage (cascading up to 4: 1-3), and Q shape
// lowshelf & highshelf = boosting frequencies in a lower resp higher range.
// stage (0-3), freq (90-10K), gain (< 5), slope (smooth or sudden transition)
// https://www.youtube.com/watch?v=-_ghMYWIz0g
// should look into biquad a bit but gets technical quick

#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//connect LEDs on pin 3, 4 and 5 to indicate which filter is active
#define led1 3
#define led2 4
#define led3 5

Bounce button1 = Bounce(1, 15);  // debounce time 15 ms
int counter = 0;
int prevCounter = 0;

int lineInLevel;
float freqLow;
float freqHigh;
float freqBand;

const int myInput = AUDIO_INPUT_LINEIN;  // alternatively: AUDIO_INPUT_MIC;

String filter = "no filter";

// GUItool: begin automatically generated code
AudioInputI2S i2s1;         //xy=257,241
AudioFilterBiquad biquad1;  //xy=415,241
AudioMixer4 mixer1;         //xy=568,223
AudioOutputI2S i2s2;        //xy=735,252
AudioConnection patchCord1(i2s1, 0, biquad1, 0);
AudioConnection patchCord2(i2s1, 0, mixer1, 0);
AudioConnection patchCord3(biquad1, 0, mixer1, 1);
AudioConnection patchCord4(mixer1, 0, i2s2, 0);
AudioConnection patchCord5(mixer1, 0, i2s2, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=463,313
// GUItool: end automatically generated code

void setup() {

  Serial.begin(9600);
  delay(3000);
  Serial.println("Started.");
  Serial.println("no filter");

  pinMode(1, INPUT_PULLUP);  //switch
  pinMode(led1, OUTPUT);     // led
  pinMode(led2, OUTPUT);     // led
  pinMode(led3, OUTPUT);     // led
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  AudioMemory(200);

  sgtl5000_1.enable();  // Enable the audio shield
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.lineInLevel(0);  //gain of line in on start (0-15)
  sgtl5000_1.volume(0.5);

  mixer1.gain(0, 0.8);  // direct from line in, default
  mixer1.gain(1, 0);    //filter in, default off

  delay(2000);
}


void loop() {
  // update pushbutton
  button1.update();

  // read knobs
  float knobA1 = (float)analogRead(A1);  // sets lineInLevel
  float knobA2 = (float)analogRead(A2);  // set frequency of low/high/bandpass filter

  // control the gain on line in w knob
  lineInLevel = map(knobA1, 0, 1023, 0, 15);  // Line In gain 0-15 max
  sgtl5000_1.lineInLevel(lineInLevel);

  if (button1.fallingEdge()) {
    prevCounter = counter;
    counter++;
  }

  if (counter == 0) {
    // feedthrough line in, filter off, leds off
    analogWrite(led1, 0);
    analogWrite(led2, 0);
    analogWrite(led3, 0);
    mixer1.gain(0, 0.8);  //feedthrough gain
    mixer1.gain(1, 0);    //filter gain
    String filter = "no filter";
    if (counter != prevCounter) {
      Serial.println(filter);
    }
    prevCounter = 0;
    Serial.print("Gain line in A1 ");
    Serial.print(lineInLevel);
    Serial.print("  |  filters off ");
    Serial.println();
  }

  if (counter == 1) {
    analogWrite(led1, 512);                                //turn on led (PWM 50% in case no resistors)
    analogWrite(led2, 0);
    analogWrite(led3, 0);
    mixer1.gain(0, 0);                                    //feedthrough gain
    mixer1.gain(1, 0.8);                                  //filter gain
    freqLow = expf((float)knobA2 / 150.0) * 10.0 + 80.0;  // quick and dirty equation for exp scale frequency adjust
    biquad1.setLowpass(0, freqLow, 0.5);                  // setLowpass(stage 0-3, corner frequency 90-10k, Q shape < 0.70)
    String filter = "lowpass";
    if (counter != prevCounter) {
      Serial.println(filter);
    }
    prevCounter = 1;
    Serial.print("Gain line in A1 ");
    Serial.print(lineInLevel);
    Serial.print("  |  lowpass freq A2 ");
    Serial.println(freqLow);
  }

  if (counter == 2) {
    analogWrite(led1, 0);
    analogWrite(led2, 512);                                //turn on led (PWM 50% in case no resistors)
    analogWrite(led3, 0);
    mixer1.gain(0, 0);                                     //feedthrough gain
    mixer1.gain(1, 0.8);                                   //filter gain
    freqHigh = expf((float)knobA2 / 150.0) * 10.0 + 80.0;  // quick and dirty equation for exp scale frequency adjust
    biquad1.setHighpass(0, freqHigh, 0.5);                 // setHighpass(stage 0-3, corner frequency 90-10k, Q shape < 0.70)
    String filter = "highpass";
    if (counter != prevCounter) {
      Serial.println(filter);
    }
    prevCounter = 2;
    Serial.print("Gain line in A1 ");
    Serial.print(lineInLevel);
    Serial.print("  |  highpass freq A2 ");
    Serial.println(freqHigh);
  }

  if (counter == 3) {
    analogWrite(led1, 0);
    analogWrite(led2, 0);
    analogWrite(led3, 512);                                //turn on led (PWM 50% in case no resistors)
    mixer1.gain(0, 0);                                     //feedthrough gain
    mixer1.gain(1, 0.8);                                   //filter gain
    freqBand = expf((float)knobA2 / 150.0) * 10.0 + 80.0;  // quick and dirty equation for exp scale frequency adjust
    biquad1.setBandpass(0, freqBand, 0.5);                 // setBandpass(stage 0-3, corner frequency 90-10k, bandwidth 10-5000
    String filter = "bandpass";
    if (counter != prevCounter) {
      Serial.println(filter);
    }
    prevCounter = 3;
    Serial.print("Gain line in A1 ");
    Serial.print(lineInLevel);
    Serial.print("  |  bandpass freq A2 ");
    Serial.println(freqBand);
  }

  if (counter > 3) {
    counter = 0;
  }
}
