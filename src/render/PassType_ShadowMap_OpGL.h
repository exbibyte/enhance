#ifndef PASSTYPE_SHADOWMAP_OPGL
#define PASSTPYE_SHADOWMAP_OPGL

class PassType_ShadowMap_OpGL {
public:
    bool Render( GLSLProgram *, Camera, Lights, Poly );
private:
    bool ProcessPassDepth();
    bool ProcessPassNormal();
    bool ProcessPassCommon();
};

#endif
