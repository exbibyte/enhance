#include "RenderMeshOrientation.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;

bool RenderMeshOrientation::ComputeCompositeMats(){
    _MatOrientationView = _MatView * _MatOrientation;
    _MatOrientationViewProjection = _MatProjection * _MatView * _MatOrientation;
    mat4 Bias(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);
    _MatOrientationViewLightProjectionBias = Bias * _MatLightProjection * _MatLightView * _MatOrientation;
    _MatNormal = glm::inverse( glm::transpose( glm::mat3( _MatOrientationView ) ) );
    return true;
}
bool RenderMeshOrientation::GetCompositeMats( mat4 & orientation_view, mat4 & orientation_view_projection, mat4 & orientation_view_lightprojection_bias, mat3 & normal ){
    orientation_view = _MatOrientationView;
    orientation_view_projection = _MatOrientationViewProjection;
    orientation_view_lightprojection_bias = _MatOrientationViewLightProjectionBias;
    normal = _MatNormal;
    return true;
}
