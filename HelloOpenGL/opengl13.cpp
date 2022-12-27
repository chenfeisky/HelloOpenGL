#include "common.h"

#if CHAPTER==13
#include "opengl13h.h"

#ifdef CHAPTER_13_COMMON
float winWidth = 800, winHeight = 600;
#endif

#ifdef CHAPTER_13_2_3

GLsizei winWidth = 500, winHeight = 500;

static void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void displayWirePolyhedra(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0); // 白色绘制
	gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glScalef(1.5, 2.0, 1.0);
	glutWireCube(1.0);

	glScalef(0.8, 0.5, 0.8);
	glTranslatef(-6.0, -5.0, 0.0);
	glutWireDodecahedron();

	glTranslatef(8.6, 8.6, 2.0);
	glutWireTetrahedron();

	glTranslatef(-3.0, -1.0, 0.0);
	glutWireOctahedron();

	glScalef(0.8, 0.8, 1.0);
	glTranslatef(4.3, -2.0, 0.5);
	glutWireIcosahedron();

	glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 20.0);

	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Wire-Frame Polyhedra");

	init();
	glutDisplayFunc(displayWirePolyhedra);
	glutReshapeFunc(winReshapeFcn);

	glutMainLoop();
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_13_COMMON

#ifdef CHAPTER_13_COMMON
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("An Example OpenGL Program");

	init();

#ifdef CHAPTER_13_xx
	code_12_1_2();
#endif

	glutMainLoop();
}
#endif


#endif 


