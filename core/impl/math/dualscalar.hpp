#ifndef E2_DUALSCALAR_HPP
#define E2_DUALSCALAR_HPP
/// Dual number: d = a + e*b, ported from Antonio Sanchez's Java file

namespace e2 { namespace math {

class dualscalar {

 public:
  float               _a;
  float               _b;

                      dualscalar();
                      dualscalar( float a, float b ); //construct dual number a + e*b
		      dualscalar( dualscalar & d ); //copy dual number
  void                set_dual_number( float a, float b ); //a + e*b
  void                set_dual_scalar( const dualscalar & d ); //copy dual number
  void                set_array( float v [] ); //sets dual numbers in v[0] + e*v[1]
  void                get_array( float v [] ) const; //fills dual numbers in v[0] + e*v[1]
  dualscalar &        operator = ( const dualscalar & d ); //copy dual number
  dualscalar          operator + ( const dualscalar & d ) const;
  dualscalar &        operator += ( const dualscalar & d );
  dualscalar          operator - ( const dualscalar & d ) const;
  dualscalar &        operator -= ( const dualscalar & d );
  dualscalar          operator * ( const dualscalar & d ) const;
  dualscalar &        operator *= ( const dualscalar & d );
  dualscalar          operator / ( const dualscalar & d ) const;
  dualscalar &        operator /= (const dualscalar & d );
  dualscalar          conjugate() const; // (a + e*b)' = (a - e*b)
  float               norm() const; // |a + e*b| = |a|
  dualscalar          sqrt() const; // d^(1/2)
  dualscalar          pow( dualscalar e ) const; // d^e
  dualscalar          invert() const; // 1/d
  float               get_real() const; //gets a
  float               get_dual() const; //gets b
};

} }

#endif
