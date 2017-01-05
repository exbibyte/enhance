#include "GLLight.h"
#include "MatrixMath.h"

//math library
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat4;
using glm::vec3;

GLLight::GLLight(){
    // _ProjectionMatLight = glm::perspective( 60.0f, 1.0f, 0.1f, 1000.0f );
    float perspective_mat[16];
    MatrixMath::Perspective( 60.0f, 1.0f, 0.1f, 1000.0f, perspective_mat );
    _ProjectionMatLight = glm::make_mat4( perspective_mat );
}
bool GLLight::SetProjectionMatLight( mat4 & proj_light ){
    _ProjectionMatLight = proj_light;
    return true;
}
bool GLLight::GetProjectionMatLight( mat4 & proj_light ) const{
    proj_light = _ProjectionMatLight;
    return true;
}
bool GLLight::SetPositionLight( vec3 & pos_light ){
    _LightPosition = vec4( pos_light, 1 );
    return true;
}
bool GLLight::GetPositionLight( vec3 & pos_light ) const{
    pos_light = vec3( _LightPosition );
    return true;
}
bool GLLight::SetLightMaterialLa( vec3 & light_material_La ){
    _LightLa = light_material_La;
    return true;
}
bool GLLight::SetLightMaterialLd( vec3 & light_material_Ld ){
    _LightLd = light_material_Ld;
    return true;
}
bool GLLight::SetLightMaterialLs( vec3 & light_material_Ls ){
    _LightLs = light_material_Ls;
    return true;
}
bool GLLight::GetLightMaterial( vec3 & light_material_La, vec3 & light_material_Ld, vec3 & light_material_Ls ) const{
    light_material_La = _LightLa;
    light_material_Ld = _LightLd;
    light_material_Ls = _LightLs;
    return true;
}
