
#include "common.h"

#ifdef TOOLS
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 

GLsizei winWidth = 800, winHeight = 600;
struct Point2{ int x, y; };
struct Line
{
	Point2 pBegin;
	Point2 pEnd;
} ;

bool continueDraw = false;

inline int Round(const float a){ return int(a + 0.5); }

Point2 tempBegin;
Point2 tempCurrent;
vector<Line> drawLine;

void setPixel(GLint xCoord, GLint yCoord)
{
	glBegin(GL_POINTS);
	glVertex2i(xCoord, yCoord);
	glEnd();
}

void drawWord(Point2 point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}

void drawLines()
{	
	for (auto & line :drawLine)
	{
		glBegin(GL_LINES);
		glVertex2i(line.pBegin.x, line.pBegin.y);
		glVertex2i(line.pEnd.x, line.pEnd.y);
		glEnd();
	}

	if (continueDraw)
	{
		glBegin(GL_LINES);
		glVertex2i(tempBegin.x, tempBegin.y);
		glVertex2i(tempCurrent.x, tempCurrent.y);
		glEnd();
	}
}

void drawCood()
{
	char str[32] = "";
	sprintf_s(str, "%d,%d", tempCurrent.x, tempCurrent.y);
	Point2 coodPoint = { tempCurrent.x + 5, tempCurrent.y };
	if (coodPoint.y > winHeight - 10)
	{
		coodPoint.x += 8;
		coodPoint.y -= 10;
	}
	if (coodPoint.x > winWidth - 65)
	{
		coodPoint.x -= 65;
	}

	drawWord({ coodPoint.x, coodPoint.y }, str);
}


void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	drawLines();
	drawCood();

	glFlush();
}

void onMouseClick(int button, int state, int x, int y)
{
	Point2 mousePoint = { x, winHeight - y };
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (continueDraw)
		{
			drawLine.push_back({ tempBegin, mousePoint });
			tempBegin = mousePoint;
			tempCurrent = mousePoint;
		}
		else
		{
			tempBegin = mousePoint;
			tempCurrent = mousePoint;
			continueDraw = true;
		}		
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		continueDraw = false;
		tempCurrent = mousePoint;
	}
	drawFunc();
}

void onMouseActiveMove(int x, int y)
{
	Point2 mousePoint = { x, winHeight - y };

	tempCurrent = mousePoint;
	drawFunc();
}

void onMouseMove(int x, int y)
{
	Point2 mousePoint = { x, winHeight - y };

	tempCurrent = mousePoint;
	drawFunc();
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
}

void main(int argc, char** argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("»­Í¼¹¤¾ß");

	init();

	glutDisplayFunc(drawFunc);
	glutMouseFunc(onMouseClick);
	glutMotionFunc(onMouseActiveMove);
	glutPassiveMotionFunc(onMouseActiveMove);

	glutMainLoop();
}
#endif
