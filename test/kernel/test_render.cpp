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

int main(){

    enEngineKernel0 engine_kernel;
    assert( engine_kernel.get_num_components() == 0 );

    engine_kernel.init();
    assert( engine_kernel.get_num_components() > 0 );

    //initGL
    vector<enComponentMeta*> inits;
    engine_kernel.get_components_by_type( enComponentType::INIT, inits );
    assert( inits.size() == 1 );
    COMPONENT_INSTANCE( initGL, enComponentInitGL, inits.front() );
    bool bret = initGL->init();
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
    ((ParserPolymesh0*)parserpolymesh0)->parse( map_buffer_info, map_buffer_info_sequence, vert_pos, vert_norm );
    
    cout << "End of Init Phase" << endl;

    double rotation_angle = 0;
    auto t1 = std::chrono::high_resolution_clock::now();

    //imgui init
    bool bInstallCallback = false;
    WindowInfo windowinfo = ((InitGL*)initGL)->GetWindowResource();
    glfwMakeContextCurrent( windowinfo._window ); // this is need when calling rendering APIs on separate thread
    ImGui_ImplGlfwGL3_Init( windowinfo._window, bInstallCallback );

    auto t_prev_frame = std::chrono::high_resolution_clock::now();

    bool bQuit = false;
    while(true){
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration_ms = t2 - t1;
	if( duration_ms.count() > 5000 ){
	    cout << "End rendering..." << endl;
	    bQuit = true;
	    break;
	}

	vector<enComponentMeta*> rendercomputes;
	engine_kernel.get_components_by_type( enComponentType::RENDERCOMPUTE, rendercomputes );
	assert( rendercomputes.size() == 1 );
	COMPONENT_INSTANCE( rendercompute0, enComponentRendercompute0, rendercomputes.front() );

	//compute render information
	RenderData renderdata = rendercompute0->compute( vert_pos, vert_norm );
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

	std::cout << "post renderdraw0." << std::endl;

	auto t_current_frame= std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> duration_frame = t_current_frame - t_prev_frame;
	while( duration_frame.count() < 40 ){
	    t_current_frame= std::chrono::high_resolution_clock::now();
	    duration_frame = t_current_frame - t_prev_frame;
	}
	t_prev_frame = t_current_frame;

	if( glfwWindowShouldClose( windowinfo._window ) )
	{
	    break;
	}
	glfwPollEvents();
    }
    
    engine_kernel.deinit();

    exit(EXIT_SUCCESS);

    return 0;
}
