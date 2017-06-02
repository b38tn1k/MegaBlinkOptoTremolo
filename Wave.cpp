#include "Wave.h"
#include <math.h>

Wave::Wave()
{
}


int Wave::getSample(unsigned long _nextInflection, unsigned long _now)
{
  double sample = 0.0;
  if (crest) {
    if (lastSample < depth) {
      sample = lastSample + ((depth - lastSample) * shape);
    } else {
      sample = depth;
    }
  } else {
    if (lastSample > 0) {
      sample = lastSample - (shape * lastSample);
    } else {
      sample = 0;
    }
  }
  lastSample = sample;
  return int(floor(sample));
}

void Wave::updateWave(int _depth, double _shape)
{
  depth = _depth;
  shape = _shape / 255;
}

void Wave::tick()
{
  crest = !crest;
}

