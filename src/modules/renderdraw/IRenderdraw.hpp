#ifndef IRENDERDRAW_H
#define IRENDERDRAW_H

#include "RenderData.hpp"

class IRenderdraw {
public:
  virtual ~IRenderdraw(){}
  virtual bool render( RenderData ){ return false; }
};

#endif
