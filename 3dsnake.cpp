#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#include <gl/gl.h>
#endif

#include <time.h>
#include "Scene.h"

#define TIMER_MS 40

Scene* globalScene = 0;
bool eyeMode;

Scene* getScene()
{
	return globalScene;
}

void TimerCallback(int value)
{
	getScene()->updateScene();
	glutPostRedisplay();
	glutTimerFunc(TIMER_MS, TimerCallback, 0);
}

void drawRect()
{
	glDisable(GL_LIGHTING);
	glColor3d(0.0, 0.0, 0.2);
	glLineWidth(6);

	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINE_LOOP);
	{
		glVertex3i (-1, -1, -1);
		glVertex3i (1, -1, -1);
		glVertex3i (1, 1, -1);
		glVertex3i (-1, 1, -1);
	}
	glEnd();
	glPopMatrix();

	glLineWidth(1);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}

void renderScene() 
{
	glViewport( 0, 0, 640 , 480 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	getScene()->render();

	if (eyeMode)
	{
		getScene()->toggleSnakeEyes();
		glEnable(GL_SCISSOR_TEST);
		glViewport( 10, 270, 180, 180 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		drawRect();
		getScene()->render();
		getScene()->toggleSnakeEyes();
		glDisable(GL_SCISSOR_TEST);
	}

	glutSwapBuffers();
}

void reshapeFunc( int width , int height )
{
	glViewport( 0, 0, width , height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	float ratio = (float) width / (float) height ;

	glFrustum( -0.1 * ratio, 0.1 * ratio, -0.1, 0.1, 0.15, 100);
	glutPostRedisplay();
}

void initProgram()
{
	eyeMode = false;

	globalScene = new Scene(10);
	globalScene->init(10, 2, 4);
	globalScene->restartGame(0);
	globalScene->togglePause();

	glClearColor(0, 0, 0, 0);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glScissor(10, 270, 180, 180);

	// set the reversed head lights (for the text)

	GLfloat light_ambient[] = { 1, 1, 1, 1 }; 
	GLfloat light_diffuse[] = { 1, 1, 1, 1 }; 
	GLfloat light_specular[] = { 1, 0, 0, 1 }; 
	GLfloat light_position[] = { 0, 0, -0.16, 1 };
	GLfloat light_dir[] = { 0, 0, 1 };

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 90);
	
	glutTimerFunc(TIMER_MS, TimerCallback, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	/* help screen

             3D Snake 
             ---------------
N	New game
Space 	Help (pause)
4/6	Turn left/right
2/8	Turn up/down
7/9	Roll left/right
G	Grid
Escape	Exit
E	Toggle eye mode
	
	*/

	switch (key) {

	case 'g':
	case 'G':
		getScene()->toggleGrid();
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		eyeMode = !eyeMode;
		break;

	case 'a': 
	case 'A':
	case '4':
		getScene()->turnLeft();
		break;
	case 'd': 
	case 'D':
	case '6':
		getScene()->turnRight();
		break;
	case 's': 
	case 'S':
	case '2':
		getScene()->turnUp();
		break;
	case 'x': 
	case 'X':
	case '8':
		getScene()->turnDown();
		break;
	case 'z': 
	case 'Z':
	case '7':
		getScene()->rollLeft();
		break;
	case 'c': 
	case 'C':
	case '9':
		getScene()->rollRight();
		break;
	
	case 'n': 
	case 'N':
		getScene()->restartGame(0);
		break;

	case 32:
		getScene()->togglePause();
		break;
	case 27:
		exit(0);
		break;
	}
}

void specialKeyboard(int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_LEFT:
		getScene()->turnLeft();
		break;
	case GLUT_KEY_RIGHT:
		getScene()->turnRight();
		break;
	case GLUT_KEY_UP:
		getScene()->turnUp();
		break;
	case GLUT_KEY_DOWN:
		getScene()->turnDown();
		break;
	case GLUT_KEY_PAGE_UP:
		getScene()->rollLeft();
		break;
	case GLUT_KEY_PAGE_DOWN:
		getScene()->rollRight();
		break;
	}
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("3D Snake");
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	
	initProgram();

	// start OpenGl main loop
	glutMainLoop();
	return 0;
}
