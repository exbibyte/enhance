#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <iterator>
#include <functional>
#include <type_traits>

//default template
template< class T, class Enable = void >
class HashTable{};

//partial specialization for value type T that are integral in nature
template< class T >
using EnableForIntegral = typename std::enable_if< std::is_integral<T>::value>::type;

template< class T >
class HashTable< T, EnableForIntegral<T> >
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
	int& operator*() { return *p; }
    };
    using iterator = MyIterator< T >;
    using const_iterator = MyIterator< const T >;
    void Insert( T );
    iterator find( T );
    void AddHashFunc( std::function<int(int)> ){} //TODO
private:
    std::vector<int> _table;
    int _size;
    int _dummy = 99; //TODO
    std::vector<std::function<int(int)>> _funcs_hash; //TODO
};
template< class T >
void HashTable<T, EnableForIntegral<T>>::Insert( T val ){
    int int_val = (int) val;
}
template< class T >
typename HashTable<T, EnableForIntegral<T>>::iterator HashTable<T, EnableForIntegral<T> >::find( T val ){
    return MyIterator< T >( &_dummy );
}
#endif
