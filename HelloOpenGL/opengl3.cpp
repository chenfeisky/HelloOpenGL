#include "common.h"


#ifdef CHAPTER_3
#include "opengl3h.h"

#ifdef CHAPTER_3_5_5
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 200.0, 0, 150.0);
}

void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);
	glBegin(GL_LINES);
	glVertex2i(180, 15);
	glVertex2i(10, 145);
	glEnd();

	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("An Example OpenGL Program");

	init();
	glutDisplayFunc(lineSegment);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_3_5_6
GLenum errorCheck()
{
	GLenum code;
	const GLubyte * string;
	code = glGetError();
	if (code != GL_NO_ERROR)
	{
		string = gluErrorString(code);
		fprintf(stderr, "OpenGL Error : %s\n", string);
	}
	return code;
}

void drawFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);
	glBegin(GL_LINES);
	glVertex2i(180, 15);
	glVertex2i(10, 145);
	glEnd();
	glFlush();
}

void code_3_5_6()
{
	glutDisplayFunc(drawFunction);
	errorCheck();
}
#endif

#ifdef CHAPTER_3_EXERCISE_1
void drawFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}
void init(void)
{
	// Ç³»ÒÉ«
	glClearColor(0.8, 0.8, 0.8, 0.0);
	// ºÚÉ«
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 200.0, 0, 150.0);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("An Example OpenGL Program");

	init();
	glutDisplayFunc(drawFunction);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_3_EXERCISE_2
void drawFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 200.0, 0, 150.0);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(75, 50);
	glutInitWindowSize(200, 150);
	glutCreateWindow("An Example OpenGL Program");

	init();
	glutDisplayFunc(drawFunction);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_3_EXERCISE_3
void drawFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(150, 250);
	glVertex2i(0, 0);
	glEnd();
	glFlush();
}
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 150, 0, 250);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(150, 250);
	glutCreateWindow("An Example OpenGL Program");

	init();
	glutDisplayFunc(drawFunction);
	glutMainLoop();
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_3_COMMON

#ifdef CHAPTER_3_COMMON

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 200.0, 0, 150.0);
}

void main(int argc, char** argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("An Example OpenGL Program");

	init();

#ifdef CHAPTER_3_5_6
	code_3_5_6();
#endif

	glutMainLoop();
}
#endif

#endif