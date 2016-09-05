#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "shortest_path_bellmanford.hpp"

#include <queue>
#include <map>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

TEST_CASE( "shortest_path_bellmanford", "[shortest_path_bellmanford]" ) {
  bool bRet;
  shortest_path_bellmanford tree;

  map< pair<int, int>, int > weightmap;
  weightmap.insert( make_pair( make_pair( 0, 1 ), 5 ) );
  weightmap.insert( make_pair( make_pair( 0, 2 ), 4 ) );
  weightmap.insert( make_pair( make_pair( 1, 2 ), 3 ) );
  weightmap.insert( make_pair( make_pair( 2, 3 ), 2 ) );
  weightmap.insert( make_pair( make_pair( 4, 3 ), 1 ) );
  weightmap.insert( make_pair( make_pair( 3, 4 ), -1 ) );
  tree.GenerateGraphFromWeightMap( weightmap );

  SECTION( "GenerateGraphFromWeightMap" ) {
      CHECK( 5 == tree._SetVertices.size() );
      CHECK( 6 == tree._EdgeWeights.size() );
  }

  SECTION( "GenerateShortestPath" ) {
      std::list< int > shortest_path;
      int id_src = 0;
      int id_dest = 4;
      bRet = tree.GenerateShortestPath( id_src, id_dest, shortest_path );
      CHECK( bRet );
      CHECK( 4 == shortest_path.size() );
      std::list<int> expected_path { 0, 2, 3, 4 };
      CHECK( ( expected_path == shortest_path ) );
  }
}
