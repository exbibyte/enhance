#ifndef MAT_H
#define MAT_H

#include "Vec.h"

///column major matrix
class Mat{
 public:
  float *       _mat; // mat data
  Vec           _dim; //mat dimension
  int           _size; //overall mat size

                Mat(); //default 4x4 matrix
  void     	ResizeInt( int count, int dim [] );
  void		ResizeVec( const Vec & v );
  Mat &         operator = ( const Mat & m ); //copy
  Mat           operator * ( const Mat & m ) const; //matrix multiplication, up to 2D mat for now
  Mat           operator + ( const Mat & m ) const; //element add
  Mat           operator - ( const Mat & m ) const; //element subtract
  float &       operator ()( int m, int n ); //accessor
  float         operator ()( int m, int n ) const; //accessor
  void          SetFromVec( const Vec & v, bool column = true); //convert from a Vec, default is a column vector
  bool          GetVec( Vec & v, int index, bool column = true ) const; //get a particular column or row of the current Mat
  bool          GetSubMat( Mat & m, int row, int col, int sizerow, int sizecol ) const; //get a sub Mat
  void          TransposeCurrent();
  Mat           Transpose() const;
};

#endif
