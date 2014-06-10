#ifndef EN_ENTITY_H
#define EN_ENTITY_H

#include "enClass.h"
#include "DualQuat.h"

class enEntity : public enClass {

 public:

  double         _rotaxis[3];
  double         _rotmag;
  double         _pos[3];  
  DualQuat       _dualquat;

};

#endif
