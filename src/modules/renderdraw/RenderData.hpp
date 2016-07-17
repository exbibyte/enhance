#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include "RenderEntity.h"
#include "RenderCamera.h"
#include "RenderContext.h"
#include "RenderLight.h"
#include "GLSLProgram.h"

class RenderData {
public:
    RenderEntity _entity;
    RenderCamera _camera;
    RenderContext _context;
    RenderLight _light;
    GLSLProgram _glslprogram;
};

#endif
