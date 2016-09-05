#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "transition_matrix.hpp"
#include <utility>
#include <string>
using namespace std;



TEST_CASE( "Transition Matrix", "[TransMatrix]" ) {

    //set FSA initialization
    TransMatrix< string > TransitionTable;
    bool bRet = TransitionTable.SetTransition( "airplane", "bus", 3 );
    bRet = TransitionTable.SetTransition( "bus", "car", 1 );
    bRet = TransitionTable.SetTransition( "car", "library", 1 );
    bRet = TransitionTable.SetTransition( "coffee", "car", 1 );
    bRet = TransitionTable.SetTransition( "library", "coffee", 1 );
    bRet = TransitionTable.SetTransition( "bus", "bus", 2 );
    bRet = TransitionTable.SetTransition( "pig", "chicken", 0 );
    bRet = TransitionTable.SetTransition( "chicken", "pig", 0 );    

    //compute the transitive closure
    TransitionTable.UpdateClosure();

    int iRetCost;
    vector<string> vPath;

    //do tests
    SECTION( "Transitive Closure" ) {
        bRet = TransitionTable.GetClosure( "airplane", "library", iRetCost, vPath );
        CHECK( bRet == true );
	CHECK( iRetCost == 5 );

        bRet = TransitionTable.GetClosure( "coffee", "library", iRetCost, vPath );
        CHECK( bRet == true );
	CHECK( iRetCost == 2 );
	REQUIRE( vPath.size() == 3 );
	CHECK( vPath[0] == "coffee" );
	CHECK( vPath[1] == "car" );
	CHECK( vPath[2] == "library" );

        bRet = TransitionTable.GetClosure( "bus", "airplane", iRetCost, vPath );
        CHECK( bRet == false );

	bRet = TransitionTable.GetTransition( "airplane", "library", iRetCost );
        CHECK( bRet == true );
	CHECK( iRetCost == 5 );
    }
    SECTION( "Cyclic Closure" ) {
        bRet = TransitionTable.GetClosure( "coffee", "coffee", iRetCost, vPath );
        CHECK( bRet == true );
	CHECK( iRetCost == 3 );
	REQUIRE( vPath.size() == 4 );
	CHECK( vPath[0] == "coffee" );
	CHECK( vPath[1] == "car" );
	CHECK( vPath[2] == "library" );
	CHECK( vPath[3] == "coffee" );

	bRet = TransitionTable.GetTransition( "coffee", "coffee", iRetCost );
	CHECK( bRet == true );
	CHECK( iRetCost == 3 );

	bRet = TransitionTable.GetTransition( "pig", "pig", iRetCost );
	CHECK( bRet == true );
	CHECK( iRetCost == 0 );
	
    }
    SECTION( "Self Closure" ) {
	bRet = TransitionTable.GetClosure( "bus", "bus", iRetCost, vPath );
        CHECK( bRet == true );
	CHECK( iRetCost == 2 );
	REQUIRE( vPath.size() == 2 );
	CHECK( vPath[0] == "bus" );
	CHECK( vPath[1] == "bus" );

	bRet = TransitionTable.GetTransition( "bus", "bus", iRetCost );
	CHECK( bRet == true );
	CHECK( iRetCost == 2 );
    }
}
