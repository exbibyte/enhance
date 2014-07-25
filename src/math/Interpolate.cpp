#include "ParametricCurve.h"
#include "Interpolate.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;

Interpolate::Interpolate()
{
  this->CurveIndex = 0;
  this->bKeepIncrementing = false;
  for(int i = 0; i < 3; i++)
    this->Position[i] = 0;

  this->bActivated = false;
}

Interpolate::~Interpolate()
{
  this->vpParametricCurve.clear();
}

void Interpolate::AddCurve(int steps, float ctrlpoint1[], float ctrlpoint2[], float ctrlpoint3[], float ctrlpoint4[])
{
  ParametricCurve * pcurve = new ParametricCurve();
 
  pcurve->SetParameter(steps, ctrlpoint1, ctrlpoint2, ctrlpoint3, ctrlpoint4);

  this->vpParametricCurve.push_back(pcurve);

  this->bActivated = true;
}
  
void Interpolate::Increment()
{
  if(this->vpParametricCurve.empty())
    return;

  bActivated = true;

  this->itCurve = this->vpParametricCurve.begin();
  this->itCurve += this->CurveIndex;

  if(this->itCurve != this->vpParametricCurve.end())
  {  
    // initialize curve 
    if((*itCurve)->Started() == false)
    {
      (*itCurve)->Start(); 
    }
    
    if((*itCurve)->Done() == false)
    {
      // update current curve
      float * pPosition = &this->Position[0];   
      (*itCurve)->GetCurrent(pPosition);
      (*itCurve)->Increment();
      if((*itCurve)->Done() == true)
      {	
	this->CurveIndex++;
	auto a = this->vpParametricCurve.begin() + this->CurveIndex;
	if(a != this->vpParametricCurve.end())
	{
	  (*a)->Start();
	}
      }
    }
    else
    {
      //go to next curve if current curve is finished
      this->CurveIndex++;
      auto a = this->vpParametricCurve.begin() + this->CurveIndex;
      if(a != this->vpParametricCurve.end())
      {
	(*a)->Start();
	// update current curve
	float * pPosition = &this->Position[0];   
	(*a)->GetCurrent(pPosition);
	(*a)->Increment();
      }
    }
  }
}

void Interpolate::PrintPosition()
{
  for(int i = 0; i < 3; i++)
  {
    cout<<this->Position[i]<<" ";
  }
  cout<<endl;
}

bool Interpolate::GetKeepIncrementing()
{
  return this->bKeepIncrementing;
}

bool Interpolate::SetKeepIncrementing(bool val)
{
  this->bKeepIncrementing = val;
  bActivated = true;
}

void Interpolate::GetPosition(float out[])
{
  for(int i = 0; i < 3; i++)
    out[i] = this->Position[i];
}

bool Interpolate::GetActivated()
{
  return this->bActivated;
}

bool Interpolate::SetActivated(bool val)
{
  this->bActivated = val;
}
