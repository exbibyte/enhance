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

#include "Funwrap3.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>
#include <memory>
#include <assert.h>

#include "GLIncludes.hpp"

#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformDriver.h"
#include "PolyMesh_Data_Arrays.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

using namespace std;

int main( int argc, char ** argv ){

    string path_poly;
    if( argc > 1 )
        path_poly = argv[1];
    else
	path_poly = "../../testcase/file/arrow.pmesh";
    
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

    //set trackball window size
    orientationmanip0->init( { 500, 500, 250, 250 } );

    //parserpolymesh0
    vector<enComponentMeta*> parserpolymeshes;
    engine_kernel.get_components_by_type( enComponentType::PARSER, parserpolymeshes );
    assert( parserpolymeshes.size() == 1 );
    COMPONENT_INSTANCE( parserpolymesh0, enComponentParserPolymesh0, parserpolymeshes.front() );

    //parse polymesh files and obtain asset information
    map< string, GLBufferInfo * > map_buffer_info; //unused for now
    map< string, GLBufferInfoSequence * > map_buffer_info_sequence; //unused for now
    vector<double> vert_pos;
    vector<double> vert_norm;
    ((ParserPolymesh0*)parserpolymesh0)->parse( path_poly, map_buffer_info, map_buffer_info_sequence, vert_pos, vert_norm );

    vector<double> vert_pos_2 = vert_pos; //duplicate position data and offset it
    for( auto & i : vert_pos_2 ){
	i += 5;
    }
    
    cout << "End of Init Phase" << endl;

    //imgui init
    bool bInstallCallback = false;
    WindowInfo windowinfo = ((InitGL*)initGL)->GetWindowResource();
    glfwMakeContextCurrent( windowinfo._window ); // this is need when calling rendering APIs on separate thread
    ImGui_ImplGlfwGL3_Init( windowinfo._window, bInstallCallback );

    //register window resource for UI event handler
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

    Vec orient_axis;
    float orient_angle;
    orient_axis[0] = 1;
    orient_angle = 0.000;

    double auto_rotate = 0.4;
    
    bool bQuit = false;
    while(true){
	std::uint64_t time_since_start_ms;
	std::uint64_t ticks_since_start;
	clock0->get_time( time_since_start_ms, ticks_since_start );
	// if( time_since_start_ms > 5000 ){
	//     cout << "End rendering..." << endl;
	//     bQuit = true;
	//     break;
	// }

	vector<enComponentMeta*> rendercomputes;
	engine_kernel.get_components_by_type( enComponentType::RENDERCOMPUTE, rendercomputes );
	assert( rendercomputes.size() == 1 );
	COMPONENT_INSTANCE( rendercompute0, enComponentRendercompute0, rendercomputes.front() );

	//process UI events
	std::list<IUi::character> characters{};
	ui0->get_characters( characters );
	for( auto & i : characters ){
	    if( IUi::input_type::MOUSE == i._input_type ){
		if( IUi::mouse_character::LEFT == i._mouse_character ){
		    std::cout << "mouse L ";
		}
		else if( IUi::mouse_character::RIGHT == i._mouse_character ){
		    std::cout << "mouse R ";
		}
		else if( IUi::mouse_character::MID == i._mouse_character ){
		    std::cout << "mouse M ";
		}

		if( IUi::state::DOWN == i._state ){
		    std::cout << "down" << std::endl;
		}
		else if( IUi::state::UP == i._state ){
		    std::cout << "up" << std::endl;
		}
	    }
	    else if( IUi::input_type::KEY == i._input_type ){
		std::cout << "key " << i._key_character << " ";

		if( IUi::state::DOWN == i._state ){
		    std::cout << "down" << std::endl;
		    if( 'Q' == i._key_character ){
			std::cout << "exit" << std::endl;
			return 0;
		    }
		}
		else if( IUi::state::UP == i._state ){
		    std::cout << "up" << std::endl;
		}
		else if( IUi::state::REPEAT == i._state ){
		    std::cout << "repeat" << std::endl;
		}
	    }else{ //MOUSE_COORD
		// std::cout << "x: " << i._coordinate._a << ", y: " << i._coordinate._b << std::endl;
	    }
	}

	//drag filter
	std::list<drag_coordinate> drag{};
        filterdrag0->process( drag, characters );
	for( auto & i : drag ){
	    if( IUi::mouse_character::LEFT == i._mouse_character ){
		std::cout << "drag: left, ";
	    }else if( IUi::mouse_character::RIGHT == i._mouse_character ){
		std::cout << "drag: right, ";
	    }else if( IUi::mouse_character::MID == i._mouse_character ){
		std::cout << "drag: mid, ";
	    }else {
		continue;
		// std::cout << "drag: other, ";
	    }
	    std::cout << "x: " << i._coordinate_delta._a << ", y: " << i._coordinate_delta._b << std::endl;
	}
	
	//orientation manipulation
	std::list<Quat> orientation;
        orientationmanip0->process( orientation, drag );
	for( auto & i : orientation ){
	    Vec axis{};
	    float angle;
	    i.ToAxisAngle( axis, angle );
	    std::cout << "axis: ";
	    std::cout << axis._vec[0] << ", ";
	    std::cout << axis._vec[1] << ", ";
	    std::cout << axis._vec[2];
	    std::cout << ", angle: " << angle << std::endl;

	    Vec orient_axis_temp;
	    i.ToAxisAngle( orient_axis_temp, orient_angle );
	    if( orient_angle == 0 ){
		continue;
	    }else{
		orient_axis = orient_axis_temp;
		orient_angle = orient_angle / 3.14 * 180;
		break;
	    }
	}

	orient_angle += auto_rotate;
	
	//compute render information
	IRendercompute::RenderDataPack render_data_0;
	render_data_0.vert_coord = vert_pos;
	render_data_0.vert_normal = vert_norm;
	render_data_0.orient_axis = orient_axis;
	render_data_0.orient_angle = orient_angle;

	IRendercompute::RenderDataPack render_data_1;
	render_data_1.vert_coord = vert_pos_2;
	render_data_1.vert_normal = vert_norm;
	render_data_1.orient_axis = orient_axis;
	render_data_1.orient_angle = orient_angle;

	RenderData renderdata = rendercompute0->compute( { render_data_0, render_data_1 } );
	renderdata._glslprogram = glslprogram.get();
	
	glfwMakeContextCurrent( windowinfo._window ); // this is need when calling rendering APIs on separate thread

	//render call ----------------------------------------------------------------
	renderdraw0->render( renderdata );

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

	glfwSwapBuffers( windowinfo._window );

	// std::cout << "post renderdraw0." << std::endl;

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
	glfwPollEvents();
    }

    clock0->pause();
    
    engine_kernel.deinit();

    exit(EXIT_SUCCESS);

    return 0;
}
