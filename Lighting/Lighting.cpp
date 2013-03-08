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
#include <glm/glm.hpp>

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
GLdouble outerMatrix[16]; //matrix which moves all rings
GLdouble middleMatrix[16];  //matrix which moves only two
GLdouble innerMatrix[16];	//moves only itself
float  light0_diff[] = {1.0, 0.9, 0.05, 1};GLint viewport[4];
GLdouble projectionMatrix[16];
//lists (unless object-buffering)

//variables
bool isFilled = true;
bool isAnimating = false;
bool light0_on = true;
bool light1_on = true;

//object colors
GLfloat brass_am[] = {0.329412, 0.223529, 0.027451, 1.000000};
GLfloat jade_am[] = {0.135000, 0.222500, 0.157500, 0.950000};
GLfloat obsidian_am[] = {0.053750, 0.050000, 0.066250, 0.820000};

GLfloat brass_dif[] = {0.780392, 0.568627, 0.113725, 1.000000};
GLfloat jade_dif[] = {0.540000, 0.890000, 0.630000, 0.950000};
GLfloat obsidian_dif[] = {0.182750, 0.170000, 0.225250, 0.820000};

GLfloat brass_spec[] = {0.992157, 0.941176, 0.807843, 1.000000};
GLfloat jade_spec[] = {0.316228, 0.316228, 0.316228, 0.950000};
GLfloat obsidian_spec[] = {0.332741, 0.328634, 0.346435, 0.820000};

GLfloat brass_shiny[] = {27.897400};
GLfloat jade_shiny[] = {12.800000};
GLfloat obsidian_shiny[] = {38.400002};

//Cone color
GLfloat cone_emit[]={0.192250, 0.192250, 0.192250, 1.000000};

//Light Colors
GLfloat light0Diffuse[] = {.1, .1, .9};
GLfloat light0Specular[] = {.9, .1, .1};
GLfloat light0Ambient[] = {.1, .1, .1};

GLfloat light1Diffuse[] = {.7, .7, .9};
GLfloat light1Specular[] = {.9, .1, .1};
GLfloat light1Ambient[] = {.1, .0, .0};


//Light positions
GLfloat light0pos[] = {-.2,.5,.2,1.0};

GLfloat light1pos[] = {6.0, .2, 0.0, 0.0};
GLfloat light1focus[] = {-10.0, 0.0, 0.0, 0.0};


//Materials
GLfloat outerDiffuse[] = {.2, .2, .2};
GLfloat outerSpecular[] = {.2, .2, .2};
GLfloat outerEmissive[] = {.0, .0, .0};
GLfloat outerShininess[] = {80};


void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(light0_on){
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_SPECULAR, light0Specular);
		glLightfv(GL_LIGHT0,GL_DIFFUSE, light0Diffuse);
		glLightfv(GL_LIGHT0,GL_AMBIENT, light0Ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, light0pos);

	}
	else glDisable(GL_LIGHT0);

	if(light1_on){
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1,GL_SPECULAR, light1Specular);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, light1Diffuse);
		glLightfv(GL_LIGHT1,GL_AMBIENT, light1Ambient);
		glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
		//spotlight stuff
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 64.0);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	//	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1focus);

		 glPushMatrix();
		    glPushAttrib(GL_LIGHTING_BIT);

		    glMaterialfv(GL_FRONT, GL_EMISSION, light0_diff);
		    glTranslatef(light1pos[0], light1pos[1], light1pos[2]);
		    glVertex3f(0.04,5, 12);
		    glutSolidSphere(.1,20,20);
		    glPopAttrib();
		    glPopMatrix();
	}
	else glDisable(GL_LIGHT1);

	glMaterialfv(GL_FRONT, GL_SPECULAR, outerSpecular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, outerDiffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, outerEmissive);
	glMaterialfv(GL_FRONT, GL_SHININESS, outerShininess);


	glPushMatrix();
	 glMultMatrixd(worldMatrix); //move objects about world

	 
	 glPushMatrix();
	  glMultMatrixd(outerMatrix); //move parent object

		  glPushAttrib(GL_LIGHTING_BIT);
		  glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT, GL_SPECULAR, brass_spec);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, brass_dif);
//			glMaterialfv(GL_FRONT, GL_SHININESS, brass_am);
			glMaterialfv(GL_FRONT, GL_AMBIENT, brass_shiny);

	        glPopAttrib();

	  	  glColor3fv(brass_am);
		glutSolidTorus(.1,4.4,100,100);
	  //stuff goes here
	  //object calls

	  glPushMatrix();
	   glMultMatrixd(middleMatrix); //move child about parent and child matrix

		  glPushAttrib(GL_LIGHTING_BIT);
		  glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT, GL_SPECULAR, jade_spec);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, jade_dif);
//			glMaterialfv(GL_FRONT, GL_SHININESS, jade_shiny);
			glMaterialfv(GL_FRONT, GL_AMBIENT, jade_am);
	        glPopAttrib();

	   glColor3fv(jade_am);
		glutSolidTorus(.1,4.2,100,100);
	   //stuff goes here
	   //object calls

	   glPushMatrix();
	    glMultMatrixd(innerMatrix);
		//more stuff
		  glPushAttrib(GL_LIGHTING_BIT);
		  glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT, GL_SPECULAR, obsidian_spec);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, obsidian_dif);
//			glMaterialfv(GL_FRONT, GL_SHININESS, obsidian_shiny);
			glMaterialfv(GL_FRONT, GL_AMBIENT, obsidian_am);

	        glPopAttrib();

	    glColor3fv(obsidian_am);
		glutSolidTorus(.1,4,100,100);
		glPopMatrix();
	  glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();

}

void animationLoop(int value){
	glPushMatrix();
		glLoadIdentity();
		//rotate once per amt frames
		glRotated(360.0/100,0,1,0);
		glMultMatrixd(outerMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX,outerMatrix);
	glPopMatrix();

	glPushMatrix();
		glLoadIdentity();
		//rotate once per amt frames
		glRotated(360.0/80,1,0,0);
		glMultMatrixd(middleMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX,middleMatrix);
	glPopMatrix();

	glPushMatrix();
		glLoadIdentity();
		//rotate once per amt frames
		glRotated(360.0/60,0,1,0);
		glMultMatrixd(innerMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX,innerMatrix);
	glPopMatrix();

	//update every 1second/60 (frames per second)
	glutPostRedisplay();
	if (isAnimating) glutTimerFunc((int) 1000/30, &animationLoop, 0);

}

void createObjects(){
	//IMPORTANT NOTE:
		//Try to keep all objects size <= 1 float, as 1 float
		//means a size of 1 "screen" (we just zoomed way out last time)
	//also, think about using "object-buffer-arrays" to do this project (in 
	//dulimartah's git source)
}

bool moveEye(int direction){
	if (!isAnimating){
		glutTimerFunc((int) 1000/30, &animationLoop, 0);
		isAnimating=true;
	} else {
		isAnimating=false;
	}
	//TODO - redo world movement
	//this is accomplished by moving the world matrix
	return true;
}

void init(){
   	glClearColor(.0,.0,.0,1.0);
	glLineWidth(2.0);
	glPointSize(3.0);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glPushMatrix();
	glLoadIdentity(); //store identity in all matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, outerMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, middleMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, innerMatrix);
	glPopMatrix();


    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);

	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_LIGHTING); //enable lighting?
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
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


	//Light 1 positioning.
	case 'i':
		light1pos[0]+=.1;
		break;
	case 'o':
		light1pos[1]+=.1;
		break;
	case 'p':
		light1pos[2]+=.1;
		break;

	case 'I':
		light1pos[0]-=.1;
		break;
	case 'O':
		light1pos[1]-=.1;
		break;
	case 'P':
		light1pos[2]-=.1;
		break;
/*
	//Light 1 focus positioning
	case 'j':
		light1focus[0]+=.1;
		break;
	case 'k':
		light1focus[1]+=.1;
		break;
	case 'l':
		light1focus[2]+=.1;
		break;

	case 'J':
		light1focus[0]-=.1;
		break;
	case 'K':
		light1focus[1]-=.1;
		break;
	case 'L':
		light1focus[2]-=.1;
		break;
*/
	case 'l':
		if(light1_on)
			light1_on = false;
		else light1_on = true;
		break;
	case 'L':
		if(light0_on)
			light0_on = false;
		else light0_on = true;
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

	fprintf(stderr, "light1pos = { %f, %f, %f, %f}\n",light1pos[0],light1pos[1],light1pos[2],light1pos[3]);
	fprintf(stderr, "light1focus= { %f, %f, %f, %f}\n",light1focus[0],light1focus[1],light1focus[2],light1focus[3]);

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
	gluLookAt(	0,0,10,
				0,0,7,
				0,1,0);
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
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
