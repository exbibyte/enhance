#ifndef TREE_RB_H
#define TREE_RB_H

#include <iostream>
#include <vector>
#include <utility>
 
typedef enum eRbColour {
    eRbColour_Red = 0,
    eRbColour_Black
} eRbColour;


template< typename TypeKey, typename TypeContainer >
class tree_node_rb{
public:    

    typedef int (*FuncRbCompareKey)( TypeKey KeyA, TypeKey KeyB );

    tree_node_rb( TypeKey Key, TypeContainer * Container );
    tree_node_rb();

    tree_node_rb & operator=( const tree_node_rb & ) = delete;
    tree_node_rb( const tree_node_rb & ) = delete;

    bool GetKeyColour( TypeKey & Key, eRbColour & Colour ) const;

    static bool Insert( tree_node_rb * Tree, tree_node_rb * NodeInsert );
    static bool Remove( tree_node_rb * NodeInsert );
    static bool Find( tree_node_rb * Tree, TypeKey KeyFind, tree_node_rb * & NodeFound );
    static bool Maximum( tree_node_rb * Tree, tree_node_rb * & NodeFound );
    static bool MaximumLocal( tree_node_rb * Tree, tree_node_rb * & NodeFound );
    static bool Minimum( tree_node_rb * Tree, tree_node_rb * & NodeFound );
    static bool MinimumLocal( tree_node_rb * Tree, tree_node_rb * & NodeFound );
    static bool RotateLeft( tree_node_rb * Tree );
    static bool RotateRight( tree_node_rb * Tree );
    static bool ConvertToRoot( tree_node_rb * Node );
    static bool GetRoot( tree_node_rb * Node, tree_node_rb * & NodeRoot );
    static bool GetKeysSubTreeInOrder( tree_node_rb * Tree, std::vector< std::pair<TypeKey, eRbColour> > & KeysResult );

    static bool SetFuncRbCompareKey( FuncRbCompareKey Func );    
    
    static bool SetContainer( tree_node_rb * Node, TypeContainer * Container );
    static bool GetContainer( tree_node_rb * Node, TypeContainer * & Container );

private:
    tree_node_rb * _Parent;
    tree_node_rb * _Left;
    tree_node_rb * _Right;
    eRbColour _Colour;
    TypeKey _Key;
    TypeContainer * _Container;
    static FuncRbCompareKey _FuncRbCompareKey;
    static bool FixUpInsert( tree_node_rb * NodeFix );
    static bool FixUpRemove( tree_node_rb * NodeFix );
    static bool Transplant( tree_node_rb * NodeOriginal, tree_node_rb * NodeReplace );
};

template< typename TypeKey, typename TypeContainer >
typename tree_node_rb< TypeKey, TypeContainer >::FuncRbCompareKey tree_node_rb< TypeKey, TypeContainer >::_FuncRbCompareKey = 0;

template< typename TypeKey, typename TypeContainer >
tree_node_rb< TypeKey, TypeContainer >::tree_node_rb( TypeKey Key, TypeContainer * Container ): _Parent(0), _Left(0), _Right(0), _Colour(eRbColour_Red), _Key(Key), _Container(Container) {
}

template< typename TypeKey, typename TypeContainer >
tree_node_rb< TypeKey, TypeContainer >::tree_node_rb(): _Parent(0), _Left(0), _Right(0), _Colour(eRbColour_Red), _Key(0), _Container(0) {
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::GetKeyColour( TypeKey & Key, eRbColour & Colour ) const {
    Key = _Key;
    Colour = _Colour;
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::Insert( tree_node_rb * Tree, tree_node_rb * NodeInsert ){
    if( !Tree ){
        return false;
    }

    tree_node_rb * current;
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
bool tree_node_rb< TypeKey, TypeContainer >::Remove( tree_node_rb * NodeRemove ){
    if( !NodeRemove ){
        return false;
    }

    eRbColour original_colour = NodeRemove->_Colour;
    tree_node_rb * NodeChild;
    tree_node_rb * NodeMove = NodeRemove;

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
bool tree_node_rb< TypeKey, TypeContainer >::FixUpInsert( tree_node_rb * NodeFix ){

    tree_node_rb * current = NodeFix;
    if( !current ){
        return false;
    }

    while( current->_Parent && eRbColour_Red == current->_Parent->_Colour){
        tree_node_rb * parent = current->_Parent; 
        if( parent->_Parent && parent == parent->_Parent->_Left )
        {
            tree_node_rb * parent_sibling = current->_Parent->_Parent->_Right;
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
            tree_node_rb * parent_sibling = current->_Parent->_Parent->_Left;
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
    
    tree_node_rb * NodeRoot;
    if( GetRoot( current, NodeRoot ) ){
        NodeRoot->_Colour = eRbColour_Black;
    }

    return true;
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::FixUpRemove( tree_node_rb * NodeFix ){
    
    tree_node_rb * current;

    while( current->_Parent && eRbColour_Black == current->_Colour ){
        if( current->_Parent->_Left == current ){
            tree_node_rb * NodeSibling;
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
            tree_node_rb * NodeSibling;
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
bool tree_node_rb< TypeKey, TypeContainer >::Transplant( tree_node_rb * NodeOriginal, tree_node_rb * NodeReplace ){
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
bool tree_node_rb< TypeKey, TypeContainer >::Find( tree_node_rb * Tree, TypeKey KeyFind, tree_node_rb * & NodeFound ){
    if( !Tree || !_FuncRbCompareKey ){
        return false;
    }
    
    tree_node_rb * current;
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
bool tree_node_rb< TypeKey, TypeContainer >::Maximum( tree_node_rb * Tree, tree_node_rb * & NodeFound ){
    if( !Tree ){
        return false;
    }

    tree_node_rb * current;
    bool bFoundRoot = GetRoot( Tree, current );
    if( !bFoundRoot ){
        return false;
    }

    return MaximumLocal( current, NodeFound ); 
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::MaximumLocal( tree_node_rb * Tree, tree_node_rb * & NodeFound ){
    if( !Tree ){
        return false;
    }

    tree_node_rb * current = Tree;
    while( current->_Right ){
        current = current->_Right;
    }
    NodeFound = current;

    return true; 
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::Minimum( tree_node_rb * Tree, tree_node_rb * & NodeFound ){
    if( !Tree ){
        return false;
    }

    tree_node_rb * current;
    bool bFoundRoot = GetRoot( Tree, current );
    if( !bFoundRoot ){
        return false;
    }

    return MinimumLocal( current, NodeFound );
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::MinimumLocal( tree_node_rb * Tree, tree_node_rb * & NodeFound ){
    if( !Tree ){
        return false;
    }

    tree_node_rb * current = Tree;
    while( current->_Left ){
        current = current->_Left;
    }
    NodeFound = current;

    return true;     
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::RotateLeft( tree_node_rb * Tree ){
    if( !Tree ){
        return false;
    }

    tree_node_rb * y = Tree->_Right;
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
bool tree_node_rb< TypeKey, TypeContainer >::RotateRight( tree_node_rb * Tree ){
    if( !Tree ){
        return false;
    }

    tree_node_rb * y = Tree->_Left;
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
bool tree_node_rb< TypeKey, TypeContainer >::ConvertToRoot( tree_node_rb * Node ){
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
bool tree_node_rb< TypeKey, TypeContainer >::GetRoot( tree_node_rb * Node, tree_node_rb * & NodeRoot ){
    if( !Node ){
        return false;
    }
    tree_node_rb * current = Node;
    while( current->_Parent ){
        current = current->_Parent;
    }
    NodeRoot = current;
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::GetKeysSubTreeInOrder( tree_node_rb * Tree, std::vector< std::pair<TypeKey, eRbColour> > & KeysResult ){
    if( !Tree || !_FuncRbCompareKey ){
        return false;
    }
    GetKeysSubTreeInOrder( Tree->_Left, KeysResult );
    eRbColour CurrentColour = Tree->_Colour;
    TypeKey CurrentKey = Tree->_Key;
    std::pair<TypeKey, eRbColour> CurrentPair( CurrentKey, CurrentColour );
    KeysResult.push_back( CurrentPair );
    GetKeysSubTreeInOrder( Tree->_Right, KeysResult );

    return true;
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::SetFuncRbCompareKey( FuncRbCompareKey Func ){
    _FuncRbCompareKey = Func;
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::SetContainer( tree_node_rb * Node, TypeContainer * Container ){
    if( !Node ){
        return false;
    }
    Node->_Container = Container;
    return true;
}

template< typename TypeKey, typename TypeContainer >
bool tree_node_rb< TypeKey, TypeContainer >::GetContainer( tree_node_rb * Node, TypeContainer * & Container ){
    if( !Node ){
        return false;
    }
    Container = Node->_Container;
    return true;
}
   
#endif
