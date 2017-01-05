#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "MatrixMath.h"
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::mat4;
using glm::vec3;

#define GLM_FORCE_RADIANS

#include <iostream>
using namespace std;

TEST_CASE( "lookat", "[lookat]" ) {

    float eye[3] = { 5,5,20 };
    float center[3] = {0,0,0};
    float up[3] = {0,1,0};
    float view[16];
    MatrixMath::LookAt( eye, center, up, view );

    mat4 view_glm = glm::lookAt( vec3(5,5,20), vec3(0,0,0), vec3(0,1,0) );

    float * view_check = nullptr;
    view_check = glm::value_ptr( view_glm );

    for( int i = 0; i < 16; ++i ){
	REQUIRE( view_check );
	cout << i << ": " << view[i] << ", check: " << *view_check << endl;
	CHECK( ( view[i] > (*view_check - 0.001 ) ) );
	CHECK( ( view[i] < (*view_check + 0.001 ) ) );
        view_check++;
    }
}
