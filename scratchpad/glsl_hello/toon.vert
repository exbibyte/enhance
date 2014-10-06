// simple toon vertex shader
// www.lighthouse3d.com

varying vec3 normal, lightDir;

uniform mat4 projection_matrix;
uniform	mat4 modelview_matrix;
 
varying vec3 vertex;

void main()
{	
           
     	lightDir = normalize(vec3(gl_LightSource[0].position));
        normal = normalize(gl_NormalMatrix * gl_Normal);
	
	gl_Position = projection_matrix * modelview_matrix * vec4(vertex, 1.0);
}
