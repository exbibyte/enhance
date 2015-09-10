#ifndef RENDER_MESH_ORIENTATION_H
#define RENDER_MESH_ORIENTATION_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;

class RenderMeshOrientation {
public:
    mat4 _MatOrientation;
    //temporary buffers
    mat4 _MatView;
    mat4 _MatProjection;
    bool ComputeCompositeMats(); // computes _MatOrientationViewProjection, _MatOrientationViewProjectionBias, _MatNormal
    bool GetCompositeMats( mat4 & orientation_view, mat4 & orientation_view_projection, mat4 & orientation_view_projection_bias, mat3 & normal );

    mat4 _MatOrientationView;
    mat4 _MatOrientationViewProjection;
    mat4 _MatOrientationViewProjectionBias;
    mat3 _MatNormal;
};

#endif
