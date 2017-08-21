#include "common.h"

#ifdef CHAPTER_8
#include "opengl8h.h"

#ifdef CHAPTER_8_COMMON
GLsizei winWidth = 800, winHeight = 600;
void setPixel(GLint xCoord, GLint yCoord)
{
	glBegin(GL_POINTS);
	glVertex2i(xCoord, yCoord);
	glEnd();

	//glRasterPos2d(xCoord, yCoord);
	//static GLfloat a[4];
	//memset(a, 0, 4 * sizeof(GLfloat));
	//glGetFloatv(GL_CURRENT_COLOR, a);
	//glDrawPixels(1, 1, GL_RGB, GL_FLOAT, a);
}
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
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 1.0);
	glViewport(0, 0, 300, 300);
	triangle(verts);

	glColor3f(1.0, 0.0, 0.0);
	glViewport(300, 0, 300, 300);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	triangle(verts);
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

#ifdef CHAPTER_8_4_16_test
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
wcPt2D verts[3] = { { 10.0, 10.0 },{ 40.0, 10.0 },{ 25.0, 40.0 } };
wcPt2D verts2[3] = { { 60.0, 10.0 },{ 90.0, 10.0 },{ 75.0, 40.0 } };
wcPt2D verts3[4] = { { 50.0, 0.0 },{ 100.0, 0.0 },{ 100.0, 50.0 },{ 50.0, 50.0 } };
int w1, w2, w3, w4;
void displayFunc2(void);
bool systemCall1 = true;
bool systemCall2 = true;
float curAngle = 0;
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
void rectBorder(float left, float right, float bottom, float top)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_LINE_LOOP);
	glVertex2f(left + 1, bottom + 1);
	glVertex2f(right - 1, bottom + 1);
	glVertex2f(right - 1, top - 1);
	glVertex2f(left + 1, top - 1);
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
	glColor3f(0.0, 0.0, 0.0);
	rectBorder(0.0, 100.0, 0.0, 50.0);
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
	glColor3f(0.0, 0.0, 0.0);
	rectBorder(0, 50.0, 0.0, 50.0);
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
	glColor3f(0.0, 0.0, 0.0);
	rectBorder(50.0, 100.0, 0.0, 50.0);
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

	// 以下显示同w1窗口，
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0.0, 100.0, 0.0, 50.0);
	//glViewport(0, 150, 600, 150);
	//glColor3f(0.0, 0.0, 0.0);
	//rectBorder(0.0, 100.0, 0.0, 50.0);
	//glColor3f(0.0, 0.0, 1.0);
	//triangle(verts);
	////glColor3f(1.0, 1.0, 1.0);
	////rect(verts3);
	//glColor3f(1.0, 0.0, 0.0);
	//triangle(verts2);
	//
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0, 50.0, 0.0, 50.0);
	//glViewport(0, 0, 300, 150);
	//glColor3f(0.0, 0.0, 0.0);
	//rectBorder(0, 50.0, 0.0, 50.0);
	//glColor3f(0.0, 0.0, 1.0);
	//triangle(verts);
	////glColor3f(1.0, 1.0, 1.0);
	////rect(verts3);
	//glColor3f(1.0, 0.0, 0.0);
	//triangle(verts2);
	//
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(50.0, 100.0, 0.0, 50.0);
	//glViewport(300, 0, 300, 150);
	////glViewport(100, 0, 300, 150);
	//glColor3f(0.0, 0.0, 0.0);
	//rectBorder(50.0, 100.0, 0.0, 50.0);
	//glColor3f(0.0, 0.0, 1.0);
	//triangle(verts);
	////glColor3f(1.0, 1.0, 1.0);
	////rect(verts3);
	//glColor3f(1.0, 0.0, 0.0);
	//triangle(verts2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 50.0);
	//glViewport(0, 0, 600, 300);
	glColor3f(0.0, 0.0, 0.0);
	rectBorder(0.0, 100.0, 0.0, 50.0);
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

		//glutSetWindow(w3);
		//glutPostRedisplay();
		//glutReshapeWindow(400, 300);
		//glutFullScreen();
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
void displayFunc3(void)
{
	int a = glutGetWindow();

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 50.0);
	//glViewport(0, 0, 600, 300);
	glColor3f(0.0, 0.0, 0.0);
	rectBorder(0.0, 100.0, 0.0, 50.0);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	//glColor3f(1.0, 1.0, 1.0);
	//rect(verts3);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts2);

	//glutSetWindow(w4);
	//glutReshapeWindow(400, 300);

	glFlush();
}
void displayFunc4(void)
{
	int a = glutGetWindow();

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 50.0);
	glViewport(0, 0, 300, 150);
	glColor3f(0.0, 0.0, 0.0);
	rectBorder(0.0, 100.0, 0.0, 50.0);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	//glColor3f(1.0, 1.0, 1.0);
	//rect(verts3);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts2);

	glFlush();
}
void winReshapeFcn2(GLint newWidth, GLint newHeight)
{
	//glViewport(0, 0, newWidth, newHeight);
}
void winReshapeFcn3(GLint newWidth, GLint newHeight)
{
	printf("winReshapeFcn4\n");
	//glViewport(0, 0, newWidth, newHeight);
}
void winReshapeFcn4(GLint newWidth, GLint newHeight)
{
	printf("winReshapeFcn4\n");
	//glViewport(0, 0, newWidth, newHeight);
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
	glutReshapeFunc(winReshapeFcn2);
	a = glutGetWindow();

	glutInitWindowPosition(50, 800);
	glutInitWindowSize(600, 300);
	w3 = glutCreateWindow("Example3");
	init();
	glutDisplayFunc(displayFunc3);
	glutReshapeFunc(winReshapeFcn3);
	a = glutGetWindow();

	w4 = glutCreateSubWindow(w3, 0, 0, 300, 150);
	init();
	glutDisplayFunc(displayFunc4);
	glutReshapeFunc(winReshapeFcn4);
	a = glutGetWindow();

	glutMainLoop();
}
#endif

#ifdef CHAPTER_8_7_1
class wcPt2D
{
public:
	GLfloat x, y;
};
inline GLint Round(const GLfloat a)
{
	return GLint(a + 0.5);
}
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
const GLint winLeftBitCode = 0x01;
const GLint winRightBitCode = 0x02;
const GLint winBottomBitCode = 0x04;
const GLint winTopBitCode = 0x08;
inline GLint inside(GLint code)
{
	return GLint(!code);
}
inline GLint reject(GLint code1, GLint code2)
{
	return GLint(code1 & code2);
}
inline GLint accept(GLint code1, GLint code2)
{
	return GLint(!(code1 | code2));
}
GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax)
{
	GLubyte code = 0x00;
	if (pt.x < winMin.x)
		code = code | winLeftBitCode;
	if (pt.x > winMax.x)
		code = code | winRightBitCode;
	if (pt.y < winMin.y)
		code = code | winBottomBitCode;
	if (pt.y > winMax.y)
		code = code | winTopBitCode;
	return (code);
}
void swapPts(wcPt2D* p1, wcPt2D* p2)
{
	wcPt2D tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}
void swapCodes(GLubyte* c1, GLubyte* c2)
{
	GLubyte tmp;
	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}
void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2)
{
	GLubyte code1, code2;
	GLint done = false, plotLine = false;
	GLfloat m;
	while (!done)
	{
		code1 = encode(p1, winMin, winMax);
		code2 = encode(p2, winMin, winMax);
		if (accept(code1, code2))
		{
			done = true;
			plotLine = true;
		}
		else if (reject(code1, code2))
			done = true;
		else
		{
			if (inside(code1))
			{
				swapPts(&p1, &p2);
				swapCodes(&code1, &code2);
			}
			if (p2.x != p1.x)
				m = (p2.y - p1.y) / (p2.x - p1.x);
			if (code1 & winLeftBitCode)
			{
				p1.y += (winMin.x - p1.x) * m;
				p1.x = winMin.x;
			}
			else if (code1 & winRightBitCode)
			{
				p1.y += (winMax.x - p1.x) * m;
				p1.x = winMax.x;
			}
			else if (code1 & winBottomBitCode)
			{
				if (p2.x != p1.x)
					p1.x += (winMin.y - p1.y) / m;
				p1.y = winMin.y;
			}
			else if (code1 & winTopBitCode)
			{
				if (p2.x != p1.x)
					p1.x += (winMax.y - p1.y) / m;
				p1.y = winMax.y;
			}
		}
	}
	if (plotLine)
		//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D winMin = {200, 220}, winMax = {520, 380};

	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	wcPt2D p1, p2;
	p1 = {106, 475}, p2 = {578, 120};
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	p1 = { 79, 346 }, p2 = { 688, 256 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	p1 = { 401, 434 }, p2 = { 294, 260 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	p1 = { 561, 399 }, p2 = { 627, 191 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	p1 = { 134, 313 }, p2 = { 378, 174 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	p1 = { 55, 249 }, p2 = { 273, 122 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	p1 = { 139, 431 }, p2 = { 139, 134 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	p1 = { 253, 440 }, p2 = { 253, 186 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	p1 = { 424, 249 }, p2 = { 479, 328 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	
	glFlush();
}
void code_8_7_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_7_2
class wcPt2D
{
private:
	GLfloat x, y;
public:
	wcPt2D()
	{
		x = y = 0.0;
	}
	void setCoords(GLfloat xCoord, GLfloat yCoord)
	{
		x = xCoord;
		y = yCoord;
	}
	GLfloat getx() const
	{
		return x;
	}
	GLfloat gety() const
	{
		return y;
	}
};
inline GLint Round(const GLfloat a)
{
	return GLint(a + 0.5);
}
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
GLint clipTest(GLfloat p, GLfloat q, GLfloat* u1, GLfloat* u2)
{
	GLfloat r;
	GLint returnValue = true;

	if (p < 0.0)
	{
		r = q / p;
		if (r > *u2)
			returnValue = false;
		else if (r > *u1)
			*u1 = r;
	}
	else if (p > 0.0)
	{
		r = q / p;
		if (r < *u1)
			returnValue = false;
		else if (r < *u2)
			*u2 = r;
	}
	else if (q < 0.0)
		returnValue = false;

	return (returnValue);
}
void lineClipLiangBarsk(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2)
{
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.getx() - p1.getx(), dy;
	if(clipTest(-dx, p1.getx() - winMin.getx(), &u1, &u2))
		if (clipTest(dx, winMax.getx() - p1.getx(), &u1, &u2))
		{
			dy = p2.gety() - p1.gety();
			if (clipTest(-dy, p1.gety() - winMin.gety(), &u1, &u2))
				if (clipTest(dy, winMax.gety() - p1.gety(), &u1, &u2))
				{
					if (u2 < 1.0)
					{
						p2.setCoords(p1.getx() + u2 * dx, p1.gety() + u2 * dy);
					}
					if (u1 > 0.0)
					{
						p1.setCoords(p1.getx() + u1 * dx, p1.gety() + u1 * dy);
					}
					//lineBres(Round(p1.getx()), Round(p1.gety()), Round(p2.getx()), Round(p2.gety())); // 精确到浮点数绘图
					lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
				}
		}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D winMin, winMax;
	winMin.setCoords(200, 220);
	winMax.setCoords(520, 380);

	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.getx(), winMin.gety());
	glVertex2f(winMax.getx(), winMin.gety());
	glVertex2f(winMax.getx(), winMax.gety());
	glVertex2f(winMin.getx(), winMax.gety());
	glEnd();

	wcPt2D p1, p2;
	p1.setCoords(106, 475);
	p2.setCoords(578, 120);
	glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

p1.setCoords(79, 346);
p2.setCoords(688, 256);
glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

p1.setCoords(401, 434);
p2.setCoords(294, 260);
glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

p1.setCoords(561, 399);
p2.setCoords(627, 191);
glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

p1.setCoords(134, 313);
p2.setCoords(378, 174);
glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

p1.setCoords(55, 249);
p2.setCoords(273, 122);
glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

p1.setCoords(139, 431);
p2.setCoords(139, 134);
glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

p1.setCoords(253, 440);
p2.setCoords(253, 186);
glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

p1.setCoords(424, 249);
p2.setCoords(479, 328);
glColor3f(1.0, 1.0, 1.0);
lineBres(p1.getx(), p1.gety(), p2.getx(), p2.gety());
glColor3f(1.0, 0.0, 0.0);
lineClipLiangBarsk(winMin, winMax, p1, p2);

glFlush();
}
void code_8_7_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_8_1
class wcPt2D
{
public:
	GLfloat x, y;
};
typedef enum { Left, Right, Bottom, Top } Boundary;
const GLint nClip = 4;
vector<wcPt2D> sPoint[nClip];
GLint inside(wcPt2D p, Boundary b, wcPt2D wMin, wcPt2D wMax)
{
	switch (b)
	{
	case Left:
		if (p.x < wMin.x)
			return (false);
		break;
	case Right:
		if (p.x > wMax.x)
			return (false);
		break;
	case Bottom:
		if (p.y < wMin.y)
			return (false);
		break;
	case Top:
		if (p.y > wMax.y)
			return (false);
		break;
	}
	return (true);
}
GLint cross(wcPt2D p1, wcPt2D p2, Boundary winEdge, wcPt2D wMin, wcPt2D wMax)
{
	if (inside(p1, winEdge, wMin, wMax) == inside(p2, winEdge, wMin, wMax))
		return (false);
	else
		return (true);
}
wcPt2D intersect(wcPt2D p1, wcPt2D p2, Boundary winEdge, wcPt2D wMin, wcPt2D wMax)
{
	wcPt2D iPt;
	GLfloat m;

	if (p1.x != p2.x)
		m = (p1.y - p2.y) / (p1.x - p2.x);
	switch (winEdge)
	{
	case Left:
		iPt.x = wMin.x;
		iPt.y = p2.y + (wMin.x - p2.x) * m;
		break;
	case Right:
		iPt.x = wMax.x;
		iPt.y = p2.y + (wMax.x - p2.x) * m;
		break;
	case Bottom:
		iPt.y = wMin.y;
		if (p1.x != p2.x)
			iPt.x = p2.x + (wMin.y - p2.y) / m;
		else
			iPt.x = p2.x;
		break;
	case Top:
		iPt.y = wMax.y;
		if (p1.x != p2.x)
			iPt.x = p2.x + (wMax.y - p2.y) / m;
		else
			iPt.x = p2.x;
		break;
	default:
		break;
	}
	return (iPt);
}
void clipPoint(wcPt2D p, Boundary winEdge, wcPt2D wMin, wcPt2D wMax, wcPt2D* pOut, int* cnt, wcPt2D* first[], wcPt2D* s)
{
	wcPt2D iPt;
	if (!first[winEdge])
		first[winEdge] = new wcPt2D{p.x, p.y};
	else
	{
		if (cross(p, s[winEdge], winEdge, wMin, wMax))
		{
			iPt = intersect(p, s[winEdge], winEdge, wMin, wMax);
			if (winEdge < Top)
				clipPoint(iPt, (Boundary)(winEdge + 1), wMin, wMax, pOut, cnt, first, s);
			else
			{
				pOut[*cnt] = iPt;
				(*cnt)++;
			}				
		}
	}

	s[winEdge] = p;
	sPoint[winEdge].push_back(p);

	if (inside(p, winEdge, wMin, wMax))
	{
		if (winEdge < Top)
			clipPoint(p, (Boundary)(winEdge + 1), wMin, wMax, pOut, cnt, first, s);
		else
		{
			pOut[*cnt] = p;
			(*cnt)++;
		}
	}
}
void closeClip(wcPt2D wMin, wcPt2D wMax, wcPt2D* pOut, GLint* cnt, wcPt2D* first[], wcPt2D* s)
{
	wcPt2D pt;
	Boundary winEdge;
	for (winEdge = Left; winEdge <= Top; winEdge = (Boundary)(winEdge + 1))
	{
		if (cross(s[winEdge], *first[winEdge], winEdge, wMin, wMax))
		{
			pt = intersect(s[winEdge], *first[winEdge], winEdge, wMin, wMax);
			if (winEdge < Top)
				clipPoint(pt, (Boundary)(winEdge + 1), wMin, wMax, pOut, cnt, first, s);
			else
			{
				pOut[*cnt] = pt;
				(*cnt)++;
			}
		}
	}
}
GLint polygonClipSuthHodg(wcPt2D wMin, wcPt2D wMax, GLint n, wcPt2D* pIn, wcPt2D* pOut)
{
	for (int i = 0; i < 4; i++)
	{
		sPoint[i].clear();
	}
	wcPt2D* first[nClip] = { 0, 0, 0, 0 }, s[nClip];
	GLint k, cnt = 0;
	for (k = 0; k < n; k++)
		clipPoint(pIn[k], Left, wMin, wMax, pOut, &cnt, first, s);
	closeClip(wMin, wMax, pOut, &cnt, first, s);
	
	printf("==================================================\n");
	for (int i = 0; i < 4; i++)
	{
		printf("%0.2f,%0.2f  ", first[i]->x, first[i]->y);
	}
	printf("\n");

	int max = 0;
	for (int i = 0; i < 4; i++)
	{
		if (max < sPoint[i].size())
			max = sPoint[i].size();
	}

	for (int i = 0; i < max; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i < sPoint[j].size())
			{
				printf("%0.2f,%0.2f  ", sPoint[j][i].x, sPoint[j][i].y);
			}
			else
			{
				printf("             ");
			}
		}
		printf("\n");			
	}
	return (cnt);
}
void polygon(wcPt2D * verts, int n)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
		glVertex2f(verts[i].x, verts[i].y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	{
		glColor3f(1.0, 1.0, 1.0);
		wcPt2D winMin = { 100, 400 }, winMax = { 250, 500 };

		glBegin(GL_LINE_LOOP);
		glVertex2f(winMin.x, winMin.y);
		glVertex2f(winMax.x, winMin.y);
		glVertex2f(winMax.x, winMax.y);
		glVertex2f(winMin.x, winMax.y);
		glEnd();

		wcPt2D verts[3] = { {137, 362},{170, 445},{ 25, 431 } };
		polygon(verts, 3);
		wcPt2D pOut[16];
		int n = polygonClipSuthHodg(winMin, winMax, 3, verts, pOut);
		glColor3f(1.0, 0.0, 0.0);
		polygon(pOut, n);
	}

	{
		glColor3f(1.0, 1.0, 1.0);
		wcPt2D winMin = { 400, 400 }, winMax = { 650, 500 };

		glBegin(GL_LINE_LOOP);
		glVertex2f(winMin.x, winMin.y);
		glVertex2f(winMax.x, winMin.y);
		glVertex2f(winMax.x, winMax.y);
		glVertex2f(winMin.x, winMax.y);
		glEnd();

		wcPt2D verts[3] = { { 549, 369 },{ 572, 546 },{ 657, 419 } };
		polygon(verts, 3);
		wcPt2D pOut[16];
		int n = polygonClipSuthHodg(winMin, winMax, 3, verts, pOut);
		glColor3f(1.0, 0.0, 0.0);
		polygon(pOut, n);
	}

	{
		glColor3f(1.0, 1.0, 1.0);
		wcPt2D winMin = { 100, 100 }, winMax = { 250, 200 };

		glBegin(GL_LINE_LOOP);
		glVertex2f(winMin.x, winMin.y);
		glVertex2f(winMax.x, winMin.y);
		glVertex2f(winMax.x, winMax.y);
		glVertex2f(winMin.x, winMax.y);
		glEnd();

		wcPt2D verts[3] = { { 308, 182 },{ 232, 249 },{ 72, 124 } };
		polygon(verts, 3);
		wcPt2D pOut[16];
		int n = polygonClipSuthHodg(winMin, winMax, 3, verts, pOut);
		glColor3f(1.0, 0.0, 0.0);
		polygon(pOut, n);
	}

	{
		glColor3f(1.0, 1.0, 1.0);
		wcPt2D winMin = { 500, 100 }, winMax = { 650, 200 };

		glBegin(GL_LINE_LOOP);
		glVertex2f(winMin.x, winMin.y);
		glVertex2f(winMax.x, winMin.y);
		glVertex2f(winMax.x, winMax.y);
		glVertex2f(winMin.x, winMax.y);
		glEnd();

		wcPt2D verts[4] = { { 577, 75 },{ 421, 187 },{ 454, 258 },{ 667,75 } };
		polygon(verts, 4);
		wcPt2D pOut[16];
		int n = polygonClipSuthHodg(winMin, winMax, 4, verts, pOut);
		glColor3f(1.0, 0.0, 0.0);
		polygon(pOut, n);
	}
		
	glFlush();
}
void code_8_8_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_1
struct Point { float x; float y; };
typedef Point Vec;
Point p0 = {100, 200};
Vec V = {155, 355};
struct Matrix
{
	Matrix(int row, int col)
	{
		_data.assign(row, std::vector<float>(col, 0));
		_row = row;
		_col = col;
	}
	std::vector<float>& operator [](int row)
	{
		return _data[row];
	}
	operator GLfloat *()
	{
		_elementData.clear();
		for (int j = 0; j < _col; j++)
		{
			for (int i = 0; i < _row; i++)
			{
				_elementData.push_back(_data[i][j]);
			}
		}
		return &_elementData[0];
	}
	std::vector<std::vector<float>> _data;
	std::vector<float> _elementData;
	int _row;
	int _col;
};
Matrix operator *(Matrix& m1, Matrix& m2)
{
	assert(m1._col == m2._row);

	Matrix ret(m1._row, m2._col);
	for (int row = 0; row < m1._row; row++)
	{
		for (int col = 0; col < m2._col; col++)
		{
			ret[row][col] = 0;
			for (int i = 0; i < m1._col; i++)
			{
				ret[row][col] += m1[row][i] * m2[i][col];
			}
		}
	}
	return ret;
}
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
Matrix translateMatrix(float tx, float ty)
{
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(Point pivotPt, float theta)
{
	Matrix matRot(3, 3);
	matrixSetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	return matRot;
}
void transformPoints(Matrix& m, std::vector<Point>& points)
{
	Matrix point(3, 1);
	Matrix temp(3, 1);
	for (auto& p : points)
	{
		point[0][0] = p.x;
		point[1][0] = p.y;
		point[2][0] = 1;
		auto temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void triangle(const std::vector<Point>& points)
{
	glBegin(GL_TRIANGLES);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void coordinate(Point o, Vec V)
{
	float distanceV = std::sqrt(V.x * V.x + V.y * V.y);
	V.x = V.x / distanceV;
	V.y = V.y / distanceV;
	Point y = { o.x + V.x * winWidth, o.y + V.y * winWidth };
	Point x = { o.x + V.y * winWidth, o.y - V.x * winWidth };
	glBegin(GL_LINES);
	glVertex2f(o.x, o.y);
	glVertex2f(x.x, x.y);
	glVertex2f(o.x, o.y);
	glVertex2f(y.x, y.y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> tri = { { 250.f, 200.f },{ 550.f, 200.f },{ 400.f, 500.f } };

	Vec v, u;
	float distanceV = std::sqrt(V.x * V.x + V.y * V.y);
	v.x = V.x / distanceV;
	v.y = V.y / distanceV;
	u.x = v.y;
	u.y = -v.x;

	// 原始世界坐标系和观察坐标系
	glViewport(0, 300, 400, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 });
	coordinate(p0, V);
	triangle(tri);

	// 转换到观察坐标系 自定义矩阵 单位向量构造旋转矩阵
	glViewport(400, 300, 400, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 });
	auto temp = tri;
	Matrix r(3, 3);
	matrixSetIdentity(r);
	r[0][0] = u.x;
	r[0][1] = u.y;
	r[1][0] = v.x;
	r[1][1] = v.y;
	transformPoints(r * translateMatrix(-p0.x, -p0.y), temp);
	triangle(temp);

	// 转换到观察坐标系 OpenGL矩阵 单位向量构造旋转矩阵
	glViewport(0, 0, 400, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 });
	Matrix r1(4, 4);
	matrixSetIdentity(r1);
	r1[0][0] = u.x;
	r1[0][1] = u.y;
	r1[1][0] = v.x;
	r1[1][1] = v.y;
	glMultMatrixf(r1);
	glTranslatef(-p0.x, -p0.y, 0.f);
	triangle(tri);

	// 转换到观察坐标系 OpenGL矩阵 直接计算角度旋转矩阵
	glViewport(400, 0, 400, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 });
	float arg = atan2(V.y, V.x);
	glRotatef((PI / 2 - arg) * 180 / PI, 0.f, 0.f, 1.f);
	glTranslatef(-p0.x, -p0.y, 0.f);
	triangle(tri);

	glFlush();
}
void code_8_exercise_1()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0 - 2, winWidth + 2, 0 - 2, winHeight + 2);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_2
struct Point { float x; float y; };
float xwmin = 40, ywmin = 50, xwmax = 340, ywmax = 250;
float xvmin = 0.3, yvmin = 0.4, xvmax = 0.7, yvmax = 0.7;
struct Matrix
{
	Matrix(int row, int col)
	{
		_data.assign(row, std::vector<float>(col, 0));
		_row = row;
		_col = col;
	}
	std::vector<float>& operator [](int row)
	{
		return _data[row];
	}
	operator GLfloat *()
	{
		_elementData.clear();
		for (int j = 0; j < _col; j++)
		{
			for (int i = 0; i < _row; i++)
			{
				_elementData.push_back(_data[i][j]);
			}
		}
		return &_elementData[0];
	}
	std::vector<std::vector<float>> _data;
	std::vector<float> _elementData;
	int _row;
	int _col;
};
Matrix operator *(Matrix& m1, Matrix& m2)
{
	assert(m1._col == m2._row);

	Matrix ret(m1._row, m2._col);
	for (int row = 0; row < m1._row; row++)
	{
		for (int col = 0; col < m2._col; col++)
		{
			ret[row][col] = 0;
			for (int i = 0; i < m1._col; i++)
			{
				ret[row][col] += m1[row][i] * m2[i][col];
			}
		}
	}
	return ret;
}
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
Matrix translateMatrix(float tx, float ty)
{
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(Point pivotPt, float theta)
{
	Matrix matRot(3, 3);
	matrixSetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	return matRot;
}
Matrix scaleMatrix(Point p, float sx, float sy)
{
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[0][2] = p.x * (1 - sx);
	ret[1][1] = sy;
	ret[1][2] = p.y * (1 - sy);
	return ret;
}
void transformPoints(Matrix& m, std::vector<Point>& points)
{
	Matrix point(3, 1);
	Matrix temp(3, 1);
	for (auto& p : points)
	{
		point[0][0] = p.x;
		point[1][0] = p.y;
		point[2][0] = 1;
		auto temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void triangle(const std::vector<Point>& points)
{
	glBegin(GL_TRIANGLES);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void rect(float xmin, float ymin, float xmax, float ymax)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(xmin, ymin);
	glVertex2f(xmax, ymin);
	glVertex2f(xmax, ymax);
	glVertex2f(xmin, ymax);
	glEnd();
}
void coordinate(float xCoord, float yCoord)
{
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(xCoord, 0);
	glVertex2f(0, 0);
	glVertex2f(0, yCoord);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> tri = { { 100.f, 80.f },{ 280.f, 80.f },{ 190.f, 300.f } };

	// 原始裁剪窗口
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-30, 370, -200, 400);
	glViewport(0, 0, 400, 600);
	coordinate(350, 350);
	rect(xwmin, ywmin, xwmax, ywmax);
	triangle(tri);

	// 转换到规范化视口 自定义矩阵 中心缩放+平移推导
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-(float)1 / 6, (float)7 / 6, -0.5, 1.5); // 放大150倍
	glViewport(400, 300, 200, 300);
	coordinate(1, 1);
	rect(xvmin , yvmin , xvmax , yvmax);
	auto temp = tri;
	transformPoints(translateMatrix((xvmax + xvmin) / 2 - (xwmax + xwmin) / 2, (yvmax + yvmin) / 2 - (ywmax + ywmin) / 2)
		* scaleMatrix({ (xwmax + xwmin) / 2 , (ywmax + ywmin) / 2 }, (xvmax - xvmin) / (xwmax - xwmin), (yvmax - yvmin) / (ywmax - ywmin)), temp);
	triangle(temp);

	// 转换到规范化视口 自定义矩阵 直接使用书上结论
	glViewport(600, 300, 200, 300);
	coordinate(1, 1);
	rect(xvmin, yvmin, xvmax, yvmax);
	temp = tri;
	Matrix m(3, 3);
	matrixSetIdentity(m);
	m[0][0] = (xvmax - xvmin) / (xwmax - xwmin);
	m[1][1] = (yvmax - yvmin) / (ywmax - ywmin);
	m[0][2] = (xwmax*xvmin - xwmin*xvmax) / (xwmax - xwmin);
	m[1][2] = (ywmax*yvmin - ywmin*yvmax) / (ywmax - ywmin);
	transformPoints(m, temp);
	triangle(temp);

	// 转换到规范化视口 OpenGL矩阵 中心缩放+平移推导
	glViewport(400, 0, 200, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate(1, 1);
	rect(xvmin, yvmin, xvmax, yvmax);
	glTranslatef((xvmax + xvmin) / 2 - (xwmax + xwmin) / 2, (yvmax + yvmin) / 2 - (ywmax + ywmin) / 2, 0.f);
	glTranslatef((xwmax + xwmin) / 2, (ywmax + ywmin) / 2, 0.f);
	glScalef((xvmax - xvmin) / (xwmax - xwmin), (yvmax - yvmin) / (ywmax - ywmin), 1.f);
	glTranslatef(-(xwmax + xwmin) / 2, -(ywmax + ywmin) / 2, 0.f);
	triangle(tri);

	// 转换到规范化视口 OpenGL矩阵 直接使用书上结论
	glViewport(600, 0, 200, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate(1, 1);
	rect(xvmin, yvmin, xvmax, yvmax);
	Matrix m1(4, 4);
	matrixSetIdentity(m1);
	m1[0][0] = (xvmax - xvmin) / (xwmax - xwmin);
	m1[1][1] = (yvmax - yvmin) / (ywmax - ywmin);
	m1[0][3] = (xwmax*xvmin - xwmin*xvmax) / (xwmax - xwmin);
	m1[1][3] = (ywmax*yvmin - ywmin*yvmax) / (ywmax - ywmin);
	glMultMatrixf(m1);
	triangle(tri);

	glFlush();
}
void code_8_exercise_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_3
struct Point { float x; float y; };
float xwmin = 40, ywmin = 50, xwmax = 340, ywmax = 250;
struct Matrix
{
	Matrix(int row, int col)
	{
		_data.assign(row, std::vector<float>(col, 0));
		_row = row;
		_col = col;
	}
	std::vector<float>& operator [](int row)
	{
		return _data[row];
	}
	operator GLfloat *()
	{
		_elementData.clear();
		for (int j = 0; j < _col; j++)
		{
			for (int i = 0; i < _row; i++)
			{
				_elementData.push_back(_data[i][j]);
			}
		}
		return &_elementData[0];
	}
	std::vector<std::vector<float>> _data;
	std::vector<float> _elementData;
	int _row;
	int _col;
};
Matrix operator *(Matrix& m1, Matrix& m2)
{
	assert(m1._col == m2._row);

	Matrix ret(m1._row, m2._col);
	for (int row = 0; row < m1._row; row++)
	{
		for (int col = 0; col < m2._col; col++)
		{
			ret[row][col] = 0;
			for (int i = 0; i < m1._col; i++)
			{
				ret[row][col] += m1[row][i] * m2[i][col];
			}
		}
	}
	return ret;
}
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
Matrix translateMatrix(float tx, float ty)
{
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(Point pivotPt, float theta)
{
	Matrix matRot(3, 3);
	matrixSetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	return matRot;
}
Matrix scaleMatrix(Point p, float sx, float sy)
{
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[0][2] = p.x * (1 - sx);
	ret[1][1] = sy;
	ret[1][2] = p.y * (1 - sy);
	return ret;
}
void transformPoints(Matrix& m, std::vector<Point>& points)
{
	Matrix point(3, 1);
	Matrix temp(3, 1);
	for (auto& p : points)
	{
		point[0][0] = p.x;
		point[1][0] = p.y;
		point[2][0] = 1;
		auto temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void triangle(const std::vector<Point>& points)
{
	glBegin(GL_TRIANGLES);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void rect(float xmin, float ymin, float xmax, float ymax)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(xmin, ymin);
	glVertex2f(xmax, ymin);
	glVertex2f(xmax, ymax);
	glVertex2f(xmin, ymax);
	glEnd();
}
void coordinate(float xCoord, float yCoord)
{
	glBegin(GL_LINES);
	glVertex2f(-xCoord, 0);
	glVertex2f(xCoord, 0);
	glVertex2f(0, -yCoord);
	glVertex2f(0, yCoord);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> tri = { { 100.f, 80.f },{ 280.f, 80.f },{ 190.f, 300.f } };

	// 原始裁剪窗口
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-30, 370, -200, 400);
	glViewport(0, 0, 400, 600);
	coordinate(350, 350);
	rect(xwmin, ywmin, xwmax, ywmax);
	triangle(tri);

	// 转换到规范化正方形 自定义矩阵 左下角缩放+平移推导
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-(float) 4 / 3, (float)4 / 3, -2, 2);	// 放大75倍
	glViewport(400, 300, 200, 300);
	coordinate(1.2, 1.8);
	rect(-1, -1, 1, 1);
	auto temp = tri;
	transformPoints(translateMatrix(-1 - xwmin, -1 - ywmin)
		* scaleMatrix({  xwmin, ywmin }, 2 / (xwmax - xwmin), 2 / (ywmax - ywmin)), temp);
	triangle(temp);

	// 转换到规范化正方形 自定义矩阵 直接使用书上结论
	glViewport(600, 300, 200, 300);
	coordinate(1.2, 1.8);
	rect(-1, -1, 1, 1);
	temp = tri;
	Matrix m(3, 3);
	matrixSetIdentity(m);
	m[0][0] = 2 / (xwmax - xwmin);
	m[1][1] = 2 / (ywmax - ywmin);
	m[0][2] = -(xwmax + xwmin) / (xwmax - xwmin);
	m[1][2] = -(ywmax + ywmin) / (ywmax - ywmin);
	transformPoints(m, temp);
	triangle(temp);

	// 转换到规范化正方形 OpenGL矩阵 左下角缩放+平移推导
	glViewport(400, 0, 200, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate(1.2, 1.8);
	rect(-1, -1, 1, 1);
	glTranslatef(-1 - xwmin, -1 - ywmin, 0.f);
	glTranslatef(xwmin, ywmin, 0.f);
	glScalef(2 / (xwmax - xwmin), 2 / (ywmax - ywmin), 1.f);
	glTranslatef(-xwmin, -ywmin, 0.f);
	triangle(tri);

	// 转换到规范化正方形 OpenGL矩阵 直接使用书上结论
	glViewport(600, 0, 200, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate(1.2, 1.8);
	rect(-1, -1, 1, 1);
	Matrix m1(4, 4);
	matrixSetIdentity(m1);
	m1[0][0] = 2 / (xwmax - xwmin);
	m1[1][1] = 2 / (ywmax - ywmin);
	m1[0][3] = -(xwmax + xwmin) / (xwmax - xwmin);
	m1[1][3] = -(ywmax + ywmin) / (ywmax - ywmin);
	glMultMatrixf(m1);
	triangle(tri);

	glFlush();
}
void code_8_exercise_3()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_4
struct Point { float x; float y; };
float xwmin = 40, ywmin = 50, xwmax = 340, ywmax = 250;
float showRate = 75.f;
struct Matrix
{
	Matrix(int row, int col)
	{
		_data.assign(row, std::vector<float>(col, 0));
		_row = row;
		_col = col;
	}
	std::vector<float>& operator [](int row)
	{
		return _data[row];
	}
	operator GLfloat *()
	{
		_elementData.clear();
		for (int j = 0; j < _col; j++)
		{
			for (int i = 0; i < _row; i++)
			{
				_elementData.push_back(_data[i][j]);
			}
		}
		return &_elementData[0];
	}
	std::vector<std::vector<float>> _data;
	std::vector<float> _elementData;
	int _row;
	int _col;
};
Matrix operator *(Matrix& m1, Matrix& m2)
{
	assert(m1._col == m2._row);

	Matrix ret(m1._row, m2._col);
	for (int row = 0; row < m1._row; row++)
	{
		for (int col = 0; col < m2._col; col++)
		{
			ret[row][col] = 0;
			for (int i = 0; i < m1._col; i++)
			{
				ret[row][col] += m1[row][i] * m2[i][col];
			}
		}
	}
	return ret;
}
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
Matrix translateMatrix(float tx, float ty)
{
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(Point pivotPt, float theta)
{
	Matrix matRot(3, 3);
	matrixSetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	return matRot;
}
Matrix scaleMatrix(Point p, float sx, float sy)
{
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[0][2] = p.x * (1 - sx);
	ret[1][1] = sy;
	ret[1][2] = p.y * (1 - sy);
	return ret;
}
void transformPoints(Matrix& m, std::vector<Point>& points)
{
	Matrix point(3, 1);
	Matrix temp(3, 1);
	for (auto& p : points)
	{
		point[0][0] = p.x;
		point[1][0] = p.y;
		point[2][0] = 1;
		auto temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void triangle(const std::vector<Point>& points)
{
	glBegin(GL_TRIANGLES);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void rect(float xmin, float ymin, float xmax, float ymax)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(xmin, ymin);
	glVertex2f(xmax, ymin);
	glVertex2f(xmax, ymax);
	glVertex2f(xmin, ymax);
	glEnd();
}
void coordinate(float _xCoord, float xCoord, float _yCoord, float yCoord)
{
	glBegin(GL_LINES);
	glVertex2f(_xCoord, 0);
	glVertex2f(xCoord, 0);
	glVertex2f(0, _yCoord);
	glVertex2f(0, yCoord);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> tri = { { -50.f, 0.f },{ 50.f, 0.f },{ 0.f, 150.f } };

	// 局部坐标系
	auto temp = tri;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100, 100, -300, 300);
	glViewport(0, 0, 200, 600);
	coordinate(-90, 90, -50, 200);
	triangle(temp);

	// 转换到世界坐标系
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-25, 275, -25, 275);
	glViewport(200, 300, 300, 300);
	coordinate(-10, 250, -10, 250);
	transformPoints(translateMatrix(150, 75), temp);
	triangle(temp);

	//// 转换到规范化正方形 自定义矩阵 左下角缩放+平移推导
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(-100, 100, -150, 150);
	//glViewport(400, 300, 200, 300);
	//coordinate(1.2 * showRate, 1.8 * showRate);
	//rect(-1 * showRate, -1 * showRate, 1 * showRate, 1 * showRate);
	//auto temp = tri;
	//transformPoints(translateMatrix(-1 - xwmin, -1 - ywmin)
	//	* scaleMatrix({ xwmin, ywmin }, 2 / (xwmax - xwmin), 2 / (ywmax - ywmin)), temp);
	//transformPoints(scaleMatrix({ 0, 0 }, showRate, showRate), temp);
	//triangle(temp);

	//// 转换到规范化正方形 自定义矩阵 直接使用书上结论
	//glViewport(600, 300, 200, 300);
	//coordinate(1.2 * showRate, 1.8 * showRate);
	//rect(-1 * showRate, -1 * showRate, 1 * showRate, 1 * showRate);
	//temp = tri;
	//Matrix m(3, 3);
	//matrixSetIdentity(m);
	//m[0][0] = 2 / (xwmax - xwmin);
	//m[1][1] = 2 / (ywmax - ywmin);
	//m[0][2] = -(xwmax + xwmin) / (xwmax - xwmin);
	//m[1][2] = -(ywmax + ywmin) / (ywmax - ywmin);
	//transformPoints(m, temp);
	//transformPoints(scaleMatrix({ 0, 0 }, showRate, showRate), temp);
	//triangle(temp);

	//// 转换到规范化正方形 OpenGL矩阵 左下角缩放+平移推导
	//glViewport(400, 0, 200, 300);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//coordinate(1.2 * showRate, 1.8 * showRate);
	//rect(-1 * showRate, -1 * showRate, 1 * showRate, 1 * showRate);
	//glScalef(showRate, showRate, 1.f);
	//glTranslatef(-1 - xwmin, -1 - ywmin, 0.f);
	//glTranslatef(xwmin, ywmin, 0.f);
	//glScalef(2 / (xwmax - xwmin), 2 / (ywmax - ywmin), 1.f);
	//glTranslatef(-xwmin, -ywmin, 0.f);
	//triangle(tri);

	//// 转换到规范化正方形 OpenGL矩阵 直接使用书上结论
	//glViewport(600, 0, 200, 300);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//coordinate(1.2 * showRate, 1.8 * showRate);
	//rect(-1 * showRate, -1 * showRate, 1 * showRate, 1 * showRate);
	//Matrix m1(4, 4);
	//matrixSetIdentity(m1);
	//m1[0][0] = 2 / (xwmax - xwmin);
	//m1[1][1] = 2 / (ywmax - ywmin);
	//m1[0][3] = -(xwmax + xwmin) / (xwmax - xwmin);
	//m1[1][3] = -(ywmax + ywmin) / (ywmax - ywmin);
	//glScalef(showRate, showRate, 1.f);
	//glMultMatrixf(m1);
	//triangle(tri);

	glFlush();
}
void code_8_exercise_4()
{
	glutDisplayFunc(drawFunc);
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_8_COMMON

#ifdef CHAPTER_8_COMMON
void init(void)
{
	// 黑色背景色
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

#ifdef CHAPTER_8_7_1
	code_8_7_1();
#endif

#ifdef CHAPTER_8_7_2
	code_8_7_2();
#endif

#ifdef CHAPTER_8_8_1
	code_8_8_1();
#endif

#ifdef CHAPTER_8_EXERCISE_1
	code_8_exercise_1();
#endif

#ifdef CHAPTER_8_EXERCISE_2
	code_8_exercise_2();
#endif

#ifdef CHAPTER_8_EXERCISE_3
	code_8_exercise_3();
#endif

#ifdef CHAPTER_8_EXERCISE_4
	code_8_exercise_4();
#endif

	glutMainLoop();
}
#endif
#endif 


