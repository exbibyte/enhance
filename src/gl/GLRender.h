#ifndef GLRENDER_H
#define GLRENDER_H

#include "WingedEdge.h"
#include "MatrixMath.h"

#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
using namespace Winged_Edge;
using namespace std;

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
    /* mat4 ViewMatrix = glm::lookAt( vec3(5.0,5.0,20.0),  */
    /*                                vec3(0.0,0.0,0.0), */
    /*                                vec3(0.0,1.0,0.0) ); */
    float eye[3] = { 5, 5, 20 };
    float at[3] = { 0, 0, 0 };
    float up[3] = { 0, 1, 0 };
    float temp_view_matrix[16];
    MatrixMath::LookAt( eye, at, up, temp_view_matrix );
    mat4 ViewMatrix = glm::make_mat4( temp_view_matrix );
    
    /* mat4 ProjectionMatrixLight = glm::perspective( 90.0f, 1.0f, 0.1f, 100.0f ); */
    float proj_matrix_light[16];
    MatrixMath::Perspective( 90.0f, 1.0f, 0.1f, 100.0f, proj_matrix_light );
    mat4 ProjectionMatrixLight = make_mat4( proj_matrix_light );
      
    GLTexture * ShadowTexture;
    if( _GLSLProgram->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
        ShadowTexture->BindFbo();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
        int iActiveTexture;
        ShadowTexture->GetActiveTexture( iActiveTexture );
        bRet = _GLSLProgram->SetUniform( "ShadowMap", iActiveTexture );
    }

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
    /* ViewMatrix = glm::lookAt( vec3(-5.0,-5.0,8.0),  */
    /*                                vec3(0.0,0.0,0.0), */
    /*                                vec3(0.0,1.0,0.0) ); */
    float eye[3] = { -5, -5, 8 };
    float at[3] = { 0, 0, 0 };
    float up[3] = { 0, 1, 0 };
    float temp_view_matrix_2[16];
    MatrixMath::LookAt( eye, at, up, temp_view_matrix_2 );
    ViewMatrix = glm::make_mat4( temp_view_matrix_2 );

    if( _GLSLProgram->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
        ShadowTexture->UnbindFbo();
    }
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // GLuint ShadowIndex = glGetSubroutineIndex( _GLSLProgram->GetHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow" );
    // glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &ShadowIndex );
    bRet = _GLSLProgram->SetUniform( "bShadeShadow", true );

    //draw on 2nd pass
    ModelViewMatrix = ViewMatrix * ModelMatrix;
    /* mat4 ProjectionMatrix = glm::perspective( 90.0f, 1.0f, 0.1f, 500.0f ); */
    float proj_mat[16];
    MatrixMath::Perspective( 90.0f, 1.0f, 0.1f, 500.0f, proj_mat );
    mat4 ProjectionMatrix = make_mat4( proj_mat );
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

    float vec_position[3];
    Vertex v0, v1, v2, v3, v4, v5, v6; //v0 to v3 corresponds to a sqaure, v4 to v6 correspoonds to a triangle
    //square
    vec_position[0] = 10.0f;
    vec_position[1] = 10.0f;
    vec_position[2] = -10.0f;
    v0.pos.SetFromArray( 3, vec_position );
    vec_position[0] = -10.0f;
    vec_position[1] = 10.0f;
    vec_position[2] = -10.0f;
    v1.pos.SetFromArray( 3, vec_position );
    vec_position[0] = -10.0f;
    vec_position[1] = -10.0f;
    vec_position[2] = -10.0f;
    v2.pos.SetFromArray( 3, vec_position );
    vec_position[0] = 10.0f;
    vec_position[1] = -10.0f;
    vec_position[2] = -10.0f;
    v3.pos.SetFromArray( 3, vec_position );
    //triangle
    vec_position[0] = -0.8f;
    vec_position[1] = -0.8f;
    vec_position[2] = 0.0f;
    v4.pos.SetFromArray( 3, vec_position );
    vec_position[0] = 0.8f;
    vec_position[1] = -0.8f;
    vec_position[2] = 0.0f;
    v5.pos.SetFromArray( 3, vec_position );
    vec_position[0] = 0.0f;
    vec_position[1] = 0.8f;
    vec_position[2] = 0.0f;
    v6.pos.SetFromArray( 3, vec_position );

    Edge e0, e1, e2, e3, e4, e5, e6, e7; // e0 to e4 makes a square, e5 to e7 makes a triangle
    MapEdge edge_map;
    edge_map[ &e0 ] = std::make_pair( &v0, &v1  );
    edge_map[ &e1 ] = std::make_pair( &v1, &v2  );
    edge_map[ &e2 ] = std::make_pair( &v2, &v0  );
    edge_map[ &e3 ] = std::make_pair( &v3, &v2  );
    edge_map[ &e4 ] = std::make_pair( &v0, &v3  );
    edge_map[ &e5 ] = std::make_pair( &v4, &v5  );
    edge_map[ &e6 ] = std::make_pair( &v5, &v6  );
    edge_map[ &e7 ] = std::make_pair( &v6, &v4  );

    Face f0, f1, f2; //f0 to f1 corresponds to square, f2 corresponds to triangle
    vector< Face * > faces { &f0, &f1, &f2 };
    
    MapFace face_map;
    bool bVerticeCCW = true;
    bool bNormalCCW = true;
    face_map[ &f0 ] = std::make_tuple( &v0, &v1, &v2, bVerticeCCW, bNormalCCW );
    face_map[ &f1 ] = std::make_tuple( &v0, &v3, &v2, !bVerticeCCW, !bNormalCCW );
    face_map[ &f2 ] = std::make_tuple( &v4, &v5, &v6, bVerticeCCW, bNormalCCW );

    vector< WingedEdge * > generated_wedges;
    bool bRet = Generate_WingedEdge( edge_map, face_map, generated_wedges ); 

    cout << "Get Triangles: " << endl;
    set< Face * > allfaces( faces.begin(), faces.end() );
    vector< Vec > vertices_pos;
    vector< Vec > vertices_normal;
    bRet = GetTriangles( allfaces, vertices_pos, vertices_normal );

    cout << "Generated number of vertice positions: " << vertices_pos.size() << endl;
    cout << "Generated number of vertice normals: " << vertices_normal.size() << endl;

    cout << "Vertex Position: " << endl;
    for( auto i : vertices_pos ){
	cout << i._vec[0] << ", " << i._vec[1] << ", " << i._vec[2] << endl;
    }
    cout << "Vertex Normals: " << endl;
    for( auto i : vertices_normal ){
	cout << i._vec[0] << ", " << i._vec[1] << ", " << i._vec[2] << endl;
    }      

    float * arrayPositionData = new float [ vertices_pos.size() * 3 ];
    float * arrayNormalData = new float[ vertices_normal.size() * 3 ];
    float * it = &arrayPositionData[0];
    for( auto i : vertices_pos ){
	*it = i._vec[0];
	it++;
	*it = i._vec[1];
	it++;
	*it = i._vec[2];
	it++;
    }
    it = &arrayNormalData[0];
    for( auto i : vertices_normal ){
	*it = i._vec[0];
	it++;
	*it = i._vec[1];
	it++;
	*it = i._vec[2];
	it++;
    }
    
    // float arrayPositionData[] = {
    //     10.0f, 10.0f, -10.0f, //floor
    //     -10.0f, 10.0f, -10.0f, 
    //     -10.0f, -10.0f, -10.0f,
    //     -10.0f, -10.0f, -10.0f, //floor
    //     10.0f, -10.0f, -10.0f,
    //     10.0f, 10.0f, -10.0f,
    //     -0.8f, -0.8f, 0.0f, // floating triangle
    //     0.8f, -0.8f, 0.0f,
    //     0.0f, 0.8f, 0.0f,
    // };

    // float arrayNormalData[] = {
    //     0.0f, 0.0f,  1.0f, //floor
    //     0.0f, 0.0f,  1.0f,
    //     0.0f, 0.0f,  1.0f, 
    //     0.0f, 0.0f,  1.0f, //floor
    //     0.0f, 0.0f,  1.0f,
    //     0.0f, 0.0f,  1.0f,
    //     0.0f, 0.0f,  1.0f, //floating triangle
    //     0.0f, 0.0f,  1.0f,
    //     0.0f, 0.0f,  1.0f,
    // };
    
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

    //deallocate data
    delete arrayPositionData;
    arrayPositionData = 0;
    delete arrayNormalData;
    arrayNormalData = 0;
}


#endif
