//=====================================================================
// Summary:             Implements a template directed graph generator 
//
// Notes:               Generates and gives best path from provided weight mapping based on lowest path weight. Currently only accepts non-negative path weights
//=============================================================================

#ifndef GRAPHDIRECTED_H
#define GRAPHDIRECTED_H

#include <map>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

template < typename KeyType >
class graph_node 
{     
public:
    graph_node( KeyType key );
    graph_node();
    KeyType mKey;
    bool bVisited;                       //flag set when all directed neighbours have been visited
    vector< graph_node * > vTransitionTo; //directed edge to another node
    int iWeightSum;                      //best cumulative weight, initialized to -1
    graph_node * PrevNode;                //best previous node travelled
};

template < typename KeyType >
graph_node< KeyType >::graph_node( KeyType key ): mKey(key), bVisited(false), iWeightSum(-1), PrevNode(0) 
{
    vTransitionTo.clear(); 
}

template < typename KeyType >
class graph_directed
{
public:
    bool GetShortestPath( KeyType Source, KeyType Destination, vector< KeyType > & vPath, int & Dist  ); //returns a vector of shortest path keys and distance if path exists
    bool GetShortestPath( KeyType Source, KeyType Destination, vector< KeyType > Intermediate, vector< KeyType > & vPath, int & Dist  ); //returns a vector of shortest path keys and distance if path exists
    bool GenerateGraphFromWeightMap( map< pair<KeyType, KeyType> , int > & ); //generates a graph from transition path weight map and saves a copy of the map, only non-negative weights are accepted

private:
    //helper functions:
    bool Getgraph_nodeFromKey( KeyType Key, graph_node<KeyType> * & ) const;
    bool GetPathWeight( KeyType Source, KeyType Destination, int & Weight ) const; //return false if transition doesn't exist
    bool ResetAllVisited();     // reset all bVisited flag for graph nodes
    bool VisitNeighbourNodes( vector<KeyType> & vPath, int & Dist, graph_node<KeyType> * SourceNode, graph_node<KeyType> * DestinationNode ); //returns best path and dist
    void VisitNeighbourNodesHelper( graph_node<KeyType> * CurrentNode, graph_node<KeyType> * DestinationNode ); //depth-first recursive path finding
    bool VisitNeighbourNodesWithIntermediate( vector<KeyType> & vPath, int & Dist, graph_node<KeyType> * SourceNode, graph_node<KeyType> * DestinationNode, vector< graph_node<KeyType> * > vIntermediate );
    void VisitNeighbourNodesWithIntermediateHelper( graph_node<KeyType> * CurrentNode, graph_node<KeyType> * DestinationNode, vector< graph_node<KeyType> * > vIntermediate, vector<KeyType> & vPath, int & Dist, vector<KeyType> vPathCurrentBranch, int DistCurrentBranch );
    
    //data mappings
    map< KeyType, graph_node<KeyType> * > mMapgraph_node;   
    map< pair<KeyType, KeyType> , int > mMapWeight; //transition path weight map: Map< Pair<KeyType Source, KeyType Destination > , int PathWeight >     
};

template< typename KeyType >
bool graph_directed<KeyType>::Getgraph_nodeFromKey( KeyType Key, graph_node<KeyType> * & Node ) const
{
    typename map< KeyType, graph_node<KeyType> * >::const_iterator it = mMapgraph_node.find( Key );
    
    if( it == mMapgraph_node.end() )
    {
        return false;
    }
    else
    {
        Node = it->second;
        return true;
    }
}

template< typename KeyType >
bool graph_directed<KeyType>::GetShortestPath( KeyType Source, KeyType Destination, vector< KeyType > & vPath, int & Dist )
{
    bool bRet = true;

    graph_node<KeyType> * pNodeSource = 0;
    graph_node<KeyType> * pNodeDestination = 0;

    bool bFound = Getgraph_nodeFromKey( Source, pNodeSource );
    if( !bFound )
    {
        bRet = false;
        return bRet;
    }
    bFound = Getgraph_nodeFromKey( Destination, pNodeDestination );
    if( !bFound )
    {
        bRet = false;
        return bRet;
    }
        
    ResetAllVisited(); //reset all nodes

    bRet = VisitNeighbourNodes( vPath, Dist, pNodeSource, pNodeDestination ); //find best path

    ResetAllVisited();

    return bRet;
}


template< typename KeyType >
bool graph_directed<KeyType>::GetShortestPath( KeyType Source, KeyType Destination, vector< KeyType > Intermediate, vector< KeyType > & vPath, int & Dist  ) //returns a vector of shortest path keys and distance if path exists
{
    bool bRet = true;

    graph_node<KeyType> * pNodeSource = 0;
    graph_node<KeyType> * pNodeDestination = 0;

    //find source and destination nodes
    bool bFound = Getgraph_nodeFromKey( Source, pNodeSource );
    if( !bFound )
    {
        bRet = false;
        return bRet;
    }
    bFound = Getgraph_nodeFromKey( Destination, pNodeDestination );
    if( !bFound )
    {
        bRet = false;
        return bRet;
    }

    //find intermediate nodes
    vector< graph_node<KeyType> * > vIntermediate;
    for( typename vector< KeyType >::iterator itIntermediate = Intermediate.begin(); itIntermediate != Intermediate.end(); ++itIntermediate )
    {
        graph_node<KeyType> * pNodeIntermediate;
        bFound = Getgraph_nodeFromKey( *itIntermediate, pNodeIntermediate );
        if( !bFound )
        {
            bRet = false;
            return bRet;
        }   
        vIntermediate.push_back( pNodeIntermediate );
    }
        
    ResetAllVisited(); //reset all nodes

    bRet = VisitNeighbourNodesWithIntermediate( vPath, Dist, pNodeSource, pNodeDestination, vIntermediate ); //find best pathx

    ResetAllVisited();

    return bRet;
}

template< typename KeyType >
bool graph_directed<KeyType>::GenerateGraphFromWeightMap( map< pair<KeyType, KeyType> , int > & MapWeight )
{
    mMapWeight.clear();
    mMapWeight = MapWeight; //copy weight mapping

    //delete existing nodes
    typename map< KeyType, graph_node<KeyType> * >::iterator itMapGraph = mMapgraph_node.begin(); 
    for( ; itMapGraph != mMapgraph_node.end(); ++itMapGraph )
    {
        delete itMapGraph->second;
        itMapGraph->second = 0;
    }
    mMapgraph_node.clear();
    
    //go through every relation in weight mapping and get Source and Destination Keys
    typename map< pair<KeyType, KeyType> , int >::const_iterator itMapWeight = mMapWeight.begin();
    for( ; itMapWeight != mMapWeight.end(); ++itMapWeight )
    {
        //check for negative weight
        if( 0 > itMapWeight->second )
        {
            return false;
        }

        KeyType Source = itMapWeight->first.first;
        KeyType Destination = itMapWeight->first.second;

        graph_node<KeyType> * pNodeSource = 0;
        graph_node<KeyType> * pNodeDestination = 0;

        //search in existing graph mapping for Source node and create new node if necessary
        bool bRet = Getgraph_nodeFromKey( Source, pNodeSource );
        if( !bRet )
        {
            pNodeSource = new graph_node<KeyType>( Source );
            mMapgraph_node[ Source ] = pNodeSource;
        }

        //search in existing graph mapping for Destination node and create new node if necessary
        bRet = Getgraph_nodeFromKey( Destination, pNodeDestination );
        if( !bRet )
        {
            pNodeDestination = new graph_node<KeyType>( Destination );
            mMapgraph_node[ Destination ] = pNodeDestination;
        }
        
        //link the Source node to the Destination node
        pNodeSource->vTransitionTo.push_back( pNodeDestination );
        
        pNodeSource = 0;
        pNodeDestination = 0;
    }

    return true;
}

template< typename KeyType >
bool graph_directed< KeyType >::GetPathWeight( KeyType Source, KeyType Destination, int & Weight ) const
{
    typename map< pair<KeyType, KeyType> , int >::const_iterator it = mMapWeight.find( std::make_pair( Source, Destination ) );
    if( it == mMapWeight.end() )
    {
        return false;
    }
    else
    {
        Weight = it->second;
        return true;    
    }
}

template< typename KeyType >
bool graph_directed< KeyType >::ResetAllVisited()
{
    typename map< KeyType, graph_node<KeyType> * >::const_iterator it = mMapgraph_node.begin();    
    for( ; it != mMapgraph_node.end(); ++it )
    {
        it->second->bVisited = false;
        it->second->PrevNode = 0;
        it->second->iWeightSum = -1;
    }

    return true;
}

template< typename KeyType >
bool graph_directed< KeyType >::VisitNeighbourNodes( vector<KeyType> & vPath, int & Dist, graph_node<KeyType> * SourceNode, graph_node<KeyType> * DestinationNode )
{
    bool bRet = true;
    graph_node< KeyType > * DummyNode = 0; //deals with cycling source and destination node
  
    vPath.clear(); //reset result path
    Dist = -1;
    SourceNode->iWeightSum = 0;
    
    VisitNeighbourNodesHelper( SourceNode, DestinationNode ); //go through graph and calculate cumulative weights

    int iSelfTransition;
    if( SourceNode == DestinationNode && !GetPathWeight( DestinationNode->mKey, DestinationNode->mKey, iSelfTransition ) ) //do an additional walk if it cycles
    {
      int iPathBestCycle = -1;
      int iPathWeight = -1;
      KeyType KeyBestCycle;
      for( typename map< KeyType, graph_node<KeyType> * >::const_iterator it = mMapgraph_node.begin(); it != mMapgraph_node.end(); it++ )
      {
	if( it->second->bVisited && GetPathWeight( it->first, DestinationNode->mKey, iPathWeight ) ) //if path exists
	{
	  if( iPathBestCycle == -1 || it->second->iWeightSum + iPathWeight < iPathBestCycle ) //found a better path
	  {
	    KeyBestCycle = it->first;
	    iPathBestCycle = iPathWeight + it->second->iWeightSum;
	  }
	}
      }
      if( iPathBestCycle == -1 ) //didn't find a path to destination
      {
	return false;
      }

      //make a node representing the final destination node
      DummyNode = new graph_node< KeyType >( DestinationNode->mKey );
      DummyNode->bVisited = true;
      DummyNode->PrevNode = mMapgraph_node[ KeyBestCycle ];
      DummyNode->iWeightSum = iPathBestCycle;

      DestinationNode = DummyNode;
    }
    
    if( DestinationNode->bVisited ) //found destination and get the best path, distance
    {
        Dist = DestinationNode->iWeightSum;
        //traverse backwards to find the chosen path
        graph_node<KeyType> * Prev = DestinationNode;
        while( Prev )
        {
            vPath.push_back( Prev->mKey );
            Prev = Prev->PrevNode;
        }
        std::reverse( vPath.begin(), vPath.end() ); //make items in vector start from Source and end with Destination
        bRet = true; 
    }
    else{
        bRet = false;
    }

    if( !DummyNode ){
      delete DummyNode;
      DummyNode = 0;
    }

    return bRet;
}

template< typename KeyType >
void graph_directed< KeyType >::VisitNeighbourNodesHelper( graph_node<KeyType> * CurrentNode, graph_node<KeyType> * DestinationNode )
{
    //return if current node has been visited
    if( CurrentNode->bVisited )
    {
        return;
    }
    
    //add weight to neighbours
    for( typename vector< graph_node<KeyType> * >::iterator it = CurrentNode->vTransitionTo.begin(); it != CurrentNode->vTransitionTo.end(); ++it )
    {
        int PathWeight;
        GetPathWeight( CurrentNode->mKey, (*it)->mKey, PathWeight );
        //do weight summation
        int WeightSum = CurrentNode->iWeightSum + PathWeight; 
        //saves new weight if less than previous or if previous doesn't exist
        if( -1 == (*it)->iWeightSum || (*it)->iWeightSum > WeightSum )
        {
            //reset node's visited flag since we found a better path and let new weights trickle down affected paths
            if( (*it)->bVisited )
            {
                (*it)->bVisited = false;
            }
            (*it)->iWeightSum = WeightSum;
            (*it)->PrevNode = CurrentNode;
        }
    }

    //mark current node as visited after its neighbours are all vistied
    CurrentNode->bVisited = true;
        
    //depth-first recurse through directed neighbours
    for( typename vector< graph_node<KeyType> * >::iterator it = CurrentNode->vTransitionTo.begin(); it != CurrentNode->vTransitionTo.end(); ++it )
    {
        VisitNeighbourNodesHelper( *it, DestinationNode );
    }
}

template< typename KeyType >
bool graph_directed< KeyType >::VisitNeighbourNodesWithIntermediate( vector<KeyType> & vPath, int & Dist, graph_node<KeyType> * CurrentNode, graph_node<KeyType> * DestinationNode, vector< graph_node<KeyType> * > vIntermediate )
{
    vector<KeyType> vPathCurrentBranch;
    int DistCurrentBranch;
    vPathCurrentBranch.clear();
    DistCurrentBranch = 0;

    vPath.clear(); //reset result path
    Dist = -1;
    CurrentNode->iWeightSum = 0;
    
    VisitNeighbourNodesWithIntermediateHelper( CurrentNode, DestinationNode, vIntermediate, vPath, Dist, vPathCurrentBranch, DistCurrentBranch ); //go through graph and calculate cumulative weights

    if( Dist >= 0 ) //found destination and get the best path, distance
    {
        return true; 
    }
    else{
        return false;
    }
}

template< typename KeyType >
void graph_directed< KeyType >::VisitNeighbourNodesWithIntermediateHelper( graph_node<KeyType> * CurrentNode, graph_node<KeyType> * DestinationNode, vector< graph_node<KeyType> * > vIntermediate, vector<KeyType> & vPath, int & Dist, vector<KeyType> vPathCurrentBranch, int DistCurrentBranch )
{  
    ResetAllVisited(); //reset all nodes

    if( !vIntermediate.empty() ) //check intermediate node to visit
    {
        for( typename vector< graph_node<KeyType> * >::iterator itIntermediate = vIntermediate.begin(); itIntermediate != vIntermediate.end(); ++itIntermediate )
        {
            graph_node<KeyType> * NextDestination = *itIntermediate;

            vector< graph_node<KeyType> * > vIntermediateNext;
            //copy all intermediate nodes except current iterator
            vIntermediateNext.insert( vIntermediateNext.end(), vIntermediate.begin(), itIntermediate );
            vIntermediateNext.insert( vIntermediateNext.end(), itIntermediate + 1, vIntermediate.end() );

            vector<KeyType> vPathTemp;
            int DistTemp;
        
            bool bFound = VisitNeighbourNodes( vPathTemp, DistTemp, CurrentNode, NextDestination ); //go through graph and find paths

            if( bFound ) //check if intermediate node is reachable
            {
                CurrentNode = NextDestination;
                if( vPathCurrentBranch.empty() )
                {
                    vPathCurrentBranch.push_back( vPathTemp.front() );
                }
                vPathCurrentBranch.insert( vPathCurrentBranch.end(), vPathTemp.begin()+1, vPathTemp.end() );
                DistCurrentBranch += DistTemp;

                //check if any remaining intermediate nodes have been visited by the last path search
                for( typename vector<KeyType>::iterator itPathTemp = vPathTemp.begin(); itPathTemp != vPathTemp.end(); ++itPathTemp )
                {
                    graph_node<KeyType> * itNode = 0;
                    typename map< KeyType, graph_node<KeyType> * >::iterator itMapNode = mMapgraph_node.find(*itPathTemp); //find node from key map
                    if( itMapNode != mMapgraph_node.end() )
                    {
                        itNode = itMapNode->second;
                        typename vector< graph_node<KeyType> * >::iterator itNodeRepeat = find(vIntermediateNext.begin(), vIntermediateNext.end(), itNode ); //find node from remaining intermediate nodes
                        if( itNodeRepeat != vIntermediateNext.end() )
                        {
                            vIntermediateNext.erase( itNodeRepeat );
                        }
                    }
                }

                VisitNeighbourNodesWithIntermediateHelper( CurrentNode, DestinationNode, vIntermediateNext, vPath, Dist, vPathCurrentBranch, DistCurrentBranch ); //recurse this algorithm
            }
            else //end current recursion branch if no path is found
            {
                return;
            }
        }
    }
    else //check last destination node
    {
        vector<KeyType> vPathTemp;
        int DistTemp;
        
        bool bFound = VisitNeighbourNodes( vPathTemp, DistTemp, CurrentNode, DestinationNode ); //go through graph and find paths

        if( bFound ) //check if destination if reachable
        {
            if( vPathCurrentBranch.empty() )
            {
                vPathCurrentBranch.push_back( vPathTemp.front() );
            }
            vPathCurrentBranch.insert( vPathCurrentBranch.end(), vPathTemp.begin()+1, vPathTemp.end() );
            DistCurrentBranch += DistTemp;
 
            //check if it is the most optimal path so far
            if( DistCurrentBranch < Dist || Dist < 0 )
            {
                vPath = vPathCurrentBranch;
                Dist = DistCurrentBranch;
            }
            return;
        }
        else //end current recursion branch if no path is found
        {
            return;
        }
    }
}

#endif
