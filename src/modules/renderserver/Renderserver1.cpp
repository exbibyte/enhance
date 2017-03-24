#include "Renderserver1.hpp"
#include "IRenderserver.hpp"

#include "GLIncludes.hpp"
#include "GLSLProgram.h"

#include <iostream>
#include <memory>
#include <list>

bool Renderserver1::init(){

    _glslprogram = std::make_shared<GLSLProgram>();

    cout << "set shaders" << endl;
    _glslprogram->CompileShaderFromFile("../../src/gl/shaders/ADS.vert", GLSLShader::VERTEX );
    _glslprogram->CompileShaderFromFile("../../src/gl/shaders/ADS.frag", GLSLShader::FRAGMENT );
    _glslprogram->AttachShaders();

    //allocate space for storing polymesh data and bind to related attribute
    GLAttribData<float> * pPositionData = new GLAttribData<float>;
    GLAttribData<float> * pNormalData = new GLAttribData<float>;
    //save mapping of data
    _glslprogram->AddMapAttrib( "VertexPosition", pPositionData );
    _glslprogram->AddMapAttrib( "VertexNormal", pNormalData );
    //bind the above attributes to vertex array object
    _glslprogram->BindMapAttrib();    

    //bind other attributes
    _glslprogram->BindFragDataLocation( 0, "FragColor" );

    _glslprogram->Link();

    cout << "After linking program" << endl;
    
    _glslprogram->PrintActiveUniforms();

    _glslprogram->PrintActiveAttribs();

    _glslprogram->Use();
    
    return true;
}

bool Renderserver1::deinit(){
    return true;
}
