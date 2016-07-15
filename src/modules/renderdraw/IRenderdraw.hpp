#ifndef IRENDERDRAW_H
#define IRENDERDRAW_H

#include "RenderData.h"

class IRenderdraw {
public:
  virtual ~IRenderdraw(){}
  virtual bool render( RenderData ){ return false; }
};

#endif
