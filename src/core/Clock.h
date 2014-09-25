#ifndef CLOCK_H
#define CLOCK_H

#include <string>
#include <chrono>

using namespace std;

/// provides timing and triggering ability
class Clock
{
 private:
  double                                           Fps;
  double                                           FpsActual;
  double                                           AutoDuration;   /// stores how many milliseconds between consecutive frames
  double                                           AutoDurationScaled;
  double                                           TimeSinceStart;   ///current time not counting paused time in milliseconds
  chrono::high_resolution_clock::time_point        TimePrev;
  chrono::high_resolution_clock::time_point        Time;
  bool                                             bRunning;   ///flag indicating if clock is running
  double                                           ClockScale;
 public:
                                                   Clock();
  bool                                             SetFps(double fps);   /// sets frames per second
  bool                                             Tick();   /// runs clock if fps is valid and clock is not paused
  void                                             Run();   /// runs the clock
  void                                             Pause();   /// pauses the clock
  double                                           GetFps() const;
  void                                             SetClockScale(double);
  bool                                             IsRunning() const;
  double                                           GetTime() const;   /// get current time in milliseconds
  virtual void                                     TickAction(string a){};   /// implementation method called after each clock Tick success
};

#endif
