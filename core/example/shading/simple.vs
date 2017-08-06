#version 400

layout(location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 La;
    // Ambient light intensity
    vec3 Ld;
    // Diffuse light intensity
    vec3 Ls;
    // Specular light intensity
};

uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka;
    // Ambient reflectivity
    vec3 Kd;
    // Diffuse reflectivity
    vec3 Ks;
    // Specular reflectivity
    float Shininess;
    // Specular shininess factor
};

uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main(){
     gl_Position.xyz = VertexPosition;
     gl_Position.w = 1.0;
}
