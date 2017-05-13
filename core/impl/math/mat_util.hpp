#ifndef E2_MATRIXMATH_HPP
#define E2_MATRIXMATH_HPP

namespace e2 { namespace math {

#define pi 3.14159265359
	
///methods for matrix arithmetic in column major format
class mat_util{
public:
  static bool invert_matrix(const float m[16], float invout[16]);   /// provides matrix inversion, from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
  static bool invert_matrix3x3(const float m[9], float invout[9]);
  static bool mat4x4_mult_4x1(float fourbyone[4], float fourbyfour[16], float out[4]);   /// provides 4x4 * 4x1 matrix operation 
  static bool mat1x4_mult_4x4(float fourbyone[4], float fourbyfour[16], float out[4]);   /// provides 1x4 * 4x4 matrix operation
  static bool mat4x4_mult_4x4(float left[16], float right[16], float out[16]);   /// provides 4x4 * 4x4 matrix operation 
  static bool mat4x4_transpose(float const in[16], float out[16]);   /// transposes 4x4 matrix
  static bool mat3x3_transpose(float const in[9], float out[9]);
  static bool mat4x4_normalize(float in[16], float out[16]);  /// normalizes transformation matrix
  static bool mat4x1_normalize(float in[4], float out[4]);
  static bool print_mat4x4(float in[16]);
  static bool print_mat4x1(float in[4]);
  static bool get_mat4x4_identity(float out[16]);
  static bool get_mat4x4_rotation(float in[16], float r[16]);
  static bool normalize_scaling_mat4x4(float in[16], float out[16]);
  static bool invert_translate_mat4x4(float in[16], float out[16]);
  static bool invert_translatez_mat4x4(float in[16], float out[16]);
  static bool invert_rotate_mat4x4(float in[16], float out[16]);
  static bool trace_4x4( float in[16], float out[16] );
  static bool perspective( float fovy, float aspect, float near, float far, float out[16] );
  static bool lookat( float eye[3], float center[3], float up[3], float out[16] );
};

} }

#endif
