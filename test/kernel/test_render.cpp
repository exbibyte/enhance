#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>
#include <memory>
#include <assert.h>

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
#include "enComponentRendercompute.hpp"
#include "enComponentRenderserver.hpp"
#include "enComponentParser.hpp"
#include "enComponentUi.hpp"
#include "enComponentFilter.hpp"
#include "enComponentOrientationmanip.hpp"
#include "enComponentCam.hpp"

#include "Funwrap3.hpp"

#include "ParserMd5.hpp"
#include "file_md5_anim.hpp"
#include "file_md5_mesh.hpp"
#include "file_md5_skel.hpp"
#include "file_md5_calc_mesh.hpp"

#include "ParserObj.hpp"
#include "file_obj_mesh.hpp"

#include "renderable_info.hpp"

#include "GLIncludes.hpp"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

using namespace std;

int main( int argc, char ** argv ){

    string path_mesh;
    vector<string> paths_anim;
    if( argc >= 3 ){
	path_mesh = argv[1];
	for( int i = 2; i < argc; ++i ){
	    paths_anim.push_back( argv[i] );
	}
    }else{
	path_mesh = "../../testcase/fileformat/md5/qshambler.md5mesh";
	paths_anim.push_back( "../../testcase/fileformat/md5/qshamblerattack01.md5anim" );
    }
    // if( argc > 1 )
    //     path_poly = argv[1];
    // else
    // 	path_poly = "../../testcase/file/arrow.pmesh";

    string path_mesh_obj = "../../testcase/file/samplemodel.txt";
    
    enEngineKernel0 engine_kernel;
    assert( engine_kernel.get_num_components() == 0 );

    engine_kernel.init();
    assert( engine_kernel.get_num_components() > 0 );

    //clock0
    vector<enComponentMeta*> clocks;
    engine_kernel.get_components_by_type( enComponentType::CLOCK, clocks );
    assert( clocks.size() == 1 );
    COMPONENT_INSTANCE( clock0, enComponentClock0, clocks.front() );

    //initGL
    vector<enComponentMeta*> inits;
    engine_kernel.get_components_by_type( enComponentType::INIT, inits );
    assert( inits.size() == 1 );
    COMPONENT_INSTANCE( initGL, enComponentInitGL, inits.front() );
    bool bret = initGL->init( { 500, 500 } );
    assert( true == bret );

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //renderdraw0
    vector<enComponentMeta*> renderdraws;
    engine_kernel.get_components_by_type( enComponentType::RENDERDRAW, renderdraws );
    assert( renderdraws.size() == 1 );
    COMPONENT_INSTANCE( renderdraw0, enComponentRenderdraw0, renderdraws.front() );

    //renderserver0
    vector<enComponentMeta*> renderservers;
    engine_kernel.get_components_by_type( enComponentType::RENDERSERVER, renderservers );
    assert( renderservers.size() == 1 );
    COMPONENT_INSTANCE( renderserver0, enComponentRenderserver0, renderservers.front() );
    renderserver0->init();
    std::shared_ptr<GLSLProgram> glslprogram = ((Renderserver0*)renderserver0)->_glslprogram;
    assert( glslprogram.get() && "resource(glslprogram*) invalid" );

    //filterdrag0
    vector<enComponentMeta*> filters;
    engine_kernel.get_components_by_type( enComponentType::FILTER, filters );
    assert( filters.size() == 1 );
    COMPONENT_INSTANCE( filterdrag0, enComponentFilterUiDrag, filters.front() );

    //Ui0
    vector<enComponentMeta*> uis;
    engine_kernel.get_components_by_type( enComponentType::UI, uis );
    assert( uis.size() == 1 );
    COMPONENT_INSTANCE( ui0, enComponentUi0, uis.front() );
	
    //orientationmanip0
    vector<enComponentMeta*> orientationmanips;
    engine_kernel.get_components_by_type( enComponentType::ORIENTATIONMANIP, orientationmanips );
    assert( orientationmanips.size() == 1 );
    COMPONENT_INSTANCE( orientationmanip0, enComponentOrientationmanip0, orientationmanips.front() );

    //camera manipulation
    vector<enComponentMeta*> cams;
    engine_kernel.get_components_by_type( enComponentType::CAM, cams );
    assert( cams.size() == 1 );
    COMPONENT_INSTANCE( cam, enComponentCam0, cams.front() );
    cam0 * camera_manip = static_cast<cam0*>( cam );
    //set trackball window size
    orientationmanip0->init( { 500, 500, 250, 250 } );

    camera_manip->process( ICam::e_pparam::register_ui, (void*) ui0 );
    camera_manip->process( ICam::e_pparam::register_orientationmanip, (void*) orientationmanip0 );
    camera_manip->process( ICam::e_pparam::register_filter, (void*) filterdrag0 );
    camera_manip->process( ICam::e_pparam::enable, (void*) 0 );
    
    //parsers
    vector<enComponentMeta*> parsers;
    engine_kernel.get_components_by_type( enComponentType::PARSER, parsers );
    assert( parsers.size() >= 1 );
    ParserMd5 * parsermd5 = nullptr;
    for( auto & i : parsers ){
	COMPONENT_INSTANCE( parser_model, enComponentParserMd5, i );
	if( nullptr != parser_model ){
	    parsermd5 = (ParserMd5 *)parser_model;
	    break;
	}
    }
    assert( nullptr != parsermd5 );
    if( nullptr == parsermd5 )
	return -1;

    ParserObj * parserobj = nullptr;
    for( auto & i : parsers ){
	COMPONENT_INSTANCE( parser_model, enComponentParserObj, i );
	if( nullptr != parser_model ){
	    parserobj = (ParserObj *)parser_model;
	    break;
	}
    }
    assert( nullptr != parserobj );
    if( nullptr == parserobj )
	return -1;

    //parse files and obtain asset information
    map< string, GLBufferInfo * > map_buffer_info; //unused for now
    map< string, GLBufferInfoSequence * > map_buffer_info_sequence; //unused for now

    std::pair<bool, ParserMd5::md5_data > retparse = parsermd5->parse( path_mesh, paths_anim );
    assert( retparse.first );
    if( !retparse.first )
	return 0;
    auto model_mesh = std::move(retparse.second._dm);
    auto model_skels = std::move(retparse.second._scs);
    assert( !model_skels.empty() );
    std::pair< ParserMd5::md5_anim_info, file_md5_skel::skel_collection > & sc = model_skels.back();
    assert( !sc.second._skels.empty() );
    //select the first frame of the current skeleton for rendering
    size_t count_tris = 0;
    for( auto & i : model_mesh._meshes ){
	count_tris += i._tris.size();
    }
    std::cout << "number of model tris: " << count_tris << std::endl;


    //obj model
    std::pair< bool, file_obj_mesh::data_mesh > retparse_obj = parserobj->parse( path_mesh_obj );
    assert( retparse_obj.first );
    if( !retparse_obj.first )
	return 0;
    auto dm_obj = std::move( retparse_obj.second );
    std::cout << "number of obj model tris: " << dm_obj._numtris << std::endl;
    
    cout << "End of Init Phase" << endl;

    //imgui init
    bool bInstallCallback = false;
    WindowInfo windowinfo = ((InitGL*)initGL)->GetWindowResource();
    glfwMakeContextCurrent( windowinfo._window ); // this is need when calling rendering APIs on separate thread
    ImGui_ImplGlfwGL3_Init( windowinfo._window, bInstallCallback );

    //register window resource for UI event handler
    ui0->init();
    ui0->register_resource_to_monitor( windowinfo._window );

    //setup clock tick callback
    bool ticked = false;
    std::function<void(std::uint64_t,std::uint64_t,std::uint64_t)> tick_cb = [&](auto a, auto b, auto c){
	ticked = true;
    };
    clock0->reset_start_time();
    clock0->set_frequency(25);
    clock0->set_func_cb( tick_cb );
    clock0->run();

    camera c;
    light l;
    l._light_position = { 50.0, 50.0, 50.0 };
    Vec orient_axis;
    float init_orient[] = { 0,0,1 };
    orient_axis.SetFromArray( 3, init_orient );
    float orient_angle;
    orient_axis[0] = 1;
    orient_angle = 1.000;

    double auto_rotate = 0.07;
    
    bool bQuit = false;
    while(true){
	if( bQuit ) { break; }

	std::uint64_t time_since_start_ms;
	std::uint64_t ticks_since_start;
	clock0->get_time( time_since_start_ms, ticks_since_start );

	vector<enComponentMeta*> rendercomputes;
	engine_kernel.get_components_by_type( enComponentType::RENDERCOMPUTE, rendercomputes );
	assert( rendercomputes.size() == 1 );
	COMPONENT_INSTANCE( rendercompute0, enComponentRendercompute0, rendercomputes.front() );

	// delegate ui to camera
	// //process UI events
	// std::list<IUi::character> characters{};
	// ui0->get_characters( characters );
	// for( auto & i : characters ){
	//     if( IUi::input_type::KEY == i._input_type ){
	// 	if( IUi::state::DOWN == i._state ){
	// 	    if( 'Q' == i._key_character ){
	// 		std::cout << "exiting.." << std::endl;
	// 		bQuit = true;
	// 	    }
	// 	}
	//     }
	// }

	// //drag filter
	// std::list<drag_coordinate> drag{};
        // filterdrag0->process( drag, characters );
	// for( auto & i : drag ){
	//     if( IUi::mouse_character::LEFT == i._mouse_character ){
	// 	std::cout << "drag: left, ";
	//     }else if( IUi::mouse_character::RIGHT == i._mouse_character ){
	// 	std::cout << "drag: right, ";
	//     }else if( IUi::mouse_character::MID == i._mouse_character ){
	// 	std::cout << "drag: mid, ";
	//     }else {
	// 	continue;
	// 	// std::cout << "drag: other, ";
	//     }
	//     std::cout << "x: " << i._coordinate_delta._a << ", y: " << i._coordinate_delta._b << std::endl;
	// }
	    
	// //orientation manipulation
	// std::list<Quat> orientation;
        // orientationmanip0->process( orientation, drag );
	// for( auto & i : orientation ){
	//     Vec axis{};
	//     float angle;
	//     i.ToAxisAngle( axis, angle );
	//     std::cout << "axis: ";
	//     std::cout << axis._vec[0] << ", ";
	//     std::cout << axis._vec[1] << ", ";
	//     std::cout << axis._vec[2];
	//     std::cout << ", angle: " << angle << std::endl;

	//     Vec orient_axis_temp;
	//     i.ToAxisAngle( orient_axis_temp, orient_angle );
	//     if( orient_angle == 0 ){
	// 	continue;
	//     }else{
	// 	orient_axis = orient_axis_temp;
	// 	orient_angle = orient_angle / 3.14 * 180;
	// 	break;
	//     }
	// }

	ICam::operation_mode movement_filter = ICam::operation_mode::latest_discard_all_after;
	camera_manip->process( ICam::e_pparam::operation, (void*) &movement_filter );
	Vec orient_axis_temp;
	camera_manip->_orientation.ToAxisAngle( orient_axis_temp, orient_angle );
	if( orient_angle != 0 ){
	    orient_axis = orient_axis_temp;
	    orient_angle = orient_angle / 3.14 * 180;
	}
	bQuit = camera_manip->_exit;
	
	// orient_angle += auto_rotate;

	//update animation starts
	//select the first frame of the current skeleton for rendering
	double animation_time = fmod( time_since_start_ms/1000.0, (double)sc.first._numframes / sc.first._framerate );
	std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> retcalc = file_md5_calc_mesh::process( model_mesh, sc.second, animation_time );
	assert( retcalc.first );
	file_md5_calc_mesh_frame::data_mesh_frame dmf = std::move( retcalc.second );
	assert( dmf._mesh_frames.size() == model_mesh._meshes.size() );
	auto it_dmesh_mesh = model_mesh._meshes.begin();
	// renderable_info_tris rit = std::move( file_md5_calc_mesh::calc_renderable_info_tris( dmf ) );
	renderable_info_tris rit = file_obj_mesh::calc_renderable_info_tris( dm_obj );	
	
	assert( renderable_check_consistency<renderable_info_tris>::process( rit ) );
	//update animation ends
	
	//compute render information
	IRendercompute::RenderDataPack render_data_0;
	render_data_0.vert_coord = std::move( rit._pos );
	render_data_0.vert_normal = std::move( rit._normal );
	render_data_0.orient_axis = orient_axis;
	render_data_0.orient_angle = orient_angle;
	Vec translate(3);
	translate[0] = 0.0f;
	translate[1] = 0.0f;
	translate[2] = 0.0f;
	render_data_0.translate = translate;
	c._camera_lookat.resize(3);
	for( int i = 0; i < 3; ++i ){
	    // c._camera_lookat[i] = (dmf._bbox_lower[i] + dmf._bbox_upper[i])/2.0;
	    // c._camera_position[i] = c._camera_lookat[i] + (dmf._bbox_upper[i] - dmf._bbox_lower[i]);
	    c._camera_lookat[i] = 0;
	    // c._camera_position[i] = 5;
	    c._camera_position[i] = camera_manip->_pos[i] + 5;
	}

	material m;
	render_data_0._material = m;
	try{
	RenderData renderdata = rendercompute0->compute( l, c, { render_data_0 } );
	renderdata._glslprogram = glslprogram.get();

	glfwMakeContextCurrent( windowinfo._window ); // this is need when calling rendering APIs on separate thread

	//render call ----------------------------------------------------------------
	renderdraw0->render( renderdata );

	}catch(...){
	    assert( 0 && "caught" );

	}
	ImVec4 clear_color = ImColor(60, 60, 90);

	//Imgui renderig starts
	ImGui_ImplGlfwGL3_NewFrame();
	{
	    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	    ImGui::SetNextWindowSize(ImVec2(200,80), ImGuiSetCond_FirstUseEver);
	    ImGui::Begin("Status UI");
	    ImGui::ColorEdit3("clear color", (float*)&clear_color);
	    ImGui::Text("Rendering: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	    ImGui::End();

	    int display_w, display_h;
	    glfwGetFramebufferSize( windowinfo._window, &display_w, &display_h);
	    glViewport(0, 0, display_w, display_h);
	    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	}
	ImGui::Render();

	//wait for clock tick
	while( !ticked ){
	    clock0->tick();
	    std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}
	ticked = false;
 
	if( glfwWindowShouldClose( windowinfo._window ) )
	{
	    break;
	}

	glfwSwapBuffers( windowinfo._window );

	// std::cout << "post renderdraw0." << std::endl;
		
	glfwPollEvents();
    }

    clock0->pause();
    
    engine_kernel.deinit();

    exit(EXIT_SUCCESS);

    return 0;
}
