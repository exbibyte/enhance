#include "Filter_ParsePolyMesh.h"
#include "Filter_ParseNode.h"
#include "ParseNode.h"

Filter_ParseNode::Filter_ParseNode(){
    _NodeTraversal = Filter_ParseNode_NodeTraversal::PRE_ORDER;
}

bool Filter_ParseNode::VisitNode( ParseNode * node ){
    switch( _NodeTraversal ){
    case Filter_ParseNode_NodeTraversal::PRE_ORDER:
	VisitNode_InOrder( node );
	break;
    default:
    {}
    }
    return true;
}

bool Filter_ParseNode::VisitNode_InOrder( ParseNode * node ){
    bool bRet = true;
    if( !node ){
	return true;
    }
    for( auto * i : node->_children ){
	bRet = VisitNode( i );
	if(!bRet)
	    return false;
    }
    bRet = TransformNode( node );
    if( !bRet )
	return false;
    return true;
}

void Filter_ParseNode::SetNodeTraversal( Filter_ParseNode_NodeTraversal::Enum mode ){
    _NodeTraversal = mode;
}
