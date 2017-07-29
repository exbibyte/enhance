#version 400

struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 Intensity; // Diffuse intensity
};

uniform LightInfo Light;
struct MaterialInfo {
    vec3 Kd; // Diffuse reflectivity
};

uniform MaterialInfo Material;
subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

// The g-buffer textures
uniform sampler2D PositionTex, NormalTex, ColorTex;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 0) out vec3 PositionData;
layout (location = 0) out vec3 NormalData;
layout (location = 0) out vec4 ColorData;

vec3 diffuseModel( vec3 pos, vec3 norm, vec3 diff )
{
    vec3 s = normalize(vec3(Light.Position) - pos);
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Intensity * diff * sDotN;
    return diffuse;
}

subroutine (RenderPassType)
void pass1()
{
    // Store position, normal, and diffuse color in g-buffer
    PositionData = Position;
    NormalData = Normal;
    ColorData = Material.Kd;
}

subroutine(RenderPassType)
void pass2()
{
    // Retrieve position, normal and color information from
    // the g-buffer textures
    vec3 pos = vec3( texture( PositionTex, TexCoord ) );
    vec3 norm = vec3( texture( NormalTex, TexCoord ) );
    vec3 diffColor = vec3( texture(ColorTex, TexCoord) );
    FragColor = vec4( diffuseModel(pos,norm,diffColor), 1.0 );
}

void main() {
    // This will call either pass1 or pass2
    RenderPass();
}
















