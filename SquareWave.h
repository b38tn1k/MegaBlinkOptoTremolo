#ifndef SquareWave_h
#define SquareWave_h

class SquareWave
{
  public:
    SquareWave();
    int getSample(unsigned long _nextInflection, unsigned long _now);
    void updateWave(int _depth, double _shape);
    void tick();
  private:
    int depth;
    bool crest = true;
    double lastSample = 0.0;
    double shape = 1;
};
#endif
