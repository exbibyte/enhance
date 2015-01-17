#include "Log.h"
#include <vector>
#include <string>
using namespace std;

vector<string> LogClass::vLogStack;

void LogFuncStr( string input ){
  cout << "LOG FUNCTION: " << input << endl;
}
