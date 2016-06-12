#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enEngineKernelAbstract.hpp"
#include "enComponentType.hpp"
#include "enComponentMeta.hpp"
#include "enComp_Clock.hpp"

using namespace std;


class TestClassA : public enComponentMeta {
public:
    int _val;
    TestClassA() : _val(1), enComponentMeta(enComponentType::LOGIC) {
    }
};

class TestClassB : public enComp_Clock {
public:
    int _val;
};

TEST_CASE( "EnEngineKernelAbstract", "[EnEngineKernelAbstract]" ) {
    SECTION( "RegisterComponent, GetComponent" ) {

	TestClassA instance_a;
	TestClassB instance_b;

	enEngineKernelAbstract engine_core;
	engine_core.register_component( &instance_a );
	engine_core.register_component( &instance_b );

	CHECK( engine_core.get_num_components() == 2 );

	TestClassA * component_frontend = dynamic_cast<TestClassA*>(engine_core.get_component(enComponentType::LOGIC));
	CHECK( component_frontend == &instance_a );
	
        enComp_Clock * component_clock = dynamic_cast<enComp_Clock*>(engine_core.get_component(enComponentType::CLOCK));
	CHECK( component_clock != nullptr );
    }
}
