#ifndef HEAP_FIB_HPP
#define HEAP_FIB_HPP

#include <memory>

//fibonacci heap capable of holding items of TypeItem( default constructable ) along with integer key
template< class TypeItem >
class heap_fib {
public:
    class node {
    public:
	node() : _parent(nullptr), _left(nullptr), _right(nullptr), _degree(0), _marked(false), _key(0) {
	    _children.clear();
	}
	node * _parent;
	list<node * > _children;
	node * _left;
        node * _right;
	size_t _degree;
	bool _marked;
	int _key;
	TypeItem _item;
    };
    //mergeable heap interfaces
    static heap_fib heap_make();
    ~heap_fib();
    static heap_fib heap_union( heap_fib & a, heap_fib & b );
    bool insert( int key, TypeItem item );
    bool min_item( TypeItem & item );
    bool min_key( int & key );
    bool min_node( node const * & n );
    bool extract_min( int & key, TypeItem * & item );
    bool decrease_key( node * n, int new_key );
    void cut( node * old_child, node * old_parent );
    void cascading_cut( node * n );
    bool remove( node * n );
    int get_potential();
    void consolidate();
    void fib_heap_link( node * as_child, node * as_parent );
private:
    heap_fib();
    heap_fib( heap_fib const & a );
    heap_fib & operator=( heap_fib const & a );
    int get_num_root_trees();
    size_t get_num_marked_nodes();
    size_t get_num_nodes();
    void remove_branch_aux( node * n );
    void remove_branch( node * n );
    void reset();
    void link_siblings( std::list< node * > & l, node * n );
    void unlink_siblings( node * n );
    std::list< node * > _root_nodes;
    size_t _num_marked;
    size_t _num_nodes;
    node * _node_min;
};

#endif
