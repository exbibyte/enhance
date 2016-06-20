#ifndef IRENDERCOMPUTE_H
#define IRENDERCOMPUTE_H

class IRenderCompute {
public:
    virtual ~IRenderCompute(){}
    bool render_compute( vector<RenderContext> context, vector<RenderEntity> entities, vector<RenderLight> light, vector<RenderCamera> camera ){ return false; }
};

#endif
