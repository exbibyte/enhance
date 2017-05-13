#ifndef E2_INTERPOLATE_HPP
#define E2_INTERPOLATE_HPP

#include "parametric_curve.hpp"

#include <vector>

namespace e2 { namespace math {

class interpolate
{
 private:

  std::vector<parametric_curve*>::iterator                itCurve;
  int CurveIndex;
  std::vector<parametric_curve * >                       vp_parametric_curve;   ///stores bezier curves to travel through
  float                                                  Position[3];   /// stores values of current step
  bool                                                   bKeepIncrementing;
  bool                                                   bActivated;
 public:
                                                         interpolate();
							 ~interpolate();
  void                                                   AddCurve(int steps, float ctrlpoint1[], float ctrlpoint2[], float ctrlpoint3[], float ctrlpoint4[]);   /// adds a bezier curve to the existing queue of curves
  void                                                   Increment();   /// go to the next step of the curve
  void                                                   PrintPosition();
  bool                                                   GetKeepIncrementing();   /// gets current state of incrementing or not
  bool                                                   SetKeepIncrementing(bool val);   /// sets to incrementing or not
  void                                                   GetPosition(float out[]);   ///gets the current interpolation
  bool                                                   GetActivated();   /// get and set flags for using interpolation
  bool                                                   SetActivated(bool val);

};

} }

#endif
