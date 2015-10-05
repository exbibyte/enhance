#ifndef GL_LIGHT
#define GL_LIGHT

//math library
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat4;
using glm::vec3;

class GLLight {
public:
    GLLight();
    bool SetProjectionMatLight( mat4 & proj_light );
    bool GetProjectionMatLight( mat4 & proj_light ) const;
    bool SetPositionLight( vec3 & pos_light );
    bool GetPositionLight( vec3 & pos_light ) const;
    bool SetLightMaterialLa( vec3 & light_material_La );
    bool SetLightMaterialLd( vec3 & light_material_Ld );
    bool SetLightMaterialLs( vec3 & light_material_Ls );
    bool GetLightMaterial( vec3 & light_material_La, vec3 & light_material_Ld, vec3 & light_material_Ls ) const;
private:
    mat4 _ProjectionMatLight;
    vec4 _LightPosition;
    vec3 _LightLa;
    vec3 _LightLd;
    vec3 _LightLs;
};

#endif
