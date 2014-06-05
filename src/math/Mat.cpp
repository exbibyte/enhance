#include "Mat.h"

#include <math.h>
#include <string.h>

Mat::Mat(){
  // sets up default 4x4 dimension
  _dim.SetDim(2);
  _dim[0] = 4;
  _dim[1] = 4;
  _size = 16;
  _mat = new float [ _size ];
}

void Mat::ResizeInt( int count, int dim [] ){
  if( count == 0 ){
    _size = 0;
    delete [] _mat;
    _mat = new float [ 0 ];
    return;
  }else{
    //set mat dimension
    _dim.SetDim(count);
    _size = 1;
    for( int i = 0; i < count; i++ ){
      _size *= dim[i];
      _dim[i] = dim[i];
    }
    _mat = new float [ _size ];
  }
}

void Mat::ResizeVec( Vec v ){
  float * dim;
  int count;
  v.GetArray( count, dim );
  int * dim_int = new int[count];
  for(int i = 0; i < count; i++ ){
    dim_int[i] = floor(dim[i] + 0.5);
  }

  ResizeInt( count, dim_int );
}

Mat & Mat::operator = ( const Mat & m ){
  _size = m._size;
  _dim = m._dim;
  _mat = new float [ _size ];
  memcpy( _mat, m._mat, sizeof(float)*_size );
}

Mat Mat::operator * ( const Mat & m ) const{

  Mat a;
  
  if( (int)_dim[1] != (int)m._dim[0] ){
    //size mismatch
    return a;
  }else{
    //new mat size after multiplication
    a._dim.SetDim(2);
    a._dim[0] = _dim[0];
    a._dim[1] = m._dim[1];
    
    a._size = (int)a._dim[0] * (int)a._dim[1];
    a._mat = new float [ a._size ];

    //row index
    for( int i = 0; i < (int)a._dim[0]; i++){
      //column index
      for( int j = 0; j < (int)a._dim[1]; j++){
	a._mat[ j * (int)a._dim[0] + i ] = 0;
	for( int x = 0; x < (int)_dim[1]; x++){
	  a._mat[ j * (int)a._dim[0] + i ] += _mat[ i + x * (int)_dim[0] ] * m._mat[ x + j * (int)m._dim[0] ];
	}
      }
    }
    return a;
  }
}

float & Mat::operator ()( int m, int n ){
  if( m >= (int)_dim[0] || n >= (int)_dim[1] ){
    return _mat[0];
  }else{
    return *( _mat + m + n * (int)_dim[0]  );
  }
}

float Mat::operator ()( int m, int n ) const{
  if( m >= (int)_dim[0] || n >= (int)_dim[1] ){
    return NAN;
  }else{
    return *( _mat + m + n * (int)_dim[0] );
  }
}
