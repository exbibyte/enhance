#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>

using namespace std;

template< typename TypeKey, typename TypeVal >
class tree_node_avl{
public:
    static bool find( tree_node_avl * tree, TypeKey key, tree_node_avl * & found );
    static bool get_root( tree_node_avl * node, tree_node_avl * & root );
    static bool insert( tree_node_avl * & root, tree_node_avl * node_insert );    
    static bool remove( tree_node_avl * & root, tree_node_avl * node_remove );
    static bool next( tree_node_avl * node, tree_node_avl * & next );
    static bool prev( tree_node_avl * node, tree_node_avl * & prev );
    tree_node_avl() : _parent( nullptr ), _left( nullptr ), _right( nullptr ), _height(1), _deleted(false) {}
    tree_node_avl( TypeKey key, TypeVal val ) : _parent( nullptr ), _left( nullptr ), _right( nullptr ), _height(1), _deleted(false) {
	_key = key;
	_val = val;
    }
    TypeKey get_key() const;
    TypeVal get_val() const;
private:
    static void rebalance( tree_node_avl * & root, tree_node_avl * Node );
    static void rebalance_left( tree_node_avl * & root, tree_node_avl * node );
    static void rebalance_right( tree_node_avl * & root, tree_node_avl * node );
    static bool link_left( tree_node_avl * node_parent, tree_node_avl * node_left );
    static bool link_right( tree_node_avl * node_parent, tree_node_avl * node_right );
    static bool left_descendent( tree_node_avl * node, tree_node_avl * & descendent );
    static bool right_descendent( tree_node_avl * node, tree_node_avl * & descendent );
    static bool left_ancestor( tree_node_avl * node, tree_node_avl * & ancestor );
    static bool right_ancestor( tree_node_avl * node, tree_node_avl * & ancestor );
    static bool update_height( tree_node_avl * node );
    static void rotate_left( tree_node_avl * & root, tree_node_avl * node );
    static void rotate_right( tree_node_avl * & root, tree_node_avl * node );
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
bool tree_node_avl< TypeKey, TypeVal >::insert( tree_node_avl * & root, tree_node_avl * node_insert ){
    tree_node_avl * prev = nullptr;
    tree_node_avl * tree = root;
    while( tree ){
	if( node_insert->_key == tree->_key ){ //already exist
	    return false;
	}else if( node_insert->_key < tree->_key ){
	    prev = tree;
	    if( tree->_left ){ //recurse
		tree = tree->_left;
	    }else{ //attach node as leaf
		tree->_left = node_insert;
		if( node_insert ){
		    node_insert->_parent = tree;
		}
		//avl specific routine
		rebalance( root, node_insert );
		return true;
	    }
	}else{
	    prev = tree;
	    if( tree->_right ){ //recurse
		tree = tree->_right;		
	    }else{ //attach node as leaf
		tree->_right = node_insert;
		if( node_insert ){
		    node_insert->_parent = tree;
		}
		//avl specific routine
		rebalance( root, node_insert );
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
		//avl specific routine
		update_height( root );
		return true;
	    }
	}else{ //node to delete is not root
	    if( node_remove->_parent->_left == node_remove ){
		link_left( node_remove->_parent, node_remove->_left );
	    }else{
		link_right( node_remove->_parent, node_remove->_left );
	    }
	    node_remove->_deleted = true;
	    //avl specific routine
	    update_height( node_remove->_parent );
	    return true;
	}
    }else{ //node to delete has right child
	tree_node_avl * next_node;
        if( !next( node_remove, next_node ) ){ //get next node
	    return false;
	}
	// next_node->_left = nullptr; //should be null anyways
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
		//avl specific routine
		update_height( root );
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
	    //avl specific routine
	    update_height( node_remove->_parent );
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
void tree_node_avl< TypeKey, TypeVal >::rebalance( tree_node_avl * & root, tree_node_avl * node ){
    if( nullptr == node ){
	return;
    }else{
	tree_node_avl * parent = node->_parent;
	int height_left = 0;
	int height_right = 0;
	if( node->_left ){
	    height_left = node->_left->_height;
	}
	if( node->_right ){
	    height_right = node->_right->_height;
	}
	if( height_left > height_right + 1 ){
	    rebalance_right( root, node );
	}
	if( height_right > height_left + 1 ){
	    rebalance_left( root, node );
	}
	update_height( node );
	if( nullptr != parent ){
	    rebalance( root, parent );
	}
    }
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
template< typename TypeKey, typename TypeVal >
bool tree_node_avl< TypeKey, TypeVal >::update_height( tree_node_avl * node ){
    if( nullptr == node ){
	return false;
    }else{
	int height_left = node->_left ? node->_left->_height : 0;
	int height_right = node->_right ? node->_right->_height : 0;
	int height_max = height_left > height_right ? height_left : height_right;
	node->_height = height_max + 1;
	return true;
    }
}
template< typename TypeKey, typename TypeVal >
void tree_node_avl< TypeKey, TypeVal >::rebalance_right( tree_node_avl * & root, tree_node_avl * node ){
    if( nullptr == node || nullptr == node->_left ){
	return;
    }else{
	int height_right = node->_left->_right ? node->_left->_right->_height : 0;
	int height_left = node->_left->_left ? node->_left->_left->_height : 0;
	if( height_right > height_left ){
	    rotate_left( root, node->_left );
	}
	rotate_right( root, node );
	return;
    }
}
template< typename TypeKey, typename TypeVal >
void tree_node_avl< TypeKey, TypeVal >::rebalance_left( tree_node_avl * & root, tree_node_avl * node ){
    if( nullptr == node || nullptr == node->_right ){
	return;
    }else{
	int height_left = node->_right->_left ? node->_right->_left->_height : 0;
	int height_right = node->_right->_right ? node->_right->_right->_height : 0;
	if( height_left > height_right ){
	    rotate_right( root, node->_right );
	}
	rotate_left( root, node );
	return;
    }
}
template< typename TypeKey, typename TypeVal >
void tree_node_avl< TypeKey, TypeVal >::rotate_right( tree_node_avl * & root, tree_node_avl * node ){
    if( !node || !node->_left ){
	return;
    }else{
	tree_node_avl * parent = node->_parent;
	tree_node_avl * a = node->_left;
	tree_node_avl * b = a->_right;
	a->_parent = parent;
	if( parent && parent->_left == node ){
	    parent->_left = a;
	}
	else if( parent && parent->_right == node ){
	    parent->_right = a;
	}else{ //parent is root
	    root = a;
	}
	node->_parent = a;
	a->_right = node;
	if( b ){
	    b->_parent = node;
	}
	node->_left = b;
	update_height(b);
	update_height(node);
	update_height(a);
	return;
    }
}
template< typename TypeKey, typename TypeVal >
void tree_node_avl< TypeKey, TypeVal >::rotate_left( tree_node_avl * & root, tree_node_avl * node ){
    if( !node || !node->_right ){
	return;
    }else{
	tree_node_avl * parent = node->_parent;
	tree_node_avl * a = node->_right;
	tree_node_avl * b = a->_left;
	a->_parent = parent;
	if( parent && parent->_left == node ){
	    parent->_left = a;
	}
	else if( parent && parent->_right == node ){
	    parent->_right = a;
	}else{ //parent is root
	    root = a;
	}
	node->_parent = a;
	a->_left = node;
	if( b ){
	    b->_parent = node;
	}
	node->_right = b;
	update_height(b);
	update_height(node);
	update_height(a);
	return;
    }
}
#endif
