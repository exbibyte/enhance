#ifndef DUALSCALAR_H
#define DUALSCALAR_H
/// Dual number: d = a + e*b, ported from Antonio Sanchez's Java file
class DualScalar {

 public:
  float               _a;
  float               _b;

                      DualScalar();
                      DualScalar( float a, float b ); //construct dual number a + e*b
		      DualScalar( DualScalar & d ); //copy dual number
  void                SetDualNumber( float a, float b ); //a + e*b
  void                SetDualScalar( const DualScalar & d ); //copy dual number
  void                SetArray( float v [] ); //sets dual numbers in v[0] + e*v[1]
  void                GetArray( float v [] ) const; //fills dual numbers in v[0] + e*v[1]
  DualScalar &        operator = ( const DualScalar & d ); //copy dual number
  DualScalar          operator + ( const DualScalar & d ) const;
  DualScalar &        operator += ( const DualScalar & d );
  DualScalar          operator - ( const DualScalar & d ) const;
  DualScalar &        operator -= ( const DualScalar & d );
  DualScalar          operator * ( const DualScalar & d ) const;
  DualScalar &        operator *= ( const DualScalar & d );
  DualScalar          operator / ( const DualScalar & d ) const;
  DualScalar &        operator /= (const DualScalar & d );
  DualScalar          Conjugate() const; // (a + e*b)' = (a - e*b)
  float               Norm() const; // |a + e*b| = |a|
  DualScalar          Sqrt() const; // d^(1/2)
  DualScalar          Pow( DualScalar e ) const; // d^e
  DualScalar          Invert() const; // 1/d
  float               GetReal() const; //gets a
  float               GetDual() const; //gets b
};

#endif
