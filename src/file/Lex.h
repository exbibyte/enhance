#ifndef LEX_H
#define LEX_H

#include <fstream>
#include <string>

class Lex{

protected:
  enum TOKEN { TOKEN_KEYWORD,
               TOKEN_INT,
               TOKEN_FLOAT,
               TOKEN_STRING,
               TOKEN_LBRACE,
               TOKEN_RBRACE,
               TOKEN_LPAREN,
               TOKEN_RPAREN,
               TOKEN_INVALID };

  // methods for lexer
  void                          readString( std::ifstream &fin, std::string &str );
  int                           readInt(std::ifstream &fin);
  float                         readFloat(std::ifstream &fin);
  void                          readVec(std::ifstream &fin, float *v, int n);

  void                          skipComments( std::ifstream &fin );
  void                          skipWhitespace( std::ifstream &fin );
  TOKEN                         getNextToken( std::ifstream &fin, std::string *tokStr = NULL );

  class Exception : public std::runtime_error {
  public:
    Exception(const std::string &msg): std::runtime_error(msg) { }
  };
};

#endif
