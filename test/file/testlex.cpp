#include <stdexcept>
#include <iostream>
#include <fstream>

#include "Lex.h"

using namespace std;

int main(int argc, char ** argv){

  //open file
  std::ifstream fin(argv[1], std::ifstream::in);

  Lex a;

  // try{
  //   a.LoadFile();
  // }catch(exception & e){
  //   cout<<e.what()<<endl;
  // }

  //get token
  std::string * tokstr = new std::string;
  
  Lex::TOKEN t;
  do{
    t = a.getNextToken(fin, tokstr);
    cout<<*tokstr<<endl;
  }while(t != Lex::TOKEN_INVALID);

  a.CloseFile();

  return 0;
}
