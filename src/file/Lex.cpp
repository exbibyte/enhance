#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cstddef>
#include <cctype>
#include <stdexcept>

#include "Lex.h"

#define IS_WHITESPACE(c) (' ' == c || '\t' == c || '\r' ==c || '\n' == c )

// reads in a string terminal and stores it in str
// (assumes opening " has already been read in)
void Lex::readString(std::ifstream &fin, std::string &str) {
  str = std::string();

  // read characters until closing " is found
  char c = '\0';
  do {
    fin.get(c);

    if ( fin.eof() )
      throw Exception ("Lex::readString(): reached end of file before \" was found");

    if ( c != '"')
      str += c;
  } while ( c != '"' );
}


void Lex::skipComments(std::ifstream &fin) {
  char c;
  fin.get(c);

  if ( c != '/' )
    throw Exception ("Lex::skipComments(): invalid comment, expected //");

  while ( !fin.eof() && c != '\n' )
    fin.get(c);

  // put back last character read
  fin.putback(c);
}


// reads until first non-whitespace character
void Lex::skipWhitespace(std::ifstream &fin) {
  char c = '\0';
  while ( !fin.eof() ) {
    fin.get(c);

    if ( !IS_WHITESPACE(c) ) {
      fin.putback(c);
      break;
    }
  }
}


// reads in next token from file and matches it to a token type,
// if tokStr is non-NULL then it will be set to the text of the token
Lex::TOKEN Lex::getNextToken(std::ifstream &fin, std::string *tokStr) {
  skipWhitespace(fin);
  std::string str;

  TOKEN t = TOKEN_INVALID;

  while ( !fin.eof() ) {
    char c = '\0';
    fin.get(c);

    // single character tokens
    if ( '{' == c || '}' == c || '(' == c || ')' == c ) {
      // if already reading in a token, treat this as a delimiter
      if ( t != TOKEN_INVALID ) {
        fin.putback(c);
        if ( tokStr != NULL )
          (*tokStr) = str;
      }
 
      if ( '{' == c )
        t = TOKEN_LBRACE;
      if ( '}' == c )
        t = TOKEN_RBRACE;
      if ( '(' == c )
        t = TOKEN_LPAREN;
      if ( ')' == c )
        t = TOKEN_RPAREN;

      if ( tokStr) {
        (*tokStr) = std::string();
        (*tokStr) += c;
      }
      return t;
    }
    if ( isdigit(c) ) {
      str += c;
      if ( TOKEN_INVALID == t )
        t = TOKEN_INT;
      else if ( t != TOKEN_INT && t != TOKEN_FLOAT && t != TOKEN_KEYWORD ) {
        std::string msg("Lex::getNextToken(): invalid token '");
        msg += str + "'";
        throw Exception (msg);
      }
    }
    if ( '-' == c ) {
      str += c;
      if ( TOKEN_INVALID == t )
        t = TOKEN_INT;
      else {
        std::string msg("Lex::getNextToken(): invalid token '");
        msg += str + "'";
        throw Exception (msg);
      }
    }
    if ( isalpha(c) ) {
      str += c;
      if ( TOKEN_INVALID == t )
        t = TOKEN_KEYWORD;
      else if ( t != TOKEN_KEYWORD ) {
        std::string msg("Lex::getNextToken(): invalid token '");
        msg += str + "'";
        throw Exception (msg);
      }
    }
    if ( '"' == c ) {
      // treat as a delimeter if already reading in a token
      if ( t != TOKEN_INVALID ) {
        fin.putback(c);
        if ( tokStr != NULL )
          (*tokStr) = str;
        return t;
      }
      readString(fin, str);

      if ( tokStr != NULL )
        (*tokStr) = str;

      return TOKEN_STRING;
    }
    if ( '.' == c ) {
      str += c;
      if ( t != TOKEN_INT ) {
        std::string msg("Lex::getNextToken(): invalid token '");
        msg += str + "'";
        throw Exception (msg);
      }
      t = TOKEN_FLOAT;
    }
    if ( '/' == c ) {
      // treat as a delimeter if already reading in a token
      if ( t != TOKEN_INVALID ) {
        if ( tokStr != NULL )
          (*tokStr) = str;
        return t;
      }
      
      skipComments(fin);
      skipWhitespace(fin);
      continue;
    }

    // treat whitespace as a delimeter
    if ( IS_WHITESPACE(c) ) {
      if ( tokStr != NULL )
        (*tokStr) = str;
      return t;
    }

    // at this point token type should be set, if it hasn't been then
    // token is invalid
    if ( TOKEN_INVALID == t ) {
        std::string msg("Lex::getNextToken(): invalid token '");
        str += c;
        msg += str + "'";
        throw Exception (msg);
    }
  }

  return TOKEN_INVALID;
}


void Lex::LoadFile(const char *filename) {

  std::cout<<"here!"<<std::endl;

  // sanity check
  if ( !filename )
    throw Exception ("Lex::loadMesh(): filename is NULL");

  // attempt to open file for reading
  std::ifstream fin(filename, std::ifstream::in);

  // was open successful?
  if ( !fin.is_open() ) {
    std::string msg = std::string("Lex::loadMesh(): unable to open ") +
                      std::string(filename) + std::string(" for reading");
    throw Exception (msg);
  }

  FileAction();
  fin.close();
}

void Lex::FileAction() {

}

void Lex::CloseFile() {

}
