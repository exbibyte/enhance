#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "textfile.h"

#include "GLHelper.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;
using glm::gtc::matrix_transform;
#include <glm/gtc/matrix_transform.hpp>

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

	// Set the viewport to be the entire window
        glViewport(0, 0, w, h);
}


void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

        glm::mat4 m_projectionMatrix = glm::perspective(60.0, 1.333, 0.1, 1000.0);

        glm::mat4 m_modelViewMatrix = glm::lookat(
            0.0,0.0,10.0, 
            0.0,0.0,-1.0,
            0.0f,1.0f,0.0f
        );
        // m_modelViewMatrix *= glm::rotate(1,1,1,1);
        // mat4 rotationMatrix = glm::rotate(mat4(1.0f), angle, vec3(0.0f,0.0f,1.0f));

        GLint loc = glGetUniformLocation(p, "modelview_matrix");
        double dArray[16] = {0.0};
        const float *pSource = (const float*)glm::value_ptr(m_modelViewMatrix);
        for (int i = 0; i < 16; ++i){
            dArray[i] = pSource[i];
        }
        glProgramUniformMatrix4fv(p, loc, 1, GL_FALSE, dArray);
    
        loc = glGetUniformLocation(p, "projection_matrix");
        double dArray2[16] = {0.0};
        const float *pSource2 = (const float*)glm::value_ptr(_projectionMatrix);
        for (int i = 0; i < 16; ++i){
            dArray2[i] = pSource2[i];
        }
        glProgramUniformMatrix4fv(p, loc, 1, GL_FALSE, dArray2);

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

        glGetUniformLocation(p, "projection_matrix");
        glGetUniformLocation(p, "modelview_matrix");

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

