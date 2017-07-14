#include "common.h"

#ifdef CHAPTER_8
#include "opengl8h.h"

#ifdef CHAPTER_8_COMMON
GLsizei winWidth = 800, winHeight = 600;
#endif

#ifdef CHAPTER_8_4_16
class wcPt2D
{
public:
	GLfloat x, y;
};
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}
void triangle(wcPt2D * verts)
{
	GLint k;
	glBegin(GL_TRIANGLES);
	for (k = 0; k < 3; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void displayFunc(void)
{
	wcPt2D verts[3] = { {-50.0, -25.0},{50.0, -25.0},{0.0, 50.0} };
	wcPt2D verts2[3] = { {100.0, -25.0 },{ 200.0, -25.0 },{ 150.0, 50.0 } };
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-50, 50.0, -100.0, 100.0);
	glViewport(0, 0, 300, 300);
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(100, 200.0, -100.0, 100.0);
	glViewport(300, 0, 300, 300);
	//triangle(verts2);
	//glColor3f(1.0, 0.0, 0.0);
	//glViewport(300, 0, 300, 300);
	//glRotatef(90.0, 0.0, 0.0, 1.0);
	//triangle(verts);
	
	//glViewport(0, 0, 300, 300);

	glFlush();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 300);
	glutCreateWindow("Split-Screen Example");

	init();
	glutDisplayFunc(displayFunc);

	glutMainLoop();
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_8_COMMON

#ifdef CHAPTER_8_COMMON
void init(void)
{
	// ºÚÉ«±³¾°É«
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth , 0, winHeight);
}

void main(int argc, char** argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("An Example OpenGL Program");

	init();

//#ifdef CHAPTER_7_1_1
//	code_7_1_1();
//#endif

	glutMainLoop();
}
#endif
#endif 


