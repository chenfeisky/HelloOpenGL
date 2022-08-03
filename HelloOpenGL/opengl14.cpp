#include "common.h"

#if CHAPTER==14
#include "opengl14h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_14_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;
#endif

#ifdef CHAPTER_14_8_2

GLsizei winWidth = 600, winHeight = 600;
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;

class wcPt3D
{
public:
	GLfloat x, y, z;
};

static void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void plotPoint(wcPt3D bezCurvePt)
{
	glBegin(GL_POINTS);
	glVertex2f(bezCurvePt.x, bezCurvePt.y);
	glEnd();
}

void binomialCoeffs(GLint n, GLint* C)
{
	GLint k, j;
	for (k = 0; k <= n; k++)
	{
		C[k] = 1;
		for (j = n; j >= k + 1; j--)
			C[k] *= j;
		for (j = n - k; j >= 2; j--)
			C[k] /= j;
	}
}

void computeBezPt(GLfloat u, wcPt3D* bezPt, GLint nCtrlPts, wcPt3D* ctrlPts, GLint* C)
{
	GLint k, n = nCtrlPts - 1;
	GLfloat bezBlendFcn;

	bezPt->x = bezPt->y = bezPt->z = 0.0;

	for (k = 0; k < nCtrlPts; k++)
	{
		bezBlendFcn = C[k] * pow(u, k) * pow(1 - u, n - k);
		bezPt->x += ctrlPts[k].x * bezBlendFcn;
		bezPt->y += ctrlPts[k].y * bezBlendFcn;
		bezPt->z += ctrlPts[k].z * bezBlendFcn;
	}
}

void bezier(wcPt3D* ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
	wcPt3D bezCurvePt;
	GLfloat u;
	GLint* C, k;

	C = new GLint[nCtrlPts];

	binomialCoeffs(nCtrlPts - 1, C);

	for (k = 0; k <= nBezCurvePts; k++)
	{
		u = GLfloat(k) / GLfloat(nBezCurvePts);
		computeBezPt(u, &bezCurvePt, nCtrlPts, ctrlPts, C);
		plotPoint(bezCurvePt);
	}

	delete[]C;
}

void displayFcn(void)
{
	GLint nCtrlPts = 4, nBezCurvePts = 1000;
	wcPt3D ctrlPts[4] = { {-40.0, -40.0, 0.0}, {-10.0, 200.0, 0.0}, {10.0, -200.0, 0.0}, {40.0, 40.0, 0.0} };

	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(4);

	glColor3f(1.0, 0.0, 0.0);

	bezier(ctrlPts, nCtrlPts, nBezCurvePts);
	glFlush();

}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);

	glClear(GL_COLOR_BUFFER_BIT);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Bezier Curve");

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_14_16_1
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat ctrlPts[4][3] = { {-40.0, -40.0, 0.0}, {-10.0, 200.0, 0.0}, {10.0, -200.0, 0.0}, {40.0, 40.0, 0.0} };
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts);
	glEnable(GL_MAP1_VERTEX_3);
	GLint k;
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (k = 0; k <= 50; k++)
		glEvalCoord1f(GLfloat(k) / 50.0);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for (k = 0; k < 4; k++)
		glVertex3fv(&ctrlPts[k][0]);
	glEnd();

	glFlush();

}
void code_14_16_1()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_14_16_1_1
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat ctrlPts[4][3] = { {-40.0, -40.0, 0.0}, {-10.0, 200.0, 0.0}, {10.0, -200.0, 0.0}, {40.0, 40.0, 0.0} };
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts);
	glEnable(GL_MAP1_VERTEX_3);
	GLint k;
	glColor3f(0.0, 0.0, 1.0);
	glMapGrid1f(50, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, 50);

	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for (k = 0; k < 4; k++)
		glVertex3fv(&ctrlPts[k][0]);
	glEnd();

	glFlush();

}
void code_14_16_1_1()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_14_16_2
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat ctrlPts[4][4][3] = {
	{ {-15, -15, 40}, {-5, -15, 20},
	  {-5, -15, -10}, {15, -15, 20} },
	{ {-15, -5, 10}, {-5, -5, 30},
	  {5, -5, -0}, {15, -5, -10} },
	{ {-15, 5, 40}, {-5, 5, 0},
	  {5, 5, 30}, {15, 5, 40} },
	{ {-15, 15, -20}, {-5, 15, -20},
	  {5, 15, 0}, {15, 15, -10} }
	};
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4, &ctrlPts[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	GLint k, j;
	glColor3f(0.0, 0.0, 1.0);
	for (k = 0; k <= 8; k++)
	{
		glBegin(GL_LINE_STRIP);
		for (j = 0; j <= 40; j++)
			glEvalCoord2f(GLfloat(j) / 40.0, GLfloat(k) / 8.0);
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (j = 0; j <= 40; j++)
			glEvalCoord2f(GLfloat(k) / 8.0, GLfloat(j) / 40.0);
		glEnd();
	}
	glFlush();
}
void code_14_16_2()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0 ));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
}
#endif

#ifdef CHAPTER_14_16_2_1
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat ctrlPts[4][4][3] = {
	{ {-15, -15, 40}, {-5, -15, 20},
	  {-5, -15, -10}, {15, -15, 20} },
	{ {-15, -5, 10}, {-5, -5, 30},
	  {5, -5, -0}, {15, -5, -10} },
	{ {-15, 5, 40}, {-5, 5, 0},
	  {5, 5, 30}, {15, 5, 40} },
	{ {-15, 15, -20}, {-5, 15, -20},
	  {5, 15, 0}, {15, 15, -10} }
	};
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4, &ctrlPts[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);

	glColor3f(0.0, 0.0, 1.0);
	glMapGrid2f(8, 0.0, 1.0, 8, 0.0, 1.0);
	glEvalMesh2(GL_LINE, 0, 8, 0, 8);
	glFlush();
}
void code_14_16_2_1()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
}
#endif


//////////////////////////////////////////////////////////////////////////
// CHAPTER_14_COMMON

#ifdef CHAPTER_14_COMMON
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
#ifdef CHAPTER_14_16_1
	widthMin = -400;
	widthMax = 400;
	heightMin = -300;
	heightMax = 300;
#endif

#ifdef CHAPTER_14_16_1_1
	widthMin = -400;
	widthMax = 400;
	heightMin = -300;
	heightMax = 300;
#endif

#ifdef CHAPTER_14_16_2_1
	widthMin = -4;
	widthMax = 4;
	heightMin = -3;
	heightMax = 3;
#endif

	//srand(time(0));
	srand(100);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("An Example OpenGL Program");

	init();

#ifdef CHAPTER_14_16_1
	code_14_16_1();
#endif

#ifdef CHAPTER_14_16_1_1
	code_14_16_1_1();
#endif

#ifdef CHAPTER_14_16_2
	code_14_16_2();
#endif

#ifdef CHAPTER_14_16_2_1
	code_14_16_2_1();
#endif

	glutMainLoop();
}
#endif


#endif 


