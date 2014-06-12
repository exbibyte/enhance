#ifndef EN_TRIGGER_H
#define EN_TRIGGER_H

#include "enEntity.h"

class enTrigger : public enEntity {
 public:
  virtual void Enable();
  virtual void Disable();
};
