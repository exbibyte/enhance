#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "TransMatrix.h"
#include <utility>
#include <string>
using namespace std;



TEST_CASE( "ThreadPool", "[ThreadPool]" ) {

    //set FSA initialization
    TransMatrix< string, int  > TransitionTable;
    bool bRet = TransitionTable.SetTransition( "airplane", "bus", 1 );
    bRet = TransitionTable.SetTransition( "bus", "car", 2 );
    bRet = TransitionTable.SetTransition( "car", "library", 3 );
    bRet = TransitionTable.SetTransition( "coffee", "library", 4 );

    //do tests
    SECTION( "Transitive Closure" ) {
        bRet = TransitionTable.GetClosure( "airplane", "library" );
        CHECK( bRet == true );
        bRet = TransitionTable.GetClosure( "coffee", "library" );
        CHECK( bRet == true );
        bRet = TransitionTable.GetClosure( "car", "coffee" );
        CHECK( bRet == false );
    }
}
