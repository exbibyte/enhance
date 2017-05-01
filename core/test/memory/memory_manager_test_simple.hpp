#ifndef E2_MEMORY_MANAGER_TEST_SIMPLE_HPP
#define E2_MEMORY_MANAGER_TEST_SIMPLE_HPP

#include <cassert>

class memory_manager_test_simple {
public:
    template< class MemoryManager, class TypeObjAlloc >
    static void test_newing( MemoryManager * mp, TypeObjAlloc *** n ){
	assert( nullptr != n );
	*n = mp-> template newing<TypeObjAlloc>();
	assert( nullptr != *n );
	assert( sizeof( ***n ) == sizeof( TypeObjAlloc ) );
    }

    template< class MemoryManager, class TypeObjAlloc >
    static bool test_deleting( MemoryManager * mp, TypeObjAlloc ** n ){
	//proper delete should cause no exceptions and failures
	bool ret = mp->deleting( n );
	return ret;
    }
};

#endif
