#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "MatrixMath.h"
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::mat4;

#include <iostream>
using namespace std;

TEST_CASE( "perspective", "[perspective]" ) {

    float fovy = 60.0;
    float aspect = 1.0;
    float near = 0.1;
    float far = 1000;
    
    float perspective[16];
    MatrixMath::Perspective( fovy, aspect, near, far, perspective );

    mat4 perspective_glm = glm::perspective( fovy, aspect, near, far );
    float * perspective_check = nullptr;
    perspective_check = glm::value_ptr( perspective_glm );

    for( int i = 0; i < 16; ++i ){
	REQUIRE( perspective_check );
	cout << i << ": " << perspective[i] << ", check: " << *perspective_check << endl;
	CHECK( ( perspective[i] > (*perspective_check - 0.002 ) ) );
	CHECK( ( perspective[i] < (*perspective_check + 0.002 ) ) );
	perspective_check++;
    }
}
