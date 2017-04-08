#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "min_span_tree.hpp"

#include <queue>
#include <map>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

TEST_CASE( "min_span_tree", "[min_span_tree]" ) {
  bool bRet;
  min_span_tree tree;

  map< pair<int, int>, int > weightmap;
  weightmap.insert( make_pair( make_pair( 0, 1 ), 5 ) );
  weightmap.insert( make_pair( make_pair( 0, 2 ), 4 ) );
  weightmap.insert( make_pair( make_pair( 1, 2 ), 3 ) );
  weightmap.insert( make_pair( make_pair( 2, 3 ), 2 ) );
  weightmap.insert( make_pair( make_pair( 4, 3 ), 1 ) );
  weightmap.insert( make_pair( make_pair( 0, 3 ), 0 ) );
  tree.GenerateGraphFromWeightMap( weightmap );

  SECTION( "GenerateGraphFromWeightMap" ) {
      CHECK( 5 == tree._SetVertices.size() );
      CHECK( 6 == tree._EdgeWeights.size() );
      int current_edge_weight = -1;
      while( !tree._EdgeWeights.empty() ){
	  min_span_tree::EdgeWeight edge = tree._EdgeWeights.top();
	  tree._EdgeWeights.pop();
	  CHECK( current_edge_weight <= edge._weight );
	  current_edge_weight = edge._weight;
      }
  }

  SECTION( "Generatemin_span_treeKruskal" ) {
      vector< min_span_tree::EdgeWeight > min_span_tree;
      bRet = tree.Generatemin_span_treeKruskal( min_span_tree );
      CHECK( bRet );
      CHECK( 4 == min_span_tree.size() );
      std::set<int> expected_min_span_tree_vertices { 0, 1, 2, 3, 4 };
      std::set<int> expected_min_span_tree_edge_weights { 0, 1, 2, 3 };
      for( auto & i : min_span_tree ){
	  expected_min_span_tree_vertices.erase( i._vert_a->_id );
	  expected_min_span_tree_vertices.erase( i._vert_b->_id );
	  expected_min_span_tree_edge_weights.erase( i._weight );
      }
      CHECK( 0 == expected_min_span_tree_vertices.size() );
      CHECK( 0 == expected_min_span_tree_edge_weights.size() );
  }
}
