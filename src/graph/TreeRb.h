#ifndef TreeRb_H
#define TreeRb_H

#include <iostream>
#include <vector>
#include <utility>
using namespace std;
 
typedef enum eRbColour {
    eRbColour_Red = 0,
    eRbColour_Black
} eRbColour;


template< typename TypeKey, typename TypeContainer >
class CTreeNodeRb{
public:    

    typedef int (*FuncRbCompareKey)( TypeKey KeyA, TypeKey KeyB );

    CTreeNodeRb( TypeKey Key, TypeContainer * Container );
    CTreeNodeRb();

    CTreeNodeRb & operator=( const CTreeNodeRb & ) = delete;
    CTreeNodeRb( const CTreeNodeRb & ) = delete;

    bool GetKeyColour( TypeKey & Key, eRbColour & Colour ) const;

    static bool Insert( CTreeNodeRb * Tree, CTreeNodeRb * NodeInsert );
    static bool Remove( CTreeNodeRb * NodeInsert );
    static bool Find( CTreeNodeRb * Tree, TypeKey KeyFind, CTreeNodeRb * & NodeFound );
    static bool Maximum( CTreeNodeRb * Tree, CTreeNodeRb * & NodeFound );
    static bool MaximumLocal( CTreeNodeRb * Tree, CTreeNodeRb * & NodeFound );
    static bool Minimum( CTreeNodeRb * Tree, CTreeNodeRb * & NodeFound );
    static bool MinimumLocal( CTreeNodeRb * Tree, CTreeNodeRb * & NodeFound );
    static bool RotateLeft( CTreeNodeRb * Tree );
    static bool RotateRight( CTreeNodeRb * Tree );
    static bool ConvertToRoot( CTreeNodeRb * Node );
    static bool GetRoot( CTreeNodeRb * Node, CTreeNodeRb * & NodeRoot );
    static bool GetKeysSubTreeInOrder( CTreeNodeRb * Tree, vector< pair<TypeKey, eRbColour> > & KeysResult );

    static bool SetFuncRbCompareKey( FuncRbCompareKey Func );    
    
    static bool SetContainer( CTreeNodeRb * Node, TypeContainer * Container );
    static bool GetContainer( CTreeNodeRb * Node, TypeContainer * & Container );

private:
    CTreeNodeRb * _Parent;
    CTreeNodeRb * _Left;
    CTreeNodeRb * _Right;
    eRbColour _Colour;
    TypeKey _Key;
    TypeContainer * _Container;
    static FuncRbCompareKey _FuncRbCompareKey;
    static bool FixUpInsert( CTreeNodeRb * NodeFix );
    static bool FixUpRemove( CTreeNodeRb * NodeFix );
    static bool Transplant( CTreeNodeRb * NodeOriginal, CTreeNodeRb * NodeReplace );
};

template< typename TypeKey, typename TypeContainer >
typename CTreeNodeRb< TypeKey, TypeContainer >::FuncRbCompareKey CTreeNodeRb< TypeKey, TypeContainer >::_FuncRbCompareKey = 0;

template< typename TypeKey, typename TypeContainer >
CTreeNodeRb< TypeKey, TypeContainer >::CTreeNodeRb( TypeKey Key, TypeContainer * Container ): _Parent(0), _Left(0), _Right(0), _Colour(eRbColour_Red), _Key(Key), _Container(Container) {
}

template< typename TypeKey, typename TypeContainer >
CTreeNodeRb< TypeKey, TypeContainer >::CTreeNodeRb(): _Parent(0), _Left(0), _Right(0), _Colour(eRbColour_Red), _Key(0), _Container(0) {
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::GetKeyColour( TypeKey & Key, eRbColour & Colour ) const {
    Key = _Key;
    Colour = _Colour;
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::Insert( CTreeNodeRb * Tree, CTreeNodeRb * NodeInsert ){
    if( !Tree ){
        return false;
    }

    CTreeNodeRb * current;
    bool bFoundRoot = GetRoot( Tree, current );
    if( !bFoundRoot ){
        return false;
    }
    
    TypeKey KeyFind = NodeInsert->_Key;

    NodeInsert->_Colour = eRbColour_Red;

    while( current ){ 
        if( 0 < _FuncRbCompareKey( KeyFind, current->_Key ) ){
            if( !current->_Left ){
                NodeInsert->_Parent = current;
                current->_Left = NodeInsert;
                current = current->_Left;
                break;
            }else{
                current = current->_Left;
            }
        }else if( 0 > _FuncRbCompareKey( KeyFind, current->_Key ) ){
            if( !current->_Right ){
                NodeInsert->_Parent = current;
                current->_Right = NodeInsert;
                current = current->_Right;
                break;
            }else{
                current = current->_Right;
            }
        }else{
            current->_Key = NodeInsert->_Key;
            current->_Container = NodeInsert->_Container;
            break;
        }
    }

    FixUpInsert( current );

    return true; 
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::Remove( CTreeNodeRb * NodeRemove ){
    if( !NodeRemove ){
        return false;
    }

    eRbColour original_colour = NodeRemove->_Colour;
    CTreeNodeRb * NodeChild;
    CTreeNodeRb * NodeMove = NodeRemove;

    if( !NodeRemove->_Left ){ //single child in NodeRemove
        NodeChild = NodeRemove->_Right;
        return Transplant( NodeRemove, NodeChild );
    }else if( !NodeRemove->_Right ){ //single child in NodeRemove
        NodeChild = NodeRemove->_Left;
        return Transplant( NodeRemove, NodeChild );
    }else{ //2 children in NodeRemove
        MinimumLocal( NodeRemove->_Right, NodeMove );
        original_colour = NodeMove->_Colour;
        NodeChild = NodeMove->_Right;

        Transplant( NodeMove, NodeMove->_Right ); //replace NodeMove with its right child
        if( NodeMove->_Parent != NodeRemove ){
            NodeMove->_Right = NodeRemove->_Right;
        }

        if( NodeMove->_Right ){
            NodeMove->_Right->_Parent = NodeMove;
        }
        Transplant( NodeRemove, NodeMove ); //replace NodeRemove with NodeMove
        NodeMove->_Left = NodeRemove->_Left;
        if( NodeMove->_Left ){
            NodeMove->_Left->_Parent = NodeMove;
        }
        NodeMove->_Colour = NodeRemove->_Colour;
    }

    if( eRbColour_Black == original_colour ){
        FixUpRemove( NodeChild );
    }

    return true; 
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::FixUpInsert( CTreeNodeRb * NodeFix ){

    CTreeNodeRb * current = NodeFix;
    if( !current ){
        return false;
    }

    while( current->_Parent && eRbColour_Red == current->_Parent->_Colour){
        CTreeNodeRb * parent = current->_Parent; 
        if( parent->_Parent && parent == parent->_Parent->_Left )
        {
            CTreeNodeRb * parent_sibling = current->_Parent->_Parent->_Right;
            if( parent_sibling && parent_sibling->_Colour == eRbColour_Red ){
                parent->_Parent->_Colour = eRbColour_Red;
                parent->_Colour = eRbColour_Black;
                parent_sibling->_Colour = eRbColour_Black;
                current = current->_Parent->_Parent;
            }else{
                if ( current == parent->_Right ){
                    current = parent;
                    RotateLeft( current );
                }
                current->_Parent->_Colour = eRbColour_Black;
                current->_Parent->_Parent->_Colour = eRbColour_Red;
                RotateRight( current->_Parent->_Parent );
            }
        }else if( parent->_Parent && parent == parent->_Parent->_Right )
        {
            CTreeNodeRb * parent_sibling = current->_Parent->_Parent->_Left;
            if( parent_sibling && parent_sibling->_Colour == eRbColour_Red ){
                parent->_Parent->_Colour = eRbColour_Red;
                parent->_Colour = eRbColour_Black;
                parent_sibling->_Colour = eRbColour_Black;
                current = current->_Parent->_Parent;
            }else{
                if ( current == parent->_Left ){
                    current = parent;
                    RotateRight( current );
                }
                current->_Parent->_Colour = eRbColour_Black;
                current->_Parent->_Parent->_Colour = eRbColour_Red;
                RotateLeft( current->_Parent->_Parent );
            }
        }else{
            break;            
        }
    }
    
    CTreeNodeRb * NodeRoot;
    if( GetRoot( current, NodeRoot ) ){
        NodeRoot->_Colour = eRbColour_Black;
    }

    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::FixUpRemove( CTreeNodeRb * NodeFix ){
    
    CTreeNodeRb * current;

    while( current->_Parent && eRbColour_Black == current->_Colour ){
        if( current->_Parent->_Left == current ){
            CTreeNodeRb * NodeSibling;
            NodeSibling = current->_Parent->_Right;
            if( !NodeSibling ){
                GetRoot( current, current );
            }
            if( eRbColour_Red == NodeSibling->_Colour ){
                NodeSibling->_Colour = eRbColour_Black;
                current->_Parent->_Colour = eRbColour_Red;
                RotateLeft( current->_Parent );
                NodeSibling = current->_Parent->_Right;
            }
            if( eRbColour_Black == NodeSibling->_Left->_Colour && eRbColour_Black == NodeSibling->_Right->_Colour ){
                NodeSibling->_Colour = eRbColour_Red;
                current = current->_Parent;
            }else{
                if( eRbColour_Black == NodeSibling->_Right->_Colour ){
                    NodeSibling->_Left->_Colour = eRbColour_Black;
                    NodeSibling->_Colour = eRbColour_Red;
                    RotateRight( NodeSibling );
                    NodeSibling = current->_Parent->_Right;
                }
                NodeSibling->_Colour = current->_Parent->_Colour;
                current->_Parent->_Colour= eRbColour_Black;
                NodeSibling->_Right->_Colour = eRbColour_Black;
                RotateLeft( current->_Parent );
                GetRoot( current, current );
            }
        }else{
            CTreeNodeRb * NodeSibling;
            NodeSibling = current->_Parent->_Left;
            if( !NodeSibling ){
                GetRoot( current, current );
            }
            if( eRbColour_Red == NodeSibling->_Colour ){
                NodeSibling->_Colour = eRbColour_Black;
                current->_Parent->_Colour = eRbColour_Red;
                RotateRight( current->_Parent );
                NodeSibling = current->_Parent->_Left;
            }
            if( eRbColour_Black == NodeSibling->_Right->_Colour && eRbColour_Black == NodeSibling->_Left->_Colour ){
                NodeSibling->_Colour = eRbColour_Red;
                current = current->_Parent;
            }else{
                if( eRbColour_Black == NodeSibling->_Left->_Colour ){
                    NodeSibling->_Right->_Colour = eRbColour_Black;
                    NodeSibling->_Colour = eRbColour_Red;
                    RotateLeft( NodeSibling );
                    NodeSibling = current->_Parent->_Left;
                }
                NodeSibling->_Colour = current->_Parent->_Colour;
                current->_Parent->_Colour= eRbColour_Black;
                NodeSibling->_Left->_Colour = eRbColour_Black;
                RotateRight( current->_Parent );
                GetRoot( current, current );
            }
        }        
    } 
    current->_Colour = eRbColour_Black;

    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::Transplant( CTreeNodeRb * NodeOriginal, CTreeNodeRb * NodeReplace ){
    if( !NodeOriginal ){
        return false;
    }

    if( NodeOriginal->_Parent ){
        if( NodeOriginal->_Parent->_Left == NodeOriginal ){
            NodeOriginal->_Parent->_Left = NodeReplace;
        }else{
            NodeOriginal->_Parent->_Right = NodeReplace;
        }
    }
    if( NodeReplace ){
        NodeReplace->_Parent = NodeOriginal->_Parent;
    }
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::Find( CTreeNodeRb * Tree, TypeKey KeyFind, CTreeNodeRb * & NodeFound ){
    if( !Tree || !_FuncRbCompareKey ){
        return false;
    }
    
    CTreeNodeRb * current;
    bool bFoundRoot = GetRoot( Tree, current );
    if( !bFoundRoot ){
        return false;
    }

    while( current ){ 
        if( 0 < _FuncRbCompareKey( KeyFind, current->_Key ) ){
            current = current->_Left;
        }else if( 0 > _FuncRbCompareKey( KeyFind, current->_Key ) ){
            current = current->_Right;
        }else{
            NodeFound = current;
            return true;
        }
    }

    return false; 
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::Maximum( CTreeNodeRb * Tree, CTreeNodeRb * & NodeFound ){
    if( !Tree ){
        return false;
    }

    CTreeNodeRb * current;
    bool bFoundRoot = GetRoot( Tree, current );
    if( !bFoundRoot ){
        return false;
    }

    return MaximumLocal( current, NodeFound ); 
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::MaximumLocal( CTreeNodeRb * Tree, CTreeNodeRb * & NodeFound ){
    if( !Tree ){
        return false;
    }

    CTreeNodeRb * current = Tree;
    while( current->_Right ){
        current = current->_Right;
    }
    NodeFound = current;

    return true; 
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::Minimum( CTreeNodeRb * Tree, CTreeNodeRb * & NodeFound ){
    if( !Tree ){
        return false;
    }

    CTreeNodeRb * current;
    bool bFoundRoot = GetRoot( Tree, current );
    if( !bFoundRoot ){
        return false;
    }

    return MinimumLocal( current, NodeFound );
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::MinimumLocal( CTreeNodeRb * Tree, CTreeNodeRb * & NodeFound ){
    if( !Tree ){
        return false;
    }

    CTreeNodeRb * current = Tree;
    while( current->_Left ){
        current = current->_Left;
    }
    NodeFound = current;

    return true;     
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::RotateLeft( CTreeNodeRb * Tree ){
    if( !Tree ){
        return false;
    }

    CTreeNodeRb * y = Tree->_Right;
    if( !y ){
        return true;
    }

    Tree->_Right = y->_Left;
    y->_Left = Tree;
    if( Tree->_Right){
        Tree->_Right->_Parent = Tree;
    }

    y->_Parent = Tree->_Parent; //exchange parent
    if( Tree->_Parent ){
        if( Tree->_Parent->_Left == Tree ){ 
            Tree->_Parent->_Left = y;
        }else{
            Tree->_Parent->_Right = y;
        }
    }
    Tree->_Parent = y;

    return true; 
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::RotateRight( CTreeNodeRb * Tree ){
    if( !Tree ){
        return false;
    }

    CTreeNodeRb * y = Tree->_Left;
    if( !y ){
        return true;
    }

    Tree->_Left = y->_Right;
    y->_Right = Tree;
    if( Tree->_Left){
        Tree->_Left->_Parent = Tree;
    }

    y->_Parent = Tree->_Parent; //exchange parent
    if( Tree->_Parent ){
        if( Tree->_Parent->_Right == Tree ){ 
            Tree->_Parent->_Right = y;
        }else{
            Tree->_Parent->_Left = y;
        }
    }
    Tree->_Parent = y;

    return true; 
}
template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::ConvertToRoot( CTreeNodeRb * Node ){
    if( !Node ){
        return false;
    }

    if( Node->_Parent ){ //unlink parent
        if( Node->_Parent->_Left == Node ){
            Node->_Parent->_Left = 0;
        }else{
            Node->_Parent->_Right = 0;
        }
        Node->_Parent = 0;
    }

    Node->_Colour = eRbColour_Black;

    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::GetRoot( CTreeNodeRb * Node, CTreeNodeRb * & NodeRoot ){
    if( !Node ){
        return false;
    }
    CTreeNodeRb * current = Node;
    while( current->_Parent ){
        current = current->_Parent;
    }
    NodeRoot = current;
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::GetKeysSubTreeInOrder( CTreeNodeRb * Tree, vector< pair<TypeKey, eRbColour> > & KeysResult ){
    if( !Tree || !_FuncRbCompareKey ){
        return false;
    }
    GetKeysSubTreeInOrder( Tree->_Left, KeysResult );
    eRbColour CurrentColour = Tree->_Colour;
    TypeKey CurrentKey = Tree->_Key;
    pair<TypeKey, eRbColour> CurrentPair( CurrentKey, CurrentColour );
    KeysResult.push_back( CurrentPair );
    GetKeysSubTreeInOrder( Tree->_Right, KeysResult );

    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::SetFuncRbCompareKey( FuncRbCompareKey Func ){
    _FuncRbCompareKey = Func;
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::SetContainer( CTreeNodeRb * Node, TypeContainer * Container ){
    if( !Node ){
        return false;
    }
    Node->_Container = Container;
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool CTreeNodeRb< TypeKey, TypeContainer >::GetContainer( CTreeNodeRb * Node, TypeContainer * & Container ){
    if( !Node ){
        return false;
    }
    Container = Node->_Container;
    return true;
}
   
#endif
