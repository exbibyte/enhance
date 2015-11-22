#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "GraphSearch.h"
#include "GraphNodeG.h"

#include <queue>
#include <map>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <memory>
using namespace std;

TEST_CASE( "GraphSearch", "[GraphSearch]" ) {
  bool bRet;

  map< int, shared_ptr< GraphNodeWeightedSimple<std::string> > > nodes;
  char szNewString [256];
  for( int i = 0; i < 6; ++i ){
      sprintf( szNewString , "Value_%d", i );
      string newString( szNewString );
      nodes.emplace( i, make_shared< GraphNodeWeightedSimple<string> >( i, newString ) );
  }

  //wire up the graph
  nodes[0]->_desc.insert( nodes[1] );
  nodes[0]->_desc.insert( nodes[2] );
  nodes[2]->_desc.insert( nodes[3] );
  nodes[3]->_desc.insert( nodes[4] );
  nodes[1]->_desc.insert( nodes[4] );
  nodes[4]->_desc.insert( nodes[5] );
  nodes[0]->_desc.insert( nodes[5] );
  
  map< pair<int, int>, int > weightmap;
  weightmap.insert( make_pair( make_pair( 0, 1 ), 5 ) );
  weightmap.insert( make_pair( make_pair( 0, 2 ), 4 ) );
  weightmap.insert( make_pair( make_pair( 1, 4 ), 3 ) );
  weightmap.insert( make_pair( make_pair( 2, 3 ), 2 ) );
  weightmap.insert( make_pair( make_pair( 3, 4 ), -1 ) );
  weightmap.insert( make_pair( make_pair( 0, 5 ), -1 ) );
  weightmap.insert( make_pair( make_pair( 4, 5 ), 6 ) );

  map< pair<int, int>, int> capacitymap;
  capacitymap.insert( make_pair( make_pair( 0, 1 ), 1 ) );
  capacitymap.insert( make_pair( make_pair( 0, 2 ), 3 ) );
  capacitymap.insert( make_pair( make_pair( 1, 4 ), 2 ) );
  capacitymap.insert( make_pair( make_pair( 2, 3 ), 1 ) );
  capacitymap.insert( make_pair( make_pair( 3, 4 ), 6 ) );
  capacitymap.insert( make_pair( make_pair( 0, 5 ), 0 ) );
  capacitymap.insert( make_pair( make_pair( 4, 5 ), 5 ) );

  map< pair<int, int>, int> flowmap;
  capacitymap.insert( make_pair( make_pair( 0, 1 ), 0 ) );
  capacitymap.insert( make_pair( make_pair( 0, 2 ), 0 ) );
  capacitymap.insert( make_pair( make_pair( 1, 4 ), 0 ) );
  capacitymap.insert( make_pair( make_pair( 2, 3 ), 0 ) );
  capacitymap.insert( make_pair( make_pair( 3, 4 ), 0 ) );
  capacitymap.insert( make_pair( make_pair( 0, 5 ), 0 ) );
  capacitymap.insert( make_pair( make_pair( 4, 5 ), 0 ) );
  
  //lambda of the edge weight function given 2 nodes
  auto weight_func = [&weightmap]( shared_ptr< GraphNodeWeightedSimple< string > > node_src, shared_ptr< GraphNodeWeightedSimple< string > > node_dest, int & edge_weight ){
      auto it = weightmap.find( make_pair( node_src->_id, node_dest->_id ) );
      if( weightmap.end() != it ){
	  edge_weight = it->second;
	  return true;
      }else{
	  return false;
      }
  };

  SECTION( "BreathFirstSearch Check" ) {

      GraphSearch<std::string >::BreathFirstSearch( weight_func, nodes[0] );
      
      auto node4_pred_1x = nodes[4]->_pred;
      auto node4_pred_2x = nodes[4]->_pred->_pred;
      auto node4_pred_3x = nodes[4]->_pred->_pred->_pred;
      auto node4_pred_4x = nodes[4]->_pred->_pred->_pred->_pred;
      CHECK( node4_pred_1x.get() == nodes[3].get() );
      CHECK( node4_pred_2x.get() == nodes[2].get() );
      CHECK( node4_pred_3x.get() == nodes[0].get() );
      CHECK( node4_pred_4x.get() == nullptr );
      CHECK( 5 == nodes[4]->_relaxed_weight );
      
      auto node5_pred_1x = nodes[5]->_pred;
      auto node5_pred_2x = nodes[5]->_pred->_pred;
      CHECK( node5_pred_1x.get() == nodes[0].get() );
      CHECK( node5_pred_2x.get() == nullptr );
      CHECK( -1 == nodes[5]->_relaxed_weight );
  }

  SECTION( "DepthFirstSearch Check" ) {

      GraphSearch<std::string >::DepthFirstSearch( weight_func, nodes[0] );
      
      auto node4_pred_1x = nodes[4]->_pred;
      auto node4_pred_2x = nodes[4]->_pred->_pred;
      auto node4_pred_3x = nodes[4]->_pred->_pred->_pred;
      auto node4_pred_4x = nodes[4]->_pred->_pred->_pred->_pred;
      CHECK( node4_pred_1x.get() == nodes[3].get() );
      CHECK( node4_pred_2x.get() == nodes[2].get() );
      CHECK( node4_pred_3x.get() == nodes[0].get() );
      CHECK( node4_pred_4x.get() == nullptr );
      CHECK( 5 == nodes[4]->_relaxed_weight );
      
      auto node5_pred_1x = nodes[5]->_pred;
      auto node5_pred_2x = nodes[5]->_pred->_pred;
      CHECK( node5_pred_1x.get() == nodes[0].get() );
      CHECK( node5_pred_2x.get() == nullptr );
      CHECK( -1 == nodes[5]->_relaxed_weight );
  }

  SECTION( "DijstraSearch Check" ) {

      GraphSearch<std::string >::DijstraSearch( weight_func, nodes[0] );
      
      auto node4_pred_1x = nodes[4]->_pred;
      auto node4_pred_2x = nodes[4]->_pred->_pred;
      auto node4_pred_3x = nodes[4]->_pred->_pred->_pred;
      auto node4_pred_4x = nodes[4]->_pred->_pred->_pred->_pred;
      CHECK( node4_pred_1x.get() == nodes[3].get() );
      CHECK( node4_pred_2x.get() == nodes[2].get() );
      CHECK( node4_pred_3x.get() == nodes[0].get() );
      CHECK( node4_pred_4x.get() == nullptr );
      CHECK( 5 == nodes[4]->_relaxed_weight );
      
      auto node5_pred_1x = nodes[5]->_pred;
      auto node5_pred_2x = nodes[5]->_pred->_pred;
      CHECK( node5_pred_1x.get() == nodes[0].get() );
      CHECK( node5_pred_2x.get() == nullptr );
      CHECK( -1 == nodes[5]->_relaxed_weight );
  }
}
