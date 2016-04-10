#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

#include "MemoryEditor.h"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main( int argc, char ** argv ){

    std::string strFileSavePath;
    if( argc > 1 ){
	strFileSavePath = argv[1];
    }
    
    //initilize sample memory
    unsigned char mem_sample[256];
    memset( mem_sample, 255, 256 );
    for( unsigned int i = 0; i < 10; ++i ){
	mem_sample[i] = i;
    }
    MemoryEditor mem_editor;

    std::cout << "Memory Address: " << std::hex << (size_t)&mem_sample[0] << std::endl;

    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(700, 700, "ImGui OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

	ImGui::SetNextWindowSize(ImVec2(500,500), ImGuiSetCond_FirstUseEver);
	mem_editor.Draw( "MEM EDITOR", mem_sample, 256, (size_t) mem_sample );
	
        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    if( argc > 1 ){
	std::ofstream out( strFileSavePath );
	std::cout.rdbuf( out.rdbuf() );
	for( unsigned int i = 0; i< 256; ++i ){
	    std::cout << mem_sample[i];
	}
	std::cout << std::endl;
    }
    
    return 0;
}
