#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enEngineCoreAbstract.h"
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

TEST_CASE( "EnEngineCoreAbstract", "[EnEngineCoreAbstract]" ) {
    SECTION( "RegisterComponent, GetComponent" ) {

	TestClassA instance_a;
	TestClassB instance_b;

	enEngineCoreAbstract engine_core;
	engine_core.RegisterComponent( TestClassA::_component_type, &instance_a );
	engine_core.RegisterComponent( TestClassB::_component_type, &instance_b );

	CHECK( engine_core._components.size() == 2 );

	TestClassA * component_frontend = engine_core.GetComponent<TestClassA>( TestClassA::_component_type );
	CHECK( component_frontend == &instance_a );
	
	TestClassB * component_backend = engine_core.GetComponent<TestClassB>( TestClassB::_component_type );
	CHECK( component_backend == &instance_b );
    }
}
