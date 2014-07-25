#ifndef PARAMETRICCURVE_H
#define PARAMETRICCURVE_H

///provides ability to trace out position of a cubic bezier curve using forward differencing. Taken from pp.365-367 of Chapter 8 of Advanced 3D Game Programming with DirectX 10.0 by Peter Walsh
class ParametricCurve
{
 private:
  /// current step
  int mCurrentStep;

  /// total steps for this curve
  int mTotalStep;

  /// bezier control points
  float mControlPoints[4][3];
  
  /// current position and derivatives' positions
  float mPoint[3];
  float mDPoint[3];
  float mDDPoint[3];
  float mDDDPoint[3];

  /// bezier basis matrix
  float mBezierBasis[4][4] = {{-1,3,-3,1}, {3,-6,3,0}, {-3,3,0,0}, {1,0,0,0}};

  bool bStarted;

 public:
  
  ParametricCurve();

  ///sets control points for the curve
  void SetParameter(int steps, float control1[], float control2[], float control3[], float control4[]);

  /// go to next step of the curve
  void Increment();

  /// returns current position
  void GetCurrent(float*& out); 

  ///initialize the curve
  void Start();

  ///see if the curve had reached the end
  bool Done();

  bool Started();
};

#endif
