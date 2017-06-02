#ifndef Wave_h
#define Wave_h

class Wave
{
  public:
    Wave();
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
