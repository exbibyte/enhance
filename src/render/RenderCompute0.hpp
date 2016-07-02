#ifndef RENDER_COMPUTE0_H
#define RENDER_COMPUTE0_H

#include "RenderCamera.h"
#include "RenderLight.h"
#include "RenderEntity.h"
#include "RenderContext.h"

class RenderCompute0 : public IRenderCompute {
public:
    bool compute( std::vector<RenderContext> context, std::vector<RenderEntity> entities, std::vector<RenderLight> light, std::vector<RenderCamera> camera );
};

#endif
