#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>

using namespace std;

template< typename TypeKey, typename TypeVal >
class tree_node_avl{
public:
    static bool find( tree_node_avl * tree, TypeKey key, tree_node_avl * & found );
    static bool get_root( tree_node_avl * node, tree_node_avl * & root );
    static bool insert( tree_node_avl * tree, tree_node_avl * node_insert );    
    static bool remove( tree_node_avl * & root, tree_node_avl * node_remove );
    static bool next( tree_node_avl * node, tree_node_avl * & next );
    static bool prev( tree_node_avl * node, tree_node_avl * & prev );
    static bool left_descendent( tree_node_avl * node, tree_node_avl * & descendent );
    static bool right_descendent( tree_node_avl * node, tree_node_avl * & descendent );
    static bool left_ancestor( tree_node_avl * node, tree_node_avl * & ancestor );
    static bool right_ancestor( tree_node_avl * node, tree_node_avl * & ancestor );
    
    tree_node_avl() : _parent( nullptr ), _left( nullptr ), _right( nullptr ), _height(1), _deleted(false) {}

    tree_node_avl( TypeKey key, TypeVal val ) : _parent( nullptr ), _left( nullptr ), _right( nullptr ), _height(1), _deleted(false) {
	_key = key;
	_val = val;
    }
    TypeKey get_key() const;
    TypeVal get_val() const;
private:
    // static void rebalance( tree_node_avl * Node );
    static bool link_left( tree_node_avl * node_parent, tree_node_avl * node_left );
    static bool link_right( tree_node_avl * node_parent, tree_node_avl * node_right );
    tree_node_avl * _parent;
    tree_node_avl * _left;
    tree_node_avl * _right;
    int _height;
    TypeKey _key;
    TypeVal _val;
    bool _deleted;
};

template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::find( tree_node_avl * tree, TypeKey key, tree_node_avl * & found ){
    found = nullptr;
    while( tree ){
	if( key == tree->_key ){
	    found = tree;
	    return true;
	}else if( key < tree->_key ){
	    tree = tree->_left;
	}else{
	    tree = tree->_right;
	}
    }
    return false;
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::get_root( tree_node_avl * node, tree_node_avl * & root ){
    root = nullptr;
    tree_node_avl * prev = nullptr;
    while( node ){
	prev = node;
	node = node->_parent;
    }
    if( !prev ){
	return false;
    }else{
	root = prev;
	return true;
    }
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::insert( tree_node_avl * tree, tree_node_avl * node_insert ){
    tree_node_avl * prev = nullptr;
    while( tree ){
	if( node_insert->_key == tree->_key ){ //already exist
	    return false;
	}else if( node_insert->_key < tree->_key ){
	    prev = tree;
	    if( tree->_left ){ //recurse
		tree = tree->_left;
	    }else{ //attach node as leaf
		tree->_left = node_insert;
		if( node_insert )
		    node_insert->_parent = tree;
		return true;
	    }
	}else{
	    prev = tree;
	    if( tree->_right ){ //recurse
		tree = tree->_right;		
	    }else{ //attach node as leaf
		tree->_right = node_insert;
		if( node_insert )
		    node_insert->_parent = tree;
		return true;
	    }
	}
    }
    return false; //tree invalid
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal>::remove( tree_node_avl * & root, tree_node_avl * node_remove ){
    if( nullptr == node_remove ){
	return false;
    }
    if( nullptr == node_remove->_right ){ //node to delete has no right child
	if( nullptr == node_remove->_parent ){ //node to delete is root?
	    if( root != node_remove ){ //root invalid
		return false;
	    }else{ //promote node left child to root
		node_remove->_deleted = true;
		root = node_remove->_left;
		root->_parent = nullptr;
		return true;
	    }
	}else{ //node to delete is not root
	    if( node_remove->_parent->_left == node_remove ){
		link_left( node_remove->_parent, node_remove->_left );
	    }else{
		link_right( node_remove->_parent, node_remove->_left );
	    }
	    node_remove->_deleted = true;
	    return true;
	}
    }else{ //node to delete has right child
	tree_node_avl * next_node;
        if( !next( node_remove, next_node ) ){ //get next node
	    return false;
	}
	next_node = nullptr; //should be null
	if( nullptr == node_remove->_parent ){ //node to delete is root?
	    if( root != node_remove ){ //root invalid
		return false;
	    }else{
		//replace node(root) with next_node
		root = next_node;
		//promote next_node right child
		if( next_node->_parent->_left == next_node ){
		    link_left( next_node->_parent, next_node->_right );
		}else{
		    link_right( next_node->_parent, next_node->_right );
		}
		root->_parent = nullptr;
		node_remove->_deleted = true;
		return true;
	    }
	}else{ //node to delete is not root
	    //promote next_node right child
	    if( next_node->_parent->_left == next_node ){
		link_left( next_node->_parent, next_node->_right );
	    }else{
		link_right( next_node->_parent, next_node->_right );
	    }
	    //replace node to delete with next_node
	    if( node_remove->_parent->_left == node_remove ){
		link_left( node_remove->_parent, next_node );
	    }else{
		link_right( node_remove->_parent, next_node );
	    }
	    node_remove->_deleted = true;
	    return true;
	}
    }
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::next( tree_node_avl * node, tree_node_avl * & next ){
    next = nullptr;
    if( nullptr == node ){
	return false;
    }else{
	if( nullptr != node->_right ){
	    return left_descendent( node->_right, next );
	}else{
	    return right_ancestor( node, next );
	}
    }
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::prev( tree_node_avl * node, tree_node_avl * & prev ){
    prev = nullptr;
    if( nullptr == node ){
	return false;
    }else{
	if( nullptr != node->_left ){
	    return right_descendent( node->_left, prev );
	}else{
	    return left_ancestor( node, prev );
	}
    }
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::left_descendent( tree_node_avl * node, tree_node_avl * & descendent ){
    descendent = nullptr;
    if( nullptr == node ){ //not found
	return false;
    }
    while( node->_left ){ //recurse
	node = node->_left;
    }
    descendent = node; //found
    return true;
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::right_descendent( tree_node_avl * node, tree_node_avl * & descendent ){
    descendent = nullptr;
    if( nullptr == node ){ //not found
	return false;
    }
    while( node->_right ){ //recurse
	node = node->_right;
    }
    descendent = node; //found
    return true;
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::left_ancestor( tree_node_avl * node, tree_node_avl * & ancestor ){
    ancestor = nullptr;
    tree_node_avl * current = node;
    while( current && current->_parent ){
	if( current == current->_parent->_right ){ //found
	    ancestor = current->_parent;
	    return true;
	}else{ //recurse
	    current = current->_parent;
	}
    }
    return false; //not found    
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::right_ancestor( tree_node_avl * node, tree_node_avl * & ancestor ){
    ancestor = nullptr;
    tree_node_avl * current = node;
    while( current && current->_parent ){
	if( current == current->_parent->_left ){ //found
	    ancestor = current->_parent;
	    return true;
	}else{ //recurse
	    current = current->_parent;
	}
    }
    return false; //not found
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::link_left( tree_node_avl * node_parent, tree_node_avl * node_left ){
    if( node_parent ){
	node_parent->_left = node_left;
    }
    if( node_left ){
	node_left->_parent = node_parent;
    }
    return true;
}
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::link_right( tree_node_avl * node_parent, tree_node_avl * node_right ){
    if( node_parent ){
	node_parent->_right = node_right;
    }
    if( node_right ){
	node_right->_parent = node_parent;
    }
    return true;
}
template< typename TypeKey, typename TypeVal >
TypeKey tree_node_avl< TypeKey, TypeVal >::get_key() const {
    return _key;
}
template< typename TypeKey, typename TypeVal >
TypeVal tree_node_avl< TypeKey, TypeVal >::get_val() const {
    return _val;
}

#endif
