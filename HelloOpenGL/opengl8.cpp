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
	gluOrtho2D(0.0, 100.0, 0.0, 50.0);
	//glMatrixMode(GL_MODELVIEW);
}
wcPt2D verts[3] = { {10.0, 10.0},{40.0, 10.0},{25.0, 40.0} };
wcPt2D verts2[3] = { { 60.0, 10.0 },{ 90.0, 10.0 },{ 75.0, 40.0 } };
wcPt2D verts3[4] = { { 50.0, 0.0 },{ 100.0, 0.0 },{ 100.0, 50.0 },{ 50.0, 50.0 } };
int w1, w2;
void displayFunc2(void);
bool systemCall1 = true;
bool systemCall2 = true;
float curAngle = 10;
void triangle(wcPt2D * verts)
{
	float xc = 0, yc = 0;
	GLint k;
	for (k = 0; k < 3; k++)
	{
		xc += verts[k].x;
		yc += verts[k].y;
	}
	xc = xc / 3;
	yc = yc / 3;
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(xc, yc, 0);
	glRotatef(curAngle, 0, 0, 1);
	glTranslatef(-xc, -yc, 0);

	glBegin(GL_TRIANGLES);
	for (k = 0; k < 3; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void rect(wcPt2D * verts)
{
	GLint k;
	glBegin(GL_POLYGON);
	for (k = 0; k < 4; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void displayFunc1(void)
{
	int a = glutGetWindow();

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 50.0);
	glViewport(0, 150, 600, 150);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	//glColor3f(1.0, 1.0, 1.0);
	//rect(verts3);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 50.0, 0.0, 50.0);
	glViewport(0, 0, 300, 150);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	//glColor3f(1.0, 1.0, 1.0);
	//rect(verts3);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts2);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(50.0, 100.0, 0.0, 50.0);
	glViewport(300, 0, 300, 150);
	//glViewport(100, 0, 300, 150);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	//glColor3f(1.0, 1.0, 1.0);
	//rect(verts3);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts2);

	if (systemCall1)
	{
		systemCall2 = false;
		glutSetWindow(w2);
		glutPostRedisplay();
	}

	if (systemCall2)
	{
		//for (auto& v : verts)
		//{
		//	v.x += 5;
		//}
		//for (auto& v : verts2)
		//{
		//	v.x += 5;
		//}
		curAngle += 10;
	}

	systemCall1 = true;

	glFlush();
}
void displayFunc2(void)
{
	int a = glutGetWindow();

	glClear(GL_COLOR_BUFFER_BIT);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0.0, 100.0, 0.0, 50.0);
	//glViewport(0, 150, 600, 150);
	//glColor3f(0.0, 0.0, 1.0);
	//triangle(verts);
	////glColor3f(1.0, 1.0, 1.0);
	////rect(verts3);
	//glColor3f(1.0, 0.0, 0.0);
	//triangle(verts2);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0, 50.0, 0.0, 50.0);
	//glViewport(0, 0, 300, 150);
	//glColor3f(0.0, 0.0, 1.0);
	//triangle(verts);
	////glColor3f(1.0, 1.0, 1.0);
	////rect(verts3);
	//glColor3f(1.0, 0.0, 0.0);
	//triangle(verts2);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(50.0, 100.0, 0.0, 50.0);
	//glViewport(300, 0, 300, 150);
	////glViewport(100, 0, 300, 150);
	//glColor3f(0.0, 0.0, 1.0);
	//triangle(verts);
	////glColor3f(1.0, 1.0, 1.0);
	////rect(verts3);
	//glColor3f(1.0, 0.0, 0.0);
	//triangle(verts2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 50.0);
	glViewport(0, 0, 600, 300);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	//glColor3f(1.0, 1.0, 1.0);
	//rect(verts3);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts2);
	
	if (systemCall2)
	{
		systemCall1 = false;
		glutSetWindow(w1);
		glutPostRedisplay();
	}

	if (systemCall1)
	{
		//for (auto& v : verts)
		//{
		//	v.x += 5;
		//}
		//for (auto& v : verts2)
		//{
		//	v.x += 5;
		//}
		curAngle += 10;
	}

	systemCall2 = true;

	glFlush();
}
void main(int argc, char** argv)
{
	int a = glutGetWindow();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 300);
	w1 = glutCreateWindow("Example1");
	init();
	glutDisplayFunc(displayFunc1);
	a = glutGetWindow();

	glutInitWindowPosition(50, 400);
	glutInitWindowSize(600, 300);
	w2 = glutCreateWindow("Example2");
	init();
	glutDisplayFunc(displayFunc2);
	a = glutGetWindow();

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


