#include "allocpool.hpp"

#include <iostream>

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
    ~B(){
	cout << "B destructed." << endl;
    }
    char data[2000000];
};

int main(){
    allocpool _allocpool;
    
    // A * a = new(&_allocpool) A;
    // a->~A();
    // _allocpool.free(a);

    B * b = new(&_allocpool) B;
    if( b ){	
	b->~B();
	_allocpool.free(b);
    }

    B * b2 = new(&_allocpool) B;
    if( b2 ){	
	b2->~B();
	_allocpool.free(b2);
    }

    try{
	B * b3 = new(&_allocpool) B;
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
