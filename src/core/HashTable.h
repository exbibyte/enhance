#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <iterator>
#include <functional>
#include <type_traits>
#include <random>
#include <cmath>
#include <iostream>

//class implementing universal hashing and chaining

//default template
template< class T, class V, class Enable = void >
class HashTable{};

//partial specialization for value type T that are integral in nature
template< class T >
using EnableForIntegral = typename std::enable_if< std::is_integral<T>::value>::type;

template< class T, class V >
class HashTable< T, V, EnableForIntegral<T> >
{
public:
    class HashNode {
    public:
	int _key;
	HashNode * _next = nullptr;
	HashNode * _prev = nullptr;
	V _val;
    };
    // template< class TypeVal >
    // class MyIterator : public std::iterator<std::input_iterator_tag, TypeVal >
    // {
    // 	TypeVal * p;
    // public:
    // 	MyIterator( TypeVal* x ) : p(x) {}
    // 	MyIterator( const MyIterator& mit ) : p( mit.p ) {}
    // 	MyIterator& operator++() { ++p; return *this; }
    // 	MyIterator operator++( int ) { MyIterator tmp( *this ); operator++(); return tmp; }
    // 	bool operator==( const MyIterator & rhs ) { return p==rhs.p; }
    // 	bool operator!=( const MyIterator & rhs ) { return p!=rhs.p; }
    // 	TypeVal & operator*() { return *p; }
    // };
    // using iterator = MyIterator< V >;
    // using const_iterator = MyIterator< const V >;
    ~HashTable();
    bool Insert( T, V );
    bool Find( T, V & );
    bool Erase( T );
    void AddHashFunc( std::function<int(int)> ); //add hash function into the univeral set
    void SetDefault();
    void ResizeTable( int size );
    size_t GetTableSize();
    bool ComputeHash( int key, int & hashed_val );
    bool SelectRandomHashFunc();
    void PrependHashNode( HashNode * & node, int key, V val );
    void RemoveHashNode( HashNode * & node );
    HashNode * FindHashNode( HashNode * node, int key );
private:
    std::vector< HashNode * > _table;
    V _dummy; //TODO
    std::vector<std::function<int(int)>> _funcs_hash; //family of hash functions to select from
    std::function<int(int)> _func_hash_selected; //selected hash function to be used
};

template< class T, class V >
HashTable<T, V, EnableForIntegral<T> >::~HashTable(){
    for( int i = 0; i < _table.size(); ++i ){
    	while( _table[i] ){
    	    RemoveHashNode( _table[i] );
    	}
    }
}
template< class T, class V >
void HashTable<T, V, EnableForIntegral<T> >::ResizeTable( int size ){
    //todo
    _table.resize(size);
}
template< class T, class V >
void HashTable<T, V, EnableForIntegral<T> >::AddHashFunc( std::function<int(int)> hash_func ){
    _funcs_hash.push_back( hash_func );
}
template< class T, class V >
size_t HashTable<T, V, EnableForIntegral<T> >::GetTableSize(){
    return _table.size();
}
template< class T, class V >
bool HashTable<T, V, EnableForIntegral<T> >::ComputeHash( int key, int & hashed_val ){
    hashed_val = _func_hash_selected( key );
    return true;
}
template< class T, class V >
bool HashTable<T, V, EnableForIntegral<T> >::Insert( T key, V value ){
    int key_converted = (int) key;
    int hashed_key;
    if( !ComputeHash( key_converted, hashed_key ) )
	return false;
    HashNode * & head = _table[ hashed_key ];
    HashNode * find = FindHashNode( head, key_converted );
    if( find ){
	find->_val = value;
    }else{
	PrependHashNode( head, key_converted, value );
    }
    return true;
}
template< class T, class V >
bool HashTable<T, V, EnableForIntegral<T> >::Find( T key, V & value ){
    int key_converted = (int) key;
    int hashed_key;
    if( !ComputeHash( key_converted, hashed_key ) )
	return false;
    HashNode * head = _table[ hashed_key ];
    HashNode * find = FindHashNode( head, key_converted );
    if( find ){
	value = find->_val;
	return true;
    }
    return false;
}
template< class T, class V >
bool HashTable<T, V, EnableForIntegral<T> >::Erase( T key ){
    int key_converted = (int) key;
    int hashed_key;
    if( !ComputeHash( key_converted, hashed_key ) )
	return false;
    HashNode * & head = _table[ hashed_key ];
    HashNode * find = FindHashNode( head, key_converted );
    if( find ){
	if( find == head ){
	    RemoveHashNode( head );
	}else{
	    RemoveHashNode( find );
	}
	return true;
    }
    return false;
}
template< class T, class V >
bool HashTable<T, V, EnableForIntegral<T> >::SelectRandomHashFunc(){
    if( 1 > _funcs_hash.size() ){
	return false;
    }
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> distribution(0, _funcs_hash.size()-1 );
    int selected = distribution(engine);
    _func_hash_selected = _funcs_hash[selected];
    return true;
}
template< class T, class V >
void HashTable<T, V, EnableForIntegral<T> >::SetDefault(){
    _funcs_hash.clear();
    unsigned int table_size = 199;
    ResizeTable( table_size );
    auto hash_1 = std::function<int(int)>([=](int key)->int{
	    return key % table_size;
	});
    auto hash_2 = std::function<int(int)>([=](int key)->int{
	    //source: https://gist.github.com/badboy/6267743
	    //hash32shiftmult
	    int c2 = 0x27d4eb2d; // a prime or an odd constant
	    int key2=  key;
	    key2 = (key2 ^ 61) ^ (key2 >> 16);
	    key2 = key2 + (key2 << 3);
	    key2 = key2 ^ (key2 >> 4);
	    key2 = key2 * c2;
	    key2 = key2 ^ (key2 >> 15);
	    return key2 % table_size;
	});
    auto hash_3 = std::function<int(int)>([=](int key)->int{
	    //source: https://gist.github.com/badboy/6267743
	    //Robert Jenkins' 32 bit
	    int a = key;
	    a = (a+0x7ed55d16) + (a<<12);
	    a = (a^0xc761c23c) ^ (a>>19);
	    a = (a+0x165667b1) + (a<<5);
	    a = (a+0xd3a2646c) ^ (a<<9);
	    a = (a+0xfd7046c5) + (a<<3);
	    a = (a^0xb55a4f09) ^ (a>>16);
	    return a % table_size;
	});
    AddHashFunc( hash_1 ); //add hash function into the univeral set
    AddHashFunc( hash_2 ); //add hash function into the univeral set
    AddHashFunc( hash_3 );  //add hash function into the univeral set
}
template< class T, class V >
void HashTable<T, V, EnableForIntegral<T> >::PrependHashNode( HashNode * & node, int key, V val ){
    HashNode * new_node = new HashNode;
    new_node->_key = key;
    new_node->_val = val;
    if( !node ){
	node = new_node;
    }else{
	new_node->_next = node;
	new_node->_prev = node->_prev;
	if( node->_prev ){
	    node->_prev->_next = new_node;
	    node->_prev = new_node;
	}else{
	    node = new_node; //head
	}
    }
}
template< class T, class V >
void HashTable<T, V, EnableForIntegral<T> >::RemoveHashNode( HashNode * & node ){
    if( !node )
	return;
    HashNode * node_prev = node->_prev;
    HashNode * node_next = node->_next;
    delete node;
    node = nullptr;
    if( node_next ){
	node_next->_prev = node_prev;
    }
    if( node_prev ){
	node_prev->_next = node_next;
    }
}
template< class T, class V >
typename HashTable<T, V, EnableForIntegral<T> >::HashNode * HashTable<T, V, EnableForIntegral<T> >::FindHashNode( HashNode * node, int key ){
    HashNode * current_node = node;
    while( current_node ){
	if( current_node->_key == key ){
	    return current_node;
	}else{
	    current_node = current_node->_next;
	}
    }
    return nullptr;
}
#endif
