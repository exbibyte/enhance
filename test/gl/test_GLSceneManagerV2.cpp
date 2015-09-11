//opengl includes
#include <GLFW/glfw3.h>

#include "GLSceneManager.h"
#include "GLSLProgram.h"
#include "WindowManagerGlfw.h"
#include "WingedEdge.h"
#include "enTPCommon.h"

#include "DataTransformDriver.h"
#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"
#include "Filter_ParsePolyMesh.h"
#include "PolyMesh_Data_Arrays.h"
#include "GLBufferInfo.h"
#include "Clock.h"

#include <functional>
#include <iostream>
#include <cassert>

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

        //parse input polymesh file
        PassParsePolyMesh pass_parse_polymesh;
        PassParsePolyMesh * p_pass_parse_polymesh = & pass_parse_polymesh;

        PassConvertPolyMeshDataStructToArray pass_convert_polymesh_to_array;
        PassConvertPolyMeshDataStructToArray * p_pass_convert_polymesh_to_array = & pass_convert_polymesh_to_array;
    
        DataTransformDriver data_transform_driver;

        //set meta info for parsing polymesh
        DataTransformMetaInfo meta_info_parse_polymesh("parse_polymesh");
        string strFilePathPolyMesh = strPathPolyMesh;
        meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATATYPE", "FILE_POLYMESH" );
        meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATAPATH", strFilePathPolyMesh );
        meta_info_parse_polymesh.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH");
        if( !pass_parse_polymesh.RegisterDataTransformMetaInfo( & meta_info_parse_polymesh ) ){
            assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
            return false;
        }

        //set meta info for converting polymesh to array
        DataTransformMetaInfo meta_info_convert_polymesh_to_array("convert_polymesh_to_array");
        meta_info_convert_polymesh_to_array.AddMetaInfo( "INPUT_DATATYPE", "DATASTRUCT_POLYMESH" );
        meta_info_convert_polymesh_to_array.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH_ARRAY");
        if( !pass_convert_polymesh_to_array.RegisterDataTransformMetaInfo( & meta_info_convert_polymesh_to_array ) ){
            assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
            return false;
        }    

        //register passes
        if( !data_transform_driver.RegisterPass( p_pass_parse_polymesh ) ){
            assert( 0 && "DataTransformDriver::RegisterPass failed.");
            return false;
        }
        if( !data_transform_driver.RegisterPass( p_pass_convert_polymesh_to_array ) ){
            assert( 0 && "DataTransformDriver::RegisterPass failed.");
            return false;
        }

        void * data_in;
        void * data_out;
        if( !data_transform_driver.ExecutePasses( data_in, data_out ) ){
            assert( 0 && "DataTransformDriver::ExecutePasses failed.");
            return false;
        }

        PolyMesh_Data_Arrays * polymesh_data_arrays = ( PolyMesh_Data_Arrays * ) data_out;

        float * data_vertex;
        float * data_normal;
        int iNumDataVertex;
        int iNumDataNormal;
        if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::VERTEX, data_vertex, iNumDataVertex ) ){
            assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
            return false;
        }
        if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::NORMAL, data_normal, iNumDataNormal ) ){
            assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
            return false;
        }

	map< string, GLBufferInfo * > map_buffer_info;
	polymesh_data_arrays->GetMapBufferInfo( map_buffer_info );
	map< string, GLBufferInfoSequence * > map_buffer_info_sequence;
	polymesh_data_arrays->GetMapBufferInfoSequence( map_buffer_info_sequence );
	for( auto i : map_buffer_info ){
	    _GLSLProgram->SetBufferInfo( i.second );
	}
	for( auto i : map_buffer_info_sequence ){
	    _GLSLProgram->SetBufferInfoSequence( i.second );
	}
	
        if( !data_transform_driver.CleanUpPasses() ){
            assert( 0 && "DataTransformDriver::CleanUpPasses failed.");
            return false;
        }
    
        float * current_vertex = data_vertex;
        float * current_normal = data_normal;

	cout << "vertex: " << endl;
        for( int i = 0; i < iNumDataVertex; i++ ){
            cout << *current_vertex++ << " ";
        }
        cout << endl;
        cout << "normal: " << endl;
        for( int i = 0; i < iNumDataNormal; i++ ){
            cout << *current_normal++ << " ";
        }
        cout << endl;
    
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
        //pPositionData->SetData( data_vertex, 3, 27 );
	//pNormalData->SetData( data_normal, 3, 27 );
        pPositionData->SetData( data_vertex, 3, iNumDataVertex );
        pNormalData->SetData( data_normal, 3, iNumDataNormal );

        _GLSLProgram->Use();

        _GLSLProgram->AddNewTexture("ShadowTexture", GLTexture::DEPTH, 2500, 2500, 0, 0 );

        //deallocate data
        delete [] data_vertex;
        data_vertex = nullptr;
        delete [] data_normal;
        data_normal = nullptr;

	// //set buffer segments
	// GLBufferInfo * buffer_info = new GLBufferInfo;
	// buffer_info->_Name = "BufSeg_01";
	// buffer_info->_Offset = 0;
	// buffer_info->_Length = 9;

	// if( !_GLSLProgram->SetBufferInfo( buffer_info ) ){
	//     return false;
	// }
	// if( !_GLSLProgram->SetCurrentBufferInfo( "BufSeg_01" ) ){
	//     return false;
	// }
	
        return true;
    }

    bool Render( GLSLProgram * & _GLSLProgram ){
        bool bRet;
//      cout << "func body 01 - Render Scene" << endl;
        dAngle += 0.005;

        mat4 Model = mat4(1.0f);
        // mat4 ModelMatrix = glm::rotate( Model, -dAngle, vec3( 0.0f, 0.2f, 0.7f ) );
	mat4 ModelMatrix = Model;
	
        //first pass render for light POV    
        glViewport( 0, 0, 2500, 2500 );
        mat4 ViewMatrix = glm::lookAt( vec3(0.0,0.0,20.0), 
                                       vec3(0.0,0.0,0.0),
                                       vec3(0.0,1.0,0.0) );
	
	// mat4 ViewMatrix = glm::lookAt( vec3(fLightPos_x,fLightPos_y,15.0), 
	// 			       vec3(0.0,0.0,0.0),
	// 			       vec3(0.0,1.0,0.0) );
        mat4 ProjectionMatrixLight = glm::perspective( 60.0f, 1.0f, 0.1f, 1000.0f );

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
        //Multiply it be the bias matrix
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
        // bRet = _GLSLProgram->SetUniform( "ProjectionMatrix", (mat4 const) ProjectionMatrixLight );
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
	float fDeltaLight = -0.01;
        vec4 LightPosition( 0.0f, 0.0f, 20.0f, 1.0 );
	// vec4 LightPosition( fLightPos_x, fLightPos_y, 15.0f, 1.0 );
	fLightPos_x += fDeltaLight;
	fLightPos_y += fDeltaLight;
        bRet = _GLSLProgram->SetUniform( "Light.Position", LightPosition );

	mat4 LightViewMatrix = ViewMatrix;
	// mat4 LightViewMatrix = Model;
	bRet = _GLSLProgram->SetUniform( "LightViewMatrix", (mat4 const) LightViewMatrix );
	
        vec3 MaterialCoeffKa( 1.0f, 1.0f, 1.0f );
        vec3 MaterialCoeffKd( 1.0f, 1.0f, 1.0f );
        vec3 MaterialCoeffKs( 1.0f, 1.0f, 1.0f );
        bRet = _GLSLProgram->SetUniform( "Material.Ka", MaterialCoeffKa );
        bRet = _GLSLProgram->SetUniform( "Material.Kd", MaterialCoeffKd );
        bRet = _GLSLProgram->SetUniform( "Material.Ks", MaterialCoeffKs );
        bRet = _GLSLProgram->SetUniform( "Material.Shininess", 2.0f );
        _GLSLProgram->BindVertexArray();
	if( !_GLSLProgram->SetCurrentBufferInfo( "square" ) ){
	    assert( 0 && "Cannot set buffer info for square." );
	    return false;
	}
	if( !_GLSLProgram->DrawCurrentBufferSegment() ){
	    return false;
	}
	//set orientation for the objects to render below
	mat4 ObjOrientationMatrix = glm::rotate( Model, -2*dAngle, vec3( 0.0f, 0.5f, 0.7f ) );
	// mat4 ObjOrientationMatrix = Model;
        mat4 ModelOrientationViewMatrix = ViewMatrix * ObjOrientationMatrix * ModelMatrix;
	// mat4 ModelOrientationViewMatrix = ObjOrientationMatrix * ModelMatrix;
	mat4 MOVP = ProjectionMatrixLight * ViewMatrix  * ObjOrientationMatrix * ModelMatrix;
	mat4 MOVPB = Bias * ProjectionMatrixLight * ViewMatrix * ObjOrientationMatrix * ModelMatrix;
        bRet = _GLSLProgram->SetUniform( "ShadowMatrix", (mat4 const) MOVPB );
        mat3 NormalMatrixOrientation = glm::inverse( glm::transpose( glm::mat3(ModelOrientationViewMatrix) ) );
        bRet = _GLSLProgram->SetUniform( "MVP", (mat4 const) MOVP );
        // // bRet = _GLSLProgram->SetUniform( "ProjectionMatrix", (mat4 const) ProjectionMatrixLight );
	LightViewMatrix = ViewMatrix;
	// LightViewMatrix = Model;
	bRet = _GLSLProgram->SetUniform( "LightViewMatrix", (mat4 const) LightViewMatrix );
	
        bRet = _GLSLProgram->SetUniform( "ModelViewMatrix", (mat4 const) ModelOrientationViewMatrix );
        bRet = _GLSLProgram->SetUniform( "NormalMatrix", (mat3 const) NormalMatrixOrientation );
	if( !_GLSLProgram->SetCurrentBufferInfoSequence( "seq_01" ) ){
	    assert( 0 && "Cannot set buffer info for seq_01." );
	    return false;
	}
	bool bIncrement = false;
	if( !_GLSLProgram->DrawCurrentBufferSequence( bIncrement ) ){
	    return false;
	}
        _GLSLProgram->UnBindVertexArray();

        //2nd pass render 
        glCullFace(GL_BACK);

        glViewport( 0, 0, 500, 500 );
        ViewMatrix = glm::lookAt( vec3(-5.0,-5.0,8.0), 
                                  vec3(0.0,0.0,0.0),
                                  vec3(0.0,1.0,0.0) );

	mat4 ViewOrientationMatrix = glm::rotate( Model, -dAngle, vec3( 0.0f, 0.2f, 0.7f ) );
	ViewMatrix = ViewMatrix * ViewOrientationMatrix;
	
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
        // bRet = _GLSLProgram->SetUniform( "ProjectionMatrix", (mat4 const) ProjectionMatrix );

	bRet = _GLSLProgram->SetUniform( "ShadowMatrix", (mat4 const) MVPB );
        bRet = _GLSLProgram->SetUniform( "MVP", (mat4 const) MVP ); //added
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
	if( !_GLSLProgram->SetCurrentBufferInfo( "square" ) ){
	    return false;
	}
	if( !_GLSLProgram->DrawCurrentBufferSegment() ){
	    return false;
	}
	LightViewMatrix = ViewMatrix;
	// LightViewMatrix = Model;
	bRet = _GLSLProgram->SetUniform( "LightViewMatrix", (mat4 const) LightViewMatrix );
	//set orientation for the objects to render below
	// ObjOrientationMatrix = glm::rotate( Model, 2*dAngle, vec3( 0.0f, 0.2f, 0.7f ) );
	MOVP = ProjectionMatrix * ViewMatrix  * ObjOrientationMatrix * ModelMatrix;
        bRet = _GLSLProgram->SetUniform( "MVP", (mat4 const) MOVP );
        bRet = _GLSLProgram->SetUniform( "ModelViewMatrix", (mat4 const) ModelOrientationViewMatrix );
        bRet = _GLSLProgram->SetUniform( "NormalMatrix", (mat3 const) NormalMatrixOrientation );

	bRet = _GLSLProgram->SetUniform( "ShadowMatrix", (mat4 const) MOVPB );
		
	if( !_GLSLProgram->SetCurrentBufferInfoSequence( "seq_01" ) ){
	    return false;
	}
	bIncrement = true;
	if( !_GLSLProgram->DrawCurrentBufferSequence( bIncrement ) ){
	    return false;
	}
        _GLSLProgram->UnBindVertexArray();
        return true;
    }
    float dAngle = 0;
    string strPathPolyMesh;
    int iWaitRender = 5;
    int iWaitRenderCurrent = 0;
    float fLightPos_x = 5;
    float fLightPos_y = 5;
};

void RenderTask( GLFWwindow * window, string strPathPolyMesh ) {

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
    sample_scene.strPathPolyMesh = strPathPolyMesh;
    
    std::function< bool( GLSLProgram * &) > func_wrap_init_01 = bind( &SampleScene::SetShaders, &sample_scene, std::placeholders::_1 );
    std::function< bool( GLSLProgram * &) > func_wrap_body_01 = bind( &SampleScene::Render, &sample_scene, std::placeholders::_1 );
    std::function< bool( GLSLProgram * &) > func_wrap_body_02 = func_body_02;
    std::function< bool( GLSLProgram * &) > func_wrap_cleanup_01 = func_cleanup_01;
    scene_manager.RegisterRoutine( "init_01", func_wrap_init_01, GLSceneRoutineType::INIT );
    scene_manager.RegisterRoutine( "body_01", func_wrap_body_01, GLSceneRoutineType::BODY );
    scene_manager.RegisterRoutine( "body_02", func_wrap_body_02, GLSceneRoutineType::BODY );
    scene_manager.UnregisterRoutine( "body_02" );
    scene_manager.RegisterRoutine( "cleanup_01", func_wrap_cleanup_01, GLSceneRoutineType::CLEANUP );

    int iWait = 5;
    int iWaitCurrent = 0;
    scene_manager.RunInit();

    Clock render_clock;
    render_clock.SetFps(25);
    bool bClockTicked = false;
    auto FuncClockTicked = [ &bClockTicked ](){
	bClockTicked = true;
    };
    render_clock.SetTickFunc( FuncClockTicked );
    render_clock.Run();

    while (!glfwWindowShouldClose(window)){
	if( bSignalExit ){
            break;
        }
	if( iWaitCurrent < 5 ){
	    iWaitCurrent++;
	    continue;
	}else{
	    iWaitCurrent = 0;
	}
	render_clock.Tick();
	if( bClockTicked ){
	    scene_manager.RunBody();
	    glfwSwapBuffers(window);
	    bClockTicked = false;
	}
    }
    render_clock.Pause();
    scene_manager.RunCleanup();
}

int main( int argc, char ** argv ){

    if( argc < 2 ){
        assert( 0 && "Require argument: path_to_polymesh_file" );
        return -1;
    }

    string strPathPolyMesh = string( argv[1] );
    
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

    std::future<void> ret1 = tp.AddTask(RenderTask, window, strPathPolyMesh );
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
