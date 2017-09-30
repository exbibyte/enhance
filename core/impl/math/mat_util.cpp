#include <iostream>
#include <cmath>

#include "mat_util.hpp"
#include "vec.hpp"
#include "mat.hpp"

namespace e2 { namespace math {

bool mat_util::invert_matrix( float const m[16], float invout[16])
{
    double inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    float invouttemp[16];
    //get the row major matrix result
    for (i = 0; i < 16; i++)
        invouttemp[i] = inv[i] * det;

    mat_util::mat4x4_transpose(invouttemp, invout);

    return true;
}

bool mat_util::invert_matrix3x3( float const m[9], float invout[9]){
    float determinant = m[0]*(m[4]*m[8]-m[5]*m[7]) - m[1]*(m[3]*m[8]-m[5]*m[6]) + m[2]*(m[3]*m[7]-m[4]*m[6]);
    if( ( determinant < 0.000001 ) && ( determinant > -0.000001 ) ){
	return false;
    }
    float t[9];
    mat3x3_transpose( m, t );
    float m00 = t[4]*t[8]-t[5]*t[7];
    float m10 = -(t[3]*t[8]-t[5]*t[6]);
    float m20 = t[3]*t[7]-t[4]*t[6];
    float m01 = t[1]*t[8]-t[2]*t[7];
    float m11 = -(t[0]*t[8]-t[2]*t[6]);
    float m21 = t[0]*t[7]-t[1]*t[6];
    float m02 = t[1]*t[5]-t[2]*t[4];
    float m12 = -(t[0]*t[5]-t[2]*t[3]);
    float m22 = t[0]*t[4]-t[1]*t[3];
    invout[0] = m00;
    invout[1] = m10;
    invout[2] = m20;
    invout[3] = m01;
    invout[4] = m11;
    invout[5] = m21;
    invout[6] = m02;
    invout[7] = m12;
    invout[8] = m22;
    for( int i = 0; i < 9; ++i ){
	invout[i] /= determinant;
    }
    return true;
}

bool mat_util::mat4x4_mult_4x1(float fourbyone[], float fourbyfour[], float out[])
{
  // for each column
  for(int i = 0; i < 4; i++)
  {
    float sum = 0;
    // for each row
    for(int j = 0; j < 4; j++)
     {
      sum += (fourbyone[j] * fourbyfour[i + j*4]);
    }    
    out[i] = sum;
  }
  return true;
}

bool mat_util::mat1x4_mult_4x4(float onebyfour[], float fourbyfour[], float out[])
{
  // for each row
  for(int i = 0; i < 4; i++)
  {
    float sum = 0;
    // for each column
    for(int j = 0; j < 4; j++)
     {
      sum += (onebyfour[j] * fourbyfour[i*4 + j]);
    }    
    out[i] = sum;
  }
  return true;
}

bool mat_util::mat4x4_mult_4x4(float left[], float right[], float out[])
{
  // for each column in right
  for(int i = 0; i < 4; i++)
  {
    // for each row in left
    for(int j = 0; j < 4; j++)
    {
      float sum = 0;
      // compute dot product of row and column
      for(int k = 0; k < 4; k++)
      {
	sum += left[j + k*4] * right[i*4 + k];
      }
      out[i + j*4] = sum;
    }    
  }
  return true;
}

bool mat_util::mat4x4_transpose(float const in[], float out[])
{
  //for each output column
  for(int i = 0; i < 4; i++)
  {
    //for each output row
    for(int j = 0; j < 4; j++)
    {
      out[i*4+j] = in[j*4+i]; 
    }
  }
  return true;
}

bool mat_util::mat3x3_transpose(float const in[], float out[])
{
  //for each output column
  for(int i = 0; i < 3; i++)
  {
    //for each output row
    for(int j = 0; j < 3; j++)
    {
      out[i*3+j] = in[j*3+i]; 
    }
  }
  return true;
}

bool mat_util::mat4x4_normalize(float in[], float out[])
{
  float factor = in[15];

  for(int i = 0; i < 16; i++)
  {
    out[i] = in[i]/factor;
  }
  return true;
}

bool mat_util::mat4x1_normalize(float in[], float out[])
{
  float factor = in[3];

  for(int i = 0; i < 4; i++)
  {
    out[i] = in[i]/factor;
  }
  return true;
}

bool mat_util::print_mat4x4(float in[])
{
  //for each row
  for(int i = 0; i < 4; i++)
  {
    //for each column
    for(int j = 0; j < 4; j++)
    {
      std::cout<<in[j + i*4]<<" ";
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  return true;
}

bool mat_util::print_mat4x1(float in[])
{
  //for each row
  for(int i = 0; i < 4; i++)
  {
    std::cout<<in[i]<<", ";
  }
  std::cout<<std::endl;

  return true;
}

bool mat_util::get_mat4x4_identity(float out[])
{
  for(int i = 0; i <16; i++)
    out[i] = 0;

  out[0] = 1;
  out[5] = 1;
  out[10] = 1;
  out[15] = 1;

  return true;
}

bool mat_util::get_mat4x4_rotation(float in[], float r[])
{
  //normalize scaling
  float sx = sqrt(pow(in[0],2) + pow(in[4],2) + pow(in[8],2));
  float sy = sqrt(pow(in[1],2) + pow(in[5],2) + pow(in[9],2));
  float sz = sqrt(pow(in[2],2) + pow(in[6],2) + pow(in[10],2));

  float mat3x3[3][3];

  //each row
  for(int i = 0; i < 3; i++)
  {
    //each column
    for(int j = 0; j < 3; j++)
    {
      if(j == 0)
	mat3x3[i][j] = in[i+j*4]/sx;
      else if(j == 1)
	mat3x3[i][j] = in[i+j*4]/sy;
      else if(j == 2)
	mat3x3[i][j] = in[i+j*4]/sz;
    }
  }

  //rx
  r[0] = atan2(mat3x3[3][2], mat3x3[3][3])*180/pi;
  //ry
  r[1] = atan2(-mat3x3[3][1], sqrt(pow(mat3x3[3][2],2) + pow(mat3x3[3][3],2)))*180/pi;
  //rz
  r[2] = atan2(mat3x3[2][1], mat3x3[1][1])*180/pi;

  return true;
}

bool mat_util::normalize_scaling_mat4x4(float in[], float out[])
{
  //normalize scaling
  float sx = sqrt(pow(in[0],2) + pow(in[4],2) + pow(in[8],2));
  float sy = sqrt(pow(in[1],2) + pow(in[5],2) + pow(in[9],2));
  float sz = sqrt(pow(in[2],2) + pow(in[6],2) + pow(in[10],2));

  for(int i = 0; i < 16; i++)
    out[i] = in[i];

  //each row
  for(int i = 0; i < 3; i++)
  {
    //each column
    for(int j = 0; j < 3; j++)
    {
      if(j == 0)
	out[i+j*4] = in[i+j*4]/sx;
      else if(j == 1)
        out[i+j*4] = in[i+j*4]/sy;
      else if(j == 2)
        out[i+j*4] = in[i+j*4]/sz;
    }  
  }
  return true;
}

bool mat_util::invert_translate_mat4x4(float in[], float out[])
{
  for(int i = 0; i < 12; i++)
  {
    out[i] = in[i];
  }
  for(int i = 12; i < 15; i++)
  {
    out[i] = -1*in[i];
  }
  out[15] = in[15];

  return true;
}

bool mat_util::invert_translatez_mat4x4(float in[], float out[])
{
  for(int i = 0; i < 14; i++)
  {
    out[i] = in[i];
  }
  out[14] = -1*in[14];
  out[15] = in[15];

  return true;
}

bool mat_util::invert_rotate_mat4x4(float in[], float out[])
{
  for(int i = 0; i < 16; i++)
    out[i] = in[i];

  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      out[j+i*4] = in[i+j*4];
    }
  }
  return true;
}

bool mat_util::trace_4x4( float in[], float * out ){
    *out = in[0] + in[5] + in[10] + in[15];
    return true;
}
bool mat_util::perspective( float fov, float aspect, float near, float far, float out[] ){
    //fov is the full field of view in y-plane, eg: fovy = fov/2
    float half_tan = tan( fov*pi/360.0 );
    for( int i = 0; i < 16; ++i ){
	out[i] = 0;
    }
    out[0] = 1.0/(aspect*half_tan);
    out[5] = 1.0/half_tan;
    out[10] = (-far+near)/(far-near);
    out[11] = -1;
    out[14] = (-2.0*far*near)/(far-near);

    return true;
}
bool mat_util::lookat( float eye[], float center[], float up[], float out[] ){
    //compute viewing plane's normal vector
    vec n(3);
    for( int i = 0; i < 3; ++i )
	n[i] = eye[i] - center[i];
    n.normalize_current();

    //compute 1 orthogonal vector to plane's normal vector
    vec up_vec(3);
    up_vec.set_from_array(3, up );
    vec u = up_vec.cross( n );
    u.normalize_current();
    
    //compute the other orthogonal vector to plane's normal vector
    vec v = n.cross( u );
    v.normalize_current();

    //space_original = a_inverse * space_camera
    //a is a rotation matrix, which means a_inverse = a_transpose
    //a is finally augmented with translations on the last column
    mat camera_view;
    camera_view( 0, 0 ) = u[0];
    camera_view( 0, 1 ) = u[1];
    camera_view( 0, 2 ) = u[2];
    camera_view( 0, 3 ) = -1*(eye[0]*u[0] + eye[1]*u[1] + eye[2]*u[2]);
    camera_view( 1, 0 ) = v[0];
    camera_view( 1, 1 ) = v[1];
    camera_view( 1, 2 ) = v[2];
    camera_view( 1, 3 ) = -1*(eye[0]*v[0] + eye[1]*v[1] + eye[2]*v[2]);
    camera_view( 2, 0 ) = n[0];
    camera_view( 2, 1 ) = n[1];
    camera_view( 2, 2 ) = n[2];
    camera_view( 2, 3 ) = -1*(eye[0]*n[0] + eye[1]*n[1] + eye[2]*n[2]);
    camera_view( 3, 0 ) = 0;
    camera_view( 3, 1 ) = 0;
    camera_view( 3, 2 ) = 0;
    camera_view( 3, 3 ) = 1;

    for( int i = 0; i < 16; ++i )
	out[i] = camera_view._mat[i];

    return true;
}

} }
