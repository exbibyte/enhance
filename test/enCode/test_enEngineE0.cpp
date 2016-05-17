#define CATCH_CONFIG_MAIN  // This tells Cactch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enEngineE0.h"
#include "enComponentType.h"

using namespace std;


class TestClassA{
public:
    static enComponentType _component_type;
    int _val;
    TestClassA() : _val(1) {}
};
enComponentType TestClassA::_component_type = enComponentType::FRONTEND;

class TestClassB{
public:
    static enComponentType _component_type;
    int _val;
    TestClassB() : _val(2) {}
};
enComponentType TestClassB::_component_type = enComponentType::BACKEND;

TEST_CASE( "EnEngineE0", "[EnEngineE0]" ) {
    SECTION( "RegisterComponent, GetComponent" ) {

	enEngineE0 engine;
	bool bRet = engine.initialize();
	CHECK( bRet );

	CHECK( engine._core_engine._components.size() == 7 );

	enFrontend0 * component_frontend = engine._core_engine.GetComponent<enFrontend0>( enFrontend0::_component_type );
	CHECK( component_frontend == &engine._frontend_engine );
	
        enBackend0 * component_backend = engine._core_engine.GetComponent<enBackend0>( enBackend0::_component_type );
	CHECK( component_backend == &engine._backend_engine );
    }
}
