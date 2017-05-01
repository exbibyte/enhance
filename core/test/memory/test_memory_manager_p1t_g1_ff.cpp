#include <list>
#include <cstdint>
#include <cassert>
#include <vector>

#include "memory_manager_test_simple.hpp"
#include "memory_manager_p1t_g1_ff.hpp"

using namespace e2::memory;

class node {
public:
    int a,b,c,d;
};

int main(){
    memory_manager_p1t_g1_ff mm;

    std::vector< node * > ns;
    for( int i = 0; i < 100; ++i ){
	node * n = nullptr;
	memory_manager_test_simple::test_newing( &mm, &n );
	assert( nullptr != n );

	n->a = 1;
	n->b = 2;
	n->c = 3;
	n->d = 4;
	ns.push_back( n );
    }

    for(auto & i : ns ){
	memory_manager_test_simple::test_deleting( &mm, i );	
    }

    return 0;
}
