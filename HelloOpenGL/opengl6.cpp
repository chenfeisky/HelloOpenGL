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
		if (p > 0)
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
		if (p > 0)
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
		if (p > 0)
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
		if (p < 0)
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
		if (p > 0)
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
		if (p > 0)
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
//		if (p > 0)
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
		if (p > 0)
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
		if (p > 0)
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
		if (p > 0)
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
		if (p > 0)
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
		if (p > 0)
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
		if (p > 0)
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
		if (p < 0)
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
		if (p > 0)
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
		if (p > 0)
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


	glutMainLoop();
}
#endif
#endif 