#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "bwt.hpp"

#include <vector>
#include <algorithm>
#include <set>
#include <string>

using namespace std;

TEST_CASE( "burrows-wheeler-transform", "[bwt]" ) {
    SECTION( "transform forward" ) {
	string input = "AGACATA$";
	string output = bwt::transform( input );
	CHECK( output == "ATG$CAAA" );
    }
    SECTION( "transform inverse" ) {
	string input = "AGGGAA$";
	string output = bwt::inverse( input );
	CHECK( output == "GAGAGA$" ) ;
    }    
}
