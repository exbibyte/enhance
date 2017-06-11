#ifndef E2_I_MATH_TRANSFORM_HPP
#define E2_I_MATH_TRANSFORM_HPP

#include <cstring>

namespace e2 { namespace interface {

template< class T >
class i_math_transform {
public:
    static bool dim( int * d, T * ){ return false; }
    static bool magnitude( double * m, T * ){ return false; }
    static bool normalize( T * ){ return false; }
    static bool assign( T *, T * ){ return false; }
    static bool sub( T *, T *, T * ){ return false; }
    static bool add( T *, T *, T * ){ return false; }
    static bool mult( T *, T *, T * ){ return false; }
    static bool div( T *, T *, T * ){ return false; }
    static bool is_equal( bool * b, T *, T * ){ return false; }
    template< class R >
    static bool index( R *, T *, int ){ return false; }
    template< class R >
    static bool index( R *, T *, int, int ){ return false; }
    template< class R >
    static bool index( R *, T *, int, int, int ){ return false; }
    static bool dot( double *, T *, T * ){ return false; }
    static bool cross( T *, T *, T * ){ return false; }
    static bool from_array( T *, int dim, float * arr ){ return false; }
    static bool from_array( T *, int dim, double * arr ){ return false; }
    static bool to_array( float * arr, int dim, T * ){ return false; }
    static bool to_array( double * arr, int dim, T * ){ return false; }
    static bool scale_add( T *, T *, double s, double add ){ return false; }
};

} }

#endif
