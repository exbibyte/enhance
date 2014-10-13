#include<iostream>
#include<vector>
#include<string>
using namespace std;

GLSLProgram::GLSLProgram(){
    _HandleProgram = glCreateProgram();
    _Linked = false;
    _LogString = "";
    _vHandleShader.clear();
}

GLuint GLSLProgram::GetHandle() const{
    return _HandleProgram;
}

bool GLSLProgram::IsLinked() const{
    return _Linked;
}
void GLSLProgram::AttachShaders(){
    for( auto i : _vHandleShader ){
        glAttachShader( _HandleProgram, i );
    }
}
bool GLSLProgram::Link(){
    return GLLinkProgram( _HandleProgram );
}

bool GLSLProgram::CompileShaderFromFile( string FileName, GLSLShader::GLSLShaderType Type ){
    GLuint Shader;
    char const * FileNameConst = FileName.c_str();
    bool bRet = GLCompileShaderFromFile( Shader, FileNameConst, Type );
    if( bRet ){
        _vHandleShader.push_back( Shader );
    }
    return bRet;
}

bool GLSLProgram::CompileShaderFromString( string const & Source, GLSLShader::GLSLShaderType Type ){
    GLuint Shader;
    char const * SourceConst = Source.c_str();
    bool bRet = GLCompileShaderFromString( Shader, SourceConst, Type );
    if( bRet ){
        _vHandleShader.push_back( Shader );
    }
    return bRet;
}

void GLSLProgram::Use(){
    GLUseProgram( _HandleProgram );
}

string GLSLProgram::Log() const{

}

void GLSLProgram::BindAttribLocation( GLuint Loc, string Name ){
    glBindAttribLocation( _HandleProgram, Loc, (char const * ) Name.c_str() );
}
void GLSLProgram::BindFragDataLocation( GLuint Loc, string Name ){
    glBindFragDataLocation( _HandleProgram, Loc, (char const * ) Name.c_str() );
}
void GLSLProgram::SetUniform( char const * Name, float x, float y, float z ){
    glm::vec3 data( x, y, z);
    SetUniform( Name, (vec3 const) data );
}
void GLSLProgram::SetUniform( char const * Name, vec3 const & v ){
    GLuint location = glGetUniformLocation( _HandleProgram, Name );
    if( location >= 0 ){
        glUniform3fv(location, 1, &v[0] );
    }
}
void GLSLProgram::SetUniform( char const * Name, vec4 const & v ){
    GLuint location = glGetUniformLocation( _HandleProgram, Name );
    if( location >= 0 ){
        glUniform4fv(location, 1, &v[0] );
    }
}
void GLSLProgram::SetUniform( char const * Name, mat3 const & m ){
    GLuint location = glGetUniformLocation( _HandleProgram, Name );
    if( location >= 0 ){
        glUniformMatrix3fv(location, 1, false, &m[0][0] );
    }
}
void GLSLProgram::SetUniform( char const * Name, mat4 const & m ){
    GLuint location = glGetUniformLocation( _HandleProgram, Name );
    if( location >= 0 ){
        glUniformMatrix4fv(location, 1, false, &m[0][0] );
    }
}
void GLSLProgram::SetUniform( char const * Name, float val ){
    GLuint location = glGetUniformLocation( _HandleProgram, Name );
    if( location >= 0 ){
        glUniform1f(location, val );
    }
}
void GLSLProgram::SetUniform( char const * Name, int val ){
    GLuint location = glGetUniformLocation( _HandleProgram, Name );
    if( location >= 0 ){
        glUniform1i(location, val );
    }
}
void GLSLProgram::SetUniform( char const * Name, bool val ){
    int data = val ? 1 : 0;
    GLuint location = glGetUniformLocation( _HandleProgram, Name );
    if( location >= 0 ){
        glUniform1i(location, data );
    }
}
void GLSLProgram::PrintActiveUniforms() const{}
void GLSLProgram::PrintActiveAttribs() const{}
