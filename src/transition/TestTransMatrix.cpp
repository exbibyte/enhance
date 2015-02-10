#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "TransMatrix.h"
#include <utility>
#include <string>
using namespace std;



TEST_CASE( "ThreadPool", "[ThreadPool]" ) {

    //set FSA initialization
    TransMatrix< string, int  > TransitionTable;
    bool bRet = TransitionTable.SetTransition( "airplane", "bus", 3, 1 );
    bRet = TransitionTable.SetTransition( "bus", "car", 1, 2 );
    bRet = TransitionTable.SetTransition( "car", "library", 1, 3 );
    bRet = TransitionTable.SetTransition( "coffee", "car", 1, 4 );
    bRet = TransitionTable.SetTransition( "library", "coffee", 1, 4 );

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
        bRet = TransitionTable.GetClosure( "car", "bus", iCost, vPath );
        CHECK( bRet == false );

	int iRetCost;
	int iTemp;
	bRet = TransitionTable.GetTransition( "airplane", "library", iRetCost, iTemp );
	CHECK( iRetCost == 5 );

	bRet = TransitionTable.GetTransition( "bus", "bus", iRetCost, iTemp );
	CHECK( bRet == false );

	bRet = TransitionTable.GetTransition( "coffee", "coffee", iRetCost, iTemp );
	CHECK( bRet == true );
	CHECK( iRetCost == 3 );

        bRet = TransitionTable.GetClosure( "coffee", "coffee", iRetCost, vPath );
        CHECK( bRet == true );
	CHECK( iRetCost == 3 );
	CHECK( vPath[0] == "coffee" );
	CHECK( vPath[1] == "car" );
	CHECK( vPath[2] == "library" );
	CHECK( vPath[3] == "coffee" );
    }
}
