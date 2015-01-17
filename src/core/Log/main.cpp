#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Log.h"
using namespace std;

void testvoid();
int testfunc( int x, int y ) {
  LogClass logFuncEntry( "testfunc" );
  LogClass::PrintLog();

  LogClass::ThrowExcepObj();

  testvoid();
  return x * y;
}

void testvoid(){
  LogClass logtestvoid("testvoid");
  LogClass::PrintLog();
  LogClass::ThrowExcepObj();
  int something = 5;
}

int main()
{
  try{

    LogClass logEntrance( "Start of main()" );
    LogClass::PrintLog();
    
    int z;
    LOGFUNC( z = testfunc(5, testfunc( 3, 4 ) ) );

    LogClass::PrintLog();
    LogClass logmain( "At end of main() " );
    LogClass::PrintLog();
    
  }
  catch( LogException & e ){
    cout << "Caught exception: " << e.what() << endl;
  }
  catch( exception & e ){
    cout << e.what() << endl;
  }
  return 0;
}
