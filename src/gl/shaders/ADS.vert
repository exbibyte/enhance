#version 330

in vec3 VertexPosition;
in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

out vec3 eyeNorm;
out vec4 eyePosition;

void getEyeSpace( out vec3 norm, out vec4 position )
{
    norm = normalize( NormalMatrix * VertexNormal );
    position = ModelViewMatrix * vec4(VertexPosition, 1.0);
}


void main()
{
    // Get the position and normal in eye space
    getEyeSpace(eyeNorm, eyePosition);

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
