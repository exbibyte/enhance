#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include "RenderEntity.h"
#include "RenderCamera.h"
#include "RenderContext.h"
#include "RenderLight.h"
#include "GLSLProgram.h"

#include <list>

class RenderData {
public:
    std::list<RenderEntity* > * _entities;
    RenderCamera * _camera;
    RenderContext * _context;
    RenderLight * _light;
    GLSLProgram * _glslprogram;
};

#endif
