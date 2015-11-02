#include "GLSceneManager.h"
#include "GLSLProgram.h"

#include <vector>
#include <iterator>
#include <string>

using namespace std;

GLSceneManager::GLSceneManager(){

}

GLSceneManager::~GLSceneManager(){

}

bool GLSceneManager::SetGLSLProgram( GLSLProgram * glsl_program ){
    _GLSLProgram = glsl_program;
    return true;
}

bool GLSceneManager::GetGLSLProgram( GLSLProgram * & glsl_program ) const {
    glsl_program = _GLSLProgram;
    return true;
}

bool GLSceneManager::RunInit(){
    for( auto i : _vec_init ){
	auto j = _map_routine.find( i );
	if( _map_routine.end() != j ){
	    bool bRet = j->second( _GLSLProgram );
	    if( !bRet ){
		return false;
	    }
	}else{
	    return false;
	}
    }
    return true;
}

bool GLSceneManager::RunCleanup(){
    for( auto i : _vec_cleanup ){
	auto j = _map_routine.find( i );
	if( _map_routine.end() != j ){
	    bool bRet = j->second( _GLSLProgram );
	    if( !bRet ){
		return false;
	    }
	}else{
	    return false;
	}
    }
    return true;
}

bool GLSceneManager::RunBody(){
    for( auto i : _vec_body ){
	auto j = _map_routine.find( i );
	if( _map_routine.end() != j ){
	    bool bRet = j->second( _GLSLProgram );
	    if( !bRet ){
		return false;
	    }
	}else{
	    return false;
	}
    }
    return true;
}

bool GLSceneManager::RunBodySpecified( string routine_name ){
    auto j = _map_routine.find( routine_name );
    if( _map_routine.end() != j ){
	bool bRet = j->second( _GLSLProgram );
	return bRet;
    }else{
	return false;
    }
    return true;
}

bool GLSceneManager::RegisterRoutine( string routine_name, std::function<bool(GLSLProgram *)> routine, GLSceneRoutineType::Enum routine_type ){
    switch( routine_type ){
    case GLSceneRoutineType::INIT:
	_vec_init.push_back( routine_name );
	break;
    case GLSceneRoutineType::BODY:
	_vec_body.push_back( routine_name );
	break;
    case GLSceneRoutineType::CLEANUP:
	_vec_cleanup.push_back( routine_name );
	break;
    default:
	return false;
    }
    _map_routine.insert(std::pair< string, std::function<bool(GLSLProgram *)> >( routine_name, routine ) );
    return true;
}

bool GLSceneManager::UnregisterRoutine( string const routine_name ){
    _map_routine.erase( routine_name );

    auto last = std::remove( _vec_init.begin(), _vec_init.end(), routine_name );
    auto count = std::distance( _vec_init.begin(), last );
    _vec_init.resize( count );

    last = std::remove( _vec_cleanup.begin(), _vec_cleanup.end(), routine_name );
    count = std::distance( _vec_cleanup.begin(), last );
    _vec_cleanup.resize( count );

    last = std::remove( _vec_body.begin(), _vec_body.end(), routine_name );
    count = std::distance( _vec_body.begin(), last );
    _vec_body.resize( count );
    
    return true;
}
