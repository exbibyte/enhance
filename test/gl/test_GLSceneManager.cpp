#include "GLSceneManager.h"
#include "GLSLProgram.h"

#include <functional>
#include <iostream>

using namespace std;

int main(){
    auto func_init_01 = []( GLSLProgram * glslprogram )->bool {
	cout << "func init 01" << endl;
	return true;
    };
    auto func_body_01 = []( GLSLProgram * glslprogram )->bool {
	cout << "func body 01" << endl;
	return true;
    };
    auto func_body_02 = []( GLSLProgram * glslprogram )->bool {
	cout << "func body 02" << endl;
	return true;
    };
    auto func_cleanup_01 = []( GLSLProgram * glslprogram )->bool {
	cout << "func cleanup 01" << endl;
	return true;
    };

    GLSLProgram * glsl_program;
    GLSceneManager scene_manager;
    scene_manager.SetGLSLProgram( glsl_program );

    std::function< bool( GLSLProgram * ) > func_wrap_init_01 = func_init_01;
    std::function< bool( GLSLProgram * ) > func_wrap_body_01 = func_body_01;
    std::function< bool( GLSLProgram * ) > func_wrap_body_02 = func_body_02;
    std::function< bool( GLSLProgram * ) > func_wrap_cleanup_01 = func_cleanup_01;
    scene_manager.RegisterRoutine( "init_01", func_wrap_init_01, GLSceneRoutineType::INIT );
    scene_manager.RegisterRoutine( "body_01", func_wrap_body_01, GLSceneRoutineType::BODY );
    scene_manager.RegisterRoutine( "body_02", func_wrap_body_02, GLSceneRoutineType::BODY );
    scene_manager.RegisterRoutine( "cleanup_01", func_wrap_cleanup_01, GLSceneRoutineType::CLEANUP );
    scene_manager.RunInit();
    scene_manager.RunBody();
    scene_manager.RunCleanup();
	
    return 0;
}
