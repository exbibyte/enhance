#ifndef IRENDERCOMPUTE_H
#define IRENDERCOMPUTE_H

class IRenderCompute {
public:
    virtual ~IRenderCompute(){}
    virtual bool compute( std::vector<RenderContext> context, std::vector<RenderEntity> entities, std::vector<RenderLight> light, std::vector<RenderCamera> camera ){ return false; }
};

#endif
