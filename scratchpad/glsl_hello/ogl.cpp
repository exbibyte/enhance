#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "textfile.h"

#include "GLHelper.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	// glLoadIdentity();
	// gluLookAt(0.0,0.0,5.0, 
	// 	      0.0,0.0,-1.0,
	// 		  0.0,1.0,0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

        // glm::mat4 m_projectionMatrix = glm::perspective(60.0f, 1.333f, 0.1f, 1000.0f);

        // glm::vec3 eye = glm::vec3(0.0f, 0.0f, 10.0f);
        // glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f);
        // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // glm::mat4 m_modelViewMatrix = glm::lookAt( eye, target, up );
        // // m_modelViewMatrix *= glm::rotate(1,1,1,1);
        // // mat4 rotationMatrix = glm::rotate(mat4(1.0f), angle, vec3(0.0f,0.0f,1.0f));

        glBindVertexArray(vaoHandle);
        glDrawArrays(GL_TRIANGLES, 0, 3 );

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
        glBindAttribLocation(p, 0, "VertexPosition");
        // Bind index 1 to the shader input variable "VertexColor"
        glBindAttribLocation(p, 1, "VertexColor");

	GLLinkProgram(p);

        float positionData[] = {
            -0.8f, -0.8f, 0.0f,
            0.8f, -0.8f, 0.0f,
            0.0f, 0.8f, 0.0f };

        float colorData[] = {
            1.0f, 0.0f, 0.0f, 
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f };


        glGenBuffers(2, vboHandles);
        GLuint positionBufferHandle = vboHandles[0];
        GLuint colorBufferHandle = vboHandles[1];
        // Populate the position buffer
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);
        // Populate the color buffer
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

        // Create and set-up the vertex array object
        glGenVertexArrays( 1, &vaoHandle );
        glBindVertexArray(vaoHandle);
        // Enable the vertex attribute arrays
        glEnableVertexAttribArray(0); // Vertex position
        glEnableVertexAttribArray(1); // Vertex color
        // Map index 0 to the position buffer
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
        // Map index 1 to the color buffer
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
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

