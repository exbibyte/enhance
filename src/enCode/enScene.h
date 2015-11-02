#ifndef EN_SCENE_H
#define EN_SCENE_H

#include <functional>
#include <string>

class enScene {
public:
    bool ExecuteCallable( std::string strName, GLSLProgram * _GLSLProgram ){
	auto it = _callables.find( strName );
	if( _callables.end() == it ){
	    return false;
	}
	bool bRet = it->second( _GLSLProgram );
	return bRet;
    }
    bool RegisterCallable( std::string strName, std::function<bool( GLSLProgram * )> callable ){
	_callables[ strName ] = callable;
	return true;
    }
    map< std::string, std::function<bool( GLSLProgram * )> > _callables; //stores all callable procedures that are registered
    float _dAngle = 0;
    float _fLightPos_x = 5;
    float _fLightPos_y = 5;
    string _strPathPolyMesh;
};

#endif
