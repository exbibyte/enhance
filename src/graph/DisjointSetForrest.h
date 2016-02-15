#ifndef DISJOINT_SET_FORREST_H
#define DISJOINT_SET_FORREST_H

class DisjointSetForrest {
public:
    class SetNode {
    public:
	SetNode() : _rank(0), _p(nullptr) {}
	SetNode * _p;
	unsigned int _rank;
    };
    static bool MakeSet( DisjointSetForrest::SetNode * & node ){
	node = new DisjointSetForrest::SetNode;
	node->_rank = 0;
	node->_p = node;
	return true;
    }
    static bool Union( DisjointSetForrest::SetNode * node_a, DisjointSetForrest::SetNode * node_b ){
	Link( FindSet( node_a ), FindSet( node_b ) );
	return true;
    }
    static bool Link( DisjointSetForrest::SetNode * node_a, DisjointSetForrest::SetNode * node_b ){
	if( node_a->_rank > node_b->_rank ){
	    node_b->_p = node_a;
	}else if( node_a->_rank < node_b->_rank ){
	    node_a->_p = node_b;
	}else{
	    node_b->_p = node_a;
	    ++node_a->_rank;
	}
	return true;
    }
    static DisjointSetForrest::SetNode * FindSet( DisjointSetForrest::SetNode * node ){
	if( node->_p != node ){
	    node->_p = FindSet( node->_p ); //recursive to top of tree and back down
	}
	return node->_p;
    }
    static bool IsSameSet( DisjointSetForrest::SetNode * node_a, DisjointSetForrest::SetNode * node_b ){
	if( FindSet( node_a ) == FindSet( node_b ) ) return true;
	else return false;
    }
};

#endif
