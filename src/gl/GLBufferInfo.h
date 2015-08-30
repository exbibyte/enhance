#ifndef GLBUFFERINFO_H
#define GLBUFFERINFO_H

#include <string>

class GLBufferInfo {
public:
    GLBufferInfo(){
	_Next = nullptr;
	_Prev = nullptr;
    }
    std::string _Name;
    GLBufferInfo * _Next;
    GLBufferInfo * _Prev;
    unsigned int _Offset;
    unsigned int _Length;
    bool RemoveCurrent(){
	if( _Next ){
	    _Next->_Prev = nullptr;
	    _Next = nullptr;
	}
	if( _Prev ){
	    _Prev->_Next = nullptr;
	    _Prev = nullptr;
	}
	return true;
    }
    bool RemoveAllLinked(){
	GLBufferInfo * next = _Next;
	RemoveCurrent();
	if( next ){
	    next->RemoveAllLinked();
	}
        GLBufferInfo * prev = _Prev;
	if( prev ){
	    prev->RemoveAllLinked();
	}
	return true;
    }
};

#endif
