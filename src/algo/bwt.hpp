#ifndef BWT_H
#define BWT_H

#include <string>

class bwt {
public:
    static std::string transform( std::string const & input );
    static std::string inverse( std::string const & input );
};

#endif
