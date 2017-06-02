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
 // Wave Functions
 Wave sqr;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); 
  analogWrite(LED_BUILTIN, 0);
  readPots();
  interval = duty * avgInterval;
  Serial.begin(9600);
}

void loop()
{
  sqr.updateWave(MAX_OUTPUT, shape);
  // write the current wave sample
  analogWrite(LED_BUILTIN, sqr.getSample(nextInflection, micros()));
  // update the timer
  if (nextInflection - micros() > interval) {
    // Update user changable values. Changing on inflection sort of debounces
    readPots();
    if (flip) {
      interval = avgInterval * duty;
      Serial.println(duty);
    } else {
      interval = avgInterval * (1 - duty);
    }
    flip = !flip;
    nextInflection = micros() + interval;
    sqr.tick();
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

