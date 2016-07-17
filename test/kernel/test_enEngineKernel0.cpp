#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enEngineKernel0.hpp"
#include "enComponentType.hpp"
#include "enComponentMeta.hpp"
#include "enComponentClock.hpp"
#include "enComponentLogger.hpp"
#include "enComponentScheduler.hpp"
#include "enComponentStat.hpp"
#include "enComponentThread.hpp"
#include "enComponentInit.hpp"
#include "enComponentRenderdraw.hpp"

#include "Funwrap3.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>
#include <memory>

#include "GLIncludes.hpp"

#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformDriver.h"
#include "PolyMesh_Data_Arrays.h"

using namespace std;

TEST_CASE( "EnEngineKernel0", "[EnEngineKernel0]" ) {

    enEngineKernel0 engine_kernel;
    CHECK( engine_kernel.get_num_components() == 0 );

    engine_kernel.init();
    CHECK( engine_kernel.get_num_components() > 0 );
	
    SECTION( "clock, clock" ) {
	//clock
	vector<enComponentMeta*> clocks;
	engine_kernel.get_components_by_type( enComponentType::CLOCK, clocks );
	REQUIRE( clocks.size() == 1 );

	enComponentClock0 * clock0 = dynamic_cast< enComponentClock0 * >( clocks[0] );
	CHECK( nullptr != clock0 );
    }

    SECTION( "logger, logger" ) {
	//logger
	vector<enComponentMeta*> loggers;
	engine_kernel.get_components_by_type( enComponentType::LOGGER, loggers );
	REQUIRE( loggers.size() == 1 );

	COMPONENT_INSTANCE( logger_stdout, enComponentLoggerStdout, loggers.front() );
	logger_stdout->Log( "Engine Kernel {%s} initialized.", "0" );

	enComponentLoggerStdout * logger = dynamic_cast< enComponentLoggerStdout * >( loggers.front() );
	CHECK( nullptr != logger );

    }
    SECTION( "thread, thread" ) {
	//thread
	vector<enComponentMeta*> threads;
	engine_kernel.get_components_by_type( enComponentType::THREAD, threads );
	REQUIRE( threads.size() == 1 );
	COMPONENT_INSTANCE( thread0, enComponentThread0, threads.front() );
	CHECK( IThread::State::STOPPED == thread0->getstate() );
	{
	    for(int j = 0; j < 3; ++j ){
		bool i = false;
		CHECK( i == false );
		Funwrap3 f;
		f.set( FunCallType::ASYNC, [&]{ i = true; } );
		std::function<void()> fun_set_bool = [&](){
		    f.apply();
		};
		thread0->settask( fun_set_bool );
		while( !thread0->setaction( IThread::Action::START ) );
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		while( !thread0->setaction( IThread::Action::END ) );
		CHECK( i == true );
	    }
	}
    }
    SECTION( "scheduler, scheduler" ) {

	//scheduler
	vector<enComponentMeta*> schedulers;
	engine_kernel.get_components_by_type( enComponentType::SCHEDULER, schedulers );
	REQUIRE( schedulers.size() == 1 );
	COMPONENT_INSTANCE( scheduler0, enComponentScheduler0, schedulers.front() );
	{
	    scheduler0->run();
	    Funwrap3 f;
	    std::function<void()> fun_print = []()->bool{
		cout << "function called." << endl;
		return true;
	    };
	    f.set( FunCallType::ASYNC, fun_print );
	    scheduler0->add( f );

	}
	Funwrap3 g;
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	scheduler0->get( g );
	g.apply();
	scheduler0->stop();
    }
    SECTION( "stat, stat" ) {
	//stat
	vector<enComponentMeta*> stats;
	engine_kernel.get_components_by_type( enComponentType::STAT, stats );
	REQUIRE( stats.size() == 1 );
	COMPONENT_INSTANCE( stat0, enComponentStat0, stats.front() );
	CHECK( 0 < stat0->getstat().size() );
	std::cout << stat0->getstat() << std::endl;
    }
    SECTION( "initGL, initGL" ) {
	//initGL
	vector<enComponentMeta*> inits;
	engine_kernel.get_components_by_type( enComponentType::INIT, inits );
	REQUIRE( inits.size() == 1 );
	COMPONENT_INSTANCE( initGL, enComponentInitGL, inits.front() );
	bool bret = initGL->init();
	CHECK( true == bret );

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	SECTION( "renderdraw0, renderdraw0" ) {
	    //renderdraw0
	    vector<enComponentMeta*> renderdraws;
	    engine_kernel.get_components_by_type( enComponentType::RENDERDRAW, renderdraws );
	    REQUIRE( renderdraws.size() == 1 );
	    COMPONENT_INSTANCE( renderdraw0, enComponentRenderdraw0, renderdraws.front() );

	    RenderData renderdata;
	    renderdata._glslprogram = new GLSLProgram;

	    vector<double> vert_pos;
	    vector<double> vert_norm;
	    {
		bool bRet;
		cout << "set shaders" << endl;
		renderdata._glslprogram->CompileShaderFromFile("../../src/gl/shaders/Shadow.vert", GLSLShader::VERTEX );
		renderdata._glslprogram->CompileShaderFromFile("../../src/gl/shaders/Shadow.frag", GLSLShader::FRAGMENT );
		renderdata._glslprogram->AttachShaders();

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
		string strFilePathPolyMesh = "../../testcase/file/BS_Skate_v2.pmesh";
		meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATATYPE", "FILE_POLYMESH" );
		meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATAPATH", strFilePathPolyMesh );
		meta_info_parse_polymesh.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH");
		if( !pass_parse_polymesh.RegisterDataTransformMetaInfo( & meta_info_parse_polymesh ) ){
		    assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
		}

		//set meta info for converting polymesh to array
		DataTransformMetaInfo meta_info_convert_polymesh_to_array("convert_polymesh_to_array");
		meta_info_convert_polymesh_to_array.AddMetaInfo( "INPUT_DATATYPE", "DATASTRUCT_POLYMESH" );
		meta_info_convert_polymesh_to_array.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH_ARRAY");
		if( !pass_convert_polymesh_to_array.RegisterDataTransformMetaInfo( & meta_info_convert_polymesh_to_array ) ){
		    assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
		}    

		//register passes
		if( !data_transform_driver.RegisterPass( p_pass_parse_polymesh ) ){
		    assert( 0 && "DataTransformDriver::RegisterPass failed.");
		}
		if( !data_transform_driver.RegisterPass( p_pass_convert_polymesh_to_array ) ){
		    assert( 0 && "DataTransformDriver::RegisterPass failed.");
		}

		void * data_in;
		void * data_out;
		if( !data_transform_driver.ExecutePasses( data_in, data_out ) ){
		    assert( 0 && "DataTransformDriver::ExecutePasses failed.");
		}

		PolyMesh_Data_Arrays * polymesh_data_arrays = ( PolyMesh_Data_Arrays * ) data_out;

		float * data_vertex;
		float * data_normal;
		int iNumDataVertex;
		int iNumDataNormal;
		if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::VERTEX, data_vertex, iNumDataVertex ) ){
		    assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
		}
		if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::NORMAL, data_normal, iNumDataNormal ) ){
		    assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
		}

		map< string, GLBufferInfo * > map_buffer_info;
		polymesh_data_arrays->GetMapBufferInfo( map_buffer_info );
		map< string, GLBufferInfoSequence * > map_buffer_info_sequence;
		polymesh_data_arrays->GetMapBufferInfoSequence( map_buffer_info_sequence );
		for( auto i : map_buffer_info ){
		    renderdata._glslprogram->SetBufferInfo( i.second );
		}
		for( auto i : map_buffer_info_sequence ){
		    renderdata._glslprogram->SetBufferInfoSequence( i.second );
		}
        
		if( !data_transform_driver.CleanUpPasses() ){
		    assert( 0 && "DataTransformDriver::CleanUpPasses failed.");
		}
    
		float * current_vertex = data_vertex;
		float * current_normal = data_normal;

		// cout << "vertex: " << endl;
		// for( int i = 0; i < iNumDataVertex; i++ ){
		// 	cout << *current_vertex++ << " ";
		// }
		// cout << endl;
		// cout << "normal: " << endl;
		// for( int i = 0; i < iNumDataNormal; i++ ){
		// 	cout << *current_normal++ << " ";
		// }
		// cout << endl;
    
		//save mapping of data
		renderdata._glslprogram->AddMapAttrib( "VertexPosition", pPositionData );
		renderdata._glslprogram->AddMapAttrib( "VertexNormal", pNormalData );
		//bind the above attributes to vertex array object
		renderdata._glslprogram->BindMapAttrib();

		renderdata._glslprogram->BindFragDataLocation( 0, "FragColor" );

		renderdata._glslprogram->Link();

		renderdata._glslprogram->PrintActiveUniforms();

		renderdata._glslprogram->PrintActiveAttribs();

		renderdata._glslprogram->Use();
    
		renderdata._glslprogram->AddNewTexture("ShadowTexture", GLTexture::DEPTH, 2500, 2500, 0, 0 );

		cout << "After linking program" << endl;

		for( int i = 0; i < iNumDataVertex; ++i ){
		    vert_pos.push_back( data_vertex[i] );
		}
		for( int i = 0; i < iNumDataNormal; ++i ){
		    vert_norm.push_back( data_normal[i] );
		}
		cout << "Size of vert_pos: " << vert_pos.size() << endl;
		cout << "Size of vert_norm: " << vert_norm.size() << endl;

		delete [] data_vertex;
		data_vertex = nullptr;
		delete [] data_normal;
		data_normal = nullptr;
    
		cout << "End of Init Phase" << endl;

	    }

	    double rotation_angle = 0;
	    auto t1 = std::chrono::high_resolution_clock::now();
	     
	    while(true){
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> duration_ms = t2 - t1;
		if( duration_ms.count() > 10000 ){
		    break;
		}
		//entities -------------------------------------------------------------------
		//orientation	
		vector<double> entity_translate    { 0, 0, 0};
		vector<double> entity_rotate_axis  { 1, 0, 0};
		// vector<double> entity_rotate_angle { 0 };
	        rotation_angle += 0.15;
		vector<double> entity_rotate_angle { rotation_angle };
		//set material data
		vector<double> entity_material_ambient   { 1.0, 1.0, 1.0 };
		vector<double> entity_material_diffuse   { 1, 1, 1 };
		vector<double> entity_material_specular  { 1, 1, 1 };
		vector<double> entity_material_shininess { 2 };
		//set vertex data 
		vector<double> entity_vertices = vert_pos;
		vector<double> entity_normals = vert_norm;

		//light ----------------------------------------------------------------------
		vector<double> light_position     { 0, 0, 10 };
		vector<double> light_lookat       { 0, 0, 0 };
		vector<double> light_up           { 0, 1, 0 };
		vector<double> light_perspective  { 60.0f, 1.0f, 0.1f, 1000.0f };
		vector<double> light_ambient      { 0.05f, 0.05f, 0.05f };
		vector<double> light_diffuse      { 0.5f, 0.5f, 0.5f };
		vector<double> light_specular     { 0.45f, 0.45f, 0.45f };

		//camera ---------------------------------------------------------------------
		vector<double> camera_position    { -5, -5, 8.0 };
		vector<double> camera_lookat      { 0, 0, 0 };
		vector<double> camera_up          { 0, 1, 0 };
		vector<double> camera_perspective { 90.0f, 1.0f, 0.1f, 500.0f };
		vector<double> camera_ambient     { 0.05f, 0.05f, 0.05f };
		vector<double> camera_diffuse     { 0.5f, 0.5f, 0.5f };
		vector<double> camera_specular    { 0.45f, 0.45f, 0.45f };

		//context --------------------------------------------------------------------
		vector<int> context_windowsize { 500, 500 };
		vector<int> context_texturesize_shadowmap { 2500, 2500 };
		string context_title = "engine0";

		//apply settings -------------------------------------------------------------
		std::list< RenderEntity * > entities = std::list< RenderEntity * >();
		RenderEntity * entity_01 = new RenderEntity;
		entity_01->AddDataSingle( RenderPolyData::Coordinate(),    entity_translate );
		entity_01->AddDataSingle( RenderPolyData::RotationAxis(),  entity_rotate_axis );
		entity_01->AddDataSingle( RenderPolyData::RotationAngle(), entity_rotate_angle );

		entity_01->AddDataSingle( RenderMaterialData::Ambient(),   entity_material_ambient );
		entity_01->AddDataSingle( RenderMaterialData::Diffuse(),   entity_material_diffuse );
		entity_01->AddDataSingle( RenderMaterialData::Specular(),  entity_material_specular );
		entity_01->AddDataSingle( RenderMaterialData::Shininess(), entity_material_shininess );
		entity_01->AddDataSingle( RenderVertexData::Normals(),  entity_normals );
		entity_01->AddDataSingle( RenderVertexData::Vertices(), entity_vertices );

		entities.push_back( entity_01 );
    
		std::shared_ptr<RenderLight> light = std::make_shared< RenderLight >();
		light->AddDataSingle( RenderLightData::Coordinate(),  light_position    );
		light->AddDataSingle( RenderLightData::Lookat(),      light_lookat      );
		light->AddDataSingle( RenderLightData::Up(),          light_up          );
		light->AddDataSingle( RenderLightData::Perspective(), light_perspective );
		light->AddDataSingle( RenderLightData::Ambient(),     light_ambient     );
		light->AddDataSingle( RenderLightData::Diffuse(),     light_diffuse     );
		light->AddDataSingle( RenderLightData::Specular(),    light_specular    );
	    
		std::shared_ptr<RenderCamera> camera = std::make_shared< RenderCamera >();
		camera->AddDataSingle( RenderCameraData::Coordinate(),  camera_position    );
		camera->AddDataSingle( RenderCameraData::Lookat(),      camera_lookat      );
		camera->AddDataSingle( RenderCameraData::Up(),          camera_up          );
		camera->AddDataSingle( RenderCameraData::Perspective(), camera_perspective );
		camera->AddDataSingle( RenderCameraData::Ambient(),     camera_ambient     );
		camera->AddDataSingle( RenderCameraData::Diffuse(),     camera_diffuse     );
		camera->AddDataSingle( RenderCameraData::Specular(),    camera_specular    );

		std::shared_ptr<RenderContext> context = std::make_shared< RenderContext >();
		context->AddDataSingle( RenderContextData::WindowSize(),           context_windowsize );
		context->AddDataSingle( RenderContextData::TextureSizeShadowMap(), context_texturesize_shadowmap );
		context->AddDataSingle( RenderContextData::Title(),                context_title );

		renderdata._entities = entities;
		renderdata._light = light;
		renderdata._camera = camera;
		renderdata._context = context;
	    
		//render call ----------------------------------------------------------------
		renderdraw0->render( renderdata );

		WindowInfo windowinfo = ((InitGL*)initGL)->GetWindowResource();
		glfwMakeContextCurrent( windowinfo._window ); // this is need when calling rendering APIs on separate thread
		glfwSwapBuffers( windowinfo._window );
	    
		std::cout << "post renderdraw0." << std::endl;

		delete entity_01;
		
		std::this_thread::sleep_for(std::chrono::milliseconds(70));
	    }
	}
	// SECTION( "GLSLProgram, GLSLProgram" ) {
	//     GLSLProgram glslprogram;
	//     // GLSLProgram * glslprogram = new GLSLProgram;
	//     std::cout << "glslprogram allocated." << std::endl;
	//     // delete glslprogram;
	// }
    }

    engine_kernel.deinit();
}
