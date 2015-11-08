#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "MinSpanTree.h"

#include <queue>
#include <map>
#include <functional>
#include <iostream>
#include <utility>
using namespace std;

TEST_CASE( "MinSpanTree", "[MinSpanTree]" ) {
  bool bRet;
  MinSpanTree tree;

  map< pair<int, int>, int > weightmap;
  weightmap.insert( make_pair( make_pair( 0, 1 ), 5 ) );
  weightmap.insert( make_pair( make_pair( 0, 2 ), 4 ) );
  weightmap.insert( make_pair( make_pair( 1, 2 ), 3 ) );
  weightmap.insert( make_pair( make_pair( 2, 3 ), 2 ) );
  weightmap.insert( make_pair( make_pair( 4, 5 ), 1 ) );
  tree.GenerateGraphFromWeightMap( weightmap );

  SECTION( "GenerateGraphFromWeightMap" ) {
      CHECK( 6 == tree._SetVertices.size() );
      CHECK( 5 == tree._EdgeWeights.size() );
      int current_edge_weight = -1;
      while( !tree._EdgeWeights.empty() ){
	  MinSpanTree::EdgeWeight edge = tree._EdgeWeights.top();
	  tree._EdgeWeights.pop();
	  CHECK( current_edge_weight <= edge._weight );
	  current_edge_weight = edge._weight;
      }
  }
}
