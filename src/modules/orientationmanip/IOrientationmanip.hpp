#ifndef IORIENTATIONMANIP_H
#define IORIENTATIONMANIP_H

#include <list>
#include <initializer_list>

template< typename T_OUT, typename T_IN >
class IOrientationmanip {
public:
    using type_out = T_OUT;
    using type_in = T_IN;
    virtual ~IOrientationmanip(){}
    virtual bool init( std::initializer_list<int> const & args ) { return false; }
    virtual bool deinit() { return false; }
    virtual bool process( std::list<T_OUT> & out, std::list<T_IN> & in ){ return false; }
};

#endif
