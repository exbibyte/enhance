#include "Clock.h"

#include <chrono>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

Clock::Clock()
{
  this->Fps = -1;
  this->bRunning = false;
  this->TimeSinceStart = 0;
  this->ClockScale = 1;
}

bool Clock::SetFps(double fps)
{
  if(fps <= 0)
    return false;

  this->Fps = fps;
  //milliseconds per frame
  this->AutoDuration = (double)1.0/fps*1000;
  this->AutoDurationScaled = this->AutoDuration / this->ClockScale;

  return true;
}

bool Clock::Tick()
{
  if(this->Fps <= 0)
    return false;
  
  if(this->bRunning == false)
    return false;
  
  //get duration between previous tick and now in milliseconds
  this->Time = chrono::high_resolution_clock::now();
  auto diff = this->Time - this->TimePrev;
  auto duration = chrono::duration<double,milli>(diff).count();

  //test if this tick is complete with clock scaling adjustment
  if(duration < this->AutoDurationScaled)
    return false;

  //calculate fps
  this->FpsActual = 1.0/(duration/1000)/this->ClockScale;
  cout<<"Time Scale: "<<this->ClockScale<< " Time Scaled FPS: "<<this->FpsActual<<endl;

  //save runnning time 
  this->TimeSinceStart += duration*this->ClockScale;

  //tick complete
  this->TimePrev = this->Time; 
  
  //fire message of tick completion
  stringstream ss;
  ss << this->TimeSinceStart;

  this->TickAction(ss.str()); 

  return true;
}

void Clock::Run()
{
  this->bRunning = true;

  this->TimePrev = chrono::high_resolution_clock::now();
}

void Clock::Pause()
{
  this->bRunning = false;
}

double Clock::GetFps() const
{
  return this->Fps;
}

bool Clock::IsRunning() const
{
  return this->bRunning;
}

double Clock::GetTime() const
{
  return this->TimeSinceStart;
}

void Clock::SetClockScale(double val)
{
  if(val<= 0)
    return;

  this->ClockScale = val;
  this->AutoDurationScaled = this->AutoDuration / this->ClockScale;
}
