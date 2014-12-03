#version 330

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 Normal;
out vec4 Position;

// Coordinate to be used for shadow map lookup
out vec4 ShadowCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform mat4 ShadowMatrix;

void getEyeSpace( out vec3 norm, out vec4 position )
{
    norm = normalize( NormalMatrix * VertexNormal );
    position = ModelViewMatrix * vec4(VertexPosition, 1.0);
}


void main()
{
    // Get the position and normal in eye space
    getEyeSpace( Normal, Position );

    // ShadowMatrix converts from modeling coordinates to shadow map coordinates.
    ShadowCoord = ShadowMatrix * vec4( VertexPosition, 1.0 );
    ShadowCoord.z = ShadowCoord.z - 0.0005;
    ShadowCoord.xyz = ShadowCoord.xyz / ShadowCoord.w;
    ShadowCoord.w = 1;

    gl_Position = MVP * vec4( VertexPosition, 1.0 );
}
