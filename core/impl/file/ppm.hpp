#ifndef E2_PPM_HPP
#define E2_PPM_HPP

#include <vector>

namespace e2 { namespace file {

class ppm {
public:	    
    static bool read( char const * filename, int & width, int & height, std::vector<unsigned char> * data );
    static bool write( char const * filename, unsigned char const * data, const int width, const int height );

};

} }

#endif

