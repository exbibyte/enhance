#ifndef MD5MESH_H
#define MD5MESH_H

#include "Lex.h"

class MD5MESH : public Lex {

 public:

  virtual void FileAction(); //parsing goes in here

  class Exception : public std::runtime_error {
  public:
    Exception(const std::string &msg): std::runtime_error(msg) { }
  };
};

#endif
