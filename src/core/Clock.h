#ifndef CLOCK_H
#define CLOCK_H

#include <string>
#include <chrono>

using namespace std;

/// provides timing and triggering ability
class Clock
{
 private:
  float                                           Fps;
  float                                           FpsActual;
  float                                           AutoDuration;   /// stores how many milliseconds between consecutive frames
  float                                           AutoDurationScaled;
  float                                           TimeSinceStart;   ///current time not counting paused time in milliseconds
  chrono::high_resolution_clock::time_point       TimePrev;
  chrono::high_resolution_clock::time_point       Time;
  bool                                            bRunning;   ///flag indicating if clock is running
  float                                           ClockScale;
 public:
                                                  Clock();
  bool                                            SetFps(float fps);   /// sets frames per second
  bool                                            Tick();   /// runs clock if fps is valid and clock is not paused
  void                                            Run();   /// runs the clock
  void                                            Pause();   /// pauses the clock
  float                                           GetFps();
  void                                            SetClockScale(float);
  bool                                            IsRunning();
  float                                           GetTime();   /// get current time in milliseconds
  virtual void                                    TickAction(string a){};   /// implementation method called after each clock Tick success
};

#endif
