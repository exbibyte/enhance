#ifndef FILTER_PARSENODE_H
#define FILTER_PARSENODE_H

#include "ParseNode.h"

class Filter_ParseNode_NodeTraversal {
public:
    enum Enum {
	IN_ORDER,
	PRE_ORDER,
	POST_ORDER,
	BREATH,	
    };
};

class Filter_ParseNode {
public:
    Filter_ParseNode();
    bool VisitNode( ParseNode * );
    bool VisitNode_PostOrder( ParseNode * node );
    void SetNodeTraversal( Filter_ParseNode_NodeTraversal::Enum mode );
    virtual bool TransformNode( ParseNode * ){ return false; }    
    Filter_ParseNode_NodeTraversal::Enum _NodeTraversal;
};

#endif
