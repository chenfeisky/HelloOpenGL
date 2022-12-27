#include "common.h"

#if CHAPTER==11
#include "opengl11h.h"

#ifdef CHAPTER_11_COMMON
float winWidth = 800, winHeight = 600;
#endif

#ifdef CHAPTER_11_4
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLint frame = 1;
	glNewList(frame, GL_COMPILE);
		glRecti(-100, 50, 100, -50);
	glEndList();

	GLint wheel = 2;
	glNewList(wheel, GL_COMPILE);
		glRecti(-25, 25, 25, -25);
	glEndList();

	GLint bicycle = 3;
	glNewList(bicycle, GL_COMPILE);
		glCallList(frame);
		
		glPushMatrix();
			glTranslatef(-100, -75, 0);
			glCallList(wheel);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(100, -75, 0);
			glCallList(wheel);
		glPopMatrix();
	glEndList();

	glTranslatef(400, 300, 0);
	glCallList(bicycle);

	glFlush();
}

void code_11_4()
{
	glutDisplayFunc(displayFcn);
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_11_COMMON

#ifdef CHAPTER_11_COMMON
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // ºÚÉ«±³¾°É«
	glColor3f(1.0, 1.0, 1.0); // °×É«»æÖÆ
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

#ifdef CHAPTER_11_4
	code_11_4();
#endif

	glutMainLoop();
}
#endif
#endif 


