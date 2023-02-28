#include "common.h"

#if CHAPTER==20
#include "opengl20h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_20_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;
#endif

#ifdef CHAPTER_20_6_1_1
GLsizei winWidth = 400, winHeight = 300;

void init(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(3.0);
	
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

	winWidth = newWidth;
	winHeight = newHeight;
	glFlush();
}

void plotPoint(GLint x, GLint y)
{
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();
}

void mousePtPlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		plotPoint(xMouse, winHeight - yMouse);

	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Mouse Plot Points");

	init();

	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mousePtPlot);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_20_6_1_2
GLsizei winWidth = 400, winHeight = 300;
GLint ptCtr = 0;

class scrPt {
public:
	GLint x, y;
};

void init(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

	winWidth = newWidth;
	winHeight = newHeight;
	glFlush();
}
void drawLineSegment(scrPt endPt1, scrPt endPt2)
{
	glBegin(GL_LINES);
		glVertex2i(endPt1.x, endPt1.y);
		glVertex2i(endPt2.x, endPt2.y);
	glEnd();
}
void polyline(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	static scrPt endPt1, endPt2;
	if (ptCtr == 0)
	{
		if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		{
			endPt1.x = xMouse;
			endPt1.y = winHeight - yMouse;
			ptCtr = 1;
		}
		else
		{
			if (button == GLUT_RIGHT_BUTTON)
				exit(0);
		}
	}
	else
	{
		if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		{
			endPt2.x = xMouse;
			endPt2.y = winHeight - yMouse;
			drawLineSegment(endPt1, endPt2);
			endPt1 = endPt2;
		}
		else
		{
			if (button == GLUT_RIGHT_BUTTON)
				exit(0);
		}
	}

	glFlush();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Draw Interactive Polyline");

	init();

	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(polyline);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_20_6_2_1
GLsizei winWidth = 400, winHeight = 300;
GLint ptCtr = 0;

class scrPt {
public:
	GLint x, y;
};

void init(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(3.0);
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

	winWidth = newWidth;
	winHeight = newHeight;
	glFlush();
}

void plotPoint(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void curveDrawing(GLubyte curvePlotKey, GLint xMouse, GLint yMouse)
{
	GLint x = xMouse;
	GLint y = winHeight - yMouse;
	switch (curvePlotKey)
	{
	case 'c':
		plotPoint(x, y);
		break;
	default:
		break;
	}
	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Draw Interactive Polyline");

	init();

	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutKeyboardFunc(curveDrawing);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_20_6_2_2
GLsizei winWidth = 400, winHeight = 300;
GLfloat edgeLength = 10.f;

void init(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

	winWidth = newWidth;
	winHeight = newHeight;
	glFlush();
}

void fillSquare(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLint x1, x2, y1, y2;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		x1 = xMouse;
		y1 = winHeight - yMouse;
		x2 = x1 + edgeLength;
		y2 = y1 + edgeLength;
		glRecti(x1, y1, x2, y2);
	}
	else
	{
		if (button == GLUT_RIGHT_BUTTON)
			exit(0);
	}
	glFlush();
}

void enlargeSquare(GLubyte sizeFactor, GLint xMouse, GLint yMouse)
{
	switch (sizeFactor)
	{
	case '2':
		edgeLength *= 2;
		break;
	case '3':
		edgeLength *= 3;
		break;
	case '4':
		edgeLength *= 4;
		break;
	default:
		break;
	}
}

void reduceSquare(GLint reductionKey, GLint xMouse, GLint yMouse)
{
	switch (reductionKey)
	{
	case GLUT_KEY_F2:
		edgeLength /= 2;
		break;
	case GLUT_KEY_F3:
		edgeLength /= 4;
		break;
	default:
		break;
	}
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Draw Interactive Polyline");

	init();

	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(fillSquare);
	glutKeyboardFunc(enlargeSquare);
	glutSpecialFunc(reduceSquare);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_20_6_7
const GLint pickBuffSize = 32;
GLsizei winWidth = 400, winHeight = 400;
GLfloat edgeLength = 10.f;

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void rects(GLenum mode)
{
	if (mode == GL_SELECT)
		glPushName(30);
	glColor3f(1.0, 0.0, 0.0);
	glRecti(40, 130, 150, 260);

	if (mode == GL_SELECT)
		glPushName(10);
	glColor3f(0.0, 0.0, 1.0);
	glRecti(150, 130, 260, 260);

	if (mode == GL_SELECT)
		glPushName(20);
	glColor3f(0.0, 1.0, 0.0);
	glRecti(40, 40, 260, 130);
}

void processPicks(GLint nPicks, GLuint pickBuffer[])
{
	GLint j, k;
	GLuint objID, * ptr;
	printf("Number of objects picked = %d\n", nPicks);
	printf("\n");
	ptr = pickBuffer;

	for (j = 0; j < nPicks; j++)
	{
		objID = *ptr;

		printf(" Stack position = %d\n", objID);
		ptr++;

		printf(" Min depth = %g,", float(*ptr / 0x7fffffff));
		ptr++;

		printf(" Max depth = %g,", float(*ptr / 0x7fffffff));
		ptr++;

		printf(" stack IDs are: \n");
		for (k = 0; k < objID; k++)
		{
			printf(" %d ", *ptr);
			ptr++;
		}
		printf("\n\n");
	}
}

void pickRects(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLuint pickBufffer[pickBuffSize];
	GLint nPicks, vpArray[4];

	if (button != GLUT_LEFT_BUTTON || action != GLUT_DOWN)
		return;

	glSelectBuffer(pickBuffSize, pickBufffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGetIntegerv(GL_VIEWPORT, vpArray);
	gluPickMatrix(GLdouble(xMouse), GLdouble(vpArray[3] - yMouse), 5.0, 5.0, vpArray);
	gluOrtho2D(0.0, 300.0, 0.0, 300.0);
	rects(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glFlush();

	nPicks = glRenderMode(GL_RENDER);
	processPicks(nPicks, pickBufffer);
	glutPostRedisplay();
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	rects(GL_RENDER);
	glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 300.0, 0.0, 300.0);

	winWidth = newWidth;
	winHeight = newHeight;
	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example Pick Program");

	init();

	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(pickRects);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_20_7_1
GLsizei winWidth = 400, winHeight = 400;
GLfloat red = 1.0, green = 1.0, blue = 1.0;
GLenum fillMode = GL_SMOOTH;

void init(void)
{
	glClearColor(0.6, 0.6, 0.6, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 300.0, 0.0, 300.0);
}
void fillOption(GLint selectedOption)
{
	switch (selectedOption)
	{
	case 1: fillMode = GL_FLAT;   break;
	case 2: fillMode = GL_SMOOTH; break;
	}
	glutPostRedisplay();
}
void displayTriangle(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(fillMode);
	glColor3f(red, green, blue);

	glBegin(GL_TRIANGLES);
	glVertex2i(280, 20);
	glVertex2i(160, 280);
	glColor3f(red, 0.0, 0.0);
	glVertex2i(20, 100);
	glEnd();

	glFlush();
}

void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLfloat(newWidth), 0.0, GLfloat(newHeight));
	displayTriangle();
	glFlush();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Menu Example");

	init();

	glutDisplayFunc(displayTriangle);

	glutCreateMenu(fillOption);
	glutAddMenuEntry("Solid-Color Fill", 1);
	glutAddMenuEntry("Color-Interpolations Fill", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutReshapeFunc(reshapeFcn);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_20_7_3
GLsizei winWidth = 400, winHeight = 400;
GLfloat red = 1.0, green = 1.0, blue = 1.0;
GLenum renderingMode = GL_SMOOTH;

void init(void)
{
	glClearColor(0.6, 0.6, 0.6, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 300.0, 0.0, 300.0);
}

void mainMenu(GLint renderingOption)
{
	switch (renderingOption)
	{
	case 1: renderingMode = GL_FLAT;   break;
	case 2: renderingMode = GL_SMOOTH; break;
	}
	glutPostRedisplay();
}

void colorSubMenu(GLint colorOption)
{
	switch (colorOption)
	{
	case 1: 
		red = 0.0, green = 0.0, blue = 1.0;
		break;
	case 2:
		red = 0.0, green = 1.0, blue = 0.0;
		break;
	case 3:
		red = 1.0, green = 1.0, blue = 1.0;
		break;
	}
	glutPostRedisplay();
}

void displayTriangle(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glShadeModel(renderingMode);
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
		glVertex2i(280, 20);
		glVertex2i(160, 280);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2i(20, 100);
	glEnd();

	glFlush();
}

void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLfloat(newWidth), 0.0, GLfloat(newHeight));
	displayTriangle();
	glFlush();
}
void main(int argc, char** argv)
{
	GLint subMenu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Submenu Example");

	init();
	glutDisplayFunc(displayTriangle);

	subMenu = glutCreateMenu(colorSubMenu);
		glutAddMenuEntry("Blue", 1);
		glutAddMenuEntry("Green", 2);
		glutAddMenuEntry("White", 3);

	glutCreateMenu(mainMenu);
		glutAddMenuEntry("Solid-Color Fill", 1);
		glutAddMenuEntry("Color-Interpolations Fill", 2);
		glutAddSubMenu("Color", subMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutReshapeFunc(reshapeFcn);

	glutMainLoop();
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_20_COMMON

#ifdef CHAPTER_20_COMMON
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

	glutMainLoop();
}
#endif


#endif 


