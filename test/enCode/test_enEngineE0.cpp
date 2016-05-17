#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
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

	TestClassA instance_a;
	TestClassB instance_b;

	enEngineE0 engine;
	engine._core_engine.RegisterComponent( TestClassA::_component_type, &instance_a );
	engine._core_engine.RegisterComponent( TestClassB::_component_type, &instance_b );

	CHECK( engine._core_engine._components.size() == 2 );

	TestClassA * component_frontend = engine._core_engine.GetComponent<TestClassA>( TestClassA::_component_type );
	CHECK( component_frontend == &instance_a );
	
	TestClassB * component_backend = engine._core_engine.GetComponent<TestClassB>( TestClassB::_component_type );
	CHECK( component_backend == &instance_b );
    }
}
