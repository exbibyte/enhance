#include "MatrixMath.h"

#include <iostream>
#include <cmath>

using namespace std;

bool MatrixMath::InvertMatrix(const float m[16], float invOut[16])
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

    float invOutTemp[16];
    //get the row major matrix result
    for (i = 0; i < 16; i++)
        invOutTemp[i] = inv[i] * det;

    MatrixMath::Mat4x4Transpose(invOutTemp, invOut);

    return true;
}

void MatrixMath::Mat4x4Mult4x1(float FourByOne[], float FourbyFour[], float out[])
{
  // for each column
  for(int i = 0; i < 4; i++)
  {
    float sum = 0;
    // for each row
    for(int j = 0; j < 4; j++)
     {
      sum += (FourByOne[j] * FourbyFour[i + j*4]);
    }    
    out[i] = sum;
  }
}

void MatrixMath::Mat1x4Mult4x4(float Onebyfour[], float FourbyFour[], float out[])
{
  // for each row
  for(int i = 0; i < 4; i++)
  {
    float sum = 0;
    // for each column
    for(int j = 0; j < 4; j++)
     {
      sum += (Onebyfour[j] * FourbyFour[i*4 + j]);
    }    
    out[i] = sum;
  }
}

void MatrixMath::Mat4x4Mult4x4(float Left[], float Right[], float out[])
{
  // for each column in Right
  for(int i = 0; i < 4; i++)
  {
    // for each row in Left
    for(int j = 0; j < 4; j++)
    {
      float sum = 0;
      // compute dot product of row and column
      for(int k = 0; k < 4; k++)
      {
	sum += Left[j + k*4] * Right[i*4 + k];
      }
      out[i + j*4] = sum;
    }    
  }
}

void MatrixMath::Mat4x4Transpose(float in[], float out[])
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
}

void MatrixMath::Mat4x4Normalize(float in[], float out[])
{
  float factor = in[15];

  for(int i = 0; i < 16; i++)
  {
    out[i] = in[i]/factor;
  }
}

void MatrixMath::Mat4x1Normalize(float in[], float out[])
{
  float factor = in[3];

  for(int i = 0; i < 4; i++)
  {
    out[i] = in[i]/factor;
  }
}

void MatrixMath::PrintMat4x4(float in[])
{
  //for each row
  for(int i = 0; i < 4; i++)
  {
    //for each column
    for(int j = 0; j < 4; j++)
    {
      cout<<in[j + i*4]<<" ";
    }
    cout<<endl;
  }
  cout<<endl;
}

void MatrixMath::PrintMat4x1(float in[])
{
  //for each row
  for(int i = 0; i < 4; i++)
  {
    cout<<in[i]<<", ";
  }
  cout<<endl;
}

void MatrixMath::GetMat4x4Identity(float out[])
{
  for(int i = 0; i <16; i++)
    out[i] = 0;

  out[0] = 1;
  out[5] = 1;
  out[10] = 1;
  out[15] = 1;
}

void MatrixMath::GetMat4x4Rotation(float in[], float r[])
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
  r[0] = atan2(mat3x3[3][2], mat3x3[3][3])*180/PI;
  //ry
  r[1] = atan2(-mat3x3[3][1], sqrt(pow(mat3x3[3][2],2) + pow(mat3x3[3][3],2)))*180/PI;
  //rz
  r[2] = atan2(mat3x3[2][1], mat3x3[1][1])*180/PI;

}

void MatrixMath::NormalizeScalingMat4x4(float in[], float out[])
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
}

void MatrixMath::InvertTranslateMat4x4(float in[], float out[])
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
}

void MatrixMath::InvertTranslateZMat4x4(float in[], float out[])
{
  for(int i = 0; i < 14; i++)
  {
    out[i] = in[i];
  }
  out[14] = -1*in[14];
  out[15] = in[15];
}

void MatrixMath::InvertRotateMat4x4(float in[], float out[])
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
}
