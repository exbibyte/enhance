#include "ParametricCurve.h"
#include "MatrixMath.h"

#include <iostream>

using namespace std;

ParametricCurve::ParametricCurve()
{
  this->bStarted = false;
}

void ParametricCurve::SetParameter(int steps, float control1[], float control2[], float control3[], float control4[])
{

  this->mTotalStep = steps;
  for(int j = 0; j < 3; j++)
  {
    this->mControlPoints[0][j] = control1[j];
    this->mControlPoints[1][j] = control2[j];
    this->mControlPoints[2][j] = control3[j];
    this->mControlPoints[3][j] = control4[j];
  }
}

void ParametricCurve::Increment()
{
  for(int i = 0; i < 3; i++)
  {
    this->mPoint[i] += this->mDPoint[i];
    this->mDPoint[i] += this->mDDPoint[i];
    this->mDDPoint[i] += this->mDDDPoint[i];
  }
  this->mCurrentStep++;
}

void ParametricCurve::GetCurrent(float*& out)
{
  for(int i = 0; i < 3; i++)
  {
    out[i] = this->mPoint[i];
  }
}

void ParametricCurve::Start()
{
  this->mCurrentStep = 0;
  
  float d = 1.f/(this->mTotalStep-1);
  float d2 = d*d;
  float d3 = d*d2;

  float px[4];
  float py[4];
  float pz[4];

  for(int i = 0; i < 4; i++)
  {
    px[i] = this->mControlPoints[i][0];
    py[i] = this->mControlPoints[i][1];
    pz[i] = this->mControlPoints[i][2];
  }

  float cVec[3][4];
  
  float * pVec = &cVec[0][0];
  float * pLeftSide = px;  
  float * basis = &this->mBezierBasis[0][0];
  MatrixMath::Mat4x4Mult4x1(pLeftSide, basis, pVec);
  pLeftSide = py;
  pVec += 4;
  MatrixMath::Mat4x4Mult4x1(pLeftSide, basis, pVec);
  pLeftSide = pz;
  pVec += 4;
  MatrixMath::Mat4x4Mult4x1(pLeftSide, basis, pVec);

  for(int i = 0; i < 3; i++)
  {
    this->mPoint[i] = mControlPoints[0][i];
  }

  for(int i = 2; i >= 0; i--)
  {
    float a = cVec[i][0];
    float b = cVec[i][1];
    float c = cVec[i][2];

    mDPoint[i] = a * d3 + b * d2 + c * d;
    mDDPoint[i] = 6 * a * d3 + 2 * b * d2;
    mDDDPoint[i] = 6 * a * d3;
  } 

  this->bStarted = true;
}

bool ParametricCurve::Done()
{
  return (this->mCurrentStep == this->mTotalStep-1);
}

bool ParametricCurve::Started()
{
  return this->bStarted;
}
