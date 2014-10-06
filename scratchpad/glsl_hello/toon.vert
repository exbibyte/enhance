#version 330

in vec3 VertexPosition;
in vec3 VertexColor;
out vec3 Color;
uniform mat4 RotationMatrix;

void main()
{
        Color = VertexColor;
        gl_Position = RotationMatrix * vec4(VertexPosition,1.0);        
}
