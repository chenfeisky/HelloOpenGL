#include "common.h"

#ifdef CHAPTER_6
#include "opengl6h.h"

#ifdef CHAPTER_6_COMMON
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


#ifdef CHAPTER_6_1_2
inline int Round(const float a){ return int(a + 0.5); }
void lineDDA(int x0, int y0, int xEnd, int yEnd)
{
	int dx = xEnd - x0, dy = yEnd - y0, steps, k;
	float xIncrement, yIncrement, x = x0, y = y0;
	if (fabs((float)dx) > fabs((float)dy))
		steps = fabs((float)dx);
	else
		steps = fabs((float)dy);
	xIncrement = float(dx) / float(steps);
	yIncrement = float(dy) / float(steps);
	setPixel(Round(x), Round(y));
	for (k = 0; k < steps; k++)
	{
		x += xIncrement;
		y += yIncrement;
		setPixel(Round(x), Round(y));
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	lineDDA(200, 100, 300, 180);
	glFlush();
}
void code_6_1_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_1_3
// Bresenham line-drawing procedure for |m| < 1.0
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
	int x, y;
	if (x0 > xEnd)
	{
		x = xEnd;
		y = yEnd;
		xEnd = x0;
	}
	else
	{
		x = x0;
		y = y0;
	}
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	lineBres(20, 10, 30, 18);
	glFlush();	
}
void code_6_1_3()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_4_2
class screenPt
{
private:
	GLint x, y;
public:
	screenPt()
	{
		x = y = 0;
	}
	void setCoords(GLint xCoordValue, GLint yCoordValue)
	{
		x = xCoordValue;
		y = yCoordValue;
	}
	GLint getx() const
	{
		return x;
	}
	GLint gety() const
	{
		return y;
	}
	void incrementx()
	{
		x++;
	}
	void decrementy()
	{
		y--;
	}
};
void circleMidpoint(GLint xc, GLint yc, GLint radius)
{
	screenPt circPt;
	GLint p = 1 - radius;
	circPt.setCoords(0, radius);
	void circlePlotPoints(GLint, GLint, screenPt);
	circlePlotPoints(xc, yc, circPt);
	while (circPt.getx() < circPt.gety())
	{
		circPt.incrementx();
		if (p < 0)
			p += 2 * circPt.getx() + 1;
		else
		{
			circPt.decrementy();
			p += 2 * (circPt.getx() - circPt.gety()) + 1;
		}
		circlePlotPoints(xc, yc, circPt);
	}	
}
void circlePlotPoints(GLint xc, GLint yc, screenPt circPt)
{
	setPixel(xc + circPt.getx(), yc + circPt.gety());
	setPixel(xc - circPt.getx(), yc + circPt.gety());
	setPixel(xc + circPt.getx(), yc - circPt.gety());
	setPixel(xc - circPt.getx(), yc - circPt.gety());
	setPixel(xc + circPt.gety(), yc + circPt.getx());
	setPixel(xc - circPt.gety(), yc + circPt.getx());
	setPixel(xc + circPt.gety(), yc - circPt.getx());
	setPixel(xc - circPt.gety(), yc - circPt.getx());
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	circleMidpoint(100, 100, 50);
	glFlush();
}
void code_6_4_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_Test_1
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dx - 2 * dy;
	int twoDy = -2 * dy, twoDxMinusDy = 2 * (dx - dy);
	int x, y;
	if (x0 > xEnd)
	{
		x = xEnd;
		y = yEnd;
		xEnd = x0;
	}
	else
	{
		x = x0;
		y = y0;
	}
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p > 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDxMinusDy;
		}
		setPixel(x, y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	lineBres(200, 100, 300, 180);
	glFlush();
}
void code_6_test_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_5_2
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void ellipseMidpoint(int xCenter, int yCenter, int Rx, int Ry)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2*y;
	void ellipsePlotPoints(int, int, int, int);
	ellipsePlotPoints(xCenter, yCenter, x, y);
	/*Region 1*/
	p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	int debugIndex = 0;
	printf("Region 1\n");
	while (px < py)
	{
		printf("k=%d,", debugIndex);
		printf("p=%d,", p);
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		printf("x=%d,y=%d,", x, y);
		printf("px=%d,py=%d\n,", px, py);
		ellipsePlotPoints(xCenter, yCenter, x, y);
		debugIndex++;
	}
	/*Region 2*/
	debugIndex = 0;
	p = Round(Ry2*(x + 0.5)*(x + 0.5) + Rx2*(y - 1)*(y - 1) - Rx2*Ry2);
	printf("Region 2\n");
	while (y>0)
	{
		printf("k=%d,", debugIndex);
		printf("p=%d,", p);
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
		printf("x=%d,y=%d,", x, y);
		printf("px=%d,py=%d\n,", px, py);
		ellipsePlotPoints(xCenter, yCenter, x, y);
		debugIndex++;
	}
}
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixel(xCenter + x, yCenter + y);
	setPixel(xCenter - x, yCenter + y);
	setPixel(xCenter + x, yCenter - y);
	setPixel(xCenter - x, yCenter - y);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	ellipseMidpoint(300, 300, 80, 60);
	glFlush();
}
void code_6_5_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_1
typedef struct { int x, y; } Point2;
vector<Point2> points = { { 160, 80 }, { 638, 236 }, { 654, 422 }, { 378, 482 }, { 337, 542 } };

inline int Round(const float a){ return int(a + 0.5); }
void lineDDA(int x0, int y0, int xEnd, int yEnd)
{
	int dx = xEnd - x0, dy = yEnd - y0, steps, k;
	float xIncrement, yIncrement, x = x0, y = y0;
	if (fabs((float)dx) > fabs((float)dy))
		steps = fabs((float)dx);
	else
		steps = fabs((float)dy);
	xIncrement = float(dx) / float(steps);
	yIncrement = float(dy) / float(steps);
	setPixel(Round(x), Round(y));
	for (k = 0; k < steps; k++)
	{
		x += xIncrement;
		y += yIncrement;
		setPixel(Round(x), Round(y));
	}
}
void drawLines()
{
	if (points.size() == 1)
	{
		setPixel(points[0].x, points[0].y);
	}
	else
	{
		for (int i = 0; i < points.size() - 1; i++)
		{
			lineDDA(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawLines();
	glFlush();
}
void code_6_exercise_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_2
typedef struct { int x, y; } Point2;

// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{	
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	lineBres(53, 95, 706, 95);		  // 水平线
	lineBres(495, 25, 495, 556);	  // 垂直线
	lineBres(3, 3, 600, 600);         // 45度斜线（m=1）
	lineBres(25, 575, 500, 100);      // 45度斜线（m=-1)

	lineBres(172, 134, 525, 243);	  // 0<m<1
	lineBres(222, 95, 521, 549);	  // m>1
	lineBres(135, 300, 733, 139);	  // -1<m<0
	lineBres(264, 487, 447, 47);	  // m<-1

	glFlush();
}
void code_6_exercise_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_3
typedef struct { int x, y; } Point2;
// 0<m<1(m>1)
// XYmirror: x,y坐标交换（沿y=x直线对称变换）
// starFormNext: 从下一个点开始绘制
void lineBres1(int x0, int y0, int xEnd, int yEnd, bool XYmirror = false, bool starFormNext = false)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;

	if (!starFormNext)
	{
		if (XYmirror)
			setPixel(y, x);
		else
			setPixel(x, y);
	}

	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}

		if (XYmirror)
			setPixel(y, x);
		else
			setPixel(x, y);
	}
}
// -1<m<0(m<-1)
// XYmirror: x,y坐标交换（沿y=x直线对称变换）
// starFormNext: 从下一个点开始绘制
void lineBres2(int x0, int y0, int xEnd, int yEnd, bool XYmirror = false, bool starFormNext = false)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (!starFormNext)
	{
		if (XYmirror)
			setPixel(y, x);
		else
			setPixel(x, y);
	}

	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}

		if (XYmirror)
			setPixel(y, x);
		else
			setPixel(x, y);
	}
}
// starFormNext: 从下一个点开始绘制
void lineBres(int x0, int y0, int xEnd, int yEnd, bool starFormNext = false)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1(y0, x0, yEnd, xEnd, true, starFormNext);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, false, starFormNext);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2(y0, x0, yEnd, xEnd, true, starFormNext);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, false, starFormNext);
		}
	}
}
void drawPolyline()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

	// 没有处理端点的折线
	glColor3f(1.0, 0.0, 0.0);
	lineBres(120, 417, 372, 464);
	glColor3f(0.0, 1.0, 0.0);
	lineBres(372, 464, 626, 393);
	glColor3f(0.0, 0.0, 1.0);
	lineBres(626, 393, 715, 466);

	// 处理端点的折线（避免重复绘制端点）
	glColor3f(1.0, 0.0, 0.0);
	lineBres(120, 114, 347, 209);
	glColor3f(0.0, 1.0, 0.0);
	lineBres(347, 209, 543, 107, true); // 此处使用|m|<1的对称算法，y步进算法在上一题中给出,两者差别不大，只有并行算法中有区别，参见第7题
	glColor3f(0.0, 0.0, 1.0);
	lineBres(543, 107, 668, 254, true);

	glFlush();
}
void code_6_exercise_3()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutDisplayFunc(drawPolyline);
}
#endif

#ifdef CHAPTER_6_EXERCISE_4
// 0<m<1
void lineBresMid(int x0, int y0, int xEnd, int yEnd)
{
	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dx - 2 * dy;
	int twoDy = 2 * dy, twoDxMinusDy = 2 * (dx - dy);
	int x, y;
	if (x0 > xEnd)
	{
		x = xEnd;
		y = yEnd;
		xEnd = x0;
	}
	else
	{
		x = x0;
		y = y0;
	}
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p > 0)
			p -= twoDy;
		else
		{
			y++;
			p += twoDxMinusDy;
		}
		setPixel(x, y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	
	lineBresMid(144,148,638,235);
	glFlush();
}
void code_6_exercise_4()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_5
// 0<m<1(m>1)
// XYmirror: x,y坐标交换（沿y=x直线对称变换）
void lineBresMid1(int x0, int y0, int xEnd, int yEnd, bool XYmirror = false)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dx - 2 * dy;
	int twoDy = 2 * dy, twoDxMinusDy = 2 * (dx - dy);
	int x = x0;
	int y = y0;

	if (XYmirror)
		setPixel(y, x);
	else
		setPixel(x, y);

	while (x < xEnd)
	{
		x++;
		if (p > 0)
			p -= twoDy;
		else
		{
			y++;
			p += twoDxMinusDy;
		}
		if (XYmirror)
			setPixel(y, x);
		else
			setPixel(x, y);
	}
}
// -1<m<0(m<-1)
// XYmirror: x,y坐标交换（沿y=x直线对称变换）
void lineBresMid2(int x0, int y0, int xEnd, int yEnd, bool XYmirror = false)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = xEnd - x0, dy = yEnd - y0;
	int p = - dx - 2 * dy;
	int twoDy = 2 * dy, twoDxMinusDy = 2 * ( - dx - dy);
	int x = x0;
	int y = y0;

	if (XYmirror)
		setPixel(y, x);
	else
		setPixel(x, y);

	while (x < xEnd)
	{
		x++;
		if (p <= 0)
			p -= twoDy;
		else
		{
			y--;
			p += twoDxMinusDy;
		}
		if (XYmirror)
			setPixel(y, x);
		else
			setPixel(x, y);
	}
}
void lineBresMid(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBresMid1(y0, x0, yEnd, xEnd, true);
		}
		else
		{
			lineBresMid1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBresMid2(y0, x0, yEnd, xEnd, true);
		}
		else
		{
			lineBresMid2(x0, y0, xEnd, yEnd);
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	lineBresMid(75, 289, 766, 289);	  // 水平线
	lineBresMid(376, 577, 376, 52);   // 垂直线
	lineBresMid(5, 5, 500, 500);      // 45度斜线（m=1）
	lineBresMid(25, 575, 500, 100);   // 45度斜线（m=-1）

	lineBresMid(144, 148, 638, 235);  // 0<m<1
	lineBresMid(214, 90, 547, 492);   // m>1
	lineBresMid(129, 534, 677, 445);  // -1<m<0
	lineBresMid(585, 563, 755, 47);   // m<-1
	glFlush();
}
void code_6_exercise_5()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_6
int nThread = 4; // 四个处理线程
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5); 
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}

void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}
void ThreadFunc(int x0, int y0, int xEnd, int yEnd)
{
	lineBres(x0, y0, xEnd, yEnd);
}
void lineParallelBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;

	double m = (double)dy / dx;

	int dxp = (dx + nThread - 1) / nThread;
	int dyp = Round(m * dxp);
	int dypEnd = Round(m * (dxp - 1));

	for (int i = 0; i < nThread; i++)
	{
		int threadX0 = x0 + i * dxp;
		int threadY0 = y0 + i * dyp;
		int threadXEnd = xEnd;
		int threadYEnd = yEnd;
		if (i != nThread - 1)
		{
			threadXEnd = threadX0 + dxp - 1;
			threadYEnd = threadY0 + dypEnd;
		}
		ThreadFunc(threadX0, threadY0, threadXEnd, threadYEnd);
	}
	
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	lineParallelBres(144, 148, 638, 235);
	glFlush();
}
void code_6_exercise_6()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_7
int nThread = 4; // 四个处理线程
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
// 0<m<1(m>1)
//void lineBres1(int x0, int y0, int xEnd, int yEnd, bool XYmirror = false)
//{
//	if (x0 > xEnd)
//	{
//		int tempx = x0;
//		int tempy = y0;
//		x0 = xEnd;
//		y0 = yEnd;
//		xEnd = tempx;
//		yEnd = tempy;
//	}
//
//	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
//	int p = 2 * dy - dx;
//	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
//
//	int x = x0;
//	int y = y0;
//
//	if (XYmirror)
//		setPixel(y, x);
//	else
//		setPixel(x, y);
//
//	while (x < xEnd)
//	{
//		x++;
//		if (p < 0)
//			p += twoDy;
//		else
//		{
//			y++;
//			p += twoDyMinusDx;
//		}
//
//		if (XYmirror)
//			setPixel(y, x);
//		else
//			setPixel(x, y);
//	}
//}
////-1<m<0(m<-1)
//void lineBres2(int x0, int y0, int xEnd, int yEnd, bool XYmirror = false)
//{
//	if (x0 > xEnd)
//	{
//		int tempx = x0;
//		int tempy = y0;
//		x0 = xEnd;
//		y0 = yEnd;
//		xEnd = tempx;
//		yEnd = tempy;
//	}
//
//	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
//	int p = 2 * dy + dx;
//	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);
//
//	int x = x0;
//	int y = y0;
//
//	if (XYmirror)
//		setPixel(y, x);
//	else
//		setPixel(x, y);
//
//	while (x < xEnd)
//	{
//		x++;
//		if (p >= 0)
//			p += twoDy;
//		else
//		{
//			y--;
//			p += twoDyAddDx;
//		}
//
//		if (XYmirror)
//			setPixel(y, x);
//		else
//			setPixel(x, y);
//	}
//}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}

void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{

			//lineBres2(y0, x0, yEnd ,xEnd, true); // 对称算法版本，和y步进算法相比，对称算法差别不大，主要是x和y方向上不同的误差造成的
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}

}
void ThreadFunc(int x0, int y0, int xEnd, int yEnd)
{
	lineBres(x0, y0, xEnd, yEnd);
}
void lineParallelBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;

	double m = 0;
	if (dx != 0)
		m = (double)dy / dx;

	if (fabs((float)dy) <= fabs((float)dx))
	{
		int dxp = (dx + nThread - 1) / nThread;
		int dyp = Round(m * dxp);
		int dypEnd = Round(m * (dxp - 1));

		for (int i = 0; i < nThread; i++)
		{
			int threadX0 = x0 + i * dxp;
			int threadY0 = y0 + i * dyp;
			int threadXEnd = xEnd;
			int threadYEnd = yEnd;
			if (i != nThread - 1)
			{
				threadXEnd = threadX0 + dxp - 1;
				threadYEnd = threadY0 + dypEnd;
			}
			ThreadFunc(threadX0, threadY0, threadXEnd, threadYEnd);
		}
	}
	else
	{
		int sign = 0;
		if (dy >= 0)
			sign = 1;
		else
			sign = -1;

		int dxp, dyp, dxpEnd;
		if (dx != 0)
		{
			dyp = (dy + sign*(nThread - 1)) / nThread;
			dxp = Round(dyp / m);
			dxpEnd = Round((dyp - sign * 1) / m);
		}
		else
		{
			dyp = (dy + sign*(nThread - 1)) / nThread;
			dxp = 0;
			dxpEnd = 0;
		}

		for (int i = 0; i < nThread; i++)
		{
			int threadX0 = x0 + i * dxp;
			int threadY0 = y0 + i * dyp;
			int threadXEnd = xEnd;
			int threadYEnd = yEnd;
			if (i != nThread - 1)
			{
				threadXEnd = threadX0 + dxpEnd;
				threadYEnd = threadY0 + dyp - sign * 1;
			}
			ThreadFunc(threadX0, threadY0, threadXEnd, threadYEnd);
		}
	}

}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	lineParallelBres(75, 289, 766, 289);   // 水平线
	lineParallelBres(376, 577, 376, 52);   // 垂直线
	lineParallelBres(5, 5, 500, 500);      // 45度斜线（m=1）
	lineParallelBres(25, 575, 500, 100);   // 45度斜线（m=-1）

	lineParallelBres(144, 148, 638, 235);  // 0<m<1
	lineParallelBres(214, 90, 547, 492);   // m>1
	lineParallelBres(129, 534, 677, 445);  // -1<m<0
	lineParallelBres(585, 563, 755, 47);   // m<-1
	
	glFlush();
}
void code_6_exercise_7()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_7_Test_1
int nThread = 4; // 四个处理线程
struct Rect
{
	int x, y, w, h;
};
void threadFunc(double A, double B, double C, Rect rt)
{
	for (int x = 0; x < rt.w; x++)
	{
		int _x = x + rt.x;
		for (int y = 0; y < rt.h; y++)
		{			
			int _y = y + rt.y;
			double d = A*_x + B*_y + C;
			if (std::fabs(d) < 0.5)
			{
				setPixel(_x, _y);
			}
		}
	}
}
void lineDistance(int x0, int y0, int xEnd, int yEnd)
{
	if (nThread % 2 != 0) return;
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	double distance = sqrt(dx*dx + dy*dy);
	double A = -dy / distance;
	double B = dx / distance;
	double C = (x0*dy-y0*dx) / distance;

	int xp = x0 < xEnd ? x0 : xEnd;
	int yp = y0 < yEnd ? y0 : yEnd;
	int width = fabs((float)(xEnd - x0));
	int height = fabs((float)(yEnd - y0));
	

	if (height == 0)
	{
		int x = xp;
		for (int i = 0; i < nThread ; i++)
		{
			threadFunc(A, B, C, { x, yp, width / nThread, 1 });
			x += width / nThread;
		}
	}
	else if (width == 0)
	{
		int y = yp;
		for (int i = 0; i < nThread; i++)
		{
			threadFunc(A, B, C, { xp, y, 1, height / nThread });
			y += height / nThread;
		}
	}
	else
	{
		int x = xp;
		int w = width / (nThread / 2);
		for (int i = 0; i < nThread / 2; i++)
		{
			threadFunc(A, B, C, { x, yp, w, height / 2 });
			x += w;
		}
		x = xp;
		for (int i = 0; i < nThread / 2; i++)
		{
			threadFunc(A, B, C, { x, yp + height / 2, w, height / 2 });
			x += w;
		}
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	lineDistance(75, 289, 766, 289);   // 水平线
	lineDistance(376, 577, 376, 52);   // 垂直线
	lineDistance(5, 5, 500, 500);      // 45度斜线（m=1）
	lineDistance(25, 575, 500, 100);   // 45度斜线（m=-1）

	lineDistance(144, 148, 638, 235);  // 0<m<1
	lineDistance(214, 90, 547, 492);   // m>1
	lineDistance(129, 534, 677, 445);  // -1<m<0
	lineDistance(585, 563, 755, 47);   // m<-1

	glFlush();
}
void code_6_exercise_7_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_7_Test_2
int nThread = 4; // 四个处理线程
int calcLineX(int x0, int y0, int xEnd, int yEnd, int y)
{
	return (double)((xEnd - x0)*(y - y0)) / (yEnd - y0) + x0;
}
int calcLineY(int x0, int y0, int xEnd, int yEnd, int x)
{
	return (double)((yEnd - y0)*(x - x0)) / (xEnd - x0) + y0;
}
void threadFunc1(int x0, int y0, int xEnd, int yEnd, int _x0, int _xEnd)
{
	for (int x = _x0; x <= _xEnd; x++)
	{
		int y = calcLineY(x0, y0, xEnd, yEnd, x);
		setPixel(x, y);
	}
}
void threadFunc2(int x0, int y0, int xEnd, int yEnd, int _y0, int _yEnd)
{
	for (int y = _y0; y <= _yEnd; y++)
	{
		int x = calcLineX(x0, y0, xEnd, yEnd, y);
		setPixel(x, y);
	}
}
void lineScan(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	bool xStep = true;
	if (dy < 0)
	{
		//m < -1，从右向左
		if (std::abs(dx) < std::abs(dy))
		{
			xStep = false;
			int tempx = x0;
			int tempy = y0;
			x0 = xEnd;
			y0 = yEnd;
			xEnd = tempx;
			yEnd = tempy;	
		}
	}
	else
	{
		if (std::abs(dx) < std::abs(dy))
			xStep = false;
	}
	if (xStep)
	{
		int dxp = (std::abs(dx) + nThread - 1) / nThread;
		for (int i = 0; i < nThread; i++)
		{
			int _x0 = x0 + i * dxp;
			int _xEnd = x0 + (i + 1) * dxp - 1;
			if (i == nThread - 1)
				_xEnd = xEnd;
			threadFunc1(x0, y0, xEnd, yEnd, _x0, _xEnd);
		}
	}
	else
	{
		int dyp = (std::abs(dy) + nThread - 1) / nThread;
		for (int i = 0; i < nThread; i++)
		{
			int _y0 = y0 + i * dyp;
			int _yEnd = y0 + (i + 1) * dyp - 1;
			if (i == nThread - 1)
				_yEnd = yEnd;
			threadFunc2(x0, y0, xEnd, yEnd, _y0, _yEnd);
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	lineScan(75, 289, 766, 289);   // 水平线
	lineScan(376, 577, 376, 52);   // 垂直线
	lineScan(5, 5, 500, 500);      // 45度斜线（m=1）
	lineScan(25, 575, 500, 100);   // 45度斜线（m=-1）

	lineScan(144, 148, 638, 235);  // 0<m<1
	lineScan(214, 90, 547, 492);   // m>1
	lineScan(129, 534, 677, 445);  // -1<m<0
	lineScan(585, 563, 755, 47);   // m<-1

	glFlush();
}
void code_6_exercise_7_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_8
void drawFunc()
{
}
void code_6_exercise_8()
{
	glutDisplayFunc(drawFunc);
	printf("(x, y)像素的帧缓存字节地址是:0 + y * 80 * 100 + x \n");
}
#endif

#ifdef CHAPTER_6_EXERCISE_9
void drawFunc()
{
}
void code_6_exercise_9()
{
	glutDisplayFunc(drawFunc);
	printf("(x, y)像素的帧缓存位地址是:0 + y * 120 * 120 + x \n");
}
#endif

#ifdef CHAPTER_6_EXERCISE_10
void drawFunc()
{
}
void code_6_exercise_10()
{
	glutDisplayFunc(drawFunc);
	printf("(x, y)像素的帧缓存位地址是:0 + y * 120 * 120 + x \n");
}
#endif

#ifdef CHAPTER_6_EXERCISE_11
int addr = 0;
void setPixel()
{
	int x = addr % winWidth;
	int y = addr / winWidth;
	
	setPixel(x, y);

	char a[16] = {};
	sprintf_s(a, "%d, %d\n",x, y);
	outPut(a);
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	addr = 0;
	int horizontalStep = 1;
	int slantingStep = winWidth + 1;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	addr = addr + y0 * winWidth + x0;
	setPixel();

	int i = x0;
	while (i < xEnd)
	{
		if (p < 0)
		{
			p += twoDy;

			addr = addr + horizontalStep;
			setPixel();
		}
			
		else
		{
			p += twoDyMinusDx;

			addr = addr + slantingStep;
			setPixel();
		}
		i++;
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	addr = 0;
	int verticalStep = winWidth;
	int slantingStep = winWidth + 1;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);

	addr = addr + y0 * winWidth + x0;
	setPixel();

	int i = y0;
	while (i < yEnd)
	{
		
		if (p > 0)
		{
			p += twoDx;
			addr = addr + verticalStep;
			setPixel();
		}			
		else
		{
			p += twoDyMinusDx;
			addr = addr + slantingStep;
			setPixel();
		}
		i++;
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	addr = 0;
	int horizontalStep = 1;
	int slantingStep = -winWidth + 1;

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	addr = addr + y0 * winWidth + x0;
	setPixel();
	int i = x0;
	while (i < xEnd)
	{
		if (p >= 0)
		{
			p += twoDy;
			addr = addr + horizontalStep;
			setPixel();
		}			
		else
		{
			p += twoDyAddDx;
			addr = addr + slantingStep;
			setPixel();
		}
		i++;
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	addr = 0;
	int verticalStep = -winWidth;
	int slantingStep = -winWidth + 1;

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	addr = addr + y0 * winWidth + x0;
	setPixel();

	int i = y0;
	while (i > yEnd)
	{
		if (p >= 0)
		{
			p += twoDx;
			addr = addr + verticalStep;
			setPixel();
		}	
		else
		{
			p += twoDyAddDx;
			addr = addr + slantingStep;
			setPixel();
		}
		i--;
	}
}
void lineBresByAddr(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	lineBresByAddr(75, 289, 766, 289);   // 水平线
	lineBresByAddr(376, 577, 376, 52);   // 垂直线
	lineBresByAddr(5, 5, 500, 500);      // 45度斜线（m=1）
	lineBresByAddr(25, 575, 500, 100);   // 45度斜线（m=-1）

	lineBresByAddr(144, 148, 638, 235);  // 0<m<1
	lineBresByAddr(214, 90, 547, 492);   // m>1
	lineBresByAddr(129, 534, 677, 445);  // -1<m<0
	lineBresByAddr(585, 563, 755, 47);   // m<-1

	glFlush();
}
void code_6_exercise_11()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_12
class screenPt
{
private:
	GLint x, y;
public:
	screenPt()
	{
		x = y = 0;
	}
	void setCoords(GLint xCoordValue, GLint yCoordValue)
	{
		x = xCoordValue;
		y = yCoordValue;
	}
	GLint getx() const
	{
		return x;
	}
	GLint gety() const
	{
		return y;
	}
	void incrementx()
	{
		x++;
	}
	void decrementy()
	{
		y--;
	}
};
void offsetPoint(int x, int y, int dx, int dy)
{
	setPixel(x + dx, y + dy);
}
void circlePlotPoints(GLint xc, GLint yc, screenPt circPt, bool offset)
{
	if (offset)
	{
		offsetPoint(xc + circPt.getx(), yc + circPt.gety(), 0, 0); // 0 - 45
		offsetPoint(xc - circPt.getx(), yc + circPt.gety(), 1, 0); // 315 - 0
		offsetPoint(xc + circPt.getx(), yc - circPt.gety(), 0, 1);// 135 - 180
		offsetPoint(xc - circPt.getx(), yc - circPt.gety(), 1, 1);// 180 - 225
		offsetPoint(xc + circPt.gety(), yc + circPt.getx(), 0, 0); // 45 - 90
		offsetPoint(xc - circPt.gety(), yc + circPt.getx(), 1, 0); // 270 - 315
		offsetPoint(xc + circPt.gety(), yc - circPt.getx(), 0, 1); // 90 - 135
		offsetPoint(xc - circPt.gety(), yc - circPt.getx(), 1, 1); // 225 - 270
	}
	else
	{
		offsetPoint(xc + circPt.getx(), yc + circPt.gety(), 0, 0); // 0 - 45
		offsetPoint(xc - circPt.getx(), yc + circPt.gety(), 0, 0); // 315 - 0
		offsetPoint(xc + circPt.getx(), yc - circPt.gety(), 0, 0);// 135 - 180
		offsetPoint(xc - circPt.getx(), yc - circPt.gety(), 0, 0);// 180 - 225
		offsetPoint(xc + circPt.gety(), yc + circPt.getx(), 0, 0); // 45 - 90
		offsetPoint(xc - circPt.gety(), yc + circPt.getx(), 0, 0); // 270 - 315
		offsetPoint(xc + circPt.gety(), yc - circPt.getx(), 0, 0); // 90 - 135
		offsetPoint(xc - circPt.gety(), yc - circPt.getx(), 0, 0); // 225 - 270
	}

}
void circleMid(GLint xc, GLint yc, GLint radius, bool offset)
{
	screenPt circPt;
	GLint p = 1 - radius;
	circPt.setCoords(0, radius);
	circlePlotPoints(xc, yc, circPt, offset);
	while (circPt.getx() < circPt.gety())
	{
		circPt.incrementx();
		if (p < 0)
			p += 2 * circPt.getx() + 1;
		else
		{
			circPt.decrementy();
			p += 2 * (circPt.getx() - circPt.gety()) + 1;
		}
		circlePlotPoints(xc, yc, circPt, offset);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// 白色圆没有几何补偿
	glColor3f(1.0, 1.0, 1.0);
	circleMid(100, 100, 50,false);

	// 红色圆有几何补偿
	glColor3f(1.0, 0.0, 0.0);
	circleMid(100, 100, 50,true);

	glFlush();
}
void code_6_exercise_12()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_13
int nThread = 4; // 四个处理线程
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
class screenPt
{
private:
	GLint x, y;
public:
	screenPt()
	{
		x = y = 0;
	}
	void setCoords(GLint xCoordValue, GLint yCoordValue)
	{
		x = xCoordValue;
		y = yCoordValue;
	}
	GLint getx() const
	{
		return x;
	}
	GLint gety() const
	{
		return y;
	}
	void incrementx()
	{
		x++;
	}
	void decrementy()
	{
		y--;
	}
};
void circlePlotPoints(GLint xc, GLint yc, screenPt circPt)
{
	setPixel(xc + circPt.getx(), yc + circPt.gety()); // 0 - 45
	setPixel(xc - circPt.getx(), yc + circPt.gety()); // 315 - 0
	setPixel(xc + circPt.getx(), yc - circPt.gety());// 135 - 180
	setPixel(xc - circPt.getx(), yc - circPt.gety());// 180 - 225
	setPixel(xc + circPt.gety(), yc + circPt.getx()); // 45 - 90
	setPixel(xc - circPt.gety(), yc + circPt.getx()); // 270 - 315
	setPixel(xc + circPt.gety(), yc - circPt.getx()); // 90 - 135
	setPixel(xc - circPt.gety(), yc - circPt.getx()); // 225 - 270
}
void threadFunc(int xc, int yc, double x, double y, int radius, int xEnd)
{
	screenPt circPt;
	GLint p = Round((x + 1)*(x + 1) + (y - 0.5) * (y - 0.5) - radius * radius);
	circPt.setCoords(Round(x), Round(y));
	circlePlotPoints(xc, yc, circPt);
	while (circPt.getx() < xEnd - 1)
	{
		circPt.incrementx();
		if (p < 0)
			p += 2 * circPt.getx() + 1;
		else
		{
			circPt.decrementy();
			p += 2 * (circPt.getx() - circPt.gety()) + 1;
		}
		circlePlotPoints(xc, yc, circPt);
	}
}
void circleParallelMid(GLint xc, GLint yc, GLint radius)
{
	double angle = PI / 4 / nThread;
	int X = Round(radius * std::sin(PI / 4));
	for (int i = 0; i < nThread; i++)
	{
		int xEnd = 0;
		if (radius * std::sin((i + 1) * angle) < X)
			xEnd = Round(radius * std::sin((i + 1) * angle));
		else
			xEnd = X + 1;
		threadFunc(xc, yc, radius * std::sin(i * angle), radius * std::cos(i * angle), radius, xEnd);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	circleParallelMid(100, 100, 50);

	glFlush();
}
void code_6_exercise_13()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_14
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void ellipseMidAntiClock(int xCenter, int yCenter, int Rx, int Ry)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = Rx;
	int y = 0;
	int px = twoRy2*x;
	int py = 0;
	void ellipsePlotPoints(int, int, int, int);
	ellipsePlotPoints(xCenter, yCenter, x, y);
	/*Region 1*/
	p = Round(Rx2 - (Ry2*Rx) + (0.25*Ry2));
	while (px > py)
	{
		y++;
		py += twoRx2;
		if (p < 0)
			p += Rx2 + py;
		else
		{
			x--;
			px -= twoRy2;
			p += Rx2 + py - px;
		}
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
	/*Region 2*/
	p = Round(Ry2*(x - 1)*(x - 1) + Rx2*(y +0.5)*(y +0.5) - Rx2*Ry2);
	while (x > 0)
	{
		x--;
		px -= twoRy2;
		if (p > 0)
			p += Ry2 - px;
		else
		{
			y++;
			py += twoRx2;
			p += Ry2 - px + py;
		}
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
}
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixel(xCenter + x, yCenter + y);
	setPixel(xCenter - x, yCenter + y);
	setPixel(xCenter + x, yCenter - y);
	setPixel(xCenter - x, yCenter - y);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	ellipseMidAntiClock(300, 300, 80, 60);
	glFlush();
}
void code_6_exercise_14()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_15
int nThread = 4; // 四个处理线程
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixel(xCenter + x, yCenter + y);
	setPixel(xCenter - x, yCenter + y);
	setPixel(xCenter + x, yCenter - y);
	setPixel(xCenter - x, yCenter - y);
}
void ThreadFunc1(int xCenter, int yCenter, int Rx, int Ry, double x, int xEnd)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	double y = ((double)Ry) / Rx * sqrt(Rx2 - x*x);
	int p;
	int _x = Round(x);
	int _y = Round(y);
	int px = Round(twoRy2 * x);
	int py = Round(twoRx2 * y);
	ellipsePlotPoints(xCenter, yCenter, _x, _y);
	p = Round(Ry2*(x + 1)*(x + 1) + Rx2*(y -0.5)*(y-0.5)- Rx2*Ry2);
	while (_x < xEnd)
	{
		_x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			_y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		ellipsePlotPoints(xCenter, yCenter, _x, _y);
	}
}

void ThreadFunc2(int xCenter, int yCenter, int Rx, int Ry, double y, int yEnd)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	double x = ((double)Rx) / Ry * sqrt(Ry2 - y*y);
	int p;
	int _x = Round(x);
	int _y = Round(y);
	int px = Round(twoRy2 * x);
	int py = Round(twoRx2 * y);
	ellipsePlotPoints(xCenter, yCenter, _x, _y);
	p = Round(Ry2*(x + 0.5)*(x + 0.5) + Rx2*(y - 1)*(y - 1) - Rx2*Ry2);
	while (_y > yEnd)
	{
		_y--;
		py -= twoRx2;
		if (p > 0)
			p += Rx2 - py;
		else
		{
			_x++;
			px += twoRy2;
			p += Rx2 - py + px;
		}
		ellipsePlotPoints(xCenter, yCenter, _x, _y);
	}
}

void ellipseParallelMid(int xCenter, int yCenter, int Rx, int Ry)
{
	if (nThread % 2 != 0)
	{
		printf("必须是偶数个线程!!!\n");
		return;
	}

	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;

	double CutX = Rx2 / sqrt(Rx2 + Ry2);
	double CutY = Ry2 / sqrt(Rx2 + Ry2);
	int d1 = Round(CutX / (nThread / 2));
	for (int i = 0; i < nThread / 2; i++)
	{
		int xEnd = 0;
		if ((i + 1)*d1 < CutX)
			xEnd = (i + 1)*d1;
		else
			xEnd = CutX;
		ThreadFunc1(xCenter, yCenter, Rx, Ry, i * d1, xEnd);
	}

	int d2 = Round(CutY / (nThread / 2));
	for (int i = 0; i < nThread / 2; i++)
	{
		int yEnd = 0;
		if (CutY - (i + 1)*d2 > 0)
			yEnd = CutY - (i + 1)*d2;
		else
			yEnd = 0;
		ThreadFunc2(xCenter, yCenter, Rx, Ry, CutY - i * d2, yEnd);
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	ellipseParallelMid(300, 300, 80, 60);
	glFlush();
}
void code_6_exercise_15()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_16
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
int PIdivide2x = PI / 2 * 10;
int PIx = PI * 10;
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(20, yCenter);
	glVertex2i(700, yCenter);

	glVertex2i(xCenter, 20);
	glVertex2i(xCenter, 500);
	glEnd();
}
void sinPlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixel(xCenter + x, yCenter + y);
	setPixel(xCenter + PIx - x, yCenter + y);
	setPixel(xCenter + PIx + x, yCenter - y);
	setPixel(xCenter + PIx + PIx - x, yCenter - y);
}
void sinMid3(int xc, int yc)
{
	int x= 0, y = 0;
	int p = 299;
	int xaddone2 = 1;
	int _xaddone2 = 3;
	int xaddonex2 = 0;
	int _xaddonex2 = 2;
	int x2 = 0;
	int _x2 = 1;

	sinPlotPoints(xc, yc, x, y);

	
	while (x < PIdivide2x)
	{
		x++;

		xaddone2 += _xaddone2;
		_xaddone2 += 2;

		xaddonex2 += _xaddonex2;
		_xaddonex2 += 2;

		x2 += _x2;
		_x2 += 2;
		if (p < 0)
			p += 600 - (xaddone2 + xaddonex2 + x2);
		else
		{
			y++;
			p += - (xaddone2 + xaddonex2 + x2);
		}
		sinPlotPoints(xc, yc, x, y);
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(1.0, 0.0, 0.0);
	coord(100, 100);

	glColor3f(1.0, 1.0, 1.0);
	sinMid3(100, 100);
	
	glFlush();
}
void code_6_exercise_16()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_16_Test1
int n = 7; // n阶泰勒展开式
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
int PIdivide2x = PI / 2 * 10;
int PIx = PI * 10;
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(20, yCenter);
	glVertex2i(700, yCenter);

	glVertex2i(xCenter, 20);
	glVertex2i(xCenter, 500);
	glEnd();
}
void sinPlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixel(xCenter + x, yCenter + y);
	setPixel(xCenter + PIx - x, yCenter + y);
	setPixel(xCenter + PIx + x, yCenter - y);
	setPixel(xCenter + PIx + PIx - x, yCenter - y);
}
int fact(int n, int count = -1)
{
	int ret = 1;
	if (count==-1)
		count = n;

	for (int i = 0; i < count; i++)
		ret *= n--;

	return ret;
}
void sinMidn(int xc, int yc)
{
	int m = (n+1) / 2;
	int x = 0, y = 0;
	__int64 p = 0; // 7阶以上时int越界，故用64位int
	for (int i = 0; i < m; i++)
	{
		p += pow(-1, i + 1 - 1)* fact(2 * m - 1, 2 * m - 1 - (2 * (i + 1) - 1)) * pow(10, 2 * m - 1 - (2 * (i + 1) - 1));
	}
	p -= Round(0.5 * fact(2 * m - 1)) * pow(10, 2 * m - 1 - 1);

	sinPlotPoints(xc, yc, x, y);

	while (x < PIdivide2x)
	{
		x++;
	
		if (p >= 0)
			y++;

		p = 0;
		for (int i = 0; i < m; i++)
		{
			p += pow(-1, i + 1 - 1) * pow(x + 1, 2 * (i + 1) - 1) * fact(2 * m - 1, 2 * m - 1 - (2 * (i + 1) - 1)) * pow(10, 2 * m - 1 - (2 * (i + 1) - 1));
		}
		p -= Round((y + 0.5) * fact(2 * m - 1)) * pow(10, 2 * m - 1 - 1);

		sinPlotPoints(xc, yc, x, y);
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(1.0, 0.0, 0.0);
	coord(100, 100);

	glColor3f(1.0, 1.0, 1.0);
	sinMidn(100, 100);
	
	glFlush();
}
void code_6_exercise_16_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_17
int n = 5; // n阶泰勒展开式
int period = 3; // 周期数
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
int PIdivide2x = PI / 2 * 10;
int PIx = PI * 10;
int twoPIx = 2 * PIx;
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(20, yCenter);
	glVertex2i(700, yCenter);

	glVertex2i(xCenter, 20);
	glVertex2i(xCenter, 500);
	glEnd();
}
void sinPlotPoints(int xCenter, int yCenter, int x, int y)
{
	for (int i = 0; i < period; i++)
	{
		setPixel(xCenter + (x) + i * twoPIx, yCenter + y);
		setPixel(xCenter + (PIx - x) + i * twoPIx, yCenter + y);
		setPixel(xCenter + (PIx + x) + i * twoPIx, yCenter - y);
		setPixel(xCenter + (twoPIx - x) + i * twoPIx, yCenter - y);
	}
}
int fact(int n, int count = -1)
{
	int ret = 1;
	if (count == -1)
		count = n;

	for (int i = 0; i < count; i++)
		ret *= n--;

	return ret;
}
void sinMidn(int xc, int yc)
{
	int m = (n + 1) / 2;
	int x = 0, y = 0;
	__int64 p = 0; // 7阶以上时int越界，故用64位int
	for (int i = 0; i < m; i++)
	{
		p += pow(-1, i + 1 - 1)* fact(2 * m - 1, 2 * m - 1 - (2 * (i + 1) - 1)) * pow(10, 2 * m - 1 - (2 * (i + 1) - 1));
	}
	p -= Round(0.5 * fact(2 * m - 1)) * pow(10, 2 * m - 1 - 1);

	sinPlotPoints(xc, yc, x, y);

	while (x < PIdivide2x)
	{
		x++;

		if (p >= 0)
			y++;

		p = 0;
		for (int i = 0; i < m; i++)
		{
			p += pow(-1, i + 1 - 1) * pow(x + 1, 2 * (i + 1) - 1) * fact(2 * m - 1, 2 * m - 1 - (2 * (i + 1) - 1)) * pow(10, 2 * m - 1 - (2 * (i + 1) - 1));
		}
		p -= Round((y + 0.5) * fact(2 * m - 1)) * pow(10, 2 * m - 1 - 1);

		sinPlotPoints(xc, yc, x, y);
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord(100, 100);

	glColor3f(1.0, 1.0, 1.0);
	sinMidn(100, 100);

	glFlush();
}
void code_6_exercise_17()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_18
int n = 5; // n阶泰勒展开式
double A = 2;
double k = 0.25;
double w = 2.5;
double sita = PI / 4;
double e = 2.71828;
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
int fact(int n, int count = -1)
{
	int ret = 1;
	if (count == -1)
		count = n;

	for (int i = 0; i < count; i++)
		ret *= n--;

	return ret;
}
int PIdivide2x = PI / 2 * 10;
int PIx = PI * 10;
int twoPIx = 2 * PIx;
double twoPI = 2 * PI;
double PIdivide2 = PI / 2;
double sinn(double x)
{
	x = fmod(x, twoPI);

	int quadrant = x / PIdivide2 + 1;
	double angle = fmod(x, PIdivide2);

	if (quadrant == 2 || quadrant == 4)
		angle = PIdivide2 - angle;

	int m = (n + 1) / 2;
	double ret = 0;
	for (int i = 0; i < m; i++)
	{
		ret += pow(-1, i + 1 - 1) * pow(angle, 2 * (i + 1) - 1) / fact(2 * (i + 1) - 1);
	}
	if (quadrant == 3 || quadrant == 4)
		ret = -ret;

	return ret;
}
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(20, yCenter);
	glVertex2i(700, yCenter);

	glVertex2i(xCenter, 20);
	glVertex2i(xCenter, 500);
	glEnd();
}
void damping(int xc, int yc)
{
	double x = 0, y = 0;
	int _x = 0;
	double _y = 0;
	bool uping = false;

	y = A * pow(e, -k * x) * sinn(w * x + sita);
	_y = y * 10;
	setPixel(xc + _x, yc + Round(_y));
	double last = y;

	while (1)
	{
		x = x + 0.1;
		_x++;

		y = A * pow(e, -k * x) * sinn(w * x + sita);
		_y = y * 10;

		if (fabs(y) > fabs(last))
		{
			uping = true;
		}
		else if (fabs(y) < fabs(last))
		{
			if (uping)
			{
				uping = false;
				if (fabs(last) < A /10)
					break;
			}
		}
		last = y;

		setPixel(xc + _x, yc + Round(_y));
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord(100, 100);

	glColor3f(1.0, 1.0, 1.0);
	damping(100, 100);

	glFlush();
}
void code_6_exercise_18()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_19
int n = 5; // n阶泰勒展开式
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
int fact(int n, int count = -1)
{
	int ret = 1;
	if (count == -1)
		count = n;

	for (int i = 0; i < count; i++)
		ret *= n--;

	return ret;
}
double twoPI = 2 * PI;
double PIdivide2 = PI / 2;
double sinn(double x)
{
	x = fmod(x, twoPI);

	int quadrant = x / PIdivide2 + 1;
	double angle = fmod(x, PIdivide2);

	if (quadrant == 2 || quadrant == 4)
		angle = PIdivide2 - angle;

	int m = (n + 1) / 2;
	double ret = 0;
	for (int i = 0; i < m; i++)
	{
		ret += pow(-1, i + 1 - 1) * pow(angle, 2 * (i + 1) - 1) / fact(2 * (i + 1) - 1);
	}
	if (quadrant == 3 || quadrant == 4)
		ret = -ret;

	return ret;
}
void sinPlotPoints(int xc, int yc, int x, int y)
{
	setPixel(xc + x, yc + y);
	setPixel(xc + winWidth / 2 - x, yc + y);
	setPixel(xc + winWidth / 2 + x, yc - y);
	setPixel(xc + winWidth - x, yc - y);
}
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(xCenter, 0);
	glVertex2i(xCenter, 1000);

	glVertex2i(0, yCenter);
	glVertex2i(1000, yCenter);
	glEnd();
}
void damping(int xc, int yc)
{
	int x = 0;

	while (x <= winWidth / 4)
	{
		sinPlotPoints(xc, yc, x, sinn(x * twoPI / winWidth) * winHeight / 2);
		x++;
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord(0, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	damping(0, winHeight / 2);

	glFlush();
}
void code_6_exercise_19()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_20
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void sinPlotPoints(int xc, int yc, int x, int y)
{
	setPixel(xc + x, yc + y);
	setPixel(xc - x, yc - y);
	printf("%d, %d\n", x, y);
}
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(xCenter, -1000);
	glVertex2i(xCenter, 1000);

	glVertex2i(-1000, yCenter);
	glVertex2i(1000, yCenter);
	glEnd();
}
void cube(int xc, int yc)
{
	int x = 0;
	int y = 0;
	sinPlotPoints(xc, yc, x, y);
	
	//找到零界点（斜率大于等于1）
	int i = 0;
	for (; i < winWidth / 2; i++)
	{
		if(0.25 * pow(i, 2) >= 1)
			break;
	}

	int p1 = pow((0 + 1), 2);
	int p2 = (0 + 1) * 0;
	int p3 = pow(0, 2);

	int xk1 = 2 * 0 + 3;
	int xk2 = 2 * 0 + 2;
	int xk3 = 2 * 0 + 1;

	int dxk = 2;

	int p = -5;

	p1 += xk1;
	p2 += xk2;
	p3 += xk3;

	x++;
	if (p > 0)
	{
		y++;
		p = p + p1 + p2 + p3 - 12;
	}
	else
	{
		p = p + p1 + p2 + p3;
	}
	sinPlotPoints(xc, yc, x, y);

	// 斜率小于1
	while (x < i)
	{
		xk1 += dxk;
		xk2 += dxk;
		xk3 += dxk;

		p1 += xk1;
		p2 += xk2;
		p3 += xk3;

		x++;
		if (p > 0)
		{
			y++;
			p = p + p1 + p2 + p3 - 12;
		}
		else
		{
			p = p + p1 + p2 + p3;
		}

		sinPlotPoints(xc, yc, x, y);
	}

	// 斜率大于1
	while (y < winHeight / 2)
	{
		double p = pow((double)12 * (y + 1), (double)1 / 3) - (x + 0.5);
		y++;
		if (p > 0)
			x++;
		sinPlotPoints(xc, yc, x, y);
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord(winWidth / 2, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	cube(winWidth / 2, winHeight / 2);

	glFlush();
}
void code_6_exercise_20()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_21
int n = 5; // n阶泰勒展开式
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
int fact(int n, int count = -1)
{
	int ret = 1;
	if (count == -1)
		count = n;

	for (int i = 0; i < count; i++)
		ret *= n--;

	return ret;
}
double twoPI = 2 * PI;
double PIdivide2 = PI / 2;
double angle = 3* PI;
double sinn(double x)
{
	x = fmod(x, twoPI);

	int quadrant = x / PIdivide2 + 1;
	double angle = fmod(x, PIdivide2);

	if (quadrant == 2 || quadrant == 4)
		angle = PIdivide2 - angle;

	int m = (n + 1) / 2;
	double ret = 0;
	for (int i = 0; i < m; i++)
	{
		ret += pow(-1, i + 1 - 1) * pow(angle, 2 * (i + 1) - 1) / fact(2 * (i + 1) - 1);
	}
	if (quadrant == 3 || quadrant == 4)
		ret = -ret;

	return ret;
}
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(xCenter, 0);
	glVertex2i(xCenter, 1000);

	glVertex2i(0, yCenter);
	glVertex2i(1000, yCenter);
	glEnd();
}
void damping(int xc, int yc)
{
	int x = 0;

	while (x <= winWidth)
	{
		setPixel(xc + x, yc + sinn(x * angle / winWidth) * winHeight / 2);
		x++;
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord(0, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	damping(0, winHeight / 2);

	glFlush();
}
void code_6_exercise_21()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_22
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void plotPoint(int xc, int yc, int x, int y)
{
	setPixel(xc + x,yc + y);
	setPixel(xc + x,yc - y);
	printf("%d, %d\n", x, y);
}
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(xCenter, 0);
	glVertex2i(xCenter, 1000);

	glVertex2i(0, yCenter);
	glVertex2i(1000, yCenter);
	glEnd();
}
void parabola(int xc, int yc)
{
	int x = -5, y = 0;
	plotPoint(xc, yc, x, y);

	float p = pow((0 + 1), 2) - 5 - (-5 + 0.5);// 起始x=-5,y=0
	while (x < -4) // x >= -4.75时，斜率小于1 
	{
		if (p > 0)
		{
			x++;
			p += 2 * y + 3 - 1;
		}
		else
		{
			p += 2 * y + 3;
		}
		y++;
		plotPoint(xc, yc, x, y);
	}

	p = pow(x + 1 + 5, 0.5) - (y + 0.5);// 起始x=-4,y=1
	while (x < 10) // 斜率大于1 
	{
		if (p > 0)
		{
			y++;
		}
		x++;
		plotPoint(xc, yc, x, y);
		p = pow(x + 1 + 5, 0.5) - (y + 0.5);
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord(winWidth / 2, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	parabola(winWidth / 2, winHeight / 2);

	glFlush();
}
void code_6_exercise_22()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_23
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void plotPoint(int xc, int yc, int x, int y)
{
	setPixel(xc + x, yc + y);
	setPixel(xc - x, yc + y);
}
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(xCenter, 0);
	glVertex2i(xCenter, 1000);

	glVertex2i(0, yCenter);
	glVertex2i(1000, yCenter);
	glEnd();
}
void parabola(int xc, int yc)
{
	int x = 0, y = 50;
	plotPoint(xc, yc, x, y);

	float p = -1 * pow((0 + 1), 2) + 50 - (50 - 0.5);// 起始x=0,y=50
	while (x < 1) // x >= 0.5时，斜率大于1 
	{
		x++;
		if (p > 0)
		{
			p += -2 * x - 1;
		}
		else
		{
			y--;
			p += -2 * x - 1 + 1;
		}
		plotPoint(xc, yc, x, y);
	}

	p = pow(50 - (y - 1), 0.5) - (x + 0.5);// 起始x=1,y=49
	while (x < 5) // 斜率大于1 
	{
		if (p > 0)
		{
			x++;
		}
		y--;
		plotPoint(xc, yc, x, y);
		p = pow(50 - (y - 1), 0.5) - (x + 0.5);
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord(winWidth / 2, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	parabola(winWidth / 2, winHeight / 2);

	glFlush();
}
void code_6_exercise_23()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_24
float a = 0.01;
float b = 10;
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void plotPoint(int xc, int yc, int x, int y)
{
	setPixel(xc + x, yc + y);
	setPixel(xc - x, yc + y);
}
void coord(int xCenter, int yCenter)
{
	glBegin(GL_LINES);
	glVertex2i(xCenter, 0);
	glVertex2i(xCenter, 1000);

	glVertex2i(0, yCenter);
	glVertex2i(1000, yCenter);
	glEnd();
}
void parabola(int xc, int yc)
{
	int x = 0, y = b;
	plotPoint(xc, yc, x, y);

	int sign = a > 0 ? 1 : -1;

	float slope = std::fabs(1 / (2 * a)); // 斜率绝对值为1

	// 斜率绝对值小于1
	float p = a * pow((0 + 1), 2) + b - (b + sign * 0.5);
	float _a = a * (1 + 2 * x);
	float da = 2 * a;
	while (x < slope)
	{
		x++;
		_a += da;

		if (p > 0)
		{			
			p += _a;
			if (a > 0)
			{
				y++;
				p -= 1;
			}
		}
		else
		{
			p += _a;
			if (a < 0)
			{
				y--;
				p += 1;
			}
		}
		plotPoint(xc, yc, x, y);
	}

	// 斜率绝对值大于1
	p = pow(((y + sign * 1) - b) / a, 0.5) - (x + 0.5);
	while (x < winWidth / 2)
	{
		y += sign * 1;
		if (p > 0)
		{
			x++;
		}		
		plotPoint(xc, yc, x, y);
		p = pow(((y + sign * 1) - b) / a, 0.5) - (x + 0.5);
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord(winWidth / 2, winHeight / 2);

	glColor3f(1.0, 1.0, 1.0);
	parabola(winWidth / 2, winHeight / 2);

	glFlush();
}
void code_6_exercise_24()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_25
float xcenter = winWidth / 2;
float ycenter = winHeight / 2;
float radius = 100;
float h = 400;
int spliteNum = 1000;
const GLdouble twoPi = 6.283185;
struct Point
{
	GLint x, y, z;
};
struct Surface
{
	vector<Point> points;
};
vector<Surface> surfaceTable;
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void coord()
{
	glBegin(GL_LINES);
	glVertex2i(xcenter, 0);
	glVertex2i(xcenter, 1000);

	glVertex2i(0, ycenter);
	glVertex2i(1000, ycenter);
	glEnd();
}
void column()
{
	for (auto& s : surfaceTable)
	{
		glBegin(GL_LINE_LOOP);
			for (auto& p : s.points)
			{
				glVertex3i(p.x, p.y, p.z);
			}
		glEnd();
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	coord();

	glColor3f(1.0, 1.0, 1.0);
	column();

	glFlush();
}
void makeSurface(float lx, float ly, float cx, float cy)
{
	surfaceTable.push_back(Surface());
	surfaceTable.back().points.push_back({ Round(xcenter + lx),Round(ycenter + ly),Round(0) });
	surfaceTable.back().points.push_back({ Round(xcenter + cx),Round(ycenter + cy),Round(0) });
	surfaceTable.back().points.push_back({ Round(xcenter + cx),Round(ycenter + cy),Round(h) });
	surfaceTable.back().points.push_back({ Round(xcenter + lx),Round(ycenter + ly),Round(h) });
}
void clac()
{
	float lx = radius;
	float ly = 0;
	for (int i = 1; i < spliteNum; i++)
	{
		double angle = i * twoPi / spliteNum;
		float cx = radius * cos(angle);
		float cy = radius * sin(angle);

		makeSurface(lx, ly, cx, cy);

		lx = cx;
		ly = cy;
	}
	makeSurface(lx, ly, radius, 0);
}
void code_6_exercise_25()
{
	clac();
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_26
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
bool needPixel(int index, const std::string& lineTypeMode)
{
	return lineTypeMode[index % lineTypeMode.size()] == '1';
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;
	
	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;

	if(needPixel(index, lineTypeMode))
		setPixel(x, y);

	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}

		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);

	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int index = 0;
	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, lineTypeMode);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, lineTypeMode);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
}
// 固定像素数划线
void lineType1(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	lineBres(x0, y0, xEnd, yEnd, lineTypeMode);
}
// 固定长度划线
void lineType2(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	int dx = std::abs(x0 - xEnd);
	int dy = std::abs(y0 - yEnd);
	float realRate = dx / std::sqrt(dx * dx + dy * dy);
	std::string newLineTypeMode;
	char curType = lineTypeMode[0];
	int curCount = 0;
	for (int i = 0;; i++)
	{
		if (i == lineTypeMode.size())
		{
			newLineTypeMode.append(Round(curCount * realRate), curType);
			break;
		}
		if (lineTypeMode[i] != curType)
		{
			newLineTypeMode.append(Round(curCount * realRate), curType);
			curType = lineTypeMode[i];
			curCount = 1;
		}
		else
		{
			curCount++;
		}
	}
	lineBres(x0, y0, xEnd, yEnd, newLineTypeMode);
}
// 分段划线
void lineType3(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float l = std::sqrt(dx * dx + dy * dy);
	float rateX = dx / l;
	float rateY = dy / l;

	int totalSize = lineTypeMode.size();
	float start;
	bool begin = false;
	int _startx, _starty;
	for (int i = 0;; i++)
	{
		start = i * totalSize;
		begin = false;
		for (int j = 0;; j++)
		{
			if (j >= totalSize)
			{
				if (begin)
				{
					lineBres(_startx, _starty, x0 + Round((start + j - 1) * rateX), y0 + Round((start + j - 1) * rateY), "1");
				}
				break;
			}

			if (lineTypeMode[j] == '1')
			{
				if (!begin)
				{
					begin = true;
					_startx = x0 + Round((start + j) * rateX);
					_starty = y0 + Round((start + j) * rateY);
				}
			}
			else
			{
				if (begin)
				{
					lineBres(_startx, _starty, x0 + Round((start + j - 1) * rateX), y0 + Round((start + j - 1) * rateY), "1");
				}
				begin = false;
			}

			// 退出条件
			if (std::abs(Round((start + j + 1) * rateX)) >= std::abs(dx))
			{
				if (begin)
				{
					lineBres(_startx, _starty, x0 + Round((start + j) * rateX), y0 + Round((start + j) * rateY), "1");
				}
				return;
			}
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	lineType1(100, 100, 200, 200, "1");// 实线
	lineType1(100, 150, 200, 250, "11111111000");// 划线
	lineType1(100, 200, 200, 300, "1100");// 点线

	lineType1(100, 450, 200, 400, "1");
	lineType1(100, 500, 200, 450, "11111111000");
	lineType1(100, 550, 200, 500, "1100");

	glColor3f(1.0, 0.0, 0.0);

	lineType2(300, 100, 400, 200, "1");
	lineType2(300, 150, 400, 250, "11111111000");
	lineType2(300, 200, 400, 300, "1100");
			
	lineType2(300, 450, 400, 400, "1");
	lineType2(300, 500, 400, 450, "11111111000");
	lineType2(300, 550, 400, 500, "1100");

	glColor3f(1.0, 1.0, 0.0);

	lineType3(500, 100, 600, 200, "1");
	lineType3(500, 150, 600, 250, "11111111000");
	lineType3(500, 200, 600, 300, "1100");
			  			
	lineType3(500, 450, 600, 400, "1");
	lineType3(500, 500, 600, 450, "11111111000");
	lineType3(500, 550, 600, 500, "1100");

	glFlush();
}
void code_6_exercise_26()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_27
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
bool needPixel(int index, const std::string& lineTypeMode)
{
	return lineTypeMode[index % lineTypeMode.size()] == '1';
}
// 0<m<1(m>1)
// XYmirror: x,y坐标交换（沿y=x直线对称变换）
void lineBresMid1(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode, bool XYmirror = false)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dx - 2 * dy;
	int twoDy = 2 * dy, twoDxMinusDy = 2 * (dx - dy);
	int x = x0;
	int y = y0;
	int index = 0;

	if (XYmirror)
	{
		if (needPixel(index, lineTypeMode))
			setPixel(y, x);
	}		
	else
	{
		if (needPixel(index, lineTypeMode))
			setPixel(x, y);
	}

	while (x < xEnd)
	{
		x++;
		if (p > 0)
			p -= twoDy;
		else
		{
			y++;
			p += twoDxMinusDy;
		}
		if (XYmirror)
		{
			if (needPixel(++index, lineTypeMode))
				setPixel(y, x);
		}
		else
		{
			if (needPixel(++index, lineTypeMode))
				setPixel(x, y);
		}			
	}
}
// -1<m<0(m<-1)
// XYmirror: x,y坐标交换（沿y=x直线对称变换）
void lineBresMid2(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode, bool XYmirror = false)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = xEnd - x0, dy = yEnd - y0;
	int p = -dx - 2 * dy;
	int twoDy = 2 * dy, twoDxMinusDy = 2 * (-dx - dy);
	int x = x0;
	int y = y0;
	int index = 0;

	if (XYmirror)
	{
		if (needPixel(index, lineTypeMode))
			setPixel(y, x);
	}
	else
	{
		if (needPixel(index, lineTypeMode))
			setPixel(x, y);
	}

	while (x < xEnd)
	{
		x++;
		if (p <= 0)
			p -= twoDy;
		else
		{
			y--;
			p += twoDxMinusDy;
		}
		if (XYmirror)
		{
			if (needPixel(++index, lineTypeMode))
				setPixel(y, x);
		}
		else
		{
			if (needPixel(++index, lineTypeMode))
				setPixel(x, y);
		};
	}
}
void lineBresMid(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBresMid1(y0, x0, yEnd, xEnd, lineTypeMode, true);
		}
		else
		{
			lineBresMid1(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBresMid2(y0, x0, yEnd, xEnd, lineTypeMode, true);
		}
		else
		{
			lineBresMid2(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
}
// 固定像素数划线
void lineType1(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	lineBresMid(x0, y0, xEnd, yEnd, lineTypeMode);
}
// 固定长度划线
void lineType2(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	int dx = std::abs(x0 - xEnd);
	int dy = std::abs(y0 - yEnd);
	float realRate = dx / std::sqrt(dx * dx + dy * dy);
	std::string newLineTypeMode;
	char curType = lineTypeMode[0];
	int curCount = 0;
	for (int i = 0;; i++)
	{
		if (i == lineTypeMode.size())
		{
			newLineTypeMode.append(Round(curCount * realRate), curType);
			break;
		}
		if (lineTypeMode[i] != curType)
		{
			newLineTypeMode.append(Round(curCount * realRate), curType);
			curType = lineTypeMode[i];
			curCount = 1;
		}
		else
		{
			curCount++;
		}
	}
	lineBresMid(x0, y0, xEnd, yEnd, newLineTypeMode);
}
// 分段划线
void lineType3(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float l = std::sqrt(dx * dx + dy * dy);
	float rateX = dx / l;
	float rateY = dy / l;

	int totalSize = lineTypeMode.size();
	float start;
	bool begin = false;
	int _startx, _starty;
	for (int i = 0;; i++)
	{
		start = i * totalSize;
		begin = false;
		for (int j = 0;; j++)
		{
			if (j >= totalSize)
			{
				if (begin)
				{
					lineBresMid(_startx, _starty, x0 + Round((start + j - 1) * rateX), y0 + Round((start + j - 1) * rateY), "1");
				}
				break;
			}

			if (lineTypeMode[j] == '1')
			{
				if (!begin)
				{
					begin = true;
					_startx = x0 + Round((start + j) * rateX);
					_starty = y0 + Round((start + j) * rateY);
				}
			}
			else
			{
				if (begin)
				{
					lineBresMid(_startx, _starty, x0 + Round((start + j - 1) * rateX), y0 + Round((start + j - 1) * rateY), "1");
				}
				begin = false;
			}

			// 退出条件
			if (std::abs(Round((start + j + 1) * rateX)) >= std::abs(dx))
			{
				if (begin)
				{
					lineBresMid(_startx, _starty, x0 + Round((start + j) * rateX), y0 + Round((start + j) * rateY), "1");
				}
				return;
			}
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	lineType1(100, 100, 200, 200, "1");  // 实线
	lineType1(100, 150, 200, 250, "11111111000"); // 划线
	lineType1(100, 200, 200, 300, "1100"); // 点线

	lineType1(100, 450, 200, 400, "1");
	lineType1(100, 500, 200, 450, "11111111000");
	lineType1(100, 550, 200, 500, "1100");

	glColor3f(1.0, 0.0, 0.0);

	lineType2(300, 100, 400, 200, "1");
	lineType2(300, 150, 400, 250, "11111111000");
	lineType2(300, 200, 400, 300, "1100");

	lineType2(300, 450, 400, 400, "1");
	lineType2(300, 500, 400, 450, "11111111000");
	lineType2(300, 550, 400, 500, "1100");

	glColor3f(1.0, 1.0, 0.0);

	lineType3(500, 100, 600, 200, "1");
	lineType3(500, 150, 600, 250, "11111111000");
	lineType3(500, 200, 600, 300, "1100");

	lineType3(500, 450, 600, 400, "1");
	lineType3(500, 500, 600, 450, "11111111000");
	lineType3(500, 550, 600, 500, "1100");

	glFlush();
}
void code_6_exercise_27()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_28
inline int Round(const double a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
bool needPixel(int index, const std::string& lineTypeMode)
{
	return lineTypeMode[index % lineTypeMode.size()] == '1';
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);

	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}

		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);

	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int index = 0;
	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, lineTypeMode);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, lineTypeMode);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
}
void threadFunc(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	lineBres(x0, y0, xEnd, yEnd, lineTypeMode);
}
// 固定像素数划线
void parallelLineType1(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	//std::vector<std::thread> threads;

	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float rateXY = (float)dy / dx;

	int totalSize = lineTypeMode.size();
	int startx, starty;
	int endx, endy;
	bool forEnd = false;
	int sgin = dx >= 0 ? 1 : -1;
	for (int i = 0;; i++)
	{
		startx = x0 + sgin * i * totalSize;
		starty = y0 + Round(sgin * i * totalSize * rateXY);
		endx = x0 + sgin * (i + 1) * totalSize;
		endy = y0 + Round(sgin * (i + 1) * totalSize * rateXY);

		if (std::abs(sgin * (i+1) * totalSize) >= std::abs(dx))
		{
			endx = xEnd;
			endy = yEnd;
			forEnd = true;
		}			
		//threads.push_back(std::thread(threadFunc, startx, starty, endx, endy, lineTypeMode));
		threadFunc(startx, starty, endx, endy, lineTypeMode);
		if (forEnd)
			break;
	}
	//for (auto& thread : threads)
	//	thread.join();
}
// 固定长度划线
void parallelLineType2(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float l = std::sqrt(std::abs(dx) * std::abs(dx) + std::abs(dy) * std::abs(dy));
	float realRate = std::abs(dx) / l;
	std::string newLineTypeMode;
	char curType = lineTypeMode[0];
	int curCount = 0;
	for (int i = 0;; i++)
	{
		if (i == lineTypeMode.size())
		{
			newLineTypeMode.append(Round(curCount * realRate), curType);
			break;
		}
		if (lineTypeMode[i] != curType)
		{
			newLineTypeMode.append(Round(curCount * realRate), curType);
			curType = lineTypeMode[i];
			curCount = 1;
		}
		else
		{
			curCount++;
		}
	}

	float rateX = dx / l;
	float rateY = dy / l;
	int totalSize = lineTypeMode.size();
	int startx, starty;
	int endx, endy;
	bool forEnd = false;
	for (int i = 0;; i++)
	{
		startx = x0 + Round(i * totalSize * rateX);
		starty = y0 + Round(i * totalSize * rateY);
		endx = x0 + Round((i + 1) * totalSize * rateX);
		endy = y0 + Round((i + 1) * totalSize * rateY);

		if (std::abs(Round((i + 1) * totalSize * rateX)) >= std::abs(dx))
		{
			endx = xEnd;
			endy = yEnd;
			forEnd = true;
		}

		threadFunc(startx, starty, endx, endy, newLineTypeMode);
		if (forEnd)
			return;
	}
}
void _threadFunc(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float l = std::sqrt(dx * dx + dy * dy);
	float rateX = dx / l;
	float rateY = dy / l;

	int totalSize = lineTypeMode.size();
	bool begin = false;
	int _startx, _starty;

	for (int j = 0;; j++)
	{
		if (j >= totalSize)
		{
			if (begin)
			{
				lineBres(_startx, _starty, x0 + Round((j - 1) * rateX), y0 + Round((j - 1) * rateY), "1");
			}
			return;
		}

		if (lineTypeMode[j] == '1')
		{
			if (!begin)
			{
				begin = true;
				_startx = x0 + Round(j * rateX);
				_starty = y0 + Round(j * rateY);
			}
		}
		else
		{
			if (begin)
			{
				lineBres(_startx, _starty, x0 + Round((j - 1) * rateX), y0 + Round((j - 1) * rateY), "1");
			}
			begin = false;
		}
	}
}
// 分段划线
void parallelLineType3(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float l = std::sqrt(std::abs(dx) * std::abs(dx) + std::abs(dy) * std::abs(dy));

	float rateX = dx / l;
	float rateY = dy / l;
	int totalSize = lineTypeMode.size();
	int startx, starty;
	int endx, endy;
	bool forEnd = false;
	for (int i = 0;; i++)
	{
		startx = x0 + Round(i * totalSize * rateX);
		starty = y0 + Round(i * totalSize * rateY);
		endx = x0 + Round((i + 1) * totalSize * rateX);
		endy = y0 + Round((i + 1) * totalSize * rateY);

		if (std::abs(Round((i + 1) * totalSize * rateX)) >= std::abs(dx))
		{
			endx = xEnd;
			endy = yEnd;
			forEnd = true;
		}

		_threadFunc(startx, starty, endx, endy, lineTypeMode);
		if (forEnd)
			return;
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	parallelLineType1(100, 100, 200, 200, "1");  // 实线
	parallelLineType1(100, 150, 200, 250, "11111111000"); // 划线
	parallelLineType1(100, 200, 200, 300, "1100"); // 点线

	parallelLineType1(100, 450, 200, 400, "1");
	parallelLineType1(100, 500, 200, 450, "11111111000");
	parallelLineType1(100, 550, 200, 500, "1100");

	glColor3f(1.0, 0.0, 0.0);

	parallelLineType2(300, 100, 400, 200, "1");
	parallelLineType2(300, 150, 400, 250, "11111111000");
	parallelLineType2(300, 200, 400, 300, "1100");
	
	parallelLineType2(300, 450, 400, 400, "1");
	parallelLineType2(300, 500, 400, 450, "11111111000");
	parallelLineType2(300, 550, 400, 500, "1100");

	glColor3f(1.0, 1.0, 0.0);

	parallelLineType3(500, 100, 600, 200, "1");
	parallelLineType3(500, 150, 600, 250, "11111111000");
	parallelLineType3(500, 200, 600, 300, "1100");
					
	parallelLineType3(500, 450, 600, 400, "1");
	parallelLineType3(500, 500, 600, 450, "11111111000");
	parallelLineType3(500, 550, 600, 500, "1100");

	glFlush();
}
void code_6_exercise_28()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_29
void setWidthPixelV(int x, int y, int width)
{
	int curWidthIdx = 0;
	for (int i = 0 ; i < width; i++)
	{
		if (i % 2)
			curWidthIdx++;
		if (i % 2)
			setPixel(x, y + curWidthIdx);
		else
			setPixel(x, y - curWidthIdx);
	}
}
void setWidthPixelH(int x, int y, int width)
{
	int curWidthIdx = 0;
	for (int i = 0; i < width; i++)
	{
		if (i % 2)
			curWidthIdx++;
		if (i % 2)
			setPixel(x + curWidthIdx, y);
		else
			setPixel(x - curWidthIdx, y);
	}
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setWidthPixelV(x, y, width);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setWidthPixelV(x, y, width);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setWidthPixelH(x, y, width);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setWidthPixelH(x, y, width);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setWidthPixelV(x, y, width);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setWidthPixelV(x, y, width);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setWidthPixelH(x, y, width);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setWidthPixelH(x, y, width);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, width);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, width);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, width);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, width);
		}
	}
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	lineBres(53, 95, 706, 95, 1);		  // 水平线
	lineBres(495, 25, 495, 556, 2);	  // 垂直线
	lineBres(3, 3, 600, 600, 3);         // 45度斜线（m=1）
	lineBres(25, 575, 500, 100, 4);      // 45度斜线（m=-1)

	lineBres(172, 134, 525, 243, 10);	  // 0<m<1
	lineBres(222, 95, 521, 549, 10);	  // m>1
	lineBres(135, 300, 733, 139, 10);	  // -1<m<0
	lineBres(264, 487, 447, 47, 10);	  // m<-1

	glFlush();
}
void code_6_exercise_29()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_30
struct Point { int x; int y; };
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

inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}

void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}

}
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
					points.push_back({ { line.currentX , curY } });

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
					points.push_back({ { line.currentX, curY } });
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
						points.back().push_back({ line.currentX - 1, curY });
					else
						points.back().push_back({ line.currentX + 1, curY });

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
void lineRect(int x0, int y0, int xEnd, int yEnd, int width)
{
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float aa;
	int offset = Round(std::modf(width / 2, &aa)) == 0 ? 1 : 0;

	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;
	int vertexX = Round(std::cos(sita) * (width / 2));
	int vertexY = Round(std::sin(sita) * (width / 2));

	std::vector<Point> points;
	points.push_back({ x0 + vertexX, y0 + vertexY });
	points.push_back({ x0 - vertexX + (std::abs(vertexX) >= 1 ? offset : 0), y0 - vertexY + (std::abs(vertexY) >= 1 ? offset : 0) });
	points.push_back({ xEnd - vertexX + (std::abs(vertexX) >= 1 ? offset : 0), yEnd - vertexY + (std::abs(vertexY) >= 1 ? offset : 0) });
	points.push_back({ xEnd + vertexX, yEnd + vertexY });

	// 直线
	//glColor3f(1.0, 1.0, 1.0);
	//lineBres(x0, y0, xEnd, yEnd);

	// 线框
	//glColor3f(1.0, 0.0, 0.0);
	//lineBres(points[0].x, points[0].y, points[1].x, points[1].y);
	//lineBres(points[1].x, points[1].y, points[2].x, points[2].y);
	//lineBres(points[2].x, points[2].y, points[3].x, points[3].y);
	//lineBres(points[3].x, points[3].y, points[0].x, points[0].y);

	// 填充
	glColor3f(1.0, 1.0, 1.0);
	fillPolygon(points);
}

void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	lineRect(53, 95, 706, 95, 10);		  // 水平线			
	lineRect(495, 25, 495, 556, 10);	  // 垂直线
	lineRect(30, 30, 340, 340, 10);         // 45度斜线（m=1）
	lineRect(25, 575, 500, 100, 10);      // 45度斜线（m=-1)

	lineRect(172, 134, 525, 243, 10);	  // 0<m<1
	lineRect(222, 95, 521, 549, 10);	  // m>1
	lineRect(135, 300, 733, 139, 10);	  // -1<m<0
	lineRect(264, 487, 447, 47, 10);	  // m<-1

	glFlush();
}
void code_6_exercise_30()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_31
struct Point { int x; int y; };
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
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void hLine(int y, int x0, int x1)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(x, y);
	}
}
void setWidthPixelV(int x, int y, int width)
{
	int curWidthIdx = 0;
	for (int i = 0; i < width; i++)
	{
		if (i % 2)
			curWidthIdx++;
		if (i % 2)
			setPixel(x, y + curWidthIdx);
		else
			setPixel(x, y - curWidthIdx);
	}
}
void setWidthPixelH(int x, int y, int width)
{
	int curWidthIdx = 0;
	for (int i = 0; i < width; i++)
	{
		if (i % 2)
			curWidthIdx++;
		if (i % 2)
			setPixel(x + curWidthIdx, y);
		else
			setPixel(x - curWidthIdx, y);
	}
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setWidthPixelV(x, y, width);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setWidthPixelV(x, y, width);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setWidthPixelH(x, y, width);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setWidthPixelH(x, y, width);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setWidthPixelV(x, y, width);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setWidthPixelV(x, y, width);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setWidthPixelH(x, y, width);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setWidthPixelH(x, y, width);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, int width)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, width);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, width);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, width);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, width);
		}
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
					points.push_back({ { line.currentX , curY } });

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
					points.push_back({ { line.currentX, curY } });
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
						points.back().push_back({ line.currentX - 1, curY });
					else
						points.back().push_back({ line.currentX + 1, curY });

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
void lineWithFillPolygon(int x0, int y0, int xEnd, int yEnd, int width)
{
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float aa;
	int offset = Round(std::modf(width / 2, &aa)) == 0 ? 1 : 0;

	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;
	int vertexX = Round(std::cos(sita) * (width / 2));
	int vertexY = Round(std::sin(sita) * (width / 2));

	std::vector<Point> points;
	points.push_back({ x0 + vertexX, y0 + vertexY });
	points.push_back({ x0 - vertexX + (std::abs(vertexX) >= 1 ? offset : 0), y0 - vertexY + (std::abs(vertexY) >= 1 ? offset : 0) });
	points.push_back({ xEnd - vertexX + (std::abs(vertexX) >= 1 ? offset : 0), yEnd - vertexY + (std::abs(vertexY) >= 1 ? offset : 0) });
	points.push_back({ xEnd + vertexX, yEnd + vertexY });

	fillPolygon(points);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	lineBres(120, 200, 285, 280, 10);		  // 垂直区段填充
	lineBres(368, 185, 460, 395, 10);		  // 水平区段填充
	lineWithFillPolygon(700, 200, 500, 400, 10); // 填充多边形

	glFlush();
}
void code_6_exercise_31()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_32
struct Point2 { int x; int y; };
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
bool needPixel(int index, const std::string& lineTypeMode)
{
	return lineTypeMode[index % lineTypeMode.size()] == '1';
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);

	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}

		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);

	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int index = 0;
	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode="1")
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, lineTypeMode);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, lineTypeMode);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
}
void lineType(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	lineBres(x0, y0, xEnd, yEnd, lineTypeMode);
}
void drawWord(Point2 point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void drawCoordinate()
{
	lineBres(0, 0, 1000, 0);
	lineBres(0, 0, 0, 1000);
	drawWord({ 0, 3 }, "0");
	for (int i = 0; i < 7; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ (i + 1) * 100 - 15, 3 }, number);
	}
	drawWord({ 790, 3 }, "x");
	for (int i = 0; i < 5; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ 0, (i + 1) * 100 - 5 }, number);
	}
	drawWord({ 0, 590 }, "y");
	glFlush();
}
void drawDataSet(int x, int y, int width, const std::string& lineTypeMode, const std::string& title)
{
	std::vector<Point2> points = { { x - width / 2, 0},{ x + width / 2, 0 },{ x + width / 2, y },{ x - width / 2, y } };
	for (int i = 0; i < points.size(); i++)
	{
		int next = (i + 1) % points.size();
		lineType(points[i].x, points[i].y, points[next].x, points[next].y, lineTypeMode);
	}
	drawWord({x - (int)((float)title.size() / 2 * 9), y + 10}, title);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	drawDataSet(200, 200, 20, "1", "abcd");
	drawDataSet(400, 300, 20, "1100", "hello world");
	drawDataSet(600, 400, 20, "11111111000","sky");
	glFlush();
}
void code_6_exercise_32()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_33
struct Point2 { int x; int y; };
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
bool needPixel(int index, const std::string& lineTypeMode)
{
	return lineTypeMode[index % lineTypeMode.size()] == '1';
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);

	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}

		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);

	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int index = 0;

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int index = 0;
	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (needPixel(index, lineTypeMode))
		setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		if (needPixel(++index, lineTypeMode))
			setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode = "1")
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, lineTypeMode);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, lineTypeMode);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, lineTypeMode);
		}
	}
}
void lineType(int x0, int y0, int xEnd, int yEnd, const std::string& lineTypeMode)
{
	lineBres(x0, y0, xEnd, yEnd, lineTypeMode);
}
void drawWord(Point2 point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void drawCoordinate()
{
	lineBres(0, 0, 1000, 0);
	lineBres(0, 0, 0, 1000);
	drawWord({ 0, 3 }, "0");
	for (int i = 0; i < 7; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ (i + 1) * 100 - 15, 3 }, number);
	}
	drawWord({ 790, 3 }, "x");
	for (int i = 0; i < 5; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ 0, (i + 1) * 100 - 5 }, number);
	}
	drawWord({ 0, 590 }, "y");
	glFlush();
}
void drawDataSet(int x, int y, int width, const std::string& lineTypeMode, const std::string& title)
{
	std::vector<Point2> points = { { x - width / 2, 0 },{ x + width / 2, 0 },{ x + width / 2, y },{ x - width / 2, y } };
	for (int i = 0; i < points.size(); i++)
	{
		int next = (i + 1) % points.size();
		lineType(points[i].x, points[i].y, points[next].x, points[next].y, lineTypeMode);
	}
	drawWord({ x - (int)((float)title.size() / 2 * 9), y + 10 }, title);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	
	glColor3f(1.0, 0.0, 0.0);
	drawDataSet(200, 200, 20, "1", "ABCD");

	glColor3f(0.0, 1.0, 0.0);
	drawDataSet(400, 300, 20, "1100", "hello opengl");
	
	glColor3f(0.0, 0.0, 1.0);
	drawDataSet(600, 400, 20, "111100010001111", "xrevii");
	glFlush();
}
void code_6_exercise_33()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_34
struct Point { int x; int y; };
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
enum class RoundType
{
	All = 1,
	LeftHalf = 2,
	RightHalf = 3,
	TopHalf = 4,
	BottomHalf = 5,
};
enum class LineCap
{
	ButtCap = 1,
	RoundCap = 2,
	ProjectingSquareCap = 3,
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
// 中点画圆算法（逆时针0-45度圆）
//void circlePlotPoints(int xc, int yc, int x, int y)
//{
//	setPixel(xc + x, yc + y);
//	setPixel(xc - x, yc + y);
//	setPixel(xc + x, yc - y);
//	setPixel(xc - x, yc - y);
//	setPixel(xc + y, yc + x);
//	setPixel(xc - y, yc + x);
//	setPixel(xc + y, yc - x);
//	setPixel(xc - y, yc - x);
//}
//void circleMidpoint(int xc, int yc, int r)
//{
//	int x = r;
//	int y = 0;
//	int d2x = 2 * r;
//	int d2y = 0;
//	int p = 1 - r;
//	circlePlotPoints(xc, yc, x, y);
//	while (x > y)
//	{
//		y++;
//		d2y += 2;
//		if (p < 0)
//		{
//			p += d2y + 1;
//		}
//		else
//		{
//			x--;
//			d2x -= 2;
//			p += d2y + 1 -d2x;
//		}
//		circlePlotPoints(xc, yc, x, y);
//	}
//}
void hLine(int y, int x0, int x1)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(x, y);
	}
}
void hLineRound(int y, int x0, int x1, int xc, int yc, RoundType type, int offset = 0)
{
	for (int x = x0; x <= x1; x++)
	{
		switch (type)
		{
		case RoundType::All:
			setPixel(xc + x, yc + y);
			setPixel(xc - x + offset, yc + y);
			setPixel(xc + x, yc - y + offset);
			setPixel(xc - x + offset, yc - y + offset);
			setPixel(xc + y, yc + x);
			setPixel(xc - y + offset, yc + x);
			setPixel(xc + y, yc - x + offset);
			setPixel(xc - y + offset, yc - x + offset);
			break;
		case RoundType::LeftHalf:
			setPixel(xc - x + offset, yc + y);
			setPixel(xc - x + offset, yc - y + offset);
			setPixel(xc - y + offset, yc + x);
			setPixel(xc - y + offset, yc - x + offset);
			break;
		case RoundType::RightHalf:
			setPixel(xc + x, yc + y);
			setPixel(xc + x, yc - y + offset);
			setPixel(xc + y, yc + x);
			setPixel(xc + y, yc - x + offset);
			break;
		case RoundType::TopHalf:
			setPixel(xc + x, yc + y);
			setPixel(xc - x + offset, yc + y);
			setPixel(xc + y, yc + x);
			setPixel(xc - y + offset, yc + x);
			break;
		case RoundType::BottomHalf:
			setPixel(xc + x, yc - y + offset);
			setPixel(xc - x + offset, yc - y + offset);
			setPixel(xc + y, yc - x + offset);
			setPixel(xc - y + offset, yc - x + offset);
			break;
		default:
			break;
		}
	}

}
void fillRound(int xc, int yc, float r, RoundType type)
{
	int xline = 0;
	int xRound = r;

	int d2x = 2 * r;
	int d2y = 0;
	int p = Round((float)5 / 4 - r);
	double aa;
	int offset = Round(std::modf(r, &aa)) == 0 ? 1 : 0; // 如果是整数则偏移，保持几何特征.如果是小数(>=0.5)则不偏移
	hLineRound(0, 0, xRound, xc, yc, type, offset);
	int endY = Round(r / std::sqrt(2));
	for (int curY = 1; curY <= endY; curY++)
	{
		d2y += 2;
		if (p < 0)
		{
			p += d2y + 1;
		}
		else
		{
			xRound--;
			d2x -= 2;
			p += d2y + 1 - d2x;
		}
		hLineRound(curY, ++xline, xRound, xc, yc, type, offset);
	}
}
void setWidthPixelV(int x, int y, int width, float sita)
{
	int curWidthIdx = 0;
	for (int i = 0; i < width; i++)
	{
		if (i % 2)
			curWidthIdx++;
		if (i % 2)
			setPixel(x + Round(std::cos(sita) * curWidthIdx), y + Round(std::sin(sita) * curWidthIdx));
		else
			setPixel(x - Round(std::cos(sita) * curWidthIdx), y - Round(std::sin(sita) * curWidthIdx));
	}
}
void setWidthPixelH(int x, int y, int width, float sita)
{
	int curWidthIdx = 0;
	for (int i = 0; i < width; i++)
	{
		if (i % 2)
			curWidthIdx++;
		if (i % 2)
			setPixel(x + Round(std::cos(sita) * curWidthIdx), y + Round(std::sin(sita) * curWidthIdx));
		else
			setPixel(x - Round(std::cos(sita) * curWidthIdx), y - Round(std::sin(sita) * curWidthIdx));
	}
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, int width,float sita)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setWidthPixelV(x, y, width, sita);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setWidthPixelV(x, y, width, sita);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, int width, float sita)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setWidthPixelH(x, y, width, sita);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setWidthPixelH(x, y, width, sita);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, int width, float sita)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setWidthPixelV(x, y, width,sita);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setWidthPixelV(x, y, width, sita);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, int width, float sita)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setWidthPixelH(x, y, width, sita);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setWidthPixelH(x, y, width, sita);
	}
}
void lineBresWithLineCap(int x0, int y0, int xEnd, int yEnd, float width, LineCap cap)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	if (cap == LineCap::ProjectingSquareCap)
	{
		int dx = xEnd - x0;
		int dy = yEnd - y0;
		float rateX = dx / std::sqrt(dx * dx + dy * dy);
		float rateY = dy / std::sqrt(dx * dx + dy * dy);
		xEnd += rateX * (width / 2);
		yEnd += rateY * (width / 2);
		x0 -= rateX * (width / 2);
		y0 -= rateY * (width / 2);
	}

	int dx = xEnd - x0;
	int dy = yEnd - y0;

	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;

	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, width, sita);		
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, width,sita);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, width, sita);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, width, sita);
		}
	}
	if (cap == LineCap::RoundCap)
	{
		fillRound(x0, y0, width / 2, RoundType::All);
		fillRound(xEnd, yEnd, width / 2, RoundType::All);
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
					points.push_back({ { line.currentX , curY } });

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
					points.push_back({ { line.currentX, curY } });
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
						points.back().push_back({ line.currentX - 1, curY });
					else
						points.back().push_back({ line.currentX + 1, curY });

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
void lineFillPolygonWithLineCap(int x0, int y0, int xEnd, int yEnd, float width, LineCap cap)
{
	if (cap == LineCap::ProjectingSquareCap)
	{
		int dx = xEnd - x0;
		int dy = yEnd - y0;
		float rateX = dx / std::sqrt(dx * dx + dy * dy);
		float rateY = dy / std::sqrt(dx * dx + dy * dy);
		xEnd += rateX * (width / 2);
		yEnd += rateY * (width / 2);
		x0 -= rateX * (width / 2);
		y0 -= rateY * (width / 2);
	}

	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float aa;
	int offset = Round(std::modf(width / 2, &aa)) == 0 ? 1 : 0;

	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;
	int vertexX = Round(std::cos(sita) * (width / 2));
	int vertexY = Round(std::sin(sita) * (width / 2));

	std::vector<Point> points;
	points.push_back({ x0 + vertexX, y0 + vertexY });
	points.push_back({ x0 - vertexX + (std::abs(vertexX) >= 1 ? offset : 0), y0 - vertexY + (std::abs(vertexY) >= 1 ? offset : 0) });
	points.push_back({ xEnd - vertexX + (std::abs(vertexX) >= 1 ? offset : 0), yEnd - vertexY + (std::abs(vertexY) >= 1 ? offset : 0) });
	points.push_back({ xEnd + vertexX, yEnd + vertexY });

	fillPolygon(points);

	//glColor3f(1.0, 0.0, 0.0);
	if (cap == LineCap::RoundCap)
	{
		fillRound(x0, y0, width / 2, RoundType::All);
		fillRound(xEnd, yEnd, width / 2, RoundType::All);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	// 垂直沿线路径区段填充（填充不完整）
	lineBresWithLineCap(102, 381, 242, 500, 20, LineCap::ButtCap);
	lineBresWithLineCap(302, 381, 442, 500, 20, LineCap::RoundCap);
	lineBresWithLineCap(502, 381, 642, 500, 20, LineCap::ProjectingSquareCap);
	lineBresWithLineCap(650, 381, 750, 381, 20, LineCap::RoundCap);

	// 多边形填充
	lineFillPolygonWithLineCap(102, 81, 242, 200, 20, LineCap::ButtCap);
	lineFillPolygonWithLineCap(302, 81, 442, 200, 20, LineCap::RoundCap);
	lineFillPolygonWithLineCap(502, 81, 642, 200, 20, LineCap::ProjectingSquareCap);
	lineFillPolygonWithLineCap(650, 81, 750, 81, 20, LineCap::RoundCap);
	glFlush();
}
void code_6_exercise_34()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_35
float MinMiterAngel = PI / 6; // 最小的斜角连接角度（小于此值将切换为斜切连接）
struct Point { int x; int y; };
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
enum class RoundType
{
	All = 1,
	LeftHalf = 2,
	RightHalf = 3,
	TopHalf = 4,
	BottomHalf = 5,
};
enum class LineCap
{
	ButtCap = 1,	// 方帽
	RoundCap = 2,	// 圆帽
	ProjectingSquareCap = 3, // 突方帽
};
enum class BrokenLineJoin
{
	MiterJoin = 1,	// 斜角连接
	RoundJoin = 2,	// 圆连接
	BevelJoin = 3,	// 斜切连接
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void hLine(int y, int x0, int x1)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(x, y);
	}
}
void hLineRound(int y, int x0, int x1, int xc, int yc, RoundType type, int offset = 0)
{
	for (int x = x0; x <= x1; x++)
	{
		switch (type)
		{
		case RoundType::All:
			setPixel(xc + x, yc + y);
			setPixel(xc - x + offset, yc + y);
			setPixel(xc + x, yc - y + offset);
			setPixel(xc - x + offset, yc - y + offset);
			setPixel(xc + y, yc + x);
			setPixel(xc - y + offset, yc + x);
			setPixel(xc + y, yc - x + offset);
			setPixel(xc - y + offset, yc - x + offset);
			break;
		case RoundType::LeftHalf:
			setPixel(xc - x + offset, yc + y);
			setPixel(xc - x + offset, yc - y + offset);
			setPixel(xc - y + offset, yc + x);
			setPixel(xc - y + offset, yc - x + offset);
			break;
		case RoundType::RightHalf:
			setPixel(xc + x, yc + y);
			setPixel(xc + x, yc - y + offset);
			setPixel(xc + y, yc + x);
			setPixel(xc + y, yc - x + offset);
			break;
		case RoundType::TopHalf:
			setPixel(xc + x, yc + y);
			setPixel(xc - x + offset, yc + y);
			setPixel(xc + y, yc + x);
			setPixel(xc - y + offset, yc + x);
			break;
		case RoundType::BottomHalf:
			setPixel(xc + x, yc - y + offset);
			setPixel(xc - x + offset, yc - y + offset);
			setPixel(xc + y, yc - x + offset);
			setPixel(xc - y + offset, yc - x + offset);
			break;
		default:
			break;
		}
	}

}
void fillRound(int xc, int yc, float r, RoundType type)
{
	int xline = 0;
	int xRound = r;

	int d2x = 2 * r;
	int d2y = 0;
	int p = Round((float)5 / 4 - r);
	double aa;
	int offset = Round(std::modf(r, &aa)) == 0 ? 1 : 0; // 如果是整数则偏移，保持几何特征.如果是小数(>=0.5)则不偏移
	hLineRound(0, 0, xRound, xc, yc, type, offset);
	int endY = Round(r / std::sqrt(2));
	for (int curY = 1; curY <= endY; curY++)
	{
		d2y += 2;
		if (p < 0)
		{
			p += d2y + 1;
		}
		else
		{
			xRound--;
			d2x -= 2;
			p += d2y + 1 - d2x;
		}
		hLineRound(curY, ++xline, xRound, xc, yc, type, offset);
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
					points.push_back({ { line.currentX , curY } });

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
					points.push_back({ { line.currentX, curY } });
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
						points.back().push_back({ line.currentX - 1, curY });
					else
						points.back().push_back({ line.currentX + 1, curY });

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
void lineFillPolygonWithLineCap(int x0, int y0, int xEnd, int yEnd, float width, LineCap cap)
{
	if (cap == LineCap::ProjectingSquareCap)
	{
		int dx = xEnd - x0;
		int dy = yEnd - y0;
		float rateX = dx / std::sqrt(dx * dx + dy * dy);
		float rateY = dy / std::sqrt(dx * dx + dy * dy);
		xEnd += rateX * (width / 2);
		yEnd += rateY * (width / 2);
		x0 -= rateX * (width / 2);
		y0 -= rateY * (width / 2);
	}

	int dx = xEnd - x0;
	int dy = yEnd - y0;
	float aa;
	int offset = Round(std::modf(width / 2, &aa)) == 0 ? 1 : 0;

	float sita;
	if (dy)
	{
		sita = std::atan((float)-dx / dy);
		sita = sita + 2 * PI;
	}
	else
		sita = PI / 2;
	int vertexX = Round(std::cos(sita) * (width / 2));
	int vertexY = Round(std::sin(sita) * (width / 2));

	std::vector<Point> points;
	points.push_back({ x0 + vertexX, y0 + vertexY });
	points.push_back({ x0 - vertexX + (std::abs(vertexX) >= 1 ? offset : 0), y0 - vertexY + (std::abs(vertexY) >= 1 ? offset : 0) });
	points.push_back({ xEnd - vertexX + (std::abs(vertexX) >= 1 ? offset : 0), yEnd - vertexY + (std::abs(vertexY) >= 1 ? offset : 0) });
	points.push_back({ xEnd + vertexX, yEnd + vertexY });

	fillPolygon(points);

	if (cap == LineCap::RoundCap)
	{
		fillRound(x0, y0, width / 2, RoundType::All);
		fillRound(xEnd, yEnd, width / 2, RoundType::All);
	}
}
float calcAngle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	float xa = x0 - x1;
	float ya = y0 - y1;
	float xb = x2 - x1;
	float yb = y2 - y1;
	float cos = (xa * xb + ya * yb) / (std::sqrt(xa*xa+ya*ya)*std::sqrt(xb*xb + yb*yb));
	float ret = std::acos(cos);
	// 规整到最小"数值"(0到180或者0到-180)
	if (ret > PI)
	{
		ret -= 2 * PI;
	}
	else if (ret < -PI)
	{
		ret += 2 * PI;
	}
	return ret;
}
int crossProduct(const Point& vector1, const Point& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
void calcOutPoint(int x0, int y0, int x1, int y1, float width, int compCross, float& xOut, float& yOut)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;

	xOut = std::cos(sita) * (width / 2);
	yOut = std::sin(sita) * (width / 2);

	if (crossProduct({ x1 - x0, y1 - y0 }, { Round(xOut), Round(yOut) }) * compCross > 0)
	{
		xOut = -xOut;
		yOut = -yOut;
	}
}

void drawbrokenLineJoin(int x0, int y0, int x1, int y1, int x2, int y2, float width, BrokenLineJoin join)
{
	if (join == BrokenLineJoin::RoundJoin)
	{
		fillRound(x1, y1, width / 2, RoundType::All);
	}
	else
	{
		int cross = crossProduct({ x1 - x0, y1 - y0 }, { x2 - x1, y2 - y1 });
		float _x1, _y1, _x2, _y2;
		calcOutPoint(x0, y0, x1, y1, width, cross, _x1, _y1);
		calcOutPoint(x1, y1, x2, y2, width, cross, _x2, _y2);
		_x1 = x1 + _x1;
		_y1 = y1 + _y1;
		_x2 = x1 + _x2;
		_y2 = y1 + _y2;
		float x, y; // 外边缘交点
		if (x1 == x0 && x1 == x2)
		{
			return;
		}
		else if (x1 == x0)
		{
			float k2 = (float)(y1 - y2) / (x1 - x2);
			x = _x1;
			y = k2 * x - k2 * _x2 + _y2;
		}
		else if (x1 == x2)
		{
			float k1 = (float)(y1 - y0) / (x1 - x0);
			x = _x2;
			y = k1 * x - k1 * _x1 + _y1;
		}
		else
		{
			float k1 = (float)(y1 - y0) / (x1 - x0);
			float k2 = (float)(y1 - y2) / (x1 - x2);
			if (k1 == k2)
				return;

			x = (_y2 - _y1 + k1 * _x1 - k2 * _x2) / (k1 - k2);
			y = k2 * x - k2 * _x2 + _y2;
		}
		
		if (join == BrokenLineJoin::MiterJoin)
		{
			if (std::fabs(calcAngle(x0, y0, x1, y1, x2, y2)) <= MinMiterAngel)
			{
				fillPolygon({ { x1, y1 },{ Round(_x1),Round(_y1) },{ Round(_x2),Round(_y2) } });
			}
			else
			{
				fillPolygon({ { x1, y1 },{ Round(_x1),Round(_y1) },{ Round(x),Round(y) },{ Round(_x2),Round(_y2) } });
			}
		}
		else
		{
			fillPolygon({ { x1, y1 },{ Round(_x1),Round(_y1) },{ Round(_x2),Round(_y2) } });
		}			
	}
}
void brokenLineWithJoin(const std::vector<Point>& points, float width, BrokenLineJoin join)
{
	for (int i = 0; i < points.size(); i++)
	{
		if (i + 1 < points.size())
		{
			lineFillPolygonWithLineCap(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, width, LineCap::ButtCap);

			if (i + 2 < points.size())
			{
				drawbrokenLineJoin(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, points[i + 2].x, points[i + 2].y, width, join);
			}
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	brokenLineWithJoin({ { 100,500 },{ 266,464 },{ 114,353 },{ 196,323 } }, 20, BrokenLineJoin::MiterJoin);
	brokenLineWithJoin({ { 324,390 },{ 241,185 },{ 463,199 },{ 391,297 } }, 20, BrokenLineJoin::RoundJoin);
	brokenLineWithJoin({ { 572,419 },{ 572,252 },{ 636,343 },{ 729,269 } }, 20, BrokenLineJoin::BevelJoin);

	brokenLineWithJoin({ { 100, 40 },{ 200, 40 },{ 100,80 } }, 20, BrokenLineJoin::MiterJoin); // 小于30度
	brokenLineWithJoin({ { 250, 40 },{ 350, 40 },{ 250,140 } }, 20, BrokenLineJoin::MiterJoin); // 大于30度

	glFlush();
}
void code_6_exercise_35()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_36
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
void drawStencil(int x, int y, const Stencil& s, std::map<int, std::set<int>> & pixelCache)
{
	for (int i = 0; i < s.stencil.size(); i++)
	{
		for (int j = 0; j < s.stencil[i].size(); j++)
		{
			if (s.stencil[i][j])
			{
				int realx = x + j - s.xc;
				int realy = y + s.stencil.size() - 1 - i - s.yc;
				if ((pixelCache.find(realy) == pixelCache.end()) ||
					(pixelCache.find(realy) != pixelCache.end() && pixelCache[realy].find(realx) == pixelCache[realy].end()))
				{
					setPixel(realx, realy);
					pixelCache[realy].insert(realx);
				}
			}
		}
	}
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, const Stencil& s, std::map<int, std::set<int>> & pixelCache)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	drawStencil(x, y, s, pixelCache);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		drawStencil(x, y, s, pixelCache);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, const Stencil& s, std::map<int, std::set<int>> & pixelCache)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	drawStencil(x, y, s, pixelCache);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		drawStencil(x, y, s, pixelCache);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, const Stencil& s, std::map<int, std::set<int>> & pixelCache)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	drawStencil(x, y, s, pixelCache);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		drawStencil(x, y, s, pixelCache);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, const Stencil& s, std::map<int, std::set<int>> & pixelCache)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	drawStencil(x, y, s, pixelCache);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		drawStencil(x, y, s, pixelCache);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, const Stencil& s, std::map<int, std::set<int>> & pixelCache)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, s, pixelCache);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, s, pixelCache);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, s, pixelCache);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, s, pixelCache);
		}
	}
}

void drawStencil(int x, int y, const Stencil& s)
{
	for (int i = 0; i < s.stencil.size(); i++)
	{
		for (int j = 0; j < s.stencil[i].size(); j++)
		{
			if (s.stencil[i][j])
			{
				int realx = x + j - s.xc;
				int realy = y + s.stencil.size() - 1 - i - s.yc;
				setPixel(realx, realy);
			}
		}
	}
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, const Stencil& s)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
	int count = 0;

	int x = x0;
	int y = y0;
	drawStencil(x, y, s);
	while (x < xEnd)
	{
		x++;
		count++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		if (count >= s.stencil[0].size())
		{
			drawStencil(x, y, s);
			count = 0;
		}			
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, const Stencil& s)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int count = 0;
	int x = x0;
	int y = y0;

	drawStencil(x, y, s);
	while (y < yEnd)
	{
		y++;
		count++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		if (count >= s.stencil.size())
		{
			drawStencil(x, y, s);
			count = 0;
		}
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, const Stencil& s)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);
	int count = 0;

	int x = x0;
	int y = y0;

	drawStencil(x, y, s);
	while (x < xEnd)
	{
		x++;
		count++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		if (count >= s.stencil[0].size())
		{
			drawStencil(x, y, s);
			count = 0;
		}
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, const Stencil& s)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);
	int count = 0;

	int x = x0;
	int y = y0;

	drawStencil(x, y, s);
	while (y > yEnd)
	{
		y--;
		count++;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		if (count >= s.stencil.size())
		{
			drawStencil(x, y, s);
			count = 0;
		}
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, const Stencil& s)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, s);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, s);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, s);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, s);
		}
	}
}
void linePen(int x0, int y0, int xEnd, int yEnd, const Stencil& s)
{
	std::map<int, std::set<int>> pixelCache;
	lineBres(x0, y0, xEnd, yEnd, s, pixelCache);
}
void lineBrush(int x0, int y0, int xEnd, int yEnd, const Stencil& s)
{
	lineBres(x0, y0, xEnd, yEnd, s);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	Stencil s1 = {
	{
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
	}, 4, 4 };
	linePen(108, 451, 427, 278, s1);
	Stencil s2 = {
	{
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
	}, 4, 4 };
	lineBrush(308, 451, 627, 278, s2);
	glFlush();
}
void code_6_exercise_36()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_37
void setGrayPixel(int x, int y, float grayPercent)
{
	glColor3f(1.0 * grayPercent , 1.0 * grayPercent, 1.0 * grayPercent);
	setPixel(x, y);
}

// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, bool antiAliasing)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	if (dy == 0)
		antiAliasing = false;

	int x = x0;
	int y = y0;
	setGrayPixel(x, y, 1.f);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		float real = fabs((float)dy / dx * (x - x0) + y0);
		float cur = fabs(real - y);
		float top = fabs(y + 1 - real);
		float bottom = fabs(real - (y - 1));
		setGrayPixel(x, y, antiAliasing ? fabs(1 - cur) : 1.f);
		if (antiAliasing)
		{
			if (top < bottom)
				setGrayPixel(x, y + 1, fabs(1 - top));
			else
				setGrayPixel(x, y - 1, fabs(1 - bottom));
		}
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, bool antiAliasing)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	if (dx == 0)
		antiAliasing = false;

	setGrayPixel(x, y, 1.f);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		float real = fabs((float)dx / dy * (y - y0) + x0);
		float cur = fabs(real - x);
		float right = fabs(x + 1 - real);
		float left = fabs(real - (x - 1));
		setGrayPixel(x, y, antiAliasing ? fabs(1 - cur) : 1.f);
		if (antiAliasing)
		{
			if (right < left)
				setGrayPixel(x + 1, y, fabs(1 - right));
			else
				setGrayPixel(x - 1, y, fabs(1 - left));
		}		
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, bool antiAliasing)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	if (dy == 0)
		antiAliasing = false;

	setGrayPixel(x, y, 1.f);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		float real = fabs((float)dy / dx * (x - x0) + y0);
		float cur = fabs(real - y);
		float top = fabs(y + 1 - real);
		float bottom = fabs(real - (y - 1));
		setGrayPixel(x, y, antiAliasing ? fabs(1 - cur) : 1.f);
		if (antiAliasing)
		{
			if (top < bottom)
				setGrayPixel(x, y + 1, fabs(1 - top));
			else
				setGrayPixel(x, y - 1, fabs(1 - bottom));
		}
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, bool antiAliasing)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	if (dx == 0)
		antiAliasing = false;

	int x = x0;
	int y = y0;
	setGrayPixel(x, y, 1.f);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		float real = fabs((float)dx / dy * (y - y0) + x0);
		float cur = fabs(real - x);
		float right = fabs(x + 1 - real);
		float left = fabs(real - (x - 1));
		setGrayPixel(x, y, antiAliasing ? fabs(1 - cur) : 1.f);
		if (antiAliasing)
		{
			if (right < left)
				setGrayPixel(x + 1, y, fabs(1 - right));
			else
				setGrayPixel(x - 1, y, fabs(1 - left));
		}
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, bool antiAliasing)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, antiAliasing);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, antiAliasing);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, antiAliasing);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, antiAliasing);
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	// 0<m<1
	lineBres(78, 503, 408, 512, false);
	lineBres(78, 453, 408, 462, true);
	// m>1
	lineBres(533, 362, 539, 564, false);
	lineBres(583, 362, 589, 564, true);
	// -1<m<0
	lineBres(79, 319, 389, 279, false);
	lineBres(79, 269, 389, 229, true);
	// m<-1
	lineBres(488, 318, 553, 79, false);
	lineBres(538, 318, 603, 79, true);
	// 水平
	lineBres(42, 49, 242, 49, true);
	// 垂直
	lineBres(282, 22, 282, 170, true);

	glColor3f(1.0, 1.0, 1.0);
	glFlush();
}
void code_6_exercise_37()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_38
struct Point2 { int x; int y; };
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void ellipsePlotPoints(int x, int y, bool originSymmetryOnly, std::vector<std::list<Point2>>& points)
{
	points[0].push_back({x, y});

	if(!originSymmetryOnly)
		points[1].push_front({x, -y});

	points[2].push_back({-x, -y });

	if (!originSymmetryOnly)
		points[3].push_front({-x, y });
}
void ellipseMidpoint(int Rx, int Ry, std::vector<std::list<Point2>>& points)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2*y;
	ellipsePlotPoints(x, y, true, points);
	/*Region 1*/
	p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		ellipsePlotPoints(x, y, false, points);
	}
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
		ellipsePlotPoints(x, y, y == 0, points);
	}
}
std::list<Point2> makeAllEllipsePoints(std::vector<std::list<Point2>>& points)
{
	std::list<Point2> ret;
	ret.splice(ret.end(), points[0]);
	ret.splice(ret.end(), points[1]);
	ret.splice(ret.end(), points[2]);
	ret.splice(ret.end(), points[3]);
	return ret;
}
void draw(int xCenter, int yCenter, const std::list<Point2>& points, const std::string& lineTypeMode)
{
	int index = 0;
	for (auto it = points.begin(); it != points.end(); it++)
	{		
		if (lineTypeMode[index] == '1')
			setPixel(xCenter + it->x, yCenter + it->y);
		index = ++index % lineTypeMode.size();
	}
}
float calcRealRate(int x, int Rx, int Ry)
{
	if (Rx == std::abs(x))
		return 1.f;

	float tan = fabs((float)Ry * x / (Rx * std::sqrt(Rx * Rx - x * x)));
	if (tan < 1.0)
		return std::sqrt(1 / (1 + tan * tan)); // cos  斜率小于1时，曲线长度与x轴相等，所以调整长度需要参照x；反之，参照y
	else
		return std::sqrt(tan * tan / (1 + tan * tan)); // sin
}
std::string makeNewLineTypeMode(int x, int Rx, int Ry, const std::string& lineTypeMode)
{
	float realRate = calcRealRate(x, Rx, Ry);
	std::string newLineTypeMode;
	char curType = lineTypeMode[0];
	int curCount = 0;
	for (int i = 0;; i++)
	{
		// 退出条件
		if (i == lineTypeMode.size())
		{
			newLineTypeMode.append(Round(curCount * realRate), curType);
			break;
		}
		if (lineTypeMode[i] != curType)
		{
			newLineTypeMode.append(Round(curCount * realRate), curType);
			curType = lineTypeMode[i];
			curCount = 1;
		}
		else
		{
			curCount++;
		}
	}
	return newLineTypeMode;
}
void drawWithSlope(int xCenter, int yCenter, int Rx, int Ry, const std::list<Point2>& points, const std::string& lineTypeMode)
{
	int index = 0;
	std::string newLineTypeMode;
	for (auto it = points.begin(); it != points.end(); it++)
	{
		if (index >= newLineTypeMode.size())
		{
			newLineTypeMode = makeNewLineTypeMode(it->x, Rx, Ry, lineTypeMode);
			index = 0;
		}

		if (newLineTypeMode[index++] == '1')
			setPixel(xCenter + it->x, yCenter + it->y);
	}
}
// 固定像素数划线
void ellipseLineType1(int xCenter, int yCenter, int Rx, int Ry, const std::string& lineTypeMode)
{
	std::vector<std::list<Point2>> points = std::vector<std::list<Point2>>(4);
	ellipseMidpoint(Rx, Ry, points);
	std::list<Point2> ellipsePoints = makeAllEllipsePoints(points);
	draw(xCenter, yCenter, ellipsePoints, lineTypeMode);
}
// 固定长度划线
void ellipseLineType2(int xCenter, int yCenter, int Rx, int Ry, const std::string& lineTypeMode)
{
	std::vector<std::list<Point2>> points = std::vector<std::list<Point2>>(4);
	ellipseMidpoint(Rx, Ry, points);
	std::list<Point2> ellipsePoints = makeAllEllipsePoints(points);
	drawWithSlope(xCenter, yCenter, Rx, Ry, ellipsePoints, lineTypeMode);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	ellipseLineType1(150, 400, 100, 50, "1");// 实线
	ellipseLineType1(400, 400, 100, 50, "11111111000");// 划线
	ellipseLineType1(650, 400, 100, 50, "1100");// 点线

	ellipseLineType2(150, 200, 100, 50, "1");// 实线
	ellipseLineType2(400, 200, 100, 50, "11111111000");// 划线
	ellipseLineType2(650, 200, 100, 50, "1100");// 点线

	glFlush();
}
void code_6_exercise_38()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_39
struct Point2 { int x; int y; };
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void setWidthPixelV(int x, int y, int width)
{
	int curWidthIdx = 0;
	for (int i = 0; i < width; i++)
	{
		if (i % 2)
			curWidthIdx++;
		if (i % 2)
			setPixel(x, y + curWidthIdx);
		else
			setPixel(x, y - curWidthIdx);
	}
}
void setWidthPixelH(int x, int y, int width)
{
	int curWidthIdx = 0;
	for (int i = 0; i < width; i++)
	{
		if (i % 2)
			curWidthIdx++;
		if (i % 2)
			setPixel(x + curWidthIdx, y);
		else
			setPixel(x - curWidthIdx, y);
	}
}
void ellipsePlotPointsH(int xCenter, int yCenter, int x, int y, int width)
{
	setWidthPixelH(xCenter + x, yCenter + y, width);
	setWidthPixelH(xCenter - x, yCenter + y, width);
	setWidthPixelH(xCenter + x, yCenter - y, width);
	setWidthPixelH(xCenter - x, yCenter - y, width);
}
void ellipsePlotPointsV(int xCenter, int yCenter, int x, int y, int width)
{
	setWidthPixelV(xCenter + x, yCenter + y, width);
	setWidthPixelV(xCenter - x, yCenter + y, width);
	setWidthPixelV(xCenter + x, yCenter - y, width);
	setWidthPixelV(xCenter - x, yCenter - y, width);
}
void ellipseHV(int xCenter, int yCenter, int Rx, int Ry, int width)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2*y;
	ellipsePlotPointsV(xCenter, yCenter, x, y, width);
	/*Region 1*/
	p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		ellipsePlotPointsV(xCenter, yCenter, x, y, width);
	}
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
		ellipsePlotPointsH(xCenter, yCenter, x, y, width);
	}
}
void ellipsePoints(int Rx, int Ry, std::map<int, std::vector<Point2>>& points)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2*y;
	points[y].push_back({x, y});
	/*Region 1*/
	p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		points[y].push_back({ x, y });
	}
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
		points[y].push_back({ x, y });
	}
}
void hLineRound(int y, int x0, int x1, int xc, int yc)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(xc + x, yc + y);
		setPixel(xc - x, yc + y);
		setPixel(xc + x, yc - y);
		setPixel(xc - x, yc - y);
	}
}
void ellipseFillRect(int xCenter, int yCenter, int Rx, int Ry, int width)
{
	std::map<int, std::vector<Point2>> ellipseOuter;
	std::map<int, std::vector<Point2>> ellipseInner;
	ellipsePoints(Rx + width / 2, Ry + width / 2, ellipseOuter);
	ellipsePoints(Rx - (width - 1) / 2, Ry - (width - 1) / 2, ellipseInner);
	for (int i = 0; i <= Ry + width / 2; i++)
	{
		assert(ellipseOuter.find(i) != ellipseOuter.end());
		hLineRound(i, 
			ellipseInner.find(i) != ellipseInner.end() ? ellipseInner[i][0].x : 0,
			ellipseOuter[i][ellipseOuter[i].size() - 1].x,
			xCenter, yCenter);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	ellipseHV(200, 400, 100, 50, 10);
	ellipseFillRect(500, 400, 100, 50, 10);
	glFlush();
}
void code_6_exercise_39()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_40
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
struct Point { int x; int y; };
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

inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void hLine(int y, int x0, int x1, const Stencil& s, Point zeroPoint)
{
	for (int x = x0; x <= x1; x++)
	{
		int mapY = s.stencil.size() - 1 - (y - zeroPoint.y + s.yc) % s.stencil.size();
		int mapX = (x - zeroPoint.x + s.xc) % (int)(s.stencil[0].size());
		if(mapX < 0)
			mapX += s.stencil[0].size();

		if(s.stencil[mapY][mapX] == 1)
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
void fillWithActiveLines(int beginY, int endY, std::vector<ActiveLine>& activeLines, const Stencil& s, Point zeroPoint)
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
					points.push_back({ { line.currentX , curY } });

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
					points.push_back({ { line.currentX, curY } });
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
						points.back().push_back({ line.currentX - 1, curY });
					else
						points.back().push_back({ line.currentX + 1, curY });

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
				hLine(points[2 * i].front().y, points[2 * i].front().x, points[2 * i + 1].back().x, s, zeroPoint);
			}
			else
			{
				points.clear();
				break;
			}
		}
	}
}
void fillPolygon(const std::vector<Point>& points, const Stencil& s)
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
		fillWithActiveLines(curY, sortedLines[i + 1].scanY, activeLines, s, { sortedLines[0].sortedLines[0].beginX, sortedLines[0].scanY});
	}

}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	Stencil s1 = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		}, 0, 0 };
	fillPolygon({ {144,421},{295, 302},{301, 165}, {172,112},{87,249}}, s1);

	Stencil s2 = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		}, 4, 4 };
	fillPolygon({ { 420,139 },{ 575, 235 },{ 680, 154 },{ 723,311 },{ 582,419 },{443,404} }, s2);
	glFlush();
}
void code_6_exercise_40()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_41
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void hLineEllipsePlot(int y, int x0, int x1, int xc, int yc)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(xc + x, yc + y);
		setPixel(xc - x, yc + y);
		setPixel(xc + x, yc - y);
		setPixel(xc - x, yc - y);
	}
}
void fillEllipse(int xCenter, int yCenter, int Rx, int Ry)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2*y;
	//hLineEllipsePlot(y, 0, x, xCenter, yCenter); // y不变时，每次绘制扫描线会造成重复
	/*Region 1*/
	p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			hLineEllipsePlot(y, 0, x - 1, xCenter, yCenter); // 绘制上一条x最大的扫描线

			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		//hLineEllipsePlot(y, 0, x, xCenter, yCenter); // y不变时，每次绘制扫描线会造成重复
	}
	hLineEllipsePlot(y, 0, x, xCenter, yCenter); // 绘制最后一条扫描线

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
		hLineEllipsePlot(y, 0, x, xCenter, yCenter);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	fillEllipse(200, 200, 100, 50);
	glFlush();
}
void code_6_exercise_41()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_42
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
struct Point2 { int x; int y; };
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void hLine(int y, int x0, int x1, const Stencil& s, int xCenter, int yCenter)
{
	for (int x = x0; x <= x1; x++)
	{
		int _mapY = (y + s.yc) % (int)(s.stencil.size());
		if (_mapY < 0)
			_mapY += s.stencil.size();
		int mapY = s.stencil.size() - 1 - _mapY;

		int mapX = (x + s.xc) % (int)(s.stencil[0].size());
		if (mapX < 0)
			mapX += s.stencil[0].size();

		if (s.stencil[mapY][mapX] == 1)
			setPixel(xCenter + x, yCenter + y);
	}
}
void hLineEllipsePlot(int y, int x0, int x1, const Stencil& s, int xCenter, int yCenter)
{
	hLine(y, x0, x1, s, xCenter, yCenter);
	hLine(y, -x1, -x0, s, xCenter, yCenter);
	hLine(-y, x0, x1, s, xCenter, yCenter);
	hLine(-y, -x1, -x0, s, xCenter, yCenter);
}
void fillEllipse(int xCenter, int yCenter, int Rx, int Ry, const Stencil& s)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2*y;
	//hLineEllipsePlot(y, 0, x, xCenter, yCenter); // y不变时，每次绘制扫描线会造成重复
	/*Region 1*/
	p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			hLineEllipsePlot(y, 0, x - 1, s, xCenter, yCenter); // 绘制上一条x最大的扫描线

			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		//hLineEllipsePlot(y, 0, x, xCenter, yCenter); // y不变时，每次绘制扫描线会造成重复
	}
	hLineEllipsePlot(y, 0, x, s, xCenter, yCenter); // 绘制最后一条扫描线

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
		hLineEllipsePlot(y, 0, x, s, xCenter, yCenter);
	}
}
void coord(int xCenter, int yCenter)
{
	for (int x = 0; x < 800; x++)
	{
		setPixel(x, yCenter);
	}
	for (int y = 0; y < 600; y++)
	{
		setPixel(xCenter, y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	Stencil s1 = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		}, 0, 0 };
	fillEllipse(250, 400, 100, 50, s1);
	glColor3f(1.0, 0.0, 0.0);
	coord(250, 400);

	glColor3f(1.0, 1.0, 1.0);
	Stencil s2 = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		}, 4, 4 };
	fillEllipse(550, 400, 100, 50, s2);
	glColor3f(1.0, 0.0, 0.0);
	coord(550, 400);
	glFlush();
}
void code_6_exercise_42()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_43
struct Vector
{
	float x, y;
};
struct Point
{
	GLfloat x, y, z;
};
// 判断浮点数相等
bool Equal(float f1, float f2){ return std::abs(f1 - f2) < 0.0001;}
bool Greater(float f1, float f2) { return Equal(f1, f2) ? false : (f1 > f2); }
bool Less(float f1, float f2) { return Equal(f1, f2) ? false : (f1 < f2); }
bool GreaterQ(float f1, float f2) { return Greater(f1, f2) || Equal(f1, f2); }
bool LessQ(float f1, float f2) { return Less(f1, f2) || Equal(f1, f2); }
inline bool operator==(const Point& p1, const Point& p2)
{
	return Equal(p1.x, p2.x) && Equal(p1.y, p2.y) && Equal(p1.z, p2.z);
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return Less(p1.x, p2.x) && Less(p1.y, p2.y) && Less(p1.z, p2.z);
}
int crossProduct(const Vector& vector1, const Vector& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
// 计算两条直线的交点（直线公式：Ax+By+C=0）
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Point& point)
{
	if (Equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	point.z = 0;
	return true;
}
bool sameDir(Vector v1, Vector v2)
{
	return GreaterQ(v1.x * v2.x, 0) && GreaterQ(v1.y * v2.y, 0);
}
bool checkRayWay(Point checkPoint, Point rayPoint, const vector<Point>& ploygon)
{
	// rayPoint在包围盒外
	float Xmin = ploygon[0].x, Xmax = ploygon[0].x, Ymin = ploygon[0].y, Ymax = ploygon[0].y;
	for (auto& p : ploygon)
	{
		if (Greater(p.x, Xmax))
			Xmax = p.x;
		if (Less(p.x, Xmin))
			Xmin = p.x;
		if (Greater(p.y, Ymax))
			Ymax = p.y;
		if (Less(p.y, Ymin))
			Ymin = p.y;
	}
	if (LessQ(Xmin, rayPoint.x) && LessQ(rayPoint.x, Xmax) && LessQ(Ymin, rayPoint.y) && LessQ(rayPoint.y, Ymax))
		return false;

	for (auto& p : ploygon)
	{
		if (p == checkPoint)
			return true;
	}

	float rayK;
	bool v = false;
	if (!Equal(rayPoint.x, checkPoint.x))
	{
		rayK = (rayPoint.y - checkPoint.y) / (rayPoint.x - checkPoint.x);
		v = false;
	}
	else
	{
		rayK = 0.f;
		v = true;
	}

	for (auto& p : ploygon)
	{
		if (!Equal(p.x, checkPoint.x) && !v)
		{	
			float k = (p.y - checkPoint.y) / (p.x - checkPoint.x);
			if (Equal(k, rayK) && sameDir({ rayPoint.x - checkPoint.x, rayPoint.y - checkPoint.y }, { p.x - checkPoint.x, p.y - checkPoint.y }))
				return false;
		}
		else if(Equal(p.x, checkPoint.x) && v)
		{
			if(sameDir({ rayPoint.x - checkPoint.x, rayPoint.y - checkPoint.y }, { p.x - checkPoint.x, p.y - checkPoint.y }))
				return false;
		}
	}
	return true;
}
bool checkInner(Point checkPoint, Point rayPoint, const vector<Point>& ploygon)
{
	for (auto& p : ploygon)
	{
		if (p == checkPoint)
			return true;
	}

	int count = 0;
	float A1 = rayPoint.y - checkPoint.y;
	float B1 = checkPoint.x - rayPoint.x;
	float C1 = rayPoint.x * checkPoint.y - checkPoint.x * rayPoint.y;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;

		float A2 = ploygon[j].y - ploygon[i].y;
		float B2 = ploygon[i].x - ploygon[j].x;
		float C2 = ploygon[j].x * ploygon[i].y - ploygon[i].x * ploygon[j].y;
		Point point;
		if (linesPoint(A1, B1, C1, A2, B2, C2, point))
		{
			float EXmax = ploygon[j].x > ploygon[i].x ? ploygon[j].x : ploygon[i].x;
			float EXmin = ploygon[j].x > ploygon[i].x ? ploygon[i].x : ploygon[j].x;
			float EYmax = ploygon[j].y > ploygon[i].y ? ploygon[j].y : ploygon[i].y;
			float EYmin = ploygon[j].y > ploygon[i].y ? ploygon[i].y : ploygon[j].y;

			float CheckXmax = rayPoint.x > checkPoint.x ? rayPoint.x : checkPoint.x;
			float CheckXmin = rayPoint.x > checkPoint.x ? checkPoint.x : rayPoint.x;
			float CheckYmax = rayPoint.y > checkPoint.y ? rayPoint.y : checkPoint.y;
			float CheckYmin = rayPoint.y > checkPoint.y ? checkPoint.y : rayPoint.y;

			if (LessQ(EXmin, point.x) && LessQ(point.x, EXmax) &&
				LessQ(EYmin, point.y) && LessQ(point.y, EYmax) &&
				LessQ(CheckXmin, point.x) && LessQ(point.x, CheckXmax) &&
				LessQ(CheckYmin, point.y) && LessQ(point.y, CheckYmax))
			{
				if (point == checkPoint)
					return true;

				if (crossProduct({ rayPoint.x - checkPoint.x, rayPoint.y - checkPoint.y },
				{ ploygon[j].x - ploygon[i].x, ploygon[j].y - ploygon[i].y }) > 0)
				{
					count++;
				}
				else
				{
					count--;
				}
			}
		}
	}
	//printf("点(%f, %f)是%s\n", checkPoint.x, checkPoint.y, count != 0 ? "内点" : "外点");
	return count != 0;
}
void fillRect(const vector<Point>& ploygon)
{
	auto time0 = GetTickCount();
	float Xmin = ploygon[0].x, Xmax = ploygon[0].x, Ymin = ploygon[0].y, Ymax = ploygon[0].y;
	for (auto& p : ploygon)
	{
		if (Greater(p.x, Xmax))
			Xmax = p.x;
		if (Less(p.x, Xmin))
			Xmin = p.x;
		if (Greater(p.y, Ymax))
			Ymax = p.y;
		if (Less(p.y, Ymin))
			Ymin = p.y;
	}
	Point rayPoint = { ploygon[0].x + 100, ploygon[0].y + 100 };
	for (float x = Xmin; x <= Xmax; x++)
	{
		for (float y = Ymin; y <= Ymax; y++)
		{
			while (1)
			{
				if (checkRayWay({x, y}, rayPoint, ploygon))
					break;
				else
				{
					rayPoint.x += std::rand() % 100 + 100;
					rayPoint.y += std::rand() % 100 + 100;
				}
			}
			if (checkInner({ x, y }, rayPoint, ploygon))
				setPixel(x, y);		
		}
	}
	auto time1 = GetTickCount();
	printf("use time %ld\n", time1 - time0);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	fillRect({{ 112, 172 }, { 644, 243 }, { 574, 454 }, { 223, 393 }, { 259,108 }, { 417, 522 }, { 479, 338 }});

	glFlush();
}
void code_6_exercise_43()
{
	srand(time(NULL));
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_44
// 最小化崩溃的原因：点击最小化按钮之后，opengl重绘窗口，但是此时窗口的尺寸为最小化（0,0），即不能画出边界线，所以边界条件判断失败
// 但是点击任务栏最小化，opengl不重绘，所以不会出问题
struct Point { int x; int y; };
//#define SHOW_DRAW // 显示绘制过程
#ifdef SHOW_DRAW
int showTime = 1; //毫秒
#endif
struct Color3f
{
	GLfloat r, g, b;
};
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}
bool operator==(const Color3f& c1, const Color3f& c2)
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}
bool operator!=(const Color3f& c1, const Color3f& c2)
{
	return !(c1 == c2);
}
Color3f getPixelColor(int x, int y)
{
	static Color3f ret = {};
	ret = {};
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &ret);
	return ret;
}
void boundaryFill4(int x, int y, Color3f fillColor, Color3f borderColor)
{
	Color3f interiorColor = getPixelColor(x, y);
	
	if ((interiorColor != borderColor) && (interiorColor != fillColor))
	{
		setPixel(x, y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif
		boundaryFill4(x + 1, y, fillColor, borderColor);
		boundaryFill4(x - 1, y, fillColor, borderColor);
		boundaryFill4(x, y + 1, fillColor, borderColor);
		boundaryFill4(x, y - 1, fillColor, borderColor);
	}
}
Point findLineBeginPoint(Point curPoint, const Color3f& fillColor, const Color3f& borderColor)
{
	Point ret = curPoint;
	Color3f curColor = getPixelColor(ret.x, ret.y);
	while (curColor != fillColor && curColor != borderColor)
	{
		ret.x--;
		curColor = getPixelColor(ret.x, ret.y);
	}
	while (curColor == fillColor || curColor == borderColor)
	{
		ret.x++;
		curColor = getPixelColor(ret.x, ret.y);
	}
	return ret;
}
void stackLinePoints(Point begin, Point end, std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	Point curPoint = findLineBeginPoint(begin, fillColor, borderColor);
	if (curPoint.x <= end.x)
	{
		bool spaceRecording = false;
		while (1)
		{
			if (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
			{
				if (!spaceRecording)
				{
					spaceRecording = true;
					pointStack.push_front(curPoint);
				}				
			}
			else
			{
				spaceRecording = false;
			}

			if (curPoint.x >= end.x)
				return;

			curPoint.x++;
		}
	}
}
void fill4Connected(std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif		
		curPoint.x++;
	}

	stackLinePoints({ begin.x, begin.y - 1 }, { end.x, end.y - 1 }, pointStack, fillColor, borderColor);
	stackLinePoints({ begin.x, begin.y + 1 }, { end.x, end.y + 1 }, pointStack, fillColor, borderColor);
	fill4Connected(pointStack, fillColor, borderColor);
}
void boundaryFill4ByStack(int x, int y, Color3f fillColor, Color3f borderColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPoint({ x, y }, fillColor, borderColor));
	fill4Connected(pointStack, fillColor, borderColor);
}
void drawRect1()
{
	std::vector<Point> points = { {100, 100}, {200, 100}, {200, 200}, {100, 200} };
	for (int i = 0; i < points.size(); i ++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}
}
void drawRect2()
{
	std::vector<Point> points = { { 390, 425 },{ 390, 225 },{ 600, 225 },{ 600, 425 },{ 550, 425 } , { 500, 375 },{ 438, 375 } ,{ 438, 425 } };
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}	

	points = { { 450, 275 },{ 520, 275 } ,{ 520, 320 } ,{ 450, 320 }};
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	drawRect1();
	boundaryFill4(150, 150, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 });

	drawRect2();
	boundaryFill4ByStack(558, 300, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 });

	glFlush();	
}
void code_6_exercise_44()
{ 
	srand(time(NULL));
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_45
struct Point { int x; int y; };
//#define SHOW_DRAW // 显示绘制过程
#ifdef SHOW_DRAW
int showTime = 1; //毫秒
#endif
struct Color3f
{
	GLfloat r, g, b;
};
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}
bool operator==(const Color3f& c1, const Color3f& c2)
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}
bool operator!=(const Color3f& c1, const Color3f& c2)
{
	return !(c1 == c2);
}
Color3f getPixelColor(int x, int y)
{
	static Color3f ret = {};
	ret = {};
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &ret);
	return ret;
}
void boundaryFill4(int x, int y, Color3f fillColor, Color3f borderColor)
{
	Color3f interiorColor = getPixelColor(x, y);

	if ((interiorColor != borderColor) && (interiorColor != fillColor))
	{
		setPixel(x, y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif
		boundaryFill4(x + 1, y, fillColor, borderColor);
		boundaryFill4(x - 1, y, fillColor, borderColor);
		boundaryFill4(x, y + 1, fillColor, borderColor);
		boundaryFill4(x, y - 1, fillColor, borderColor);
	}
}
void boundaryFill8(int x, int y, Color3f fillColor, Color3f borderColor)
{
	Color3f interiorColor = getPixelColor(x, y);

	if ((interiorColor != borderColor) && (interiorColor != fillColor))
	{
		setPixel(x, y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif
		boundaryFill8(x + 1, y, fillColor, borderColor);
		boundaryFill8(x - 1, y, fillColor, borderColor);
		boundaryFill8(x, y + 1, fillColor, borderColor);
		boundaryFill8(x, y - 1, fillColor, borderColor);

		boundaryFill8(x - 1, y - 1, fillColor, borderColor);
		boundaryFill8(x + 1, y - 1, fillColor, borderColor);
		boundaryFill8(x + 1, y + 1, fillColor, borderColor);
		boundaryFill8(x - 1, y + 1, fillColor, borderColor);
	}
}
Point findLineBeginPoint(Point curPoint, const Color3f& fillColor, const Color3f& borderColor)
{
	Point ret = curPoint;
	Color3f curColor = getPixelColor(ret.x, ret.y);
	while (curColor != fillColor && curColor != borderColor)
	{
		ret.x--;
		curColor = getPixelColor(ret.x, ret.y);
	}
	while (curColor == fillColor || curColor == borderColor)
	{
		ret.x++;
		curColor = getPixelColor(ret.x, ret.y);
	}
	return ret;
}
void stackLinePoints(Point begin, Point end, std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	Point curPoint = findLineBeginPoint(begin, fillColor, borderColor);
	if (curPoint.x <= end.x)
	{
		bool spaceRecording = false;
		while (1)
		{
			if (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
			{
				if (!spaceRecording)
				{
					spaceRecording = true;
					pointStack.push_front(curPoint);
				}
			}
			else
			{
				spaceRecording = false;
			}

			if (curPoint.x >= end.x)
				return;

			curPoint.x++;
		}
	}
}
void fill4Connected(std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif	
		curPoint.x++;
	}

	stackLinePoints({ begin.x, begin.y - 1 }, { end.x, end.y - 1 }, pointStack, fillColor, borderColor);
	stackLinePoints({ begin.x, begin.y + 1 }, { end.x, end.y + 1 }, pointStack, fillColor, borderColor);
	fill4Connected(pointStack, fillColor, borderColor);
}
void boundaryFill4ByStack(int x, int y, Color3f fillColor, Color3f borderColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPoint({ x, y }, fillColor, borderColor));
	fill4Connected(pointStack, fillColor, borderColor);
}
void fill8Connected(std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif		
		curPoint.x++;
	}

	stackLinePoints({ begin.x - 1, begin.y - 1 }, { end.x + 1, end.y - 1 }, pointStack, fillColor, borderColor);
	stackLinePoints({ begin.x - 1, begin.y + 1 }, { end.x + 1, end.y + 1 }, pointStack, fillColor, borderColor);
	fill8Connected(pointStack, fillColor, borderColor);
}
void boundaryFill8ByStack(int x, int y, Color3f fillColor, Color3f borderColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPoint({ x, y }, fillColor, borderColor));
	fill8Connected(pointStack, fillColor, borderColor);
}
void drawRect(const std::vector<Point>& points)
{
	
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> points1 = { { 100, 350 },{ 151, 350 },{ 151, 399 },{ 200, 399 },{ 200, 450 },{ 150, 450 },{ 150, 400 },{ 100, 400 } };
	std::vector<Point> points2 = { { 300, 350 },{ 351, 350 },{ 351, 399 },{ 400, 399 },{ 400, 450 },{ 350, 450 },{ 350, 400 },{ 300, 400 } };
	std::vector<Point> points3 = { { 100, 150 },{ 151, 150 },{ 151, 199 },{ 200, 199 },{ 200, 250 },{ 150, 250 },{ 150, 200 },{ 100, 200 } };
	std::vector<Point> points4 = { { 300, 150 },{ 351, 150 },{ 351, 199 },{ 400, 199 },{ 400, 250 },{ 350, 250 },{ 350, 200 },{ 300, 200 } };
	// 4联通
	drawRect(points1);
	boundaryFill4(120, 370, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 });

	// 8联通
	drawRect(points2);
	boundaryFill8(320, 370, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 });

	// 4联通（使用堆栈）
	drawRect(points3);
	boundaryFill4ByStack(120, 170, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 });
	
	// 8联通（使用堆栈）
	drawRect(points4);
	boundaryFill8ByStack(320, 170, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 });

	glFlush();
}
void code_6_exercise_45()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_46
struct Point { int x; int y; };
#define SHOW_DRAW // 显示绘制过程
#ifdef SHOW_DRAW
int showTime = 1; //毫秒
#endif
struct Color3f
{
	GLfloat r, g, b;
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void ellipseMidpoint(int xCenter, int yCenter, int Rx, int Ry)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2*y;
	void ellipsePlotPoints(int, int, int, int);
	ellipsePlotPoints(xCenter, yCenter, x, y);
	/*Region 1*/
	p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
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
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
}
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixel(xCenter + x, yCenter + y);
	setPixel(xCenter - x, yCenter + y);
	setPixel(xCenter + x, yCenter - y);
	setPixel(xCenter - x, yCenter - y);
}
bool operator==(const Color3f& c1, const Color3f& c2)
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}
bool operator!=(const Color3f& c1, const Color3f& c2)
{
	return !(c1 == c2);
}
Color3f getPixelColor(int x, int y)
{
	static Color3f ret = {};
	ret = {};
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &ret);
	return ret;
}
void boundaryFill4(int x, int y, Color3f fillColor, Color3f borderColor)
{
	Color3f interiorColor = getPixelColor(x, y);

	if ((interiorColor != borderColor) && (interiorColor != fillColor))
	{
		setPixel(x, y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif
		boundaryFill4(x + 1, y, fillColor, borderColor);
		boundaryFill4(x - 1, y, fillColor, borderColor);
		boundaryFill4(x, y + 1, fillColor, borderColor);
		boundaryFill4(x, y - 1, fillColor, borderColor);
	}
}
Point findLineBeginPoint(Point curPoint, const Color3f& fillColor, const Color3f& borderColor)
{
	Point ret = curPoint;
	Color3f curColor = getPixelColor(ret.x, ret.y);
	while (curColor != fillColor && curColor != borderColor)
	{
		ret.x--;
		curColor = getPixelColor(ret.x, ret.y);
	}
	while (curColor == fillColor || curColor == borderColor)
	{
		ret.x++;
		curColor = getPixelColor(ret.x, ret.y);
	}
	return ret;
}
void stackLinePoints(Point begin, Point end, std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	Point curPoint = findLineBeginPoint(begin, fillColor, borderColor);
	if (curPoint.x <= end.x)
	{
		bool spaceRecording = false;
		while (1)
		{
			if (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
			{
				if (!spaceRecording)
				{
					spaceRecording = true;
					pointStack.push_front(curPoint);
				}
			}
			else
			{
				spaceRecording = false;
			}

			if (curPoint.x >= end.x)
				return;

			curPoint.x++;
		}
	}
}
void fill4Connected(std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif		
		curPoint.x++;
	}

	stackLinePoints({ begin.x, begin.y - 1 }, { end.x, end.y - 1 }, pointStack, fillColor, borderColor);
	stackLinePoints({ begin.x, begin.y + 1 }, { end.x, end.y + 1 }, pointStack, fillColor, borderColor);
	fill4Connected(pointStack, fillColor, borderColor);
}
void boundaryFill4ByStack(int x, int y, Color3f fillColor, Color3f borderColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPoint({ x, y }, fillColor, borderColor));
	fill4Connected(pointStack, fillColor, borderColor);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	// 4联通
	ellipseMidpoint(200, 300, 100, 50);
	boundaryFill4(200, 300, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 });

	// 4联通（使用堆栈）
	ellipseMidpoint(500, 300, 100, 50);
	boundaryFill4ByStack(500, 300, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 });

	glFlush();	
}
void code_6_exercise_46()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_47
struct Point { int x; int y; };
#define SHOW_DRAW // 显示绘制过程
#ifdef SHOW_DRAW
int showTime = 1; //毫秒
#endif
struct Color3f
{
	GLfloat r, g, b;
};
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void ellipseMidpoint(int xCenter, int yCenter, int Rx, int Ry)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2*y;
	void ellipsePlotPoints(int, int, int, int);
	ellipsePlotPoints(xCenter, yCenter, x, y);
	/*Region 1*/
	p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
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
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
}
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixel(xCenter + x, yCenter + y);
	setPixel(xCenter - x, yCenter + y);
	setPixel(xCenter + x, yCenter - y);
	setPixel(xCenter - x, yCenter - y);
}
bool operator==(const Color3f& c1, const Color3f& c2)
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}
bool operator!=(const Color3f& c1, const Color3f& c2)
{
	return !(c1 == c2);
}
Color3f getPixelColor(int x, int y)
{
	static Color3f ret = {};
	ret = {};
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &ret);
	return ret;
}
void floodFill4(int x, int y, Color3f fillColor, Color3f interiorColor)
{
	Color3f color = getPixelColor(x, y);

	if (color == interiorColor)
	{
		setPixel(x, y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif
		floodFill4(x + 1, y, fillColor, interiorColor);
		floodFill4(x - 1, y, fillColor, interiorColor);
		floodFill4(x, y + 1, fillColor, interiorColor);
		floodFill4(x, y - 1, fillColor, interiorColor);
	}
}
void floodFill8(int x, int y, Color3f fillColor, Color3f interiorColor)
{
	Color3f color = getPixelColor(x, y);

	if (color == interiorColor)
	{
		setPixel(x, y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif
		floodFill8(x + 1, y, fillColor, interiorColor);
		floodFill8(x - 1, y, fillColor, interiorColor);
		floodFill8(x, y + 1, fillColor, interiorColor);
		floodFill8(x, y - 1, fillColor, interiorColor);

		floodFill8(x - 1, y - 1, fillColor, interiorColor);
		floodFill8(x + 1, y - 1, fillColor, interiorColor);
		floodFill8(x + 1, y + 1, fillColor, interiorColor);
		floodFill8(x - 1, y + 1, fillColor, interiorColor);
	}
}
Point findLineBeginPoint(Point begin, Point end, const Color3f& interiorColor)
{
	Point ret = begin;
	Color3f curColor = getPixelColor(ret.x, ret.y);
	while (curColor == interiorColor)
	{
		ret.x--;
		curColor = getPixelColor(ret.x, ret.y);
	}
	while (curColor != interiorColor && ret.x <= end.x)
	{
		ret.x++;
		curColor = getPixelColor(ret.x, ret.y);
	}
	return ret;
}
void stackLinePoints(Point begin, Point end, std::list<Point>& pointStack, const Color3f& interiorColor)
{
	Point curPoint = findLineBeginPoint(begin, end, interiorColor);
	if (curPoint.x <= end.x)
	{
		bool spaceRecording = false;
		while (1)
		{
			if (getPixelColor(curPoint.x, curPoint.y) == interiorColor)
			{
				if (!spaceRecording)
				{
					spaceRecording = true;
					pointStack.push_front(curPoint);
				}
			}
			else
			{
				spaceRecording = false;
			}

			if (curPoint.x >= end.x)
				return;

			curPoint.x++;
		}
	}
}
void fill4Connected(std::list<Point>& pointStack, const Color3f& interiorColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) == interiorColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif		
		curPoint.x++;
	}

	stackLinePoints({ begin.x, begin.y - 1 }, { end.x, end.y - 1 }, pointStack, interiorColor);
	stackLinePoints({ begin.x, begin.y + 1 }, { end.x, end.y + 1 }, pointStack, interiorColor);
	fill4Connected(pointStack, interiorColor);
}
void floodFill4ByStack(int x, int y, Color3f fillColor, Color3f interiorColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPoint({ x, y }, {9999, 9999}, interiorColor));
	fill4Connected(pointStack, interiorColor);
}
void fill8Connected(std::list<Point>& pointStack, const Color3f& interiorColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) == interiorColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif		
		curPoint.x++;
	}

	stackLinePoints({ begin.x - 1, begin.y - 1 }, { end.x + 1, end.y - 1 }, pointStack, interiorColor);
	stackLinePoints({ begin.x - 1, begin.y + 1 }, { end.x + 1, end.y + 1 }, pointStack, interiorColor);
	fill8Connected(pointStack, interiorColor);
}
void floodFill8ByStack(int x, int y, Color3f fillColor, Color3f interiorColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPoint({ x, y }, { 9999, 9999 }, interiorColor));
	fill8Connected(pointStack, interiorColor);
}
void drawRect(const std::vector<Point>& points, const std::vector<Color3f>& colors)
{
	for (int i = 0, j = 0; i < points.size(); i++, j++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		auto color = colors[j = j >= colors.size() ? 0 : j];
		glColor3f(color.r, color.g, color.b);
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> points1 = { { 100, 350 },{ 151, 350 },{ 151, 399 },{ 200, 399 },{ 200, 450 },{ 150, 450 },{ 150, 400 },{ 100, 400 } };
	std::vector<Point> points2 = { { 300, 350 },{ 351, 350 },{ 351, 399 },{ 400, 399 },{ 400, 450 },{ 350, 450 },{ 350, 400 },{ 300, 400 } };
	std::vector<Point> points3 = { { 100, 150 },{ 151, 150 },{ 151, 199 },{ 200, 199 },{ 200, 250 },{ 150, 250 },{ 150, 200 },{ 100, 200 } };
	std::vector<Point> points4 = { { 300, 150 },{ 351, 150 },{ 351, 199 },{ 400, 199 },{ 400, 250 },{ 350, 250 },{ 350, 200 },{ 300, 200 } };
	std::vector<Color3f> colors = { { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f } };
	// 4联通
	drawRect(points1, colors);
	glColor3f(1.0, 1.0, 1.0);
	floodFill4(120, 370, { 1.0, 1.0, 1.0 }, { 0.0, 0.0, 0.0 });

	// 8联通
	drawRect(points2, colors);
	glColor3f(1.0, 1.0, 1.0);
	floodFill8(320, 370, { 1.0, 1.0, 1.0 }, { 0.0, 0.0, 0.0 });

	// 4联通（使用堆栈）
	drawRect(points3, colors);
	glColor3f(1.0, 1.0, 1.0);
	floodFill4ByStack(120, 170, { 1.0, 1.0, 1.0 }, { 0.0, 0.0, 0.0 });

	// 8联通（使用堆栈）
	drawRect(points4, colors);
	glColor3f(1.0, 1.0, 1.0);
	floodFill8ByStack(320, 170, { 1.0, 1.0, 1.0 }, { 0.0, 0.0, 0.0 });

	// 4联通（使用堆栈）
	glColor3f(1.0, 0.0, 0.0);
	ellipseMidpoint(600, 300, 100, 50);
	glColor3f(1.0, 1.0, 1.0);
	floodFill4ByStack(600, 300, { 1.0, 1.0, 1.0 }, { 0.0, 0.0, 0.0 });

	glFlush();
}
void code_6_exercise_47()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_48
struct Point { int x; int y; };
#define SHOW_DRAW // 显示绘制过程
#ifdef SHOW_DRAW
int showTime = 1; //毫秒
#endif
struct Color3f
{
	GLfloat r, g, b;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
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
struct LineRecord
{
	int beginX;
	int endX;
};

inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}
void setPixelByStencil(int x, int y, const Stencil& s, Point zeroPoint)
{
	int mapY = s.stencil.size() - 1 - (y - zeroPoint.y + s.yc) % s.stencil.size();
	int mapX = (x - zeroPoint.x + s.xc) % (int)(s.stencil[0].size());
	if (mapX < 0)
		mapX += s.stencil[0].size();

	if (s.stencil[mapY][mapX] == 1)
	{
		setPixel(x, y);
#ifdef SHOW_DRAW
		glFlush();
		Sleep(showTime);
#endif
	}
}
void hLine(int y, int x0, int x1, const Stencil& s, Point zeroPoint)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixelByStencil(x, y, s, zeroPoint);
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
void fillWithActiveLines(int beginY, int endY, std::vector<ActiveLine>& activeLines, const Stencil& s, Point zeroPoint)
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
					points.push_back({ { line.currentX , curY } });

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
					points.push_back({ { line.currentX, curY } });
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
						points.back().push_back({ line.currentX - 1, curY });
					else
						points.back().push_back({ line.currentX + 1, curY });

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
				hLine(points[2 * i].front().y, points[2 * i].front().x, points[2 * i + 1].back().x, s, zeroPoint);
			}
			else
			{
				points.clear();
				break;
			}
		}
	}
}
void fillPolygon(const std::vector<Point>& points, const Stencil& s, Point zeroPoint)
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
		fillWithActiveLines(curY, sortedLines[i + 1].scanY, activeLines, s, zeroPoint);
	}
}
bool operator==(const Color3f& c1, const Color3f& c2)
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}
bool operator!=(const Color3f& c1, const Color3f& c2)
{
	return !(c1 == c2);
}
Color3f getPixelColor(int x, int y)
{
	static Color3f ret = {};
	ret = {};
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &ret);
	return ret;
}
bool newPixel(int x, int y, const Color3f& fillColor, const Color3f& borderColor, std::map<int, std::vector<LineRecord>>& lineRecord)
{
	if (lineRecord.find(y) != lineRecord.end())
	{
		for (auto& lineR : lineRecord[y])
		{
			if (lineR.beginX <= x && x <= lineR.endX)
				return false;
		}
	}

	return getPixelColor(x, y) != fillColor && getPixelColor(x, y) != borderColor;
}
Point findLineBeginPoint(Point curPoint, const Color3f& fillColor, const Color3f& borderColor, std::map<int, std::vector<LineRecord>>& lineRecord)
{
	Point ret = curPoint;
	while (newPixel(ret.x, ret.y, fillColor, borderColor, lineRecord))
	{
		ret.x--;
	}
	while (!newPixel(ret.x, ret.y, fillColor, borderColor, lineRecord))
	{
		ret.x++;
	}
	return ret;
}
void stackLinePoints(Point begin, Point end, std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor, std::map<int, std::vector<LineRecord>>& lineRecord)
{
	Point curPoint = findLineBeginPoint(begin, fillColor, borderColor, lineRecord);
	if (curPoint.x <= end.x)
	{
		bool spaceRecording = false;
		while (1)
		{
			if (newPixel(curPoint.x, curPoint.y, fillColor, borderColor, lineRecord))
			{
				if (!spaceRecording)
				{
					spaceRecording = true;
					pointStack.push_front(curPoint);
				}
			}
			else
			{
				spaceRecording = false;
			}

			if (curPoint.x >= end.x)
				return;

			curPoint.x++;
		}
	}
}
void fill4Connected(std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor, std::map<int, std::vector<LineRecord>>& lineRecord, const Stencil& s, Point zeroPoint)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (newPixel(curPoint.x, curPoint.y, fillColor, borderColor, lineRecord))
	{
		end = curPoint;
		//setPixel(curPoint.x, curPoint.y);
		setPixelByStencil(curPoint.x, curPoint.y, s, zeroPoint);		
		curPoint.x++;
	}
	lineRecord[begin.y].push_back({ begin.x, end.x });

	stackLinePoints({ begin.x, begin.y - 1 }, { end.x, end.y - 1 }, pointStack, fillColor, borderColor, lineRecord);
	stackLinePoints({ begin.x, begin.y + 1 }, { end.x, end.y + 1 }, pointStack, fillColor, borderColor, lineRecord);
	fill4Connected(pointStack, fillColor, borderColor, lineRecord, s, zeroPoint);
}
void boundaryFill4ByStack(int x, int y, Color3f fillColor, Color3f borderColor, const Stencil& s, Point zeroPoint)
{
	std::list<Point> pointStack;
	std::map<int, std::vector<LineRecord>> lineRecord;
	pointStack.push_front(findLineBeginPoint({ x, y }, fillColor, borderColor, lineRecord));
	fill4Connected(pointStack, fillColor, borderColor, lineRecord, s, zeroPoint);
}
void fill8Connected(std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor, std::map<int, std::vector<LineRecord>>& lineRecord, const Stencil& s, Point zeroPoint)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (newPixel(curPoint.x, curPoint.y, fillColor, borderColor, lineRecord))
	{
		end = curPoint;
		//setPixel(curPoint.x, curPoint.y);
		setPixelByStencil(curPoint.x, curPoint.y, s, zeroPoint);	
		curPoint.x++;
	}
	lineRecord[begin.y].push_back({ begin.x, end.x });
	stackLinePoints({ begin.x - 1, begin.y - 1 }, { end.x + 1, end.y - 1 }, pointStack, fillColor, borderColor, lineRecord);
	stackLinePoints({ begin.x - 1, begin.y + 1 }, { end.x + 1, end.y + 1 }, pointStack, fillColor, borderColor, lineRecord);
	fill8Connected(pointStack, fillColor, borderColor, lineRecord, s, zeroPoint);
}
void boundaryFill8ByStack(int x, int y, Color3f fillColor, Color3f borderColor, const Stencil& s, Point zeroPoint)
{
	std::list<Point> pointStack;
	std::map<int, std::vector<LineRecord>> lineRecord;
	pointStack.push_front(findLineBeginPoint({ x, y }, fillColor, borderColor, lineRecord));
	fill8Connected(pointStack, fillColor, borderColor, lineRecord, s, zeroPoint);
}
bool newPixelFlood(int x, int y, const Color3f& interiorColor, std::map<int, std::vector<LineRecord>>& lineRecord)
{
	if (lineRecord.find(y) != lineRecord.end())
	{
		for (auto& lineR : lineRecord[y])
		{
			if (lineR.beginX <= x && x <= lineR.endX)
				return false;
		}
	}

	return getPixelColor(x, y) == interiorColor;
}
Point findLineBeginPointFlood(Point begin, Point end, const Color3f& interiorColor, std::map<int, std::vector<LineRecord>>& lineRecord)
{
	Point ret = begin;
	while (newPixelFlood(ret.x, ret.y, interiorColor, lineRecord))
	{
		ret.x--;
	}
	while (!newPixelFlood(ret.x, ret.y, interiorColor, lineRecord) && ret.x <= end.x)
	{
		ret.x++;
	}
	return ret;
}
void stackLinePointsFlood(Point begin, Point end, std::list<Point>& pointStack, const Color3f& interiorColor, std::map<int, std::vector<LineRecord>>& lineRecord)
{
	Point curPoint = findLineBeginPointFlood(begin, end, interiorColor, lineRecord);
	if (curPoint.x <= end.x)
	{
		bool spaceRecording = false;
		while (1)
		{
			if (newPixelFlood(curPoint.x, curPoint.y, interiorColor, lineRecord))
			{
				if (!spaceRecording)
				{
					spaceRecording = true;
					pointStack.push_front(curPoint);
				}
			}
			else
			{
				spaceRecording = false;
			}

			if (curPoint.x >= end.x)
				return;

			curPoint.x++;
		}
	}
}
void fill4ConnectedFlood(std::list<Point>& pointStack, const Color3f& interiorColor, std::map<int, std::vector<LineRecord>>& lineRecord, const Stencil& s, Point zeroPoint)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (newPixelFlood(curPoint.x, curPoint.y, interiorColor, lineRecord))
	{
		end = curPoint;
		//setPixel(curPoint.x, curPoint.y);
		setPixelByStencil(curPoint.x, curPoint.y, s, zeroPoint);		
		curPoint.x++;
	}
	lineRecord[begin.y].push_back({ begin.x, end.x });

	stackLinePointsFlood({ begin.x, begin.y - 1 }, { end.x, end.y - 1 }, pointStack, interiorColor, lineRecord);
	stackLinePointsFlood({ begin.x, begin.y + 1 }, { end.x, end.y + 1 }, pointStack, interiorColor, lineRecord);
	fill4ConnectedFlood(pointStack, interiorColor, lineRecord, s, zeroPoint);
}
void floodFill4ByStack(int x, int y, Color3f interiorColor, const Stencil& s, Point zeroPoint)
{
	std::list<Point> pointStack;
	std::map<int, std::vector<LineRecord>> lineRecord;
	pointStack.push_front(findLineBeginPointFlood({ x, y }, {9999, 9999}, interiorColor, lineRecord));
	fill4ConnectedFlood(pointStack, interiorColor, lineRecord, s, zeroPoint);
}
void fill8ConnectedFlood(std::list<Point>& pointStack, const Color3f& interiorColor, std::map<int, std::vector<LineRecord>>& lineRecord, const Stencil& s, Point zeroPoint)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (newPixelFlood(curPoint.x, curPoint.y, interiorColor, lineRecord))
	{
		end = curPoint;
		//setPixel(curPoint.x, curPoint.y);
		setPixelByStencil(curPoint.x, curPoint.y, s, zeroPoint);	
		curPoint.x++;
	}
	lineRecord[begin.y].push_back({ begin.x, end.x });
	stackLinePointsFlood({ begin.x - 1, begin.y - 1 }, { end.x + 1, end.y - 1 }, pointStack, interiorColor, lineRecord);
	stackLinePointsFlood({ begin.x - 1, begin.y + 1 }, { end.x + 1, end.y + 1 }, pointStack, interiorColor, lineRecord);
	fill8ConnectedFlood(pointStack, interiorColor, lineRecord, s, zeroPoint);
}
void floodFill8ByStack(int x, int y, Color3f interiorColor, const Stencil& s, Point zeroPoint)
{
	std::list<Point> pointStack;
	std::map<int, std::vector<LineRecord>> lineRecord;
	pointStack.push_front(findLineBeginPointFlood({ x, y }, { 9999, 9999 }, interiorColor, lineRecord));
	fill8ConnectedFlood(pointStack, interiorColor, lineRecord, s, zeroPoint);
}
void drawRect(const std::vector<Point>& points)
{
	for (int i = 0, j = 0; i < points.size(); i++, j++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}
}
void drawRect(const std::vector<Point>& points, const std::vector<Color3f>& colors)
{
	for (int i = 0, j = 0; i < points.size(); i++, j++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		auto color = colors[j = j >= colors.size() ? 0 : j];
		glColor3f(color.r, color.g, color.b);
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> points1 = { {76, 207},{125, 150},{185, 205},{235, 150},{220, 313},{155, 425} };
	std::vector<Point> points2 = { { 350, 350 },{ 401, 350 },{ 401, 399 },{ 450, 399 },{ 450, 450 },{ 400, 450 },{ 400, 400 },{ 350, 400 } };
	std::vector<Point> points3 = { { 550, 350 },{ 601, 350 },{ 601, 399 },{ 650, 399 },{ 650, 450 },{ 600, 450 },{ 600, 400 },{ 550, 400 } };
	std::vector<Point> points4 = { { 350, 150 },{ 401, 150 },{ 401, 199 },{ 450, 199 },{ 450, 250 },{ 400, 250 },{ 400, 200 },{ 350, 200 } };
	std::vector<Point> points5 = { { 550, 150 },{ 601, 150 },{ 601, 199 },{ 650, 199 },{ 650, 250 },{ 600, 250 },{ 600, 200 },{ 550, 200 } };
	std::vector<Color3f> colors = { { 1.f, 0.f, 0.f },{ 0.f, 1.f, 0.f },{ 0.f, 0.f, 1.f } };

	Stencil s = {
		{
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
			{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
		}, 0, 0 };

	// 扫描线填充
	drawRect(points1);
	fillPolygon(points1, s, { 0, 0 });

	// 边界填充
	// 4联通（使用堆栈）
	drawRect(points2);
	boundaryFill4ByStack(370, 370, {1.f, 1.f, 1.f}, { 1.f, 1.f, 1.f }, s, { 0, 0 });

	// 8联通（使用堆栈）
	drawRect(points3);
	boundaryFill8ByStack(570, 370, { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, s, { 0, 0 });

	// 泛滥填充
	// 4联通（使用堆栈）
	drawRect(points4, colors);
	glColor3f(1.0, 1.0, 1.0);
	floodFill4ByStack(370, 170, { 0.f, 0.f, 0.f }, s, { 0, 0 });

	// 8联通（使用堆栈）
	drawRect(points5, colors);
	glColor3f(1.0, 1.0, 1.0);
	floodFill8ByStack(570, 170, { 0.f, 0.f, 0.f }, s, { 0, 0 });

	glFlush();
}
void code_6_exercise_48()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_49
struct Point { int x; int y; };
struct Color3f
{
	GLfloat r, g, b;
};
struct ColorMix
{
	Color3f color;
	float percent;
};
bool SHOW_DRAW = true; // 显示绘制过程
int showTime = 1; //毫秒
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}
bool Equal(float f1, float f2) { return std::abs(f1 - f2) < 0.01; }
bool operator==(const Color3f& c1, const Color3f& c2)
{
	return Equal(c1.r, c2.r) && Equal(c1.g, c2.g) && Equal(c1.b, c2.b);
}
bool operator!=(const Color3f& c1, const Color3f& c2)
{
	return !(c1 == c2);
}
Color3f getPixelColor(int x, int y)
{
	static Color3f ret = {};
	ret = {};
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &ret);
	return ret;
}
Point findLineBeginPoint(Point curPoint, const Color3f& fillColor, const Color3f& borderColor)
{
	Point ret = curPoint;
	Color3f curColor = getPixelColor(ret.x, ret.y);
	while (curColor != fillColor && curColor != borderColor)
	{
		ret.x--;
		curColor = getPixelColor(ret.x, ret.y);
	}
	while (curColor == fillColor || curColor == borderColor)
	{
		ret.x++;
		curColor = getPixelColor(ret.x, ret.y);
	}
	return ret;
}
void stackLinePoints(Point begin, Point end, std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	Point curPoint = findLineBeginPoint(begin, fillColor, borderColor);
	if (curPoint.x <= end.x)
	{
		bool spaceRecording = false;
		while (1)
		{
			if (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
			{
				if (!spaceRecording)
				{
					spaceRecording = true;
					pointStack.push_front(curPoint);
				}
			}
			else
			{
				spaceRecording = false;
			}

			if (curPoint.x >= end.x)
				return;

			curPoint.x++;
		}
	}
}
void fill4Connected(std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
		if (SHOW_DRAW)
		{
			glFlush();
			Sleep(showTime);
		}	
		curPoint.x++;
	}

	stackLinePoints({ begin.x, begin.y - 1 }, { end.x, end.y - 1 }, pointStack, fillColor, borderColor);
	stackLinePoints({ begin.x, begin.y + 1 }, { end.x, end.y + 1 }, pointStack, fillColor, borderColor);
	fill4Connected(pointStack, fillColor, borderColor);
}
void boundaryFill4ByStack(int x, int y, Color3f fillColor, Color3f borderColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPoint({ x, y }, fillColor, borderColor));
	fill4Connected(pointStack, fillColor, borderColor);
}
void fill8Connected(std::list<Point>& pointStack, const Color3f& fillColor, const Color3f& borderColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) != fillColor && getPixelColor(curPoint.x, curPoint.y) != borderColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
		if (SHOW_DRAW)
		{
			glFlush();
			Sleep(showTime);
		}
		curPoint.x++;
	}

	stackLinePoints({ begin.x - 1, begin.y - 1 }, { end.x + 1, end.y - 1 }, pointStack, fillColor, borderColor);
	stackLinePoints({ begin.x - 1, begin.y + 1 }, { end.x + 1, end.y + 1 }, pointStack, fillColor, borderColor);
	fill8Connected(pointStack, fillColor, borderColor);
}
void boundaryFill8ByStack(int x, int y, Color3f fillColor, Color3f borderColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPoint({ x, y }, fillColor, borderColor));
	fill8Connected(pointStack, fillColor, borderColor);
}
Point findLineBeginPointFlood(Point begin, Point end, const Color3f& interiorColor)
{
	Point ret = begin;
	Color3f curColor = getPixelColor(ret.x, ret.y);
	while (curColor == interiorColor)
	{
		ret.x--;
		curColor = getPixelColor(ret.x, ret.y);
	}
	while (curColor != interiorColor && ret.x <= end.x)
	{
		ret.x++;
		curColor = getPixelColor(ret.x, ret.y);
	}
	return ret;
}
void stackLinePointsFlood(Point begin, Point end, std::list<Point>& pointStack, const Color3f& interiorColor)
{
	Point curPoint = findLineBeginPointFlood(begin, end, interiorColor);
	if (curPoint.x <= end.x)
	{
		bool spaceRecording = false;
		while (1)
		{
			if (getPixelColor(curPoint.x, curPoint.y) == interiorColor)
			{
				if (!spaceRecording)
				{
					spaceRecording = true;
					pointStack.push_front(curPoint);
				}
			}
			else
			{
				spaceRecording = false;
			}

			if (curPoint.x >= end.x)
				return;

			curPoint.x++;
		}
	}
}
void fill4ConnectedFlood(std::list<Point>& pointStack, const Color3f& interiorColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) == interiorColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
		if (SHOW_DRAW)
		{
			glFlush();
			Sleep(showTime);
		}
		curPoint.x++;
	}

	stackLinePointsFlood({ begin.x, begin.y - 1 }, { end.x, end.y - 1 }, pointStack, interiorColor);
	stackLinePointsFlood({ begin.x, begin.y + 1 }, { end.x, end.y + 1 }, pointStack, interiorColor);
	fill4ConnectedFlood(pointStack, interiorColor);
}
void floodFill4ByStack(int x, int y, Color3f fillColor, Color3f interiorColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPointFlood({ x, y }, {9999, 9999}, interiorColor));
	fill4ConnectedFlood(pointStack, interiorColor);
}
void fill8ConnectedFlood(std::list<Point>& pointStack, const Color3f& interiorColor)
{
	if (pointStack.empty())
		return;

	auto curPoint = pointStack.front();
	pointStack.pop_front();

	Point begin = curPoint;
	Point end = curPoint;
	while (getPixelColor(curPoint.x, curPoint.y) == interiorColor)
	{
		end = curPoint;
		setPixel(curPoint.x, curPoint.y);
		if (SHOW_DRAW)
		{
			glFlush();
			Sleep(showTime);
		}
		curPoint.x++;
	}

	stackLinePointsFlood({ begin.x - 1, begin.y - 1 }, { end.x + 1, end.y - 1 }, pointStack, interiorColor);
	stackLinePointsFlood({ begin.x - 1, begin.y + 1 }, { end.x + 1, end.y + 1 }, pointStack, interiorColor);
	fill8ConnectedFlood(pointStack, interiorColor);
}
void floodFill8ByStack(int x, int y, Color3f fillColor, Color3f interiorColor)
{
	std::list<Point> pointStack;
	pointStack.push_front(findLineBeginPointFlood({ x, y }, {9999, 9999}, interiorColor));
	fill8ConnectedFlood(pointStack, interiorColor);
}

void drawRect(const std::vector<Point>& points)
{
	for (int i = 0, j = 0; i < points.size(); i++, j++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}
}
void drawRect(const std::vector<Point>& points, const std::vector<Color3f>& colors)
{
	for (int i = 0, j = 0; i < points.size(); i++, j++)
	{
		int next = i + 1 >= points.size() ? 0 : i + 1;
		auto color = colors[j = j >= colors.size() ? 0 : j];
		glColor3f(color.r, color.g, color.b);
		lineBres(points[i].x, points[i].y, points[next].x, points[next].y);
	}
}
Color3f mixColors(const std::vector<ColorMix>& colors)
{
	Color3f ret = {0, 0, 0};
	for (auto& c : colors)
	{
		ret.r += c.color.r * c.percent;
		ret.g += c.color.g * c.percent;
		ret.b += c.color.b * c.percent;
	}
	return ret;
}
int calcInverseNumberByBefore(const std::vector<int>& permutation)
{
	// 前向搜索更大数算法
	int ret = 0;
	for (int i = 0; i < permutation.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (permutation[j] > permutation[i])
				ret++;
		}
	}
	return ret;
}
int calcInverseNumberByAfter(const std::vector<int>& permutation)
{
	// 后向搜索更小数算法
	int ret = 0;
	for (int i = 0; i < permutation.size(); i++)
	{
		for (int j = i + 1; j < permutation.size(); j++)
		{
			if (permutation[j] < permutation[i])
				ret++;
		}
	}
	return ret;
}
void calcDetPermutations(std::map<int, bool>& info, std::vector<int>& current, std::vector<std::vector<int>>& result)
{
	for (int i = 0; i < info.size(); i++)
	{
		if (!info[i])
		{
			info[i] = true;

			current.push_back(i);
			if (current.size() >= info.size())
				result.push_back(current);

			calcDetPermutations(info, current, result);

			info[i] = false;
			current.pop_back();
		}
	}
}
float calcDet(const std::vector<std::vector<float>>& det)
{
	assert(det.size() && det.size() == det[0].size());
	std::vector<std::vector<int>> permutations;
	std::map<int, bool> info;
	for (int i = 0; i < det.size(); i++)
		info[i] = false;
	std::vector<int> current;
	calcDetPermutations(info, current, permutations);

	float ret = 0.f;
	for (auto p : permutations)
	{
		int t = calcInverseNumberByBefore(p);
		float _ret = std::pow(-1, t);
		for (int i = 0; i < p.size(); i++)
			_ret *= det[i][p[i]];
		ret += _ret;
	}
	return ret;
}
bool calcFillParam(Color3f current, std::vector<ColorMix>& colors)
{
	if (colors.size() == 0 || colors.size() == 1)
	{
		return false;
	}
	else if (colors.size() == 2)
	{
		if (Equal(colors[0].color.r, colors[1].color.r))
			return false;
		colors[0].percent = (current.r - colors[1].color.r) / (colors[0].color.r - colors[1].color.r);
		colors[1].percent = 1 - colors[0].percent;
		return true;
	}
	else if (colors.size() == 3)
	{
		std::vector<std::vector<float>> detD = {
			{ colors[0].color.r - colors[2].color.r, colors[1].color.r - colors[2].color.r},
			{ colors[0].color.g - colors[2].color.g, colors[1].color.g - colors[2].color.g}, };
		std::vector<std::vector<float>> detD1 = {
			{ current.r - colors[2].color.r, colors[1].color.r - colors[2].color.r },
			{ current.g - colors[2].color.g, colors[1].color.g - colors[2].color.g }, };
		std::vector<std::vector<float>> detD2 = {
			{ colors[0].color.r - colors[2].color.r, current.r - colors[2].color.r },
			{ colors[0].color.g - colors[2].color.g, current.g - colors[2].color.g }, };


		float D = calcDet(detD);
		if (Equal(D, 0))
			return false;
		float D1 = calcDet(detD1);
		float D2 = calcDet(detD2);
		colors[0].percent = D1 / D;
		colors[1].percent = D2 / D;
		colors[2].percent = 1 - colors[0].percent - colors[1].percent;
		return true;
	}
	else if (colors.size() == 4)
	{
		std::vector<std::vector<float>> detD = {
			{ colors[0].color.r - colors[3].color.r, colors[1].color.r - colors[3].color.r, colors[2].color.r - colors[3].color.r },
			{ colors[0].color.g - colors[3].color.g, colors[1].color.g - colors[3].color.g, colors[2].color.g - colors[3].color.g },
			{ colors[0].color.b - colors[3].color.b, colors[1].color.b - colors[3].color.b, colors[2].color.b - colors[3].color.b }, };
		std::vector<std::vector<float>> detD1 = {
			{ current.r - colors[3].color.r, colors[1].color.r - colors[3].color.r, colors[2].color.r - colors[3].color.r },
			{ current.g - colors[3].color.g, colors[1].color.g - colors[3].color.g, colors[2].color.g - colors[3].color.g },
			{ current.b - colors[3].color.b, colors[1].color.b - colors[3].color.b, colors[2].color.b - colors[3].color.b }, };
		std::vector<std::vector<float>> detD2 = {
			{ colors[0].color.r - colors[3].color.r, current.r - colors[3].color.r, colors[2].color.r - colors[3].color.r },
			{ colors[0].color.g - colors[3].color.g, current.g - colors[3].color.g, colors[2].color.g - colors[3].color.g },
			{ colors[0].color.b - colors[3].color.b, current.b - colors[3].color.b, colors[2].color.b - colors[3].color.b }, };
		std::vector<std::vector<float>> detD3 = {
			{ colors[0].color.r - colors[3].color.r, colors[1].color.r - colors[3].color.r, current.r - colors[3].color.r },
			{ colors[0].color.g - colors[3].color.g, colors[1].color.g - colors[3].color.g, current.g - colors[3].color.g },
			{ colors[0].color.b - colors[3].color.b, colors[1].color.b - colors[3].color.b, current.b - colors[3].color.b }, };
		
		float D = calcDet(detD);
		float D1 = calcDet(detD1);
		float D2 = calcDet(detD2);
		float D3 = calcDet(detD3);
		if (Equal(D, 0))
			return false;
		colors[0].percent = D1 / D;
		colors[1].percent = D2 / D;
		colors[2].percent = D3 / D;
		colors[3].percent = 1 - colors[0].percent - colors[1].percent - colors[2].percent;
		return true;
	}
	return false;
}
void linearSoftFillBoundary4(Point fillPoint, const std::vector<ColorMix>& sourceColors, const std::vector<Color3f>& newColors)
{
	assert(sourceColors.size() == newColors.size());

	// 模拟出源填充
	auto sourceColor = mixColors(sourceColors);
	glColor3f(sourceColor.r, sourceColor.g, sourceColor.b);
	SHOW_DRAW = false;
	boundaryFill4ByStack(fillPoint.x, fillPoint.y, sourceColor, { 1.0, 1.0, 1.0 });
	
	// 计算填充系数，并填充新颜色
	std::vector<ColorMix> sourceColorsMix;
	for (auto c : sourceColors)
		sourceColorsMix.push_back({ c.color, 0.f });
	assert(calcFillParam(getPixelColor(fillPoint.x, fillPoint.y), sourceColorsMix));
	std::vector<ColorMix> newColorsMix;
	for (int i = 0; i < newColors.size(); i++)
		newColorsMix.push_back({ newColors[i], sourceColorsMix[i].percent });
	auto newColor = mixColors(newColorsMix);
	glColor3f(newColor.r, newColor.g, newColor.b);
	SHOW_DRAW = true;
	boundaryFill4ByStack(fillPoint.x, fillPoint.y, newColor, { 1.0, 1.0, 1.0 });
}
void linearSoftFillBoundary8(Point fillPoint, const std::vector<ColorMix>& sourceColors, const std::vector<Color3f>& newColors)
{
	assert(sourceColors.size() == newColors.size());

	// 模拟出源填充
	auto sourceColor = mixColors(sourceColors);
	glColor3f(sourceColor.r, sourceColor.g, sourceColor.b);
	SHOW_DRAW = false;
	boundaryFill8ByStack(fillPoint.x, fillPoint.y, sourceColor, { 1.0, 1.0, 1.0 });

	// 计算填充系数，并填充新颜色
	std::vector<ColorMix> sourceColorsMix;
	for (auto c : sourceColors)
		sourceColorsMix.push_back({ c.color, 0.f });
	assert(calcFillParam(getPixelColor(fillPoint.x, fillPoint.y), sourceColorsMix));
	std::vector<ColorMix> newColorsMix;
	for (int i = 0; i < newColors.size(); i++)
		newColorsMix.push_back({ newColors[i], sourceColorsMix[i].percent });
	auto newColor = mixColors(newColorsMix);
	glColor3f(newColor.r, newColor.g, newColor.b);
	SHOW_DRAW = true;
	boundaryFill8ByStack(fillPoint.x, fillPoint.y, newColor, { 1.0, 1.0, 1.0 });
}
void linearSoftFillFlood4(Point fillPoint, const std::vector<ColorMix>& sourceColors, const std::vector<Color3f>& newColors)
{
	assert(sourceColors.size() == newColors.size());

	// 模拟出源填充
	auto sourceColor = mixColors(sourceColors);
	glColor3f(sourceColor.r, sourceColor.g, sourceColor.b);
	SHOW_DRAW = false;
	floodFill4ByStack(fillPoint.x, fillPoint.y, sourceColor, { 0.0, 0.0, 0.0 });

	// 计算填充系数，并填充新颜色
	std::vector<ColorMix> sourceColorsMix;
	for (auto c : sourceColors)
		sourceColorsMix.push_back({ c.color, 0.f });
	assert(calcFillParam(getPixelColor(fillPoint.x, fillPoint.y), sourceColorsMix));
	std::vector<ColorMix> newColorsMix;
	for (int i = 0; i < newColors.size(); i++)
		newColorsMix.push_back({ newColors[i], sourceColorsMix[i].percent });
	auto newColor = mixColors(newColorsMix);
	glColor3f(newColor.r, newColor.g, newColor.b);
	SHOW_DRAW = true;
	floodFill4ByStack(fillPoint.x, fillPoint.y, newColor, sourceColor);
}
void linearSoftFillFlood8(Point fillPoint, const std::vector<ColorMix>& sourceColors, const std::vector<Color3f>& newColors)
{
	assert(sourceColors.size() == newColors.size());

	// 模拟出源填充
	auto sourceColor = mixColors(sourceColors);
	glColor3f(sourceColor.r, sourceColor.g, sourceColor.b);
	SHOW_DRAW = false;
	floodFill8ByStack(fillPoint.x, fillPoint.y, sourceColor, { 0.0, 0.0, 0.0 });

	// 计算填充系数，并填充新颜色
	std::vector<ColorMix> sourceColorsMix;
	for (auto c : sourceColors)
		sourceColorsMix.push_back({ c.color, 0.f });
	assert(calcFillParam(getPixelColor(fillPoint.x, fillPoint.y), sourceColorsMix));
	std::vector<ColorMix> newColorsMix;
	for (int i = 0; i < newColors.size(); i++)
		newColorsMix.push_back({ newColors[i], sourceColorsMix[i].percent });
	auto newColor = mixColors(newColorsMix);
	glColor3f(newColor.r, newColor.g, newColor.b);
	SHOW_DRAW = true;
	floodFill8ByStack(fillPoint.x, fillPoint.y, newColor, sourceColor);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> points1 = { { 100, 350 },{ 151, 350 },{ 151, 399 },{ 200, 399 },{ 200, 450 },{ 150, 450 },{ 150, 400 },{ 100, 400 } };
	std::vector<Point> points2 = { { 300, 350 },{ 351, 350 },{ 351, 399 },{ 400, 399 },{ 400, 450 },{ 350, 450 },{ 350, 400 },{ 300, 400 } };
	std::vector<Point> points3 = { { 100, 150 },{ 151, 150 },{ 151, 199 },{ 200, 199 },{ 200, 250 },{ 150, 250 },{ 150, 200 },{ 100, 200 } };
	std::vector<Point> points4 = { { 300, 150 },{ 351, 150 },{ 351, 199 },{ 400, 199 },{ 400, 250 },{ 350, 250 },{ 350, 200 },{ 300, 200 } };
	std::vector<Point> points5 = { { 450, 150 },{ 501, 150 },{ 501, 199 },{ 550, 199 },{ 550, 250 },{ 500, 250 },{ 500, 200 },{ 450, 200 } };
	std::vector<Point> points6 = { { 650, 150 },{ 701, 150 },{ 701, 199 },{ 750, 199 },{ 750, 250 },{ 700, 250 },{ 700, 200 },{ 650, 200 } };
	
	std::vector<Color3f> colors = { { 1.f, 0.f, 0.f },{ 0.f, 1.f, 0.f },{ 0.f, 0.f, 1.f } };

	// 2种颜色（F+B）
	drawRect(points1);
	linearSoftFillBoundary4({ 120, 370 }, { { 1.0, 1.0, 1.0, 0.5 },{ 0.0, 0.0, 0.0, 0 } }, { {1.0, 0.0, 0.0},{ 0.0, 0.0, 0.0 } });

	glColor3f(1.0, 1.0, 1.0);
	drawRect(points2);
	linearSoftFillBoundary8({ 320, 370 }, { { 1.0, 1.0, 1.0, 0.5 },{ 0.0, 0.0, 0.0, 0 } }, { { 1.0, 0.0, 0.0 },{ 0.0, 0.0, 0.0 } });

	// 3种颜色（F+B1+B2或F1+F2+B）
	glColor3f(1.0, 1.0, 1.0);
	drawRect(points3);
	linearSoftFillBoundary4(
	{ 120, 170 },
	{ { 1.0, 0.0, 0.0, 0.5 },{ 0.0, 1.0, 0.0, 0.5 }, { 0.0, 0.0, 0.0, 0 } },
	{ { 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 } }
	);

	glColor3f(1.0, 1.0, 1.0);
	drawRect(points4);
	linearSoftFillBoundary8(
	{ 320, 170 },
	{ { 1.0, 0.0, 0.0, 0.5 },{ 0.0, 1.0, 0.0, 0.5 },{ 0.0, 0.0, 0.0, 0 } },
	{ { 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0 } }
	);

	// 4种颜色（F + B1 + B2 + B3或F1 + F2 + B1 + B2或F1 + F2 + F3 + B）
	drawRect(points5, colors);
	linearSoftFillFlood4(
	{ 470, 170 },
	{ { 1.0, 1.0, 1.0, 0.3f },{ 0.5, 0.2f, 0.0, 0.2f },{ 0.0, 0.0, 1.0, 0.2f }, { 0.0, 0.0, 0.0, 0.3f } },
	{ { 1.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f, 1.0f }, { 0.0, 0.0, 0.0 } }
	);

	drawRect(points6, colors);
	linearSoftFillFlood8(
	{ 670, 170 },
	{ { 1.0, 1.0, 1.0, 0.3f },{ 0.5, 0.2f, 0.0, 0.2f },{ 0.0, 0.0, 1.0, 0.2f },{ 0.0, 0.0, 0.0, 0.3f } },
	{ { 1.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f, 1.0f },{ 0.0, 0.0, 0.0 } }
	);

	glFlush();
}
void code_6_exercise_49()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_50
struct Point { int x; int y; };
struct Color3f
{
	GLfloat r, g, b;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
struct TextInfo
{
	int space;
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void drawStencil(int x, int y, const Stencil& s)
{
	for (int i = 0; i < s.stencil.size(); i++)
	{
		for (int j = 0; j < s.stencil[i].size(); j++)
		{
			if(s.stencil[i][j] == 1)
				setPixel(x + j, y + s.stencil.size() - 1 - i);
		}
	}
}
void drawString(int x, int y, const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	for (auto& c : str)
	{
		if(texts.find(c) != texts.end())
		{
			const Stencil& s = texts.find(c)->second;
			drawStencil(x, y, s);
			x += (s.stencil[0].size() + info.space);
		}
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

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

	std::map<char, Stencil> texts;
	texts['A'] = A;
	texts['B'] = B;
	texts['C'] = C;
	TextInfo info = { 2 };
	drawString(100, 100, "ABC", info, texts);

	glFlush();
}
void code_6_exercise_50()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_51
struct Point { int x; int y; };
struct Color3f
{
	GLfloat r, g, b;
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
void drawStencil(double x, double y, const Stencil& s, float rotate)
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
void drawString(int x, int y, const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	double posX = x;
	double posY = y;
	float charRotate = -1 * PI / 2 + info.upVector;
	for (auto& c : str)
	{
		if (texts.find(c) != texts.end())
		{
			const Stencil& s = texts.find(c)->second;
			drawStencil(posX, posY, s, charRotate);
			switch (info.textPath)
			{
			case TextPath::UP:
				posX += (s.stencil.size() + info.space) * std::cos(info.upVector);
				posY += (s.stencil.size() + info.space) * std::sin(info.upVector);
				break;
			case TextPath::DOWN:
				posX += (s.stencil.size() + info.space) * std::cos(info.upVector + PI);
				posY += (s.stencil.size() + info.space) * std::sin(info.upVector + PI);
				break;
			case TextPath::LEFT:
				posX += (s.stencil[0].size() + info.space) * std::cos(info.upVector + PI / 2);
				posY += (s.stencil[0].size() + info.space) * std::sin(info.upVector + PI / 2);
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
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

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

	std::map<char, Stencil> texts;
	texts['A'] = A;
	texts['B'] = B;
	texts['C'] = C;

	// PI/2 向上向量
	glColor3f(1.0f, 0.0f, 0.0f);
	drawString(120, 420, "ABC", { 2, PI / 2, TextPath::UP }, texts);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawString(120, 380, "ABC", { 2, PI / 2, TextPath::DOWN }, texts);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawString(100, 400, "ABC", { 2, PI / 2, TextPath::LEFT }, texts);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawString(140, 400, "ABC", { 2, PI / 2, TextPath::RIGHT }, texts);

	// 0 向上向量
	glColor3f(1.0f, 0.0f, 0.0f);
	drawString(340, 400, "ABC", { 2, 0, TextPath::UP }, texts);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawString(300, 400, "ABC", { 2, 0, TextPath::DOWN }, texts);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawString(320, 420, "ABC", { 2, 0, TextPath::LEFT }, texts);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawString(320, 380, "ABC", { 2, 0, TextPath::RIGHT }, texts);

	// PI 向上向量
	glColor3f(1.0f, 0.0f, 0.0f);
	drawString(500, 400, "ABC", { 2, PI, TextPath::UP }, texts);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawString(540, 400, "ABC", { 2, PI, TextPath::DOWN }, texts);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawString(520, 380, "ABC", { 2, PI, TextPath::LEFT }, texts);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawString(520, 420, "ABC", { 2, PI, TextPath::RIGHT }, texts);

	// PI/6 向上向量
	glColor3f(1.0f, 0.0f, 0.0f);
	drawString(120, 220, "ABC", { 2, PI/6, TextPath::UP }, texts);
	glColor3f(0.0f, 1.0f, 0.0f);	 
	drawString(120, 180, "ABC", { 2, PI/6, TextPath::DOWN }, texts);
	glColor3f(0.0f, 0.0f, 1.0f);	 
	drawString(100, 200, "ABC", { 2, PI/6, TextPath::LEFT }, texts);
	glColor3f(1.0f, 1.0f, 1.0f);	 
	drawString(140, 200, "ABC", { 2, PI/6, TextPath::RIGHT }, texts);

	// PI/4 向上向量
	glColor3f(1.0f, 0.0f, 0.0f);
	drawString(320, 220, "ABC", { 2, PI / 4, TextPath::UP }, texts);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawString(320, 180, "ABC", { 2, PI / 4, TextPath::DOWN }, texts);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawString(300, 200, "ABC", { 2, PI / 4, TextPath::LEFT }, texts);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawString(340, 200, "ABC", { 2, PI / 4, TextPath::RIGHT }, texts);

	// PI*3/4 向上向量
	glColor3f(1.0f, 0.0f, 0.0f);
	drawString(520, 220, "ABC", { 2, PI * 3 / 4, TextPath::UP }, texts);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawString(520, 180, "ABC", { 2, PI * 3 / 4, TextPath::DOWN }, texts);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawString(500, 200, "ABC", { 2, PI * 3 / 4, TextPath::LEFT }, texts);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawString(540, 200, "ABC", { 2, PI * 3 / 4, TextPath::RIGHT }, texts);

	// -PI/4 向上向量
	glColor3f(1.0f, 0.0f, 0.0f);
	drawString(740, 200, "ABC", { 2, -PI / 4, TextPath::UP }, texts);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawString(700, 200, "ABC", { 2, -PI / 4, TextPath::DOWN }, texts);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawString(720, 220, "ABC", { 2, -PI / 4, TextPath::LEFT }, texts);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawString(720, 180, "ABC", { 2,- PI / 4, TextPath::RIGHT }, texts);
	
	glFlush();
}
void code_6_exercise_51()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_52
struct Point { int x; int y; };
struct Rect
{
	double x0;
	double y0;
	double width;
	double height;
};
struct Color3f
{
	GLfloat r, g, b;
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
enum class H_ALIGN
{
	LEFT,
	CENTER,
	RIGHT,
};
enum class V_ALIGN
{
	TOP,
	CENTER,
	BOTTOM,
};
struct TextInfo
{
	int spaceChar;
	int spaceLine;
	double upVector;
	TextPath textPath;
	H_ALIGN hAlign;
	V_ALIGN vAlign;
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void drawStencil(double x, double y, const Stencil& s, float rotate)
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
int getLineMaxHeight(int begin, const string& str, const std::map<char, Stencil>& texts)
{
	int ret = 0;
	for (int i = begin; i < str.size(); i++)
	{
		if (str[i] == '\n')
			return ret;

		if (texts.find(str[i]) != texts.end())
		{
			const Stencil& s = texts.find(str[i])->second;
			if (ret < s.stencil.size())
				ret = s.stencil.size();
		}
	}
	return ret;
}
int getLineMaxWidth(int begin, const string& str, const std::map<char, Stencil>& texts)
{
	int ret = 0;
	for (int i = begin; i < str.size(); i++)
	{
		if (str[i] == '\n')
			return ret;

		if (texts.find(str[i]) != texts.end())
		{
			const Stencil& s = texts.find(str[i])->second;
			if (ret < s.stencil[0].size())
				ret = s.stencil[0].size();
		}
	}
	return ret;
}
int getLineDistance(int begin, const string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	int ret = 0;
	bool isCount = false;
	for (int i = begin; i < str.size(); i++)
	{
		if (str[i] == '\n')
		{
			if (isCount)
				ret -= info.spaceChar;
			return ret;
		}

		if (texts.find(str[i]) != texts.end())
		{
			const Stencil& s = texts.find(str[i])->second;

			if (info.textPath == TextPath::LEFT || info.textPath == TextPath::RIGHT)
				ret += s.stencil[0].size();
			else
				ret += s.stencil.size();

			ret += info.spaceChar;
			isCount = true;
		}
	}

	if (isCount)
		ret -= info.spaceChar;
	return ret;
}
int getTextMaxWidth(const string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	int ret = 0;
	int curLineWidth = getLineDistance(0, str, info, texts);
	for (int i = 1; i < str.size(); i++)
	{
		if (str[i] == '\n')
		{
			if (curLineWidth > ret)
				ret = curLineWidth;
			curLineWidth = getLineDistance(i + 1, str, info, texts);
		}
	}

	if (curLineWidth > ret)
		ret = curLineWidth;

	return ret;
}
int getTextMaxHeight(const string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	int total = 0;

	if (info.textPath == TextPath::LEFT || info.textPath == TextPath::RIGHT)
		total = getLineMaxHeight(0, str, texts);
	else
		total = getLineMaxWidth(0, str, texts);

	for (int i = 1; i < str.size(); i++)
	{
		if (str[i] == '\n')
		{
			total += info.spaceLine;
			if (info.textPath == TextPath::LEFT || info.textPath == TextPath::RIGHT)
				total += getLineMaxHeight(i + 1, str, texts);
			else
				total += getLineMaxWidth(i + 1, str, texts);
		}
	}
	return total;
}
void drawString(double x, double y, const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	double _x = x;
	double _y = y;

	// 左右初次定位到行底部
	switch (info.textPath)
	{
	case TextPath::UP:
	case TextPath::DOWN:
	break;
	case TextPath::LEFT:
	case TextPath::RIGHT:
	{
		int nextMaxHeight = getLineMaxHeight(0, str, texts);
		_x += (nextMaxHeight)* std::cos(info.upVector + PI);
		_y += (nextMaxHeight)* std::sin(info.upVector + PI);
	}
	break;
	}

	double maxWidth = getTextMaxWidth(str, info, texts);
	double posX = _x;
	double posY = _y;

	float charRotate = -1 * PI / 2 + info.upVector;
	bool newLine = true;
	int lineBegin = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '\n')
		{
			// 换行定位
			switch (info.textPath)
			{
			case TextPath::UP:
			case TextPath::DOWN:
			{
				int nextMaxWidth = getLineMaxWidth(lineBegin, str, texts);
				_x += (nextMaxWidth + info.spaceLine) * std::cos(info.upVector - PI / 2);
				_y += (nextMaxWidth + info.spaceLine) * std::sin(info.upVector - PI / 2);
				lineBegin = i + 1;
			}
			break;
			case TextPath::LEFT:
			case TextPath::RIGHT:
			{
				int nextMaxHeight = getLineMaxHeight(i + 1, str, texts);
				_x += (nextMaxHeight + info.spaceLine) * std::cos(info.upVector + PI);
				_y += (nextMaxHeight + info.spaceLine) * std::sin(info.upVector + PI);
			}
			break;
			}
			posX = _x;
			posY = _y;
			newLine = true;
		}
		else
		{
			if (newLine)
			{
				// 对齐定位
				double curLineWidth = getLineDistance(i, str, info, texts);
				switch (info.textPath)
				{
				case TextPath::UP:
				{
					switch (info.vAlign)
					{
					case V_ALIGN::TOP:
					{
						double offset = maxWidth - curLineWidth;
						posX += offset * std::cos(info.upVector);
						posY += offset * std::sin(info.upVector);
					}
					break;
					case V_ALIGN::CENTER:
					{
						double offset = (maxWidth - curLineWidth) / 2;
						posX += offset * std::cos(info.upVector);
						posY += offset * std::sin(info.upVector);
					}
					break;
					case V_ALIGN::BOTTOM:
						break;
					}
				}
					break;
				case TextPath::DOWN:
				{
					switch (info.vAlign)
					{
					case V_ALIGN::TOP:
					break;
					case V_ALIGN::CENTER:
					{
						double offset = (maxWidth - curLineWidth) / 2;
						posX += offset * std::cos(info.upVector + PI);
						posY += offset * std::sin(info.upVector + PI);
					}
					break;
					case V_ALIGN::BOTTOM:
					{
						double offset = maxWidth - curLineWidth;
						posX += offset * std::cos(info.upVector + PI);
						posY += offset * std::sin(info.upVector + PI);
					}
						break;
					}
				}
					break;
				case TextPath::LEFT:
				{
					switch (info.hAlign)
					{
					case H_ALIGN::LEFT:
					{
						double offset = maxWidth - curLineWidth;
						posX += offset * std::cos(info.upVector + PI / 2);
						posY += offset * std::sin(info.upVector + PI / 2);
					}
					break;
					case H_ALIGN::CENTER:
					{
						double offset = (maxWidth - curLineWidth) / 2;
						posX += offset * std::cos(info.upVector + PI / 2);
						posY += offset * std::sin(info.upVector + PI / 2);
					}
					break;
					case H_ALIGN::RIGHT:
					break;
					}
				}
					break;
				case TextPath::RIGHT:
				{
					switch (info.hAlign)
					{
					case H_ALIGN::LEFT:
						break;
					case H_ALIGN::CENTER:
					{
						double offset = (maxWidth - curLineWidth) / 2;
						posX += offset * std::cos(info.upVector - PI / 2);
						posY += offset * std::sin(info.upVector - PI / 2);
					}
						break;
					case H_ALIGN::RIGHT:
					{
						double offset = maxWidth - curLineWidth;
						posX += offset * std::cos(info.upVector - PI / 2);
						posY += offset * std::sin(info.upVector - PI / 2);
					}
					break;
					}
				}
					break;
				}
			}			

			if (texts.find(str[i]) != texts.end())
			{
				const Stencil& s = texts.find(str[i])->second;
				// 每一个文字定位
				switch (info.textPath)
				{
				case TextPath::DOWN:
				{
					posX += (s.stencil.size() + (newLine ? 0 : info.spaceChar)) * std::cos(info.upVector + PI);
					posY += (s.stencil.size() + (newLine ? 0 : info.spaceChar)) * std::sin(info.upVector + PI);
					break;
				}
				case TextPath::LEFT:
				{
					posX += (s.stencil[0].size() + (newLine ? 0 : info.spaceChar)) * std::cos(info.upVector + PI / 2);
					posY += (s.stencil[0].size() + (newLine ? 0 : info.spaceChar)) * std::sin(info.upVector + PI / 2);
					break;
				}
				default:
					break;
				}
				drawStencil(posX, posY, s, charRotate);
				switch (info.textPath)
				{
				case TextPath::UP:
					posX += (s.stencil.size() + info.spaceChar) * std::cos(info.upVector);
					posY += (s.stencil.size() + info.spaceChar) * std::sin(info.upVector);
					break;
				case TextPath::RIGHT:
					posX += (s.stencil[0].size() + info.spaceChar) * std::cos(info.upVector - PI / 2);
					posY += (s.stencil[0].size() + info.spaceChar) * std::sin(info.upVector - PI / 2);
					break;
				default:
					break;
				}
			}
			newLine = false;
		}		
	}
}
void drawRect(const Rect& rect)
{
	int x0 = Round(rect.x0);
	int y0 = Round(rect.y0);
	int width = Round(rect.width);
	int height = Round(rect.height);

	for (int i = x0; i < x0 + width; i++)
	{
		setPixel(i, y0);
		setPixel(i, y0 + height);
	}
	for (int i = y0; i < y0 + height; i++)
	{
		setPixel(x0, i);
		setPixel(x0 + width, i);
	}
}
void calcStartPointFromCenter(const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts, double& outX, double& outY)
{
	outX = 0;
	outY = 0;
	double width = getTextMaxWidth(str, info, texts);
	double height = getTextMaxHeight(str, info, texts);
	switch (info.textPath)
	{
	case TextPath::UP:
	{
		outX = width / 2 * std::cos(info.upVector + PI);
		outY = width / 2 * std::sin(info.upVector + PI);
		outX += height / 2 * std::cos(info.upVector + PI / 2);
		outY += height / 2 * std::sin(info.upVector + PI / 2);
	}
	break;
	case TextPath::DOWN:
	{
		outX = width / 2 * std::cos(info.upVector);
		outY = width / 2 * std::sin(info.upVector);
		outX += height / 2 * std::cos(info.upVector + PI / 2);
		outY += height / 2 * std::sin(info.upVector + PI / 2);
	}
	break;
	case TextPath::LEFT:
	{
		outX = height / 2 * std::cos(info.upVector);
		outY = height / 2 * std::sin(info.upVector);
		outX += width / 2 * std::cos(info.upVector - PI / 2);
		outY += width / 2 * std::sin(info.upVector - PI / 2);
	}
	break;
	case TextPath::RIGHT:
	{
		outX = height / 2 * std::cos(info.upVector);
		outY = height / 2 * std::sin(info.upVector);
		outX += width / 2 * std::cos(info.upVector + PI / 2);
		outY += width / 2 * std::sin(info.upVector + PI / 2);
	}
	break;
	}
}
void calcAlignBoundingBox(const Rect& outBoundingBox, Rect& innerBoundingBox, const TextInfo& info)
{
	if (outBoundingBox.width < innerBoundingBox.width)
		printf("warning: out boundingbox width less than innner boundingbox width!\n");
	switch (info.hAlign)
	{
	case H_ALIGN::LEFT:
		innerBoundingBox.x0 = outBoundingBox.x0;
		break;
	case H_ALIGN::CENTER:
		innerBoundingBox.x0 = outBoundingBox.x0 + (outBoundingBox.width - innerBoundingBox.width) / 2;
		break;
	case H_ALIGN::RIGHT:
		innerBoundingBox.x0 = outBoundingBox.x0 + (outBoundingBox.width - innerBoundingBox.width);
		break;
	}

	if (outBoundingBox.height < innerBoundingBox.height)
		printf("warning: out boundingbox height less than innner boundingbox height!\n");
	switch (info.vAlign)
	{
	case V_ALIGN::TOP:
		innerBoundingBox.y0 = outBoundingBox.y0 + (outBoundingBox.height - innerBoundingBox.height);
		break;
	case V_ALIGN::CENTER:
		innerBoundingBox.y0 = outBoundingBox.y0 + (outBoundingBox.height - innerBoundingBox.height) / 2;
		break;
	case V_ALIGN::BOTTOM:
		innerBoundingBox.y0 = outBoundingBox.y0;
		break;
	}
}
void calcBoundingBox(const std::string& str, const TextInfo& info, const std::map<char, Stencil>& texts, double& outWidth, double& outHeight)
{
	double x1, y1;
	double x2, y2;
	double x3, y3;
	double x4, y4;
	double width;
	double height;
	if (info.textPath == TextPath::LEFT || info.textPath == TextPath::RIGHT)
	{
		width = getTextMaxWidth(str, info, texts);
		height = getTextMaxHeight(str, info, texts);
	}
	else
	{
		width = getTextMaxHeight(str, info, texts);
		height = getTextMaxWidth(str, info, texts);
	}

	double upX = height / 2 * std::cos(info.upVector);
	double upY = height / 2 * std::sin(info.upVector);
	double downX = height / 2 * std::cos(info.upVector + PI);
	double downY = height / 2 * std::sin(info.upVector + PI);
	
	x1 = width / 2 * std::cos(info.upVector + PI / 2) + upX;
	y1 = width / 2 * std::sin(info.upVector + PI / 2) + upY;
	x2 = width / 2 * std::cos(info.upVector - PI / 2) + upX;
	y2 = width / 2 * std::sin(info.upVector - PI / 2) + upY;
	x3 = width / 2 * std::cos(info.upVector + PI / 2) + downX;
	y3 = width / 2 * std::sin(info.upVector + PI / 2) + downY;
	x4 = width / 2 * std::cos(info.upVector - PI / 2) + downX;
	y4 = width / 2 * std::sin(info.upVector - PI / 2) + downY;

	std::vector<double> vecx = { x1, x2, x3, x4 };
	std::sort(vecx.begin(), vecx.end());
	std::vector<double> vecy = { y1, y2, y3, y4 };
	std::sort(vecy.begin(), vecy.end());
	outWidth = std::fabs(vecx[3] - vecx[0]);
	outHeight = std::fabs(vecy[3] - vecy[0]);
}
void stringAlign(const std::string& str, Rect rect, const TextInfo& info, const std::map<char, Stencil>& texts)
{
	double x, y;
	double width, height;
	calcBoundingBox(str, info, texts, width, height);
	Rect textRect = {0, 0, width, height};
	calcAlignBoundingBox(rect, textRect, info);
	calcStartPointFromCenter(str, info, texts, x, y);
	double xc = textRect.x0 + textRect.width / 2;
	double yc = textRect.y0 + textRect.height / 2;
	drawString(x + xc, y + yc, str, info, texts);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

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

	std::string str = "ABCDE\nECAB\nCD";
	Rect rect = { 200, 200, 400, 300 };
	drawRect(rect);

	stringAlign(str, rect, { 3, 10, 0 , TextPath::UP, H_ALIGN::LEFT, V_ALIGN::TOP }, texts);
	stringAlign(str, rect, { 3, 10, PI / 4 , TextPath::UP, H_ALIGN::CENTER, V_ALIGN::TOP }, texts);
	stringAlign(str, rect, { 3, 10, PI , TextPath::DOWN, H_ALIGN::RIGHT, V_ALIGN::TOP }, texts);
	stringAlign(str, rect, { 3, 10, PI * 3 / 2 , TextPath::DOWN, H_ALIGN::LEFT, V_ALIGN::CENTER }, texts);
	stringAlign(str, rect, { 3, 10, PI / 2 , TextPath::RIGHT, H_ALIGN::CENTER, V_ALIGN::CENTER }, texts);
	stringAlign(str, rect, { 3, 10, PI / 2 , TextPath::RIGHT, H_ALIGN::RIGHT, V_ALIGN::CENTER }, texts);
	stringAlign(str, rect, { 3, 10, PI / 2 , TextPath::RIGHT, H_ALIGN::LEFT, V_ALIGN::BOTTOM }, texts);
	stringAlign(str, rect, { 3, 10, - PI / 3 , TextPath::LEFT, H_ALIGN::CENTER, V_ALIGN::BOTTOM }, texts);
	stringAlign(str, rect, { 3, 10, PI * 5 / 4 , TextPath::LEFT, H_ALIGN::RIGHT, V_ALIGN::BOTTOM }, texts);
	
	glFlush();
}
void code_6_exercise_52()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_53
struct Point { int x; int y; };
struct Color3f
{
	GLfloat r, g, b;
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
void hLineRound(int y, int x0, int x1, int xc, int yc, int offset = 0)
{
	for (int x = x0; x <= x1; x++)
	{
		setPixel(xc + x, yc + y);
		setPixel(xc - x + offset, yc + y);
		setPixel(xc + x, yc - y + offset);
		setPixel(xc - x + offset, yc - y + offset);
		setPixel(xc + y, yc + x);
		setPixel(xc - y + offset, yc + x);
		setPixel(xc + y, yc - x + offset);
		setPixel(xc - y + offset, yc - x + offset);
	}
}
void fillRound(int xc, int yc, float r)
{
	int xline = 0;
	int xRound = r;

	int d2x = 2 * r;
	int d2y = 0;
	int p = Round((float)5 / 4 - r);
	double aa;
	int offset = Round(std::modf(r, &aa)) == 0 ? 1 : 0; // 如果是整数则偏移，保持几何特征.如果是小数(>=0.5)则不偏移
	hLineRound(0, 0, xRound, xc, yc, offset);
	int endY = Round(r / std::sqrt(2));
	for (int curY = 1; curY <= endY; curY++)
	{
		d2y += 2;
		if (p < 0)
		{
			p += d2y + 1;
		}
		else
		{
			xRound--;
			d2x -= 2;
			p += d2y + 1 - d2x;
		}
		hLineRound(curY, ++xline, xRound, xc, yc, offset);
	}
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
}

void drawMarkerPoint(Point p, int size, Color3f c)
{
	glColor3f(c.r, c.g, c.b);
	int beginX = p.x - (size - 1) / 2;
	int endX = beginX + size - 1;
	int beginY = p.y - (size - 1) / 2;
	int endY = beginY + size - 1;
	for (int y = beginY; y <= endY; y++)
	{
		lineBres(beginX, y, endX, y);
	}
	glColor3f(1.0, 1.0, 1.0);
}
void drawMarkerRound(Point p, int size, Color3f c)
{
	glColor3f(c.r, c.g, c.b);
	int baseR = 3;
	float r = size * baseR;
	fillRound(p.x, p.y, r);
	glColor3f(1.0, 1.0, 1.0);
}
void drawMarkerCross(Point p, int size, Color3f c)
{
	glColor3f(c.r, c.g, c.b);
	int baseR = 3;
	float r = size * baseR;
	lineBres(p.x - r, p.y, p.x + r, p.y);
	lineBres(p.x, p.y - r, p.x, p.y + r);
	glColor3f(1.0, 1.0, 1.0);
}
void drawMarkerCrossX(Point p, int size, Color3f c)
{
	glColor3f(c.r, c.g, c.b);
	int baseR = 3;
	float r = size * baseR;
	lineBres(p.x - r, p.y + r, p.x + r, p.y - r);
	lineBres(p.x - r, p.y - r, p.x + r, p.y + r);
	glColor3f(1.0, 1.0, 1.0);
}
void drawMarkerStar(Point p, int size, Color3f c)
{
	drawMarkerCross(p, size, c);
	drawMarkerCrossX(p, size, c);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	// 点标记
	drawMarkerPoint({ 100, 520 }, 1, { 1.0, 1.0, 1.0 });
	drawMarkerPoint({ 300, 520 }, 2, { 1.0, 0.0, 0.0 });
	drawMarkerPoint({ 500, 520 }, 5, { 0.0, 1.0, 0.0 });
	drawMarkerPoint({ 700, 520 }, 10, { 0.0, 0.0, 1.0 });

	// 圆标记
	drawMarkerRound({ 100, 420 }, 1, { 1.0, 1.0, 1.0 });
	drawMarkerRound({ 300, 420 }, 2, { 1.0, 0.0, 0.0 });
	drawMarkerRound({ 500, 420 }, 5, { 0.0, 1.0, 0.0 });
	drawMarkerRound({ 700, 420 }, 10, { 0.0, 0.0, 1.0 });

	// 十字标记
	drawMarkerCross({ 100, 320 }, 1, { 1.0, 1.0, 1.0 });
	drawMarkerCross({ 300, 320 }, 2, { 1.0, 0.0, 0.0 });
	drawMarkerCross({ 500, 320 }, 5, { 0.0, 1.0, 0.0 });
	drawMarkerCross({ 700, 320 }, 10, { 0.0, 0.0, 1.0 });

	// 十字X标记
	drawMarkerCrossX({ 100, 220 }, 1, { 1.0, 1.0, 1.0 });
	drawMarkerCrossX({ 300, 220 }, 2, { 1.0, 0.0, 0.0 });
	drawMarkerCrossX({ 500, 220 }, 5, { 0.0, 1.0, 0.0 });
	drawMarkerCrossX({ 700, 220 }, 10, { 0.0, 0.0, 1.0 });

	// 星标记
	drawMarkerStar({ 100, 120 }, 1, { 1.0, 1.0, 1.0 });
	drawMarkerStar({ 300, 120 }, 2, { 1.0, 0.0, 0.0 });
	drawMarkerStar({ 500, 120 }, 5, { 0.0, 1.0, 0.0 });
	drawMarkerStar({ 700, 120 }, 10, { 0.0, 0.0, 1.0 });

	glFlush();
}
void code_6_exercise_53()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_54
struct Point
{
	int x, y;
};
struct BaseInfo
{
	int x0;
	int y0;
	int xEnd;
	int yEnd;
	int level;
};
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
void setGrayPixel(int x, int y, float grayPercent)
{
	glColor3f(1.0 * grayPercent, 1.0 * grayPercent, 1.0 * grayPercent);
	setPixel(x, y);
}
void refreshLastPoints(const BaseInfo& baseInfo, const std::map<Point, int>& lastInfo)
{
	for (auto p : lastInfo)
	{
		float count = p.second;
		if (count > baseInfo.level * baseInfo.level)
			count = baseInfo.level * baseInfo.level;

		setGrayPixel(baseInfo.x0 + p.first.x, baseInfo.y0 + p.first.y, count / (baseInfo.level*baseInfo.level));
	}
}
void setSubPixelDx(int subX, int subY, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	//printf("x=%d, y=%d\n", subX, subY);
	int curX = floor((float)subX / baseInfo.level);
	int curY = floor((float)subY / baseInfo.level);
	/*if (curX != lastInfo.begin()->first.x)
	{
		refreshLastPoints(baseInfo, lastInfo);
		lastInfo.clear();
	}*/
	lastInfo[{curX, curY}]++;
}
void setSubPixelDy(int subX, int subY, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	//printf("x=%d, y=%d\n", subX, subY);
	int curX = floor((float)subX / baseInfo.level);
	int curY = floor((float)subY / baseInfo.level);
	/*if (curY != lastInfo.begin()->first.y)
	{
		refreshLastPoints(baseInfo, lastInfo);
		lastInfo.clear();
	}*/
	lastInfo[{curX, curY}]++;
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setSubPixelDx(x, y, baseInfo, lastInfo);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setSubPixelDx(x, y, baseInfo, lastInfo);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setSubPixelDy(x, y, baseInfo, lastInfo);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setSubPixelDy(x, y, baseInfo, lastInfo);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setSubPixelDx(x, y, baseInfo, lastInfo);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setSubPixelDx(x, y, baseInfo, lastInfo);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setSubPixelDy(x, y, baseInfo, lastInfo);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setSubPixelDy(x, y, baseInfo, lastInfo);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{ 
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd, baseInfo, lastInfo);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd, baseInfo, lastInfo);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd, baseInfo, lastInfo);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd, baseInfo, lastInfo);
		}
	}
}
void lineBresAA(int x0, int y0, int xEnd, int yEnd, int level)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int subX0 = (level - 1) / 2;
	int subY0 = (level - 1) / 2;
	int subXEnd = subX0 + (xEnd - x0) * level;
	int SubYEnd = subY0 + (yEnd - y0) * level;
	
	std::map<Point, int> lastInfo;
	BaseInfo baseInfo = { x0, y0, xEnd, yEnd, level };
	lastInfo[{0, 0}] = level;
	lineBres(subX0 - 4, subY0, subXEnd - 4, SubYEnd, baseInfo, lastInfo);
	lineBres(subX0 - 3, subY0, subXEnd - 3, SubYEnd, baseInfo, lastInfo);
	lineBres(subX0 - 2, subY0, subXEnd - 2, SubYEnd, baseInfo, lastInfo);
	lineBres(subX0 - 1, subY0, subXEnd - 1, SubYEnd, baseInfo, lastInfo);
	lineBres(subX0, subY0, subXEnd, SubYEnd, baseInfo, lastInfo);
	lineBres(subX0 + 1, subY0, subXEnd + 1, SubYEnd, baseInfo, lastInfo);
	lineBres(subX0 + 2, subY0, subXEnd + 2, SubYEnd, baseInfo, lastInfo);
	lineBres(subX0 + 3, subY0, subXEnd + 3, SubYEnd, baseInfo, lastInfo);
	lineBres(subX0 + 4, subY0, subXEnd + 4, SubYEnd, baseInfo, lastInfo);
	lastInfo[{xEnd - x0, yEnd - y0}] = level;
	refreshLastPoints(baseInfo, lastInfo);
	lastInfo.clear();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	/*lineBresAA(300, 100, 100, 100, 3);
	lineBresAA(200, 200, 200, 400, 3);
	lineBresAA(120, 120, 250, 250, 3);
	lineBresAA(250, 120, 120, 250, 3);

	lineBresAA(150, 150, 650, 400, 3);
	lineBresAA(380, 180, 500, 400, 3);
	lineBresAA(380, 580, 600, 500, 3);
	lineBresAA(600, 250, 700, 10, 3);*/

	lineBresAA(100, 100, 130, 400, 4);

	glFlush();
}
void code_6_exercise_54()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test1
struct Point { int x; int y; };
struct Color3f
{
	GLfloat r, g, b;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
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
struct LineRecord
{
	int beginX;
	int endX;
};
struct BaseInfo
{
	int x0;
	int y0;
	int xEnd;
	int yEnd;
	int AALevel;
};
//// 判断浮点数相等
//bool Equal(float f1, float f2) { return std::abs(f1 - f2) < 0.0001; }
//bool Greater(float f1, float f2) { return Equal(f1, f2) ? false : (f1 > f2); }
//bool Less(float f1, float f2) { return Equal(f1, f2) ? false : (f1 < f2); }
//bool GreaterQ(float f1, float f2) { return Greater(f1, f2) || Equal(f1, f2); }
//bool LessQ(float f1, float f2) { return Less(f1, f2) || Equal(f1, f2); }
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
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
void setGrayPixel(int x, int y, float grayPercent)
{
	glColor3f(1.0 * grayPercent, 1.0 * grayPercent, 1.0 * grayPercent);
	setPixel(x, y);
	//printf("%d,%d(%f)\n", x, y, grayPercent);
}
void refreshLastPoints(const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	//int max = -1;
	//int min = 99999;
	//int  y = 0;
	float squareAA = baseInfo.AALevel * baseInfo.AALevel;
	for (auto p : lastInfo)
	{
		float count = p.second;
		if (count > squareAA)
			count = squareAA;

		//if (p.first.x > max)
		//	max = p.first.x;

		//if (p.first.x < min)
		//	min = p.first.x;

		//y = p.first.y;

		setGrayPixel(baseInfo.x0 + p.first.x, baseInfo.y0 + p.first.y, count / squareAA);
	}
	//printf("%d,%d(%d) -> %d,%d(%d)\n", min, y, lastInfo[{min, y}],  max, y, lastInfo[{max, y}]);
}
void setSubPixel(int subX, int subY, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	//printf("x=%d, y=%d\n", subX, subY);
	int curX = floor((float)subX / baseInfo.AALevel);
	int curY = floor((float)subY / baseInfo.AALevel);
	if (!lastInfo.empty())
	{
		if (curY != lastInfo.begin()->first.y)
		{
			refreshLastPoints(baseInfo, lastInfo);
			lastInfo.clear();
		}
	}
	lastInfo[{curX, curY}]++;
}
void hLine(int y, int x0, int x1, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	for (int x = x0; x <= x1; x++)
	{
		if (baseInfo.AALevel > 1)
			setSubPixel(x, y, baseInfo, lastInfo);
		else
			setPixel(baseInfo.x0 + x, baseInfo.y0 + y);
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
void fillWithActiveLines(int beginY, int endY, std::vector<ActiveLine>& activeLines, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, bool realPoint = false)
{
	if (realPoint)
	{ // 直接计算扫描线交点
		std::vector<Point> points;
		for (int curY = beginY; curY < endY; curY++)
		{
			for (auto& line : activeLines)
			{
				if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
				{
					points.push_back({ line.currentX , curY });
					//printf("x=%d, y=%d\n", baseInfo.x0 + line.currentX, baseInfo.y0 + curY);
					float x = (curY + 1 - line.sortedLine.minY) / ((float)(line.sortedLine.dy) / line.sortedLine.dx) + line.sortedLine.beginX;
					line.currentX = Round(x);
					//printf("x=%f, y=%d\n", baseInfo.x0 + x, baseInfo.y0 + curY + 1);
				}
			}
			std::sort(points.begin(), points.end(), [](auto& a, auto&b) {return a.x < b.x;});
			for (int i = 0; ; i++)
			{
				if (2 * i < points.size() && 2 * i + 1 < points.size())
				{
					hLine(points[2 * i].y, points[2 * i].x, points[2 * i + 1].x, baseInfo, lastInfo);
				}
				else
				{
					points.clear();
					break;
				}
			}
		}
	}
	else
	{ // bres直线算法
		std::vector<std::vector<Point>> points;
		for (int curY = beginY; curY < endY; curY++)
		{
			for (auto& line : activeLines)
			{
				if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
				{
					if (std::abs(line.sortedLine.dy) >= std::abs(line.sortedLine.dx))
					{// |m|>1			
						points.push_back({ { line.currentX , curY } });
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
						points.push_back({ { line.currentX, curY } });
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
						points.back().push_back({ line.currentX - 1, curY });
					}
				}
			}
			std::sort(points.begin(), points.end(), [](auto& a, auto&b) {return a.front().x < b.front().x;});
			for (int i = 0; ; i++)
			{
				if (2 * i < points.size() && 2 * i + 1 < points.size())
				{
					hLine(points[2 * i].front().y, points[2 * i].front().x, points[2 * i + 1].back().x, baseInfo, lastInfo);
				}
				else
				{
					points.clear();
					break;
				}
			}
		}
	}
}
void fillPolygon(const std::vector<Point>& points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, bool realPoint = false)
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

		fillWithActiveLines(curY, sortedLines[i + 1].scanY, activeLines, baseInfo, lastInfo, realPoint);
	}

}
void lineSSAA(int x0, int y0, int xEnd, int yEnd, int AAlevel, bool realPoint = false)
{
	glColor3f(1.0, 1.0, 1.0);
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	
	int subX0 = 0;
	int subY0 = 0;
	int subXEnd = subX0 + (xEnd - x0) * AAlevel;
	int SubYEnd = subY0 + (yEnd - y0) * AAlevel;

	BaseInfo baseInfo = { x0 , y0, xEnd, yEnd, AAlevel};
	std::map<Point, int> lastInfo;

	int dx = subXEnd - subX0;
	int dy = SubYEnd - subY0;
	float aa;
	int offset = Round(std::modf(AAlevel / 2, &aa)) == 0 ? 1 : 0;

	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;

	int fixed = 0;
	switch (AAlevel)
	{
	case 2:
		fixed = 0;
		break;
	case 4:
		fixed = 1;
		break;
	case 8:
		fixed = 2;
		break;
	default:
		break;
	}
	int vertexX = Round(std::cos(sita) * ((float)AAlevel / 2 + fixed));
	int vertexY = Round(std::sin(sita) * ((float)AAlevel / 2 + fixed));

	std::vector<Point> points;
	points.push_back({ subX0 + vertexX, subY0 + vertexY });
	points.push_back({ subX0 - vertexX , subY0 - vertexY });
	points.push_back({ subXEnd - vertexX , SubYEnd - vertexY });
	points.push_back({ subXEnd + vertexX , SubYEnd + vertexY });
	
	//lastInfo[{0, 0}] = AAlevel * AAlevel;
	fillPolygon(points, baseInfo, lastInfo, realPoint);
	refreshLastPoints(baseInfo, lastInfo);
}
void polygonSSAA(const std::vector<Point>& points, int AAlevel, bool realPoint = false)
{
	glColor3f(1.0, 1.0, 1.0);
	std::vector<Point> subPoints;

	for (int i = 0; i < points.size(); i++)
	{
		int subX0 = (points[i].x - points[0].x) * AAlevel;
		int subY0 = (points[i].y - points[0].y) * AAlevel;

		subPoints.push_back({ subX0, subY0 });
	}

	BaseInfo baseInfo = { points[0].x , points[0].y, points[points.size() - 1].x, points[points.size() - 1].y, AAlevel };
	std::map<Point, int> lastInfo;

	fillPolygon(subPoints, baseInfo, lastInfo, realPoint);
	refreshLastPoints(baseInfo, lastInfo);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	// Bresenham直线边算法
	lineBres(20, 568, 158, 573);
	lineSSAA(20, 538, 158, 543, 2);
	lineSSAA(20, 508, 158, 513, 4);
	lineSSAA(20, 478, 158, 483, 8);

	glColor3f(1.0, 1.0, 1.0);
	lineBres(210, 495, 218, 590);
	lineSSAA(240, 495, 248, 590, 2);
	lineSSAA(270, 495, 278, 590, 4);
	lineSSAA(300, 495, 308, 590, 8);

	glColor3f(1.0, 1.0, 1.0);
	polygonSSAA({ { 119, 387 },{ 322, 398 },{ 277, 450 },{ 145, 443 } }, 1);
	polygonSSAA({ { 119, 287 },{ 322, 298 },{ 277, 350 },{ 145, 343 } }, 2);
	polygonSSAA({ { 119, 187 },{ 322, 198 },{ 277, 250 },{ 145, 243 } }, 4);
	polygonSSAA({ { 119, 87 },{ 322, 98 },{ 277, 150 },{ 145, 143 } }, 8);

	//auto time0 = clock();
	//auto time1 = clock();
	//auto aaa = time1 - time0;
	
	// 直接计算边，直接计算扫描线交点
	glColor3f(1.0, 1.0, 1.0);
	lineBres(420, 568, 558, 573);
	lineSSAA(420, 538, 558, 543, 2, true);
	lineSSAA(420, 508, 558, 513, 4, true);
	lineSSAA(420, 478, 558, 483, 8, true);

	glColor3f(1.0, 1.0, 1.0);
	lineBres(610, 495, 618, 590);
	lineSSAA(640, 495, 648, 590, 2, true);
	lineSSAA(670, 495, 678, 590, 4, true);
	lineSSAA(700, 495, 708, 590, 8, true);

	glColor3f(1.0, 1.0, 1.0);
	polygonSSAA({ { 519, 387 },{ 722, 398 },{ 677, 450 },{ 545, 443 } }, 1, true);
	polygonSSAA({ { 519, 287 },{ 722, 298 },{ 677, 350 },{ 545, 343 } }, 2, true);
	polygonSSAA({ { 519, 187 },{ 722, 198 },{ 677, 250 },{ 545, 243 } }, 4, true);
	polygonSSAA({ { 519, 87 },{ 722, 98 },{ 677, 150 },{ 545, 143 } }, 8, true);

	auto time0 = clock();
	//lineSSAA(20, 538, 158, 543, 8);
	//lineSSAA(270, 495, 278, 590, 8);
	//polygonSSAA({ { 519, 187 },{ 722, 198 },{ 677, 250 },{ 545, 243 } }, 8);
	auto time1 = clock();
	auto aaa = time1 - time0;
	
	glFlush();
}
void code_6_exercise_54_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test2
struct Point { int x; int y; };
struct Color3f
{
	GLfloat r, g, b;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
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
struct LineRecord
{
	int beginX;
	int endX;
};
struct BaseInfo
{
	int x0;
	int y0;
	int xEnd;
	int yEnd;
	int AALevel;
};
//// 判断浮点数相等
//bool Equal(float f1, float f2) { return std::abs(f1 - f2) < 0.0001; }
//bool Greater(float f1, float f2) { return Equal(f1, f2) ? false : (f1 > f2); }
//bool Less(float f1, float f2) { return Equal(f1, f2) ? false : (f1 < f2); }
//bool GreaterQ(float f1, float f2) { return Greater(f1, f2) || Equal(f1, f2); }
//bool LessQ(float f1, float f2) { return Less(f1, f2) || Equal(f1, f2); }
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
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
void setGrayPixel(int x, int y, float grayPercent)
{
	glColor3f(1.0 * grayPercent, 1.0 * grayPercent, 1.0 * grayPercent);
	setPixel(x, y);
	//printf("%d,%d(%f)\n", x, y, grayPercent);
}
void setHLine(Point begin, Point end, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, int hCount)
{
	//printf("%d,%d(%d) -> %d,%d(%d)\n", begin.x, begin.y, lastInfo[begin], end.x, end.y, lastInfo[end]);

	float squareAA = baseInfo.AALevel * baseInfo.AALevel;
	for (int i = begin.x; i <= end.x; i++)
	{
		auto it = lastInfo.find({ i, begin.y });
		if (it != lastInfo.end())
		{
			float count = it->second;
			if (count > squareAA)
				count = squareAA;

			setGrayPixel(baseInfo.x0 + i, baseInfo.y0 + begin.y, count / squareAA);
		}
		else
		{
			if (hCount != baseInfo.AALevel)
				setGrayPixel(baseInfo.x0 + i, baseInfo.y0 + begin.y, (float)hCount / baseInfo.AALevel);
			else
				setGrayPixel(baseInfo.x0 + i, baseInfo.y0 + begin.y, 1);
		}
	}
	lastInfo.clear();
}
Point dealLeftPoint(const std::vector<Point> points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	int curY = floor((float)points.front().y / baseInfo.AALevel);

	int beginX = floor((float)points.front().x / baseInfo.AALevel);
	int endX = floor((float)points.back().x / baseInfo.AALevel);
	if (beginX > endX)
		std::swap(beginX, endX);

	for (auto p : points)
	{
		int curX = floor((float)p.x / baseInfo.AALevel);
		for (int i = beginX; i <= endX; i++)
		{
			if (curX == i)
			{
				if (p.x < 0)
				{
					float remainder = (-1 * p.x) % baseInfo.AALevel;
					if (remainder == 0)
						remainder = baseInfo.AALevel;
					lastInfo[{i, curY}] += remainder;
				}
				else
				{
					lastInfo[{i, curY}] += baseInfo.AALevel - (p.x % baseInfo.AALevel);
				}				
			}
			else if(i > curX)
			{
				lastInfo[{i, curY}] += baseInfo.AALevel;
			}
		}
	}
	return { min(beginX, endX), curY };
}
Point dealRightPoint(const std::vector<Point> points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	int curY = floor((float)points.front().y / baseInfo.AALevel);

	int beginX = floor((float)points.front().x / baseInfo.AALevel);
	int endX = floor((float)points.back().x / baseInfo.AALevel);

	if (beginX > endX)
		std::swap(beginX, endX);

	for (auto p : points)
	{
		int curX = floor((float)p.x / baseInfo.AALevel);
		for (int i = beginX; i <= endX; i++)
		{
			if (curX == i)
			{
				if (p.x < 0)
				{
					float remainder = (-1 * p.x) % baseInfo.AALevel;
					if (remainder == 0)
						remainder = baseInfo.AALevel;
					lastInfo[{i, curY}] += baseInfo.AALevel - remainder + 1;
				}
				else
				{
					lastInfo[{i, curY}] += (p.x % baseInfo.AALevel) + 1;
				}				
			}
			else if (i < curX)
			{
				lastInfo[{i, curY}] += baseInfo.AALevel;
			}
		}
	}
	return{ max(beginX, endX), curY };
}
void hLine(const std::vector<std::vector<Point>> points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	std::vector<Point> lefts;
	std::vector<Point> rights;
	for (auto p : points)
	{
		lefts.push_back(p[0]);
		rights.push_back(p[1]);
	}
	int leftBeginX = floor((float)lefts.front().x / baseInfo.AALevel);
	int leftEndX = floor((float)lefts.back().x / baseInfo.AALevel);
	if (leftBeginX > leftEndX)
		std::swap(leftBeginX, leftEndX);

	int rightBeginX = floor((float)rights.front().x / baseInfo.AALevel);
	int rightEndX = floor((float)rights.back().x / baseInfo.AALevel);
	if (rightBeginX > rightEndX)
		std::swap(rightBeginX, rightEndX);

	if (leftEndX >= rightBeginX)
	{
		int y = floor((float)lefts.front().y / baseInfo.AALevel);
		for (int i = 0 ; i < lefts.size(); i++)
		{
			for (int curX = lefts[i].x; curX <= rights[i].x; curX++)
			{
				int x = floor((float)curX / baseInfo.AALevel);
				lastInfo[{x, y}] ++;
			}
		}
		setHLine({ leftBeginX , y}, { rightEndX , y }, baseInfo, lastInfo, points.size());
	}
	else
	{
		auto begin = dealLeftPoint(lefts, baseInfo, lastInfo);
		auto end = dealRightPoint(rights, baseInfo, lastInfo);
		setHLine(begin, end, baseInfo, lastInfo, points.size());
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
			std::swap(line.y0, line.y1)CHAPTER_6_EXERCISE_54_Test3
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
void fillWithActiveLines(int beginY, int endY, std::vector<ActiveLine>& activeLines, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, std::map<int, std::vector<std::vector<Point>>>& pixelInfo, int& lastY)
{
	std::vector<std::vector<Point>> points;
	int currentY = 0;
	for (int curY = beginY; curY < endY; curY++)
	{
		for (auto& line : activeLines)
		{
			if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
			{
				if (std::abs(line.sortedLine.dy) >= std::abs(line.sortedLine.dx))
				{// |m|>1			
					points.push_back({ { line.currentX , curY } });
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
					points.push_back({ { line.currentX, curY } });
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
					points.back().push_back({ line.currentX - 1, curY });
				}
			}
		}
		std::sort(points.begin(), points.end(), [](auto& a, auto&b) {return a.front().x < b.front().x;});
		for (int i = 0; ; i++)
		{
			if (2 * i < points.size() && 2 * i + 1 < points.size())
			{					
				currentY = floor((float)points[2 * i].front().y / baseInfo.AALevel);
					
				if (lastY == -99999)
					lastY = currentY;

				if (currentY != lastY)
				{
					for (auto& info : pixelInfo)
					{
						if (info.second.size())
						{
							hLine(info.second, baseInfo, lastInfo);
							info.second.clear();
						}
					}
					lastY = currentY;
				}

				if (pixelInfo.find(i) == pixelInfo.end())
					pixelInfo[i] = std::vector<std::vector<Point>>();

				pixelInfo[i].push_back({
					{ points[2 * i].front().x , points[2 * i].front().y },
					{ points[2 * i + 1].back().x , points[2 * i + 1].back().y } });
			}
			else
			{
				points.clear();
				break;
			}
		}
	}
}
void fillPolygon(const std::vector<Point>& points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo)
{
	std::vector<SortedLineSet> sortedLines = SortLines(points);
	std::vector<ActiveLine> activeLines;
	std::map<int, std::vector<std::vector<Point>>> pixelInfo;
	int lastY = -99999;

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

		fillWithActiveLines(curY, sortedLines[i + 1].scanY, activeLines, baseInfo, lastInfo, pixelInfo, lastY);
	}

	for (auto& info : pixelInfo)
	{
		if (info.second.size())
		{
			hLine(info.second, baseInfo, lastInfo);
			info.second.clear();
		}
	}
}
void lineMSAA(int x0, int y0, int xEnd, int yEnd, int AAlevel)
{
	glColor3f(1.0, 1.0, 1.0);
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int subX0 = 0;
	int subY0 = 0;
	int subXEnd = subX0 + (xEnd - x0) * AAlevel;
	int SubYEnd = subY0 + (yEnd - y0) * AAlevel;

	BaseInfo baseInfo = { x0 , y0, xEnd, yEnd, AAlevel };
	std::map<Point, int> lastInfo;

	int dx = subXEnd - subX0;
	int dy = SubYEnd - subY0;
	float aa;
	int offset = Round(std::modf(AAlevel / 2, &aa)) == 0 ? 1 : 0;

	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;

	int fixed = 0;
	switch (AAlevel)
	{
	case 2:
		fixed = 0;
		break;
	case 4:
		fixed = 1;
		break;
	case 8:
		fixed = 2;
		break;
	default:
		break;
	}
	int vertexX = Round(std::cos(sita) * ((float)AAlevel / 2 + fixed));
	int vertexY = Round(std::sin(sita) * ((float)AAlevel / 2 + fixed));

	std::vector<Point> points;
	points.push_back({ subX0 + vertexX, subY0 + vertexY });
	points.push_back({ subX0 - vertexX , subY0 - vertexY });
	points.push_back({ subXEnd - vertexX , SubYEnd - vertexY });
	points.push_back({ subXEnd + vertexX , SubYEnd + vertexY });

	fillPolygon(points, baseInfo, lastInfo);
}
void polygonMSAA(const std::vector<Point>& points, int AAlevel)
{
	glColor3f(1.0, 1.0, 1.0);
	std::vector<Point> subPoints;

	for (int i = 0; i < points.size(); i++)
	{
		int subX0 = (points[i].x - points[0].x) * AAlevel;
		int subY0 = (points[i].y - points[0].y) * AAlevel;

		subPoints.push_back({ subX0, subY0 });
	}

	BaseInfo baseInfo = { points[0].x , points[0].y, points[points.size() - 1].x, points[points.size() - 1].y, AAlevel };
	std::map<Point, int> lastInfo;

	fillPolygon(subPoints, baseInfo, lastInfo);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	
	lineBres(20, 568, 158, 573);
	lineMSAA(20, 538, 158, 543, 2);
	lineMSAA(20, 508, 158, 513, 4);
	lineMSAA(20, 478, 158, 483, 8);
	
	glColor3f(1.0, 1.0, 1.0);
	lineBres(210, 495, 218, 590);
	lineMSAA(240, 495, 248, 590, 2);
	lineMSAA(270, 495, 278, 590, 4);
	lineMSAA(300, 495, 308, 590, 8);
	
	glColor3f(1.0, 1.0, 1.0);
	polygonMSAA({ { 119, 387 },{ 322, 398 },{ 277, 450 },{ 145, 443 } }, 1);
	polygonMSAA({ { 119, 287 },{ 322, 298 },{ 277, 350 },{ 145, 343 } }, 2);
	polygonMSAA({ { 119, 187 },{ 322, 198 },{ 277, 250 },{ 145, 243 } }, 4);
	polygonMSAA({ { 119, 87 },{ 322, 98 },{ 277, 150 },{ 145, 143 } }, 8);

	auto time0 = clock();
	//lineMSAA(20, 538, 158, 543, 8);
	//lineMSAA(270, 495, 278, 590, 8);
	//polygonMSAA({ { 519, 187 },{ 722, 198 },{ 677, 250 },{ 545, 243 } }, 8);
	auto time1 = clock();
	auto aaa = time1 - time0;

	glFlush();
}
void code_6_exercise_54_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test3
struct Point { int x; int y; };
struct Color3f
{
	GLfloat r, g, b;
};
struct Stencil
{
	std::vector<std::vector<int>> stencil;
	int xc;
	int yc;
};
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
struct LineRecord
{
	int beginX;
	int endX;
};
struct BaseInfo
{
	int x0;
	int y0;
	int xEnd;
	int yEnd;
	int AALevel;
};
struct WeightingMaskInfo
{
	std::vector<std::vector<int>> weightingMask;
	int total;
};
inline int Round(const float a)
{
	if (a >= 0)
		return int(a + 0.5);
	else
		return int(a - 0.5);
}
// 0<m<1
void lineBres1(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);

	int x = x0;
	int y = y0;
	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// m>1
void lineBres1M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = fabs((float)xEnd - x0), dy = fabs((float)yEnd - y0);
	int p = dy - 2 * dx;
	int twoDx = -2 * dx, twoDyMinusDx = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y < yEnd)
	{
		y++;
		if (p > 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
// -1<m<0
void lineBres2(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = 2 * dy + dx;
	int twoDy = 2 * dy, twoDyAddDx = 2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (x < xEnd)
	{
		x++;
		if (p >= 0)
			p += twoDy;
		else
		{
			y--;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
// m<-1
void lineBres2M(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;
		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int dx = (float)xEnd - x0, dy = (float)yEnd - y0;
	int p = -2 * dx - dy;
	int twoDx = -2 * dx, twoDyAddDx = -2 * (dy + dx);

	int x = x0;
	int y = y0;

	setPixel(x, y);
	while (y > yEnd)
	{
		y--;
		if (p >= 0)
			p += twoDx;
		else
		{
			x++;
			p += twoDyAddDx;
		}
		setPixel(x, y);
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}
	int dx = xEnd - x0;
	int dy = yEnd - y0;
	if (dy > 0)
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres1M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres1(x0, y0, xEnd, yEnd);
		}
	}
	else
	{
		if (fabs((float)dy) > fabs((float)dx))
		{
			lineBres2M(x0, y0, xEnd, yEnd);
		}
		else
		{
			lineBres2(x0, y0, xEnd, yEnd);
		}
	}
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
void setGrayPixel(int x, int y, float grayPercent)
{
	glColor3f(1.0 * grayPercent, 1.0 * grayPercent, 1.0 * grayPercent);
	setPixel(x, y);
	//printf("%d,%d(%f)\n", x, y, grayPercent);
}
int getWeighting(int subx, int suby, const WeightingMaskInfo& weightInfo)
{
	int x = subx % (int)weightInfo.weightingMask.size();
	if (x < 0)
	{
		x += weightInfo.weightingMask.size();
	}
	int y = suby % (int)weightInfo.weightingMask.size();
	if (y < 0)
	{
		y += weightInfo.weightingMask.size();
	}
	return weightInfo.weightingMask[weightInfo.weightingMask.size() - 1 - y][x];
}
void refreshLastPoints(const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, const WeightingMaskInfo& weightInfo)
{
	//int max = -1;
	//int min = 99999;
	//int  y = 0;
	for (auto p : lastInfo)
	{
		float count = p.second;
		if (count > weightInfo.total)
			count = weightInfo.total;

		//if (p.first.x > max)
		//	max = p.first.x;

		//if (p.first.x < min)
		//	min = p.first.x;

		//y = p.first.y;

		setGrayPixel(baseInfo.x0 + p.first.x, baseInfo.y0 + p.first.y, count / weightInfo.total);
	}
	//printf("%d,%d(%d) -> %d,%d(%d)\n", min, y, lastInfo[{min, y}],  max, y, lastInfo[{max, y}]);
}
void setSubPixel(int subX, int subY, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, const WeightingMaskInfo& weightInfo)
{
	//printf("x=%d, y=%d\n", subX, subY);
	int curX = floor((float)subX / baseInfo.AALevel);
	int curY = floor((float)subY / baseInfo.AALevel);
	if (!lastInfo.empty())
	{
		if (curY != lastInfo.begin()->first.y)
		{
			refreshLastPoints(baseInfo, lastInfo, weightInfo);
			lastInfo.clear();
		}
	}
	lastInfo[{curX, curY}] += getWeighting(subX, subY, weightInfo);
}
void hLine(int y, int x0, int x1, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, const WeightingMaskInfo& weightInfo)
{
	for (int x = x0; x <= x1; x++)
	{
		if (baseInfo.AALevel > 1)
			setSubPixel(x, y, baseInfo, lastInfo, weightInfo);
		else
			setPixel(baseInfo.x0 + x, baseInfo.y0 + y);
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
void fillWithActiveLines(int beginY, int endY, std::vector<ActiveLine>& activeLines, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, bool realPoint, const WeightingMaskInfo& weightInfo)
{
	if (realPoint)
	{ // 直接计算扫描线交点
		std::vector<Point> points;
		for (int curY = beginY; curY < endY; curY++)
		{
			for (auto& line : activeLines)
			{
				if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
				{
					points.push_back({ line.currentX , curY });
					//printf("x=%d, y=%d\n", baseInfo.x0 + line.currentX, baseInfo.y0 + curY);
					float x = (curY + 1 - line.sortedLine.minY) / ((float)(line.sortedLine.dy) / line.sortedLine.dx) + line.sortedLine.beginX;
					line.currentX = Round(x);
					//printf("x=%f, y=%d\n", baseInfo.x0 + x, baseInfo.y0 + curY + 1);
				}
			}
			std::sort(points.begin(), points.end(), [](auto& a, auto&b) {return a.x < b.x;});
			for (int i = 0; ; i++)
			{
				if (2 * i < points.size() && 2 * i + 1 < points.size())
				{
					hLine(points[2 * i].y, points[2 * i].x, points[2 * i + 1].x, baseInfo, lastInfo, weightInfo);
				}
				else
				{
					points.clear();
					break;
				}
			}
		}
	}
	else
	{ // bres直线算法
		std::vector<std::vector<Point>> points;
		for (int curY = beginY; curY < endY; curY++)
		{
			for (auto& line : activeLines)
			{
				if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
				{
					if (std::abs(line.sortedLine.dy) >= std::abs(line.sortedLine.dx))
					{// |m|>1			
						points.push_back({ { line.currentX , curY } });
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
						points.push_back({ { line.currentX, curY } });
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
						points.back().push_back({ line.currentX - 1, curY });
					}
				}
			}
			std::sort(points.begin(), points.end(), [](auto& a, auto&b) {return a.front().x < b.front().x;});
			for (int i = 0; ; i++)
			{
				if (2 * i < points.size() && 2 * i + 1 < points.size())
				{
					hLine(points[2 * i].front().y, points[2 * i].front().x, points[2 * i + 1].back().x, baseInfo, lastInfo, weightInfo);
				}
				else
				{
					points.clear();
					break;
				}
			}
		}
	}
}
void fillPolygon(const std::vector<Point>& points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, bool realPoint, const WeightingMaskInfo& weightInfo)
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

		fillWithActiveLines(curY, sortedLines[i + 1].scanY, activeLines, baseInfo, lastInfo, realPoint, weightInfo);
	}

}
void calcWeightingTotal(WeightingMaskInfo& weightInfo)
{
	weightInfo.total = 0;
	for (int i = 0; i < weightInfo.weightingMask.size(); i++)
	{
		for (int j = 0; j < weightInfo.weightingMask[i].size(); j++)
			weightInfo.total += weightInfo.weightingMask[i][j];
	}
}
void lineSSAA(int x0, int y0, int xEnd, int yEnd, int AAlevel, bool realPoint, WeightingMaskInfo& weightInfo)
{
	glColor3f(1.0, 1.0, 1.0);
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int subX0 = 0;
	int subY0 = 0;
	int subXEnd = subX0 + (xEnd - x0) * AAlevel;
	int SubYEnd = subY0 + (yEnd - y0) * AAlevel;

	BaseInfo baseInfo = { x0 , y0, xEnd, yEnd, AAlevel };
	std::map<Point, int> lastInfo;

	int dx = subXEnd - subX0;
	int dy = SubYEnd - subY0;
	float aa;
	int offset = Round(std::modf(AAlevel / 2, &aa)) == 0 ? 1 : 0;

	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;

	int fixed = 0;
	switch (AAlevel)
	{
	case 2:
		fixed = 0;
		break;
	case 4:
		fixed = 1;
		break;
	case 8:
		fixed = 2;
		break;
	default:
		break;
	}
	int vertexX = Round(std::cos(sita) * ((float)AAlevel / 2 + fixed));
	int vertexY = Round(std::sin(sita) * ((float)AAlevel / 2 + fixed));

	std::vector<Point> points;
	points.push_back({ subX0 + vertexX, subY0 + vertexY });
	points.push_back({ subX0 - vertexX , subY0 - vertexY });
	points.push_back({ subXEnd - vertexX , SubYEnd - vertexY });
	points.push_back({ subXEnd + vertexX , SubYEnd + vertexY });

	//lastInfo[{0, 0}] = AAlevel * AAlevel;
	calcWeightingTotal(weightInfo);
	fillPolygon(points, baseInfo, lastInfo, realPoint, weightInfo);
	refreshLastPoints(baseInfo, lastInfo, weightInfo);
}
void polygonSSAA(const std::vector<Point>& points, int AAlevel, bool realPoint, WeightingMaskInfo& weightInfo)
{
	glColor3f(1.0, 1.0, 1.0);
	std::vector<Point> subPoints;

	for (int i = 0; i < points.size(); i++)
	{
		int subX0 = (points[i].x - points[0].x) * AAlevel;
		int subY0 = (points[i].y - points[0].y) * AAlevel;

		subPoints.push_back({ subX0, subY0 });
	}

	BaseInfo baseInfo = { points[0].x , points[0].y, points[points.size() - 1].x, points[points.size() - 1].y, AAlevel };
	std::map<Point, int> lastInfo;

	calcWeightingTotal(weightInfo);
	fillPolygon(subPoints, baseInfo, lastInfo, realPoint, weightInfo);
	refreshLastPoints(baseInfo, lastInfo, weightInfo);
}
void setHLine(Point begin, Point end, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, const WeightingMaskInfo& weightInfo)
{
	//printf("%d,%d(%d) -> %d,%d(%d)\n", begin.x, begin.y, lastInfo[begin], end.x, end.y, lastInfo[end]);

	for (int i = begin.x; i <= end.x; i++)
	{
		auto it = lastInfo.find({ i, begin.y });
		if (it != lastInfo.end())
		{
			float count = it->second;
			if (count > weightInfo.total)
				count = weightInfo.total;

			setGrayPixel(baseInfo.x0 + i, baseInfo.y0 + begin.y, count / weightInfo.total);
		}
		else
		{
			setGrayPixel(baseInfo.x0 + i, baseInfo.y0 + begin.y, 1);
		}
	}
	lastInfo.clear();
}
int getWeightingLeft(int subx, int suby, const WeightingMaskInfo& weightInfo)
{
	int ret = 0;
	int x = subx % (int)weightInfo.weightingMask.size();
	if (x < 0)
	{
		x += weightInfo.weightingMask.size();
	}
	int y = suby % (int)weightInfo.weightingMask.size();
	if (y < 0)
	{
		y += weightInfo.weightingMask.size();
	}
	for (int i = x; i < weightInfo.weightingMask.size(); i++)
	{
		ret += weightInfo.weightingMask[weightInfo.weightingMask.size() - 1 - y][i];
	}
	return ret;
}
int getWeightingRight(int subx, int suby, const WeightingMaskInfo& weightInfo)
{
	int ret = 0;
	int x = subx % (int)weightInfo.weightingMask.size();
	if (x < 0)
	{
		x += weightInfo.weightingMask.size();
	}
	int y = suby % (int)weightInfo.weightingMask.size();
	if (y < 0)
	{
		y += weightInfo.weightingMask.size();
	}
	for (int i = 0; i <= x; i++)
	{
		ret += weightInfo.weightingMask[weightInfo.weightingMask.size() - 1 - y][i];
	}
	return ret;
}
Point dealLeftPoint(const std::vector<Point> points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, const WeightingMaskInfo& weightInfo)
{
	int curY = floor((float)points.front().y / baseInfo.AALevel);

	int beginX = floor((float)points.front().x / baseInfo.AALevel);
	int endX = floor((float)points.back().x / baseInfo.AALevel);
	if (beginX > endX)
		std::swap(beginX, endX);

	for (auto p : points)
	{
		int curX = floor((float)p.x / baseInfo.AALevel);
		for (int i = beginX; i <= endX; i++)
		{
			if (curX == i)
			{
				lastInfo[{i, curY}] += getWeightingLeft(p.x, p.y, weightInfo);
			}
			else if (i > curX)
			{
				lastInfo[{i, curY}] += getWeightingLeft(0, p.y, weightInfo);
			}
		}
	}
	return{ min(beginX, endX), curY };
}
Point dealRightPoint(const std::vector<Point> points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, const WeightingMaskInfo& weightInfo)
{
	int curY = floor((float)points.front().y / baseInfo.AALevel);

	int beginX = floor((float)points.front().x / baseInfo.AALevel);
	int endX = floor((float)points.back().x / baseInfo.AALevel);

	if (beginX > endX)
		std::swap(beginX, endX);

	for (auto p : points)
	{
		int curX = floor((float)p.x / baseInfo.AALevel);
		for (int i = beginX; i <= endX; i++)
		{
			if (curX == i)
			{
				lastInfo[{i, curY}] += getWeightingRight(p.x, p.y, weightInfo);
			}
			else if (i < curX)
			{
				lastInfo[{i, curY}] += getWeightingLeft(0, p.y, weightInfo);
			}
		}
	}
	return{ max(beginX, endX), curY };
}
void hLineMSAA(const std::vector<std::vector<Point>> points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, const WeightingMaskInfo& weightInfo)
{
	std::vector<Point> lefts;
	std::vector<Point> rights;
	for (auto p : points)
	{
		lefts.push_back(p[0]);
		rights.push_back(p[1]);
	}
	int leftBeginX = floor((float)lefts.front().x / baseInfo.AALevel);
	int leftEndX = floor((float)lefts.back().x / baseInfo.AALevel);
	if (leftBeginX > leftEndX)
		std::swap(leftBeginX, leftEndX);

	int rightBeginX = floor((float)rights.front().x / baseInfo.AALevel);
	int rightEndX = floor((float)rights.back().x / baseInfo.AALevel);
	if (rightBeginX > rightEndX)
		std::swap(rightBeginX, rightEndX);

	if (leftEndX >= rightBeginX)
	{
		int y = floor((float)lefts.front().y / baseInfo.AALevel);
		for (int i = 0; i < lefts.size(); i++)
		{
			for (int curX = lefts[i].x; curX <= rights[i].x; curX++)
			{
				int x = floor((float)curX / baseInfo.AALevel);
				lastInfo[{x, y}] += getWeighting(curX, lefts[i].y, weightInfo);
			}
		}
		setHLine({ leftBeginX , y }, { rightEndX , y }, baseInfo, lastInfo, weightInfo);
	}
	else
	{
		auto begin = dealLeftPoint(lefts, baseInfo, lastInfo, weightInfo);
		auto end = dealRightPoint(rights, baseInfo, lastInfo, weightInfo);
		if (points.size() < baseInfo.AALevel)
		{
			int weight = 0;
			for (int j = 0; j < lefts.size(); j++)
			{
				weight += getWeightingLeft(0, lefts[j].y, weightInfo);
			}

			for (int i = begin.x; i <= end.x; i++)
			{
				if (lastInfo.find({ i, begin.y }) == lastInfo.end())
				{
					lastInfo[{i, begin.y}] = weight;
				}
			}
		}
		setHLine(begin, end, baseInfo, lastInfo, weightInfo);
	}
}
void fillWithActiveLinesMSAA(int beginY, int endY, std::vector<ActiveLine>& activeLines, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, std::map<int, std::vector<std::vector<Point>>>& pixelInfo, int& lastY, const WeightingMaskInfo& weightInfo)
{
	std::vector<std::vector<Point>> points;
	int currentY = 0;
	for (int curY = beginY; curY < endY; curY++)
	{
		for (auto& line : activeLines)
		{
			if (curY >= line.sortedLine.minY && curY <= line.sortedLine.maxY)
			{
				if (std::abs(line.sortedLine.dy) >= std::abs(line.sortedLine.dx))
				{// |m|>1			
					points.push_back({ { line.currentX , curY } });
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
					points.push_back({ { line.currentX, curY } });
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
					points.back().push_back({ line.currentX - 1, curY });
				}
			}
		}
		std::sort(points.begin(), points.end(), [](auto& a, auto&b) {return a.front().x < b.front().x;});
		for (int i = 0; ; i++)
		{
			if (2 * i < points.size() && 2 * i + 1 < points.size())
			{
				currentY = floor((float)points[2 * i].front().y / baseInfo.AALevel);

				if (lastY == -99999)
					lastY = currentY;

				if (currentY != lastY)
				{
					for (auto& info : pixelInfo)
					{
						if (info.second.size())
						{
							hLineMSAA(info.second, baseInfo, lastInfo, weightInfo);
							info.second.clear();
						}
					}
					lastY = currentY;
				}

				if (pixelInfo.find(i) == pixelInfo.end())
					pixelInfo[i] = std::vector<std::vector<Point>>();

				pixelInfo[i].push_back({
					{ points[2 * i].front().x , points[2 * i].front().y },
					{ points[2 * i + 1].back().x , points[2 * i + 1].back().y } });
			}
			else
			{
				points.clear();
				break;
			}
		}
	}
}
void fillPolygonMSAA(const std::vector<Point>& points, const BaseInfo& baseInfo, std::map<Point, int>& lastInfo, const WeightingMaskInfo& weightInfo)
{
	std::vector<SortedLineSet> sortedLines = SortLines(points);
	std::vector<ActiveLine> activeLines;
	std::map<int, std::vector<std::vector<Point>>> pixelInfo;
	int lastY = -99999;

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

		fillWithActiveLinesMSAA(curY, sortedLines[i + 1].scanY, activeLines, baseInfo, lastInfo, pixelInfo, lastY, weightInfo);
	}

	for (auto& info : pixelInfo)
	{
		if (info.second.size())
		{
			hLineMSAA(info.second, baseInfo, lastInfo, weightInfo);
			info.second.clear();
		}
	}
}
void lineMSAA(int x0, int y0, int xEnd, int yEnd, int AAlevel, WeightingMaskInfo& weightInfo)
{
	glColor3f(1.0, 1.0, 1.0);
	if (x0 > xEnd)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = xEnd;

		y0 = yEnd;
		xEnd = tempx;
		yEnd = tempy;
	}

	int subX0 = 0;
	int subY0 = 0;
	int subXEnd = subX0 + (xEnd - x0) * AAlevel;
	int SubYEnd = subY0 + (yEnd - y0) * AAlevel;

	BaseInfo baseInfo = { x0 , y0, xEnd, yEnd, AAlevel };
	std::map<Point, int> lastInfo;

	int dx = subXEnd - subX0;
	int dy = SubYEnd - subY0;
	float aa;
	int offset = Round(std::modf(AAlevel / 2, &aa)) == 0 ? 1 : 0;

	float sita;
	if (dy)
		sita = std::atan((float)-dx / dy);
	else
		sita = PI / 2;

	int fixed = 0;
	switch (AAlevel)
	{
	case 2:
		fixed = 0;
		break;
	case 4:
		fixed = 1;
		break;
	case 8:
		fixed = 2;
		break;
	default:
		break;
	}
	int vertexX = Round(std::cos(sita) * ((float)AAlevel / 2 + fixed));
	int vertexY = Round(std::sin(sita) * ((float)AAlevel / 2 + fixed));

	std::vector<Point> points;
	points.push_back({ subX0 + vertexX, subY0 + vertexY });
	points.push_back({ subX0 - vertexX , subY0 - vertexY });
	points.push_back({ subXEnd - vertexX , SubYEnd - vertexY });
	points.push_back({ subXEnd + vertexX , SubYEnd + vertexY });

	calcWeightingTotal(weightInfo);
	fillPolygonMSAA(points, baseInfo, lastInfo, weightInfo);
}
//void polygonMSAA(const std::vector<Point>& points, int AAlevel)
//{
//	glColor3f(1.0, 1.0, 1.0);
//	std::vector<Point> subPoints;
//
//	for (int i = 0; i < points.size(); i++)
//	{
//		int subX0 = (points[i].x - points[0].x) * AAlevel;
//		int subY0 = (points[i].y - points[0].y) * AAlevel;
//
//		subPoints.push_back({ subX0, subY0 });
//	}
//
//	BaseInfo baseInfo = { points[0].x , points[0].y, points[points.size() - 1].x, points[points.size() - 1].y, AAlevel };
//	std::map<Point, int> lastInfo;
//
//	fillPolygonMSAA(subPoints, baseInfo, lastInfo);
//}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	WeightingMaskInfo weightInfo3 = {
		{
			{ 1,2,1 },
			{ 2,4,2 },
			{ 1,2,1 },
		},0
	};

	WeightingMaskInfo weightInfo4 = {
		{
			{ 1,2,2,1 },
			{ 2,4,4,2 },
			{ 2,4,4,2 },
			{ 1,2,2,1 },
		},0
	};

	WeightingMaskInfo weightInfo8 = {
		{
			{ 1,2,2,2,2,2,2,1 },
			{ 2,4,4,4,4,4,4,2 },
			{ 2,4,4,4,4,4,4,2 },
			{ 2,4,4,4,4,4,4,2 },
			{ 2,4,4,4,4,4,4,2 },
			{ 2,4,4,4,4,4,4,2 },
			{ 2,4,4,4,4,4,4,2 },
			{ 1,2,2,2,2,2,2,1 },
		},0
	};



	// 超采样
	//glColor3f(1.0, 1.0, 1.0);
	//lineBres(20, 568, 158, 573);
	//lineSSAA(20, 538, 158, 543, 3, false, weightInfo3);
	//lineSSAA(20, 508, 158, 513, 4, false, weightInfo4);
	//lineSSAA(20, 478, 158, 483, 8, false, weightInfo8);
	//
	//glColor3f(1.0, 1.0, 1.0);
	//lineBres(210, 495, 218, 590);
	//lineSSAA(240, 495, 248, 590, 3, false, weightInfo3);
	//lineSSAA(270, 495, 278, 590, 4, false, weightInfo4);
	//lineSSAA(300, 495, 308, 590, 8, false, weightInfo8);
	//
	//glColor3f(1.0, 1.0, 1.0);
	//polygonSSAA({ { 119, 387 },{ 322, 398 },{ 277, 450 },{ 145, 443 } }, 1, false, weightInfo3);
	//polygonSSAA({ { 119, 287 },{ 322, 298 },{ 277, 350 },{ 145, 343 } }, 3, false, weightInfo3);
	//polygonSSAA({ { 119, 187 },{ 322, 198 },{ 277, 250 },{ 145, 243 } }, 4, false, weightInfo4);
	//polygonSSAA({ { 119, 87 },{ 322, 98 },{ 277, 150 },{ 145, 143 } }, 8, false, weightInfo8);

	// 多重采样
	glColor3f(1.0, 1.0, 1.0);
	lineBres(420, 568, 558, 573);
	//lineMSAA(420, 538, 558, 543, 2);
	lineMSAA(420, 508, 558, 513, 4, weightInfo4);
	//lineMSAA(420, 478, 558, 483, 8);

	/*glColor3f(1.0, 1.0, 1.0);
	lineBres(610, 495, 618, 590);
	lineMSAA(640, 495, 648, 590, 2);
	lineMSAA(670, 495, 678, 590, 4);
	lineMSAA(700, 495, 708, 590, 8);

	glColor3f(1.0, 1.0, 1.0);
	polygonMSAA({ { 519, 387 },{ 722, 398 },{ 677, 450 },{ 545, 443 } }, 1);
	polygonMSAA({ { 519, 287 },{ 722, 298 },{ 677, 350 },{ 545, 343 } }, 2);
	polygonMSAA({ { 519, 187 },{ 722, 198 },{ 677, 250 },{ 545, 243 } }, 4);
	polygonMSAA({ { 519, 87 },{ 722, 98 },{ 677, 150 },{ 545, 143 } }, 8);*/

	glFlush();
}
void code_6_exercise_54_3()
{
	glutDisplayFunc(drawFunc);
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_6_COMMON

#ifdef CHAPTER_6_COMMON
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
#ifdef CHAPTER_6_1_2
	code_6_1_2();
#endif

#ifdef CHAPTER_6_1_3
	code_6_1_3();
#endif

#ifdef CHAPTER_6_4_2
	code_6_4_2();
#endif

#ifdef CHAPTER_6_Test_1
	code_6_test_1();
#endif

#ifdef CHAPTER_6_5_2
	code_6_5_2();
#endif

#ifdef CHAPTER_6_EXERCISE_1
	code_6_exercise_1();
#endif

#ifdef CHAPTER_6_EXERCISE_2
	code_6_exercise_2();
#endif

#ifdef CHAPTER_6_EXERCISE_3 
	code_6_exercise_3();
#endif

#ifdef CHAPTER_6_EXERCISE_4
	code_6_exercise_4();
#endif

#ifdef CHAPTER_6_EXERCISE_5
	code_6_exercise_5();
#endif

#ifdef CHAPTER_6_EXERCISE_6
	code_6_exercise_6();
#endif

#ifdef CHAPTER_6_EXERCISE_7
	code_6_exercise_7();
#endif

#ifdef CHAPTER_6_EXERCISE_7_Test_1
	code_6_exercise_7_1();
#endif

#ifdef CHAPTER_6_EXERCISE_7_Test_2
	code_6_exercise_7_2();
#endif

#ifdef CHAPTER_6_EXERCISE_8
	code_6_exercise_8();
#endif

#ifdef CHAPTER_6_EXERCISE_9
	code_6_exercise_9();
#endif

#ifdef CHAPTER_6_EXERCISE_10
	code_6_exercise_10();
#endif

#ifdef CHAPTER_6_EXERCISE_11
	code_6_exercise_11();
#endif

#ifdef CHAPTER_6_EXERCISE_12
	code_6_exercise_12();
#endif

#ifdef CHAPTER_6_EXERCISE_13
	code_6_exercise_13();
#endif

#ifdef CHAPTER_6_EXERCISE_14
	code_6_exercise_14();
#endif

#ifdef CHAPTER_6_EXERCISE_15
	code_6_exercise_15();
#endif

#ifdef CHAPTER_6_EXERCISE_16
	code_6_exercise_16();
#endif

#ifdef CHAPTER_6_EXERCISE_16_Test1	
	code_6_exercise_16_1();
#endif

#ifdef CHAPTER_6_EXERCISE_17
	code_6_exercise_17();
#endif

#ifdef CHAPTER_6_EXERCISE_18
	code_6_exercise_18();
#endif

#ifdef CHAPTER_6_EXERCISE_19
	code_6_exercise_19();
#endif

#ifdef CHAPTER_6_EXERCISE_20
	code_6_exercise_20();
#endif

#ifdef CHAPTER_6_EXERCISE_21
	code_6_exercise_21();
#endif

#ifdef CHAPTER_6_EXERCISE_22
	code_6_exercise_22();
#endif

#ifdef CHAPTER_6_EXERCISE_23
	code_6_exercise_23();
#endif

#ifdef CHAPTER_6_EXERCISE_24
	code_6_exercise_24();
#endif

#ifdef CHAPTER_6_EXERCISE_25
	code_6_exercise_25();
#endif

#ifdef CHAPTER_6_EXERCISE_26
	code_6_exercise_26();
#endif

#ifdef CHAPTER_6_EXERCISE_27
	code_6_exercise_27();
#endif

#ifdef CHAPTER_6_EXERCISE_28
	code_6_exercise_28();
#endif

#ifdef CHAPTER_6_EXERCISE_29
	code_6_exercise_29();
#endif

#ifdef CHAPTER_6_EXERCISE_30
	code_6_exercise_30();
#endif

#ifdef CHAPTER_6_EXERCISE_31
	code_6_exercise_31();
#endif

#ifdef CHAPTER_6_EXERCISE_32
	code_6_exercise_32();
#endif

#ifdef CHAPTER_6_EXERCISE_33
	code_6_exercise_33();
#endif

#ifdef CHAPTER_6_EXERCISE_34
	code_6_exercise_34();
#endif

#ifdef CHAPTER_6_EXERCISE_35
	code_6_exercise_35();
#endif

#ifdef CHAPTER_6_EXERCISE_36
	code_6_exercise_36();
#endif

#ifdef CHAPTER_6_EXERCISE_37
	code_6_exercise_37();
#endif

#ifdef CHAPTER_6_EXERCISE_38
	code_6_exercise_38();
#endif

#ifdef CHAPTER_6_EXERCISE_39
	code_6_exercise_39();
#endif

#ifdef CHAPTER_6_EXERCISE_40
	code_6_exercise_40();
#endif

#ifdef CHAPTER_6_EXERCISE_41
	code_6_exercise_41();
#endif

#ifdef CHAPTER_6_EXERCISE_42
	code_6_exercise_42();
#endif
	
#ifdef CHAPTER_6_EXERCISE_43
	code_6_exercise_43();
#endif

#ifdef CHAPTER_6_EXERCISE_44
	code_6_exercise_44();
#endif

#ifdef CHAPTER_6_EXERCISE_45
	code_6_exercise_45();
#endif

#ifdef CHAPTER_6_EXERCISE_46
	code_6_exercise_46();
#endif

#ifdef CHAPTER_6_EXERCISE_47
	code_6_exercise_47();
#endif

#ifdef CHAPTER_6_EXERCISE_48
	code_6_exercise_48();
#endif

#ifdef CHAPTER_6_EXERCISE_49
	code_6_exercise_49();
#endif

#ifdef CHAPTER_6_EXERCISE_50
	code_6_exercise_50();
#endif

#ifdef CHAPTER_6_EXERCISE_51
	code_6_exercise_51();
#endif

#ifdef CHAPTER_6_EXERCISE_52
	code_6_exercise_52();
#endif

#ifdef CHAPTER_6_EXERCISE_53
	code_6_exercise_53();
#endif

#ifdef CHAPTER_6_EXERCISE_54
	code_6_exercise_54();
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test1
	code_6_exercise_54_1();
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test2
	code_6_exercise_54_2();
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test3
	code_6_exercise_54_3();
#endif


	glutMainLoop();
}
#endif
#endif 


