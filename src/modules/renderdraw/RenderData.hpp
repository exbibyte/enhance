#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include "RenderEntity.h"
#include "RenderCamera.h"
#include "RenderContext.h"
#include "RenderLight.h"
#include "GLSLProgram.h"

#include <list>
#include <memory>

class RenderData {
public:
    std::list<std::shared_ptr<RenderEntity> > _entities;
    std::shared_ptr<RenderCamera> _camera;
    std::shared_ptr<RenderContext> _context;
    std::shared_ptr<RenderLight> _light;
    GLSLProgram * _glslprogram;
};

#endif
