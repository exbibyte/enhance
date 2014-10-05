/*


  Simple Demo for GLSL 2.0

  www.lighthouse3d.com

*/
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "textfile.h"

#include "GLHelper.h"

GLuint v,f,f2,p;
float lpos[4] = {1,0.5,1,0};

GLuint vaoHandle;

//Create the buffer objects
GLuint vboHandles[2];
float colourData[] = { 1, 1, 1 };
GLuint colorBufferHandle;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);


}


void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

        glBindVertexArray(vaoHandle);

	glutSolidTeapot(1);

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}


void setShaders() {

	char *vs = NULL,*fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);


	vs = textFileRead("toon.vert");
	fs = textFileRead("toon.frag");

	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);	
        glShaderSource(f, 1, &ff,NULL);

	free(vs);
        free(fs);

	glCompileShader(v);
	glCompileShader(f);

        GLCompileShader(v);
        GLCompileShader(f);

	GLCreateProgram(p);

	glAttachShader(p,f);
	glAttachShader(p,v);

        // Bind index 0 to the shader input variable "VertexPosition"
        glBindAttribLocation(p, 0, "mod_colour");

        glGenBuffers(2, vboHandles);
        colorBufferHandle = vboHandles[0];

        //Create and set-up the vertex array object
        glGenVertexArrays( 1, &vaoHandle );
        glBindVertexArray(vaoHandle);
        //Enable the vertex attribute arrays
        glEnableVertexAttribArray(0); // mod_colour attribute
        //Map index 0 to the color buffer
        //Populate the color buffer
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), colourData,
                     GL_STATIC_DRAW);

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

	GLLinkProgram(p);
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("MM 2004-05");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0,1.0,1.0,1.0);
//	glEnable(GL_CULL_FACE);

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
		printf("Ready for OpenGL 3.0\n");
	else {
		printf("OpenGL 3.0 not supported\n");
		exit(1);
	}

        const char * p_glversion = (const char * ) glGetString(GL_VERSION);
        printf("GL Version: %s\n", p_glversion);

	setShaders();

	glutMainLoop();

	// just for compatibiliy purposes
	return 0;
}

