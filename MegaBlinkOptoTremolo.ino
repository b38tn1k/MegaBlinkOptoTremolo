/* MEGA-BLINK!
 * A Waveform Generator for a LED based Opto-Trem Guitar Effect
 * 
 * Tempo knob is locked to whole number BPMs
 * 
 * Shape allows modification of the selected waveform
 * 
 * Depth can be controlled by a potentionmeter connected to the LED
 * 
 * Wave generator moves between a 'sharp' Wave and
 * one with rounded edges. This is achieved using Proportional
 * Gain control of the sample. The rise and fall of shape has its 
 * own time constant. A high frequency tremolo with a high 'Shape'
 * value may end up just sounding like a quiet guitar.
 */
 #define MAX_OUTPUT   255
 #define MAX_INPUT    1023
 #define MICROS       1000000
 #define MILLIS       1000
 #define MIN_BPM      60
 #define MIN_SHAPE    0.1
 #define TEMPO_PIN    A7
 #define SHAPE_PIN    A0
 #define DUTY_PIN     A3
 #include "Wave.h"
 // Timer
 unsigned long avgInterval;
 unsigned long interval;
 unsigned long nextInflection = 0;
 int bpm = 120;
 double shape = 0.0;
 double duty = 1.0;
 bool flip = true;
 int sample = 0;
 unsigned long _now;
 // Serial Timer
 unsigned long serialInterval = 1000000/50;
 unsigned long serialUpdate = 0;
 // Wave Functions
 Wave sqr;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); 
  readPots();
  interval = duty * avgInterval;
  Serial.begin(9600);
}

void loop()
{
  // write the current wave sample
  sample = sqr.getSample(nextInflection, _now);
  analogWrite(LED_BUILTIN, sample);
  // update the timer
  _now = micros();
  if (nextInflection - _now > interval) {
    // Update user changable values. Changing on inflection sort of debounces
    readPots();
    if (flip) {
      interval = avgInterval * duty;
    } else {
      interval = avgInterval * (1 - duty);
    }
    flip = !flip;
    nextInflection = _now + interval;
    sqr.tick();
  }
  sqr.updateWave(MAX_OUTPUT, shape);
//  write2Serial(sample, _now);    // THIS WILL SLOW IT DOWN A BIT BUT IS USEFUL FOR PLOTTING
}

void write2Serial(double sample, unsigned long _now) {
  if (serialUpdate - _now > serialInterval) {
    serialUpdate = _now + serialInterval;
    String forSerial = String(_now);
    forSerial = forSerial + " " + String(sample);
    Serial.println(forSerial);
  }
}

void readPots() {
  avgInterval = bpm2avgInterval(analogRead(TEMPO_PIN) + MIN_BPM);
  shape = double(analogRead(SHAPE_PIN)) / 1023 + MIN_SHAPE;
  duty = double(analogRead(DUTY_PIN)) / 1023;
}

unsigned long bpm2avgInterval(int bpm)
{
  return (60 * MICROS) / bpm;
}

