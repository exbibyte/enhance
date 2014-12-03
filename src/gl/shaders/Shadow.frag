#version 330
// Declare any uniforms needed for the Phong shading model
uniform sampler2DShadow ShadowMap;

in vec4 Position;
in vec3 Normal;
in vec4 ShadowCoord;

out vec4 FragColor;

struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 La; // Ambient light intensity
    vec3 Ld; // Diffuse light intensity
    vec3 Ls; // Specular light intensity
};

uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka; // Ambient reflectivity
    vec3 Kd; // Diffuse reflectivity
    vec3 Ks; // Specular reflectivity
    float Shininess; // Specular shininess factor
};

uniform MaterialInfo Material;

vec3 phongModel( vec4 position, vec3 norm )
{
    vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect( -s, norm );
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Ls * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess );
    return diffuse + spec;
}

uniform int bShadeShadow;

// subroutine void RenderPassType();
// subroutine uniform RenderPassType RenderPass;

//subroutine (RenderPassType)
void shadeWithShadow()
{
  // Do the shadow-map look-up
//  float shadow = textureProj(ShadowMap, ShadowCoord);
  // If the fragment is in shadow, use ambient light only.
  vec3 ambient = Light.La * Material.Ka;

  // The sum of the comparisons with nearby texels
  float sum = 0; 

  // Sum contributions from texels around ShadowCoord
  sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( -1,-1 ) );
  sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( -1, 1 ) );
  sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( 1, 1 ) );
  sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( 1, -1 ) );
  // sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( -1, 0 ) );
  // sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( 1, 0 ) );
  // sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( 0, -1 ) );
  // sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( 0, 1 ) );
  sum += textureProjOffset( ShadowMap, ShadowCoord, ivec2( 0, 0 ) );

  float shadow = sum * 0.2;

  vec3 DiffSpec = phongModel( Position, Normal );

  FragColor = vec4( shadow * DiffSpec + ambient, 1.0 );
}

//subroutine (RenderPassType)
void recordDepth()
{
  // Do nothing, depth will be written automatically
}

void main() {
  // This will call either shadeWithShadow or recordDepth

    if( bShadeShadow == 1 )
    {
        shadeWithShadow();
    }
    else
    {
        recordDepth();
    }
//  RenderPass();
}

