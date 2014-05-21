#include "DualQuat.h"

DualQuat::DualQuat(Quat a, Quat b){
  _A = a;
  _B = b;
}

DualQuat::DualQuat(){
  _A._quat[0] = 0;
  _A._quat[1] = 0;
  _A._quat[2] = 0; 
  _A._quat[3] = 1;

  _B._quat[0] = 0;
  _B._quat[1] = 0;
  _B._quat[2] = 0; 
  _B._quat[3] = 1;
}

DualQuat DualQuat::operator + (const DualQuat & q) const{
  DualQuat d;

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    d._A._quat[i] = _A._quat[i] + q._A._quat[i];
    d._B._quat[i] = _B._quat[i] + q._B._quat[i];
  }

  return d;
}

DualQuat DualQuat::operator - (const DualQuat & q) const{
  DualQuat d;

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    d._A._quat[i] = _A._quat[i] - q._A._quat[i];
    d._B._quat[i] = _B._quat[i] - q._B._quat[i];
  }

  return d;
}

DualQuat DualQuat::operator * (const DualQuat & q) const{

  Quat a;
  Quat b;

  a = _A * q._A;
  b = ( _A * q._B ) + ( _B * q._A );

  DualQuat d(a, b);

  return d;
}
