#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class LogException : public exception {
 public:
  LogException( string info ){
    strInfo = info;
  }
  string strInfo;
  const char* what() const noexcept {
    return strInfo.c_str();
  }
};

class LogClass{
 public:
  LogClass( string var ){
    vLogStack.push_back( var );
  }
  LogClass( int var ){
    char szInt[1048];
    sprintf( szInt, "%d", var );
    vLogStack.push_back( szInt );
  }
  LogClass( double var ){
    char szDouble[1048];
    sprintf( szDouble, "%f", var );
    vLogStack.push_back( szDouble );
  }
  ~LogClass(){
    if( ! vLogStack.empty() ){
      vLogStack.pop_back();
    }
  }
  static vector<string> vLogStack;
  static void PrintLog(){
    for( unsigned int i = 0; i < vLogStack.size() - 1; i++ ){
      cout << vLogStack[i] << "::";
    }
    cout << vLogStack[ vLogStack.size()-1 ] << endl;
  }
  static void ThrowExcepObj(){
    string strError;
    for( unsigned int i = 0; i < vLogStack.size(); i++ ){
      strError += vLogStack[i];
      strError += "::";
    }
    strError += "Error here";
    LogException ErrorObj( strError );
    throw ErrorObj;
  }
};

void LogFuncStr( string input );

#define LOGFUNC( info ) LogFuncStr(#info); info

#endif
