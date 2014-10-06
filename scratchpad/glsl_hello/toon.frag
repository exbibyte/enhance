#version 330

varying vec3 Color;
out vec4 FragColor;

void main() {
     FragColor = vec4(Color, 1.0);
}
