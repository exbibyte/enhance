#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <iterator>
#include <functional>
#include <type_traits>

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
    template< class TypeVal >
    class MyIterator : public std::iterator<std::input_iterator_tag, TypeVal >
    {
	TypeVal * p;
    public:
	MyIterator( TypeVal* x ) : p(x) {}
	MyIterator( const MyIterator& mit ) : p( mit.p ) {}
	MyIterator& operator++() { ++p; return *this; }
	MyIterator operator++( int ) { MyIterator tmp( *this ); operator++(); return tmp; }
	bool operator==( const MyIterator & rhs ) { return p==rhs.p; }
	bool operator!=( const MyIterator & rhs ) { return p!=rhs.p; }
	TypeVal & operator*() { return *p; }
    };
    using iterator = MyIterator< V >;
    using const_iterator = MyIterator< const V >;
    void Insert( T, V );
    iterator find( T );
    void AddHashFunc( std::function<int(int)> ){} //TODO
    void ResizeTable( int size );
private:
    std::vector<int> _table;
    int _size = 0;
    V _dummy; //TODO
    std::vector<std::function<int(int)>> _funcs_hash; //TODO
};
template< class T, class V >
void HashTable<T, V, EnableForIntegral<T>>::Insert( T key, V val ){
    int int_val = (int) val;
    _dummy = int_val;
}
template< class T, class V >
typename HashTable<T, V, EnableForIntegral<T>>::iterator HashTable<T, V, EnableForIntegral<T> >::find( T key ){
    return MyIterator< V >( &_dummy );
}
template< class T, class V >
void HashTable<T, V, EnableForIntegral<T> >::ResizeTable( int size ){
    _size = size;
    
}
#endif
