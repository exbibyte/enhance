#ifndef GLRENDER_H
#define GLRENDER_H

namespace GLRender {
    float Angle = 0;
    GLSLProgram * _GLSLProgram;
    void ChangeSize( int, int );
    void RenderScene( void );
    void ProcessNormalKeys(unsigned char key, int x, int y);
    void SetShaders();
};

void GLRender::ChangeSize( int w, int h ) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    float ratio = 1.0* w / h;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
}

void GLRender::RenderScene( void ) {

    bool bRet;

    GLRender::Angle += 0.005;

    mat4 Model = mat4(1.0f);
    mat4 ModelMatrix = glm::rotate( Model, Angle, vec3( 0.0f, 0.2f, 0.7f ) );

    //first pass render for light POV    
    glViewport( 0, 0, 1000, 1000 );
    mat4 ViewMatrix = glm::lookAt( vec3(5.0,5.0,20.0), 
                                   vec3(0.0,0.0,0.0),
                                   vec3(0.0,1.0,0.0) );
    mat4 ProjectionMatrixLight = glm::perspective( 90.0f, 1.0f, 0.1f, 100.0f );

    GLTexture * ShadowTexture;
    if( _GLSLProgram->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
        ShadowTexture->BindFbo();
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    bRet = _GLSLProgram->SetUniform( "ShadowMap", 0 );
    // GLuint RecordDepthIndex = glGetSubroutineIndex( _GLSLProgram->GetHandle(), GL_FRAGMENT_SHADER, "recordDepth" ); 
    // glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &RecordDepthIndex);
    bRet = _GLSLProgram->SetUniform( "bShadeShadow", false );
    glCullFace(GL_BACK);

    //draw on first pass
    // Multiply it be the bias matrix
    glm::mat4 Bias(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    mat4 ModelViewMatrix = ViewMatrix  * ModelMatrix;
    mat4 MVP = ProjectionMatrixLight * ViewMatrix  * ModelMatrix;
    mat4 MVPB = Bias * ProjectionMatrixLight * ViewMatrix * ModelMatrix;
    bRet = _GLSLProgram->SetUniform( "ShadowMatrix", (mat4 const) MVPB );
    mat3 NormalMatrix = glm::inverse( glm::transpose( glm::mat3(ModelViewMatrix) ) );
    bRet = _GLSLProgram->SetUniform( "MVP", (mat4 const) MVP );
    bRet = _GLSLProgram->SetUniform( "ProjectionMatrix", (mat4 const) ProjectionMatrixLight );
    bRet = _GLSLProgram->SetUniform( "ModelViewMatrix", (mat4 const) ModelViewMatrix );
    bRet = _GLSLProgram->SetUniform( "NormalMatrix", (mat3 const) NormalMatrix );
    vec3 LightLa;
    vec3 LightLd;
    vec3 LightLs;
    LightLa = vec3( 0.05, 0.05, 0.05 );
    LightLd = vec3( 0.5, 0.5, 0.5 );
    LightLs = vec3( 0.45, 0.45, 0.45 );
    bRet = _GLSLProgram->SetUniform( "Light.La", LightLa );
    bRet = _GLSLProgram->SetUniform( "Light.Ld", LightLd );
    bRet = _GLSLProgram->SetUniform( "Light.Ls", LightLs );
    vec4 LightPosition( 5.0f, 5.0f, 20.0f, 1.0 );
    bRet = _GLSLProgram->SetUniform( "Light.Position", LightPosition );
    vec3 MaterialCoeffKa( 1.0f, 1.0f, 1.0f );
    vec3 MaterialCoeffKd( 1.0f, 1.0f, 1.0f );
    vec3 MaterialCoeffKs( 1.0f, 1.0f, 1.0f );
    bRet = _GLSLProgram->SetUniform( "Material.Ka", MaterialCoeffKa );
    bRet = _GLSLProgram->SetUniform( "Material.Kd", MaterialCoeffKd );
    bRet = _GLSLProgram->SetUniform( "Material.Ks", MaterialCoeffKs );
    bRet = _GLSLProgram->SetUniform( "Material.Shininess", 2.0f );
    _GLSLProgram->BindVertexArray();
    glDrawArrays( GL_TRIANGLES, 0, 9 );
    _GLSLProgram->UnBindVertexArray();

    //2nd pass render 
    glCullFace(GL_FRONT);

    glViewport( 0, 0, 500, 500 );
    ViewMatrix = glm::lookAt( vec3(-5.0,-5.0,8.0), 
                                   vec3(0.0,0.0,0.0),
                                   vec3(0.0,1.0,0.0) );
    if( _GLSLProgram->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
        ShadowTexture->UnbindFbo();
    }
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // GLuint ShadowIndex = glGetSubroutineIndex( _GLSLProgram->GetHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow" );
    // glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &ShadowIndex );
    bRet = _GLSLProgram->SetUniform( "bShadeShadow", true );

    //draw on 2nd pass
    ModelViewMatrix = ViewMatrix * ModelMatrix;
    mat4 ProjectionMatrix = glm::perspective( 90.0f, 1.0f, 0.1f, 500.0f );
    MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    NormalMatrix = glm::inverse( glm::transpose( glm::mat3(ModelViewMatrix) ) );
    bRet = _GLSLProgram->SetUniform( "MVP", (mat4 const) MVP );
    bRet = _GLSLProgram->SetUniform( "ProjectionMatrix", (mat4 const) ProjectionMatrix );
    bRet = _GLSLProgram->SetUniform( "ModelViewMatrix", (mat4 const) ModelViewMatrix );
    bRet = _GLSLProgram->SetUniform( "NormalMatrix", (mat3 const) NormalMatrix );
    bRet = _GLSLProgram->SetUniform( "Light.La", LightLa );
    bRet = _GLSLProgram->SetUniform( "Light.Ld", LightLd );
    bRet = _GLSLProgram->SetUniform( "Light.Ls", LightLs );
    bRet = _GLSLProgram->SetUniform( "Light.Position", LightPosition );
    bRet = _GLSLProgram->SetUniform( "Material.Ka", MaterialCoeffKa );
    bRet = _GLSLProgram->SetUniform( "Material.Kd", MaterialCoeffKd );
    bRet = _GLSLProgram->SetUniform( "Material.Ks", MaterialCoeffKs );
    bRet = _GLSLProgram->SetUniform( "Material.Shininess", 1.0f );
    _GLSLProgram->BindVertexArray();
    glDrawArrays( GL_TRIANGLES, 0, 9 );
    _GLSLProgram->UnBindVertexArray();
}

void GLRender::ProcessNormalKeys(unsigned char key, int x, int y) {
    if (key == 27) 
        exit(0);
}

void GLRender::SetShaders() {

    _GLSLProgram = new GLSLProgram;
    _GLSLProgram->CompileShaderFromFile("../../src/gl/shaders/Shadow.vert", GLSLShader::VERTEX );
    _GLSLProgram->CompileShaderFromFile("../../src/gl/shaders/Shadow.frag", GLSLShader::FRAGMENT );
    _GLSLProgram->AttachShaders();

    //set buffer data
    GLAttribData<float> * pPositionData = new GLAttribData<float>;
    GLAttribData<float> * pNormalData = new GLAttribData<float>;
    float arrayPositionData[] = {
        10.0f, 10.0f, -10.0f, //floor
        -10.0f, 10.0f, -10.0f, 
        -10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f, -10.0f, //floor
        10.0f, -10.0f, -10.0f,
        10.0f, 10.0f, -10.0f,
        -0.8f, -0.8f, 0.0f, // floating triangle
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f,
    };

    float arrayNormalData[] = {
        0.0f, 0.0f,  1.0f, //floor
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f, 
        0.0f, 0.0f,  1.0f, //floor
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f, //floating triangle
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
    };
    
    //save mapping of data
    _GLSLProgram->AddMapAttrib( "VertexPosition", pPositionData );
    _GLSLProgram->AddMapAttrib( "VertexNormal", pNormalData );
    //bind attributes
    _GLSLProgram->BindMapAttrib();

    _GLSLProgram->BindFragDataLocation( 0, "FragColor" );

    _GLSLProgram->Link();

    _GLSLProgram->PrintActiveUniforms();

    _GLSLProgram->PrintActiveAttribs();

    //generate VBO, populate and bind data to vertex attribute arrays
    pPositionData->SetData( &arrayPositionData[0], 3, 27 );
    pNormalData->SetData( &arrayNormalData[0], 3, 27 );

    _GLSLProgram->Use();

    _GLSLProgram->AddNewTexture("ShadowTexture", GLTexture::DEPTH, 1000, 1000, 0, 0 );
}


#endif