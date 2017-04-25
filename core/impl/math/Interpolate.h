#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include "ParametricCurve.h"

#include <vector>

using namespace std;

class Interpolate
{
 private:

  vector<ParametricCurve*>::iterator                     itCurve;
  int CurveIndex;
  vector<ParametricCurve * >                             vpParametricCurve;   ///stores bezier curves to travel through
  float                                                  Position[3];   /// stores values of current step
  bool                                                   bKeepIncrementing;
  bool                                                   bActivated;
 public:
                                                         Interpolate();
							 ~Interpolate();
  void                                                   AddCurve(int steps, float ctrlpoint1[], float ctrlpoint2[], float ctrlpoint3[], float ctrlpoint4[]);   /// adds a bezier curve to the existing queue of curves
  void                                                   Increment();   /// go to the next step of the curve
  void                                                   PrintPosition();
  bool                                                   GetKeepIncrementing();   /// gets current state of incrementing or not
  bool                                                   SetKeepIncrementing(bool val);   /// sets to incrementing or not
  void                                                   GetPosition(float out[]);   ///gets the current interpolation
  bool                                                   GetActivated();   /// get and set flags for using interpolation
  bool                                                   SetActivated(bool val);

};

#endif
