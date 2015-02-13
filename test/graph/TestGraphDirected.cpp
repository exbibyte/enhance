#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "GraphDirected.h"

#include <map>
#include <utility>
using namespace std;

TEST_CASE( "GraphDirected", "[GraphDirected]" ) {

  GraphDirected<int> _Optimizer;

  map< pair< int, int >, int > _Connections;
  _Connections.emplace( std::pair<int,int>(0,1), 1 );
  _Connections.emplace( std::pair<int,int>(1,2), 1 );
  _Connections.emplace( std::pair<int,int>(2,0), 1 );
  _Connections.emplace( std::pair<int,int>(0,3), 1 );
  _Connections.emplace( std::pair<int,int>(3,4), 1 );
  _Connections.emplace( std::pair<int,int>(4,1), 1 );
  _Connections.emplace( std::pair<int,int>(1,0), 1 );
  _Connections.emplace( std::pair<int,int>(2,6), 1 );
  _Connections.emplace( std::pair<int,int>(1,6), 1 );

  _Optimizer.GenerateGraphFromWeightMap( _Connections );
      
  SECTION( "Shortest Path" ) {

    vector< int > _Path;
    int _Dist;
    _Optimizer.GetShortestPath( 0, 6, _Path, _Dist  );
    REQUIRE( 3 == _Path.size() );
    CHECK( 2 == _Dist );
    CHECK( 0 == _Path[0] );
    CHECK( 1 == _Path[1] );
    CHECK( 6 == _Path[2] );
  }

}
