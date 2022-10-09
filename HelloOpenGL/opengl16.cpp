#include "common.h"

#if CHAPTER==16
#include "opengl16h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_16_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;
#endif

#ifdef CHAPTER_16_14_1
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(10, 10, 0);
	glVertex3f(0, 10, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(2, 2, -10);
	glVertex3f(2, 12, -10);
	glVertex3f(12, 12, -10);
	glVertex3f(12, 2, -10);
	glEnd();

	glFlush();
}

void code_16_14_1()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
#endif

#ifdef CHAPTER_16_14_2
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(10, 10, 0);
	glVertex3f(0, 10, 0);
	glEnd();

	glColor3f(0.f, 1.f, 0.f);
	glBegin(GL_POLYGON);
	glVertex3f(2, 2, -10);
	glVertex3f(12, 2, -10);
	glVertex3f(12, 12, -10);
	glVertex3f(2, 12, -10);
	glEnd();

	glFlush();
}

void code_16_14_2()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
	glEnable(GL_DEPTH_TEST);
}
#endif

#ifdef CHAPTER_16_14_3
float leftOffset = -20, rightOffset = 20;
void drawFrontPolygon(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0 + leftOffset, 0, 0);
	glVertex3f(10 + leftOffset, 0, 0);
	glVertex3f(10 + leftOffset, 10, 0);
	glVertex3f(0 + leftOffset, 10, 0);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0 + leftOffset, 0, 0);
	glVertex3f(10 + leftOffset, 0, 0);
	glVertex3f(10 + leftOffset, 10, 0);
	glVertex3f(0 + leftOffset, 10, 0);
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);
}

void drawBackPolygon(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(2 + leftOffset, 2, -10);
	glVertex3f(12 + leftOffset, 2, -10);
	glVertex3f(12 + leftOffset, 12, -10);
	glVertex3f(2 + leftOffset, 12, -10);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(2 + leftOffset, 2, -10);
	glVertex3f(12 + leftOffset, 2, -10);
	glVertex3f(12 + leftOffset, 12, -10);
	glVertex3f(2 + leftOffset, 12, -10);
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);
}

GLint points[8][3] = { {0 + rightOffset, 0, 0}, {0 + rightOffset, 10, 0},{10 + rightOffset, 0, 0},{10 + rightOffset, 10, 0},
	{0 + rightOffset, 0, 10},{0 + rightOffset, 10, 10},{10 + rightOffset, 0, 10},{10 + rightOffset, 10, 10} };
void quad(GLint n1, GLint n2, GLint n3, GLint n4)
{
	glBegin(GL_QUADS);
	glVertex3iv(points[n1]);
	glVertex3iv(points[n2]);
	glVertex3iv(points[n3]);
	glVertex3iv(points[n4]);
	glEnd();
}

void cube()
{
	quad(6, 2, 3, 7);
	quad(5, 1, 0, 4);
	quad(7, 3, 1, 5);
	quad(4, 0, 2, 6);
	quad(2, 0, 1, 3);
	quad(7, 5, 4, 6);
}

void drawCube(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	cube();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	cube();

	glDisable(GL_POLYGON_OFFSET_FILL);
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	drawBackPolygon();
	drawFrontPolygon();

	drawCube();

	glFlush();
}

void code_16_14_3()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	camera = new Camera(Point(0, 36, 100), Rotator(-20, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
}
#endif

#ifdef CHAPTER_16_14_4
float leftOffset = -20, rightOffset = 20;
void drawFrontPolygon(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0 + leftOffset, 0, 0);
	glVertex3f(10 + leftOffset, 0, 0);
	glVertex3f(10 + leftOffset, 10, 0);
	glVertex3f(0 + leftOffset, 10, 0);
	glEnd();
}

void drawBackPolygon(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(2 + leftOffset, 2, -10);
	glVertex3f(12 + leftOffset, 2, -10);
	glVertex3f(12 + leftOffset, 12, -10);
	glVertex3f(2 + leftOffset, 12, -10);
	glEnd();
}

GLint points[8][3] = { {0 + rightOffset, 0, 0}, {0 + rightOffset, 10, 0},{10 + rightOffset, 0, 0},{10 + rightOffset, 10, 0},
	{0 + rightOffset, 0, 10},{0 + rightOffset, 10, 10},{10 + rightOffset, 0, 10},{10 + rightOffset, 10, 10} };
void quad(GLint n1, GLint n2, GLint n3, GLint n4)
{
	glBegin(GL_QUADS);
	glVertex3iv(points[n1]);
	glVertex3iv(points[n2]);
	glVertex3iv(points[n3]);
	glVertex3iv(points[n4]);
	glEnd();
}

void cube()
{
	quad(6, 2, 3, 7);
	quad(5, 1, 0, 4);
	quad(7, 3, 1, 5);
	quad(4, 0, 2, 6);
	quad(2, 0, 1, 3);
	quad(7, 5, 4, 6);
}

void drawCube(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	cube();

	glDisable(GL_POLYGON_OFFSET_FILL);
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);
	GLfloat fogColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_LINEAR);

	drawFrontPolygon();
	drawBackPolygon();

	drawCube();

	glFlush();
}

void code_16_14_4()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	camera = new Camera(Point(0, 36, 100), Rotator(-20, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_14_COMMON

#ifdef CHAPTER_16_COMMON
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // 黑色背景色
	glColor3f(1.0, 1.0, 1.0); // 白色绘制
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(widthMin, widthMax, heightMin, heightMax);
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

#ifdef CHAPTER_16_14_1
	code_16_14_1();
#endif

#ifdef CHAPTER_16_14_2
	code_16_14_2();
#endif

#ifdef CHAPTER_16_14_3
	code_16_14_3();
#endif

#ifdef CHAPTER_16_14_4
	code_16_14_4();
#endif

	glutMainLoop();
}
#endif


#endif 


