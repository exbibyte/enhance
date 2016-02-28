#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "SortCount.h"

#include <vector>
#include <algorithm>

using namespace std;

TEST_CASE( "SortCount", "[SortCount]" ) {
  vector< unsigned int > input = { 9, 3, 4, 6, 1, 0 };
  vector< unsigned int > output;
  vector< unsigned int > sort_check = input;
  std::sort( sort_check.begin(), sort_check.end() );
  cout << endl;
  SECTION( "Counting Sort" ) {
      bool bRet;
      bRet = SortCount::Sort( input, output );
      CHECK( bRet );
      unsigned int size_output = output.size();
      unsigned int size_input = input.size();
      CHECK( size_output == size_input );
      for( int i = 0; i < input.size(); i++ ){
	  unsigned int val_output = output[ i ];
	  unsigned int val_sort_check = sort_check[ i ];
	  CHECK( val_output == val_sort_check );
      }
  }
}
