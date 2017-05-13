#include "parametric_curve.hpp"
#include "interpolate.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

namespace e2 { namespace math {

interpolate::interpolate()
{
    this->CurveIndex = 0;
    this->bKeepIncrementing = false;
    for(int i = 0; i < 3; i++)
	this->Position[i] = 0;

    this->bActivated = false;
}

interpolate::~interpolate()
{
    this->vp_parametric_curve.clear();
}

void interpolate::AddCurve(int steps, float ctrlpoint1[], float ctrlpoint2[], float ctrlpoint3[], float ctrlpoint4[])
{
    parametric_curve * pcurve = new parametric_curve();
 
    pcurve->set_parameter(steps, ctrlpoint1, ctrlpoint2, ctrlpoint3, ctrlpoint4);

    this->vp_parametric_curve.push_back(pcurve);

    this->bActivated = true;
}
  
void interpolate::Increment()
{
    if(this->vp_parametric_curve.empty())
	return;

    bActivated = true;

    this->itCurve = this->vp_parametric_curve.begin();
    this->itCurve += this->CurveIndex;

    if(this->itCurve != this->vp_parametric_curve.end())
    {  
	// initialize curve 
	if((*itCurve)->started() == false)
	{
	    (*itCurve)->start(); 
	}
    
	if((*itCurve)->done() == false)
	{
	    // update current curve
	    float * pPosition = &this->Position[0];   
	    (*itCurve)->get_current(pPosition);
	    (*itCurve)->increment();
	    if((*itCurve)->done() == true)
	    {	
		this->CurveIndex++;
		auto a = this->vp_parametric_curve.begin() + this->CurveIndex;
		if(a != this->vp_parametric_curve.end())
		{
		    (*a)->start();
		}
	    }
	}
	else
	{
	    //go to next curve if current curve is finished
	    this->CurveIndex++;
	    auto a = this->vp_parametric_curve.begin() + this->CurveIndex;
	    if(a != this->vp_parametric_curve.end())
	    {
		(*a)->start();
		// update current curve
		float * pPosition = &this->Position[0];   
		(*a)->get_current(pPosition);
		(*a)->increment();
	    }
	}
    }
}

void interpolate::PrintPosition()
{
    for(int i = 0; i < 3; i++)
    {
	std::cout<<this->Position[i]<<" ";
    }
    std::cout<<std::endl;
}

bool interpolate::GetKeepIncrementing()
{
    return this->bKeepIncrementing;
}

bool interpolate::SetKeepIncrementing(bool val)
{
    this->bKeepIncrementing = val;
    bActivated = true;
}

void interpolate::GetPosition(float out[])
{
    for(int i = 0; i < 3; i++)
	out[i] = this->Position[i];
}

bool interpolate::GetActivated()
{
    return this->bActivated;
}

bool interpolate::SetActivated(bool val)
{
    this->bActivated = val;
}

} }
