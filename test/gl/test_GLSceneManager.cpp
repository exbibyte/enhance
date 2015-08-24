//opengl includes
#include <GLFW/glfw3.h>

#include "GLSceneManager.h"
#include "GLSLProgram.h"
#include "WindowManagerGlfw.h"
#include "WingedEdge.h"
#include "enTPCommon.h"

#include <functional>
#include <iostream>

//math library
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat4;
using glm::vec3;

using namespace std;
using namespace Winged_Edge;

bool bSignalExit = false;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
bool key_callback_0()
{
    cout << "triggered key_callback_0" << endl;
    return true;
}
bool key_callback_1()
{
    cout << "triggered key_callback_1" << endl;
    bSignalExit = true;
    return true;
}

class SampleScene {
public:
    bool SetShaders( GLSLProgram * & _GLSLProgram ){
	bool bRet;
	cout << "func init 01 - Set Shaders" << endl;
	_GLSLProgram = new GLSLProgram;
	_GLSLProgram->CompileShaderFromFile("./src/gl/shaders/Shadow.vert", GLSLShader::VERTEX );
	_GLSLProgram->CompileShaderFromFile("./src/gl/shaders/Shadow.frag", GLSLShader::FRAGMENT );
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
	bRet = Generate_WingedEdge( edge_map, face_map, generated_wedges ); 

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

	return true;
    }

    bool Render( GLSLProgram * & _GLSLProgram ){
	bool bRet;
//	cout << "func body 01 - Render Scene" << endl;
	dAngle += 0.005;

	mat4 Model = mat4(1.0f);
	mat4 ModelMatrix = glm::rotate( Model, dAngle, vec3( 0.0f, 0.2f, 0.7f ) );

	//first pass render for light POV    
	glViewport( 0, 0, 1000, 1000 );
	mat4 ViewMatrix = glm::lookAt( vec3(5.0,5.0,20.0), 
				       vec3(0.0,0.0,0.0),
				       vec3(0.0,1.0,0.0) );
	mat4 ProjectionMatrixLight = glm::perspective( 90.0f, 1.0f, 0.1f, 100.0f );
      
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
	return true;
    }
    float dAngle = 0;
};

void RenderTask( GLFWwindow * window ) {

    auto func_body_02 = []( GLSLProgram * & glslprogram )->bool {
	cout << "func body 02" << endl;
	return true;
    };
    auto func_cleanup_01 = []( GLSLProgram * & glslprogram )->bool {
	cout << "func cleanup 01" << endl;
	return true;
    };
    
    glfwMakeContextCurrent( window );
    GLPrintInfo();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0);

    GLSLProgram * glsl_program;
    GLSceneManager scene_manager;
    scene_manager.SetGLSLProgram( glsl_program );

    SampleScene sample_scene;
    std::function< bool( GLSLProgram * &) > func_wrap_init_01 = bind( &SampleScene::SetShaders, &sample_scene, std::placeholders::_1 );
    std::function< bool( GLSLProgram * &) > func_wrap_body_01 = bind( &SampleScene::Render, &sample_scene, std::placeholders::_1 );
    std::function< bool( GLSLProgram * &) > func_wrap_body_02 = func_body_02;
    std::function< bool( GLSLProgram * &) > func_wrap_cleanup_01 = func_cleanup_01;
    scene_manager.RegisterRoutine( "init_01", func_wrap_init_01, GLSceneRoutineType::INIT );
    scene_manager.RegisterRoutine( "body_01", func_wrap_body_01, GLSceneRoutineType::BODY );
    scene_manager.RegisterRoutine( "body_02", func_wrap_body_02, GLSceneRoutineType::BODY );
    scene_manager.UnregisterRoutine( "body_02" );
    scene_manager.RegisterRoutine( "cleanup_01", func_wrap_cleanup_01, GLSceneRoutineType::CLEANUP );

    scene_manager.RunInit();
    while (!glfwWindowShouldClose(window)){
	if( bSignalExit ){
	    break;
	}
    scene_manager.RunBody();
    glfwSwapBuffers(window);
    }
    scene_manager.RunCleanup();
}

int main(){
    
    WindowManagerGlfw * manager_window;
    GLFWwindow * window;

    //init window
    if (!glfwInit())
	exit(EXIT_FAILURE);

    glfwSetErrorCallback(error_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // yes, 3 and 2!!!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS,24);

    //get version
    int major, minor, rev;
    glfwGetVersion( &major, &minor, &rev );
    cout<< "major: "<<major<<", minor: "<<minor<<", rev: "<<rev<<endl;

    //create window
    manager_window = new WindowManagerGlfw;
    int id_window = 1;
    if( !manager_window->CreateWindow( id_window, 500, 500, "Shadow Test" ) ){
	cout << "Error creating window" << endl;
	glfwTerminate();
	exit( EXIT_FAILURE );
    }
    if( !manager_window->GetWindow( window ) ){
	cout << "Error getting window" << endl;
	glfwTerminate();
	exit( EXIT_FAILURE );
    }

    glfwMakeContextCurrent(window);
    //add key combo and associated callback
    map<KeyButtonWhich::Enum, KeyButtonState::Enum> map_key_combo {
	{ KeyButtonWhich::KEY_J, KeyButtonState::DOWN },
	{ KeyButtonWhich::KEY_A, KeyButtonState::DOWN }      
    };
    if( !manager_window->SetKeyComboCallback( map_key_combo, key_callback_0 ) ){
	cout << "Error setting key combo callback" << endl;
	glfwTerminate();
	exit( EXIT_FAILURE );
    }
    map<KeyButtonWhich::Enum, KeyButtonState::Enum> map_key_combo_exit {
	{ KeyButtonWhich::KEY_L, KeyButtonState::DOWN }
    };
    if( !manager_window->SetKeyComboCallback( map_key_combo_exit, key_callback_1 ) ){
	cout << "Error setting key combo callback" << endl;
	glfwTerminate();
	exit( EXIT_FAILURE );
    }
    //register callback
    if( !manager_window->SetDefaultCb() ){
	cout << "Error registering key combo callback" << endl;
	exit( EXIT_FAILURE );
    }
  
    enTPCommon tp;
    tp.SetNumThreads(4);
    enTPCommon * ptp = &tp;

    int count = 0;

    std::future<void> ret1 = tp.AddTask(RenderTask, window );
//  std::future<void> ret2 = tp.AddTask( Idle, count, ptp );

    tp.RunThreads();

    while (!glfwWindowShouldClose(window))
    {
	if( bSignalExit ){
	    break;
	}
	glfwPollEvents();
	manager_window->ProcessKeyButtonCombo();
    }
  
    ret1.get();
//  ret2.get();

    tp.EndAllThreads();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
  
    return 0;
}
