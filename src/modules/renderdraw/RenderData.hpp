#ifndef RENDER_DATA_H
#define RENDER_DATA_H

class RenderData {
public:
    RenderEntity _entity;
    RenderCameraData _camera;
    RenderContextData _context;
    RenderLightData _light;
    GLSLProgram _glslprogram;
};

#endif
