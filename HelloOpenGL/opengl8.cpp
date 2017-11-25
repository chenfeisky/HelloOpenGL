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
// 判断浮点数相等
inline bool Equal(float f1, float f2) { return std::abs(f1 - f2) < 0.0001; }
inline bool Greater(float f1, float f2) { return Equal(f1, f2) ? false : (f1 > f2); }
inline bool Less(float f1, float f2) { return Equal(f1, f2) ? false : (f1 < f2); }
inline bool GreaterQ(float f1, float f2) { return Greater(f1, f2) || Equal(f1, f2); }
inline bool LessQ(float f1, float f2) { return Less(f1, f2) || Equal(f1, f2); }
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

	p1 = { 100, 240 }, p2 = { 400, 240 };
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

	p1.setCoords(100, 240);
	p2.setCoords(400, 240);
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
typedef Point Vec;
Point pv0 = {17, 103}; // 观察坐标系原点
Point pv1 = {84, 237}; // 观察坐标系向上向量方向点（相对于pv0点）
float xwmin = 30, ywmin = 20, xwmax = 200, ywmax = 130;  // 裁剪窗口(观察坐标系中定义)
float xvmin = 150, yvmin = 50, xvmax = 235, yvmax = 105;  // 视口(设备坐标系中定义)
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
void rect(const std::vector<Point>& points)
{
	glBegin(GL_LINE_LOOP);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void coordinate(Point o, Vec V, float _xCoord, float xCoord, float _yCoord, float yCoord)
{
	Vec v, u;
	float distanceV = std::sqrt(V.x * V.x + V.y * V.y);
	v.x = V.x / distanceV;
	v.y = V.y / distanceV;
	u.x = v.y;
	u.y = -v.x;
	
	glBegin(GL_LINES);
	glVertex2f(o.x + v.x * _yCoord, o.y + v.y * _yCoord);
	glVertex2f(o.x + v.x * yCoord, o.y + v.y * yCoord);
	glVertex2f(o.x + u.x * _xCoord, o.y + u.y * _xCoord);
	glVertex2f(o.x + u.x * xCoord, o.y + u.y * xCoord);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> tri = { { -50.f, 0.f },{ 50.f, 0.f },{ 0.f, 130.f }};

	auto temp = tri;

	// 0.局部坐标系
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100, 100, -300, 300);
	glViewport(0, 0, 200, 600);
	coordinate({ 0, 0 }, { 0, 1 }, -90, 90, -50, 200);
	triangle(temp);

	// 1.转换到世界坐标系
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-25, 275, -25, 275);
	glViewport(200, 300, 300, 300);
	coordinate({ 0, 0 }, { 0, 1 }, -10, 250, -10, 250);
	transformPoints(translateMatrix(150, 100), temp);
	Vec V = { pv1.x - pv0.x, pv1.y - pv0.y };
	coordinate(pv0, V, -10, 200, -10, 180);
	triangle(temp);
	std::vector<Point> tempWin = { { xwmin, ywmin },{ xwmax, ywmin },{ xwmax, ywmax },{ xwmin, ywmax } };
	Vec v, u;
	float distanceV = std::sqrt(V.x * V.x + V.y * V.y);
	v.x = V.x / distanceV;
	v.y = V.y / distanceV;
	u.x = v.y;
	u.y = -v.x;
	Matrix r(3, 3); // 绕pv0旋转到v, u坐标系
	matrixSetIdentity(r);
	r[0][0] = u.x;
	r[0][1] = v.x;
	r[1][0] = u.y;
	r[1][1] = v.y;
	r[0][2] = pv0.x * (1 - r[0][0]) + pv0.y * r[1][0];
	r[1][2] = pv0.y * (1 - r[0][0]) - pv0.x * r[1][0];
	transformPoints(r * translateMatrix(pv0.x, pv0.y), tempWin);
	rect(tempWin);

	// 2.转换到观察坐标系 使用观察坐标系（8.2.1）
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-50, 250, -80, 220);
	glViewport(500, 300, 300, 300);
	coordinate({ 0, 0 }, { 0, 1 }, -20, 230, -40, 200);
	rect(xwmin, ywmin, xwmax, ywmax);
	Matrix r1(3, 3);
	matrixSetIdentity(r1);
	r1[0][0] = u.x;
	r1[0][1] = u.y;
	r1[1][0] = v.x;
	r1[1][1] = v.y;
	transformPoints(r1 * translateMatrix(-pv0.x, -pv0.y), temp);
	triangle(temp);

	// 3.转换到规范化设备坐标系(规范化正方形)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2, 2, -2, 2); // 放大75倍
	glViewport(200, 0, 300, 300);
	coordinate({ 0, 0 }, { 0, 1 }, -1.2, 1.2, -1.2, 1.2);	
	rect(-1, -1, 1, 1);
	Matrix r2(3, 3);
	matrixSetIdentity(r2);
	r2[0][0] = 2 / (xwmax - xwmin);
	r2[1][1] = 2 / (ywmax - ywmin);
	r2[0][2] = -(xwmax + xwmin) / (xwmax - xwmin);
	r2[1][2] = -(ywmax + ywmin) / (ywmax - ywmin);
	transformPoints(r2, temp);
	triangle(temp);

	// 4.转换到设备坐标系
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-25, 275, -100, 200);
	glViewport(500, 0, 300, 300);
	coordinate({ 0, 0 }, { 0, 1 }, -10, 250, -50, 180);
	rect(xvmin, yvmin, xvmax, yvmax);
	Matrix r3(3, 3);
	matrixSetIdentity(r3);
	r3[0][0] = (xvmax - xvmin) / 2;
	r3[1][1] = (yvmax - yvmin) / 2;
	r3[0][2] = (xvmin + xvmax) / 2;
	r3[1][2] = (yvmin + yvmax) / 2;
	transformPoints(r3, temp);
	triangle(temp);

	glFlush();
}
void code_8_exercise_4()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_4_1
struct Point { float x; float y; };
typedef Point Vec;
Point p0 = { 150, 143 }; // 对象参考点（中心点）
Point p1 = { 200, 240 }; // 向量方向点（相对于p0点）(和上一题中相同)
float xwmin = 30 - 101.07, ywmin = 20 - 95.25, xwmax = 200 - 101.07, ywmax = 130 - 95.25;  // 裁剪窗口(世界坐标系中定义， 和上一习题中位置相同， (101.07, 95.25)是上一习题中计算出的三角形中心在观察坐标系中的坐标)
float xvmin = 150.f, yvmin = 50.f, xvmax = 235.f, yvmax = 105.f; // 视口(设备坐标系中定义， 和上一习题中位置相同)
float xnvmin = xvmin / 250, ynvmin = yvmin / 180, xnvmax = xvmax / 250, ynvmax = yvmax / 180;  // 规范化视口(规范化设备坐标系中定义， 250，180是上一习题中设备坐标系的边界（显示窗口边界）)
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
void rect(const std::vector<Point>& points)
{
	glBegin(GL_LINE_LOOP);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void coordinate(Point o, Vec V, float _xCoord, float xCoord, float _yCoord, float yCoord)
{
	Vec v, u;
	float distanceV = std::sqrt(V.x * V.x + V.y * V.y);
	v.x = V.x / distanceV;
	v.y = V.y / distanceV;
	u.x = v.y;
	u.y = -v.x;

	glBegin(GL_LINES);
	glVertex2f(o.x + v.x * _yCoord, o.y + v.y * _yCoord);
	glVertex2f(o.x + v.x * yCoord, o.y + v.y * yCoord);
	glVertex2f(o.x + u.x * _xCoord, o.y + u.y * _xCoord);
	glVertex2f(o.x + u.x * xCoord, o.y + u.y * xCoord);
	glEnd();
}
void point(Point p)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
}
void Vector(Point o, Vec V)
{
	glColor3f(1.0, 0.0, 0.0);

	float arrowH = 10, arrowW = 5;

	Point end = { o.x + V.x, o.y + V.y };
	glBegin(GL_LINES);
	glVertex2f(o.x, o.y);
	glVertex2f(end.x, end.y);
	glEnd();

	Vec v, u;
	float distanceV = std::sqrt(V.x * V.x + V.y * V.y);
	v.x = V.x / distanceV;
	v.y = V.y / distanceV;
	u.x = v.y;
	u.y = -v.x;
	Point bottomCenter = { end.x - v.x * arrowH, end.y - v.y * arrowH };
	Point bottomLeft = { bottomCenter.x - u.x * arrowW / 2, bottomCenter.y - u.y * arrowW / 2 };
	Point bottomRight = { bottomCenter.x + u.x * arrowW / 2, bottomCenter.y + u.y * arrowW / 2 };
	triangle({ bottomLeft, bottomRight, end });

	glColor3f(1.0, 1.0, 1.0);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> tri = { { -50.f, 0.f },{ 50.f, 0.f },{ 0.f, 130.f } };

	// 0.局部坐标系
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100, 100, -300, 300);
	glViewport(0, 0, 200, 600);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 }, -90, 90, -50, 200);
	triangle(tri);

	// 1.转换到世界坐标系
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-25, 275, -25, 275);
	glViewport(200, 300, 300, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 }, -10, 250, -10, 250);
	glTranslatef(150.f, 100.f, 0.f);
	triangle(tri);
	glLoadIdentity();
	Vec V = { p1.x - p0.x, p1.y - p0.y };
	point(p0);
	Vector(p0, V);

	// 2.直接变换世界对象（8.2.2）
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-150, 150, -150, 150);
	glViewport(500, 300, 300, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 }, -130, 130, -130, 130);
	rect(xwmin, ywmin, xwmax, ywmax);
	Vec v, u;
	float distanceV = std::sqrt(V.x * V.x + V.y * V.y);
	v.x = V.x / distanceV;
	v.y = V.y / distanceV;
	u.x = v.y;
	u.y = -v.x;
	Matrix r1(4, 4);
	matrixSetIdentity(r1);
	r1[0][0] = u.x;
	r1[0][1] = u.y;
	r1[1][0] = v.x;
	r1[1][1] = v.y;
	glMultMatrixf(r1);
	glTranslatef(-p0.x, -p0.y, 0.f);
	glTranslatef(150.f, 100.f, 0.f);
	triangle(tri);

	// 3.转换到规范化设备坐标系(规范化视口)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.25, 1.25, -0.25, 1.25); // 放大200倍
	glViewport(200, 0, 300, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 }, -0.15, 1.15, -0.15, 1.15);
	rect(xnvmin, ynvmin, xnvmax, ynvmax);
	Matrix r2(4, 4);
	matrixSetIdentity(r2);
	r2[0][0] = (xnvmax - xnvmin) / (xwmax - xwmin);
	r2[1][1] = (ynvmax - ynvmin) / (ywmax - ywmin);
	r2[0][3] = (xwmax*xnvmin - xwmin*xnvmax) / (xwmax - xwmin);
	r2[1][3] = (ywmax*ynvmin - ywmin*ynvmax) / (ywmax - ywmin);
	glMultMatrixf(r2);
	glMultMatrixf(r1);
	glTranslatef(-p0.x, -p0.y, 0.f);
	glTranslatef(150.f, 100.f, 0.f);
	triangle(tri);

	// 4.转换到设备坐标系
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-25, 275, -100, 200);
	glViewport(500, 0, 300, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	coordinate({ 0, 0 }, { 0, 1 }, -10, 250, -50, 180);
	rect(xvmin, yvmin, xvmax, yvmax);
	glScalef(250.f , 180.f, 1.f);
	glMultMatrixf(r2);
	glMultMatrixf(r1);
	glTranslatef(-p0.x, -p0.y, 0.f);
	glTranslatef(150.f, 100.f, 0.f);
	triangle(tri);

	glFlush();
}
void code_8_exercise_4_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_6
float FPS = 60;
float speedA = 6 * PI / 180;
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
int lastTick = 0;
float delta = 0.f;
float angle = PI / 2;
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D winMin = { 200, 220 }, winMax = { 520, 380 };
	wcPt2D winCenter = { (winMin.x + winMax.x) / 2, (winMin.y + winMax.y) / 2 };

	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	wcPt2D p1, p2;
	p1 = { winCenter.x, winCenter.y + 280}, p2 = { winCenter.x, winCenter.y - 200 };
	angle -= delta * speedA;
	printf("angle = %f\n", angle);
	p1.x = 280 * cos(angle) + winCenter.x;
	p1.y = 280 * sin(angle) + winCenter.y;
	p2.x = -200 * cos(angle) + winCenter.x;
	p2.y = -200 * sin(angle) + winCenter.y;

	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);

	glFlush();
}
void onTimer(int id)
{
	int curTick = GetTickCount();
	delta = (curTick - lastTick) / (float)1000;
	lastTick = curTick;
	glutPostRedisplay();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
void code_8_exercise_6()
{
	glutDisplayFunc(drawFunc);
	lastTick = GetTickCount();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
#endif

#ifdef CHAPTER_8_EXERCISE_8
std::map<int, int> opCount;
#define ADD 0
#define MINUS 1
#define MULTIPLY 2
#define DIVISION 3
class wcPt2D
{
public:
	GLfloat x, y;
};
typedef enum { Left, Right, Bottom, Top } Boundary;
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
	opCount.clear();
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
			{
				opCount[MINUS] += 2;
				opCount[DIVISION] += 1;
				m = (p2.y - p1.y) / (p2.x - p1.x);
			}
				
			if (code1 & winLeftBitCode)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y += (winMin.x - p1.x) * m;
				p1.x = winMin.x;
			}
			else if (code1 & winRightBitCode)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y += (winMax.x - p1.x) * m;
				p1.x = winMax.x;
			}
			else if (code1 & winBottomBitCode)
			{
				if (p2.x != p1.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x += (winMin.y - p1.y) / m;
				}					
				p1.y = winMin.y;
			}
			else if (code1 & winTopBitCode)
			{
				if (p2.x != p1.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x += (winMax.y - p1.y) / m;
				}
				p1.y = winMax.y;
			}
		}
	}
	if (plotLine)
	{
		//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
	}
	printf("Cohen-Sutherland: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY],opCount[DIVISION]);
}
void lineClipCohSuthOptimize(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2)
{
	opCount.clear();
	GLubyte code1, code2;
	GLint done = false, plotLine = false;
	GLfloat m;
	if (p2.x != p1.x)
	{
		opCount[MINUS] += 2;
		opCount[DIVISION] += 1;
		m = (p2.y - p1.y) / (p2.x - p1.x);
	}

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

			if (code1 & winLeftBitCode)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y += (winMin.x - p1.x) * m;
				p1.x = winMin.x;
			}
			else if (code1 & winRightBitCode)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y += (winMax.x - p1.x) * m;
				p1.x = winMax.x;
			}
			else if (code1 & winBottomBitCode)
			{
				if (p2.x != p1.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x += (winMin.y - p1.y) / m;
				}
				p1.y = winMin.y;
			}
			else if (code1 & winTopBitCode)
			{
				if (p2.x != p1.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x += (winMax.y - p1.y) / m;
				}
				p1.y = winMax.y;
			}
		}
	}
	if (plotLine)
	{
		//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
	}
	printf("Cohen-Sutherland Optimize: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY], opCount[DIVISION]);
}
GLint clipTest(GLfloat p, GLfloat q, GLfloat* u1, GLfloat* u2)
{
	GLfloat r;
	GLint returnValue = true;

	if (p < 0.0)
	{
		opCount[DIVISION] += 1;
		r = q / p;
		if (r > *u2)
			returnValue = false;
		else if (r > *u1)
			*u1 = r;
	}
	else if (p > 0.0)
	{
		opCount[DIVISION] += 1;
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
	opCount.clear();
	opCount[MINUS] += 1;
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy;
	opCount[MINUS] += 1;
	if (clipTest(-dx, p1.x - winMin.x, &u1, &u2))
	{
		opCount[MINUS] += 1;
		if (clipTest(dx, winMax.x - p1.x, &u1, &u2))
		{
			opCount[MINUS] += 1;
			dy = p2.y - p1.y;
			opCount[MINUS] += 1;
			if (clipTest(-dy, p1.y - winMin.y, &u1, &u2))
			{
				opCount[MINUS] += 1;
				if (clipTest(dy, winMax.y - p1.y, &u1, &u2))
				{
					if (u2 < 1.0)
					{
						opCount[ADD] += 2;
						opCount[MULTIPLY] += 2;
						p2.x = p1.x + u2 * dx;
						p2.y = p1.y + u2 * dy;
					}
					if (u1 > 0.0)
					{
						opCount[ADD] += 2;
						opCount[MULTIPLY] += 2;
						p1.x = p1.x + u1 * dx;
						p1.y = p1.y + u1 * dy;
					}
					//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
					lineBres(p1.x, p1.y, p2.x, p2.y);
				}
			}
		}
	}
	printf("Liang-Barsky: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY], opCount[DIVISION]);
}
bool clipTestBoundary(GLfloat p, wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2, Boundary b, GLfloat* u1, GLfloat* u2)
{
	switch (b)
	{
	case Boundary::Left:
	{
		if (p1.x < winMin.x && p2.x < winMin.x)
			return false;
		else if (p1.x > winMin.x && p2.x > winMin.x)
			return true;
		else
		{
			opCount[MINUS] += 1;
			return clipTest(p, p1.x - winMin.x, u1, u2);
		}
	}
	break;
	case Boundary::Right:
	{
		if (p1.x > winMax.x && p2.x > winMax.x)
			return false;
		else if (p1.x < winMax.x && p2.x < winMax.x)
			return true;
		else
		{
			opCount[MINUS] += 1;
			return clipTest(p, winMax.x - p1.x, u1, u2);
		}
	}
	break;
	case Boundary::Bottom:
	{
		if (p1.y < winMin.y && p2.y < winMin.y)
			return false;
		else if (p1.y > winMin.y && p2.y > winMin.y)
			return true;
		else
		{
			opCount[MINUS] += 1;
			return clipTest(p, p1.y - winMin.y, u1, u2);
		}
	}
	break;
	case Boundary::Top:
	{
		if (p1.y > winMax.y && p2.y > winMax.y)
			return false;
		else if (p1.y < winMax.y && p2.y < winMax.y)
			return true;
		else
		{
			opCount[MINUS] += 1;
			return clipTest(p, winMax.y - p1.y, u1, u2);
		}
	}
	break;
	default:
		return false;
	}
}
void lineClipLiangBarskOptimize(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2)
{
	opCount.clear();
	opCount[MINUS] += 1;
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy;
	if (clipTestBoundary(-dx, winMin, winMax, p1, p2, Boundary::Left, &u1, &u2))
		if (clipTestBoundary(dx, winMin, winMax, p1, p2, Boundary::Right, &u1, &u2))
		{
			opCount[MINUS] += 1;
			dy = p2.y - p1.y;
			if (clipTestBoundary(-dy, winMin, winMax, p1, p2, Boundary::Bottom, &u1, &u2))
				if (clipTestBoundary(dy, winMin, winMax, p1, p2, Boundary::Top, &u1, &u2))
				{
					if (u2 < 1.0)
					{
						opCount[ADD] += 2;
						opCount[MULTIPLY] += 2;
						p2.x = p1.x + u2 * dx;
						p2.y = p1.y + u2 * dy;
					}
					if (u1 > 0.0)
					{
						opCount[ADD] += 2;
						opCount[MULTIPLY] += 2;
						p1.x = p1.x + u1 * dx;
						p1.y = p1.y + u1 * dy;
					}
					//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
					lineBres(p1.x, p1.y, p2.x, p2.y);
				}
		}
	printf("Liang-Barsky Optimize: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY], opCount[DIVISION]);	
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D winMin = { 200, 220 }, winMax = { 520, 380 };

	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();
		
	wcPt2D p1, p2;

	p1 = { 106, 475 }, p2 = { 578, 120 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 79, 346 }, p2 = { 688, 256 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 401, 434 }, p2 = { 294, 260 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 561, 399 }, p2 = { 627, 191 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 134, 313 }, p2 = { 378, 174 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 55, 249 }, p2 = { 273, 122 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 139, 431 }, p2 = { 139, 134 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 253, 440 }, p2 = { 253, 186 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 424, 249 }, p2 = { 479, 328 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 100, 240 }, p2 = { 400, 240 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);

	glFlush();
}
void code_8_exercise_8()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_10
float FPS = 60;
float speedA = 6 * PI / 180;
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
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy;
	if (clipTest(-dx, p1.x - winMin.x, &u1, &u2))
		if (clipTest(dx, winMax.x - p1.x, &u1, &u2))
		{
			dy = p2.y - p1.y;
			if (clipTest(-dy, p1.y - winMin.y, &u1, &u2))
				if (clipTest(dy, winMax.y - p1.y, &u1, &u2))
				{
					if (u2 < 1.0)
					{
						p2.x = p1.x + u2 * dx;
						p2.y = p1.y + u2 * dy;
					}
					if (u1 > 0.0)
					{
						p1.x = p1.x + u1 * dx;
						p1.y = p1.y + u1 * dy;
					}
					//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
					lineBres(p1.x, p1.y, p2.x, p2.y);
				}
		}
}
int lastTick = 0;
float delta = 0.f;
float angle = PI / 2;
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D winMin = { 200, 220 }, winMax = { 520, 380 };
	wcPt2D winCenter = { (winMin.x + winMax.x) / 2, (winMin.y + winMax.y) / 2 };

	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	wcPt2D p1, p2;
	p1 = { winCenter.x, winCenter.y + 280 }, p2 = { winCenter.x, winCenter.y - 200 };
	angle -= delta * speedA;
	printf("angle = %f\n", angle);
	p1.x = 280 * cos(angle) + winCenter.x;
	p1.y = 280 * sin(angle) + winCenter.y;
	p2.x = -200 * cos(angle) + winCenter.x;
	p2.y = -200 * sin(angle) + winCenter.y;

	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipLiangBarsk(winMin, winMax, p1, p2);

	glFlush();
}
void onTimer(int id)
{
	int curTick = GetTickCount();
	delta = (curTick - lastTick) / (float)1000;
	lastTick = curTick;
	glutPostRedisplay();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
void code_8_exercise_10()
{
	glutDisplayFunc(drawFunc);
	lastTick = GetTickCount();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
#endif

#ifdef CHAPTER_8_EXERCISE_11
class Point
{
public:
	GLfloat x, y;
};
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
void transformPoint(Matrix& m, Point& point)
{
	Matrix p(3, 1);
	p[0][0] = point.x;
	p[1][0] = point.y;
	p[2][0] = 1;
	auto temp = m * p;
	point.x = temp[0][0];
	point.y = temp[1][0];
}
inline GLint Round(const GLfloat a)
{
	return GLint(a + 0.5);
}
void point(Point p)
{
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}
void rect(Point winMin, Point winMax)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();
}
void rorate(Point& p, Point& winMin, Point& winMax, float angle)
{
	Point center = { (winMin.x + winMax.x) / 2, (winMin.y + winMax.y) / 2 };
	transformPoint(rotateMatrix(center, angle * PI / 180), p);
	if (angle == 90 || angle == 270)
	{
		float w = winMax.x - winMin.x;
		float h = winMax.y - winMin.y;
		winMin.x = center.x - h / 2;
		winMax.x = center.x + h / 2;
		winMin.y = center.y - w / 2;
		winMax.y = center.y + w / 2;
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glViewport(0, 0, winWidth, winHeight);
	glBegin(GL_LINES);
	glVertex2f(0, winHeight / 2);
	glVertex2f(winWidth, winHeight / 2);
	glVertex2f(260, 0);
	glVertex2f(260, winHeight);
	glVertex2f(520, 0);
	glVertex2f(520, winHeight);
	glEnd();

	Point winMin = { 400 , 300}, winMax = { 480 , 350};

	// 上
	Point p = { 465, 370 };
	Point _winMin = winMin, _winMax = winMax;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(310, 570, 250, 400);
	glViewport(0, 450, 260, 150);
	point(p);
	rect(_winMin, _winMax);
	glViewport(0, 300, 260, 150);
	rorate(p, _winMin, _winMax, 90);
	point(p);
	rect(_winMin, _winMax);

	// 右上
	p = { 500, 370 };
	_winMin = winMin, _winMax = winMax;
	glViewport(260, 450, 260, 150);
	point(p);
	rect(_winMin, _winMax);
	glViewport(260, 300, 260, 150);
	rorate(p, _winMin, _winMax, 90);
	point(p);
	rect(_winMin, _winMax);

	// 右
	p = { 520, 344 };
	_winMin = winMin, _winMax = winMax;
	glViewport(520, 450, 260, 150);
	point(p);
	rect(_winMin, _winMax);
	glViewport(520, 300, 260, 150);
	rorate(p, _winMin, _winMax, 180);
	point(p);
	rect(_winMin, _winMax);

	// 右下
	p = { 490, 260 };
	_winMin = winMin, _winMax = winMax;
	glViewport(520, 150, 260, 150);
	point(p);
	rect(_winMin, _winMax);
	glViewport(520, 0, 260, 150);
	rorate(p, _winMin, _winMax, 180);
	point(p);
	rect(_winMin, _winMax);

	// 下
	p = { 440, 270 };
	_winMin = winMin, _winMax = winMax;
	glViewport(260, 150, 260, 150);
	point(p);
	rect(_winMin, _winMax);
	glViewport(260, 0, 260, 150);
	rorate(p, _winMin, _winMax, 270);
	point(p);
	rect(_winMin, _winMax);

	// 左下
	p = { 375, 290 };
	_winMin = winMin, _winMax = winMax;
	glViewport(0, 150, 260, 150);
	point(p);
	rect(_winMin, _winMax);
	glViewport(0, 0, 260, 150);
	rorate(p, _winMin, _winMax, 270);
	point(p);
	rect(_winMin, _winMax);

	glFlush();
}
void code_8_exercise_11()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_12
class Point
{
public:
	GLfloat x, y;
};
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
void transformPoint(Matrix& m, Point& point)
{
	Matrix p(3, 1);
	p[0][0] = point.x;
	p[1][0] = point.y;
	p[2][0] = 1;
	auto temp = m * p;
	point.x = temp[0][0];
	point.y = temp[1][0];
}
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
inline GLint reject(GLint code1, GLint code2)
{
	return GLint(code1 & code2);
}
inline GLint accept(GLint code1, GLint code2)
{
	return GLint(!(code1 | code2));
}
GLubyte encode(Point pt, Point winMin, Point winMax)
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
void point(Point p)
{
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}
void rect(Point winMin, Point winMax)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();
}
void rorate(Point& p1, Point& p2, Point& winMin, Point& winMax, float angle)
{
	Point center = { (winMin.x + winMax.x) / 2, (winMin.y + winMax.y) / 2 };
	//auto m = rotateMatrix(center, angle * PI / 180);
	//transformPoint(m, p1);
	//transformPoint(m, p2);
	// 简化计算
	float temp = 0.f;
	if (angle == 90 || angle == -270)
	{
		temp = p1.x;
		p1.x = -(p1.y -center.y) + center.x;
		p1.y = (temp - center.x) + center.y;

		temp = p2.x;
		p2.x = -(p2.y - center.y) + center.x;
		p2.y = (temp - center.x) + center.y;
	}
	else if(angle == 180 || angle == -180)
	{
		p1.x = -(p1.x - center.x) + center.x;
		p1.y = -(p1.y - center.y) + center.y;

		p2.x = -(p2.x - center.x) + center.x;
		p2.y = -(p2.y - center.y) + center.y;
	}
	else if (angle == 270 || angle == -90)
	{
		temp = p1.x;
		p1.x = (p1.y - center.y) + center.x;
		p1.y = -(temp - center.x) + center.y;

		temp = p2.x;
		p2.x = (p2.y - center.y) + center.x;
		p2.y = -(temp - center.x) + center.y;
	}
	if (angle == 90 || angle == 270 || angle == -90 || angle == -270)
	{
		float w = winMax.x - winMin.x;
		float h = winMax.y - winMin.y;
		winMin.x = center.x - h / 2;
		winMax.x = center.x + h / 2;
		winMin.y = center.y - w / 2;
		winMax.y = center.y + w / 2;
	}
}
bool inside(GLint code)
{
	return code == 0;
}
bool leftTop(GLint code)
{
	return code == (winLeftBitCode | winTopBitCode);
}
bool left(GLint code)
{
	return code == winLeftBitCode;
}
bool leftBottom(GLint code)
{
	return code == (winLeftBitCode | winBottomBitCode);
}
bool rightTop(GLint code)
{
	return code == (winRightBitCode | winTopBitCode);
}
bool right(GLint code)
{
	return code == winRightBitCode;
}
bool rightBottom(GLint code)
{
	return code == (winRightBitCode | winBottomBitCode);
}
bool top(GLint code)
{
	return code == winTopBitCode;
}
bool bottom(GLint code)
{
	return code == winBottomBitCode;
}
void swapPts(Point& p1, Point& p2)
{
	Point tmp;
	tmp = p1;
	p1 = p2;
	p2 = tmp;
}
void swapCodes(GLubyte& c1, GLubyte& c2)
{
	GLubyte tmp;
	tmp = c1;
	c1 = c2;
	c2 = tmp;
}
bool lineClipNLNStandardInside(Point winMin, Point winMax, Point& p1, Point& p2)
{
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	float slopeLine = 0.f;
	float slopeLB = 0.f;
	float slopeBR = 0.f;
	float slopeTR = 0.f;
	float slopeTL = 0.f;

	if (dx)
		slopeLine = dy / dx;

	if (winMin.x - p1.x)
		slopeLB = (winMin.y - p1.y) / (winMin.x - p1.x);

	if (winMax.x - p1.x)
		slopeBR = (winMin.y - p1.y) / (winMax.x - p1.x);

	if (dy < 0 && (dx == 0 || (winMin.x - p1.x ? slopeLB < slopeLine : false) || (winMax.x - p1.x ? slopeLine < slopeBR : false)))
	{
		p2.y = winMin.y;
		if (dx)
			p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
	}
	else
	{
		if (winMax.x - p1.x)
			slopeTR = (winMax.y - p1.y) / (winMax.x - p1.x);

		if (dx > 0 && (winMax.x - p1.x ? slopeBR <= slopeLine && slopeLine <= slopeTR : true))
		{
			p2.x = winMax.x;
			p2.y = p1.y + (winMax.x - p1.x) * slopeLine;
		}
		else
		{
			if (winMin.x - p1.x)
				slopeTL = (winMax.y - p1.y) / (winMin.x - p1.x);

			if (dy > 0 && (dx == 0 || (winMax.x - p1.x ? slopeTR < slopeLine : false) || (winMin.x - p1.x ? slopeLine < slopeTL : false)))
			{
				p2.y = winMax.y;
				if (dx)
					p2.x = p1.x + (winMax.y - p1.y) / slopeLine;
			}
			else if (dx < 0 && (winMin.x - p1.x ? slopeTL <= slopeLine && slopeLine <= slopeLB : true))
			{
				p2.x = winMin.x;
				p2.y = p1.y + (winMin.x - p1.x) * slopeLine;
			}
		}
	}
	return true;
}
bool lineClipNLNStandardLeft(Point winMin, Point winMax, Point& p1, Point& p2)
{
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	float slopeLine = 0.f;
	float slopeLB = 0.f;
	float slopeBR = 0.f;
	float slopeTR = 0.f;
	float slopeTL = 0.f;

	slopeLine = dy / dx;

	slopeLB = (winMin.y - p1.y) / (winMin.x - p1.x);

	if (slopeLine < slopeLB)
	{
		return false;
	}
	else
	{
		slopeBR = (winMin.y - p1.y) / (winMax.x - p1.x);
		if (slopeLB <= slopeLine && slopeLine < slopeBR)
		{
			p1.y = p1.y + slopeLine * (winMin.x - p1.x);
			p1.x = winMin.x;

			if (p2.y < winMin.y)
			{
				p2.y = winMin.y;
				p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
			}
			return true;
		}
		else
		{
			slopeTR = (winMax.y - p1.y) / (winMax.x - p1.x);
			if (slopeBR <= slopeLine && slopeLine < slopeTR)
			{
				p1.y = p1.y + slopeLine * (winMin.x - p1.x);
				p1.x = winMin.x;

				if (p2.x > winMax.x)
				{
					p2.x = winMax.x;
					p2.y = p1.y + slopeLine * (p2.x - p1.x);
				}
				return true;
			}
			else
			{
				slopeTL = (winMax.y - p1.y) / (winMin.x - p1.x);
				if (slopeTR <= slopeLine && slopeLine <= slopeTL)
				{
					p1.y = p1.y + slopeLine * (winMin.x - p1.x);
					p1.x = winMin.x;

					if (p2.y > winMax.y)
					{
						p2.y = winMax.y;
						p2.x = p1.x + (winMax.y - p1.y) / slopeLine;
					}
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}
bool lineClipNLNStandardLeftL(Point winMin, Point winMax, Point& p1, Point& p2)
{
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	float slopeLine = 0.f;
	float slopeLB = 0.f;
	float slopeBR = 0.f;
	float slopeTR = 0.f;
	float slopeTL = 0.f;

	slopeLine = dy / dx;

	slopeLB = (winMin.y - p1.y) / (winMin.x - p1.x);

	if (slopeLine < slopeLB)
	{
		return false;
	}
	else
	{
		slopeTL = (winMax.y - p1.y) / (winMin.x - p1.x);
		if (slopeLB <= slopeLine && slopeLine < slopeTL)
		{
			p1.y = p1.y + slopeLine * (winMin.x - p1.x);
			p1.x = winMin.x;

			if (p2.y < winMin.y)
			{
				p2.y = winMin.y;
				p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
			}
			return true;
		}
		else
		{
			slopeBR = (winMin.y - p1.y) / (winMax.x - p1.x);
			if (slopeTL <= slopeLine && slopeLine < slopeBR)
			{
				p1.x = p1.x + (winMax.y - p1.y) / slopeLine;
				p1.y = winMax.y;

				if (p2.y < winMin.y)
				{
					p2.y = winMin.y;
					p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
				}
				return true;
			}
			else
			{
				slopeTR = (winMax.y - p1.y) / (winMax.x - p1.x);
				if (slopeBR <= slopeLine && slopeLine <= slopeTR)
				{
					p1.x = p1.x + (winMax.y - p1.y) / slopeLine;
					p1.y = winMax.y;

					if (p2.x > winMax.x)
					{
						p2.x = winMax.x;
						p2.y = p1.y + slopeLine * (winMax.x - p1.x);
					}
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}
bool lineClipNLNStandardLeftT(Point winMin, Point winMax, Point& p1, Point& p2)
{
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	float slopeLine = 0.f;
	float slopeLB = 0.f;
	float slopeBR = 0.f;
	float slopeTR = 0.f;
	float slopeTL = 0.f;

	slopeLine = dy / dx;

	slopeLB = (winMin.y - p1.y) / (winMin.x - p1.x);

	if (slopeLine < slopeLB)
	{
		return false;
	}
	else
	{
		slopeBR = (winMin.y - p1.y) / (winMax.x - p1.x);
		if (slopeLB <= slopeLine && slopeLine < slopeBR)
		{
			p1.y = p1.y + slopeLine * (winMin.x - p1.x);
			p1.x = winMin.x;

			if (p2.y < winMin.y)
			{
				p2.y = winMin.y;
				p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
			}
			return true;
		}
		else
		{
			slopeTL = (winMax.y - p1.y) / (winMin.x - p1.x);
			if (slopeBR <= slopeLine && slopeLine < slopeTL)
			{
				p1.y = p1.y + slopeLine * (winMin.x - p1.x);
				p1.x = winMin.x;

				if (p2.x > winMax.x)
				{
					p2.x = winMax.x;
					p2.y = p1.y + slopeLine * (winMax.x - p1.x);
				}
				return true;
			}
			else
			{
				slopeTR = (winMax.y - p1.y) / (winMax.x - p1.x);
				if (slopeTL <= slopeLine && slopeLine <= slopeTR)
				{
					p1.x = p1.x + (winMax.y - p1.y) / slopeLine;
					p1.y = winMax.y;

					if (p2.x > winMax.x)
					{
						p2.x = winMax.x;
						p2.y = p1.y + slopeLine * (winMax.x - p1.x);
					}
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}
bool lineClipNLNStandard(Point winMin, Point winMax, Point& p1, Point& p2, GLint code1, GLint code2)
{
	if (inside(code1))
	{
		return lineClipNLNStandardInside(winMin, winMax, p1, p2);
	}
	else if (left(code1))
	{
		return lineClipNLNStandardLeft(winMin, winMax, p1, p2);
	}
	else if (leftTop(code1))
	{
		if((p1.y - winMin.y) / (p1.x - winMax.x) < (winMax.y - winMin.y) / (winMin.x - winMax.x))
			return lineClipNLNStandardLeftL(winMin, winMax, p1, p2);
		else
			return lineClipNLNStandardLeftT(winMin, winMax, p1, p2);
	}
	return false;
}
void lineClipNLN(Point winMin, Point winMax, Point p1, Point p2)
{
	GLubyte code1 = encode(p1, winMin, winMax);
	GLubyte code2 = encode(p2, winMin, winMax);
	bool draw = false;
	if (accept(code1, code2))
	{
		draw = true;
	}
	else if (reject(code1, code2))
	{
		draw = false;
	}
	else
	{
		float rotateAngle = 0;
		if (inside(code1) || left(code1) || leftTop(code1))
		{

		}
		else if (inside(code2) || left(code2) || leftTop(code2))
		{
			swapPts(p1, p2);
			swapCodes(code1, code2);
		}
		else
		{
			if (top(code1) || rightTop(code1))
			{
				rotateAngle = 90;
			}
			else if (right(code1) || rightBottom(code1))
			{
				rotateAngle = 180;
			}
			else
			{
				rotateAngle = 270;
			}
			rorate(p1, p2, winMin, winMax, rotateAngle);
			code1 = encode(p1, winMin, winMax);
			code2 = encode(p2, winMin, winMax);
		}
		draw = lineClipNLNStandard(winMin, winMax, p1, p2, code1, code2);
		if (rotateAngle > 0)
		{
			rorate(p1, p2, winMin, winMax, -rotateAngle);
		}
	}

	if (draw)
	{
		lineBres(p1.x, p1.y, p2.x, p2.y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	Point winMin = { 200, 220 }, winMax = { 520, 380 };

	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	Point p1, p2;
	
	p1 = { 106, 475 }, p2 = { 578, 120 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 79, 346 }, p2 = { 688, 256 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 401, 434 }, p2 = { 294, 260 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 561, 399 }, p2 = { 627, 191 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 134, 313 }, p2 = { 378, 174 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 55, 249 }, p2 = { 273, 122 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 139, 431 }, p2 = { 139, 134 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 253, 440 }, p2 = { 253, 186 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 424, 249 }, p2 = { 479, 328 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	p1 = { 100, 240 }, p2 = { 400, 240 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(winMin, winMax, p1, p2);

	glFlush();
}
void code_8_exercise_12()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_13
class Point
{
public:
	GLfloat x, y;
};
std::map<int, int> opCount;
#define ADD 0
#define MINUS 1
#define MULTIPLY 2
#define DIVISION 3
#define TRIGONOMETRIC 4 // 三角函数
typedef enum { Left, Right, Bottom, Top } Boundary;
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
// Cohen-Sutherland
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
GLubyte encode(Point pt, Point winMin, Point winMax)
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
void swapPts(Point& p1, Point& p2)
{
	Point tmp;
	tmp = p1;
	p1 = p2;
	p2 = tmp;
}
void swapCodes(GLubyte& c1, GLubyte& c2)
{
	GLubyte tmp;
	tmp = c1;
	c1 = c2;
	c2 = tmp;
}
void lineClipCohSuth(Point winMin, Point winMax, Point p1, Point p2)
{
	opCount.clear();
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
				swapPts(p1, p2);
				swapCodes(code1, code2);
			}
			if (p2.x != p1.x)
			{
				opCount[MINUS] += 2;
				opCount[DIVISION] += 1;
				m = (p2.y - p1.y) / (p2.x - p1.x);
			}

			if (code1 & winLeftBitCode)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y += (winMin.x - p1.x) * m;
				p1.x = winMin.x;
			}
			else if (code1 & winRightBitCode)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y += (winMax.x - p1.x) * m;
				p1.x = winMax.x;
			}
			else if (code1 & winBottomBitCode)
			{
				if (p2.x != p1.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x += (winMin.y - p1.y) / m;
				}
				p1.y = winMin.y;
			}
			else if (code1 & winTopBitCode)
			{
				if (p2.x != p1.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x += (winMax.y - p1.y) / m;
				}
				p1.y = winMax.y;
			}
		}
	}
	if (plotLine)
	{
		//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
	}
	printf("Cohen-Sutherland: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY], opCount[DIVISION]);
}
void lineClipCohSuthOptimize(Point winMin, Point winMax, Point p1, Point p2)
{
	opCount.clear();
	GLubyte code1, code2;
	GLint done = false, plotLine = false;
	GLfloat m;
	if (p2.x != p1.x)
	{
		opCount[MINUS] += 2;
		opCount[DIVISION] += 1;
		m = (p2.y - p1.y) / (p2.x - p1.x);
	}

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
				swapPts(p1, p2);
				swapCodes(code1, code2);
			}

			if (code1 & winLeftBitCode)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y += (winMin.x - p1.x) * m;
				p1.x = winMin.x;
			}
			else if (code1 & winRightBitCode)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y += (winMax.x - p1.x) * m;
				p1.x = winMax.x;
			}
			else if (code1 & winBottomBitCode)
			{
				if (p2.x != p1.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x += (winMin.y - p1.y) / m;
				}
				p1.y = winMin.y;
			}
			else if (code1 & winTopBitCode)
			{
				if (p2.x != p1.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x += (winMax.y - p1.y) / m;
				}
				p1.y = winMax.y;
			}
		}
	}
	if (plotLine)
	{
		//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
	}
	printf("Cohen-Sutherland Optimize: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY], opCount[DIVISION]);
}
// 梁友栋-Barsky
GLint clipTest(GLfloat p, GLfloat q, GLfloat* u1, GLfloat* u2)
{
	GLfloat r;
	GLint returnValue = true;

	if (p < 0.0)
	{
		opCount[DIVISION] += 1;
		r = q / p;
		if (r > *u2)
			returnValue = false;
		else if (r > *u1)
			*u1 = r;
	}
	else if (p > 0.0)
	{
		opCount[DIVISION] += 1;
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
void lineClipLiangBarsk(Point winMin, Point winMax, Point p1, Point p2)
{
	opCount.clear();
	opCount[MINUS] += 1;
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy;
	opCount[MINUS] += 1;
	if (clipTest(-dx, p1.x - winMin.x, &u1, &u2))
	{
		opCount[MINUS] += 1;
		if (clipTest(dx, winMax.x - p1.x, &u1, &u2))
		{
			opCount[MINUS] += 1;
			dy = p2.y - p1.y;
			opCount[MINUS] += 1;
			if (clipTest(-dy, p1.y - winMin.y, &u1, &u2))
			{
				opCount[MINUS] += 1;
				if (clipTest(dy, winMax.y - p1.y, &u1, &u2))
				{
					if (u2 < 1.0)
					{
						opCount[ADD] += 2;
						opCount[MULTIPLY] += 2;
						p2.x = p1.x + u2 * dx;
						p2.y = p1.y + u2 * dy;
					}
					if (u1 > 0.0)
					{
						opCount[ADD] += 2;
						opCount[MULTIPLY] += 2;
						p1.x = p1.x + u1 * dx;
						p1.y = p1.y + u1 * dy;
					}
					//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
					lineBres(p1.x, p1.y, p2.x, p2.y);
				}
			}
		}
	}
	printf("Liang-Barsky: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY], opCount[DIVISION]);
}
bool clipTestBoundary(GLfloat p, Point winMin, Point winMax, Point p1, Point p2, Boundary b, GLfloat* u1, GLfloat* u2)
{
	switch (b)
	{
	case Boundary::Left:
	{
		if (p1.x < winMin.x && p2.x < winMin.x)
			return false;
		else if (p1.x > winMin.x && p2.x > winMin.x)
			return true;
		else
		{
			opCount[MINUS] += 1;
			return clipTest(p, p1.x - winMin.x, u1, u2);
		}
	}
	break;
	case Boundary::Right:
	{
		if (p1.x > winMax.x && p2.x > winMax.x)
			return false;
		else if (p1.x < winMax.x && p2.x < winMax.x)
			return true;
		else
		{
			opCount[MINUS] += 1;
			return clipTest(p, winMax.x - p1.x, u1, u2);
		}
	}
	break;
	case Boundary::Bottom:
	{
		if (p1.y < winMin.y && p2.y < winMin.y)
			return false;
		else if (p1.y > winMin.y && p2.y > winMin.y)
			return true;
		else
		{
			opCount[MINUS] += 1;
			return clipTest(p, p1.y - winMin.y, u1, u2);
		}
	}
	break;
	case Boundary::Top:
	{
		if (p1.y > winMax.y && p2.y > winMax.y)
			return false;
		else if (p1.y < winMax.y && p2.y < winMax.y)
			return true;
		else
		{
			opCount[MINUS] += 1;
			return clipTest(p, winMax.y - p1.y, u1, u2);
		}
	}
	break;
	default:
		return false;
	}
}
void lineClipLiangBarskOptimize(Point winMin, Point winMax, Point p1, Point p2)
{
	opCount.clear();
	opCount[MINUS] += 1;
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy;
	if (clipTestBoundary(-dx, winMin, winMax, p1, p2, Boundary::Left, &u1, &u2))
		if (clipTestBoundary(dx, winMin, winMax, p1, p2, Boundary::Right, &u1, &u2))
		{
			opCount[MINUS] += 1;
			dy = p2.y - p1.y;
			if (clipTestBoundary(-dy, winMin, winMax, p1, p2, Boundary::Bottom, &u1, &u2))
				if (clipTestBoundary(dy, winMin, winMax, p1, p2, Boundary::Top, &u1, &u2))
				{
					if (u2 < 1.0)
					{
						opCount[ADD] += 2;
						opCount[MULTIPLY] += 2;
						p2.x = p1.x + u2 * dx;
						p2.y = p1.y + u2 * dy;
					}
					if (u1 > 0.0)
					{
						opCount[ADD] += 2;
						opCount[MULTIPLY] += 2;
						p1.x = p1.x + u1 * dx;
						p1.y = p1.y + u1 * dy;
					}
					//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
					lineBres(p1.x, p1.y, p2.x, p2.y);
				}
		}
	printf("Liang-Barsky Optimize: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY], opCount[DIVISION]);
}
// NLN
std::map<int, int> opCountRotate;
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
				opCountRotate[MULTIPLY] += 1;
				opCountRotate[ADD] += 1;
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
Matrix rotateMatrix(Point pivotPt, float theta)
{
	opCountRotate[ADD] += 1;
	opCountRotate[MINUS] += 3;
	opCountRotate[MULTIPLY] += 4;
	opCountRotate[TRIGONOMETRIC] += 2;
	
	auto cosValue = cos(theta);
	auto sinValue = sin(theta);
	Matrix matRot(3, 3);
	matrixSetIdentity(matRot);
	matRot[0][0] = cosValue;
	matRot[0][1] = -sinValue;
	matRot[0][2] = pivotPt.x * (1 - cosValue) + pivotPt.y * sinValue;
	matRot[1][0] = sinValue;
	matRot[1][1] = cosValue;
	matRot[1][2] = pivotPt.y * (1 - cosValue) - pivotPt.x * sinValue;
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
void transformPoint(Matrix& m, Point& point)
{
	Matrix p(3, 1);
	p[0][0] = point.x;
	p[1][0] = point.y;
	p[2][0] = 1;
	auto temp = m * p;
	point.x = temp[0][0];
	point.y = temp[1][0];
}
void rorate(Point& p1, Point& p2, Point& winMin, Point& winMax, float angle)
{
	opCountRotate[ADD] += 2;
	opCountRotate[DIVISION] += 2;
	Point center = { (winMin.x + winMax.x) / 2, (winMin.y + winMax.y) / 2 };
	//auto m = rotateMatrix(center, angle * PI / 180);
	//transformPoint(m, p1);
	//transformPoint(m, p2);
	// 简化计算
	float temp = 0.f;
	if (angle == 90 || angle == -270)
	{
		opCountRotate[ADD] += 4;
		opCountRotate[MINUS] += 4;
		temp = p1.x;
		p1.x = -(p1.y - center.y) + center.x;
		p1.y = (temp - center.x) + center.y;

		temp = p2.x;
		p2.x = -(p2.y - center.y) + center.x;
		p2.y = (temp - center.x) + center.y;
	}
	else if (angle == 180 || angle == -180)
	{
		opCountRotate[ADD] += 4;
		opCountRotate[MINUS] += 4;
		p1.x = -(p1.x - center.x) + center.x;
		p1.y = -(p1.y - center.y) + center.y;

		p2.x = -(p2.x - center.x) + center.x;
		p2.y = -(p2.y - center.y) + center.y;
	}
	else if (angle == 270 || angle == -90)
	{
		opCountRotate[ADD] += 4;
		opCountRotate[MINUS] += 4;
		temp = p1.x;
		p1.x = (p1.y - center.y) + center.x;
		p1.y = -(temp - center.x) + center.y;

		temp = p2.x;
		p2.x = (p2.y - center.y) + center.x;
		p2.y = -(temp - center.x) + center.y;
	}
	if (angle == 90 || angle == 270 || angle == -90 || angle == -270)
	{
		opCountRotate[ADD] += 2;
		opCountRotate[MINUS] += 4;
		opCountRotate[DIVISION] += 4;
		float w = winMax.x - winMin.x;
		float h = winMax.y - winMin.y;
		winMin.x = center.x - h / 2;
		winMax.x = center.x + h / 2;
		winMin.y = center.y - w / 2;
		winMax.y = center.y + w / 2;
	}
}
bool leftTop(GLint code)
{
	return code == (winLeftBitCode | winTopBitCode);
}
bool left(GLint code)
{
	return code == winLeftBitCode;
}
bool leftBottom(GLint code)
{
	return code == (winLeftBitCode | winBottomBitCode);
}
bool rightTop(GLint code)
{
	return code == (winRightBitCode | winTopBitCode);
}
bool right(GLint code)
{
	return code == winRightBitCode;
}
bool rightBottom(GLint code)
{
	return code == (winRightBitCode | winBottomBitCode);
}
bool top(GLint code)
{
	return code == winTopBitCode;
}
bool bottom(GLint code)
{
	return code == winBottomBitCode;
}
bool lineClipNLNStandardInside(Point winMin, Point winMax, Point& p1, Point& p2)
{
	opCount[MINUS] += 2;
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	float slopeLine = 0.f;
	float slopeLB = 0.f;
	float slopeBR = 0.f;
	float slopeTR = 0.f;
	float slopeTL = 0.f;

	if (dx)
	{
		opCount[DIVISION] += 1;
		slopeLine = dy / dx;
	}

	opCount[MINUS] += 1;
	if (winMin.x - p1.x)
	{
		opCount[MINUS] += 2;
		opCount[DIVISION] += 1;
		slopeLB = (winMin.y - p1.y) / (winMin.x - p1.x);
	}		

	opCount[MINUS] += 1;
	if (winMax.x - p1.x)
	{
		opCount[MINUS] += 2;
		opCount[DIVISION] += 1;
		slopeBR = (winMin.y - p1.y) / (winMax.x - p1.x);
	}

	if (dy < 0 && (dx == 0 || (winMin.x - p1.x ? slopeLB < slopeLine : false) || (winMax.x - p1.x ? slopeLine < slopeBR : false)))
	{
		p2.y = winMin.y;
		if (dx)
		{
			opCount[ADD] += 1;
			opCount[MINUS] += 1;
			opCount[DIVISION] += 1;
			p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
		}			
	}
	else
	{
		opCount[MINUS] += 1;
		if (winMax.x - p1.x)
		{
			opCount[MINUS] += 2;
			opCount[DIVISION] += 1;
			slopeTR = (winMax.y - p1.y) / (winMax.x - p1.x);
		}			

		if (dx > 0 && (winMax.x - p1.x ? slopeBR <= slopeLine && slopeLine <= slopeTR : true))
		{
			p2.x = winMax.x;
			opCount[ADD] += 1;
			opCount[MINUS] += 1;
			opCount[MULTIPLY] += 1;
			p2.y = p1.y + (winMax.x - p1.x) * slopeLine;
		}
		else
		{
			opCount[MINUS] += 1;
			if (winMin.x - p1.x)
			{
				opCount[MINUS] += 2;
				opCount[DIVISION] += 1;
				slopeTL = (winMax.y - p1.y) / (winMin.x - p1.x);
			}			

			if (dy > 0 && (dx == 0 || (winMax.x - p1.x ? slopeTR < slopeLine : false) || (winMin.x - p1.x ? slopeLine < slopeTL : false)))
			{
				p2.y = winMax.y;
				if (dx)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p2.x = p1.x + (winMax.y - p1.y) / slopeLine;
				}					
			}
			else if (dx < 0 && (winMin.x - p1.x ? slopeTL <= slopeLine && slopeLine <= slopeLB : true))
			{
				p2.x = winMin.x;
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p2.y = p1.y + (winMin.x - p1.x) * slopeLine;
			}
		}
	}
	return true;
}
bool lineClipNLNStandardLeft(Point winMin, Point winMax, Point& p1, Point& p2)
{
	opCount[MINUS] += 2;
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	float slopeLine = 0.f;
	float slopeLB = 0.f;
	float slopeBR = 0.f;
	float slopeTR = 0.f;
	float slopeTL = 0.f;

	opCount[DIVISION] += 1;
	slopeLine = dy / dx;

	opCount[MINUS] += 2;
	opCount[DIVISION] += 1;
	slopeLB = (winMin.y - p1.y) / (winMin.x - p1.x);

	if (slopeLine < slopeLB)
	{
		return false;
	}
	else
	{
		opCount[MINUS] += 2;
		opCount[DIVISION] += 1;
		slopeBR = (winMin.y - p1.y) / (winMax.x - p1.x);
		if (slopeLB <= slopeLine && slopeLine < slopeBR)
		{
			opCount[ADD] += 1;
			opCount[MINUS] += 1;
			opCount[MULTIPLY] += 1;
			p1.y = p1.y + slopeLine * (winMin.x - p1.x);
			p1.x = winMin.x;

			if (p2.y < winMin.y)
			{
				p2.y = winMin.y;
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[DIVISION] += 1;
				p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
			}
			return true;
		}
		else
		{
			opCount[MINUS] += 2;
			opCount[DIVISION] += 1;
			slopeTR = (winMax.y - p1.y) / (winMax.x - p1.x);
			if (slopeBR <= slopeLine && slopeLine < slopeTR)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y = p1.y + slopeLine * (winMin.x - p1.x);
				p1.x = winMin.x;

				if (p2.x > winMax.x)
				{
					p2.x = winMax.x;
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[MULTIPLY] += 1;
					p2.y = p1.y + slopeLine * (p2.x - p1.x);
				}
				return true;
			}
			else
			{
				opCount[MINUS] += 2;
				opCount[DIVISION] += 1;
				slopeTL = (winMax.y - p1.y) / (winMin.x - p1.x);
				if (slopeTR <= slopeLine && slopeLine <= slopeTL)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[MULTIPLY] += 1;
					p1.y = p1.y + slopeLine * (winMin.x - p1.x);
					p1.x = winMin.x;

					if (p2.y > winMax.y)
					{
						p2.y = winMax.y;
						opCount[ADD] += 1;
						opCount[MINUS] += 1;
						opCount[DIVISION] += 1;
						p2.x = p1.x + (winMax.y - p1.y) / slopeLine;
					}
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}
bool lineClipNLNStandardLeftL(Point winMin, Point winMax, Point& p1, Point& p2)
{
	opCount[MINUS] += 2;
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	float slopeLine = 0.f;
	float slopeLB = 0.f;
	float slopeBR = 0.f;
	float slopeTR = 0.f;
	float slopeTL = 0.f;

	opCount[DIVISION] += 1;
	slopeLine = dy / dx;

	opCount[MINUS] += 2;
	opCount[DIVISION] += 1;
	slopeLB = (winMin.y - p1.y) / (winMin.x - p1.x);

	if (slopeLine < slopeLB)
	{
		return false;
	}
	else
	{
		opCount[MINUS] += 2;
		opCount[DIVISION] += 1;
		slopeTL = (winMax.y - p1.y) / (winMin.x - p1.x);
		if (slopeLB <= slopeLine && slopeLine < slopeTL)
		{
			opCount[ADD] += 1;
			opCount[MINUS] += 1;
			opCount[MULTIPLY] += 1;
			p1.y = p1.y + slopeLine * (winMin.x - p1.x);
			p1.x = winMin.x;

			if (p2.y < winMin.y)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[DIVISION] += 1;
				p2.y = winMin.y;
				p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
			}
			return true;
		}
		else
		{
			opCount[MINUS] += 2;
			opCount[DIVISION] += 1;
			slopeBR = (winMin.y - p1.y) / (winMax.x - p1.x);
			if (slopeTL <= slopeLine && slopeLine < slopeBR)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[DIVISION] += 1;
				p1.x = p1.x + (winMax.y - p1.y) / slopeLine;
				p1.y = winMax.y;

				if (p2.y < winMin.y)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p2.y = winMin.y;
					p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
				}
				return true;
			}
			else
			{
				opCount[MINUS] += 2;
				opCount[DIVISION] += 1;
				slopeTR = (winMax.y - p1.y) / (winMax.x - p1.x);
				if (slopeBR <= slopeLine && slopeLine <= slopeTR)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x = p1.x + (winMax.y - p1.y) / slopeLine;
					p1.y = winMax.y;

					if (p2.x > winMax.x)
					{
						opCount[ADD] += 1;
						opCount[MINUS] += 1;
						opCount[MULTIPLY] += 1;
						p2.x = winMax.x;
						p2.y = p1.y + slopeLine * (winMax.x - p1.x);
					}
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}
bool lineClipNLNStandardLeftT(Point winMin, Point winMax, Point& p1, Point& p2)
{
	opCount[MINUS] += 2;
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	float slopeLine = 0.f;
	float slopeLB = 0.f;
	float slopeBR = 0.f;
	float slopeTR = 0.f;
	float slopeTL = 0.f;

	opCount[DIVISION] += 1;
	slopeLine = dy / dx;

	opCount[MINUS] += 2;
	opCount[DIVISION] += 1;
	slopeLB = (winMin.y - p1.y) / (winMin.x - p1.x);

	if (slopeLine < slopeLB)
	{
		return false;
	}
	else
	{
		opCount[MINUS] += 2;
		opCount[DIVISION] += 1;
		slopeBR = (winMin.y - p1.y) / (winMax.x - p1.x);
		if (slopeLB <= slopeLine && slopeLine < slopeBR)
		{
			opCount[ADD] += 1;
			opCount[MINUS] += 1;
			opCount[MULTIPLY] += 1;
			p1.y = p1.y + slopeLine * (winMin.x - p1.x);
			p1.x = winMin.x;

			if (p2.y < winMin.y)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[DIVISION] += 1;
				p2.y = winMin.y;
				p2.x = p1.x + (winMin.y - p1.y) / slopeLine;
			}
			return true;
		}
		else
		{
			opCount[MINUS] += 2;
			opCount[DIVISION] += 1;
			slopeTL = (winMax.y - p1.y) / (winMin.x - p1.x);
			if (slopeBR <= slopeLine && slopeLine < slopeTL)
			{
				opCount[ADD] += 1;
				opCount[MINUS] += 1;
				opCount[MULTIPLY] += 1;
				p1.y = p1.y + slopeLine * (winMin.x - p1.x);
				p1.x = winMin.x;

				if (p2.x > winMax.x)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[MULTIPLY] += 1;
					p2.x = winMax.x;
					p2.y = p1.y + slopeLine * (winMax.x - p1.x);
				}
				return true;
			}
			else
			{
				opCount[MINUS] += 2;
				opCount[DIVISION] += 1;
				slopeTR = (winMax.y - p1.y) / (winMax.x - p1.x);
				if (slopeTL <= slopeLine && slopeLine <= slopeTR)
				{
					opCount[ADD] += 1;
					opCount[MINUS] += 1;
					opCount[DIVISION] += 1;
					p1.x = p1.x + (winMax.y - p1.y) / slopeLine;
					p1.y = winMax.y;

					if (p2.x > winMax.x)
					{
						opCount[ADD] += 1;
						opCount[MINUS] += 1;
						opCount[MULTIPLY] += 1;
						p2.x = winMax.x;
						p2.y = p1.y + slopeLine * (winMax.x - p1.x);
					}
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}
bool lineClipNLNStandard(Point winMin, Point winMax, Point& p1, Point& p2, GLint code1, GLint code2)
{
	if (inside(code1))
	{
		return lineClipNLNStandardInside(winMin, winMax, p1, p2);
	}
	else if (left(code1))
	{
		return lineClipNLNStandardLeft(winMin, winMax, p1, p2);
	}
	else if (leftTop(code1))
	{
		opCount[MINUS] += 4;
		opCount[DIVISION] += 2;
		if ((p1.y - winMin.y) / (p1.x - winMax.x) < (winMax.y - winMin.y) / (winMin.x - winMax.x))
			return lineClipNLNStandardLeftL(winMin, winMax, p1, p2);
		else
			return lineClipNLNStandardLeftT(winMin, winMax, p1, p2);
	}
	return false;
}
void lineClipNLN(Point winMin, Point winMax, Point p1, Point p2)
{
	opCount.clear();
	opCountRotate.clear();
	GLubyte code1 = encode(p1, winMin, winMax);
	GLubyte code2 = encode(p2, winMin, winMax);
	bool draw = false;
	if (accept(code1, code2))
	{
		draw = true;
	}
	else if (reject(code1, code2))
	{
		draw = false;
	}
	else
	{
		float rotateAngle = 0;
		if (inside(code1) || left(code1) || leftTop(code1))
		{

		}
		else if (inside(code2) || left(code2) || leftTop(code2))
		{
			swapPts(p1, p2);
			swapCodes(code1, code2);
		}
		else
		{
			if (top(code1) || rightTop(code1))
			{
				rotateAngle = 90;
			}
			else if (right(code1) || rightBottom(code1))
			{
				rotateAngle = 180;
			}
			else
			{
				rotateAngle = 270;
			}
			rorate(p1, p2, winMin, winMax, rotateAngle);
			code1 = encode(p1, winMin, winMax);
			code2 = encode(p2, winMin, winMax);
		}
		draw = lineClipNLNStandard(winMin, winMax, p1, p2, code1, code2);
		if (rotateAngle > 0)
		{
			rorate(p1, p2, winMin, winMax, -rotateAngle);
		}
	}

	if (draw)
	{
		lineBres(p1.x, p1.y, p2.x, p2.y);
	}
	printf("NLN: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d TRIGONOMETRIC: %d\n", opCount[ADD] + opCountRotate[ADD], opCount[MINUS] + opCountRotate[MINUS], opCount[MULTIPLY] + opCountRotate[MULTIPLY], opCount[DIVISION] + opCountRotate[DIVISION], opCountRotate[TRIGONOMETRIC]);
	printf("NLN standard: \nADD: %d MINUS: %d MULTIPLY: %d DIVISION: %d\n", opCount[ADD], opCount[MINUS], opCount[MULTIPLY], opCount[DIVISION]);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	Point winMin = { 200, 220 }, winMax = { 520, 380 };

	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	Point p1, p2;

	p1 = { 106, 475 }, p2 = { 578, 120 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 79, 346 }, p2 = { 688, 256 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 401, 434 }, p2 = { 294, 260 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 561, 399 }, p2 = { 627, 191 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 134, 313 }, p2 = { 378, 174 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 55, 249 }, p2 = { 273, 122 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 139, 431 }, p2 = { 139, 134 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 253, 440 }, p2 = { 253, 186 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 424, 249 }, p2 = { 479, 328 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	printf("=================================================\n");
	p1 = { 100, 240 }, p2 = { 400, 240 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipCohSuthOptimize(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
	lineClipLiangBarskOptimize(winMin, winMax, p1, p2);
	lineClipNLN(winMin, winMax, p1, p2);

	glFlush();
}
void code_8_exercise_13()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_14
class Point
{
public:
	GLfloat x, y;
};
typedef enum { None, Left, Right, Bottom, Top } Boundary;
inline GLint Round(const GLfloat a)
{
	return GLint(a + 0.5);
}
void drawPolygon(const vector<Point>& polygon)
{
	glBegin(GL_POLYGON);
	for(auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
GLint inside(Point p, Boundary b, Point wMin, Point wMax)
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
void clipBoundary1(Point winMin, Point winMax, std::vector<Point>& polygon, Boundary b)
{
	auto temp = polygon;
	polygon.clear();
	for (int i = 0; i < temp.size(); i++)
	{
		int next = i + 1 < temp.size() ? i + 1 : 0;
		GLfloat u1 = 0.0, u2 = 1.0, dx = temp[next].x - temp[i].x, dy = temp[next].y - temp[i].y;
		switch (b)
		{
		case Boundary::Left:
			clipTest(-dx, temp[i].x - winMin.x, &u1, &u2);
			break;
		case Boundary::Right:
			clipTest(dx, winMax.x - temp[i].x, &u1, &u2);
			break;
		case Boundary::Bottom:
			clipTest(-dy, temp[i].y - winMin.y, &u1, &u2);
			break;
		case Boundary::Top:
			clipTest(dy, winMax.y - temp[i].y, &u1, &u2);
			break;
		default:
			break;
		}

		bool in1 = inside(temp[i], b, winMin, winMax);
		bool in2 = inside(temp[next], b, winMin, winMax);
		if ((in1 && in2) || (in1 && (!in2)))
		{
			polygon.push_back({ temp[i].x + u2 * dx, temp[i].y + u2 * dy });
		}
		else if ((!in1) && in2)
		{
			polygon.push_back({ temp[i].x + u1 * dx, temp[i].y + u1 * dy });
			polygon.push_back({ temp[i].x + u2 * dx, temp[i].y + u2 * dy });
		}
	}
}
void polygonClipLiangBarsk1(Point winMin, Point winMax, std::vector<Point>& polygon)
{
	clipBoundary1(winMin, winMax, polygon, Boundary::Left);
	clipBoundary1(winMin, winMax, polygon, Boundary::Right);
	clipBoundary1(winMin, winMax, polygon, Boundary::Bottom);
	clipBoundary1(winMin, winMax, polygon, Boundary::Top);
}
void clipBoundary2(Point winMin, Point winMax, std::vector<Point>& polygon, Boundary b)
{
	auto temp = polygon;
	polygon.clear();
	for (int i = 0; i < temp.size(); i++)
	{
		int next = i + 1 < temp.size() ? i + 1 : 0;
		GLfloat u1 = 0.0, u2 = 1.0, dx = temp[next].x - temp[i].x, dy = temp[next].y - temp[i].y;
		switch (b)
		{
		case Boundary::Left:
			clipTest(-dx, temp[i].x - winMin.x, &u1, &u2);
			break;
		case Boundary::Right:
			clipTest(dx, winMax.x - temp[i].x, &u1, &u2);
			break;
		case Boundary::Bottom:
			clipTest(-dy, temp[i].y - winMin.y, &u1, &u2);
			break;
		case Boundary::Top:
			clipTest(dy, winMax.y - temp[i].y, &u1, &u2);
			break;
		default:
			break;
		}

		bool in1 = inside(temp[i], b, winMin, winMax);
		bool in2 = inside(temp[next], b, winMin, winMax);
		if ((in1 && in2) || ((!in1) && in2))
		{
			polygon.push_back({ temp[i].x + u1 * dx, temp[i].y + u1 * dy });
		}
		else if (in1 && (!in2))
		{
			polygon.push_back({ temp[i].x + u1 * dx, temp[i].y + u1 * dy });
			polygon.push_back({ temp[i].x + u2 * dx, temp[i].y + u2 * dy });
		}
	}
}
void polygonClipLiangBarsk2(Point winMin, Point winMax, std::vector<Point>& polygon)
{
	clipBoundary2(winMin, winMax, polygon, Boundary::Left);
	clipBoundary2(winMin, winMax, polygon, Boundary::Right);
	clipBoundary2(winMin, winMax, polygon, Boundary::Bottom);
	clipBoundary2(winMin, winMax, polygon, Boundary::Top);
}
enum class BoundaryAntiClock
{
	None, 
	Left, 
	Bottom, 
	Right, 
	Top
};
struct Line
{
	Point begin;
	Point end;
	BoundaryAntiClock beginBoundary;
	BoundaryAntiClock endBoundary;
};
GLint clipTest3(GLfloat p, GLfloat q, GLfloat* u1, GLfloat* u2, bool& u1Cliped, bool& u2Cliped)
{
	GLfloat r;
	GLint returnValue = true;
	u1Cliped = false;
	u2Cliped = false;

	if (p < 0.0)
	{
		r = q / p;
		if (r > *u2)
			returnValue = false;
		else if (r > *u1)
		{
			*u1 = r;
			u1Cliped = true;
		}
	}
	else if (p > 0.0)
	{
		r = q / p;
		if (r < *u1)
			returnValue = false;
		else if (r < *u2)
		{
			*u2 = r;
			u2Cliped = true;
		}
	}
	else if (q < 0.0)
		returnValue = false;

	return (returnValue);
}
void setBoundaryPoint(Point winMin, Point winMax, std::map<BoundaryAntiClock, Point>& boundaryPoint)
{
	boundaryPoint[BoundaryAntiClock::Left] = winMin;
	boundaryPoint[BoundaryAntiClock::Bottom] = { winMax.x, winMin.y };
	boundaryPoint[BoundaryAntiClock::Right] = winMax;
	boundaryPoint[BoundaryAntiClock::Top] = { winMin.x, winMax.y };
}
void polygonClipLiangBarsk3(Point winMin, Point winMax, std::vector<Point>& polygon)
{
	std::map<BoundaryAntiClock, Point> boundaryPoint;
	setBoundaryPoint(winMin, winMax, boundaryPoint);

	std::vector<Line> clipLines;
	for (int i = 0; i < polygon.size(); i++)
	{
		int next = i + 1 < polygon.size() ? i + 1 : 0;

		GLfloat u1 = 0.0, u2 = 1.0, dx = polygon[next].x - polygon[i].x, dy;
		bool u1Cliped = false, u2Cliped = false;
		Line line;
		line.begin = { polygon[i].x ,  polygon[i].y };
		line.end = { polygon[next].x ,  polygon[next].y };
		line.beginBoundary = BoundaryAntiClock::None;
		line.endBoundary = BoundaryAntiClock::None;
		if (clipTest3(-dx, polygon[i].x - winMin.x, &u1, &u2, u1Cliped, u2Cliped))
		{
			if (u1Cliped)
				line.beginBoundary = BoundaryAntiClock::Left;
			if (u2Cliped)
				line.endBoundary = BoundaryAntiClock::Left;
			if (clipTest3(dx, winMax.x - polygon[i].x, &u1, &u2, u1Cliped, u2Cliped))
			{
				if (u1Cliped)
					line.beginBoundary = BoundaryAntiClock::Right;
				if (u2Cliped)
					line.endBoundary = BoundaryAntiClock::Right;

				dy = polygon[next].y - polygon[i].y;
				if (clipTest3(-dy, polygon[i].y - winMin.y, &u1, &u2, u1Cliped, u2Cliped))
				{
					if (u1Cliped)
						line.beginBoundary = BoundaryAntiClock::Bottom;
					if (u2Cliped)
						line.endBoundary = BoundaryAntiClock::Bottom;
					if (clipTest3(dy, winMax.y - polygon[i].y, &u1, &u2, u1Cliped, u2Cliped))
					{
						if (u1Cliped)
							line.beginBoundary = BoundaryAntiClock::Top;
						if (u2Cliped)
							line.endBoundary = BoundaryAntiClock::Top;
						if (u2 < 1.0)
						{
							line.end = { line.begin.x + u2 * dx, line.begin.y + u2 * dy };
						}
						if (u1 > 0.0)
						{
							line.begin = { line.begin.x + u1 * dx, line.begin.y + u1 * dy };
						}
						clipLines.push_back(line);
					}
				}					
			}
		}
	}

	polygon.clear();
	for (int i = 0; i < clipLines.size(); i++)
	{
		polygon.push_back(clipLines[i].end);
		int next = i + 1 < clipLines.size() ? i + 1 : 0;
		if (clipLines[next].begin.x != clipLines[i].end.x || clipLines[next].begin.y != clipLines[i].end.y)
		{
			BoundaryAntiClock curBoundary = clipLines[i].endBoundary;
			while (curBoundary != clipLines[next].beginBoundary)
			{
				polygon.push_back(boundaryPoint[curBoundary]);
				curBoundary = (BoundaryAntiClock)((int)curBoundary + 1);
				if (curBoundary > BoundaryAntiClock::Top)
					curBoundary = BoundaryAntiClock::Left;
			}
			polygon.push_back(clipLines[next].begin);
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Point winMin = { 50, 220 }, winMax = { 200, 330 };

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 260, 0, winHeight);

	// polygonClipLiangBarsk1 整体裁剪，每一边整体裁剪多边形，然后传到下一条边，参见P237
	glViewport(0, 0, 260, winHeight);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	std::vector<Point> polygon1 = { { 118, 146 },{ 166, 300 },{ 12, 234 }};
	drawPolygon(polygon1);
	polygonClipLiangBarsk1(winMin, winMax, polygon1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(polygon1);

	// polygonClipLiangBarsk2 整体裁剪，但使用与P238中不同的保留点规则
	glViewport(260, 0, 260, winHeight);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	std::vector<Point> polygon2 = { { 130, 182 },{ 234, 277 },{ 117, 364 } };
	drawPolygon(polygon2);
	polygonClipLiangBarsk2(winMin, winMax, polygon2);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(polygon2);

	// polygonClipLiangBarsk3 并行裁剪，多边形每条边都进行单独裁剪，最后按照裁剪边界添加角点
	glViewport(520, 0, 260, winHeight);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	std::vector<Point> polygon3 = { { 92, 183 },{ 189, 235 },{ 242, 348 },{ 92, 359 } };
	drawPolygon(polygon3);
	polygonClipLiangBarsk3(winMin, winMax, polygon3);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(polygon3);

	glFlush();
}
void code_8_exercise_14()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_15
float FPS = 60;
class Point
{
public:
	GLfloat x, y;
};
typedef enum { None, Left, Right, Bottom, Top } Boundary;
inline GLint Round(const GLfloat a)
{
	return GLint(a + 0.5);
}
void drawPolygon(const vector<Point>& polygon)
{
	glBegin(GL_POLYGON);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
GLint inside(Point p, Boundary b, Point wMin, Point wMax)
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
void clipBoundary1(Point winMin, Point winMax, std::vector<Point>& polygon, Boundary b)
{
	auto temp = polygon;
	polygon.clear();
	for (int i = 0; i < temp.size(); i++)
	{
		int next = i + 1 < temp.size() ? i + 1 : 0;
		GLfloat u1 = 0.0, u2 = 1.0, dx = temp[next].x - temp[i].x, dy = temp[next].y - temp[i].y;
		switch (b)
		{
		case Boundary::Left:
			clipTest(-dx, temp[i].x - winMin.x, &u1, &u2);
			break;
		case Boundary::Right:
			clipTest(dx, winMax.x - temp[i].x, &u1, &u2);
			break;
		case Boundary::Bottom:
			clipTest(-dy, temp[i].y - winMin.y, &u1, &u2);
			break;
		case Boundary::Top:
			clipTest(dy, winMax.y - temp[i].y, &u1, &u2);
			break;
		default:
			break;
		}

		bool in1 = inside(temp[i], b, winMin, winMax);
		bool in2 = inside(temp[next], b, winMin, winMax);
		if ((in1 && in2) || (in1 && (!in2)))
		{
			polygon.push_back({ temp[i].x + u2 * dx, temp[i].y + u2 * dy });
		}
		else if ((!in1) && in2)
		{
			polygon.push_back({ temp[i].x + u1 * dx, temp[i].y + u1 * dy });
			polygon.push_back({ temp[i].x + u2 * dx, temp[i].y + u2 * dy });
		}
	}
}
void polygonClipLiangBarsk1(Point winMin, Point winMax, std::vector<Point>& polygon)
{
	clipBoundary1(winMin, winMax, polygon, Boundary::Left);
	clipBoundary1(winMin, winMax, polygon, Boundary::Right);
	clipBoundary1(winMin, winMax, polygon, Boundary::Bottom);
	clipBoundary1(winMin, winMax, polygon, Boundary::Top);
}
void clipBoundary2(Point winMin, Point winMax, std::vector<Point>& polygon, Boundary b)
{
	auto temp = polygon;
	polygon.clear();
	for (int i = 0; i < temp.size(); i++)
	{
		int next = i + 1 < temp.size() ? i + 1 : 0;
		GLfloat u1 = 0.0, u2 = 1.0, dx = temp[next].x - temp[i].x, dy = temp[next].y - temp[i].y;
		switch (b)
		{
		case Boundary::Left:
			clipTest(-dx, temp[i].x - winMin.x, &u1, &u2);
			break;
		case Boundary::Right:
			clipTest(dx, winMax.x - temp[i].x, &u1, &u2);
			break;
		case Boundary::Bottom:
			clipTest(-dy, temp[i].y - winMin.y, &u1, &u2);
			break;
		case Boundary::Top:
			clipTest(dy, winMax.y - temp[i].y, &u1, &u2);
			break;
		default:
			break;
		}

		bool in1 = inside(temp[i], b, winMin, winMax);
		bool in2 = inside(temp[next], b, winMin, winMax);
		if ((in1 && in2) || ((!in1) && in2))
		{
			polygon.push_back({ temp[i].x + u1 * dx, temp[i].y + u1 * dy });
		}
		else if (in1 && (!in2))
		{
			polygon.push_back({ temp[i].x + u1 * dx, temp[i].y + u1 * dy });
			polygon.push_back({ temp[i].x + u2 * dx, temp[i].y + u2 * dy });
		}
	}
}
void polygonClipLiangBarsk2(Point winMin, Point winMax, std::vector<Point>& polygon)
{
	clipBoundary2(winMin, winMax, polygon, Boundary::Left);
	clipBoundary2(winMin, winMax, polygon, Boundary::Right);
	clipBoundary2(winMin, winMax, polygon, Boundary::Bottom);
	clipBoundary2(winMin, winMax, polygon, Boundary::Top);
}
enum class BoundaryAntiClock
{
	None,
	Left,
	Bottom,
	Right,
	Top
};
struct Line
{
	Point begin;
	Point end;
	BoundaryAntiClock beginBoundary;
	BoundaryAntiClock endBoundary;
};
GLint clipTest3(GLfloat p, GLfloat q, GLfloat* u1, GLfloat* u2, bool& u1Cliped, bool& u2Cliped)
{
	GLfloat r;
	GLint returnValue = true;
	u1Cliped = false;
	u2Cliped = false;

	if (p < 0.0)
	{
		r = q / p;
		if (r > *u2)
			returnValue = false;
		else if (r > *u1)
		{
			*u1 = r;
			u1Cliped = true;
		}
	}
	else if (p > 0.0)
	{
		r = q / p;
		if (r < *u1)
			returnValue = false;
		else if (r < *u2)
		{
			*u2 = r;
			u2Cliped = true;
		}
	}
	else if (q < 0.0)
		returnValue = false;

	return (returnValue);
}
void setBoundaryPoint(Point winMin, Point winMax, std::map<BoundaryAntiClock, Point>& boundaryPoint)
{
	boundaryPoint[BoundaryAntiClock::Left] = winMin;
	boundaryPoint[BoundaryAntiClock::Bottom] = { winMax.x, winMin.y };
	boundaryPoint[BoundaryAntiClock::Right] = winMax;
	boundaryPoint[BoundaryAntiClock::Top] = { winMin.x, winMax.y };
}
void polygonClipLiangBarsk3(Point winMin, Point winMax, std::vector<Point>& polygon)
{
	std::map<BoundaryAntiClock, Point> boundaryPoint;
	setBoundaryPoint(winMin, winMax, boundaryPoint);

	std::vector<Line> clipLines;
	for (int i = 0; i < polygon.size(); i++)
	{
		int next = i + 1 < polygon.size() ? i + 1 : 0;

		GLfloat u1 = 0.0, u2 = 1.0, dx = polygon[next].x - polygon[i].x, dy;
		bool u1Cliped = false, u2Cliped = false;
		Line line;
		line.begin = { polygon[i].x ,  polygon[i].y };
		line.end = { polygon[next].x ,  polygon[next].y };
		line.beginBoundary = BoundaryAntiClock::None;
		line.endBoundary = BoundaryAntiClock::None;
		if (clipTest3(-dx, polygon[i].x - winMin.x, &u1, &u2, u1Cliped, u2Cliped))
		{
			if (u1Cliped)
				line.beginBoundary = BoundaryAntiClock::Left;
			if (u2Cliped)
				line.endBoundary = BoundaryAntiClock::Left;
			if (clipTest3(dx, winMax.x - polygon[i].x, &u1, &u2, u1Cliped, u2Cliped))
			{
				if (u1Cliped)
					line.beginBoundary = BoundaryAntiClock::Right;
				if (u2Cliped)
					line.endBoundary = BoundaryAntiClock::Right;

				dy = polygon[next].y - polygon[i].y;
				if (clipTest3(-dy, polygon[i].y - winMin.y, &u1, &u2, u1Cliped, u2Cliped))
				{
					if (u1Cliped)
						line.beginBoundary = BoundaryAntiClock::Bottom;
					if (u2Cliped)
						line.endBoundary = BoundaryAntiClock::Bottom;
					if (clipTest3(dy, winMax.y - polygon[i].y, &u1, &u2, u1Cliped, u2Cliped))
					{
						if (u1Cliped)
							line.beginBoundary = BoundaryAntiClock::Top;
						if (u2Cliped)
							line.endBoundary = BoundaryAntiClock::Top;
						if (u2 < 1.0)
						{
							line.end = { line.begin.x + u2 * dx, line.begin.y + u2 * dy };
						}
						if (u1 > 0.0)
						{
							line.begin = { line.begin.x + u1 * dx, line.begin.y + u1 * dy };
						}
						clipLines.push_back(line);
					}
				}
			}
		}
	}

	polygon.clear();
	for (int i = 0; i < clipLines.size(); i++)
	{
		polygon.push_back(clipLines[i].end);
		int next = i + 1 < clipLines.size() ? i + 1 : 0;
		if (clipLines[next].begin.x != clipLines[i].end.x || clipLines[next].begin.y != clipLines[i].end.y)
		{
			BoundaryAntiClock curBoundary = clipLines[i].endBoundary;
			while (curBoundary != clipLines[next].beginBoundary)
			{
				polygon.push_back(boundaryPoint[curBoundary]);
				curBoundary = (BoundaryAntiClock)((int)curBoundary + 1);
				if (curBoundary > BoundaryAntiClock::Top)
					curBoundary = BoundaryAntiClock::Left;
			}
			polygon.push_back(clipLines[next].begin);
		}
	}
}
Point speed = { 12, -8 };
int lastTick = 0;
float delta = 0.f;
float curTime = 0;
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	Point winMin = { 55, 250 }, winMax = { 205, 350 };
	curTime += delta;
	std::vector<Point> polygon = { { 15, 354 },{ 40, 354 },{ 50, 371 },{ 40, 393 }, { 15, 393 },{ 7, 371 } };
	std::vector<Point> curPolygon;
	for (auto & p : polygon)
	{
		curPolygon.push_back({ p.x + speed.x * curTime, p.y + speed.y * curTime });
	}

	if (curPolygon[5].x > winMax.x)
	{
		curTime = 0;
		curPolygon = polygon;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 260, 0, winHeight);

	// polygonClipLiangBarsk1 整体裁剪，每一边整体裁剪多边形，然后传到下一条边，参见P237
	auto polygon1 = curPolygon;
	glViewport(0, 0, 260, winHeight);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	drawPolygon(polygon1);
	polygonClipLiangBarsk1(winMin, winMax, polygon1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(polygon1);

	// polygonClipLiangBarsk2 整体裁剪，但使用与P238中不同的保留点规则
	auto polygon2 = curPolygon;
	glViewport(260, 0, 260, winHeight);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	drawPolygon(polygon2);
	polygonClipLiangBarsk2(winMin, winMax, polygon2);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(polygon2);

	// polygonClipLiangBarsk3 并行裁剪，多边形每条边都进行单独裁剪，最后按照裁剪边界添加角点
	auto polygon3 = curPolygon;
	glViewport(520, 0, 260, winHeight);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glEnd();

	drawPolygon(polygon3);
	polygonClipLiangBarsk3(winMin, winMax, polygon3);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(polygon3);

	glFlush();
}
void onTimer(int id)
{
	int curTick = GetTickCount();
	delta = (curTick - lastTick) / (float)1000;
	lastTick = curTick;
	glutPostRedisplay();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
void code_8_exercise_15()
{
	glutDisplayFunc(drawFunc);
	lastTick = GetTickCount();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
#endif

#ifdef CHAPTER_8_EXERCISE_16
class Point
{
public:
	GLfloat x, y;
};
enum class PointType
{
	None,    // 无类型
	Polygon, // 多边形点
	ClipWindow,	 // 裁剪窗口点
	CrossIn, // 裁剪窗口进交点
	CrossOut, //裁剪窗口出交点
};
struct CrossPointInfo
{
	float u1 = 0.0f;	// 裁剪窗口边 直线参数方程u
	float u2 = 0.0f;	// 多边形边	直线参数方程u
	Point* point;       // 交点
};
struct LineWithCross
{
	Point* begin;
	Point* end;
	std::vector<CrossPointInfo> crossPoints;
};
struct PointInfo
{
	PointType type; // 类型
	int idx1;   // 裁剪窗口顶点 数组索引
	int idx2;	// 多边形顶点 数组索引
	bool dealed; // 是否已处理

	PointInfo::PointInfo() : type(PointType::None), idx1(-1), idx2(-1), dealed(false) {}
};
inline GLint Round(const GLfloat a)
{
	return GLint(a + 0.5);
}
// 只能是凸多边形
void drawPolygon(const vector<Point>& polygon)
{
	glBegin(GL_POLYGON);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
//////////////////////////////////////////////////////////////////////////
// 任意形状多边形填充算法
struct Line
{
	int x0;
	int y0;
	int x1;
	int y1;
};
struct SortedLine
{
	int maxY;
	int minY;
	int beginX;
	int endX;
	int dx;
	int dy;
};
struct SortedLineSet
{
	int scanY;
	std::vector<SortedLine> sortedLines;
};
struct ActiveLine
{
	SortedLine sortedLine;
	int counter;
	int currentX;
};
void hLine(int y, int x0, int x1)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(x, y);
	}
}
std::vector<SortedLineSet> SortLines(const std::vector<Point>& points)
{
	std::vector<Line> lines;
	for (int i = 0; i < points.size(); i++)
	{
		int next = (i + 1) % points.size();
		// 跳过水平线
		if (points[i].y == points[next].y)
			continue;

		lines.push_back(Line());
		lines.back().x0 = points[i].x;
		lines.back().y0 = points[i].y;
		lines.back().x1 = points[next].x;
		lines.back().y1 = points[next].y;
	}

	for (int i = 0; i < lines.size(); i++)
	{
		int next = (i + 1) % lines.size();
		if (lines[i].y1 - lines[i].y0 > 0 && lines[next].y1 - lines[next].y0 > 0)
			lines[i].y1--;
		else if (lines[i].y1 - lines[i].y0 < 0 && lines[next].y1 - lines[next].y0 < 0)
			lines[next].y0--;
	}

	// 再次检查水平线
	for (auto it = lines.begin(); it != lines.end();)
	{
		if (it->y0 == it->y1)
		{
			it = lines.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (auto& line : lines)
	{
		if (line.y0 > line.y1)
		{
			std::swap(line.x0, line.x1);
			std::swap(line.y0, line.y1);
		}
	}

	std::sort(lines.begin(), lines.end(), [](auto& a, auto& b)
	{
		if (a.y0 == b.y0)
		{
			if (a.x0 == b.x0)
			{
				if (a.x1 == b.x1)
					return a.y1 < b.y1;
				return a.x1 < b.x1;
			}
			return a.x0 < b.x0;
		}
		return a.y0 < b.y0;
	});
	std::vector<SortedLineSet> lineSet;
	int lastY = -99999;
	int maxY = -99999;
	for (auto& line : lines)
	{
		if (line.y0 != lastY)
		{
			lineSet.push_back(SortedLineSet());
		}
		lineSet.back().scanY = line.y0;
		lineSet.back().sortedLines.push_back(SortedLine());
		lineSet.back().sortedLines.back().beginX = line.x0;
		lineSet.back().sortedLines.back().endX = line.x1;
		lineSet.back().sortedLines.back().maxY = line.y1;
		lineSet.back().sortedLines.back().minY = line.y0;
		lineSet.back().sortedLines.back().dx = line.x1 - line.x0;
		lineSet.back().sortedLines.back().dy = line.y1 - line.y0;
		lastY = line.y0;

		if (maxY < line.y1)
			maxY = line.y1;
	}
	lineSet.push_back({ maxY + 1 ,{} }); // 结尾
	return lineSet;
}
void fillWithActiveLines(int beginY, int endY, std::vector<ActiveLine>& activeLines)
{
	std::vector<std::vector<Point>> points;
	for (int curY = beginY; curY < endY; curY++)
	{
		for (auto& line : activeLines)
		{
			if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
			{
				if (std::abs(line.sortedLine.dy) >= std::abs(line.sortedLine.dx))
				{// |m|>1			
					points.push_back({ { (float)line.currentX , (float)curY } });

					line.counter += std::abs(line.sortedLine.dx * 2);

					if (line.counter >= line.sortedLine.dy)
					{
						if (line.sortedLine.dx > 0)
							line.currentX++;
						else
							line.currentX--;
						line.counter -= line.sortedLine.dy * 2;
					}
				}
				else
				{// |m|<1
					points.push_back({ { (float)line.currentX, (float)curY } });
					while (true)
					{
						if (line.sortedLine.dx > 0)
							line.currentX++;
						else
							line.currentX--;

						line.counter += std::abs(line.sortedLine.dy * 2);
						if ((line.counter >= std::abs(line.sortedLine.dx)) ||
							(line.sortedLine.dx > 0 ? line.currentX > line.sortedLine.endX : line.currentX < line.sortedLine.endX) /* 结束条件*/)
						{
							line.counter -= std::abs(line.sortedLine.dx * 2);
							break;
						}
					}
					if (line.sortedLine.dx > 0)
						points.back().push_back({ (float)line.currentX - 1, (float)curY });
					else
						points.back().push_back({ (float)line.currentX + 1, (float)curY });

					std::sort(points.back().begin(), points.back().end(), [](auto& a, auto&b) {return a.x < b.x;});
				}
			}
		}
		std::sort(points.begin(), points.end(), [](auto& a, auto&b)
		{
			if (a.front().x == b.front().x)
				return a.back().x < b.back().x;
			return a.front().x < b.front().x;
		});
		for (int i = 0; ; i++)
		{
			if (2 * i < points.size() && 2 * i + 1 < points.size())
			{
				hLine(points[2 * i].front().y, points[2 * i].front().x, points[2 * i + 1].back().x);
			}
			else
			{
				points.clear();
				break;
			}
		}
	}
}
void fillPolygon(const std::vector<Point>& points)
{
	std::vector<SortedLineSet> sortedLines = SortLines(points);
	std::vector<ActiveLine> activeLines;
	for (int i = 0; i < sortedLines.size() - 1; i++)
	{
		int curY = sortedLines[i].scanY;
		for (auto it = activeLines.begin(); it != activeLines.end();)
		{
			if (curY > it->sortedLine.maxY)
			{
				it = activeLines.erase(it);
			}
			else
			{
				it++;
			}
		}
		for (auto& _sortedLine : sortedLines[i].sortedLines)
		{
			activeLines.push_back(ActiveLine());
			activeLines.back().sortedLine = _sortedLine;
			activeLines.back().counter = 0;
			activeLines.back().currentX = _sortedLine.beginX;
		}
		fillWithActiveLines(curY, sortedLines[i + 1].scanY, activeLines);
	}
}
//////////////////////////////////////////////////////////////////////////
void calcLines(std::vector<Point>& points, std::vector<LineWithCross>& lines)
{
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 < points.size() ? i + 1 : 0;
		lines.push_back(LineWithCross());
		lines.back().begin = new Point{points[i]};
		lines.back().end = new Point{ points[next] };
		lines.back().crossPoints.clear();
	}
}
bool crossPoint(LineWithCross& line1, LineWithCross& line2, float& u1, float& u2)
{
	float dx1 = line1.end->x - line1.begin->x;
	float dy1 = line1.end->y - line1.begin->y;
	float dx2 = line2.end->x - line2.begin->x;
	float dy2 = line2.end->y - line2.begin->y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (dy1 / dx1 == dy2 / dx2)
		return false;

	float x01 = line1.begin->x;
	float y01 = line1.begin->y;
	float x02 = line2.begin->x;
	float y02 = line2.begin->y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);
	if ((u1 < 0 || u1 > 1) || (u2 < 0 || u2 > 1))
		return false;

	return true;
}
bool calcCrossPoint(LineWithCross& clipWindowLine, LineWithCross& polygonLine, CrossPointInfo& crossPointInfo, std::map<Point*, PointInfo>& pointInfo)
{
	if (!crossPoint(clipWindowLine, polygonLine, crossPointInfo.u1, crossPointInfo.u2))
		return false;
	
	float dx1 = clipWindowLine.end->x - clipWindowLine.begin->x;
	float dy1 = clipWindowLine.end->y - clipWindowLine.begin->y;
	float dx2 = polygonLine.end->x - polygonLine.begin->x;
	float dy2 = polygonLine.end->y - polygonLine.begin->y;
	float x01 = clipWindowLine.begin->x;
	float y01 = clipWindowLine.begin->y;

	crossPointInfo.point = new Point{ x01 + crossPointInfo.u1 * dx1, y01 + crossPointInfo.u1 * dy1 };
	if (dx1 * dy2 - dy1 * dx2 > 0)
	{
		pointInfo[crossPointInfo.point].type = PointType::CrossIn;
	}
	else
	{
		pointInfo[crossPointInfo.point].type = PointType::CrossOut;
	}
	return true;
}
void calcPointInfo(std::vector<Point>& clipWindow, std::vector<Point>& polygon, std::vector<Point*>& clipWindowPoints, std::vector<Point*>& polygonPoints, std::map<Point*, PointInfo>& pointInfo)
{
	std::vector<LineWithCross> clipWindowLines;
	std::vector<LineWithCross> polygonLines;
	calcLines(clipWindow, clipWindowLines);
	calcLines(polygon, polygonLines);

	CrossPointInfo crossPointInfo;
	for (int i = 0; i < clipWindowLines.size(); i++)
	{
		for (int j = 0; j < polygonLines.size(); j++)
		{
			if (calcCrossPoint(clipWindowLines[i], polygonLines[j], crossPointInfo, pointInfo))
			{
				clipWindowLines[i].crossPoints.push_back(crossPointInfo);
				polygonLines[j].crossPoints.push_back(crossPointInfo);
			}
		}
	}
	for (auto l : clipWindowLines)
	{
		clipWindowPoints.push_back(l.begin);
		pointInfo[clipWindowPoints.back()].type = PointType::ClipWindow;
		pointInfo[clipWindowPoints.back()].idx1 = clipWindowPoints.size() - 1;

		if (l.crossPoints.size() > 1)
		{
			std::sort(l.crossPoints.begin(), l.crossPoints.end(), [](CrossPointInfo& a, CrossPointInfo& b)
			{
				return a.u1 < b.u1;
			});
		}
		for (auto& cp : l.crossPoints)
		{
			clipWindowPoints.push_back(cp.point);
			pointInfo[clipWindowPoints.back()].idx1 = clipWindowPoints.size() - 1;
		}
	}
	for (auto l : polygonLines)
	{
		polygonPoints.push_back(l.begin);
		pointInfo[polygonPoints.back()].type = PointType::Polygon;
		pointInfo[polygonPoints.back()].idx2 = polygonPoints.size() - 1;

		if (l.crossPoints.size() > 1)
		{
			std::sort(l.crossPoints.begin(), l.crossPoints.end(), [](CrossPointInfo& a, CrossPointInfo& b)
			{
				return a.u2 < b.u2;
			});
		}
		for (auto& cp : l.crossPoints)
		{
			polygonPoints.push_back(cp.point);
			pointInfo[polygonPoints.back()].idx2 = polygonPoints.size() - 1;
		}
	}
}
bool dealPoint(Point* point, bool record, std::map<Point*, PointInfo>& pointInfo, std::vector<std::vector<Point>>& reslutPolygon)
{
	assert(pointInfo.find(point) != pointInfo.end());
	if (pointInfo[point].dealed)
		return false;

	pointInfo[point].dealed = true;

	if(record)
		reslutPolygon.back().push_back(*point);

	return true;
}
void walkClipWindow(std::vector<Point*> clipWindowPoints, int idx, std::vector<Point*> polygonPoints, std::map<Point*, PointInfo>& pointInfo, std::vector<std::vector<Point>>& reslutPolygon);
void walkPolygon(std::vector<Point*> polygonPoints, int idx, bool record, std::vector<Point*> clipWindowPoints, std::map<Point*, PointInfo>& pointInfo, std::vector<std::vector<Point>>& reslutPolygon, bool skipCurPoint = false)
{
	if (!skipCurPoint && !dealPoint(polygonPoints[idx], record, pointInfo, reslutPolygon))
	{
		if (!reslutPolygon.back().empty())
			reslutPolygon.push_back(std::vector<Point>());
		return;
	}

	idx = idx + 1 >= polygonPoints.size() ? 0 : idx + 1;

	auto p = polygonPoints[idx];
	assert(pointInfo.find(p) != pointInfo.end());
	if (pointInfo[p].type == PointType::CrossIn)
	{
		walkPolygon(polygonPoints, idx, true, clipWindowPoints, pointInfo, reslutPolygon);
	}		
	else if (pointInfo[p].type == PointType::CrossOut)
	{
		walkClipWindow(clipWindowPoints, pointInfo[p].idx1, polygonPoints, pointInfo, reslutPolygon);
		walkPolygon(polygonPoints, idx, false, clipWindowPoints, pointInfo, reslutPolygon, true);
	}
	else if (pointInfo[p].type == PointType::Polygon)
	{
		walkPolygon(polygonPoints, idx, record, clipWindowPoints, pointInfo, reslutPolygon);
	}
}
void walkClipWindow(std::vector<Point*> clipWindowPoints, int idx, std::vector<Point*> polygonPoints, std::map<Point*, PointInfo>& pointInfo, std::vector<std::vector<Point>>& reslutPolygon)
{
	if (!dealPoint(clipWindowPoints[idx], true, pointInfo, reslutPolygon))
	{
		if (!reslutPolygon.back().empty())
			reslutPolygon.push_back(std::vector<Point>());
		return;
	}

	idx = idx + 1 >= clipWindowPoints.size() ? 0 : idx + 1;

	auto p = clipWindowPoints[idx];
	assert(pointInfo.find(p) != pointInfo.end());
	if (pointInfo[p].type == PointType::CrossIn)
	{
		walkPolygon(polygonPoints, pointInfo[p].idx2, true, clipWindowPoints, pointInfo, reslutPolygon);
	}
	else if (pointInfo[p].type == PointType::ClipWindow)
	{
		walkClipWindow(clipWindowPoints, idx, polygonPoints, pointInfo, reslutPolygon);
	}
}
bool sign(float f)
{
	return f > 0;
}
bool checkRay(LineWithCross& line, std::vector<Point>& polygon)
{
	float dx = line.end->x - line.begin->x;
	float dy = line.end->y - line.begin->y;
	for (auto& p : polygon)
	{
		bool check = false;
		float _dx = p.x - line.begin->x;
		float _dy = p.y - line.begin->y;
		if (dx)
		{
			if (_dx)
			{
				if (dy / dx != _dy / _dx)
				{
					check = true;
				}
				else
				{
					if (sign(dx) == sign(_dx) && sign(dy) == sign(_dy))
						check = false;
					else
						check = true;
				}
			}
			else
			{
				check = true;
			}
		}
		else
		{
			if (_dx)
			{
				check = true;
			}
			else
			{
				if (sign(dx) == sign(_dx) && sign(dy) == sign(_dy))
					check = false;
				else
					check = true;
			}
		}
		if (!check)
			return false;
	}
	return true;
}
int crossProduct(LineWithCross& line1, LineWithCross& line2)
{
	return (line1.end->x - line1.begin->x) * (line2.end->y - line2.begin->y) - (line1.end->y - line1.begin->y) * (line2.end->x - line2.begin->x);
}
void boundBox(std::vector<Point>& polygon, float& minX, float& maxX, float& minY, float& maxY)
{
	maxX = polygon[0].x, minX = polygon[0].x, maxY = polygon[0].y, minY = polygon[0].y;
	for (int i = 1; i < polygon.size(); i++)
	{
		if (polygon[i].x < minX)
			minX = polygon[i].x;
		if (polygon[i].x > maxX)
			maxX = polygon[i].x;
		if (polygon[i].y < minY)
			minY = polygon[i].y;
		if (polygon[i].y > maxY)
			maxY = polygon[i].y;
	}
}
bool checkVertex(Point p, std::vector<Point>& polygon)
{
	for (auto& _p : polygon)
	{
		if (p.x == _p.x && p.y == _p.y)
			return false;
	}
	return true;
}
bool checkIn(Point p, std::vector<Point>& polygon)
{
	if (!checkVertex(p, polygon))
		return false;

	float minX, maxX, minY, maxY;
	boundBox(polygon, minX, maxX, minY, maxY);
	float length = (maxX - minX) + (maxY - minY);
	float theta = 0;
	float dtheta = PI / 180;
	while (theta < 2 * PI)
	{
		Point end = { p.x + length * cos(theta), p.y + length * sin(theta) };
		LineWithCross ray;
		ray.begin = &p;
		ray.end = &end;
		if (checkRay(ray, polygon))
		{
			float u1 = 0, u2 = 0;
			int count = 0;
			for (int i = 0; i < polygon.size(); i++)
			{
				int next = i + 1 < polygon.size() ? i + 1 : 0;
				LineWithCross edge;
				edge.begin = &polygon[i];
				edge.end = &polygon[next];

				if (crossPoint(ray, edge, u1, u2))
				{
					if (crossProduct(ray, edge) > 0)
						count++;
					else
						count--;
				}
			}
			return count >= 1;
		}
		else
		{
			theta += dtheta;
		}
	}
	assert(0 && "can not find suitable ray!!!");
	return false;
}
void polygonClipWeilerAtherton(std::vector<Point>& clipWindow, std::vector<Point>& polygon, std::vector<std::vector<Point>>& reslutPolygon)
{
	std::vector<Point*> clipWindowPoints;
	std::vector<Point*> polygonPoints;
	std::map<Point*, PointInfo> pointInfo;
	calcPointInfo(clipWindow, polygon, clipWindowPoints, polygonPoints, pointInfo);

	reslutPolygon.clear();
	reslutPolygon.push_back(std::vector<Point>());
	walkPolygon(polygonPoints, 0, checkIn(polygon[0], clipWindow), clipWindowPoints, pointInfo, reslutPolygon);
	if (reslutPolygon.back().empty())
		reslutPolygon.erase(reslutPolygon.end() - 1);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::vector<Point> polygon, clipWindow;
	std::vector <std::vector<Point>> polygons;

	glViewport(0, 300, winWidth / 2, winHeight / 2);
	polygon = { { 54, 227 },{ 85, 145 },{ 207, 183 },{ 191, 267 },{ 104, 283 } };
	clipWindow = { { 150, 50 },{ 300, 50 },{ 300, 250 },{ 150, 250 } };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	polygons.clear();
	polygonClipWeilerAtherton(clipWindow, polygon, polygons);
	glColor3f(1.0, 0.0, 0.0);
	for (auto po : polygons)
	{
		drawPolygon(po);
	}

	glViewport(400, 300, winWidth / 2, winHeight / 2);
	polygon = { { 238, 182 },{ 207, 276 },{ 9, 165 },{ 74, 58 },{ 198, 95 },{77, 122} };
	clipWindow = { { 150, 50 },{ 350, 50 },{ 350, 220 },{ 150, 220 } };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	polygons.clear();
	polygonClipWeilerAtherton(clipWindow, polygon, polygons);
	glColor3f(1.0, 0.0, 0.0);
	for (auto po : polygons)
	{
		drawPolygon(po);
	}

	glViewport(0, 0, winWidth / 2, winHeight / 2);
	polygon = { { 328, 238 },{ 161, 279 },{ 178, 236 },{ 117, 110 },{ 167, 24 },{ 216, 33 } };
	clipWindow = { { 81, 14 },{ 211, 68 },{ 204, 145 },{ 79, 159 },{ 41, 87 } };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	polygons.clear();
	polygonClipWeilerAtherton(clipWindow, polygon, polygons);
	glColor3f(1.0, 0.0, 0.0);
	for (auto po : polygons)
	{
		drawPolygon(po);
	}

	glViewport(400, 0, winWidth / 2, winHeight / 2);
	polygon = { { 128, 62 },{ 176, 119 },{ 303, 62 },{ 326, 216 },{ 95, 254 },{ 76, 82 } };
	clipWindow = { { 184, 30 },{ 290, 115 },{ 169, 95 },{ 145, 131 },{ 88, 107 },{40, 136},{45, 32} };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	polygons.clear();
	polygonClipWeilerAtherton(clipWindow, polygon, polygons);
	glColor3f(1.0, 0.0, 0.0);
	for (auto po : polygons)
	{
		drawPolygon(po);
	}

	glFlush();
}
void code_8_exercise_16()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth / 2, 0, winHeight / 2);

	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_16_1
class Point
{
public:
	GLfloat x, y;
};
struct CrossPointInfo
{
	float u1 = 0.0f;	// 裁剪窗口边 直线参数方程u
	float u2 = 0.0f;	// 多边形边	直线参数方程u
	Point* point;       // 交点
};
struct LineWithCross
{
	Point* begin;
	Point* end;
	std::vector<CrossPointInfo> crossPoints;
};
inline GLint Round(const GLfloat a)
{
	return GLint(a + 0.5);
}
void drawPoint(float x, float y)
{
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}
// 只能是凸多边形
void drawPolygon(const vector<Point>& polygon)
{
	glBegin(GL_POLYGON);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
//////////////////////////////////////////////////////////////////////////
// 任意形状多边形填充算法
struct Line
{
	int x0;
	int y0;
	int x1;
	int y1;
};
struct SortedLine
{
	int maxY;
	int minY;
	int beginX;
	int endX;
	int dx;
	int dy;
};
struct SortedLineSet
{
	int scanY;
	std::vector<SortedLine> sortedLines;
};
struct ActiveLine
{
	SortedLine sortedLine;
	int counter;
	int currentX;
};
void hLine(int y, int x0, int x1)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(x, y);
	}
}
std::vector<SortedLineSet> SortLines(const std::vector<Point>& points)
{
	std::vector<Line> lines;
	for (int i = 0; i < points.size(); i++)
	{
		int next = (i + 1) % points.size();
		// 跳过水平线
		if (points[i].y == points[next].y)
			continue;

		lines.push_back(Line());
		lines.back().x0 = points[i].x;
		lines.back().y0 = points[i].y;
		lines.back().x1 = points[next].x;
		lines.back().y1 = points[next].y;
	}

	for (int i = 0; i < lines.size(); i++)
	{
		int next = (i + 1) % lines.size();
		if (lines[i].y1 - lines[i].y0 > 0 && lines[next].y1 - lines[next].y0 > 0)
			lines[i].y1--;
		else if (lines[i].y1 - lines[i].y0 < 0 && lines[next].y1 - lines[next].y0 < 0)
			lines[next].y0--;
	}

	// 再次检查水平线
	for (auto it = lines.begin(); it != lines.end();)
	{
		if (it->y0 == it->y1)
		{
			it = lines.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (auto& line : lines)
	{
		if (line.y0 > line.y1)
		{
			std::swap(line.x0, line.x1);
			std::swap(line.y0, line.y1);
		}
	}

	std::sort(lines.begin(), lines.end(), [](auto& a, auto& b)
	{
		if (a.y0 == b.y0)
		{
			if (a.x0 == b.x0)
			{
				if (a.x1 == b.x1)
					return a.y1 < b.y1;
				return a.x1 < b.x1;
			}
			return a.x0 < b.x0;
		}
		return a.y0 < b.y0;
	});
	std::vector<SortedLineSet> lineSet;
	int lastY = -99999;
	int maxY = -99999;
	for (auto& line : lines)
	{
		if (line.y0 != lastY)
		{
			lineSet.push_back(SortedLineSet());
		}
		lineSet.back().scanY = line.y0;
		lineSet.back().sortedLines.push_back(SortedLine());
		lineSet.back().sortedLines.back().beginX = line.x0;
		lineSet.back().sortedLines.back().endX = line.x1;
		lineSet.back().sortedLines.back().maxY = line.y1;
		lineSet.back().sortedLines.back().minY = line.y0;
		lineSet.back().sortedLines.back().dx = line.x1 - line.x0;
		lineSet.back().sortedLines.back().dy = line.y1 - line.y0;
		lastY = line.y0;

		if (maxY < line.y1)
			maxY = line.y1;
	}
	lineSet.push_back({ maxY + 1 ,{} }); // 结尾
	return lineSet;
}
void fillWithActiveLines(int beginY, int endY, std::vector<ActiveLine>& activeLines)
{
	std::vector<std::vector<Point>> points;
	for (int curY = beginY; curY < endY; curY++)
	{
		for (auto& line : activeLines)
		{
			if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
			{
				if (std::abs(line.sortedLine.dy) >= std::abs(line.sortedLine.dx))
				{// |m|>1			
					points.push_back({ { (float)line.currentX , (float)curY } });

					line.counter += std::abs(line.sortedLine.dx * 2);

					if (line.counter >= line.sortedLine.dy)
					{
						if (line.sortedLine.dx > 0)
							line.currentX++;
						else
							line.currentX--;
						line.counter -= line.sortedLine.dy * 2;
					}
				}
				else
				{// |m|<1
					points.push_back({ { (float)line.currentX, (float)curY } });
					while (true)
					{
						if (line.sortedLine.dx > 0)
							line.currentX++;
						else
							line.currentX--;

						line.counter += std::abs(line.sortedLine.dy * 2);
						if ((line.counter >= std::abs(line.sortedLine.dx)) ||
							(line.sortedLine.dx > 0 ? line.currentX > line.sortedLine.endX : line.currentX < line.sortedLine.endX) /* 结束条件*/)
						{
							line.counter -= std::abs(line.sortedLine.dx * 2);
							break;
						}
					}
					if (line.sortedLine.dx > 0)
						points.back().push_back({ (float)line.currentX - 1, (float)curY });
					else
						points.back().push_back({ (float)line.currentX + 1, (float)curY });

					std::sort(points.back().begin(), points.back().end(), [](auto& a, auto&b) {return a.x < b.x;});
				}
			}
		}
		std::sort(points.begin(), points.end(), [](auto& a, auto&b)
		{
			if (a.front().x == b.front().x)
				return a.back().x < b.back().x;
			return a.front().x < b.front().x;
		});
		for (int i = 0; ; i++)
		{
			if (2 * i < points.size() && 2 * i + 1 < points.size())
			{
				hLine(points[2 * i].front().y, points[2 * i].front().x, points[2 * i + 1].back().x);
			}
			else
			{
				points.clear();
				break;
			}
		}
	}
}
void fillPolygon(const std::vector<Point>& points)
{
	std::vector<SortedLineSet> sortedLines = SortLines(points);
	std::vector<ActiveLine> activeLines;
	for (int i = 0; i < sortedLines.size() - 1; i++)
	{
		int curY = sortedLines[i].scanY;
		for (auto it = activeLines.begin(); it != activeLines.end();)
		{
			if (curY > it->sortedLine.maxY)
			{
				it = activeLines.erase(it);
			}
			else
			{
				it++;
			}
		}
		for (auto& _sortedLine : sortedLines[i].sortedLines)
		{
			activeLines.push_back(ActiveLine());
			activeLines.back().sortedLine = _sortedLine;
			activeLines.back().counter = 0;
			activeLines.back().currentX = _sortedLine.beginX;
		}
		fillWithActiveLines(curY, sortedLines[i + 1].scanY, activeLines);
	}
}
//////////////////////////////////////////////////////////////////////////
bool crossPoint(LineWithCross& line1, LineWithCross& line2, float& u1, float& u2)
{
	float dx1 = line1.end->x - line1.begin->x;
	float dy1 = line1.end->y - line1.begin->y;
	float dx2 = line2.end->x - line2.begin->x;
	float dy2 = line2.end->y - line2.begin->y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (dy1 / dx1 == dy2 / dx2)
		return false;

	float x01 = line1.begin->x;
	float y01 = line1.begin->y;
	float x02 = line2.begin->x;
	float y02 = line2.begin->y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);
	if ((u1 < 0 || u1 > 1) || (u2 < 0 || u2 > 1))
		return false;

	return true;
}
bool sign(float f)
{
	return f > 0;
}
bool checkRay(LineWithCross& line, std::vector<Point>& polygon)
{
	float dx = line.end->x - line.begin->x;
	float dy = line.end->y - line.begin->y;
	for (auto& p : polygon)
	{
		bool check = false;
		float _dx = p.x - line.begin->x;
		float _dy = p.y - line.begin->y;
		if (dx)
		{
			if (_dx)
			{
				if (dy / dx != _dy / _dx)
				{
					check = true;
				}
				else
				{
					if (sign(dx) == sign(_dx) && sign(dy) == sign(_dy))
						check = false;
					else
						check = true;
				}
			}
			else
			{
				check = true;
			}
		}
		else
		{
			if (_dx)
			{
				check = true;
			}
			else
			{
				if (sign(dx) == sign(_dx) && sign(dy) == sign(_dy))
					check = false;
				else
					check = true;
			}
		}
		if (!check)
			return false;
	}
	return true;
}
int crossProduct(LineWithCross& line1, LineWithCross& line2)
{
	float ax = line1.end->x - line1.begin->x;
	float by = line2.end->y - line2.begin->y;
	float ay = line1.end->y - line1.begin->y;
	float bx = line2.end->x - line2.begin->x;
	auto ret = ax * by - ay * bx;
	return (line1.end->x - line1.begin->x) * (line2.end->y - line2.begin->y) - (line1.end->y - line1.begin->y) * (line2.end->x - line2.begin->x);
}
void boundBox(std::vector<Point>& polygon, float& minX, float& maxX, float& minY, float& maxY)
{
	maxX = polygon[0].x, minX = polygon[0].x, maxY = polygon[0].y, minY = polygon[0].y;
	for (int i = 1; i < polygon.size(); i++)
	{
		if (polygon[i].x < minX)
			minX = polygon[i].x;
		if (polygon[i].x > maxX)
			maxX = polygon[i].x;
		if (polygon[i].y < minY)
			minY = polygon[i].y;
		if (polygon[i].y > maxY)
			maxY = polygon[i].y;
	}
}
bool boundBoxIntersection(std::vector<Point>& polygon1, std::vector<Point>& polygon2, float& minX, float& maxX, float& minY, float& maxY)
{
	float minX1, maxX1, minY1, maxY1;
	boundBox(polygon1, minX1, maxX1, minY1, maxY1);
	float minX2, maxX2, minY2, maxY2;
	boundBox(polygon2, minX2, maxX2, minY2, maxY2);

	minX = std::max(minX1, minX2);
	maxX = std::min(maxX1, maxX2);
	minY = std::max(minY1, minY2);
	maxY = std::min(maxY1, maxY2);

	return (maxX > minX && maxY > minY);
}
void boundBoxUnion(std::vector<Point>& polygon1, std::vector<Point>& polygon2, float& minX, float& maxX, float& minY, float& maxY)
{
	float minX1, maxX1, minY1, maxY1;
	boundBox(polygon1, minX1, maxX1, minY1, maxY1);
	float minX2, maxX2, minY2, maxY2;
	boundBox(polygon2, minX2, maxX2, minY2, maxY2);

	minX = std::min(minX1, minX2);
	maxX = std::max(maxX1, maxX2);
	minY = std::min(minY1, minY2);
	maxY = std::max(maxY1, maxY2);
}
bool checkVertex(Point p, std::vector<Point>& polygon)
{
	for (auto& _p : polygon)
	{
		if (p.x == _p.x && p.y == _p.y)
			return false;
	}
	return true;
}
bool checkIn(Point p, std::vector<Point>& polygon)
{
	if (!checkVertex(p, polygon))
		return false;

	float minX, maxX, minY , maxY;
	boundBox(polygon, minX, maxX, minY, maxY);
	float length = (maxX - minX) + (maxY - minY);
	float theta = 0;
	float dtheta = PI / 180;
	while (theta < 2 * PI)
	{
		Point end = { p.x + length * cos(theta), p.y + length * sin(theta) };
		LineWithCross ray;
		ray.begin = &p;
		ray.end = &end;
		if (checkRay(ray, polygon))
		{
			float u1 = 0, u2 = 0;
			int count = 0;
			for (int i = 0; i < polygon.size(); i++)
			{
				int next = i + 1 < polygon.size() ? i + 1 : 0;
				LineWithCross edge;
				edge.begin = &polygon[i];
				edge.end = &polygon[next];

				if (crossPoint(ray, edge, u1, u2))
				{
					if (crossProduct(ray, edge) > 0)
						count++;
					else
						count--;
				}
			}
			return count >= 1;
		}
		else
		{
			theta += dtheta;
		}
	}
	assert(0 && "can not find suitable ray!!!");
	return false;
}
void polygonClipCheckIn(std::vector<Point>& clipWindow, std::vector<Point>& polygon)
{
	float minX, maxX, minY, maxY;

	if (boundBoxIntersection(clipWindow, polygon, minX, maxX, minY, maxY))
	{
		for (float y = minY; y <= maxY; y++)
		{
			for (float x = minX; x <= maxX; x++)
			{
				if (checkIn({ x, y }, clipWindow) && checkIn({ x, y }, polygon))
					drawPoint(x, y);
			}
		}
	}
}
bool checkIntersection(Point p, std::vector<Point>& polygon1, std::vector<Point>& polygon2)
{
	if (!checkVertex(p, polygon1) || !checkVertex(p, polygon2))
		return false;

	float minX, maxX, minY, maxY;
	boundBoxUnion(polygon1, polygon2, minX, maxX, minY, maxY);
	float length = (maxX - minX) + (maxY - minY);
	float theta = 0;
	float dtheta = PI / 180;
	while (theta < 2 * PI)
	{
		Point end = { p.x + length * cos(theta), p.y + length * sin(theta) };
		LineWithCross ray;
		ray.begin = &p;
		ray.end = &end;
		if (checkRay(ray, polygon1) && checkRay(ray, polygon2))
		{
			float u1 = 0, u2 = 0;
			int count = 0;
			for (int i = 0; i < polygon1.size(); i++)
			{
				int next = i + 1 < polygon1.size() ? i + 1 : 0;
				LineWithCross edge;
				edge.begin = &polygon1[i];
				edge.end = &polygon1[next];

				if (crossPoint(ray, edge, u1, u2))
				{
					if (crossProduct(ray, edge) > 0)
						count++;
					else
						count--;
				}
			}
			for (int i = 0; i < polygon2.size(); i++)
			{
				int next = i + 1 < polygon2.size() ? i + 1 : 0;
				LineWithCross edge;
				edge.begin = &polygon2[i];
				edge.end = &polygon2[next];

				if (crossPoint(ray, edge, u1, u2))
				{
					if (crossProduct(ray, edge) > 0)
						count++;
					else
						count--;
				}
			}
			return count >= 2;
		}
		else
		{
			theta += dtheta;
		}
	}
	assert(0 && "can not find suitable ray!!!");
	return false;
}
void polygonClipCheckIntersection(std::vector<Point>& clipWindow, std::vector<Point>& polygon)
{
	float minX, maxX, minY, maxY;

	if (boundBoxIntersection(clipWindow, polygon, minX, maxX, minY, maxY))
	{
		for (float y = minY; y <= maxY; y++)
		{
			for (float x = minX; x <= maxX; x++)
			{
				if (checkIntersection({ x, y }, clipWindow, polygon))
					drawPoint(x, y);
			}
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<Point> polygon, clipWindow;

	glViewport(0, winHeight / 2, winWidth / 3, winHeight / 2);
	polygon = { { 34, 105 },{ 133, 123 },{ 193, 113 },{237, 237},{ 84, 200 },{17, 134} };
	clipWindow = { { 233, 183 },{ 107, 128 },{ 40, 148 },{ 130, 39 },{170, 94},{226, 75} };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipCheckIn(clipWindow, polygon);

	glViewport(winWidth / 3, winHeight / 2, winWidth / 3, winHeight / 2);
	polygon = { { 56, 62 },{ 114, 123 },{ 29, 158 }};
	clipWindow = { { 114, 50 },{ 190, 14 },{ 234, 79 },{ 166, 188 },{ 114, 188 } };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipCheckIn(clipWindow, polygon);

	glViewport(2 * winWidth / 3, winHeight / 2, winWidth / 3, winHeight / 2);
	polygon = { { 102, 46 },{ 139, 111 },{ 74, 174 },{49, 82} };
	clipWindow = { { 139, 111 },{ 121, 26 },{ 236, 71 },{ 127, 196 }};
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipCheckIn(clipWindow, polygon);

	glViewport(0, 0, winWidth / 3, winHeight / 2);
	polygon = { { 54, 118 },{ 116, 76 },{ 218, 174 },{ 124, 207 } };
	clipWindow = { { 54, 54 },{ 165, 54 },{ 165, 180 },{ 54, 180 } };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipCheckIntersection(clipWindow, polygon);

	glViewport(winWidth / 3, 0, winWidth / 3, winHeight / 2);
	polygon = { { 134, 91 },{ 191, 114 },{ 126, 153 },{ 108, 132 } };
	clipWindow = { { 60, 48 },{ 189, 58 },{ 208, 138 },{ 194, 274 },{50, 252},{ 108, 132 } };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipCheckIntersection(clipWindow, polygon);

	glViewport(2 * winWidth / 3, 0, winWidth / 3, winHeight / 2);
	polygon = { { 124, 107 },{ 61, 139 },{ 13, 122 },{ 67, 58 } };
	clipWindow = { { 115, 20 },{ 177, 139 },{ 115, 228 },{ 61, 139 }};
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipCheckIntersection(clipWindow, polygon);

	glFlush();
}
void code_8_exercise_16_1()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth / 3, 0, winHeight / 2);

	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_20
class Point
{
public:
	GLfloat x, y;
};
struct Ellipse_
{
	float xc;
	float yc;
	float rx;
	float ry;
};
struct Rect
{
	float minX;
	float minY;
	float maxX;
	float maxY;
};
struct EllipseArea
{
	bool RightTop;
	bool RightBottom;
	bool LeftTop;
	bool LeftBottom;
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void hLineEllipsePlot(int y, int x0, int x1, float xc, float yc)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(xc + x, yc + y);
		setPixel(xc - x, yc + y);
		setPixel(xc + x, yc - y);
		setPixel(xc - x, yc - y);
	}
}
void fliiEllipse(const Ellipse_& ellipse)
{
	int Rx2 = ellipse.rx * ellipse.rx;
	int Ry2 = ellipse.ry * ellipse.ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = ellipse.ry;
	int px = 0;
	int py = twoRx2*y;
	//hLineEllipsePlot(y, 0, x, xCenter, yCenter); // y不变时，每次绘制扫描线会造成重复
	/*Region 1*/
	p = Round(Ry2 - (Rx2 * ellipse.ry) + (0.25 * Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			hLineEllipsePlot(y, 0, x - 1, ellipse.xc, ellipse.yc); // 绘制上一条x最大的扫描线

			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		//hLineEllipsePlot(y, 0, x, xCenter, yCenter); // y不变时，每次绘制扫描线会造成重复
	}
	hLineEllipsePlot(y, 0, x, ellipse.xc, ellipse.yc); // 绘制最后一条扫描线

	/*Region 2*/
	p = Round(Ry2*(x + 0.5)*(x + 0.5) + Rx2*(y - 1)*(y - 1) - Rx2*Ry2);
	while (y > 0)
	{
		y--;
		py -= twoRx2;
		if (p > 0)
			p += Rx2 - py;
		else
		{
			x++;
			px += twoRy2;
			p += Rx2 - py + px;
		}
		hLineEllipsePlot(y, 0, x, ellipse.xc, ellipse.yc);
	}
}
bool rectIntersection(const Rect& rect1, const Rect& rect2)
{
	return (std::min(rect1.maxX, rect2.maxX) > std::max(rect1.minX, rect2.minX) &&
		std::min(rect1.maxY, rect2.maxY) > std::max(rect1.minY, rect2.minY));
}
bool clipHLineEllipsePlot(int y, int x0, int x1, float xc, float yc, Rect clipWindow, EllipseArea area)
{
	float begin, end;
	bool bContinus = true;
	if (area.RightTop)
	{
		float curY = yc + y;
		if (curY <= clipWindow.maxY && curY >= clipWindow.minY)
		{
			begin = std::max(xc + x0, clipWindow.minX);
			end = std::min(xc + x1, clipWindow.maxX);
			for (float x = begin; x <= end; x++)
			{
				setPixel(x, curY);
			}
		}
		if (bContinus)
			bContinus = curY > clipWindow.minY;
	}

	if (area.RightBottom)
	{
		float curY = yc - y;
		if (curY <= clipWindow.maxY && curY >= clipWindow.minY)
		{
			begin = std::max(xc + x0, clipWindow.minX);
			end = std::min(xc + x1, clipWindow.maxX);
			for (float x = begin; x <= end; x++)
			{
				setPixel(x, curY);
			}
		}
		if (bContinus)
			bContinus = curY < clipWindow.maxY;
	}

	if (area.LeftTop)
	{
		float curY = yc + y;
		if (curY <= clipWindow.maxY && curY >= clipWindow.minY)
		{
			begin = std::min(xc - x0, clipWindow.maxX);
			end = std::max(xc - x1, clipWindow.minX);
			for (float x = begin; x >= end; x--)
			{
				setPixel(x, curY);
			}
		}
		if (bContinus)
			bContinus = curY > clipWindow.minY;
	}

	if (area.LeftBottom)
	{
		float curY = yc - y;
		if (curY <= clipWindow.maxY && curY >= clipWindow.minY)
		{
			begin = std::min(xc - x0, clipWindow.maxX);
			end = std::max(xc - x1, clipWindow.minX);
			for (float x = begin; x >= end; x--)
			{
				setPixel(x, curY);
			}
		}
		if (bContinus)
			bContinus = curY < clipWindow.maxY;
	}

	return bContinus;
}
void _clipEllipse(const Ellipse_& ellipse, const Rect& clipWindow, EllipseArea area)
{
	int Rx2 = ellipse.rx * ellipse.rx;
	int Ry2 = ellipse.ry * ellipse.ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = ellipse.ry;
	int px = 0;
	int py = twoRx2*y;
	//hLineEllipsePlot(y, 0, x, xCenter, yCenter); // y不变时，每次绘制扫描线会造成重复
	/*Region 1*/
	p = Round(Ry2 - (Rx2 * ellipse.ry) + (0.25 * Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			if(!clipHLineEllipsePlot(y, 0, x - 1, ellipse.xc, ellipse.yc, clipWindow , area)) // 绘制上一条x最大的扫描线
				return; 

			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		//hLineEllipsePlot(y, 0, x, xCenter, yCenter); // y不变时，每次绘制扫描线会造成重复
	}
	if (!clipHLineEllipsePlot(y, 0, x, ellipse.xc, ellipse.yc, clipWindow, area)) // 绘制最后一条扫描线
		return;

	/*Region 2*/
	p = Round(Ry2*(x + 0.5)*(x + 0.5) + Rx2*(y - 1)*(y - 1) - Rx2*Ry2);
	while (y > 0)
	{
		y--;
		py -= twoRx2;
		if (p > 0)
			p += Rx2 - py;
		else
		{
			x++;
			px += twoRy2;
			p += Rx2 - py + px;
		}
		if (!clipHLineEllipsePlot(y, 0, x, ellipse.xc, ellipse.yc, clipWindow, area))
			return;
	}
}
void clipEllipse(const Ellipse_& ellipse, const Rect& clipWindow)
{
	EllipseArea area;
	area.RightTop = rectIntersection(clipWindow, { ellipse.xc, ellipse.yc, ellipse.xc + ellipse.rx, ellipse.yc + ellipse.ry });
	area.RightBottom = rectIntersection(clipWindow, { ellipse.xc, ellipse.yc - ellipse.ry, ellipse.xc + ellipse.rx, ellipse.yc });
	area.LeftTop = rectIntersection(clipWindow, { ellipse.xc - ellipse.rx, ellipse.yc, ellipse.xc, ellipse.yc + ellipse.ry });
	area.LeftBottom = rectIntersection(clipWindow, { ellipse.xc - ellipse.rx, ellipse.yc - ellipse.ry, ellipse.xc, ellipse.yc });

	if(area.RightTop || area.RightBottom || area.LeftTop || area.LeftBottom)
		_clipEllipse(ellipse, clipWindow, area);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Ellipse_ ellipse;
	Rect clipWindow;

	glViewport(0, 400, 200, 200);
	ellipse = { 138 , 142 , 24, 18};
	clipWindow = { 23, 50, 133, 135 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY},
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(200, 400, 200, 200);
	ellipse = { 138 , 59 , 24, 18 };
	clipWindow = { 23, 50, 133, 135 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(400, 400, 200, 200);
	ellipse = { 62 , 88 , 24, 18 };
	clipWindow = { 70, 50, 167, 135 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(600, 400, 200, 200);
	ellipse = { 72 , 42 , 24, 18 };
	clipWindow = { 23, 50, 133, 135 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(0, 200, 200, 200);
	ellipse = { 77 , 127 , 24, 18 };
	clipWindow = { 70, 50, 167, 135 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(200, 200, 200, 200);
	ellipse = { 42 , 64 , 24, 18 };
	clipWindow = { 23, 50, 133, 135 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(400, 200, 200, 200);
	ellipse = { 100 , 100 , 80, 50 };
	clipWindow = { 30, 60, 170, 140 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(600, 200, 200, 200);
	ellipse = { 100 , 100 , 70, 40 };
	clipWindow = { 20, 70, 120, 115 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(0, 0, 200, 200);
	ellipse = { 100 , 100 , 70, 40 };
	clipWindow = { 20, 110, 180, 130 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(200, 0, 200, 200);
	ellipse = { 100 , 100 , 70, 40 };
	clipWindow = { 80, 80, 120, 160 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(400, 0, 200, 200);
	ellipse = { 100 , 100 , 70, 40 };
	clipWindow = { 60, 80, 120, 120 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glViewport(600, 0, 200, 200);
	ellipse = { 100 , 100 , 60, 30 };
	clipWindow = { 30, 60, 170, 140 };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	fliiEllipse(ellipse);
	glColor3f(1.0, 0.0, 0.0);
	clipEllipse(ellipse, clipWindow);

	glFlush();
}
void code_8_exercise_20()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 200, 0, 200);

	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_21
struct Point { float x; float y; };
struct Rect
{
	float minX;
	float minY;
	float maxX;
	float maxY;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
enum class TextPath
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct TextInfo
{
	int space;
	double upVector;
	TextPath textPath;
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void drawStencil(float x, float y, const Stencil& s, float rotate)
{
	float sin = std::sin(rotate);
	float cos = std::cos(rotate);
	for (int i = 0; i < s.stencil.size(); i++)
	{
		for (int j = 0; j < s.stencil[i].size(); j++)
		{
			if (s.stencil[i][j] == 1)
			{
				int curX = j;
				int curY = s.stencil.size() - 1 - i;

				setPixel(Round(x + curX * cos - curY * sin), Round(y + curY * cos + curX * sin));
			}
		}
	}
}
void drawString(Point pos, const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	float posX = pos.x;
	float posY = pos.y;
	float charRotate = -1 * PI / 2 + info.upVector;
	for (int i = 0; i < str.size(); i++)
	{
		if (texts.find(str[i]) != texts.end())
		{
			const Stencil& s = texts.find(str[i])->second;
			drawStencil(posX, posY, s, charRotate);
			switch (info.textPath)
			{
			case TextPath::UP:
				posX += (s.stencil.size() + info.space) * std::cos(info.upVector);
				posY += (s.stencil.size() + info.space) * std::sin(info.upVector);
				break;
			case TextPath::DOWN:
				if (i + 1 < str.size())
				{
					const Stencil& nextS = texts.find(str[i + 1])->second;
					posX += (nextS.stencil.size() + info.space) * std::cos(info.upVector + PI);
					posY += (nextS.stencil.size() + info.space) * std::sin(info.upVector + PI);
				}
				break;
			case TextPath::LEFT:
				if (i + 1 < str.size())
				{
					const Stencil& nextS = texts.find(str[i + 1])->second;
					posX += (nextS.stencil[0].size() + info.space) * std::cos(info.upVector + PI / 2);
					posY += (nextS.stencil[0].size() + info.space) * std::sin(info.upVector + PI / 2);
				}
				break;
			case TextPath::RIGHT:
				posX += (s.stencil[0].size() + info.space) * std::cos(info.upVector - PI / 2);
				posY += (s.stencil[0].size() + info.space) * std::sin(info.upVector - PI / 2);
				break;
			default:
				break;
			}
		}
	}
}
bool pointInRect(Point p, Rect rect)
{
	return p.x >= rect.minX && p.x <= rect.maxX && p.y >= rect.minY && p.y <= rect.maxY;
}
void clipString_all_or_none_string_clipping(Rect clipWindow, Point pos, const std::string& str, const TextInfo& info, std::map<char, Stencil>& texts)
{
	Point leftBottom, rightBottom, rightTop, leftTop;

	float width = 0, height = 0;

	if (info.textPath == TextPath::RIGHT)
	{
		for (auto& c : str)
		{
			if (texts.find(c) != texts.end())
			{
				const Stencil& s = texts.find(c)->second;
				
				width += s.stencil[0].size();
				height = std::max(height, (float)s.stencil.size());
			}
		}
		width += (str.size() - 1) * info.space;

		leftBottom = pos;
		leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
		rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
		rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
	}
	else if (info.textPath == TextPath::LEFT)
	{
		for (auto& c : str)
		{
			if (texts.find(c) != texts.end())
			{
				const Stencil& s = texts.find(c)->second;

				width += s.stencil[0].size();
				height = std::max(height, (float)s.stencil.size());
			}
		}
		width += (str.size() - 1) * info.space;

		float firstWidth = texts[str[0]].stencil[0].size();
		rightBottom = { pos.x + firstWidth * (float)cos(info.upVector - PI / 2), pos.y + firstWidth * (float)sin(info.upVector - PI / 2) };
		rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
		leftBottom = { rightBottom.x + width * (float)cos(info.upVector + PI / 2), rightBottom.y + width * (float)sin(info.upVector + PI / 2) };
		leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
	}
	else if (info.textPath == TextPath::UP)
	{
		for (auto& c : str)
		{
			if (texts.find(c) != texts.end())
			{
				const Stencil& s = texts.find(c)->second;

				height += s.stencil.size();
				width = std::max(width, (float)s.stencil[0].size());
			}
		}
		height += (str.size() - 1) * info.space;

		leftBottom = pos;
		leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
		rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
		rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
	}
	else if (info.textPath == TextPath::DOWN)
	{
		for (auto& c : str)
		{
			if (texts.find(c) != texts.end())
			{
				const Stencil& s = texts.find(c)->second;

				height += s.stencil.size();
				width = std::max(width, (float)s.stencil[0].size());
			}
		}
		height += (str.size() - 1) * info.space;

		float firstHeight = texts[str[0]].stencil.size();
		leftTop = { pos.x + firstHeight * (float)cos(info.upVector), pos.y + firstHeight * (float)sin(info.upVector) };
		leftBottom = { leftTop.x + height * (float)cos(info.upVector + PI), leftTop.y + height * (float)sin(info.upVector + PI) };
		rightTop = { leftTop.x + width * (float)cos(info.upVector - PI / 2), leftTop.y + width * (float)sin(info.upVector - PI / 2) };
		rightBottom = { rightTop.x + height * (float)cos(info.upVector + PI), rightTop.y + height * (float)sin(info.upVector + PI) };
	}

	if (pointInRect(leftBottom, clipWindow) &&
		pointInRect(rightBottom, clipWindow) &&
		pointInRect(rightTop, clipWindow) &&
		pointInRect(leftTop, clipWindow))
	{
		drawString(pos, str, info, texts);
	}	
}
void clipString_all_or_none_character_clipping(Rect clipWindow, Point pos, const std::string& str, const TextInfo& info, std::map<char, Stencil>& texts)
{
	float charRotate = -1 * PI / 2 + info.upVector;

	Point leftBottom, rightBottom, rightTop, leftTop;
	switch (info.textPath)
	{
	case TextPath::UP:
	{
		leftBottom = pos;
	}
	break;
	case TextPath::DOWN:
	{
		float firstHeight = texts[str[0]].stencil.size();
		leftTop = { pos.x + firstHeight * (float)cos(info.upVector), pos.y + firstHeight * (float)sin(info.upVector) };
	}
	break;
	case TextPath::RIGHT:
	{
		leftBottom = pos;
	}
	break;
	case TextPath::LEFT:
	{
		float firstWidth = texts[str[0]].stencil[0].size();
		rightBottom = { pos.x + firstWidth * (float)cos(info.upVector - PI / 2), pos.y + firstWidth * (float)sin(info.upVector - PI / 2) };
	}
	break;
	default:
	break;
	}

	float width = 0, height = 0;

	for (auto& c : str)
	{
		if (texts.find(c) != texts.end())
		{
			const Stencil& s = texts.find(c)->second;
			width = s.stencil[0].size();
			height = s.stencil.size();

			switch (info.textPath)
			{
			case TextPath::UP:
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				break;
			case TextPath::DOWN:
				leftBottom = { leftTop.x + height * (float)cos(info.upVector + PI), leftTop.y + height * (float)sin(info.upVector + PI) };
				rightTop = { leftTop.x + width * (float)cos(info.upVector - PI / 2), leftTop.y + width * (float)sin(info.upVector - PI / 2) };
				rightBottom = { rightTop.x + height * (float)cos(info.upVector + PI), rightTop.y + height * (float)sin(info.upVector + PI) };
				break;
			case TextPath::RIGHT:
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				break;
			case TextPath::LEFT:
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				leftBottom = { rightBottom.x + width * (float)cos(info.upVector + PI / 2), rightBottom.y + width * (float)sin(info.upVector + PI / 2) };
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				break;
			default:
				break;
			}

			if (pointInRect(leftBottom, clipWindow) &&
				pointInRect(rightBottom, clipWindow) &&
				pointInRect(rightTop, clipWindow) &&
				pointInRect(leftTop, clipWindow))
			{
				drawStencil(leftBottom.x, leftBottom.y, s, charRotate);
			}
			
			switch (info.textPath)
			{
			case TextPath::UP:
				leftBottom = { leftTop.x + info.space * (float)cos(info.upVector), leftTop.y + info.space * (float)sin(info.upVector) };
				break;
			case TextPath::DOWN:
				leftTop = { leftBottom.x + info.space * (float)cos(info.upVector + PI), leftBottom.y + info.space * (float)sin(info.upVector + PI) };
				break;
			case TextPath::RIGHT:
				leftBottom = { rightBottom.x + info.space * (float)cos(info.upVector - PI / 2), rightBottom.y + info.space * (float)sin(info.upVector - PI / 2) };
				break;
			case TextPath::LEFT:
				rightBottom = { leftBottom.x + info.space * (float)cos(info.upVector + PI / 2), leftBottom.y + info.space * (float)sin(info.upVector + PI / 2) };
				break;
			default:
				break;
			}
		}
	}
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Stencil A = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 1, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		}, 0, 0 };
	Stencil B = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil C = {
		{
			{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 },
			{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil D = {
		{
			{ 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil E = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		}, 0, 0 };

	std::map<char, Stencil> texts;
	texts['A'] = A;
	texts['B'] = B;
	texts['C'] = C;
	texts['D'] = D;
	texts['E'] = E;

	Rect clipWindow = { 45, 70, 155, 220 };
	Point pos;
	std::string str = "ABCDE";
	TextInfo info;
	
	// “全部保留或全部舍弃字符串”
	// 右文本路径
	glViewport(0, 300, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = {118, 222}, info = { 2, PI / 4, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 32, 169 }, info = { 2, -PI / 6, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 52, 96 }, info = { 2, 0, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 73, 155 }, info = { 2, PI / 2, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 120, 80 }, info = { 2, PI / 2, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 70, 190 }, info = { 2, PI, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	// 左文本路径
	glViewport(200, 300, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 119, 226 }, info = { 2, PI / 2, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 25, 93 }, info = { 2, -PI / 6, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 131, 157 }, info = { 2, PI / 4, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 126, 126 }, info = { 2, PI / 2, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 104, 98 }, info = { 2, PI, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 148, 99 }, info = { 2, 0, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	// 上文本路径
	glViewport(400, 300, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 54, 240 }, info = { 2, 0, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 123, 137 }, info = { 2, PI / 2, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 120, 80 }, info = { 2, PI, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 55, 104 }, info = { 2, PI / 4, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 20, 185 }, info = { 2, -PI / 6, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 150, 46 }, info = { 2, PI / 2, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	// 下文本路径
	glViewport(600, 300, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 54, 147 }, info = { 2, PI / 4, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 75, 240 }, info = { 2, PI / 2, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 105, 192 }, info = { 2, PI / 2, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 182, 146 }, info = { 2, -PI / 6, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 124, 95 }, info = { 2, 0, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 76, 66 }, info = { 2, PI, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_string_clipping(clipWindow, pos, str, info, texts);

	// “全部保留或全部舍弃字符”
	// 右文本路径
	glViewport(0, 0, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 118, 222 }, info = { 2, PI / 4, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 32, 169 }, info = { 2, -PI / 6, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 52, 96 }, info = { 2, 0, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 73, 155 }, info = { 2, PI / 2, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 120, 80 }, info = { 2, PI / 2, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 70, 190 }, info = { 2, PI, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	// 左文本路径
	glViewport(200, 0, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 119, 226 }, info = { 2, PI / 2, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 25, 93 }, info = { 2, -PI / 6, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 131, 157 }, info = { 2, PI / 4, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 126, 126 }, info = { 2, PI / 2, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 104, 98 }, info = { 2, PI, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 148, 99 }, info = { 2, 0, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	// 上文本路径
	glViewport(400, 0, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 54, 240 }, info = { 2, 0, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 123, 137 }, info = { 2, PI / 2, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 120, 80 }, info = { 2, PI, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 55, 104 }, info = { 2, PI / 4, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 20, 185 }, info = { 2, -PI / 6, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 150, 46 }, info = { 2, PI / 2, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	// 下文本路径
	glViewport(600, 0, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 54, 147 }, info = { 2, PI / 4, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 75, 240 }, info = { 2, PI / 2, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 105, 192 }, info = { 2, PI / 2, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 182, 146 }, info = { 2, -PI / 6, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 124, 95 }, info = { 2, 0, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 76, 66 }, info = { 2, PI, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_all_or_none_character_clipping(clipWindow, pos, str, info, texts);

	glFlush();
}
void code_8_exercise_21()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 200, 0, 300);

	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_22
float FPS = 60;
struct Point { float x; float y; };
typedef Point Vec;
enum class TextPath
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct TextInfo
{
	int space;
	double upVector;
	TextPath textPath;
};
struct Text
{
	std::string str;
	Point pos;
	TextInfo info;
};
struct MoveConfig
{
	std::vector<Text> texts;
	Vec speed;
	float moveTime;
};
struct Rect
{
	float minX;
	float minY;
	float maxX;
	float maxY;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void drawStencil(float x, float y, const Stencil& s, float rotate)
{
	float sin = std::sin(rotate);
	float cos = std::cos(rotate);
	for (int i = 0; i < s.stencil.size(); i++)
	{
		for (int j = 0; j < s.stencil[i].size(); j++)
		{
			if (s.stencil[i][j] == 1)
			{
				int curX = j;
				int curY = s.stencil.size() - 1 - i;

				setPixel(Round(x + curX * cos - curY * sin), Round(y + curY * cos + curX * sin));
			}
		}
	}
}
void drawString(Point pos, const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	float posX = pos.x;
	float posY = pos.y;
	float charRotate = -1 * PI / 2 + info.upVector;
	for (int i = 0; i < str.size(); i++)
	{
		if (texts.find(str[i]) != texts.end())
		{
			const Stencil& s = texts.find(str[i])->second;
			drawStencil(posX, posY, s, charRotate);
			switch (info.textPath)
			{
			case TextPath::UP:
				posX += (s.stencil.size() + info.space) * std::cos(info.upVector);
				posY += (s.stencil.size() + info.space) * std::sin(info.upVector);
				break;
			case TextPath::DOWN:
				if (i + 1 < str.size())
				{
					const Stencil& nextS = texts.find(str[i + 1])->second;
					posX += (nextS.stencil.size() + info.space) * std::cos(info.upVector + PI);
					posY += (nextS.stencil.size() + info.space) * std::sin(info.upVector + PI);
				}
				break;
			case TextPath::LEFT:
				if (i + 1 < str.size())
				{
					const Stencil& nextS = texts.find(str[i + 1])->second;
					posX += (nextS.stencil[0].size() + info.space) * std::cos(info.upVector + PI / 2);
					posY += (nextS.stencil[0].size() + info.space) * std::sin(info.upVector + PI / 2);
				}
				break;
			case TextPath::RIGHT:
				posX += (s.stencil[0].size() + info.space) * std::cos(info.upVector - PI / 2);
				posY += (s.stencil[0].size() + info.space) * std::sin(info.upVector - PI / 2);
				break;
			default:
				break;
			}
		}
	}
}
bool pointInRect(Point p, Rect rect)
{
	return p.x >= rect.minX && p.x <= rect.maxX && p.y >= rect.minY && p.y <= rect.maxY;
}
void clipString_all_or_none_string_clipping(Rect clipWindow, Point pos, const std::string& str, const TextInfo& info, std::map<char, Stencil>& texts)
{
	Point leftBottom, rightBottom, rightTop, leftTop;

	float width = 0, height = 0;

	if (info.textPath == TextPath::RIGHT)
	{
		for (auto& c : str)
		{
			if (texts.find(c) != texts.end())
			{
				const Stencil& s = texts.find(c)->second;

				width += s.stencil[0].size();
				height = std::max(height, (float)s.stencil.size());
			}
		}
		width += (str.size() - 1) * info.space;

		leftBottom = pos;
		leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
		rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
		rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
	}
	else if (info.textPath == TextPath::LEFT)
	{
		for (auto& c : str)
		{
			if (texts.find(c) != texts.end())
			{
				const Stencil& s = texts.find(c)->second;

				width += s.stencil[0].size();
				height = std::max(height, (float)s.stencil.size());
			}
		}
		width += (str.size() - 1) * info.space;

		float firstWidth = texts[str[0]].stencil[0].size();
		rightBottom = { pos.x + firstWidth * (float)cos(info.upVector - PI / 2), pos.y + firstWidth * (float)sin(info.upVector - PI / 2) };
		rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
		leftBottom = { rightBottom.x + width * (float)cos(info.upVector + PI / 2), rightBottom.y + width * (float)sin(info.upVector + PI / 2) };
		leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
	}
	else if (info.textPath == TextPath::UP)
	{
		for (auto& c : str)
		{
			if (texts.find(c) != texts.end())
			{
				const Stencil& s = texts.find(c)->second;

				height += s.stencil.size();
				width = std::max(width, (float)s.stencil[0].size());
			}
		}
		height += (str.size() - 1) * info.space;

		leftBottom = pos;
		leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
		rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
		rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
	}
	else if (info.textPath == TextPath::DOWN)
	{
		for (auto& c : str)
		{
			if (texts.find(c) != texts.end())
			{
				const Stencil& s = texts.find(c)->second;

				height += s.stencil.size();
				width = std::max(width, (float)s.stencil[0].size());
			}
		}
		height += (str.size() - 1) * info.space;

		float firstHeight = texts[str[0]].stencil.size();
		leftTop = { pos.x + firstHeight * (float)cos(info.upVector), pos.y + firstHeight * (float)sin(info.upVector) };
		leftBottom = { leftTop.x + height * (float)cos(info.upVector + PI), leftTop.y + height * (float)sin(info.upVector + PI) };
		rightTop = { leftTop.x + width * (float)cos(info.upVector - PI / 2), leftTop.y + width * (float)sin(info.upVector - PI / 2) };
		rightBottom = { rightTop.x + height * (float)cos(info.upVector + PI), rightTop.y + height * (float)sin(info.upVector + PI) };
	}

	if (pointInRect(leftBottom, clipWindow) &&
		pointInRect(rightBottom, clipWindow) &&
		pointInRect(rightTop, clipWindow) &&
		pointInRect(leftTop, clipWindow))
	{
		drawString(pos, str, info, texts);
	}
}
void clipString_all_or_none_character_clipping(Rect clipWindow, Point pos, const std::string& str, const TextInfo& info, std::map<char, Stencil>& texts)
{
	float charRotate = -1 * PI / 2 + info.upVector;

	Point leftBottom, rightBottom, rightTop, leftTop;
	switch (info.textPath)
	{
	case TextPath::UP:
	{
		leftBottom = pos;
	}
	break;
	case TextPath::DOWN:
	{
		float firstHeight = texts[str[0]].stencil.size();
		leftTop = { pos.x + firstHeight * (float)cos(info.upVector), pos.y + firstHeight * (float)sin(info.upVector) };
	}
	break;
	case TextPath::RIGHT:
	{
		leftBottom = pos;
	}
	break;
	case TextPath::LEFT:
	{
		float firstWidth = texts[str[0]].stencil[0].size();
		rightBottom = { pos.x + firstWidth * (float)cos(info.upVector - PI / 2), pos.y + firstWidth * (float)sin(info.upVector - PI / 2) };
	}
	break;
	default:
		break;
	}

	float width = 0, height = 0;

	for (auto& c : str)
	{
		if (texts.find(c) != texts.end())
		{
			const Stencil& s = texts.find(c)->second;
			width = s.stencil[0].size();
			height = s.stencil.size();

			switch (info.textPath)
			{
			case TextPath::UP:
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				break;
			case TextPath::DOWN:
				leftBottom = { leftTop.x + height * (float)cos(info.upVector + PI), leftTop.y + height * (float)sin(info.upVector + PI) };
				rightTop = { leftTop.x + width * (float)cos(info.upVector - PI / 2), leftTop.y + width * (float)sin(info.upVector - PI / 2) };
				rightBottom = { rightTop.x + height * (float)cos(info.upVector + PI), rightTop.y + height * (float)sin(info.upVector + PI) };
				break;
			case TextPath::RIGHT:
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				break;
			case TextPath::LEFT:
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				leftBottom = { rightBottom.x + width * (float)cos(info.upVector + PI / 2), rightBottom.y + width * (float)sin(info.upVector + PI / 2) };
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				break;
			default:
				break;
			}

			if (pointInRect(leftBottom, clipWindow) &&
				pointInRect(rightBottom, clipWindow) &&
				pointInRect(rightTop, clipWindow) &&
				pointInRect(leftTop, clipWindow))
			{
				drawStencil(leftBottom.x, leftBottom.y, s, charRotate);
			}

			switch (info.textPath)
			{
			case TextPath::UP:
				leftBottom = { leftTop.x + info.space * (float)cos(info.upVector), leftTop.y + info.space * (float)sin(info.upVector) };
				break;
			case TextPath::DOWN:
				leftTop = { leftBottom.x + info.space * (float)cos(info.upVector + PI), leftBottom.y + info.space * (float)sin(info.upVector + PI) };
				break;
			case TextPath::RIGHT:
				leftBottom = { rightBottom.x + info.space * (float)cos(info.upVector - PI / 2), rightBottom.y + info.space * (float)sin(info.upVector - PI / 2) };
				break;
			case TextPath::LEFT:
				rightBottom = { leftBottom.x + info.space * (float)cos(info.upVector + PI / 2), leftBottom.y + info.space * (float)sin(info.upVector + PI / 2) };
				break;
			default:
				break;
			}
		}
	}
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
int lastTick = 0;
float delta = 0.f;
std::vector<MoveConfig> moveConfig =
{
	{{
		{ "ABCDE",{ -10, 440 },{ 2, 0, TextPath::RIGHT } },
		{ "ABCDE",{ -21, 385 },{ 2, -PI / 6, TextPath::RIGHT } },
		{ "ABCDE",{ -62, 329 },{ 2, PI / 4, TextPath::RIGHT } },
		{ "ABCDE",{ -62, 270 },{ 2, PI / 2, TextPath::RIGHT } },
		{ "ABCDE",{ -30, 208 },{ 2, PI, TextPath::RIGHT } },
		}, {30, 0}, (float)480 / 30},

	{ {
		{ "ABCDE",{ 52, 103 },{ 2, -PI / 6, TextPath::UP } },
		{ "ABCDE",{ 145, 55 },{ 2, PI / 4, TextPath::UP } },
		{ "ABCDE",{ 220, 38 },{ 2, PI / 2, TextPath::UP } },
		{ "ABCDE",{ 310, 70 },{ 2, PI, TextPath::UP } },
		{ "ABCDE",{ 281, 46 },{ 2, 0, TextPath::UP } },
		},{ 0, 30 }, (float)480 / 30 },

	{ {
		{ "ABCDE",{ 453, 139 },{ 2, PI / 4, TextPath::LEFT } },
		{ "ABCDE",{ 458, 220 },{ 2, PI / 2, TextPath::LEFT } },
		{ "ABCDE",{ 415, 281 },{ 2, 0, TextPath::LEFT } },
		{ "ABCDE",{ 455, 357 },{ 2, PI, TextPath::LEFT } },
		{ "ABCDE",{ 417, 395 },{ 2, -PI / 6, TextPath::LEFT } },
		},{ -30, 0 }, (float)480 / 30 },

	{ {
		{ "ABCDE",{ 313, 567 },{ 2, PI / 2, TextPath::DOWN } },
		{ "ABCDE",{ 298, 514 },{ 2, -PI / 6, TextPath::DOWN } },
		{ "ABCDE",{ 215, 545 },{ 2, 0, TextPath::DOWN } },
		{ "ABCDE",{ 123, 509 },{ 2, PI, TextPath::DOWN } },
		{ "ABCDE",{ 111, 550 },{ 2, PI / 4, TextPath::DOWN } },
		},{ 0, -30 }, (float)480 / 30 },
};
int curConfigIdx = 0;
float moveTime = 0.f;
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Stencil A = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 1, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		}, 0, 0 };
	Stencil B = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil C = {
		{
			{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 },
			{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil D = {
		{
			{ 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil E = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		}, 0, 0 };

	std::map<char, Stencil> texts;
	texts['A'] = A;
	texts['B'] = B;
	texts['C'] = C;
	texts['D'] = D;
	texts['E'] = E;

	Rect clipWindow = { 80, 200, 320, 420 };
	Point pos;
	moveTime += delta;

	if (moveTime > moveConfig[curConfigIdx].moveTime)
	{
		moveTime = 0;
		curConfigIdx = (++curConfigIdx) % moveConfig.size();
	}

	// “全部保留或全部舍弃字符串”
	glViewport(0, 120, 400, 380);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });
	
	for (auto& t : moveConfig[curConfigIdx].texts)
	{
		glColor3f(1.f, 1.f, 1.f);
		pos = { t.pos.x + moveTime * moveConfig[curConfigIdx].speed.x, t.pos.y + moveTime * moveConfig[curConfigIdx].speed.y };
		drawString(pos, t.str, t.info, texts);
		glColor3f(1.f, 0.f, 0.f);
		clipString_all_or_none_string_clipping(clipWindow, pos, t.str, t.info, texts);
	}

	// “全部保留或全部舍弃字符”
	glViewport(400, 120, 400, 380);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	for (auto& t : moveConfig[curConfigIdx].texts)
	{
		glColor3f(1.f, 1.f, 1.f);
		pos = { t.pos.x + moveTime * moveConfig[curConfigIdx].speed.x, t.pos.y + moveTime * moveConfig[curConfigIdx].speed.y };
		drawString(pos, t.str, t.info, texts);
		glColor3f(1.f, 0.f, 0.f);
		clipString_all_or_none_character_clipping(clipWindow, pos, t.str, t.info, texts);
	}
	
	glFlush();
}
void onTimer(int id)
{
	int curTick = GetTickCount();
	delta = (curTick - lastTick) / (float)1000;
	lastTick = curTick;
	glutPostRedisplay();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
void code_8_exercise_22()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 400, 120, 500);

	glutDisplayFunc(drawFunc);
	lastTick = GetTickCount();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
#endif

#ifdef CHAPTER_8_EXERCISE_23
struct Point { float x; float y; };
struct Rect
{
	float minX;
	float minY;
	float maxX;
	float maxY;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
enum class TextPath
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct TextInfo
{
	int space;
	double upVector;
	TextPath textPath;
};
enum LocationType
{
	Unkown,  // 不确定
	Outer,  // 完全在外部
	Inner,  // 完全在内部
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void drawStencil(float x, float y, const Stencil& s, float rotate, bool check, const Rect& clipWindow)
{
	float sin = std::sin(rotate);
	float cos = std::cos(rotate);
	for (int i = 0; i < s.stencil.size(); i++)
	{
		for (int j = 0; j < s.stencil[i].size(); j++)
		{
			if (s.stencil[i][j] == 1)
			{
				int curX = j;
				int curY = s.stencil.size() - 1 - i;

				int realX = Round(x + curX * cos - curY * sin);
				int realY = Round(y + curY * cos + curX * sin);

				bool draw = true;
				if (check)
				{
					draw = realX > (int)clipWindow.minX &&
						realX < (int)clipWindow.maxX &&
						realY >(int)clipWindow.minY &&
						realY < (int)clipWindow.maxY;
				}

				if (draw)
				{
					setPixel(realX, realY);
				}					
			}
		}
	}
}
void drawString(Point pos, const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	float posX = pos.x;
	float posY = pos.y;
	float charRotate = -1 * PI / 2 + info.upVector;
	for (int i = 0; i < str.size(); i++)
	{
		if (texts.find(str[i]) != texts.end())
		{
			const Stencil& s = texts.find(str[i])->second;
			drawStencil(posX, posY, s, charRotate, false, {});
			switch (info.textPath)
			{
			case TextPath::UP:
				posX += (s.stencil.size() + info.space) * std::cos(info.upVector);
				posY += (s.stencil.size() + info.space) * std::sin(info.upVector);
				break;
			case TextPath::DOWN:
				if (i + 1 < str.size())
				{
					const Stencil& nextS = texts.find(str[i + 1])->second;
					posX += (nextS.stencil.size() + info.space) * std::cos(info.upVector + PI);
					posY += (nextS.stencil.size() + info.space) * std::sin(info.upVector + PI);
				}
				break;
			case TextPath::LEFT:
				if (i + 1 < str.size())
				{
					const Stencil& nextS = texts.find(str[i + 1])->second;
					posX += (nextS.stencil[0].size() + info.space) * std::cos(info.upVector + PI / 2);
					posY += (nextS.stencil[0].size() + info.space) * std::sin(info.upVector + PI / 2);
				}
				break;
			case TextPath::RIGHT:
				posX += (s.stencil[0].size() + info.space) * std::cos(info.upVector - PI / 2);
				posY += (s.stencil[0].size() + info.space) * std::sin(info.upVector - PI / 2);
				break;
			default:
				break;
			}
		}
	}
}
int encode(Point pt, Rect clipWindow)
{
	static GLint winLeftBitCode = 0x01;
	static GLint winRightBitCode = 0x02;
	static GLint winBottomBitCode = 0x04;
	static GLint winTopBitCode = 0x08;

	int code = 0x00;
	if (pt.x < clipWindow.minX)
		code = code | winLeftBitCode;
	if (pt.x > clipWindow.maxX)
		code = code | winRightBitCode;
	if (pt.y < clipWindow.minY)
		code = code | winBottomBitCode;
	if (pt.y > clipWindow.maxY)
		code = code | winTopBitCode;
	return code;
}
LocationType checkLocation(Rect clipWindow, const std::vector<Point>& polygon)
{
	std::vector<int> codes;
	for (auto& p : polygon)
	{
		codes.push_back(encode(p, clipWindow));
	}

	int check = 0x00;
	for (auto& c : codes)
	{
		check = check | c;
	}
	if (!check)
		return LocationType::Inner;

	check = 0x0F;
	for (auto& c : codes)
	{
		check = check & c;
	}
	if (check)
		return LocationType::Outer;

	return LocationType::Unkown;

}
void clipString_single_character(Rect clipWindow, Point pos, const std::string& str, const TextInfo& info, std::map<char, Stencil>& texts)
{
	float charRotate = -1 * PI / 2 + info.upVector;

	Point leftBottom, rightBottom, rightTop, leftTop;
	switch (info.textPath)
	{
	case TextPath::UP:
	{
		leftBottom = pos;
	}
	break;
	case TextPath::DOWN:
	{
		float firstHeight = texts[str[0]].stencil.size();
		leftTop = { pos.x + firstHeight * (float)cos(info.upVector), pos.y + firstHeight * (float)sin(info.upVector) };
	}
	break;
	case TextPath::RIGHT:
	{
		leftBottom = pos;
	}
	break;
	case TextPath::LEFT:
	{
		float firstWidth = texts[str[0]].stencil[0].size();
		rightBottom = { pos.x + firstWidth * (float)cos(info.upVector - PI / 2), pos.y + firstWidth * (float)sin(info.upVector - PI / 2) };
	}
	break;
	default:
		break;
	}

	float width = 0, height = 0;

	for (auto& c : str)
	{
		if (texts.find(c) != texts.end())
		{
			const Stencil& s = texts.find(c)->second;
			width = s.stencil[0].size();
			height = s.stencil.size();

			switch (info.textPath)
			{
			case TextPath::UP:
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				break;
			case TextPath::DOWN:
				leftBottom = { leftTop.x + height * (float)cos(info.upVector + PI), leftTop.y + height * (float)sin(info.upVector + PI) };
				rightTop = { leftTop.x + width * (float)cos(info.upVector - PI / 2), leftTop.y + width * (float)sin(info.upVector - PI / 2) };
				rightBottom = { rightTop.x + height * (float)cos(info.upVector + PI), rightTop.y + height * (float)sin(info.upVector + PI) };
				break;
			case TextPath::RIGHT:
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				break;
			case TextPath::LEFT:
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				leftBottom = { rightBottom.x + width * (float)cos(info.upVector + PI / 2), rightBottom.y + width * (float)sin(info.upVector + PI / 2) };
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				break;
			default:
				break;
			}

			switch (checkLocation(clipWindow, { leftBottom, rightBottom, rightTop, leftTop }))
			{
			case LocationType::Outer:
				break;
			case LocationType::Inner:
				drawStencil(leftBottom.x, leftBottom.y, s, charRotate, false, {});
				break;
			case LocationType::Unkown:
				drawStencil(leftBottom.x, leftBottom.y, s, charRotate, true, clipWindow);
				break;
			default:
				break;
			}

			switch (info.textPath)
			{
			case TextPath::UP:
				leftBottom = { leftTop.x + info.space * (float)cos(info.upVector), leftTop.y + info.space * (float)sin(info.upVector) };
				break;
			case TextPath::DOWN:
				leftTop = { leftBottom.x + info.space * (float)cos(info.upVector + PI), leftBottom.y + info.space * (float)sin(info.upVector + PI) };
				break;
			case TextPath::RIGHT:
				leftBottom = { rightBottom.x + info.space * (float)cos(info.upVector - PI / 2), rightBottom.y + info.space * (float)sin(info.upVector - PI / 2) };
				break;
			case TextPath::LEFT:
				rightBottom = { leftBottom.x + info.space * (float)cos(info.upVector + PI / 2), leftBottom.y + info.space * (float)sin(info.upVector + PI / 2) };
				break;
			default:
				break;
			}
		}
	}
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Stencil A = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 1, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		}, 0, 0 };
	Stencil B = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil C = {
		{
			{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 },
			{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil D = {
		{
			{ 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil E = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		}, 0, 0 };

	std::map<char, Stencil> texts;
	texts['A'] = A;
	texts['B'] = B;
	texts['C'] = C;
	texts['D'] = D;
	texts['E'] = E;

	Rect clipWindow = { 45, 70, 155, 220 };
	Point pos;
	std::string str = "ABCDE";
	TextInfo info;

	// 右文本路径
	glViewport(0, 150, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 118, 222 }, info = { 2, PI / 4, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 32, 169 }, info = { 2, -PI / 6, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 52, 96 }, info = { 2, 0, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 73, 155 }, info = { 2, PI / 2, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 120, 80 }, info = { 2, PI / 2, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 70, 190 }, info = { 2, PI, TextPath::RIGHT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	// 左文本路径
	glViewport(200, 150, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 119, 226 }, info = { 2, PI / 2, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 25, 93 }, info = { 2, -PI / 6, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 131, 157 }, info = { 2, PI / 4, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 126, 126 }, info = { 2, PI / 2, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 104, 98 }, info = { 2, PI, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 148, 99 }, info = { 2, 0, TextPath::LEFT };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	// 上文本路径
	glViewport(400, 150, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 54, 240 }, info = { 2, 0, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 123, 137 }, info = { 2, PI / 2, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 120, 80 }, info = { 2, PI, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 55, 104 }, info = { 2, PI / 4, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 20, 185 }, info = { 2, -PI / 6, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 150, 46 }, info = { 2, PI / 2, TextPath::UP };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	// 下文本路径
	glViewport(600, 150, 200, 300);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	glColor3f(1.f, 1.f, 1.f);
	pos = { 54, 147 }, info = { 2, PI / 4, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 75, 240 }, info = { 2, PI / 2, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 105, 192 }, info = { 2, PI / 2, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 182, 146 }, info = { 2, -PI / 6, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 124, 95 }, info = { 2, 0, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glColor3f(1.f, 1.f, 1.f);
	pos = { 76, 66 }, info = { 2, PI, TextPath::DOWN };
	drawString(pos, str, info, texts);
	glColor3f(1.f, 0.f, 0.f);
	clipString_single_character(clipWindow, pos, str, info, texts);

	glFlush();
}
void code_8_exercise_23()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 200, 0, 300);

	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_24
float FPS = 60;
struct Point { float x; float y; };
typedef Point Vec;
enum class TextPath
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct TextInfo
{
	int space;
	double upVector;
	TextPath textPath;
};
struct Text
{
	std::string str;
	Point pos;
	TextInfo info;
};
struct MoveConfig
{
	std::vector<Text> texts;
	Vec speed;
	float moveTime;
};
struct Rect
{
	float minX;
	float minY;
	float maxX;
	float maxY;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
enum LocationType
{
	Unkown,  // 不确定
	Outer,  // 完全在外部
	Inner,  // 完全在内部
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void drawStencil(float x, float y, const Stencil& s, float rotate, bool check, const Rect& clipWindow)
{
	float sin = std::sin(rotate);
	float cos = std::cos(rotate);
	for (int i = 0; i < s.stencil.size(); i++)
	{
		for (int j = 0; j < s.stencil[i].size(); j++)
		{
			if (s.stencil[i][j] == 1)
			{
				int curX = j;
				int curY = s.stencil.size() - 1 - i;

				int realX = Round(x + curX * cos - curY * sin);
				int realY = Round(y + curY * cos + curX * sin);

				bool draw = true;
				if (check)
				{
					draw = realX > (int)clipWindow.minX &&
						realX < (int)clipWindow.maxX &&
						realY >(int)clipWindow.minY &&
						realY < (int)clipWindow.maxY;
				}

				if (draw)
				{
					setPixel(realX, realY);
				}
			}
		}
	}
}
void drawString(Point pos, const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	float posX = pos.x;
	float posY = pos.y;
	float charRotate = -1 * PI / 2 + info.upVector;
	for (int i = 0; i < str.size(); i++)
	{
		if (texts.find(str[i]) != texts.end())
		{
			const Stencil& s = texts.find(str[i])->second;
			drawStencil(posX, posY, s, charRotate, false, {});
			switch (info.textPath)
			{
			case TextPath::UP:
				posX += (s.stencil.size() + info.space) * std::cos(info.upVector);
				posY += (s.stencil.size() + info.space) * std::sin(info.upVector);
				break;
			case TextPath::DOWN:
				if (i + 1 < str.size())
				{
					const Stencil& nextS = texts.find(str[i + 1])->second;
					posX += (nextS.stencil.size() + info.space) * std::cos(info.upVector + PI);
					posY += (nextS.stencil.size() + info.space) * std::sin(info.upVector + PI);
				}
				break;
			case TextPath::LEFT:
				if (i + 1 < str.size())
				{
					const Stencil& nextS = texts.find(str[i + 1])->second;
					posX += (nextS.stencil[0].size() + info.space) * std::cos(info.upVector + PI / 2);
					posY += (nextS.stencil[0].size() + info.space) * std::sin(info.upVector + PI / 2);
				}
				break;
			case TextPath::RIGHT:
				posX += (s.stencil[0].size() + info.space) * std::cos(info.upVector - PI / 2);
				posY += (s.stencil[0].size() + info.space) * std::sin(info.upVector - PI / 2);
				break;
			default:
				break;
			}
		}
	}
}
int encode(Point pt, Rect clipWindow)
{
	static GLint winLeftBitCode = 0x01;
	static GLint winRightBitCode = 0x02;
	static GLint winBottomBitCode = 0x04;
	static GLint winTopBitCode = 0x08;

	int code = 0x00;
	if (pt.x < clipWindow.minX)
		code = code | winLeftBitCode;
	if (pt.x > clipWindow.maxX)
		code = code | winRightBitCode;
	if (pt.y < clipWindow.minY)
		code = code | winBottomBitCode;
	if (pt.y > clipWindow.maxY)
		code = code | winTopBitCode;
	return code;
}
LocationType checkLocation(Rect clipWindow, const std::vector<Point>& polygon)
{
	std::vector<int> codes;
	for (auto& p : polygon)
	{
		codes.push_back(encode(p, clipWindow));
	}

	int check = 0x00;
	for (auto& c : codes)
	{
		check = check | c;
	}
	if (!check)
		return LocationType::Inner;

	check = 0x0F;
	for (auto& c : codes)
	{
		check = check & c;
	}
	if (check)
		return LocationType::Outer;

	return LocationType::Unkown;

}
void clipString_single_character(Rect clipWindow, Point pos, const std::string& str, const TextInfo& info, std::map<char, Stencil>& texts)
{
	float charRotate = -1 * PI / 2 + info.upVector;

	Point leftBottom, rightBottom, rightTop, leftTop;
	switch (info.textPath)
	{
	case TextPath::UP:
	{
		leftBottom = pos;
	}
	break;
	case TextPath::DOWN:
	{
		float firstHeight = texts[str[0]].stencil.size();
		leftTop = { pos.x + firstHeight * (float)cos(info.upVector), pos.y + firstHeight * (float)sin(info.upVector) };
	}
	break;
	case TextPath::RIGHT:
	{
		leftBottom = pos;
	}
	break;
	case TextPath::LEFT:
	{
		float firstWidth = texts[str[0]].stencil[0].size();
		rightBottom = { pos.x + firstWidth * (float)cos(info.upVector - PI / 2), pos.y + firstWidth * (float)sin(info.upVector - PI / 2) };
	}
	break;
	default:
		break;
	}

	float width = 0, height = 0;

	for (auto& c : str)
	{
		if (texts.find(c) != texts.end())
		{
			const Stencil& s = texts.find(c)->second;
			width = s.stencil[0].size();
			height = s.stencil.size();

			switch (info.textPath)
			{
			case TextPath::UP:
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				break;
			case TextPath::DOWN:
				leftBottom = { leftTop.x + height * (float)cos(info.upVector + PI), leftTop.y + height * (float)sin(info.upVector + PI) };
				rightTop = { leftTop.x + width * (float)cos(info.upVector - PI / 2), leftTop.y + width * (float)sin(info.upVector - PI / 2) };
				rightBottom = { rightTop.x + height * (float)cos(info.upVector + PI), rightTop.y + height * (float)sin(info.upVector + PI) };
				break;
			case TextPath::RIGHT:
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				rightBottom = { leftBottom.x + width * (float)cos(info.upVector - PI / 2), leftBottom.y + width * (float)sin(info.upVector - PI / 2) };
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				break;
			case TextPath::LEFT:
				rightTop = { rightBottom.x + height * (float)cos(info.upVector), rightBottom.y + height * (float)sin(info.upVector) };
				leftBottom = { rightBottom.x + width * (float)cos(info.upVector + PI / 2), rightBottom.y + width * (float)sin(info.upVector + PI / 2) };
				leftTop = { leftBottom.x + height * (float)cos(info.upVector), leftBottom.y + height * (float)sin(info.upVector) };
				break;
			default:
				break;
			}

			switch (checkLocation(clipWindow, { leftBottom, rightBottom, rightTop, leftTop }))
			{
			case LocationType::Outer:
				break;
			case LocationType::Inner:
				drawStencil(leftBottom.x, leftBottom.y, s, charRotate, false, {});
				break;
			case LocationType::Unkown:
				drawStencil(leftBottom.x, leftBottom.y, s, charRotate, true, clipWindow);
				break;
			default:
				break;
			}

			switch (info.textPath)
			{
			case TextPath::UP:
				leftBottom = { leftTop.x + info.space * (float)cos(info.upVector), leftTop.y + info.space * (float)sin(info.upVector) };
				break;
			case TextPath::DOWN:
				leftTop = { leftBottom.x + info.space * (float)cos(info.upVector + PI), leftBottom.y + info.space * (float)sin(info.upVector + PI) };
				break;
			case TextPath::RIGHT:
				leftBottom = { rightBottom.x + info.space * (float)cos(info.upVector - PI / 2), rightBottom.y + info.space * (float)sin(info.upVector - PI / 2) };
				break;
			case TextPath::LEFT:
				rightBottom = { leftBottom.x + info.space * (float)cos(info.upVector + PI / 2), leftBottom.y + info.space * (float)sin(info.upVector + PI / 2) };
				break;
			default:
				break;
			}
		}
	}
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
int lastTick = 0;
float delta = 0.f;
std::vector<MoveConfig> moveConfig =
{
	{ {
		{ "ABCDE",{ -10, 440 },{ 2, 0, TextPath::RIGHT } },
		{ "ABCDE",{ -21, 385 },{ 2, -PI / 6, TextPath::RIGHT } },
		{ "ABCDE",{ -62, 329 },{ 2, PI / 4, TextPath::RIGHT } },
		{ "ABCDE",{ -62, 270 },{ 2, PI / 2, TextPath::RIGHT } },
		{ "ABCDE",{ -30, 208 },{ 2, PI, TextPath::RIGHT } },
		},{ 30, 0 }, (float)480 / 30 },

		{ {
			{ "ABCDE",{ 52, 103 },{ 2, -PI / 6, TextPath::UP } },
			{ "ABCDE",{ 145, 55 },{ 2, PI / 4, TextPath::UP } },
			{ "ABCDE",{ 220, 38 },{ 2, PI / 2, TextPath::UP } },
			{ "ABCDE",{ 310, 70 },{ 2, PI, TextPath::UP } },
			{ "ABCDE",{ 281, 46 },{ 2, 0, TextPath::UP } },
			},{ 0, 30 }, (float)480 / 30 },

			{ {
				{ "ABCDE",{ 453, 139 },{ 2, PI / 4, TextPath::LEFT } },
				{ "ABCDE",{ 458, 220 },{ 2, PI / 2, TextPath::LEFT } },
				{ "ABCDE",{ 415, 281 },{ 2, 0, TextPath::LEFT } },
				{ "ABCDE",{ 455, 357 },{ 2, PI, TextPath::LEFT } },
				{ "ABCDE",{ 417, 395 },{ 2, -PI / 6, TextPath::LEFT } },
				},{ -30, 0 }, (float)480 / 30 },

				{ {
					{ "ABCDE",{ 313, 567 },{ 2, PI / 2, TextPath::DOWN } },
					{ "ABCDE",{ 298, 514 },{ 2, -PI / 6, TextPath::DOWN } },
					{ "ABCDE",{ 215, 545 },{ 2, 0, TextPath::DOWN } },
					{ "ABCDE",{ 123, 509 },{ 2, PI, TextPath::DOWN } },
					{ "ABCDE",{ 111, 550 },{ 2, PI / 4, TextPath::DOWN } },
					},{ 0, -30 }, (float)480 / 30 },
};
int curConfigIdx = 0;
float moveTime = 0.f;
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Stencil A = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 1, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 1, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		}, 0, 0 };
	Stencil B = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil C = {
		{
			{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 },
			{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil D = {
		{
			{ 1, 1, 1, 1, 1, 0, 0 },
			{ 1, 0, 0, 0, 0, 1, 0 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 0, 0 },
		}, 0, 0 };
	Stencil E = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		}, 0, 0 };

	std::map<char, Stencil> texts;
	texts['A'] = A;
	texts['B'] = B;
	texts['C'] = C;
	texts['D'] = D;
	texts['E'] = E;

	Rect clipWindow = { 80, 200, 320, 420 };
	Point pos;
	moveTime += delta;

	if (moveTime > moveConfig[curConfigIdx].moveTime)
	{
		moveTime = 0;
		curConfigIdx = (++curConfigIdx) % moveConfig.size();
	}

	glViewport(200, 120, 400, 380);
	glColor3f(1.f, 1.f, 1.f);
	drawPolygonLine({ { clipWindow.minX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.minY },
	{ clipWindow.maxX, clipWindow.maxY } ,
	{ clipWindow.minX, clipWindow.maxY } });

	for (auto& t : moveConfig[curConfigIdx].texts)
	{
		glColor3f(1.f, 1.f, 1.f);
		pos = { t.pos.x + moveTime * moveConfig[curConfigIdx].speed.x, t.pos.y + moveTime * moveConfig[curConfigIdx].speed.y };
		drawString(pos, t.str, t.info, texts);
		glColor3f(1.f, 0.f, 0.f);
		clipString_single_character(clipWindow, pos, t.str, t.info, texts);
	}

	glFlush();
}
void onTimer(int id)
{
	int curTick = GetTickCount();
	delta = (curTick - lastTick) / (float)1000;
	lastTick = curTick;
	glutPostRedisplay();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
void code_8_exercise_24()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 400, 120, 500);

	glutDisplayFunc(drawFunc);
	lastTick = GetTickCount();
	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1
struct Point
{
	float x, y;
};
//////////////////////////////////////////////////////////////////////////
// 分割凹多边形
typedef Point Vec;
inline bool operator==(const Point& p1, const Point& p2)
{
	//return p1.x == p2.x && p1.y == p2.y;
	return Equal(p1.x, p2.x) && Equal(p1.y, p2.y);
}
inline bool operator!=(const Point& p1, const Point& p2)
{
	//return p1.x == p2.x && p1.y == p2.y;
	return !(p1 == p2);
}
bool operator < (const Point& p1, const Point& p2)
{
	if (p1.x < p2.x)
	{
		return true;
	}
	else if (p2.x < p1.x)
	{
		return false;
	}
	else
	{
		return p1.y < p2.y;
	}
}
int crossProduct(const Vec& vec1, const Vec& vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}
bool crossPoint(Point lineBegin1, Point lineEnd1, Point lineBegin2, Point lineEnd2, float& u1, float& u2)
{
	float dx1 = lineEnd1.x - lineBegin1.x;
	float dy1 = lineEnd1.y - lineBegin1.y;
	float dx2 = lineEnd2.x - lineBegin2.x;
	float dy2 = lineEnd2.y - lineBegin2.y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if(dx1 && dx2 && Equal(dy1 / dx1, dy2 / dx2))
		return false;

	float x01 = lineBegin1.x;
	float y01 = lineBegin1.y;
	float x02 = lineBegin2.x;
	float y02 = lineBegin2.y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);
	return true;
}
bool sameLine(Point begin, Point middle, Point end)
{
	float dx1 = middle.x - begin.x;
	float dy1 = middle.y - begin.y;
	float dx2 = end.x - middle.x;
	float dy2 = end.y - middle.y;

	if (dx1 == 0 && dx2 == 0)
		return true;

	if (dx1 && dx2 && Equal(dy1 / dx1, dy2 / dx2))
		return true;

	return false;
}
int getRealIndx(const std::vector<Point>& ploygon, int idx)
{
	return (idx + ploygon.size()) % ploygon.size();
}
void cutPolygon(const std::vector<Point>& ploygon, std::vector<std::vector<Point>>& result)
{
	std::vector<Vec> edges;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int next = getRealIndx(ploygon, i + 1);
		edges.push_back({ ploygon[next].x - ploygon[i].x, ploygon[next].y - ploygon[i].y });
	}
	int index = -1;
	for (int i = 0; i < edges.size(); ++i)
	{
		float xp = crossProduct(edges[i], edges[getRealIndx(edges, i + 1)]);
		if (xp < 0)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		result.push_back(ploygon);
		return;
	}

	Point cutStartP = ploygon[index];
	Point cutEndP = ploygon[getRealIndx(ploygon, index + 1)];
	
	float crossU1 = -1.f;
	float crossU2 = -1.f;
	int curIndex = -1;
	for (int i = 0; i < ploygon.size(); i++)
	{
		float u1, u2;
		if (crossPoint(cutStartP, cutEndP, ploygon[i], ploygon[getRealIndx(ploygon, i + 1)], u1, u2))
		{
			if (u1 > 1.f && u2 >= 0.f && u2 <= 1.f)
			{
				if (u1 < crossU1 || crossU1 == -1.f)
				{
					crossU1 = u1;
					crossU2 = u2;
					curIndex = i;
				}
			}
		}
	}

	assert(curIndex >= 0);

	Point cutPoint;
	vector<Point> newPloygon = ploygon;
	if (Equal(crossU2, 0.f))
	{
		cutPoint = ploygon[curIndex];
	}
	else if(Equal(crossU2, 1.f))
	{
		cutPoint = ploygon[getRealIndx(ploygon, curIndex + 1)];
	}
	else
	{
		float x0 = ploygon[curIndex].x;
		float y0 = ploygon[curIndex].y;
		int next = getRealIndx(ploygon, curIndex + 1);
		float xEnd = ploygon[next].x;
		float yEnd = ploygon[next].y;
		cutPoint = {x0 + crossU2 * (xEnd - x0), y0 + crossU2 * (yEnd - y0)};
		newPloygon.insert(newPloygon.begin() + curIndex + 1, cutPoint);
	}
	
	vector<Point> newPloygon1;
	vector<Point> newPloygon2;
	bool f = false;
	for (int i = 0; i < newPloygon.size(); i++)
	{
		if (newPloygon[i] == cutEndP)
		{
			if (!sameLine(newPloygon[getRealIndx(newPloygon, i - 1)], cutEndP, cutPoint))
			{
				if (!f)
				{
					newPloygon1.push_back(newPloygon[i]);
				}
				else
				{
					newPloygon2.push_back(newPloygon[i]);
				}
			}
			f = !f;
			if (!sameLine(cutPoint, cutEndP, newPloygon[getRealIndx(newPloygon, i + 1)]))
			{
				if (!f)
				{
					newPloygon1.push_back(newPloygon[i]);
				}
				else
				{
					newPloygon2.push_back(newPloygon[i]);
				}
			}
		}
		else if (newPloygon[i] == cutPoint)
		{
			if (!sameLine(newPloygon[getRealIndx(newPloygon, i - 1)], cutPoint, cutEndP))
			{
				if (!f)
				{
					newPloygon1.push_back(newPloygon[i]);
				}
				else
				{
					newPloygon2.push_back(newPloygon[i]);
				}
			}
			f = !f;
			if (!sameLine(cutEndP, cutPoint, newPloygon[getRealIndx(newPloygon, i + 1)]))
			{
				if (!f)
				{
					newPloygon1.push_back(newPloygon[i]);
				}
				else
				{
					newPloygon2.push_back(newPloygon[i]);
				}
			}
		}
		else
		{
			if (!f)
			{
				newPloygon1.push_back(newPloygon[i]);
			}
			else
			{
				newPloygon2.push_back(newPloygon[i]);
			}
		}
	}
	cutPolygon(newPloygon1, result);
	cutPolygon(newPloygon2, result);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Sutherlan-Hodgman多边形裁剪算法(凸多边形)
typedef enum { None = -1, Left, Right, Bottom, Top } Boundary;
typedef Point wcPt2D;
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
		first[winEdge] = new wcPt2D{ p.x, p.y };
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
		if (first[winEdge] && cross(s[winEdge], *first[winEdge], winEdge, wMin, wMax))
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
GLint polygonClipSuthHodg1(wcPt2D wMin, wcPt2D wMax, GLint n, wcPt2D* pIn, wcPt2D* pOut)
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
		if(first[i])
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
//////////////////////////////////////////////////////////////////////////

enum class PointType
{
	None,    // 无类型
	Normal,  // 普通点 
	Cross,   // 交点 
};
struct CrossPointInfo
{
	float u1 = 0;
	float u2 = 0;
	float u = 0;
	Point point;       // 交点
};
struct LineWithCross
{
	Point begin;
	Point end;
	std::vector<CrossPointInfo> crossPoints;
};
//////////////////////////////////////////////////////////////////////////
// 检查内外
bool sign(float f)
{
	return f > 0;
}
bool checkRay(LineWithCross& line, const std::vector<Point>& polygon)
{
	float dx = line.end.x - line.begin.x;
	float dy = line.end.y - line.begin.y;
	for (auto& p : polygon)
	{
		bool check = false;
		float _dx = p.x - line.begin.x;
		float _dy = p.y - line.begin.y;
		if (dx)
		{
			if (_dx)
			{
				if (dy / dx != _dy / _dx)
				{
					check = true;
				}
				else
				{
					if (sign(dx) == sign(_dx) && sign(dy) == sign(_dy))
						check = false;
					else
						check = true;
				}
			}
			else
			{
				check = true;
			}
		}
		else
		{
			if (_dx)
			{
				check = true;
			}
			else
			{
				if (sign(dx) == sign(_dx) && sign(dy) == sign(_dy))
					check = false;
				else
					check = true;
			}
		}
		if (!check)
			return false;
	}
	return true;
}
int crossProduct(LineWithCross& line1, LineWithCross& line2)
{
	return (line1.end.x - line1.begin.x) * (line2.end.y - line2.begin.y) - (line1.end.y - line1.begin.y) * (line2.end.x - line2.begin.x);
}
void boundBox(const std::vector<Point>& polygon, float& minX, float& maxX, float& minY, float& maxY)
{
	maxX = polygon[0].x, minX = polygon[0].x, maxY = polygon[0].y, minY = polygon[0].y;
	for (int i = 1; i < polygon.size(); i++)
	{
		if (polygon[i].x < minX)
			minX = polygon[i].x;
		if (polygon[i].x > maxX)
			maxX = polygon[i].x;
		if (polygon[i].y < minY)
			minY = polygon[i].y;
		if (polygon[i].y > maxY)
			maxY = polygon[i].y;
	}
}
bool checkVertex(Point p, const std::vector<Point>& polygon)
{
	for (auto& _p : polygon)
	{
		if (p.x == _p.x && p.y == _p.y)
			return false;
	}
	return true;
}
bool crossPoint_(LineWithCross& line1, LineWithCross& line2, float& u1, float& u2);
bool checkIn(Point p, const std::vector<Point>& polygon)
{
	if (!checkVertex(p, polygon))
		return false;

	float minX, maxX, minY, maxY;
	boundBox(polygon, minX, maxX, minY, maxY);
	float length = (maxX - minX) + (maxY - minY);
	float theta = 0;
	float dtheta = PI / 180;
	while (theta < 2 * PI)
	{
		Point end = { p.x + length * cos(theta), p.y + length * sin(theta) };
		LineWithCross ray;
		ray.begin = p;
		ray.end = end;
		if (checkRay(ray, polygon))
		{
			float u1 = 0, u2 = 0;
			int count = 0;
			for (int i = 0; i < polygon.size(); i++)
			{
				int next = i + 1 < polygon.size() ? i + 1 : 0;
				LineWithCross edge;
				edge.begin = polygon[i];
				edge.end = polygon[next];

				if (crossPoint_(ray, edge, u1, u2))
				{
					if (crossProduct(ray, edge) > 0)
						count++;
					else
						count--;
				}
			}
			return count >= 1;
		}
		else
		{
			theta += dtheta;
		}
	}
	assert(0 && "can not find suitable ray!!!");
	return false;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 合并多边形算法（不允许结果有中空，不允许多个多边形共同交于一交点）
struct PointInfo
{
	PointType type; // 类型
	std::map<int, int> idxInfo; // 交点索引信息
	bool dealed; // 是否已处理

	PointInfo::PointInfo() : type(PointType::None), dealed(false) {}
};
void calcLines(const std::vector<Point>& points, std::vector<LineWithCross>& lines)
{
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 < points.size() ? i + 1 : 0;
		lines.push_back(LineWithCross());
		lines.back().begin = points[i];
		lines.back().end = points[next];
		lines.back().crossPoints.clear();
	}
}
bool crossPoint_(LineWithCross& line1, LineWithCross& line2, float& u1, float& u2)
{
	float dx1 = line1.end.x - line1.begin.x;
	float dy1 = line1.end.y - line1.begin.y;
	float dx2 = line2.end.x - line2.begin.x;
	float dy2 = line2.end.y - line2.begin.y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (Equal(dy1 / dx1, dy2 / dx2))
		return false;

	float x01 = line1.begin.x;
	float y01 = line1.begin.y;
	float x02 = line2.begin.x;
	float y02 = line2.begin.y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);
	if (Equal(u1, 0))
		u1 = 0;
	if (Equal(u1, 1))
		u1 = 1;
	if (Equal(u2, 0))
		u2 = 0;
	if (Equal(u2, 1))
		u2 = 1;

	if (u1 < 0 || u1 > 1 || u2 < 0 || u2 > 1)
		return false;

	return true;
}
bool calcCrossPoint(LineWithCross& line1, LineWithCross& line2, CrossPointInfo& crossPointInfo, std::map<Point, PointInfo>& pointInfoMap)
{
	if (!crossPoint_(line1, line2, crossPointInfo.u1, crossPointInfo.u2))
		return false;

	// 如果u1,u2是特殊值，则优先使用
	if (Equal(crossPointInfo.u1, 0))
		crossPointInfo.point = line1.begin;
	else if (Equal(crossPointInfo.u1, 1))
		crossPointInfo.point = line1.end;
	else if (Equal(crossPointInfo.u2, 0))
		crossPointInfo.point = line2.begin;
	else if (Equal(crossPointInfo.u2, 1))
		crossPointInfo.point = line2.end;
	else
	{
		float dx1 = line1.end.x - line1.begin.x;
		float dy1 = line1.end.y - line1.begin.y;
		float dx2 = line2.end.x - line2.begin.x;
		float dy2 = line2.end.y - line2.begin.y;
		float x01 = line1.begin.x;
		float y01 = line1.begin.y;
		crossPointInfo.point = { x01 + crossPointInfo.u1 * dx1, y01 + crossPointInfo.u1 * dy1 };
	}

	if(pointInfoMap.find(crossPointInfo.point) == pointInfoMap.end())
		pointInfoMap[crossPointInfo.point].type = PointType::Cross;

	return true;
}
void calcPointInfo(const std::vector<std::vector<Point>>& polygons, std::map<Point, PointInfo>& pointInfoMap, std::map<int, std::map<int, int>>& polygonCrossInfoMap, std::vector<std::vector<Point>>& polygonPoints)
{
	std::vector<std::vector<LineWithCross>> polygonLines;
	for (auto& polygon : polygons)
	{
		polygonLines.push_back(std::vector<LineWithCross>());
		calcLines(polygon, polygonLines.back());
	}

	CrossPointInfo crossPointInfo;
	for (int i = 0; i < polygonLines.size(); i++)
	{
		for (int j = i + 1; j < polygonLines.size(); j++)
		{

			for (int a = 0; a < polygonLines[i].size(); a++)
			{
				for (int b = 0; b < polygonLines[j].size(); b++)
				{
					if (calcCrossPoint(polygonLines[i][a], polygonLines[j][b], crossPointInfo, pointInfoMap))
					{
						crossPointInfo.u = crossPointInfo.u1;
						polygonLines[i][a].crossPoints.push_back(crossPointInfo);
						crossPointInfo.u = crossPointInfo.u2;
						polygonLines[j][b].crossPoints.push_back(crossPointInfo);
						polygonCrossInfoMap[i][j] = 1;
						polygonCrossInfoMap[j][i] = 1;
					}
				}
			}

		}
	}
	
	for (int idx = 0; idx < polygonLines.size(); idx++)
	{
		polygonPoints.push_back({});
		for (auto& l : polygonLines[idx])
		{
			if (pointInfoMap.find(l.begin) == pointInfoMap.end())
			{
				polygonPoints.back().push_back(l.begin);
				pointInfoMap[l.begin].type = PointType::Normal;
				pointInfoMap[l.begin].idxInfo[idx] = polygonPoints.back().size() - 1;
			}

			if (l.crossPoints.size() > 1)
			{
				std::sort(l.crossPoints.begin(), l.crossPoints.end(), [](CrossPointInfo& a, CrossPointInfo& b)
				{
					return a.u < b.u;
				});
			}
			for (auto& cp : l.crossPoints)
			{
				if (pointInfoMap.find(cp.point) != pointInfoMap.end() && 
					pointInfoMap[cp.point].idxInfo.find(idx) == pointInfoMap[cp.point].idxInfo.end())
				{
					polygonPoints.back().push_back(cp.point);
					pointInfoMap[cp.point].idxInfo[idx] = polygonPoints.back().size() - 1;
				}
			}

		}
	}
}

void calcPolygons(const std::vector<std::vector<Point>>& polygons, std::map<int, std::map<int, int>>& polygonCrossInfoMap, std::vector<std::vector<int>>& result)
{
	for (int i = 0; i < polygons.size(); i++)
	{
		for (int j = i + 1; j < polygons.size(); j++)
		{
			if (polygonCrossInfoMap[i][j] != 1)
			{
				if (checkIn(polygons[i][0], polygons[j]) || checkIn(polygons[j][0], polygons[i]))
				{
					polygonCrossInfoMap[i][j] = 1;
					polygonCrossInfoMap[j][i] = 1;
				}
			}
		}
	}

	for (int i = 0; i < polygons.size(); i++)
	{
		std::vector<int> newIds = { i };
		for (auto it = result.begin(); it != result.end();)
		{
			bool merge = false;
			for (auto& j : *it)
			{
				if (polygonCrossInfoMap[i][j] == 1)
				{
					merge = true;
					break;
				}
			}

			if (merge)
			{
				newIds.insert(newIds.end(), it->begin(), it->end());
				it = result.erase(it);
			}
			else
			{
				it++;
			}
		}
		result.push_back(newIds);
	}
}
bool dealPoint(const Point& point, std::map<Point, PointInfo>& pointInfoMap, std::vector<Point>& result)
{
	assert(pointInfoMap.find(point) != pointInfoMap.end());
	if (pointInfoMap[point].dealed)
		return false;

	pointInfoMap[point].dealed = true;

	result.push_back(point);

	return true;
}
void walkPolygons(const std::vector<std::vector<Point>>& polygonPoints, int polygonIdx, int pointIdx, std::map<Point, PointInfo>& pointInfoMap, std::vector<Point>& result)
{
	const Point& curPoint = polygonPoints[polygonIdx][pointIdx];
	if (pointInfoMap[curPoint].type == PointType::Cross)
	{
		int otherPolygonIdx = -1;
		int otherPointIdx = -1;
		for (auto& i : pointInfoMap[curPoint].idxInfo)
		{
			if (i.first != polygonIdx)
			{
				otherPolygonIdx = i.first;
				otherPointIdx = i.second;
				break;
			}
		}
		assert(otherPolygonIdx != -1);
		otherPointIdx = otherPointIdx + 1 < polygonPoints[otherPolygonIdx].size() ? otherPointIdx + 1 : 0;

		const Point& nextPoint = polygonPoints[otherPolygonIdx][otherPointIdx];
		if (!sameLine(result.back(), curPoint, nextPoint))
		{
			if (!dealPoint(curPoint, pointInfoMap, result))
				return;
		}
		walkPolygons(polygonPoints, otherPolygonIdx, otherPointIdx, pointInfoMap, result);
	}
	else
	{
		if (!dealPoint(curPoint, pointInfoMap, result))
			return;

		pointIdx = pointIdx + 1 < polygonPoints[polygonIdx].size() ? pointIdx + 1 : 0;
		walkPolygons(polygonPoints, polygonIdx, pointIdx, pointInfoMap, result);
	}
}
void findOutterPointInfo(const std::vector<std::vector<Point>>& polygons, const std::vector<std::vector<int>>& result, std::map<int, int>& resultInfo)
{
	for (auto& r : result)
	{
		int polygonIdx = r[0], pointIdx = 0;
		for (auto& id : r)
		{
			for (int i = 0; i < polygons[id].size(); i++)
			{
				if (polygons[id][i].x < polygons[polygonIdx][pointIdx].x)
				{
					polygonIdx = id;
					pointIdx = i;
				}
				else if (polygons[id][i].x == polygons[polygonIdx][pointIdx].x)
				{
					if (polygons[id][i].y < polygons[polygonIdx][pointIdx].y)
					{
						polygonIdx = id;
						pointIdx = i;
					}
				}
			}
		}

		resultInfo[polygonIdx] = pointIdx;
	}
}
void mergePolygons(const std::vector<std::vector<Point>>& polygons, std::vector<std::vector<Point>>& result)
{
	std::vector<std::vector<Point>> walkPoints;
	std::map<Point, PointInfo> pointInfoMap;
	std::map<int, std::map<int, int>> polygonCrossInfoMap;
	calcPointInfo(polygons, pointInfoMap, polygonCrossInfoMap, walkPoints);

	std::vector<std::vector<int>> polygonIdxs;
	calcPolygons(polygons, polygonCrossInfoMap, polygonIdxs);

	std::map<int, int> beginInfo;
	findOutterPointInfo(polygons, polygonIdxs, beginInfo);

	for (auto & b : beginInfo)
	{
		result.push_back({});
		walkPolygons(walkPoints, b.first, b.second, pointInfoMap, result.back());
	}
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 任意形状多边形填充算法
struct Line
{
	int x0;
	int y0;
	int x1;
	int y1;
};
struct SortedLine
{
	int maxY;
	int minY;
	int beginX;
	int endX;
	int dx;
	int dy;
};
struct SortedLineSet
{
	int scanY;
	std::vector<SortedLine> sortedLines;
};
struct ActiveLine
{
	SortedLine sortedLine;
	int counter;
	int currentX;
};
void hLine(int y, int x0, int x1)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(x, y);
	}
}
std::vector<SortedLineSet> SortLines(const std::vector<Point>& points)
{
	std::vector<Line> lines;
	for (int i = 0; i < points.size(); i++)
	{
		int next = (i + 1) % points.size();
		// 跳过水平线
		if (points[i].y == points[next].y)
			continue;

		lines.push_back(Line());
		lines.back().x0 = points[i].x;
		lines.back().y0 = points[i].y;
		lines.back().x1 = points[next].x;
		lines.back().y1 = points[next].y;
	}

	for (int i = 0; i < lines.size(); i++)
	{
		int next = (i + 1) % lines.size();
		if (lines[i].y1 - lines[i].y0 > 0 && lines[next].y1 - lines[next].y0 > 0)
			lines[i].y1--;
		else if (lines[i].y1 - lines[i].y0 < 0 && lines[next].y1 - lines[next].y0 < 0)
			lines[next].y0--;
	}

	// 再次检查水平线
	for (auto it = lines.begin(); it != lines.end();)
	{
		if (it->y0 == it->y1)
		{
			it = lines.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (auto& line : lines)
	{
		if (line.y0 > line.y1)
		{
			std::swap(line.x0, line.x1);
			std::swap(line.y0, line.y1);
		}
	}

	std::sort(lines.begin(), lines.end(), [](auto& a, auto& b)
	{
		if (a.y0 == b.y0)
		{
			if (a.x0 == b.x0)
			{
				if (a.x1 == b.x1)
					return a.y1 < b.y1;
				return a.x1 < b.x1;
			}
			return a.x0 < b.x0;
		}
		return a.y0 < b.y0;
	});
	std::vector<SortedLineSet> lineSet;
	int lastY = -99999;
	int maxY = -99999;
	for (auto& line : lines)
	{
		if (line.y0 != lastY)
		{
			lineSet.push_back(SortedLineSet());
		}
		lineSet.back().scanY = line.y0;
		lineSet.back().sortedLines.push_back(SortedLine());
		lineSet.back().sortedLines.back().beginX = line.x0;
		lineSet.back().sortedLines.back().endX = line.x1;
		lineSet.back().sortedLines.back().maxY = line.y1;
		lineSet.back().sortedLines.back().minY = line.y0;
		lineSet.back().sortedLines.back().dx = line.x1 - line.x0;
		lineSet.back().sortedLines.back().dy = line.y1 - line.y0;
		lastY = line.y0;

		if (maxY < line.y1)
			maxY = line.y1;
	}
	lineSet.push_back({ maxY + 1 ,{} }); // 结尾
	return lineSet;
}
void fillWithActiveLines(int beginY, int endY, std::vector<ActiveLine>& activeLines)
{
	std::vector<std::vector<Point>> points;
	for (int curY = beginY; curY < endY; curY++)
	{
		for (auto& line : activeLines)
		{
			if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
			{
				if (std::abs(line.sortedLine.dy) >= std::abs(line.sortedLine.dx))
				{// |m|>1			
					points.push_back({ { (float)line.currentX , (float)curY } });

					line.counter += std::abs(line.sortedLine.dx * 2);

					if (line.counter >= line.sortedLine.dy)
					{
						if (line.sortedLine.dx > 0)
							line.currentX++;
						else
							line.currentX--;
						line.counter -= line.sortedLine.dy * 2;
					}
				}
				else
				{// |m|<1
					points.push_back({ { (float)line.currentX, (float)curY } });
					while (true)
					{
						if (line.sortedLine.dx > 0)
							line.currentX++;
						else
							line.currentX--;

						line.counter += std::abs(line.sortedLine.dy * 2);
						if ((line.counter >= std::abs(line.sortedLine.dx)) ||
							(line.sortedLine.dx > 0 ? line.currentX > line.sortedLine.endX : line.currentX < line.sortedLine.endX) /* 结束条件*/)
						{
							line.counter -= std::abs(line.sortedLine.dx * 2);
							break;
						}
					}
					if (line.sortedLine.dx > 0)
						points.back().push_back({ (float)line.currentX - 1, (float)curY });
					else
						points.back().push_back({ (float)line.currentX + 1, (float)curY });

					std::sort(points.back().begin(), points.back().end(), [](auto& a, auto&b) {return a.x < b.x;});
				}
			}
		}
		std::sort(points.begin(), points.end(), [](auto& a, auto&b)
		{
			if (a.front().x == b.front().x)
				return a.back().x < b.back().x;
			return a.front().x < b.front().x;
		});
		for (int i = 0; ; i++)
		{
			if (2 * i < points.size() && 2 * i + 1 < points.size())
			{
				hLine(points[2 * i].front().y, points[2 * i].front().x, points[2 * i + 1].back().x);
			}
			else
			{
				points.clear();
				break;
			}
		}
	}
}
void fillPolygon(const std::vector<Point>& points)
{
	std::vector<SortedLineSet> sortedLines = SortLines(points);
	std::vector<ActiveLine> activeLines;
	for (int i = 0; i < sortedLines.size() - 1; i++)
	{
		int curY = sortedLines[i].scanY;
		for (auto it = activeLines.begin(); it != activeLines.end();)
		{
			if (curY > it->sortedLine.maxY)
			{
				it = activeLines.erase(it);
			}
			else
			{
				it++;
			}
		}
		for (auto& _sortedLine : sortedLines[i].sortedLines)
		{
			activeLines.push_back(ActiveLine());
			activeLines.back().sortedLine = _sortedLine;
			activeLines.back().counter = 0;
			activeLines.back().currentX = _sortedLine.beginX;
		}
		fillWithActiveLines(curY, sortedLines[i + 1].scanY, activeLines);
	}
}
//////////////////////////////////////////////////////////////////////////

// 把凹多边形分割成凸多边形
void polygonClipSutherlanHodgman1(const std::vector<Point>& polygon, const std::vector<Point>& clipWindow, std::vector<std::vector<Point>>& result)
{
	std::vector<std::vector<Point>> cutPolygons;
	cutPolygon(polygon, cutPolygons);

	std::vector<std::vector<Point>> clipPolygons;
	for (auto& p : cutPolygons)
	{
		std::vector<Point> points(32, { 0, 0 });
		int n = polygonClipSuthHodg1(clipWindow[0], clipWindow[2], p.size(), &p[0], &points[0]);
		if (n)
		{
			points.resize(n);
			clipPolygons.push_back(points);
		}
	}
	
	mergePolygons(clipPolygons, result);
}


//////////////////////////////////////////////////////////////////////////
// Sutherlan-Hodgman多边形裁剪算法(凸多边形)
enum class AntiClockBoundary 
{ None = -1, Left, Bottom, Right, Top } ;
wcPt2D intersect2(wcPt2D p1, wcPt2D p2, Boundary winEdge, wcPt2D wMin, wcPt2D wMax, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo)
{
	wcPt2D iPt;
	GLfloat m;

	if (p1.x != p2.x)
		m = (p1.y - p2.y) / (p1.x - p2.x);
	switch (winEdge)
	{
	case Boundary::Left:
		iPt.x = wMin.x;
		iPt.y = p2.y + (wMin.x - p2.x) * m;

		if(iPt.y >= wMin.y && iPt.y <= wMax.y)
			_boundaryOrderInfo[AntiClockBoundary::Left].push_back(iPt);

		break;
	case Boundary::Right:
		iPt.x = wMax.x;
		iPt.y = p2.y + (wMax.x - p2.x) * m;

		if(iPt.y >= wMin.y && iPt.y <= wMax.y)
			_boundaryOrderInfo[AntiClockBoundary::Right].push_back(iPt);

		break;
	case Boundary::Bottom:
		iPt.y = wMin.y;
		if (p1.x != p2.x)
			iPt.x = p2.x + (wMin.y - p2.y) / m;
		else
			iPt.x = p2.x;

		if(iPt.x >= wMin.x && iPt.x <= wMax.x)
			_boundaryOrderInfo[AntiClockBoundary::Bottom].push_back(iPt);

		break;
	case Boundary::Top:
		iPt.y = wMax.y;
		if (p1.x != p2.x)
			iPt.x = p2.x + (wMax.y - p2.y) / m;
		else
			iPt.x = p2.x;

		if(iPt.x >= wMin.x && iPt.x <= wMax.x)
			_boundaryOrderInfo[AntiClockBoundary::Top].push_back(iPt);

		break;
	default:
		break;
	}

	return (iPt);
}
void clipPoint2(wcPt2D p, Boundary winEdge, wcPt2D wMin, wcPt2D wMax, wcPt2D* pOut, int* cnt, wcPt2D* first[], wcPt2D* s, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo)
{
	wcPt2D iPt;
	if (!first[winEdge])
		first[winEdge] = new wcPt2D{ p.x, p.y };
	else
	{
		if (cross(p, s[winEdge], winEdge, wMin, wMax))
		{
			iPt = intersect2(p, s[winEdge], winEdge, wMin, wMax, _boundaryOrderInfo);
			if (winEdge < Boundary::Top)
				clipPoint2(iPt, (Boundary)(winEdge + 1), wMin, wMax, pOut, cnt, first, s, _boundaryOrderInfo);
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
		if (winEdge < Boundary::Top)
			clipPoint2(p, (Boundary)(winEdge + 1), wMin, wMax, pOut, cnt, first, s, _boundaryOrderInfo);
		else
		{
			pOut[*cnt] = p;
			(*cnt)++;
		}
	}
}
void closeClip2(wcPt2D wMin, wcPt2D wMax, wcPt2D* pOut, GLint* cnt, wcPt2D* first[], wcPt2D* s, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo)
{
	wcPt2D pt;
	Boundary winEdge;
	for (winEdge = Boundary::Left; winEdge <= Boundary::Top; winEdge = (Boundary)(winEdge + 1))
	{
		if (first[winEdge] && cross(s[winEdge], *first[winEdge], winEdge, wMin, wMax))
		{
			pt = intersect2(s[winEdge], *first[winEdge], winEdge, wMin, wMax, _boundaryOrderInfo);
			if (winEdge < Boundary::Top)
				clipPoint2(pt, (Boundary)(winEdge + 1), wMin, wMax, pOut, cnt, first, s, _boundaryOrderInfo);
			else
			{
				pOut[*cnt] = pt;
				(*cnt)++;
			}
		}
	}
}
GLint polygonClipSuthHodg2(wcPt2D wMin, wcPt2D wMax, GLint n, wcPt2D* pIn, wcPt2D* pOut, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo)
{
	for (int i = 0; i < 4; i++)
	{
		sPoint[i].clear();
	}
	wcPt2D* first[nClip] = { 0, 0, 0, 0 }, s[nClip];
	GLint k, cnt = 0;
	for (k = 0; k < n; k++)
		clipPoint2(pIn[k], Boundary::Left, wMin, wMax, pOut, &cnt, first, s, _boundaryOrderInfo);
	closeClip2(wMin, wMax, pOut, &cnt, first, s, _boundaryOrderInfo);

	printf("==================================================\n");
	for (int i = 0; i < 4; i++)
	{
		if (first[i])
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
//////////////////////////////////////////////////////////////////////////

void cutResult(const std::vector<Point>& polygon, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo, std::vector<std::vector<Point>>& result)
{
	for (auto& b : _boundaryOrderInfo)
	{
		switch (b.first)
		{
		case AntiClockBoundary::Left:
			std::sort(b.second.begin(), b.second.end(), [](Point& a, Point& b)
			{
				return a.y > b.y;
			});
			break;
		case AntiClockBoundary::Bottom:
			std::sort(b.second.begin(), b.second.end(), [](Point& a, Point& b)
			{
				return a.x < b.x;
			});
			break;
		case AntiClockBoundary::Right:
			std::sort(b.second.begin(), b.second.end(), [](Point& a, Point& b)
			{
				return a.y < b.y;
			});
			break;
		case AntiClockBoundary::Top:
			std::sort(b.second.begin(), b.second.end(), [](Point& a, Point& b)
			{
				return a.x > b.x;
			});
			break;
		default:
			break;
		}
	}

	std::map<Point, Point> _pointOrderInfo;
	for (auto& b : _boundaryOrderInfo)
	{
		for (int i = 0; i < b.second.size(); i++)
		{
			if (i + 1 < b.second.size())
			{
				_pointOrderInfo[b.second[i]] = b.second[i + 1];
			}
			else
			{
				for (int j = 0; j < 4; j++)
				{
					int edge = ((int)b.first + j + 1) % 4; 
					if (_boundaryOrderInfo[(AntiClockBoundary)edge].size() > 0)
					{
						_pointOrderInfo[b.second[i]] = _boundaryOrderInfo[(AntiClockBoundary)edge][0];
						break;
					}
				}
			}
		}
	}

	result.push_back({});
	for (int i = 0; i < polygon.size(); i++)
	{
		result.back().push_back(polygon[i]);

		int next = i + 1 < polygon.size() ? i + 1 : 0;
		if (_pointOrderInfo.find(polygon[i]) != _pointOrderInfo.end() &&
			_pointOrderInfo.find(polygon[next]) != _pointOrderInfo.end() &&
			_pointOrderInfo[polygon[i]] != polygon[next])
		{
			result.push_back({});
		}
	}
	if (result.size() > 1)
	{
		result[0].insert(result[0].end(), result[result.size() - 1].begin(), result[result.size() - 1].end());
		result.erase(result.end() - 1);
	}

	for (auto it = result.begin(); it != result.end(); )
	{
		if (it->size() < 3)
			it = result.erase(it);
		else
			it++;
	}
}

// 分割边界上的多组顶点
void polygonClipSutherlanHodgman2(const std::vector<Point>& polygon, const std::vector<Point>& clipWindow, std::vector<std::vector<Point>>& result)
{
	std::map<AntiClockBoundary, std::vector<Point>> _boundaryOrderInfo;

	std::vector<Point> points(32, { 0, 0 });
	int n = polygonClipSuthHodg2(clipWindow[0], clipWindow[2], polygon.size(), (Point*)&polygon[0], &points[0], _boundaryOrderInfo);
	if(n)
	{
		points.resize(n);
		cutResult(points, _boundaryOrderInfo, result);
	}
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	std::vector<Point> polygon, clipWindow;
	std::vector<std::vector<Point>> result;

	glViewport(0, winHeight / 2, winWidth / 3, winHeight / 2);
	polygon = { {49, 61},{ 99, 72 },{67, 104}, {109, 134},{40, 147}};
	clipWindow = { { 80, 50 },{ 180, 50 },{ 180, 170 },{ 80, 170 } };
	result.clear();
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipSutherlanHodgman1(polygon, clipWindow, result);
	for (auto& r : result)
	{
		fillPolygon(r);
	}

	glViewport(0, 0, winWidth / 3, winHeight / 2);
	result.clear();
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipSutherlanHodgman2(polygon, clipWindow, result);
	for (auto& r : result)
	{
		fillPolygon(r);
	}

	glViewport(winWidth / 3, winHeight / 2, winWidth / 3, winHeight / 2);
	polygon = { { 78, 20 },{ 99, 72 },{ 67, 104 },{ 109, 134 },{ 40, 147 } };
	clipWindow = { { 80, 50 },{ 180, 50 },{ 180, 170 },{ 80, 170 } };
	result.clear();
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipSutherlanHodgman1(polygon, clipWindow, result);
	for (auto& r : result)
	{
		fillPolygon(r);
	}

	glViewport(winWidth / 3, 0, winWidth / 3, winHeight / 2);
	result.clear();
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipSutherlanHodgman2(polygon, clipWindow, result);
	for (auto& r : result)
	{
		fillPolygon(r);
	}

	glViewport(2 * winWidth / 3, winHeight / 2, winWidth / 3, winHeight / 2);
	polygon = { { 78, 20 },{ 99, 72 },{ 48, 173 },{ 136, 185 },{ 162, 148 },{203, 170},{9, 243} };
	clipWindow = { { 80, 50 },{ 180, 50 },{ 180, 170 },{ 80, 170 } };
	result.clear();
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipSutherlanHodgman1(polygon, clipWindow, result);
	for (auto& r : result)
	{
		fillPolygon(r);
	}

	glViewport(2 * winWidth / 3, 0, winWidth / 3, winHeight / 2);
	result.clear();
	glColor3f(1.0, 1.0, 1.0);
	drawPolygonLine(clipWindow);
	fillPolygon(polygon);
	glColor3f(1.0, 0.0, 0.0);
	polygonClipSutherlanHodgman2(polygon, clipWindow, result);
	for (auto& r : result)
	{
		fillPolygon(r);
	}
	
	glFlush();
}
void code_8_exercise_add_1()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth / 3, 0, winHeight / 2);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_1
class Point
{
public:
	GLfloat x, y;
};
typedef Point Vec;
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
int crossProduct(const Vec& vec1, const Vec& vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}
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
GLint encode(const std::vector<Point>& polygon, Point p)
{
	GLint ret = 0;
	for (int i = 0; i < polygon.size(); i++)
	{
		int next = i + 1 < polygon.size() ? i + 1 : 0;
		if (crossProduct({ polygon[next].x - polygon[i].x, polygon[next].y - polygon[i].y },
		{ polygon[i].x - p.x, polygon[i].y - p.y }) > 0)
		{
			ret += std::pow(2, i);
		}
	}
	return ret;
}
void swapPts(Point* p1, Point* p2)
{
	Point tmp;
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
bool crossPoint(Point line1Begin, Point line1End, Point line2Begin, Point line2End, float& u1, float& u2)
{
	float dx1 = line1End.x - line1Begin.x;
	float dy1 = line1End.y - line1Begin.y;
	float dx2 = line2End.x - line2Begin.x;
	float dy2 = line2End.y - line2Begin.y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (Equal(dy1 / dx1, dy2 / dx2))
		return false;

	float x01 = line1Begin.x;
	float y01 = line1Begin.y;
	float x02 = line2Begin.x;
	float y02 = line2Begin.y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);

	return true;
}
// 按照Cohen-Sutherlan原始算法, 先把p1裁到内点，再交换p1,p2，再将p2裁到内点，可能需要两次遍历所有边进行裁剪
void lineClipCohSuth1(const std::vector<Point>& polygon, Point p1, Point p2)
{
	GLubyte code1, code2;
	bool done = false, plotLine = false;
	code1 = encode(polygon, p1);
	code2 = encode(polygon, p2);

	while (!done)
	{
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
			for (int i = 0; i < polygon.size(); i++)
			{
				if (code1 & (int)std::pow(2, i))
				{
					int next = i + 1 < polygon.size() ? i + 1 : 0;
					float u1, u2;
					assert(crossPoint(p1, p2, polygon[i], polygon[next], u1, u2));
					p1.x += u1 * (p2.x - p1.x);
					p1.y += u1 * (p2.y - p1.y);
					code1 = encode(polygon, p1);
					break;
				}
			}
		}
	}
	if (plotLine)
		//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
}
// 改良算法, 一次遍历所有边进行可能的裁剪
void lineClipCohSuth2(const std::vector<Point>& polygon, Point p1, Point p2)
{
	GLubyte code1, code2;
	bool plotLine = false;
	code1 = encode(polygon, p1);
	code2 = encode(polygon, p2);

	for (int i = 0; i < polygon.size(); i++)
	{
		if (accept(code1, code2))
		{
			plotLine = true;
			break;
		}
		else if (reject(code1, code2))
		{
			break;
		}
		else
		{
			bool clip = false;
			if (code2 & (int)std::pow(2, i))
			{
				swapPts(&p1, &p2);
				swapCodes(&code1, &code2);
				clip = true;
			}
			else
			{
				clip = code1 & (int)std::pow(2, i);
			}

			if (clip)
			{
				int next = i + 1 < polygon.size() ? i + 1 : 0;
				float u1, u2;
				assert(crossPoint(p1, p2, polygon[i], polygon[next], u1, u2));
				p1.x += u1 * (p2.x - p1.x);
				p1.y += u1 * (p2.y - p1.y);
				code1 = encode(polygon, p1);
			}
		}
	}
	if (!plotLine && accept(code1, code2))
	{
		plotLine = true;
	}
	if (plotLine)
		//lineBres(Round(p1.x), Round(p1.y), Round(p2.x), Round(p2.y)); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::vector<Point> polygon = { { 77, 188 },{ 202, 127 },{ 281, 232 },{ 121, 378 } };
	Point p1, p2;

	glColor3f(1.0, 1.0, 1.0);
	glViewport(0, 0, winWidth / 2, winHeight);
	drawPolygonLine(polygon);

	p1 = { 56, 53 }, p2 = { 27, 297 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth1(polygon, p1, p2);

	p1 = { 63, 296 }, p2 = { 103, 142 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth1(polygon, p1, p2);

	p1 = { 109, 97 }, p2 = { 191, 390 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth1(polygon, p1, p2);

	p1 = { 116, 453 }, p2 = { 191, 50 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth1(polygon, p1, p2);

	p1 = { 300, 322 }, p2 = { 175, 274 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth1(polygon, p1, p2);

	p1 = { 185, 195 }, p2 = { 190, 245 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth1(polygon, p1, p2);

	p1 = { 245, 130 }, p2 = { 313, 143 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth1(polygon, p1, p2);

	glColor3f(1.0, 1.0, 1.0);
	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	drawPolygonLine(polygon);

	p1 = { 56, 53 }, p2 = { 27, 297 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth2(polygon, p1, p2);

	p1 = { 63, 296 }, p2 = { 103, 142 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth2(polygon, p1, p2);

	p1 = { 109, 97 }, p2 = { 191, 390 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth2(polygon, p1, p2);

	p1 = { 116, 453 }, p2 = { 191, 50 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth2(polygon, p1, p2);

	p1 = { 300, 322 }, p2 = { 175, 274 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth2(polygon, p1, p2);

	p1 = { 185, 195 }, p2 = { 190, 245 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth2(polygon, p1, p2);

	p1 = { 245, 130 }, p2 = { 313, 143 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCohSuth2(polygon, p1, p2);

	glFlush();
}
void code_8_exercise_add_1_1()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth / 2, 0, winHeight);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_2
class Point
{
public:
	GLfloat x, y;
};
typedef Point Vec;
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
int crossProduct(const Vec& vec1, const Vec& vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

bool crossPoint(Point line1Begin, Point line1End, Point line2Begin, Point line2End, float& u1, float& u2)
{
	float dx1 = line1End.x - line1Begin.x;
	float dy1 = line1End.y - line1Begin.y;
	float dx2 = line2End.x - line2Begin.x;
	float dy2 = line2End.y - line2Begin.y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (Equal(dy1 / dx1, dy2 / dx2))
		return false;

	float x01 = line1Begin.x;
	float y01 = line1Begin.y;
	float x02 = line2Begin.x;
	float y02 = line2Begin.y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);

	return true;
}
GLint clipTest(Point p1, Point p2, Point lineBegin, Point lineEnd, float& u1, float& u2)
{
	GLfloat r1, r2;
	GLint returnValue = true;

	if (crossPoint(p1, p2, lineBegin, lineEnd, r1, r2))
	{
		if (crossProduct({ lineEnd.x - lineBegin.x, lineEnd.y - lineBegin.y },
		{ p2.x - p1.x, p2.y - p1.y }) > 0)
		{
			if (r1 > u2)
				returnValue = false;
			else if (r1 > u1)
				u1 = r1;
		}
		else
		{
			if (r1 < u1)
				returnValue = false;
			else if (r1 < u2)
				u2 = r1;
		}
	}
	else
	{
		if (crossProduct({ lineEnd.x - lineBegin.x, lineEnd.y - lineBegin.y },
		{ lineBegin.x - p1.x, lineBegin.y - p1.y }) > 0)
		{
			returnValue = false;
		}
	}

	return (returnValue);
}
void lineClipLiangBarsk(const std::vector<Point>& polygon, Point p1, Point p2)
{
	float u1 = 0.f, u2 = 1.f;
	bool test = true;
	for (int i = 0; i < polygon.size(); i++)
	{
		int next = i + 1 < polygon.size() ? i + 1 : 0;
		if (!clipTest(p1, p2, polygon[i], polygon[next], u1, u2))
		{
			test = false;
			break;
		}
	}
	if (test)
	{
		float dx = p2.x - p1.x, dy = p2.y - p1.y;
		if (u2 < 1.0)
		{
			p2.x = p1.x + u2 * dx;
			p2.y = p1.y + u2 * dy;
		}
		if (u1 > 0.0)
		{
			p1.x = p1.x + u1 * dx;
			p1.y = p1.y + u1 * dy;;
		}
		//lineBres(Round(p1.getx()), Round(p1.gety()), Round(p2.getx()), Round(p2.gety())); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::vector<Point> polygon = { { 77, 188 },{ 202, 127 },{ 281, 232 },{ 121, 378 } };
	Point p1, p2;

	glColor3f(1.0, 1.0, 1.0);
	glViewport(0, 0, winWidth / 2, winHeight);
	drawPolygonLine(polygon);

	p1 = { 56, 53 }, p2 = { 27, 297 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipLiangBarsk(polygon, p1, p2);

	p1 = { 63, 296 }, p2 = { 103, 142 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipLiangBarsk(polygon, p1, p2);

	p1 = { 109, 97 }, p2 = { 191, 390 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipLiangBarsk(polygon, p1, p2);

	p1 = { 116, 453 }, p2 = { 191, 50 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipLiangBarsk(polygon, p1, p2);

	p1 = { 300, 322 }, p2 = { 175, 274 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipLiangBarsk(polygon, p1, p2);

	p1 = { 185, 195 }, p2 = { 190, 245 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipLiangBarsk(polygon, p1, p2);

	p1 = { 245, 130 }, p2 = { 313, 143 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipLiangBarsk(polygon, p1, p2);

	glFlush();
}
void code_8_exercise_add_1_2()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth / 2, 0, winHeight);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_3
class Point
{
public:
	GLfloat x, y;
};
typedef Point Vec;
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
int crossProduct(const Vec& vec1, const Vec& vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}
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
void swapPts(Point& p1, Point& p2)
{
	Point tmp;
	tmp = p1;
	p1 = p2;
	p2 = tmp;
}
void swapCodes(GLubyte& c1, GLubyte& c2)
{
	GLubyte tmp;
	tmp = c1;
	c1 = c2;
	c2 = tmp;
}
GLint encode(const std::vector<Point>& polygon, Point p)
{
	GLint ret = 0;
	for (int i = 0; i < polygon.size(); i++)
	{
		int next = i + 1 < polygon.size() ? i + 1 : 0;
		if (crossProduct({ polygon[next].x - polygon[i].x, polygon[next].y - polygon[i].y },
		{ polygon[i].x - p.x, polygon[i].y - p.y }) > 0)
		{
			ret += std::pow(2, i);
		}
	}
	return ret;
}
int smallVec(const Vec& vec1, const Vec& vec2)
{
	return crossProduct(vec1, vec2) >= 0;
}
int realIdx(const std::vector<Point>& ploygon, int idx)
{
	return (idx + ploygon.size()) % ploygon.size();
}
int findFirstIdx(Point p0, const std::vector<Point>& polygon)
{
	for (int i = 0; i < polygon.size(); i++)
	{
		Vec curVec = { polygon[i].x - p0.x, polygon[i].y - p0.y };
		if (smallVec(curVec, { polygon[realIdx(polygon, i + 1)].x - p0.x, polygon[realIdx(polygon, i + 1)].y - p0.y }) &&
			smallVec(curVec, { polygon[realIdx(polygon, i - 1)].x - p0.x, polygon[realIdx(polygon, i - 1)].y - p0.y }))
		{
			return i;
		}
	}
	assert(0);
	return -1;
}
bool crossPoint(Point line1Begin, Point line1End, Point line2Begin, Point line2End, float& u1, float& u2)
{
	float dx1 = line1End.x - line1Begin.x;
	float dy1 = line1End.y - line1Begin.y;
	float dx2 = line2End.x - line2Begin.x;
	float dy2 = line2End.y - line2Begin.y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (Equal(dy1 / dx1, dy2 / dx2))
		return false;

	float x01 = line1Begin.x;
	float y01 = line1Begin.y;
	float x02 = line2Begin.x;
	float y02 = line2Begin.y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);

	return true;
}
void calcClipPoint(int clockIdx, int antiClockIdx, const std::vector<Point>& polygon, Point& p1, Point& p2)
{
	int nextClockIdx = realIdx(polygon, clockIdx + 1);
	int nextAntiClockIdx = realIdx(polygon, antiClockIdx - 1);
	
	// 直线从外到内，一定是先进入clock边，再离开antiClock边，理由如下：
	// 由于第一个初始点是逆时针方向的第一个点，逆时针即叉积正方向，即第一个初始点是沿直线方向的最右点，而其他点则是由初始点从右往左。
	// 1.对于顺时针点：
	// 有（直线方向）X（初始点到顺时针点）为正，而初始点到顺时针点的方向与该边方向相反（边为逆时针），即有（直线方向）X（该边方向）为负，即有（该边方向）X（直线方向）为正，即直线进入该边（顺时针点边）
	// 2.对于逆时针点：
	// 有（直线方向）X（初始点到逆时针点）为正，而初始点到逆时针点的方向与该边方向相同（边为逆时针），即有（直线方向）X（该边方向）为正，即有（该边方向）X（直线方向）为负，即直线离开该边（逆时针点边）
	// 而进入一定在前，离开一定在后，所以按直线P1到P2的方向一定是P1进入，P2离开
	float dx = p2.x - p1.x, dy = p2.y - p1.y;
	auto _p1 = p1;
	float u1, u2;
	if (crossPoint(_p1, p2, polygon[clockIdx], polygon[nextClockIdx], u1, u2))
	{
		if (0 <= u1 && u1 <= 1 && 0 <= u2 && u2 <= 1)
		{
			p1.x = _p1.x + u1 * dx;
			p1.y = _p1.y + u1 * dy;
		}
	}
	if (crossPoint(_p1, p2, polygon[antiClockIdx], polygon[nextAntiClockIdx], u1, u2))
	{
		if (0 <= u1 && u1 <= 1 && 0 <= u2 && u2 <= 1)
		{
			p2.x = _p1.x + u1 * dx;
			p2.y = _p1.y + u1 * dy;
		}
	}
}
bool walkPoint(int clockIdx, int antiClockIdx, const std::vector<Point>& polygon, Point& p1, Point& p2)
{
	Vec lineVec = { p2.x - p1.x, p2.y - p1.y };
	if (clockIdx == antiClockIdx)
	{
		if (smallVec(lineVec, { polygon[clockIdx].x - p1.x, polygon[clockIdx].y - p1.y }))
		{
			calcClipPoint(clockIdx, antiClockIdx, polygon, p1, p2);
			return true;
		}
		return false;
	}
	else
	{
		if (smallVec({ polygon[clockIdx].x - p1.x, polygon[clockIdx].y - p1.y }, { polygon[antiClockIdx].x - p1.x, polygon[antiClockIdx].y - p1.y }))
		{
			if (smallVec(lineVec, { polygon[clockIdx].x - p1.x, polygon[clockIdx].y - p1.y }))
			{
				calcClipPoint(clockIdx, antiClockIdx, polygon, p1, p2);
				return true;
			}
			else
			{
				return walkPoint(realIdx(polygon, clockIdx - 1), antiClockIdx, polygon, p1, p2);
			}
		}
		else
		{
			if (smallVec(lineVec, { polygon[antiClockIdx].x - p1.x, polygon[antiClockIdx].y - p1.y }))
			{
				calcClipPoint(clockIdx, antiClockIdx, polygon, p1, p2);
				return true;
			}
			else
			{
				return walkPoint(clockIdx, realIdx(polygon, antiClockIdx + 1), polygon, p1, p2);
			}
		}
	}
}
bool lineClipNLNOutter(const std::vector<Point>& polygon, Point& p1, Point& p2)
{
	bool ret = false;
	int firstIdx = findFirstIdx(p1, polygon);
	if (smallVec({ polygon[firstIdx].x - p1.x, polygon[firstIdx].y - p1.y }, { p2.x - p1.x, p2.y - p1.y }))
	{
		ret = walkPoint(realIdx(polygon, firstIdx - 1), realIdx(polygon, firstIdx + 1), polygon, p1, p2);
	}
	return ret;
}
bool lineClipNLNInside(const std::vector<Point>& polygon, Point& p1, Point& p2)
{
	Vec lineVec = { p2.x - p1.x, p2.y - p1.y };

	for (int i = 0; i < polygon.size(); i++)
	{
		int next = i + 1 < polygon.size() ? i + 1 : 0;
		if (smallVec({ polygon[i].x - p1.x, polygon[i].y - p1.y }, lineVec) &&
			smallVec(lineVec, { polygon[next].x - p1.x, polygon[next].y - p1.y }))
		{
			float u1, u2;
			if (crossPoint(p1, p2, polygon[i], polygon[next], u1, u2))
			{
				if (0 <= u1 && u1 <= 1 && 0 <= u2 && u2 <= 1)
				{
					p2.x = p1.x + u1 * (p2.x - p1.x);
					p2.y = p1.y + u1 * (p2.y - p1.y);
				}
			}
			return true;
		}
	}
	return false;
}
void lineClipNLN(const std::vector<Point>& polygon, Point p1, Point p2)
{
	GLubyte code1 = encode(polygon, p1);
	GLubyte code2 = encode(polygon, p2);
	bool draw = false;
	if (accept(code1, code2))
	{
		draw = true;
	}
	else if (reject(code1, code2))
	{
		
	}
	else
	{
		if (inside(code2))
		{
			swapPts(p1, p2);
			swapCodes(code1, code2);
		}
		if (inside(code1))
		{
			draw = lineClipNLNInside(polygon, p1, p2);
		}
		else
		{
			draw = lineClipNLNOutter(polygon, p1, p2);
		}
	}

	if (draw)
		//lineBres(Round(p1.getx()), Round(p1.gety()), Round(p2.getx()), Round(p2.gety())); // 精确到浮点数绘图
		lineBres(p1.x, p1.y, p2.x, p2.y);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<Point> polygon;
	Point p1, p2;

	glColor3f(1.0, 1.0, 1.0);
	glViewport(0, 0, winWidth / 2, winHeight);
	polygon = { { 100, 220 },{ 300, 220 },{ 300, 360 },{ 100, 360 }};
	drawPolygonLine(polygon);

	p1 = { 208, 428 }, p2 = { 119, 109 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 224, 262 }, p2 = { 68, 431 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 166, 230 }, p2 = { 53, 355 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 131, 237 }, p2 = { 41, 237 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 334, 310 }, p2 = { 239, 204 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 371, 239 }, p2 = { 204, 157 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 264, 413 }, p2 = { 264, 252 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 50, 170 }, p2 = { 300, 420 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 350, 410 }, p2 = { 250, 310 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	glColor3f(1.0, 1.0, 1.0);
	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	polygon = { { 178, 212 },{ 316, 234 },{ 312, 307 },{ 193, 389 },{ 128, 293 } };
	drawPolygonLine(polygon);

	p1 = { 85, 242 }, p2 = { 248, 58 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 147, 360 }, p2 = { 251, 396 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 89, 351 }, p2 = { 374, 235 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 170, 290 }, p2 = { 136, 217 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 243, 233 }, p2 = { 185, 334 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 280, 147 }, p2 = { 304, 193 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 128, 162 }, p2 = { 328, 362 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	p1 = { 362, 357 }, p2 = { 282, 277 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipNLN(polygon, p1, p2);

	glFlush();
}
void code_8_exercise_add_1_3()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth / 2, 0, winHeight);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_4
class Point
{
public:
	GLfloat x, y;
};
typedef Point Vec;
void lineBres(float x0, float y0, float xEnd, float yEnd)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(xEnd, yEnd);
	glEnd();
	return;
}
void drawPolygonLine(const vector<Point>& polygon)
{
	glBegin(GL_LINE_LOOP);
	for (auto& p : polygon)
		glVertex2f(p.x, p.y);
	glEnd();
}
int crossProduct(const Vec& vec1, const Vec& vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}
bool crossPoint(Point line1Begin, Point line1End, Point line2Begin, Point line2End, float& u1, float& u2)
{
	float dx1 = line1End.x - line1Begin.x;
	float dy1 = line1End.y - line1Begin.y;
	float dx2 = line2End.x - line2Begin.x;
	float dy2 = line2End.y - line2Begin.y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (Equal(dy1 / dx1, dy2 / dx2))
		return false;

	float x01 = line1Begin.x;
	float y01 = line1Begin.y;
	float x02 = line2Begin.x;
	float y02 = line2Begin.y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);

	if (u1 < 0 || u1 > 1 || u2 < 0 || u2 > 1)
		return false;

	return true;
}
//////////////////////////////////////////////////////////////////////////
// 检查内外
bool sign(float f)
{
	return f > 0;
}
bool checkRay(Point begin, Point end, const std::vector<Point>& polygon)
{
	float dx = end.x - begin.x;
	float dy = end.y - begin.y;
	for (auto& p : polygon)
	{
		bool check = false;
		float _dx = p.x - begin.x;
		float _dy = p.y - begin.y;
		if (dx)
		{
			if (_dx)
			{
				if (dy / dx != _dy / _dx)
				{
					check = true;
				}
				else
				{
					if (sign(dx) == sign(_dx) && sign(dy) == sign(_dy))
						check = false;
					else
						check = true;
				}
			}
			else
			{
				check = true;
			}
		}
		else
		{
			if (_dx)
			{
				check = true;
			}
			else
			{
				if (sign(dx) == sign(_dx) && sign(dy) == sign(_dy))
					check = false;
				else
					check = true;
			}
		}
		if (!check)
			return false;
	}
	return true;
}
void boundBox(const std::vector<Point>& polygon, float& minX, float& maxX, float& minY, float& maxY)
{
	maxX = polygon[0].x, minX = polygon[0].x, maxY = polygon[0].y, minY = polygon[0].y;
	for (int i = 1; i < polygon.size(); i++)
	{
		if (polygon[i].x < minX)
			minX = polygon[i].x;
		if (polygon[i].x > maxX)
			maxX = polygon[i].x;
		if (polygon[i].y < minY)
			minY = polygon[i].y;
		if (polygon[i].y > maxY)
			maxY = polygon[i].y;
	}
}
bool checkEdge(Point p, const std::vector<Point>& polygon)
{
	for (int i = 0; i < polygon.size(); i++)
	{
		int next = i + 1 < polygon.size() ? i + 1 : 0;

		float dx1 = p.x - polygon[i].x;
		float dy1 = p.y - polygon[i].y;
		float dx2 = polygon[next].x - polygon[i].x;
		float dy2 = polygon[next].y - polygon[i].y;

		if (dx1 == 0 && dy1 == 0)
			return true;

		bool isParallel = false;
		if (dx1 == 0 && dx2 == 0)
		{
			isParallel = true;
		}
		else if (dx1 == 0 || dx2 == 0)
		{
			isParallel = false;
		}
		else
		{
			isParallel = dy1 / dx1 == dy2 / dx2;
		}


		if (isParallel)
		{
			float u = -1;
			if (dx1 == 0)
			{
				u = dy1 / dy2;
			}
			else
			{
				u = dx1 / dx2;
			}
			if (u >= 0 && u <= 1)
				return true;
		}
	}
	return false;
}
bool checkIn(Point p, const std::vector<Point>& polygon)
{
	if (checkEdge(p, polygon))
		return true;

	float minX, maxX, minY, maxY;
	boundBox(polygon, minX, maxX, minY, maxY);
	float length = (maxX - minX) + (maxY - minY);
	float theta = 0;
	float dtheta = PI / 180;
	while (theta < 2 * PI)
	{
		Point end = { p.x + length * cos(theta), p.y + length * sin(theta) };
		if (checkRay(p, end, polygon))
		{
			float u1 = 0, u2 = 0;
			int count = 0;
			for (int i = 0; i < polygon.size(); i++)
			{
				int next = i + 1 < polygon.size() ? i + 1 : 0;
				if (crossPoint(p, end, polygon[i], polygon[next], u1, u2))
				{
					if (crossProduct({ end.x - p.x, end.y - p.y }, 
					{ polygon[next].x - polygon[i].x, polygon[next].y - polygon[i].y }) > 0)
						count++;
					else
						count--;
				}
			}
			return count >= 1;
		}
		else
		{
			theta += dtheta;
		}
	}
	assert(0 && "can not find suitable ray!!!");
	return false;
}
//////////////////////////////////////////////////////////////////////////
enum class CrossPointType
{
	None,   // 无
	Enter, // 进入
	Exit, // 离开
};
struct CrossPointInfo
{
	float u1; // 直线的u
	float u2; // 边的u
	int edgeBeginIdx; 
	int edgeEndIdx;
	CrossPointType type;
};
bool _crossPoint(Point line1Begin, Point line1End, Point line2Begin, Point line2End, float& u1, float& u2)
{
	float dx1 = line1End.x - line1Begin.x;
	float dy1 = line1End.y - line1Begin.y;
	float dx2 = line2End.x - line2Begin.x;
	float dy2 = line2End.y - line2Begin.y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (Equal(dy1 / dx1, dy2 / dx2))
		return false;

	float x01 = line1Begin.x;
	float y01 = line1Begin.y;
	float x02 = line2Begin.x;
	float y02 = line2Begin.y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);

	return true;
}
void calcCrossPoint(const std::vector<Point>& polygon, Point p1, Point p2, std::vector<CrossPointInfo>& crossPoints)
{
	for (int i = 0; i < polygon.size(); i ++)
	{
		int next = i + 1 < polygon.size() ? i + 1 : 0;
		float u1, u2;
		if (_crossPoint(p1, p2, polygon[i], polygon[next], u1, u2) && (u2 >= 0.f && u2 <= 1.f) )
		{
			if (crossProduct({ polygon[next].x - polygon[i].x, polygon[next].y - polygon[i].y },
			{ p2.x - p1.x, p2.y - p1.y }) > 0)
			{
				crossPoints.push_back({ u1, u2, i, next, CrossPointType::Enter});
			}
			else
			{
				crossPoints.push_back({ u1, u2, i, next, CrossPointType::Exit });
			}
		}
	}
}
void swapCrossPointInfo(CrossPointInfo& a, CrossPointInfo& b)
{
	auto temp = a;
	a = b;
	b = temp;
}
void checkCross(const std::vector<Point>& polygon, Point p1, Point p2, std::vector<CrossPointInfo>& crossPoints)
{
	std::sort(crossPoints.begin(), crossPoints.end(), [](CrossPointInfo& a, CrossPointInfo& b)
	{
		return a.u1 < b.u1;
	});

	bool updateIt = false;
	auto last = crossPoints.end();
	for (auto it = crossPoints.begin(); it < crossPoints.end();)
	{
		updateIt = false;
		if (last < crossPoints.end())
		{
			// 过交点时，先确定进出关系
			if (it->u1 == last->u1 && it->type != last->type)
			{
				CrossPointType lltype = CrossPointType::None;
				if (last > crossPoints.begin())
				{
					auto llast = last - 1;
					lltype = llast->type;
				}
				else
				{
					lltype = CrossPointType::Exit;
				}

				if (lltype == CrossPointType::Enter)
				{
					if (last->type == CrossPointType::Enter)
					{
						swapCrossPointInfo(*last, *it);
					}
				}
				else
				{
					if (last->type == CrossPointType::Exit)
					{
						swapCrossPointInfo(*last, *it);
					}
				}
			}

			if ((it->u2 == 0.f || it->u2 == 1.f) && (last->u2 == 0.f || last->u2 == 1.f))
			{
				auto lastCrossIdx = last->u2 == 0.f ? last->edgeBeginIdx : last->edgeEndIdx;
				auto itCrossIdx = it->u2 == 0.f ? it->edgeBeginIdx : it->edgeEndIdx;
				if (itCrossIdx == lastCrossIdx ||
					((itCrossIdx - 1) + polygon.size()) % polygon.size() == lastCrossIdx ||
					((itCrossIdx + 1) + polygon.size()) % polygon.size() == lastCrossIdx)
				{
					if (it->type == last->type)
					{
						if (it->type == CrossPointType::Enter)
						{
							it = crossPoints.erase(it);
							updateIt = true;
						}
						else
						{
							auto llast = crossPoints.end(); 
							if (last > crossPoints.begin())
							{
								llast = last - 1;
							}

							it = crossPoints.erase(last);

							last = llast;
						}
					}
					else
					{
						if (last->type == CrossPointType::Exit && it->type == CrossPointType::Enter)
						{
							auto llast = crossPoints.end();
							if (last > crossPoints.begin())
							{
								llast = last - 1;
							}

							it = crossPoints.erase(last);
							it = crossPoints.erase(it);

							last = llast;

							updateIt = true;
						}
					}
				}
			}
		}		

		if (!updateIt)
		{
			last = it;
			it++;
		}			
	}

	for (auto it = crossPoints.begin(); it < crossPoints.end();)
	{
		if (it->u1 < 0 || it->u1 > 1)
		{
			it = crossPoints.erase(it);
		}
		else
		{
			it++;
		}
	}

	auto beginType = CrossPointType::None;

	bool and = true;
	if (crossPoints.size() > 0)
	{
		if (crossPoints[0].u1 == 0)
		{
			and = false;
		}
		else
		{
			beginType = crossPoints[0].type == CrossPointType::Enter ? CrossPointType::Exit : CrossPointType::Enter;
		}
	}
	else
	{
		beginType = checkIn(p1, polygon) ? CrossPointType::Enter : CrossPointType::Exit;
	}
	if(and)
		crossPoints.insert(crossPoints.begin(), { 0, -1, -1, -1, beginType });

	auto end = crossPoints.end() - 1;
	and = end->u1 < 1;
	if(and)
		crossPoints.push_back({ 1, -1, -1, -1, end->type == CrossPointType::Enter ? CrossPointType::Exit : CrossPointType::Enter });
}

void lineClipCrossPoint(const std::vector<Point>& polygon, Point p1, Point p2)
{
	std::vector<CrossPointInfo> crossPoints;
	calcCrossPoint(polygon, p1, p2, crossPoints);
	checkCross(polygon, p1, p2, crossPoints);

	float dx = p2.x - p1.x, dy = p2.y - p1.y;
	std::vector<std::vector<Point>> result;
	for (int i = 0; i < crossPoints.size(); i++)
	{
		if (crossPoints[i].type == CrossPointType::Exit)
		{
			int last = i - 1;
			if (last >= 0 && crossPoints[last].type == CrossPointType::Enter && crossPoints[i].u1 != crossPoints[last].u1)
			{
				result.push_back({ { p1.x + crossPoints[last].u1 * dx, p1.y + crossPoints[last].u1 * dy },
				{ p1.x + crossPoints[i].u1 * dx, p1.y + crossPoints[i].u1 * dy } });
			}
		}
	}

	for (auto line : result)
	{
		//lineBres(Round(p1.getx()), Round(p1.gety()), Round(p2.getx()), Round(p2.gety())); // 精确到浮点数绘图
		lineBres(line[0].x, line[0].y, line[1].x, line[1].y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<Point> polygon;
	Point p1, p2;

	glViewport(0, winHeight / 2, winWidth / 2, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	polygon = { { 177, 86 },{ 272, 42 },{ 361, 260 },{ 183, 149 }, {126, 216},{88, 62} };
	drawPolygonLine(polygon);

	p1 = { 88, 199 }, p2 = { 374, 227 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 135, 190 }, p2 = { 296, 172 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 98, 180 }, p2 = { 246, 155 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 195, 176 }, p2 = { 212, 260 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 257, 27 }, p2 = { 136, 284 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 216, 127 }, p2 = { 263, 92 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 90, 168 }, p2 = { 171, 50 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 106, 77 }, p2 = { 319, 60 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	glViewport(winWidth / 2, winHeight / 2, winWidth / 2, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	polygon = { { 80, 100 },{ 290, 100 },{ 290, 220 },{ 80, 220 }};
	drawPolygonLine(polygon);

	p1 = { 305, 23 }, p2 = { 60, 285 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 209, 75 }, p2 = { 350, 149 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 207, 187 }, p2 = { 381, 157 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 113, 35 }, p2 = { 229, 61 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 103, 259 }, p2 = { 103, 68 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 10, 30 }, p2 = { 260, 280 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 240, 170 }, p2 = { 350, 280 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	
	glViewport(0, 0, winWidth / 2, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	polygon = { { 44, 43 },{ 353, 70 },{ 290, 213 },{ 248, 162 },{226, 278},{175, 100},{146, 227},{106, 140},{77, 195} };
	drawPolygonLine(polygon);

	p1 = { 40, 140 }, p2 = { 179, 140 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 140, 100 }, p2 = { 202, 100 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 127, 227 }, p2 = { 260, 227 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 193, 213 }, p2 = { 318, 213 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	glViewport(winWidth / 2, 0, winWidth / 2, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	polygon = { { 40, 40 },{ 330, 40 },{ 330, 213 },{ 296, 213 },{ 296, 106 },{ 266, 106 },{ 266, 243 },{ 230, 243 },{ 230, 137 },{205, 137},{205, 82},{150, 82},{150, 282},{105, 282},{105, 183},{40, 183} };
	drawPolygonLine(polygon);

	p1 = { 10, 183 }, p2 = { 194, 183 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 126, 282 }, p2 = { 85, 282 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 128, 82 }, p2 = { 223, 82 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 222, 137 }, p2 = { 216, 137 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 135, 243 }, p2 = { 278, 243 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 135, 106 }, p2 = { 278, 106 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 316, 213 }, p2 = { 197, 213 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 19, 40 }, p2 = { 66, 40 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 337, 40 }, p2 = { 306, 40 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 181, 40 }, p2 = { 215, 40 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 296, 213 }, p2 = { 266, 106 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 120, 52 }, p2 = { 240, 172 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 40, 183 }, p2 = { 105, 282 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);
			
	p1 = { 150, 282 }, p2 = { 40, 40 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	p1 = { 296, 106 }, p2 = { 330, 213 };
	glColor3f(1.0, 1.0, 1.0);
	lineBres(p1.x, p1.y, p2.x, p2.y);
	glColor3f(1.0, 0.0, 0.0);
	lineClipCrossPoint(polygon, p1, p2);

	glFlush();
}
void code_8_exercise_add_1_4()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth / 2, 0, winHeight / 2);

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

#ifdef CHAPTER_8_EXERCISE_4_1
	code_8_exercise_4_1();
#endif

#ifdef CHAPTER_8_EXERCISE_6
	code_8_exercise_6();
#endif

#ifdef CHAPTER_8_EXERCISE_8
	code_8_exercise_8();
#endif

#ifdef CHAPTER_8_EXERCISE_10
	code_8_exercise_10();
#endif

#ifdef CHAPTER_8_EXERCISE_11
	code_8_exercise_11();
#endif

#ifdef CHAPTER_8_EXERCISE_12
	code_8_exercise_12();
#endif

#ifdef CHAPTER_8_EXERCISE_13
	code_8_exercise_13();
#endif

#ifdef CHAPTER_8_EXERCISE_14
	code_8_exercise_14();
#endif

#ifdef CHAPTER_8_EXERCISE_15
	code_8_exercise_15();
#endif

#ifdef CHAPTER_8_EXERCISE_16
	code_8_exercise_16();
#endif

#ifdef CHAPTER_8_EXERCISE_16_1
	code_8_exercise_16_1();
#endif

#ifdef CHAPTER_8_EXERCISE_20
	code_8_exercise_20();
#endif

#ifdef CHAPTER_8_EXERCISE_21
	code_8_exercise_21();
#endif

#ifdef CHAPTER_8_EXERCISE_22
	code_8_exercise_22();
#endif

#ifdef CHAPTER_8_EXERCISE_23
	code_8_exercise_23();
#endif

#ifdef CHAPTER_8_EXERCISE_24
	code_8_exercise_24();
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1
	code_8_exercise_add_1();
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_1
	code_8_exercise_add_1_1();
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_2
	code_8_exercise_add_1_2();
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_3
	code_8_exercise_add_1_3();
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_4
	code_8_exercise_add_1_4();
#endif

	glutMainLoop();
}
#endif
#endif 


