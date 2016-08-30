#include "allocpool.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;

class A {
public:
    A(){
	cout << "A constructed." << endl;
    }
    ~A(){
	cout << "A destructed." << endl;
    }
    int blah( int a ){
	return a;
    }
    int x;
    int y;
    int z;
};

class B {
public:
    B(){
	cout << "B constructed." << endl;
    }
    B( int a, int b, int c ){
	cout << "B constructed with arguments: " << a << ", " << b << ", " << c << endl;	
    }
    ~B(){
	cout << "B destructed." << endl;
    }
    char data[200'000'000];
};

int main(){
    allocpool _allocpool;
    
    B* b = _allocpool.pool_new<B>( 6, 7, 8 );
    cout << "alloc pool loading: " << _allocpool.get_loading() << endl;
    if( b ){
	for( auto & i : b->data ){
	    i = 5;
	}
	allocpool::pool_delete(b);
    }
    cout << "alloc pool loading: " << _allocpool.get_loading() << endl;
    
    B * b2 = new(&_allocpool) B;
    if( b2 ){
	for( auto & i : b2->data ){
	    i = 5;
	}
	b2->~B();
	_allocpool.pool_free(b2);
    }
    cout << "alloc pool loading: " << _allocpool.get_loading() << endl;
    try{
	B * b3 = new(&_allocpool) B;
	cout << "alloc pool loading: " << _allocpool.get_loading() << endl;
	cout << "size of class data: " << sizeof(b3->data) << endl;
	for( auto & i : b3->data ){
	    i = 5;
	}
	if( b3 ){	
	    b3->~B();
	    operator delete( b3, &_allocpool );
	}
    }catch(...){
	cout << "allocation error caught." << endl;
    }
    
    _allocpool.dereserve();
    return 0;
}
