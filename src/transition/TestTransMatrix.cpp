#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "TransMatrix.h"
#include <utility>
#include <string>
using namespace std;



TEST_CASE( "ThreadPool", "[ThreadPool]" ) {

    //set FSA initialization
    TransMatrix< string, int  > TransitionTable;
    bool bRet = TransitionTable.SetTransition( "airplane", "bus", 1, 1 );
    bRet = TransitionTable.SetTransition( "bus", "car", 1, 2 );
    bRet = TransitionTable.SetTransition( "car", "library", 1, 3 );
    bRet = TransitionTable.SetTransition( "coffee", "library", 1, 4 );

    //compute the transitive closure
    TransitionTable.UpdateClosure();

    //do tests
    SECTION( "Transitive Closure" ) {
        int iCost;
        vector<string> vPath;
        bRet = TransitionTable.GetClosure( "airplane", "library", iCost, vPath );
        CHECK( bRet == true );
        bRet = TransitionTable.GetClosure( "coffee", "library", iCost, vPath );
        CHECK( bRet == true );
        bRet = TransitionTable.GetClosure( "car", "coffee", iCost, vPath );
        CHECK( bRet == false );
    }
}
