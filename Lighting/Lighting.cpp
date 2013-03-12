/***********************************
Lighting test in OpenGL
@authors: Matthew Shrider and James Uhe

Creates a set of ring in OpenGL which create a gyroscope.
The Gyroscope has realistic lighting on it.

Keybindings:
** w,a,s,d,q,e	---  Moves camera forwards/back/sideways/and vertically
** left, right, up, down arrows - Turns camera 
**
** 1,2,3,4,5,6			 ---	moves point light around
** 7,8,9,0,-,=			 ---	moves spotlight around
** shift-[arrowkeys]	 --- turns spotlight
** l, L					 ---   toggles each light individually.
**
** z,x,c,v,b,n	  ---	   moves middle ring around
** ctrl-[zxcvbn] ---		turns middle ring
**
** Z,X,C,V,B,N	  ---	  Moves inner right around
** ctrl-[ZXCVBN] ---	  turns inner ring around
**
** space ---	 starts and stops the animation
** <,>	---	 speeds up and slows down the animation
** 
** t	 ---	 toggles wireframes
***********************************/

#if (defined(WIN32) | defined(_WIN32))
#define _USE_MATH_DEFINES
#endif
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//functions
void init();
void resize(int w, int h);
void render(void);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void animationLoop(int value);

//matrices
GLdouble worldMatrix[16];  //matrix which moves everything but camera
GLdouble outerMatrix[16]; //matrix which moves all rings
GLdouble middleMatrix[16];  //matrix which moves only two
GLdouble innerMatrix[16];	//moves only itself
GLdouble light0Matrix[16];
GLdouble light1Matrix[16];
GLdouble previousTranslate[16];
float  light0_diff[] = {1.0, 0.9, 0.05, 1};
GLint viewport[4];
GLdouble projectionMatrix[16];

//variables
bool isFilled = true;
bool isAnimating = false;
bool light0_on = true;
bool light1_on = true;
int speed = 80;

//object colors
GLfloat brass_am[] = {0.329412, 0.223529, 0.027451, 1.000000};
GLfloat jade_am[] = {0.135000, 0.222500, 0.157500, 0.950000};
GLfloat ruby_am[] = {0.374500, 0.011750, 0.011750, 0.550000};
GLfloat ball_am[] = {0.0, .6, 0.0, .3};

GLfloat brass_dif[] = {0.780392, 0.568627, 0.113725, 1.000000};
GLfloat jade_dif[] = {0.540000, 0.890000, 0.630000, 0.950000};
GLfloat ruby_dif[] = {0.714240, 0.041360, 0.041360, 0.550000};

GLfloat brass_spec[] = {0.992157, 0.941176, 0.807843, 1.000000};
GLfloat jade_spec[] = {0.316228, 0.316228, 0.316228, 0.950000};
GLfloat ruby_spec[] = {0.827811, 0.626959, 0.626959, 0.550000};

GLfloat brass_shiny[] = {27.897400};
GLfloat jade_shiny[] = {12.800000};
GLfloat ruby_shiny[] = {76.800003};
GLfloat ball_shiny[] = {12};
GLfloat ball_emissive[] = {0.0, .1, 0.0, .3};

//Light Colors
GLfloat light0Diffuse[] = {.9, .9, .9};
GLfloat light0Specular[] = {.5, 1, .3};
GLfloat light0Ambient[] = {.05, .05, .05};

GLfloat light1Diffuse[] = {.7, .7, .7};
GLfloat light1Specular[] = {.8, .8, 1};
GLfloat light1Ambient[] = {0, .0, .0};

//Light positions
GLfloat light0pos[] = {0,0,0,1.0};

GLfloat light1pos[] = {0.0, 0, 0.0,1.0};
GLfloat light1focus[] = {-1.0, 0.0, 0.0, 0.0};

/*********************************
* Renders the OpenGL scene
* *********************************/
void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glMultMatrixd(worldMatrix); //move objects about world

	if(light1_on){		 //Spotlight attributes and setup
		glPushMatrix();
		glMultMatrixd(light1Matrix);
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1,GL_SPECULAR, light1Specular);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, light1Diffuse);
		glLightfv(GL_LIGHT1,GL_AMBIENT, light1Ambient);
		glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, .1);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, .05);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,10.0);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1focus);
		glTranslated(-.5,0,0);
		glutSolidSphere(.2,10,10);
		glPopMatrix();
	}
	else glDisable(GL_LIGHT1);

	glPushMatrix();
	if (isAnimating){ //vibrate the gyroscope a small amount each frame
		double xamt = ((rand() % 11 - 5)) / 100.0;
		double yamt = ((rand() % 11 - 5)) / 100.0;
		double zamt = ((rand() % 11 - 5)) / 100.0;
		GLdouble tempTranslateMatrix[16] = {1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			xamt,yamt,zamt,1};
		glMultMatrixd(tempTranslateMatrix);
	}
	glMultMatrixd(outerMatrix); //move parent object

	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, brass_spec);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, brass_dif);
	glMaterialfv(GL_FRONT, GL_SHININESS, brass_shiny);
	glMaterialfv(GL_FRONT, GL_AMBIENT, brass_am);

	glPopAttrib();

	glColor3fv(brass_am);
	glutSolidTorus(.1,4.4,100,100);

	glPushMatrix();
	glMultMatrixd(middleMatrix); //move child about parent and child matrix

	glPushAttrib(GL_LIGHTING_BIT);
	glMaterialfv(GL_FRONT, GL_SPECULAR, jade_spec);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, jade_dif);
	glMaterialfv(GL_FRONT, GL_SHININESS, jade_shiny);
	glMaterialfv(GL_FRONT, GL_AMBIENT, jade_am);
	glPopAttrib();

	glColor3fv(jade_am);
	glutSolidTorus(.1,4.2,100,100);

	glPushMatrix();
	glMultMatrixd(innerMatrix);
	glPushAttrib(GL_LIGHTING_BIT);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ruby_spec);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby_dif);
	glMaterialfv(GL_FRONT, GL_SHININESS, ruby_shiny);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ruby_am);

	glPopAttrib();
	glColor3fv(ruby_am);
	glutSolidTorus(.1,4,100,100);

	if(light0_on){
		glPushMatrix();
		glMultMatrixd(light0Matrix);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_SPECULAR, light0Specular);
		glLightfv(GL_LIGHT0,GL_DIFFUSE, light0Diffuse);
		glLightfv(GL_LIGHT0,GL_AMBIENT, light0Ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, .1);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, .3);

		glMaterialfv(GL_FRONT, GL_AMBIENT, light0Ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, ball_shiny);
		glColor3fv(light0Diffuse);
		glutSolidSphere(.3,30,30);
		glPopMatrix();
	}
	else glDisable(GL_LIGHT0);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

/**********************************
* Spins the gyroscope around and animates the scene
************************************/
void animationLoop(int value){
	//create a temporary translation
	glPushMatrix();
	glLoadIdentity();
	//rotate once per amt frames
	glRotated(360.0/speed,0,1,0);
	glMultMatrixd(outerMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX,outerMatrix);
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	//rotate once per amt frames
	glRotated(360.0/speed,1,0,0);
	glMultMatrixd(middleMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX,middleMatrix);
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	//rotate once per amt frames
	glRotated(360.0/speed,0,1,0);
	glMultMatrixd(innerMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX,innerMatrix);
	glPopMatrix();

	glutPostRedisplay();
	//update 30 times per second
	if (isAnimating) glutTimerFunc((int) 1000/30, &animationLoop, 0);
}

/****************************************
* Sets up the background color, and initializes the object coordinate frames
*****************************************/
void init(){
	srand(time(NULL));
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
	glGetDoublev(GL_MODELVIEW_MATRIX, light0Matrix);
	glTranslated(7,0,0);
	glGetDoublev(GL_MODELVIEW_MATRIX, light1Matrix);
	glPopMatrix();

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_LIGHTING); 
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}

/**********************************
* Sets up the keyboard handler
* ********************************/
void keyboard(unsigned char key, int x, int y){
	glPushMatrix();
	int modifiers = glutGetModifiers();
	switch (key)
	{
	case '\x1B':
		exit(EXIT_SUCCESS);
		break;
	case 'w':
		glLoadIdentity();
		glTranslated(0,0,.3);
		glMultMatrixd(worldMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		break;
	case 's':
		glLoadIdentity();
		glTranslated(0,0,-.3);
		glMultMatrixd(worldMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		break;
	case 'a':
		glLoadIdentity();
		glTranslated(.3,0,0);
		glMultMatrixd(worldMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		break;
	case 'd':
		glLoadIdentity();
		glTranslated(-.3,0,0);
		glMultMatrixd(worldMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		break;
	case 'q':
		glLoadIdentity();
		glTranslated(0,-.3,0);
		glMultMatrixd(worldMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		break;
	case 'e':
		glLoadIdentity();
		glTranslated(0,.3,0);
		glMultMatrixd(worldMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		break;
	case 'z':
		glLoadIdentity();
		glMultMatrixd(middleMatrix);
		if (modifiers == GLUT_ACTIVE_ALT){
			glRotated(6,1,0,0);
		} else {
			glTranslated(0,0,.3);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, middleMatrix);
		break;
	case 'x':
		glLoadIdentity();
		glMultMatrixd(middleMatrix);
		if (modifiers == GLUT_ACTIVE_ALT){
			glRotated(-6,1,0,0);
		} else {
			glTranslated(0,0,-.3);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, middleMatrix);
		break;
	case 'c':
		glLoadIdentity();
		glMultMatrixd(middleMatrix);
		if (modifiers == GLUT_ACTIVE_ALT){
			glRotated(6,0,1,0);
		} else {
			glTranslated(-.3,0,0);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, middleMatrix);
		break;
	case 'v':
		glLoadIdentity();
		glMultMatrixd(middleMatrix);
		if (modifiers == GLUT_ACTIVE_ALT){
			glRotated(-6,0,1,0);
		} else {
			glTranslated(.3,0,0);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, middleMatrix);
		break;
	case 'b':
		glLoadIdentity();
		glMultMatrixd(middleMatrix);
		if (modifiers == GLUT_ACTIVE_ALT){
			glRotated(6,0,0,1);
		} else {
			glTranslated(0,.3,0);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, middleMatrix);
		break;
	case 'n':
		glLoadIdentity();
		glMultMatrixd(middleMatrix);
		if (modifiers == GLUT_ACTIVE_ALT){
			glRotated(-6,0,0,1);
		} else {
			glTranslated(0,-.3,0);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, middleMatrix);
		break;
	case 'Z':
		glLoadIdentity();
		glMultMatrixd(innerMatrix);
		if (modifiers == GLUT_ACTIVE_ALT + GLUT_ACTIVE_SHIFT){
			glRotated(6,1,0,0);
		} else {
			glTranslated(0,0,.3);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, innerMatrix);
		break;
	case 'X':
		glLoadIdentity();
		glMultMatrixd(innerMatrix);
		if (modifiers == GLUT_ACTIVE_SHIFT + GLUT_ACTIVE_ALT){
			glRotated(-6,1,0,0);
		} else {
			glTranslated(0,0,-.3);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, innerMatrix);
		break;
	case 'C':
		glLoadIdentity();
		glMultMatrixd(innerMatrix);
		if (modifiers == GLUT_ACTIVE_SHIFT + GLUT_ACTIVE_ALT){
			glRotated(6,0,1,0);
		} else {
			glTranslated(-.3,0,0);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, innerMatrix);
		break;
	case 'V':
		glLoadIdentity();
		glMultMatrixd(innerMatrix);
		if (modifiers == GLUT_ACTIVE_SHIFT + GLUT_ACTIVE_ALT){
			glRotated(-6,0,1,0);
		} else {
			glTranslated(.3,0,0);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, innerMatrix);
		break;
	case 'B':
		glLoadIdentity();
		glMultMatrixd(innerMatrix);
		if (modifiers == GLUT_ACTIVE_SHIFT + GLUT_ACTIVE_ALT){
			glRotated(6,0,0,1);
		} else {
			glTranslated(0,.3,0);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, innerMatrix);
		break;
	case 'N':
		glLoadIdentity();
		glMultMatrixd(innerMatrix);
		if (modifiers == GLUT_ACTIVE_SHIFT + GLUT_ACTIVE_ALT){
			glRotated(-6,0,0,1);
		} else {
			glTranslated(0,-.3,0);
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, innerMatrix);
		break;
	case '1':
		glLoadIdentity();
		glTranslated(-.1,0,0);
		glMultMatrixd(light0Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light0Matrix);
		break;
	case '2':
		glLoadIdentity();
		glTranslated(.1,0,0);
		glMultMatrixd(light0Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light0Matrix);
		break;
	case '3':
		glLoadIdentity();
		glTranslated(0,-.1,0);
		glMultMatrixd(light0Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light0Matrix);
		break;
	case '4':
		glLoadIdentity();
		glTranslated(0,.1,0);
		glMultMatrixd(light0Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light0Matrix);
		break;
	case '5':
		glLoadIdentity();
		glTranslated(0,0,.1);
		glMultMatrixd(light0Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light0Matrix);
		break;
	case '6':
		glLoadIdentity();
		glTranslated(0,0,-.1);
		glMultMatrixd(light0Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light0Matrix);
		break;
	case '7':
		glLoadIdentity();
		glTranslated(-.1,0,0);
		glMultMatrixd(light1Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light1Matrix);
		break;
	case '8':
		glLoadIdentity();
		glTranslated(.1,0,0);
		glMultMatrixd(light1Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light1Matrix);
		break;
	case '9':
		glLoadIdentity();
		glTranslated(0,-.1,0);
		glMultMatrixd(light1Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light1Matrix);
		break;
	case '0':
		glLoadIdentity();
		glTranslated(0,.1,0);
		glMultMatrixd(light1Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light1Matrix);
		break;
	case '-':
		glLoadIdentity();
		glTranslated(0,0,.1);
		glMultMatrixd(light1Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light1Matrix);
		break;
	case '=':
		glLoadIdentity();
		glTranslated(0,0,-.1);
		glMultMatrixd(light1Matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, light1Matrix);
		break;

	case 'l':
		if(light1_on)
			light1_on = false;
		else light1_on = true;
		glutPostRedisplay();
		break;
	case 'L':
		if(light0_on)
			light0_on = false;
		else light0_on = true;
		glutPostRedisplay();
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
	case ' ':
		if (!isAnimating){
			glutTimerFunc((int) 1000/30, &animationLoop, 0);
			isAnimating=true;
		} else {
			isAnimating=false;
		}
		break;
	case '>':
		speed -= 2;
		if (speed < 1) speed = 1;
		break;
	case '<':
		speed += 2;
		if (speed > 360) speed = 360;
		break;
	}
	glPopMatrix();
	glutPostRedisplay();
}

/************************************
* Keyhandler for arrowkeys
* ***********************************/
void specialKeys(int key, int x, int y){
	glPushMatrix();
	int modifiers = glutGetModifiers();
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if ( modifiers == GLUT_ACTIVE_SHIFT ){
			glLoadIdentity();
			glMultMatrixd(light1Matrix);
			glRotated(-6,0,0,1);
			glGetDoublev(GL_MODELVIEW_MATRIX,light1Matrix);
		} else {
			glLoadIdentity();
			glTranslated(0,0,15);
			glRotated(-6,0,1,0);
			glTranslated(0,0,-15);
			glMultMatrixd(worldMatrix);
			glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (modifiers == GLUT_ACTIVE_SHIFT ){
			glLoadIdentity();
			glMultMatrixd(light1Matrix);
			glRotated(6,0,0,1);
			glGetDoublev(GL_MODELVIEW_MATRIX,light1Matrix);} else {
				glLoadIdentity();
				glTranslated(0,0,15);
				glRotated(6,0,1,0);
				glTranslated(0,0,-15);
				glMultMatrixd(worldMatrix);
				glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
			}
			break;
	case GLUT_KEY_UP:
		if (modifiers == GLUT_ACTIVE_SHIFT) {
			glLoadIdentity();
			glMultMatrixd(light1Matrix);
			glRotated(-6,0,1,0);
			glGetDoublev(GL_MODELVIEW_MATRIX,light1Matrix);

		} else {
			glLoadIdentity();
			glTranslated(0,0,15);
			glRotated(-6,1,0,0);
			glTranslated(0,0,-15);
			glMultMatrixd(worldMatrix);
			glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		}
		break;
	case GLUT_KEY_DOWN:
		if (modifiers == GLUT_ACTIVE_SHIFT) {
			glLoadIdentity();
			glMultMatrixd(light1Matrix);
			glRotated(6,0,1,0);
			glGetDoublev(GL_MODELVIEW_MATRIX,light1Matrix);
		} else {
			glLoadIdentity();
			glTranslated(0,0,15);
			glRotated(6,1,0,0);
			glTranslated(0,0,-15);
			glMultMatrixd(worldMatrix);
			glGetDoublev(GL_MODELVIEW_MATRIX, worldMatrix);
		}
		break;
	}
	glPopMatrix();
	glutPostRedisplay();
}

/*************************************
* Resizes the window and sets up the projection matrix
* ***********************************/
void resize(int w, int h){
	glLoadIdentity();
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
	gluLookAt(	0,0,15,
		0,0,7,
		0,1,0);
}

/************************************
* Sets up the OpenGL callbacks and begins the main loop
* ***********************************/
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(800,650);
	glutCreateWindow("Lighting Test by Matthew Shrider and James Uhe");

	init();

	glutKeyboardFunc(&keyboard);
	glutSpecialFunc(&specialKeys);
	glutDisplayFunc(&render);
	glutReshapeFunc(&resize);
	glutMainLoop();

	return EXIT_SUCCESS;
}
