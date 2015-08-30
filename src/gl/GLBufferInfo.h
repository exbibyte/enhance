#ifndef GLBUFFERINFO_H
#define GLBUFFERINFO_H

#include <string>
#include <vector>

class GLBufferInfo {
public:
    std::string _Name;
    unsigned int _Offset;
    unsigned int _Length;
};

class GLBufferInfoSequence {
public:
    std::string _Name;
    std::vector< GLBufferInfo * > _vec_BufferInfo;
    int _CurrentIndex;
};

#endif
