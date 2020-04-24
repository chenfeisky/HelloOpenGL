#include "common.h"

#ifdef CHAPTER_12
#include "opengl12h.h"

#ifdef CHAPTER_12_COMMON
float winWidth = 800, winHeight = 600;
#endif

#ifdef CHAPTER_12_10
const double TWO_PI = 6.2831853;
GLsizei winWidth = 500, winHeight = 500;
GLuint regHex;
static GLfloat rotTheta = 0.0;

class scrPt
{
	public :
		GLint x, y;
};

static void init(void)
{
	scrPt hexVertex;
	GLdouble hexTheta;
	GLint k;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	regHex = glGenLists(1);
	glNewList(regHex, GL_COMPILE);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (k = 0; k < 6; k++)
	{
		hexTheta = TWO_PI * k / 6;
		hexVertex.x = 150 + 100 * cos(hexTheta);
		hexVertex.y = 150 + 100 * sin(hexTheta);
		glVertex2i(hexVertex.x, hexVertex.y);
	}
	glEnd();
	glEndList();
}

void displayHex(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPushMatrix();
	glRotatef(-rotTheta, 0.0, 0.0, 1.0);
	glCallList(regHex);
	glPopMatrix();

	glutSwapBuffers();

	glFlush();
}

void rotateHex(void)
{
	rotTheta += 3.0;
	if (rotTheta > 360.0)
		rotTheta -= 360.0;

	glutPostRedisplay();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-320.0, 320.0, -320.0, 320.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClear(GL_COLOR_BUFFER_BIT);
}

void mouseFcn(GLint button, GLint action, GLint x, GLint y)
{
	switch (button)
	{
	case GLUT_MIDDLE_BUTTON:
		if (action == GLUT_DOWN)
			glutIdleFunc(rotateHex);
		break;
	case GLUT_RIGHT_BUTTON:
		if (action == GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Animation Example");

	init();
	glutDisplayFunc(displayHex);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mouseFcn);

	glutMainLoop();
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_12_COMMON

#ifdef CHAPTER_12_COMMON
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // 黑色背景色
	glColor3f(1.0, 1.0, 1.0); // 白色绘制
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth , 0, winHeight);
}

void main(int argc, char** argv)
{
	//srand(time(0));
	srand(100);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("An Example OpenGL Program");

	init();


	glutMainLoop();
}
#endif
#endif 


