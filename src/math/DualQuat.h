#ifndef DUALQUAT_H
#define DUALQUAT_H

#include "Quat.h"

class DualQuat{
 public:

  //Dual = A + eB
  Quat              _A;		
  Quat              _B;

                    DualQuat();
                    DualQuat(Quat a, Quat b);

  DualQuat          operator + ( const DualQuat & q ) const;
  DualQuat          operator - ( const DualQuat & q ) const;
  DualQuat          operator * ( const DualQuat & q ) const;
  DualQuat inline   Conjugate(){ return DualQuat( _A.Conjugate(), _B.Conjugate()); }
  
};

#endif
