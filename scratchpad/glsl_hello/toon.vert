// simple toon vertex shader
// www.lighthouse3d.com

varying vec3 normal, lightDir;

varying vec3 mod_colour;
varying vec3 mod_colour2;

void main()
{	
        mod_colour2 = mod_colour;
             
     	lightDir = normalize(vec3(gl_LightSource[0].position));
        normal = normalize(gl_NormalMatrix * gl_Normal);
	
	gl_Position = ftransform();
}
