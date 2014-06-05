#include "Mat.h"

#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;

Mat::Mat(){
  // sets up default 4x4 dimension
  int dim[2] = { 4, 4 };
  ResizeInt( 2, dim );
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
    for( int i = 0; i < _size; i++ ){
      _mat[i] = 0;
    }
  }
}

void Mat::ResizeVec( const Vec & v ){
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
    cout<<"mismatch"<<endl;
    return a;
  }else{
    //new mat size after multiplication
    int dim[2];
    dim[0] = _dim[0];
    dim[1] = m._dim[1];
    a.ResizeInt( 2, dim );
   
    //row index
    for( int i = 0; i < (int)a._dim[0]; i++){
      //column index
      for( int j = 0; j < (int)a._dim[1]; j++){
	// for( int x = 0; x < (int)_dim[1]; x++){
	//   a( i, j ) += (*this)( i, x ) * m( x, j );
	// }
	for( int x = 0; x < (int)_dim[1]; x++){
	  a._mat[ j * (int)a._dim[0] + i ] += _mat[ i + x * (int)_dim[0] ] * m._mat[ x + j * (int)m._dim[0] ];
	}
      }
    }
    return a;
  }
}

Mat Mat::operator + ( const Mat & a ) const{
  Mat m;
  int dim[2];
  if( _dim[0] != a._dim[0] || _dim[1] != a._dim[1] ){
    m.ResizeInt( 0, dim );
    return m;
  }else{
    dim[0] = _dim[0];
    dim[1] = _dim[1];
    m.ResizeInt( 2, dim );
    for( int j = 0; j < _dim[1]; j++ ){
      for( int i = 0; i < _dim[0]; i++ ){
	m( i, j ) = (*this)( i, j ) + a( i, j );
      }
    }
    return m;
  }
}

Mat Mat::operator - ( const Mat & a ) const{
  Mat m;
  int dim[2];
  if( _dim[0] != a._dim[0] || _dim[1] != a._dim[1] ){
    m.ResizeInt( 0, dim );
    return m;
  }else{
    dim[0] = _dim[0];
    dim[1] = _dim[1];
    m.ResizeInt( 2, dim );
    for( int j = 0; j < _dim[1]; j++ ){
      for( int i = 0; i < _dim[0]; i++ ){
	m( i, j ) = (*this)( i, j ) - a( i, j );
      }
    }
    return m;
  }
}

float & Mat::operator ()( int m, int n ){
  if( m >= (int)_dim[0] || n >= (int)_dim[1] ){
    return _mat[0];
  }else{
    return _mat[ m + n * (int)_dim[0] ];
  }
}

float Mat::operator ()( int m, int n ) const{
  if( m >= (int)_dim[0] || n >= (int)_dim[1] ){
    return NAN;
  }else{
    return _mat[ m + n * (int)_dim[0] ];
  }
}

void Mat::SetFromVec( const Vec & v, bool column ){
  int dim[2];
  int len = v._dim;
  if( column == true ){
    dim[0] = len;
    dim[1] = 1;
    ResizeInt( 2, dim );
  }else{
    dim[0] = 1;
    dim[1] = len;
    ResizeInt( 2, dim );
  }
  for( int i = 0; i < len; i++ ){
    _mat[i] = v[i];
  }
}

bool Mat::GetVec( Vec & v, int index, bool column ) const{
  if( column == true ){
    if( index >= _dim[1] ){
      return false;
    }
    v.SetDim( _dim[0] );
    for( int i = 0; i < _dim[0]; i++ ){
      v[i] = (*this)( i, index );
    }
  }else{
    if( index >= _dim[0] ){
      return false;
    }
    v.SetDim( _dim[1] );
    for( int i = 0; i < _dim[1]; i++ ){
      v[i] = (*this)( index, i );
    }
  }
  return true;
}

bool Mat::GetSubMat( Mat & m, int row, int col, int sizerow, int sizecol ) const{
  if( row >= _dim[0] || col >= _dim[1] || row + sizerow - 1 >= _dim[0] || col + sizecol - 1 >= _dim[1] ){
    return false;
  }

  int dim[2];
  dim[0] = sizerow;
  dim[1] = sizecol;
  m.ResizeInt( 2, dim );

  for(int j = 0; j < sizecol; j++){
    for(int i = 0; i < sizerow; i++){
      m( i, j ) = (*this)( row + i, col + j );
    }
  }

  return true;
}

void Mat::TransposeCurrent(){
  (*this) = Transpose();
}

Mat Mat::Transpose() const{
  Mat m;
  int dim[2];
  dim[0] = _dim[1];
  dim[1] = _dim[0];
  m.ResizeInt( 2, dim );
  
  for( int j = 0; j < dim[1]; j++ ){
    for( int i = 0; i < dim[0]; i++ ){
      m( i, j ) = (*this)( j, i );
    }
  }
  return m;
}
