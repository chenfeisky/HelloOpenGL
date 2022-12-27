#include "common.h"

#if CHAPTER==18
#include "opengl18h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_18_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;
#endif

#ifdef CHAPTER_18_5_1
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLint k;
	GLubyte texLine[16];
	for (k = 0; k <= 2; k += 2)
	{
		texLine[4 * k] = 0;
		texLine[4 * k + 1] = 255;
		texLine[4 * k + 2] = 0;
		texLine[4 * k + 3] = 255;
	}

	for (k = 1; k <= 3; k += 2)
	{
		texLine[4 * k] = 255;
		texLine[4 * k + 1] = 0;
		texLine[4 * k + 2] = 0;
		texLine[4 * k + 3] = 255;
	}

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, texLine);

	glEnable(GL_TEXTURE_1D);

	glBegin(GL_LINES);
	glTexCoord1f(0.0);
	GLfloat endPt1[] = { -20, 0, 0 };
	glVertex3fv(endPt1);
	glTexCoord1f(1.0);
	GLfloat endPt2[] = { 20, 0, 0 };
	glVertex3fv(endPt2);
	glEnd();

	glDisable(GL_TEXTURE_1D);


	glFlush();
}

void code_18_5_1()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_18_COMMON

#ifdef CHAPTER_18_COMMON
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

#ifdef CHAPTER_18_5_1
	code_18_5_1();
#endif

	glutMainLoop();
}
#endif


#endif 


