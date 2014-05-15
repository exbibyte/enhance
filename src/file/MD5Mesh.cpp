#include "MD5Mesh.h"

#include <string>
#include <stdexcept>
#include <fstream>

void MD5Mesh::FileAction() {
  
}

void MD5Mesh::loadMesh(const char *filename) {
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

  // close input file (should be done destructor anyway...)
  fin.close();

  // calculate vertex positions and normals from information in joints
  buildVerts();
  buildNormals();
}
