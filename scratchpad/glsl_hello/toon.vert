in vec3 VertexPosition;
in vec3 VertexColor;
varying vec3 Color;

void main()
{
        Color = VertexColor;
        gl_Position = vec4(VertexPosition,1.0);
}