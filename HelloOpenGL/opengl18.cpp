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

#ifdef CHAPTER_18_5_2
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLubyte texArray[32][32][4];
	for (int y = 0; y < 32; y ++)
	{
		for (int x = 0; x < 32; x++)
		{
			if (y == 0 || y == 10 || y == 20)
			{
				texArray[y][x][0] = 255;
				texArray[y][x][1] = 0;
				texArray[y][x][2] = 0;
				texArray[y][x][3] = 255;
			}
			else
			{
				texArray[y][x][0] = 255;
				texArray[y][x][1] = 255;
				texArray[y][x][2] = 255;
				texArray[y][x][3] = 255;
			}			
		}
	}

	GLfloat vertex1[] = { -20, 0, 0 };
	GLfloat vertex2[] = { 20, 0, 0 };
	GLfloat vertex3[] = { 20, 30, 0 };
	GLfloat vertex4[] = { -20, 30, 0 };

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, texArray);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3fv(vertex1);
		glTexCoord2f(1.0, 0.0); glVertex3fv(vertex2);
		glTexCoord2f(1.0, 1.0); glVertex3fv(vertex3);
		glTexCoord2f(0.0, 1.0); glVertex3fv(vertex4);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glFlush();
}

void code_18_5_2()
{
	glutDisplayFunc(displayFcn);
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

#ifdef CHAPTER_18_5_2
	widthMin = -400;
	widthMax = 400;
	heightMin = -300;
	heightMax = 300;
#endif

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

#ifdef CHAPTER_18_5_2
	code_18_5_2();
#endif

	glutMainLoop();
}
#endif


#endif 


