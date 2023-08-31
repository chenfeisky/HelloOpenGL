#include "common.h"

#if CHAPTER==23
#include "opengl23h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_23_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;
#endif

#ifdef CHAPTER_23_1_9_1
GLsizei winWidth = 600, winHeight = 600;
GLfloat xComplexMin = -0.25, xComplexMax = 1.25;
GLfloat yComplexMin = -0.75, yComplexMax = 0.75;
struct complexNum
{
	GLfloat x, y;
};
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
void plotPoint(complexNum z)
{
	glBegin(GL_POINTS);
		glVertex2f(z.x, z.y);
	glEnd();
}
void solveQuadraticEq(complexNum lambda, complexNum* z)
{
	GLfloat lambdaMagSq, discrMag;
	complexNum discr;
	static complexNum fourOverLambda = { 0.0, 0.0 };
	static GLboolean firstPoint = true;

	if (firstPoint)
	{
		lambdaMagSq = lambda.x * lambda.x + lambda.y * lambda.y;
		fourOverLambda.x = 4.0 * lambda.x / lambdaMagSq;
		fourOverLambda.y = -4.0 * lambda.y / lambdaMagSq;
		firstPoint = false;
	}

	discr.x = 1.0 - (z->x * fourOverLambda.x - z->y * fourOverLambda.y);
	discr.y = z->x * fourOverLambda.y + z->y * fourOverLambda.x;
	discrMag = sqrt(discr.x * discr.x + discr.y * discr.y);

	if (discrMag + discr.x < 0)
		z->x = 0;
	else
		z->x = sqrt((discrMag + discr.x) / 2.0);
	if (discrMag - discr.x < 0)
		z->y = 0;
	else
		z->y = 0.5 * sqrt((discrMag - discr.x) / 2.0);

	if (rand() < RAND_MAX / 2)
	{
		z->x = -z->x;
		z->y = -z->y;
	}

	if (discr.y < 0)
		z->x = -z->x;

	z->x = 0.5 * (1 - z->x);
}
void selfSqTransf(complexNum lambda, complexNum z, GLint numPoints)
{
	GLint k;
	for (k = 0; k < 10; k++)
		solveQuadraticEq(lambda, &z);

	for (k = 0; k < numPoints; k++)
	{
		solveQuadraticEq(lambda, &z);
		plotPoint(z);
	}
}
void displayFcn(void)
{
	GLint numPoints = 10000;
	complexNum lambda = { 3.0, 0.0 };
	//complexNum lambda = { 2.0, 1.0 };
	complexNum z0 = { 1.5, 4.0 };
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);

	selfSqTransf(lambda, z0, numPoints);
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xComplexMin, xComplexMax, yComplexMin, yComplexMax);
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Self-Squaring Fractal");
	int glewCode = glewInit();
	if (glewCode != GLEW_OK)
	{
		printf("glewInit err:%d\n", glewCode);
		return;
	}
	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_23_1_9_2
GLsizei winWidth = 500, winHeight = 500;
GLfloat xComplexMin = -2.00, xComplexMax = 0.50;
GLfloat yComplexMin = -1.25, yComplexMax = 1.25;
GLfloat complexWidth = xComplexMax - xComplexMin;
GLfloat complexHeight = yComplexMax - yComplexMin;
class complexNum
{
public:
	GLfloat x, y;
};
struct color { GLfloat r, g, b; };
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
void plotPoint(complexNum z)
{
	glBegin(GL_POINTS);
	glVertex2f(z.x, z.y);
	glEnd();
}
complexNum complexSquare(complexNum z)
{
	complexNum zSquare;
	zSquare.x = z.x * z.x - z.y * z.y;
	zSquare.y = 2 * z.x * z.y;
	return zSquare;
}
GLint mandelSqTransf(complexNum z0, GLint maxIter)
{
	complexNum z = z0;
	GLint count = 0;

	while ((z.x * z.x + z.y * z.y <= 4.0) && (count < maxIter))
	{
		z = complexSquare(z);
		z.x += z0.x;
		z.y += z0.y;
		count++;
	}
	return count;
}
void mandelbrot(GLint nx, GLint ny, GLint maxIter)
{
	complexNum z, zIncr;
	color ptColor;
	GLint iterCount;
	zIncr.x = complexWidth / GLfloat(nx);
	zIncr.y = complexHeight / GLfloat(ny);
	for (z.x = xComplexMin; z.x < xComplexMax; z.x += zIncr.x)
	{
		for (z.y = yComplexMin; z.y < yComplexMax; z.y += zIncr.y)
		{
			iterCount = mandelSqTransf(z, maxIter);
			if (iterCount >= maxIter)
			{
				ptColor.r = ptColor.g = ptColor.b = 0.0;
			}				
			else if (iterCount > (maxIter / 8))
			{
				ptColor.r = 1.0;
				ptColor.g = 0.5;
				ptColor.b = 0.0;
			}
			else if (iterCount > (maxIter / 10))
			{
				ptColor.r = 1.0;
				ptColor.g = ptColor.b = 0.0;
			}
			else if (iterCount > (maxIter / 20))
			{
				ptColor.b = 0.5;
				ptColor.r = ptColor.g = 0.0;
			}
			else if (iterCount > (maxIter / 40))
			{
				ptColor.r = ptColor.g = 1.0;
				ptColor.b = 0.0;
			}
			else if (iterCount > (maxIter / 100))
			{
				ptColor.r = ptColor.b = 0.0;
				ptColor.g = 0.3;
			}
			else
			{
				ptColor.r = 0.0;
				ptColor.g = ptColor.b = 1.0;
			}
			glColor3f(ptColor.r, ptColor.g, ptColor.b);
			plotPoint(z);
		}
	}
}
void displayFcn(void)
{
	GLint nx = 1000, ny = 1000, maxIter = 1000;
	glClear(GL_COLOR_BUFFER_BIT);
	mandelbrot(nx, ny, maxIter);
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xComplexMin, xComplexMax, yComplexMin, yComplexMax);
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Mandelbrot Set");
	int glewCode = glewInit();
	if (glewCode != GLEW_OK)
	{
		printf("glewInit err:%d\n", glewCode);
		return;
	}
	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);

	glutMainLoop();
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_23_COMMON

#ifdef CHAPTER_23_COMMON
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("An Example OpenGL Program");
	int glewCode = glewInit();
	if (glewCode != GLEW_OK)
	{
		printf("glewInit err:%d\n", glewCode);
		return;
	}
	init();

	glutMainLoop();
}
#endif


#endif 


