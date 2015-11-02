#ifndef EN_SCENE_H
#define EN_SCENE_H

class enScene {
public:
    virtual bool SetShaders( GLSLProgram * _GLSLProgram ){
        return false;
    }
    virtual bool Render( GLSLProgram * _GLSLProgram ){
        return false;
    }
    float _dAngle = 0;
    float _fLightPos_x = 5;
    float _fLightPos_y = 5;
    string _strPathPolyMesh;
};

#endif
