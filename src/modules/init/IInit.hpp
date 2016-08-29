#ifndef IINIT_H
#define IINIT_H

#include <string>
#include <initializer_list>

class IInit {
public:
    virtual ~IInit(){}
    virtual bool init( std::initializer_list<unsigned> const & window_args ){ return false; }
};

#endif
