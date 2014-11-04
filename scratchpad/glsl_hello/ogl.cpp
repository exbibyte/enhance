#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "textfile.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
using namespace std;

using glm::mat4;
using glm::vec3;

#include "GLSLProgram.h"
#include "GLAttribData.h"

GLuint v,f,f2,p;
float lpos[4] = {1,0.5,1,0};
GLuint vaoHandle;

//Create the buffer objects
GLuint vboHandles[2];
float colourData[] = { 1, 1, 1 };
GLuint colorBufferHandle;

float angle = 0;

GLSLProgram * _GLSLProgram;

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

    angle+=0.01;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    mat4 rotationMatrix = glm::rotate(mat4(1.0f), angle, vec3(0.0f,0.0f,1.0f));
    _GLSLProgram->SetUniform( "RotationMatrix", (mat4 const) rotationMatrix );

    _GLSLProgram->BindVertexArray();

    // GLAttribData<float> * data;
    // _GLSLProgram->GetMapAttrib( "VertexColor", data );
    // data->DisableVertexArray();

    glDrawArrays( GL_TRIANGLES, 0, 3 );

    _GLSLProgram->UnBindVertexArray();

    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27) 
        exit(0);
}


void setShaders() {

    _GLSLProgram = new GLSLProgram;
    _GLSLProgram->CompileShaderFromFile("toon.vert", GLSLShader::VERTEX );
    _GLSLProgram->CompileShaderFromFile("toon.frag", GLSLShader::FRAGMENT );
    _GLSLProgram->AttachShaders();

    //set buffer data
    GLAttribData<float> * pPositionData = new GLAttribData<float>;
    GLAttribData<float> * pColorData = new GLAttribData<float>;
    float arrayPositionData[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f };
    float arrayColorData[] = {
        1.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };
    
    //save mapping of data
    _GLSLProgram->AddMapAttrib( "VertexPosition", pPositionData );
    _GLSLProgram->AddMapAttrib( "VertexColor", pColorData );
    //bind attributes
    _GLSLProgram->BindMapAttrib();

    _GLSLProgram->BindFragDataLocation( 0, "FragColor" );

    _GLSLProgram->Link();

    _GLSLProgram->PrintActiveUniforms();

    _GLSLProgram->PrintActiveAttribs();

    //generate VBO, populate and bind data to vertex attribute arrays
    pPositionData->SetData( &arrayPositionData[0], 3, 9 );
    pColorData->SetData( &arrayColorData[0], 3, 9 );

    _GLSLProgram->Use();
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

    GLPrintInfo();

    setShaders();

    glutMainLoop();

    // just for compatibiliy purposes
    return 0;
}

