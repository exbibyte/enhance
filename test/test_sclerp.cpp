#include "DualQuat.h"
#include <unistd.h>
#include <iostream>
#include <iostream>
#include <stdexcept>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "Trackball.h"
#include "Quat.h"

using namespace std;

// I wouldn't be using globals if this wasn't glut!
Trackball trackball;
int windowWidth  = 640,
    windowHeight = 480;
bool tracking = false;
int lastTime = glutGet(GLUT_ELAPSED_TIME);
int currAnim = 0;
float zdist = -300.0f;
int count = 0;

float transform_mat[16];

void init(int argc, char **argv) {
  lastTime = glutGet(GLUT_ELAPSED_TIME);

  // set necessary OpenGL states
  glClearColor(0.9, 0.9, 0.9, 1.0);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // light parameters
  GLfloat lightAmbient []= { 0.7f,  0.7f, 0.7f, 1.0f };
  GLfloat lightDiffuse []= { 1.0f,  1.0f, 1.0f, 1.0f };
  GLfloat lightPosition[]= { 1.0f, -1.0f, 0.0f, 0.0f }; // direction light
  GLfloat lightSpecular[]= { 0.5f,  0.5f, 0.5f, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}


void resizeCallback(int width, int height) {
  if (height==0)
    height=1;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, GLfloat(width)/GLfloat(height), 1.0f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);

  windowWidth = width;
  windowHeight = height;
}


void displayCallback() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
  glLoadIdentity();

  glTranslatef(0.0f, 0.0f, zdist);

  // get rotation matrix from trackball
  float mat[16];
  trackball.look(mat);
  glMultMatrixf(mat);

  DualQuat qA;
  DualQuat qB;
  float axis_start[3] = {0,0,1};
  float axis_end[3] = {0,1,1};
  qA._A.AxisAngleDegree( axis_start, 0);
  qB._A.AxisAngleDegree( axis_end, 180);

  qA._B._quat[0] = 0;
  qA._B._quat[1] = 0;
  qA._B._quat[2] = 0;
  qA._B._quat[3] = 0;

  qB._B._quat[0] = 30;
  qB._B._quat[1] = 30;
  qB._B._quat[2] = -30;
  qB._B._quat[3] = 0;

  if(count <= 100){
    float slerp_t = (float) count/100;
    cout<<"slerp time: "<<slerp_t<<endl;
    
    DualQuat q;
    q = InterpolateSclerp( qA, qB, slerp_t );  
    q.GetRigidTransform(transform_mat); 
    count++;

    // float arr[8];
    // q.GetArray(arr);
    // for( int i = 0; i < 8; i++ ){
    //   cout<<arr[i]<<" ";
    // }
    // cout<<endl;
  }

  glMultMatrixf(transform_mat);

  glutSolidTeapot(30);
  glutSwapBuffers();
}


// called continuously during program's execution
void idleCallback() {
  int currTime = glutGet(GLUT_ELAPSED_TIME);
  // cout << "elapsed time: "<<currTime<<endl;

  // make sure some minimum threshold of time (1 ms in this case) has passed
  // before stepping animation
  if ( currTime - lastTime > 1 ) {
    // cout<<"here"<<endl;
    float dt = float(currTime - lastTime)/1000.0f;
    lastTime = currTime;

    // need to update display
    glutPostRedisplay();
  }
}


// called when user presses a key
void kbdCallback(unsigned char key, int x, int y) {
    switch( key ) {
    case 27:
      exit(0);

    // zoom in/out with a and z keys
    case 'a':
    case 'A':
      zdist -= 10.0f;
      break;
    case 'z':
    case 'Z':
      zdist += 10.0f;
      break;

    // if user presses '-' key then change to previous animation,
    // and loop around once animation gets to zero
    case '-':
    case '_':
    // if user presses '+' key then change to next animation,
    // and loop around after last animation
    case '+':
    case '=':
    default:
      break;
    }

    // need to update display
    glutPostRedisplay();
}


// called when user presses a mouse button
void mouseButtonCallback(int button, int state, int x, int y) {
  if ( GLUT_LEFT_BUTTON == button ) {
    if ( GLUT_DOWN == state ) {
      trackball.startMotion(x, y, windowWidth, windowHeight);
      tracking = true;
    }

    if ( GLUT_UP == state )
      tracking = false;
  }
}


// called when user moves mouse
void mouseMotionCallback(int x, int y) {
  if ( tracking ) {
    trackball.move(x, y, windowWidth, windowHeight);

    // since trackball has moved, have to render again
    glutPostRedisplay();
  }
}


int main(int argc, char **argv) {
  // initialize glut
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  // create window
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("MD5 Loader Demo by A.J. Tavakoli"
                   " (use +/- to cycle animations)");

  //init GLEW
  GLenum err = glewInit();
  if(GLEW_OK != err)
  {
    cout<<"glew init failed"<<endl;
    return -1;
  }

  // register callback functions with glut
  glutDisplayFunc(displayCallback);
  glutKeyboardFunc(kbdCallback);
  glutMouseFunc(mouseButtonCallback);
  glutMotionFunc(mouseMotionCallback);
  glutReshapeFunc(resizeCallback);
  glutIdleFunc(idleCallback);

  try {
    init(argc, argv);

    // begin main loop
    glutMainLoop();
  }
  catch ( std::runtime_error &e ) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
