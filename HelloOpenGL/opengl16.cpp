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
	glEnable(GL_CULL_FACE);

	glutWireCube(10.f);
	//glutSolidCube(10.f);
	glFlush();
}

void code_16_14_1()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}
#endif

#ifdef CHAPTER_16_14_3
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);

	glDisable(GL_POLYGON_OFFSET_FILL);

	glFlush();
}

void code_16_14_3()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
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

#ifdef CHAPTER_16_14_3
	code_16_14_3();
#endif

	glutMainLoop();
}
#endif


#endif 


