/**
MODEL REQUIREMENTS
   1) Design two 3D objects using GLUT Solid: sphere, cone, torus, ... and GLUquadric objects: gluCylinder, gluDisk, ...
   2) One of these objects must include a moving component
   3) Maintain a separate coordinate frame matrix for each object
   4) Use the mouse or keyboard to translate and rotate each individual object w.r.t to its own coordinate frame.
   Design the mouse/keyboard controls such that each object can be moved to any position and orientation in the 3D world

ANIMATION REQUIREMENTS
   5) Use a timer to animate a periodic motion of your object(s) or part of it
   6) In addition to simple rotation or repetitive translation, the animation must also demonstrate a composite motion. Some examples of composite motions:
	 A rolling wheel (rotation + translation)
	 Earth rotation around its axis and revolution around the sun
	 A football thrown from a quarterback to his receiver (spinning on its axis while its trajectory follows a parabolic curve)

	 The following motions do not exhibit a composite motion:
	 clock hands
	 spin of propeller blades

SHADING REQUIREMENTS
   7) Render the image with shading enabled. Use at least two light sources.
   8) Provide keyboard controls to enable / disable each light source.
   9) Use combination of material ambient, diffuse, specular, and shininess coefficients that show the visual effect of real materials.
      You may use the following table as your guideline for material selection.

EXTRA CREDIT
   Use a parametric curve (other than a circle) to control the placement of the animated object(s)
   Render the light source as an emissive object and use keyboard controls to position the ligh source.
   Use a separate coordinate frame to control the camera
**/


/***********************************
Lighting test in OpenGL
@authors: Matthew Shrider and James Uhe

Creates objects in opengl which are lighted realistically
***********************************/

#if (defined(WIN32) | defined(_WIN32))
#define _USE_MATH_DEFINES
#endif
#include <GL/glut.h>
#include <math.h>

//functions
void init();
void resize(int w, int h);
void createObjects();
void render(void);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void animationLoop(int value);
bool moveEye(int direction);

//matrices
GLdouble worldMatrix[16];  //matrix which moves everything but camera
GLdouble parentMatrix[16]; //matrix which moves parent+child
GLdouble childMatrix[16];  //matrix which moves only child

GLint viewport[4];
GLdouble projectionMatrix[16];
//lists (unless object-buffering)

//variables
bool isFilled = true;
bool isAnimating = false;

void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	 glMultMatrixd(worldMatrix); //move objects about world

	 glPushMatrix();
	  glMultMatrixd(parentMatrix); //move parent object
	  //stuff goes here
	  //object calls

	  glPushMatrix();
	   glMultMatrixd(childMatrix); //move child about parent and child matrix
	   //stuff goes here
	   //object calls

	  glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}

void animationLoop(int value){

}

void createObjects(){
	//IMPORTANT NOTE:
		//Try to keep all objects size <= 1 float, as 1 float
		//means a size of 1 "screen" (we just zoomed way out last time)
	//also, think about using "object-buffer-arrays" to do this project (in 
	//dulimartah's git source)
}

bool moveEye(int direction){
	//TODO - redo world movement
	//this is accomplished by moving the world matrix
}

void init(){
   	glClearColor(0,0,0,1.0);
	glLineWidth(2.0);
	glPointSize(3.0);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glPushMatrix();
	glLoadIdentity(); //store identity in all matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, parentMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, childMatrix);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING); //enable lighting?
}

void keyboard(unsigned char key, int x, int y){
	switch (key)
   {
    case '\x1B':
      exit(EXIT_SUCCESS);
      break;
	case 'w':
		moveEye(0);
		break;
	case 's':
		moveEye(1);
		break;
	case 'a':
		moveEye(2);
		break;
	case 'd':
		moveEye(3);
		break;
	case 'r':
		moveEye(8);
		break;
	case 'f':
		moveEye(9);
		break;
	case 't':
		if (isFilled) {
			glPolygonMode(GL_FRONT,GL_LINE);
			isFilled = false;
			glutPostRedisplay();
		} else {
			glPolygonMode(GL_FRONT,GL_FILL);
			isFilled = true;
			glutPostRedisplay();
		}
		break;
	}
}

void specialKeys(int key, int x, int y){
	int modifiers = glutGetModifiers();
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if ( modifiers == GLUT_ACTIVE_SHIFT ){
			moveEye(10);
		}
		else if(modifiers == GLUT_ACTIVE_CTRL) {
			moveEye(13);
		} else {
		moveEye(4);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (modifiers == GLUT_ACTIVE_SHIFT ){
		moveEye(11);
		} else if(modifiers == GLUT_ACTIVE_CTRL) {
			moveEye(12);
		} else {
			moveEye(5);
		}
		break;
	case GLUT_KEY_UP:
		if (modifiers == GLUT_ACTIVE_CTRL) {
			moveEye(14);
		} else {
		moveEye(6);
		}
		break;
	case GLUT_KEY_DOWN:
		if (modifiers == GLUT_ACTIVE_CTRL) {
			moveEye(15);
		} else {
		moveEye(7);
		}
		break;
	}
}


void resize(int w, int h){
	glViewport(0,0,(GLint) w, (GLint) h);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	GLfloat ratio;
	if (w <= h){
		ratio = static_cast<GLfloat> (h) / w;
	} else {
		ratio = static_cast<GLfloat> (w) / h;
	}
	gluPerspective(60, ratio,1,60);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	0,0,1,
				0,0,0,
				0,1,0);
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(800,650);
  glutCreateWindow("Lighting Test by Matthew Shrider and James Uhe");

  init();
  createObjects();

  glutKeyboardFunc(&keyboard);
  glutSpecialFunc(&specialKeys);
  glutDisplayFunc(&render);
  glutReshapeFunc(&resize);
  glutMainLoop();

  return EXIT_SUCCESS;
}
