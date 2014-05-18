
// MD5 Loader, by A.J. Tavakoli

#ifdef _WIN32
#include <windows.h> // yuck
#endif

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cstddef>
#include <cctype>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h> 

#include "MD5Model.h"
#include "Lex.h"
#include "Quat.h"
#include "PPM.hpp"

#define IS_WHITESPACE(c) (' ' == c || '\t' == c || '\r' ==c || '\n' == c )


MD5Model::MD5Model():
     numJoints(0),
     numMeshes(0),
     currAnim(-1),
     currFrame(0),
     animTime(0.0f) {

}

MD5Model::~MD5Model() {
  clear();
}


void MD5Model::clear() {
  // delete meshes
  for ( size_t i=0; i < meshes.size(); i++ )
    if ( meshes[i] )
      delete meshes[i];

  // delete animations
  for ( size_t i=0; i < anims.size(); i++ )
    if ( anims[i] )
      delete anims[i];

  meshes.clear();
  anims.clear();
  joints.clear();
}


void MD5Model::animate(float dt) {
  // sanity check #1
  if ( currAnim < 0 || currAnim >= int(anims.size()) || !anims[currAnim] )
    throw Exception("MD5Model::animate(): currAnim is invalid");

  Anim *anim = anims[currAnim];

  // sanity check #2
  if ( currFrame < 0 || currFrame >= int(anim->numFrames) )
    throw Exception("MD5Model::animate(): currFrame is invalid");

  // compute index of next frame
  int nextFrameIndex = currFrame >= anim->numFrames - 1 ? 0 : currFrame + 1;

  // update animation time
  animTime += dt*float(anim->frameRate);
  if ( animTime > 1.0f ) {
    while ( animTime > 1.0f )
      animTime -= 1.0f;
    //setFrame(nextFrameIndex);
    currFrame = nextFrameIndex;
    nextFrameIndex = currFrame >= anim->numFrames - 1 ? 0 : currFrame + 1;
  }

  // make sure size of storage for interpolated frame is correct
  if ( int(interpFrame.joints.size()) != numJoints )
    interpFrame.joints.resize(numJoints);

  ///// now interpolate between the two frames /////

  Frame &frame     = anim->frames[currFrame],
        &nextFrame = anim->frames[nextFrameIndex];

  // interpolate between the joints of the current frame and those of the next
  // frame and store them in interpFrame
  for ( int i=0; i < numJoints; i++ ) {
    Joint &interpJoint = interpFrame.joints[i];

    // linearly interpolate between joint positions
    float *pos1 = frame.joints[i].pos,
          *pos2 = nextFrame.joints[i].pos;
    interpJoint.pos[0] = pos1[0] + animTime*(pos2[0] - pos1[0]);
    interpJoint.pos[1] = pos1[1] + animTime*(pos2[1] - pos1[1]);
    interpJoint.pos[2] = pos1[2] + animTime*(pos2[2] - pos1[2]);

    interpJoint.quat = Slerp(frame.joints[i].quat, nextFrame.joints[i].quat, animTime);
  }

  buildVerts(interpFrame);
  buildNormals();
}


// sets current animation
void MD5Model::setAnim(int animIndex, int frameIndex) {
  if ( animIndex < 0 || animIndex >= int(anims.size()) )
    throw Exception("MD5Model::setAnim(): invalid animation index");

  if ( currAnim != animIndex ) {
    currAnim = animIndex;
    setFrame(frameIndex);
  }
}


void MD5Model::setFrame(int frameIndex) {
  // sanity check #1
  if ( anims.size() == 0 || currAnim < 0 )
    throw Exception("MD5Model::setFrame(): no animation has beens set");

  // sanity check #2
  if ( frameIndex < 0 || !anims[currAnim] ||
       anims[currAnim]->numFrames <= 0 ||
       anims[currAnim]->numFrames <= frameIndex )
    throw Exception("MD5Model::setFrame(): frame index is invalid");

  buildVerts(anims[currAnim]->frames[frameIndex]);
  buildNormals();
  currFrame = frameIndex;
  animTime = 0.0f;
}


void MD5Model::render() {
  glPushAttrib(GL_CURRENT_BIT);
  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  glEnable( GL_TEXTURE_2D );

  for( size_t i=0; i < meshes.size(); i++ ) {
    const Mesh *mesh = meshes[i];

    glBindTexture( GL_TEXTURE_2D, mesh->textureID);

    glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(Vertex)), mesh->verts[0].pos);
    glNormalPointer(GL_FLOAT, GLsizei(sizeof(Vertex)), mesh->verts[0].n);
    glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(Vertex)), mesh->verts[0].tc);

    glDrawElements(GL_TRIANGLES, GLsizei(mesh->tris.size()*3), GL_UNSIGNED_INT, &mesh->tris[0]);
  }

  glPopClientAttrib();
  glPopAttrib();
}


void MD5Model::loadMesh(const char *filename) {
  // sanity check
  if ( !filename )
    throw Exception("MD5Model::loadMesh(): filename is NULL");

  // attempt to open file for reading
  std::ifstream fin(filename, std::ifstream::in);

  // was open successful?
  if ( !fin.is_open() ) {
    std::string msg = std::string("MD5Model::loadMesh(): unable to open ") +
                      std::string(filename) + std::string(" for reading");
    throw Exception(msg);
  }

  // read in file version
  std::string str;
  getNextToken(fin, &str);

  // token must read "MD5Version"
  if ( str != "MD5Version" )
    throw Exception("MD5Model::loadMesh(): expected 'MD5Version'");

  // get version #
  int ver = readInt(fin);

  // must be version 10
  if ( ver != 10 )
    throw Exception("MD5Model::loadMesh(): MD5Version must be 10");

  // clear any data before reading file
  clear();

  // read in all of the MD5Model elements
  readElements(fin);

  //load textures
  if(this->ignoreTexture == false) {
    for(int i = 0; i < this->meshes.size(); i++) {
      this->meshes[i]->LoadTexture();
    }
  }

  // close input file (should be done destructor anyway...)
  fin.close();

  // calculate vertex positions and normals from information in joints
  buildVerts();
  buildNormals();
}


int MD5Model::loadAnim(const char *filename) {
  // attempt to open file for reading
  std::ifstream fin(filename, std::ifstream::in);

  if ( !fin.is_open() ) {
    std::string msg = std::string("MD5Model::loadAnim(): unable to open ") +
                      std::string(filename) + std::string(" for reading");
    throw Exception(msg);
  }

  // read in file version
  std::string str;
  getNextToken(fin, &str);

  // token must read "MD5Version"
  if ( str != "MD5Version" )
    throw Exception("MD5Model::loadAnim(): expected 'MD5Version'");
  
  // get version #
  int ver = readInt(fin);

  // must be version 10
  if ( ver != 10 )
    throw Exception("MD5Model::loadAnim(): MD5Version must be 10");

  Anim *anim = new Anim;
  if ( !anim )
    throw Exception("MD5Model::loadAnim(): could not allocate storage for animation");

  readAnimElements(fin, *anim);

  // close file (should be done by destructor anyway)
  fin.close();

  // add to array of animations
  int animIndex = (int)anims.size();
  anims.push_back(anim);

  // build frames for this animation
  buildFrames(*anim);

  // make this the current animation
  setAnim(animIndex, 0);

  return animIndex;
}


void MD5Model::readElements(std::ifstream &fin) {
  while ( !fin.eof() ) {
    std::string str;
    TOKEN tok = getNextToken(fin, &str);
  
    // token is TOKEN_INVALID when end of file is reached
    if ( TOKEN_INVALID == tok )
      break;
  
    if ( str == "commandline" )
      readCommandLineEl(fin);
    else if ( str == "numJoints" )
      readNumJointsEl(fin);
    else if ( str == "numMeshes" )
      readNumMeshesEl(fin);
    else if ( str == "joints" )
      readJointsEl(fin);
    else if ( str == "mesh" )
      readMeshEl(fin);
    else {
      // invalid element
      throw Exception( std::string("MD5Model::readElements(): unknown element type '") + str + "'");
    }
  } // while ( not EOF )
}


void MD5Model::readAnimElements(std::ifstream &fin, Anim &anim) {
  while ( !fin.eof() ) {
    std::string str;
    TOKEN tok = getNextToken(fin, &str);

    // token is TOKEN_INVALID when end of file is reached
    if ( TOKEN_INVALID == tok )
      break;

    if ( str == "commandline" )
      readCommandLineEl(fin);
    else if ( str == "numJoints" ) {
      // just make sure that the number of joints is the same as the number
      // specified in the mesh file
      int n = readInt(fin);

      if ( n != numJoints )
        throw Exception("MD5Model::readAnimElements(): anim file does not match mesh");
    }
    else if ( str == "numMeshes" ) {
      // just make sure that the number of meshes is the same as the number
      // specified in the mesh file
      int n = readInt(fin);

      if ( n != numMeshes )
        throw Exception("MD5Model::readAnimElements(): anim file does not match mesh");
    }
    else if ( str == "numFrames" )
      readNumFramesEl(fin, anim);
    else if ( str == "frameRate" )
      readFrameRateEl(fin, anim);
    else if ( str == "numAnimatedComponents" )
      readNumAnimatedComponentsEl(fin, anim);
    else if ( str == "hierarchy" )
      readHierarchyEl(fin, anim);
    else if ( str == "bounds" )
      readBoundsEl(fin, anim);
    else if ( str == "baseframe" )
      readBaseframeEl(fin, anim);
    else if ( str == "frame" )
      readFrameEl(fin, anim);
    else {
      // invalid element
      throw Exception( std::string("MD5Model::readAnimElements(): unknown element type '") + str + "'");
    }
  }
}


void MD5Model::readCommandLineEl(std::ifstream &fin) {
  // commandline elements can be ignored, but make sure the syntax is correct
  if ( getNextToken(fin) != TOKEN_STRING )
    throw Exception("MD5Model::readCommandLineEl(): expected string");
}


void MD5Model::readNumJointsEl(std::ifstream &fin) {
  // if number of joints has already been set, can't set it again
  if ( numJoints != 0 )
    throw Exception("MD5Model::readNumJointsEl(): numJoints has already been set");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("MD5Model::readNumJointsEl(): numJoints must be greater than 0");

  numJoints =  n;
  //joints.resize(numJoints);
}


void MD5Model::readNumMeshesEl(std::ifstream &fin) {
  // if number of meshes has already been set, can't set it again
  if ( numMeshes != 0 )
    throw Exception("MD5Model::readNumMeshesEl(): numMeshes has already been set");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("MD5Model::readNumMeshesEl(): numMeshes must be greater than 0");

  numMeshes =  n;
  //meshes.resize(numMeshes);
}


void MD5Model::readNumFramesEl(std::ifstream &fin, Anim &anim) {
  // if number of frames has already been set, can't set it again
  if ( anim.numFrames != 0 )
    throw Exception("MD5Model::readNumFramesEl(): numFrames has already been set");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("MD5Model::readNumFramesEl(): numFrames must be greater than 0");

  anim.numFrames =  n;
  anim.frames.resize(n);
}


void MD5Model::readFrameRateEl(std::ifstream &fin, Anim &anim) {
  // if framerate has already been set, can't set it again
  if ( anim.frameRate != 0 )
    throw Exception("MD5Model::readFrameRateEl(): frameRate has already been set");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("MD5Model::readFrameRateEl(): frameRate must be a positive integer");

  anim.frameRate =  n;
}


void MD5Model::readNumAnimatedComponentsEl(std::ifstream &fin, Anim &anim) {
  // make sure parameter hasn't been set, as has been done with all of the
  // others
  if ( anim.numAnimatedComponents != 0 )
    throw Exception("MD5Model::readNumAnimatedComponentsEl(): numAnimatedComponents has already been set");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("MD5Model::readNumAnimatedComponentsEl(): numAnimatedComponents must be a positive integer");

  anim.numAnimatedComponents = n;
}


void MD5Model::readJointsEl(std::ifstream &fin) {
  // make sure numJoints has been set
  if ( numJoints <= 0 )
    throw Exception("MD5Model::readJointsEl(): numJoints needs to be set before 'joints' block");

  TOKEN t = getNextToken(fin);

  // expect an opening brace { to begin block of joints
  if ( t != TOKEN_LBRACE )
    throw Exception("MD5Model::readJointsEl(): expected { to follow 'joints'");

  // read in each joint in block until '}' is hit
  // (if EOF is reached before this, then the read*() methods
  //  will close the file and throw an exception)
  std::string str;
  t = getNextToken(fin, &str);
  while ( t != TOKEN_RBRACE ) {
    Joint joint;

    // token should be name of joint (a string)
    if ( t != TOKEN_STRING )
      throw Exception("MD5Model::readJointsEl(): expected joint name (string)'");

    // read in index of parent joint
    int parentIndex = readInt(fin);
  
    // read in joint position
    readVec(fin, joint.pos, 3);
  
    // read in first 3 components of quaternion (must compute 4th)
    float quat[4];
    readVec(fin, quat, 3);

    joint.quat = buildQuat(quat[0], quat[1], quat[2]);

    // add index to parent's list of child joints (if it has a parent,
    // root joints have parent index -1)
    if ( parentIndex >= 0 ) {
      if ( size_t(parentIndex) >= joints.size() )
        throw Exception("MD5Model::readJointsEl(): parent index is invalid");

      joints[parentIndex].children.push_back( int(joints.size()) );
    }

    // add joint to vector of joints
    joints.push_back(joint);

    // get next token
    t = getNextToken(fin, &str);
  }
}


void MD5Model::readMeshEl(std::ifstream &fin) {
  // make sure numMeshes has been set
  if ( numMeshes <= 0 )
    throw Exception("MD5Model::readMeshesEl(): numMeshes needs to be set before 'mesh' block");

  TOKEN t = getNextToken(fin);

  // expect an opening brace { to begin block of joints
  if ( t != TOKEN_LBRACE )
    throw Exception("MD5Model::readMeshEl(): expected { to follow 'mesh'");

  Mesh *mesh = new Mesh;

  // read in all mesh information in block until '}' is hit
  // (if EOF is reached before this, then the read*() methods
  //  will close the file and throw an exception)
  std::string str;
  t = getNextToken(fin, &str);
  while ( t != TOKEN_RBRACE ) {
    if ( str == "vert" ) {
      // syntax: vert (vertex index) '(' (x) (y) (z) ')' (weight index) (weight count)
      Vertex vert;

      int index = readInt(fin);
      readVec(fin, vert.tc, 2);
      vert.weightIndex = readInt(fin);
      vert.weightCount = readInt(fin);

      // make sure index is within bounds of vector of vertices
      if ( size_t(index) >= mesh->verts.size() )
        throw Exception("MD5Model::readMeshEl(): vertex index >= numverts");

      mesh->verts[index] = vert;
    }
    else if ( str == "tri" ) {
      // syntax: tri (triangle index) (v0 index) (v1 index) (v2 index)
      Tri tri;
      
      int index = readInt(fin);

      // make sure index is within bounds of vector of triangles
      if ( size_t(index) >= mesh->tris.size() )
        throw Exception("MD5Model::readMeshEl(): triangle index >= numtris");

      tri.v[0] = readInt(fin);
      tri.v[1] = readInt(fin);
      tri.v[2] = readInt(fin);
      mesh->tris[index] = tri;
    }
    else if ( str == "weight" ) {
      Weight weight;

      int weightIndex = readInt(fin);
      weight.joint = readInt(fin);
      weight.w = readFloat(fin);
      readVec(fin, weight.pos, 3);

      if ( size_t(weightIndex) >= mesh->weights.size() )
        throw Exception("MD5Model::readMeshEl(): weight index >= numweights");

      mesh->weights[weightIndex] = weight;
    }
    else if ( str == "shader" ) {
      std::string shader;
      if ( getNextToken(fin, &shader) != TOKEN_STRING )
        throw Exception("MD5Model::readMeshEl(): expected string to follow 'shader'");
      
      mesh->texture = shader;

    }
    else if ( str == "numverts" ) {
      if ( mesh->verts.size() > 0 )
        throw Exception("MD5Model::readMeshEl(): numverts has already been set");

      int n = readInt(fin);

      if ( n <= 0 )
        throw Exception("MD5Model::readMeshEl(): numverts must be greater than 0");

      mesh->verts.resize(n);
    }
    else if ( str == "numtris" ) {
      if ( mesh->tris.size() > 0 )
        throw Exception("MD5Model::readMeshEl(): numtris has already been set");

      int n = readInt(fin);

      if ( n <= 0 )
        throw Exception("MD5Model::readMeshEl(): numtris must be greater than 0");

      mesh->tris.resize(n);
    }
    else if ( str == "numweights" ) {
      if ( mesh->weights.size() > 0 )
        throw Exception("MD5Model::readMeshEl(): numweights has already been set");

      int n = readInt(fin);

      if ( n <= 0 )
        throw Exception("MD5Model::readMeshEl(): numweights must be greater than 0");

      mesh->weights.resize(n);
    }

    // read next token
    t = getNextToken(fin, &str);
  }

  meshes.push_back(mesh);
}


// reads in hierarchy block from anim file
void MD5Model::readHierarchyEl(std::ifstream &fin, Anim &anim) {
  TOKEN t = getNextToken(fin);

  // expect an opening brace { to begin hierarchy block
  if ( t != TOKEN_LBRACE )
    throw Exception("MD5Model::readHierarchyEl(): expected { to follow 'hierarchy'");

  anim.jointInfo.clear();

  // read in each joint in block until '}' is hit
  // (if EOF is reached before this, then the read*() methods
  //  will close the file and throw an exception)
  std::string str;
  t = getNextToken(fin, &str);
  while ( t != TOKEN_RBRACE ) {
    JointInfo info;

    // token should be name of a joint (a string)
    if ( t != TOKEN_STRING )
      throw Exception("MD5Model::readHierarchyEl(): expected name (string)");

    info.name        = str;
    info.parentIndex = readInt(fin);
    info.flags       = readInt(fin);
    info.startIndex  = readInt(fin);

    anim.jointInfo.push_back(info);
    
    // get next token
    t = getNextToken(fin, &str);
  }

  if ( int(anim.jointInfo.size()) != numJoints )
    throw Exception("MD5Model::readHierarchyEl(): number of entires in hierarchy block does not match numJoints");
}


// reads in bounds block from anim file
void MD5Model::readBoundsEl(std::ifstream &fin, Anim &anim) {
  TOKEN t = getNextToken(fin);

  // expect an opening brace { to begin block
  if ( t != TOKEN_LBRACE )
    throw Exception("MD5Model::readBoundsEl(): expected { to follow 'bounds'");

  if ( anim.numFrames != int(anim.frames.size()) )
    throw Exception("MD5Model::readBoundsEl(): frames must be allocated before setting bounds");

  // read in bound for each frame
  for ( int i=0; i < anim.numFrames; i++ ) {
    readVec(fin, anim.frames[i].min, 3);
    readVec(fin, anim.frames[i].max, 3);
  }

  // next token must be a closing brace }
  t = getNextToken(fin);

  if ( TOKEN_LPAREN == t )
    throw Exception("MD5Model::readBoundsEl(): number of bounds exceeds number of frames");

  // expect a closing brace } to end block
  if ( t != TOKEN_RBRACE )
    throw Exception("MD5Model::readBoundsEl(): expected }");
}


void MD5Model::readBaseframeEl(std::ifstream &fin, Anim &anim) {
  TOKEN t = getNextToken(fin);

  // expect an opening brace { to begin block
  if ( t != TOKEN_LBRACE )
    throw Exception("MD5Model::readBaseframeEl(): expected { to follow 'baseframe'");

  anim.baseJoints.resize(numJoints);

  int i;
  for ( i=0; i < numJoints; i++ ) {
    // read in joint position
    readVec(fin, anim.baseJoints[i].pos, 3);

    // read in first 3 components of quaternion (must compute 4th)
    float quat[3];
    readVec(fin, quat, 3);

    anim.baseJoints[i].quat = buildQuat(quat[0], quat[1], quat[2]);
  }

  if ( i < numJoints - 1 )
    throw Exception("MD5Model::readBaseframeEl(): not enough joints");

  // next token must be a closing brace }
  t = getNextToken(fin);

  if ( TOKEN_LPAREN == t )
    throw Exception("MD5Model::readBaseframeEl(): too many joints");

  // expect a closing brace } to end block
  if ( t != TOKEN_RBRACE )
    throw Exception("MD5Model::readBaseframeEl(): expected }");
}


void MD5Model::readFrameEl(std::ifstream &fin, Anim &anim) {
  // numAnimatedComponents has to have been set before frame element
  if ( 0 == anim.numAnimatedComponents )
    throw Exception("MD5Model::readFrameEl(): numAnimatedComponents must be set before 'frame' block");

  // read frame index
  int frameIndex = readInt(fin);

  if ( frameIndex < 0 || frameIndex >= anim.numFrames )
    throw Exception("MD5Model::readFrameEl(): invalid frame index");

  // get reference to frame and set number of animated components
  Frame &frame = anim.frames[frameIndex];
  frame.animatedComponents.resize(anim.numAnimatedComponents);

  TOKEN t = getNextToken(fin);

  // expect an opening brace { to begin block
  if ( t != TOKEN_LBRACE )
    throw Exception("MD5Model::readFrameEl(): expected { to follow frame index");

  for ( int i=0; i < anim.numAnimatedComponents; i++ )
    frame.animatedComponents[i] = readFloat(fin);

  t = getNextToken(fin);

  // expect a closing brace } to end block
  if ( t != TOKEN_RBRACE )
    throw Exception("MD5Model::readFrameEl(): expected }");
}

// builds mesh when no animation has been set
void MD5Model::buildVerts() {
  for ( size_t i=0; i < meshes.size(); i++ ) {
    for ( size_t j=0; j < meshes[i]->verts.size(); j++ ) {
      Vertex &v = meshes[i]->verts[j];
      v.pos[0] = v.pos[1] = v.pos[2] = 0.0;
      
      for ( int k=0; k < v.weightCount; k++ ) {
        Weight &w = meshes[i]->weights[v.weightIndex + k];
        Joint &joint = joints[w.joint];
        
        Quat q(w.pos[0], w.pos[1], w.pos[2], 0.0f);
        Quat result = joint.quat*q*joint.quat.Conjugate();
        v.pos[0] += (joint.pos[0] + result[0])*w.w;
        v.pos[1] += (joint.pos[1] + result[1])*w.w;
        v.pos[2] += (joint.pos[2] + result[2])*w.w;
      } // for (weights)
    } // for (mesh vertices)
  } // for (meshes)
}


void MD5Model::buildVerts(Frame &frame) {
  for ( size_t i=0; i < meshes.size(); i++ ) {
    for ( size_t j=0; j < meshes[i]->verts.size(); j++ ) {
      Vertex &v = meshes[i]->verts[j];
      v.pos[0] = v.pos[1] = v.pos[2] = 0.0;
      
      for ( int k=0; k < v.weightCount; k++ ) {
        Weight &w = meshes[i]->weights[v.weightIndex + k];
        Joint &joint = frame.joints[w.joint];
        
        Quat q(w.pos[0], w.pos[1], w.pos[2], 0.0f);
        Quat result = joint.quat*q*joint.quat.Conjugate();
        v.pos[0] += (joint.pos[0] + result[0])*w.w;
        v.pos[1] += (joint.pos[1] + result[1])*w.w;
        v.pos[2] += (joint.pos[2] + result[2])*w.w;
      } // for (weights)
    } // for (mesh vertices)
  } // for (meshes)
}


void MD5Model::buildNormals() {
  // zero out the normals
  for ( size_t i=0; i < meshes.size(); i++) {
    for ( size_t j=0; j < meshes[i]->verts.size(); j++ ) {
      meshes[i]->verts[j].n[0] = 0.0;
      meshes[i]->verts[j].n[1] = 0.0;
      meshes[i]->verts[j].n[2] = 0.0;
    }

    // for each normal, add contribution to normal from every face that vertex
    // is part of
    for ( size_t j=0; j < meshes[i]->tris.size(); j++ ) {
      Vertex &v0 = meshes[i]->verts[ meshes[i]->tris[j].v[0] ];
      Vertex &v1 = meshes[i]->verts[ meshes[i]->tris[j].v[1] ];
      Vertex &v2 = meshes[i]->verts[ meshes[i]->tris[j].v[2] ];

      float Ax = v1.pos[0] - v0.pos[0];
      float Ay = v1.pos[1] - v0.pos[1];
      float Az = v1.pos[2] - v0.pos[2];

      float Bx = v2.pos[0] - v0.pos[0];
      float By = v2.pos[1] - v0.pos[1];
      float Bz = v2.pos[2] - v0.pos[2];

      float nx = Ay * Bz - By * Az;
      float ny = -(Ax * Bz - Bx * Az);
      float nz = Ax * By - Bx * Ay;

      v0.n[0] += nx;
      v0.n[1] += ny;
      v0.n[2] += nz;

      v1.n[0] += nx;
      v1.n[1] += ny;
      v1.n[2] += nz;

      v2.n[0] += nx;
      v2.n[1] += ny;
      v2.n[2] += nz;
    }

    // normalize each normal
    for ( size_t j=0; j < meshes[i]->verts.size(); j++ ) {
      Vertex &v = meshes[i]->verts[j];

      float mag = (float)sqrt( float(v.n[0]*v.n[0] + v.n[1]*v.n[1] + v.n[2]*v.n[2]) );

      // avoid division by zero
      if ( mag > 0.0001f ) {
        v.n[0] /= mag;
        v.n[1] /= mag;
        v.n[2] /= mag;
      }
    }
  } // for (meshes)
}


void MD5Model::buildFrames(Anim &anim) {
  for ( int i=0; i < anim.numFrames; i++ ) {
    // allocate storage for joints for this frame
    anim.frames[i].joints.resize(numJoints);

    for ( int j=0; j < numJoints; j++ ) {
      const Joint &joint = anim.baseJoints[j];
  
      float pos[3]    = { joint.pos[0],  joint.pos[1],  joint.pos[2]  };
      float orient[3] = { joint.quat[0], joint.quat[1], joint.quat[2] };
  
      int n = 0;
      for ( int k=0; k < 3; k++ )
        if ( anim.jointInfo[j].flags & (1 << k)  ) {
          pos[k] = anim.frames[i].animatedComponents[anim.jointInfo[j].startIndex + n];
          n++;
        }

      for ( int k=0; k < 3; k++ )
        if ( anim.jointInfo[j].flags & (8 << k)  ) {
          orient[k] = anim.frames[i].animatedComponents[anim.jointInfo[j].startIndex + n];
          n++;
        }

      Quat q = buildQuat(orient[0], orient[1], orient[2]);

      Joint &frameJoint = anim.frames[i].joints[j];
      frameJoint.name   = anim.jointInfo[j].name;
      frameJoint.parent = anim.jointInfo[j].parentIndex;

      // root joint?
      if ( anim.jointInfo[j].parentIndex < 0 ) {
        frameJoint.pos[0] = pos[0];
        frameJoint.pos[1] = pos[1];
        frameJoint.pos[2] = pos[2];
        frameJoint.quat = q;
      }
      else {
        Joint &parent = anim.frames[i].joints[anim.jointInfo[j].parentIndex];
  
        // rotate position (qp is quaternion representation of position)
        Quat qp(pos[0], pos[1], pos[2], 0.0f);
        Quat result = parent.quat*qp*parent.quat.Conjugate();
  
        frameJoint.pos[0] = result[0] + parent.pos[0];
        frameJoint.pos[1] = result[1] + parent.pos[1];
        frameJoint.pos[2] = result[2] + parent.pos[2];
  
        // store orientation of this joint
        frameJoint.quat = parent.quat*q;
        frameJoint.quat.Normalize();
      } // else
    } // for
  } // for
}


Quat MD5Model::buildQuat(float x, float y, float z) const {
  // compute the 4th component of the quaternion
  float w = 1.0f - x*x - y*y - z*z;
  w = w < 0.0 ? 0.0f : (float)-sqrt( double(w) );

  Quat q(x, y, z, w);
  q.Normalize();

  return q;
}


MD5Model::Anim::Anim():
     numFrames(0),
     frameRate(0),
     numAnimatedComponents(0) {

}

void MD5Model::Mesh::LoadTexture()
/**
Load texture image files and bind to OpenGL
*/
{ 
  glEnable( GL_TEXTURE_2D );

  //get number of files to load
  this->textureID_p = & this->textureID;

  //generate texture holders
  glGenTextures(1,this->textureID_p);

  //get texture from file
  std::string FileName = this->texture;
  int width;
  int height;
  GLubyte * ImgData = PPM::Read(FileName, width, height);
  
  //bind texture to current texture holder
  glBindTexture(GL_TEXTURE_2D, this->textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  //pass texture to openGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImgData);

  //let opengl automatically build mipmap
  glGenerateMipmap(GL_TEXTURE_2D);
    
  //delte image data
  delete [] ImgData;
  ImgData = NULL;

}
