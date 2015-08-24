#ifndef GLSCENEMANAGER_H
#define GLSCENEMANAGER_H

#include <vector>
#include <functional>
#include <map>
#include <string>

//types of routines that are executed by the scene manager
class GLSceneRoutineType {
public:
    enum Enum {
	INIT,
	BODY,
	CLEANUP,
	ENUM_SIZE,
    };
};

class GLSLProgram;

//scene manager to executing routine for rendering scene
class GLSceneManager {
public:
    GLSceneManager();
    ~GLSceneManager();
    bool SetGLSLProgram( GLSLProgram * glsl_program );
    bool GetGLSLProgram( GLSLProgram * & glsl_program ) const;
    bool RunInit();
    bool RunCleanup();
    bool RunBody();
    bool RegisterRoutine( std::string routine_name, std::function<bool(GLSLProgram *)> routine, GLSceneRoutineType::Enum routine_type );
    bool UnregisterRoutine( std::string const routine_name );
protected:
    std::vector< std::string > _vec_init;
    std::vector< std::string > _vec_cleanup;
    std::vector< std::string > _vec_body;
    std::map< std::string, std::function<bool(GLSLProgram *)> > _map_routine;
    GLSLProgram * _GLSLProgram;
};

#endif
