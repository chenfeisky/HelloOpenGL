#include "common.h"

#if CHAPTER==7
#include "opengl7h.h"

#ifdef CHAPTER_7_COMMON
GLsizei winWidth = 800, winHeight = 600;
#endif

#ifdef CHAPTER_7_1_1
class wcPt2D
{
public:
	GLfloat x, y;
};
void translatePolygon(wcPt2D* verts, GLint nVerts, GLfloat tx, GLfloat ty)
{
	GLint k;
	for (k = 0; k < nVerts; k++)
	{
		verts[k].x = verts[k].x + tx;
		verts[k].y = verts[k].y + ty;
	}
	glBegin(GL_POLYGON);
	for (k = 0; k < nVerts; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	
	wcPt2D verts[] = { {120, 172}, {186, 104},{300, 150} ,{182, 224} };
	glBegin(GL_POLYGON);
	for (int k = 0; k < 4; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();

	translatePolygon(verts, 4, 400, 60);

	glFlush();
}
void code_7_1_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_7_1_2
class wcPt2D
{
public:
	GLfloat x, y;
};
void rotatePolygon(wcPt2D* verts, GLint nVerts, wcPt2D pivPt, GLdouble theta)
{
	wcPt2D* vertsRot = new wcPt2D[nVerts];
	GLint k;
	for (k = 0; k < nVerts; k++)
	{
		vertsRot[k].x = pivPt.x + (verts[k].x - pivPt.x) * std::cos(theta) - (verts[k].y - pivPt.y) * std::sin(theta);
		vertsRot[k].y = pivPt.y + (verts[k].x - pivPt.x) * std::sin(theta) + (verts[k].y - pivPt.y) * std::cos(theta);
	}
	glBegin(GL_POLYGON);
	for (k = 0; k < nVerts; k++)
		glVertex2f(vertsRot[k].x, vertsRot[k].y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D verts[] = { { 520, 232 },{ 586, 164 },{ 700, 210 } ,{ 582, 284 } };
	glBegin(GL_POLYGON);
	for (int k = 0; k < 4; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();

	rotatePolygon(verts, 4, {275, 242}, PI / 3);

	glFlush();
}
void code_7_1_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_7_1_3
class wcPt2D
{
public:
	GLfloat x, y;
};
void scalePolygon(wcPt2D* verts, GLint nVerts, wcPt2D fixedPt, GLfloat sx, GLfloat sy)
{
	wcPt2D* vertsNew = new wcPt2D[nVerts];
	GLint k;
	for (k = 0; k < nVerts; k++)
	{
		vertsNew[k].x = verts[k].x * sx + fixedPt.x * (1 - sx);
		vertsNew[k].y = verts[k].y * sy + fixedPt.y * (1 - sy);
	}
	glBegin(GL_POLYGON);
	for (k = 0; k < nVerts; k++)
		glVertex2f(vertsNew[k].x, vertsNew[k].y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D verts[] = { { 120, 172 },{ 186, 104 },{ 300, 150 } ,{ 182, 224 } };
	glBegin(GL_POLYGON);
	for (int k = 0; k < 4; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();

	scalePolygon(verts, 4, { 100, 100 }, 3, 3);

	glFlush();
}
void code_7_1_3()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_7_4_11
GLfloat xwcMin = 0.0, xwcMax = 225.0;
GLfloat ywcMin = 0.0, ywcMax = 225.0;
class wcPt2D
{
public:
	GLfloat x, y;
};
typedef GLfloat Matrix3x3[3][3];
Matrix3x3 matComposite;

void matrix3x3SetIdentity(Matrix3x3& matIdent3x3)
{
	GLint row, col;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matIdent3x3[row][col] = (row == col);
}
void matrix3x3PreMultiply(Matrix3x3& m1, Matrix3x3& m2)
{
	GLint row, col;
	Matrix3x3 matTemp;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col];
		}

	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			m2[row][col] = matTemp[row][col];
}
void translate2D(GLfloat tx, GLfloat ty)
{
	Matrix3x3 matTransl;
	matrix3x3SetIdentity(matTransl);
	matTransl[0][2] = tx;
	matTransl[1][2] = ty;
	matrix3x3PreMultiply(matTransl, matComposite);
}
void rotate2D(wcPt2D pivotPt, GLfloat theta)
{
	Matrix3x3 matRot;
	matrix3x3SetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	matrix3x3PreMultiply(matRot, matComposite);
}
void scale2D(GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
	Matrix3x3 matScale;
	matrix3x3SetIdentity(matScale);
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx) * fixedPt.x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy) * fixedPt.y;
	matrix3x3PreMultiply(matScale, matComposite);
}
void transformVerts2D(GLint nVerts, wcPt2D* verts)
{
	GLint k;
	GLfloat temp;
	for (k = 0; k < nVerts; k++)
	{
		temp = matComposite[0][0] * verts[k].x + matComposite[0][1] * verts[k].y + matComposite[0][2];
		verts[k].y = matComposite[1][0] * verts[k].x + matComposite[1][1] * verts[k].y + matComposite[1][2];
		verts[k].x = temp;
	}
}
void triangle(wcPt2D* verts)
{
	GLint k;
	glBegin(GL_TRIANGLES);
	for (k = 0; k < 3; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void displayFcn(void)
{
	GLint nVerts = 3;
	wcPt2D verts[3] = { {50.0, 25.0}, {150.0, 25.0}, {100.0, 100.0} };
	wcPt2D centroidPt;
	GLint k, xSum = 0, ySum = 0;
	for (k = 0; k < nVerts; k++)
	{
		xSum += verts[k].x;
		ySum += verts[k].y;
	}
	centroidPt.x = GLfloat(xSum) / GLfloat(nVerts);
	centroidPt.y = GLfloat(ySum) / GLfloat(nVerts);
	wcPt2D pivPt, fixedPt;
	pivPt = centroidPt;
	fixedPt = centroidPt;
	GLfloat tx = 0.0, ty = 100.0;
	GLfloat sx = 0.5, sy = 0.5;
	GLdouble theta = PI / 2.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	matrix3x3SetIdentity(matComposite);
	scale2D(sx, sy, fixedPt);
	rotate2D(pivPt, theta);
	translate2D(tx, ty);
	transformVerts2D(nVerts, verts);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts);
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);
	glClear(GL_COLOR_BUFFER_BIT);
}
void code_7_4_11()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
}
#endif

#ifdef CHAPTER_7_10
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	
	glColor3f(0.0, 0.0, 1.0);
	glRecti(50, 100, 200, 150);

	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-200.0, -50.0, 0.0);
	glRecti(50, 100, 200, 150);

	glLoadIdentity();
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glRecti(50, 100, 200, 150);

	glLoadIdentity();
	glScalef(-0.5, 1.0, 1.0);
	glRecti(50, 100, 200, 150);

	glFlush();
}
void code_7_10()
{
	glLoadIdentity();
	gluOrtho2D(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2);
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_1
struct Point { float x; float y; };
float angle = 5 * PI / 180;
int loopCount = 0;
std::vector<Point> originalPoints = { {150, 0},{ 200, 0 },{ 175, 100 } };
std::vector<Point> curPoints;
bool Equal(float f1, float f2) { return std::abs(f1 - f2) < 0.0001; }
bool Greater(float f1, float f2) { return Equal(f1, f2) ? false : (f1 > f2); }
bool GreaterQ(float f1, float f2) { return Greater(f1, f2) || Equal(f1, f2); }
float smallSin()
{
	return angle;
	//return std::sin(angle);
}
float smallCos()
{
	return 1.f;
	//return std::cos(angle);
}
void drawCoordinate()
{
	glBegin(GL_LINES);
		glVertex2i(-winWidth / 2, 0);
		glVertex2i(winWidth / 2, 0);
		glVertex2i(0, -winHeight / 2);
		glVertex2i(0, winHeight / 2);
	glEnd();
}
void triangle(const std::vector<Point>& points)
{
	glBegin(GL_TRIANGLES);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	triangle(curPoints);
	glFlush();
}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//printf("onTimer: %d\n", dwTime);
	loopCount++;
	if (GreaterQ(loopCount, 2 * PI / angle))
	{
		curPoints = originalPoints;
		loopCount = 0;
	}
	else
	{
		float tempX = 0.f, tempY = 0.f;
		for (auto & p : curPoints)
		{
			tempX = p.x * smallCos() - p.y * smallSin();
			tempY = p.x * smallSin() + p.y * smallCos();
			p.x = tempX;
			p.y = tempY;
		}
	}

	displayFcn();
}
void code_7_exercise_1()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2);

	SetTimer(NULL, NULL, 100, onTimer);

	curPoints = originalPoints;

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_2
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void triangle(const std::vector<Point>& points)
{
	glBegin(GL_TRIANGLES);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
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
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::vector<Point> originalPoints = { { 150, 0 },{ 200, 0 },{ 175, 100 } };

	glColor3f(1.0, 1.0, 1.0);
	
	// 连续变换两次 
	std::vector<Point> curPoints = originalPoints;
	auto m1 = rotateMatrix({ 0, 0 }, 10 * PI / 180);
	auto m2 = rotateMatrix({ 0, 0 }, 20 * PI / 180);
	transformPoints(m1, curPoints);
	transformPoints(m2, curPoints);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50.0, 300.0, 0.0);
	triangle(curPoints);

	// 复合矩阵
	curPoints = originalPoints;
	auto m = m2 * m1;
	transformPoints(m, curPoints);
	glLoadIdentity();
	glTranslatef(250.0, 300.0, 0.0);
	triangle(curPoints);

	// 变换一次(两角之和)
	curPoints = originalPoints;
	transformPoints(rotateMatrix({ 0, 0 }, 30 * PI / 180), curPoints);
	glLoadIdentity();
	glTranslatef(450.0, 300.0, 0.0);
	triangle(curPoints);

	glColor3f(1.0, 0.0, 0.0);
	// 连续变换两次 
	curPoints = originalPoints;
	m1 = rotateMatrix({ 50, 50 }, 10 * PI / 180);
	m2 = rotateMatrix({ 50, 50 }, 20 * PI / 180);
	transformPoints(m1, curPoints);
	transformPoints(m2, curPoints);
	glLoadIdentity();
	glTranslatef(50.0, 100.0, 0.0);
	triangle(curPoints);

	// 复合矩阵
	curPoints = originalPoints;
	m = m2 * m1;
	transformPoints(m, curPoints);
	glLoadIdentity();
	glTranslatef(250.0, 100.0, 0.0);
	triangle(curPoints);
	
	// 变换一次(两角之和)
	curPoints = originalPoints;
	transformPoints(rotateMatrix({ 50, 50 }, 30 * PI / 180), curPoints);
	glLoadIdentity();
	glTranslatef(450.0, 100.0, 0.0);
	triangle(curPoints);

	glFlush();
}
void code_7_exercise_2()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_3
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void square(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix dirScaleMatrix(float s1, float s2, float theta)
{
	// 基于原点的定向旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = s1 * std::pow(cos(theta), 2) + s2 * std::pow(sin(theta), 2);
	ret[0][1] = (s2 - s1) * cos(theta) * sin(theta);
	ret[1][0] = (s2 - s1) * cos(theta) * sin(theta);
	ret[1][1] = s1 * std::pow(sin(theta), 2) + s2 * std::pow(cos(theta), 2);
	return ret;
}
Matrix dirScaleByPointMatrix(float tx, float ty, float s1, float s2, float theta)
{
	// 基于指定点的定向旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = s1 * std::pow(cos(theta), 2) + s2 * std::pow(sin(theta), 2);
	ret[0][1] = (s2 - s1) * cos(theta) * sin(theta);
	ret[0][2] = -tx * (s1 * std::pow(cos(theta), 2) + s2 * std::pow(sin(theta), 2)) 
		-ty * (s2 - s1) * cos(theta) * sin(theta)
		+ tx;
	ret[1][0] = (s2 - s1) * cos(theta) * sin(theta);
	ret[1][1] = s1 * std::pow(sin(theta), 2) + s2 * std::pow(cos(theta), 2);
	ret[1][2] = -tx * (s2 - s1) * cos(theta) * sin(theta)
		- ty * (s1 * std::pow(sin(theta), 2) + s2 * std::pow(cos(theta), 2))
		+ ty;
	return ret;
}
Matrix translateMatrix(float tx, float ty)
{
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
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
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::vector<Point> originalPoints = { { 0, 0 },{ 100, 0 },{ 100, 100 },{ 0, 100 } };
	std::vector<Point> curPoints;

	glColor3f(1.0, 1.0, 1.0);

	// 原点的定向缩放
	curPoints = originalPoints;
	transformPoints(dirScaleMatrix(1, 2, 45 * PI / 180), curPoints);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(150.0, 350.0, 0.0);
	drawCoordinate();
	square(curPoints);

	glColor3f(1.0, 0.0, 0.0);

	// 由一系列变换而成的 指定点定向缩放
	curPoints = originalPoints;
	transformPoints(translateMatrix(-50, -50), curPoints);
	transformPoints(dirScaleMatrix(1, 2, 45 * PI / 180), curPoints);
	transformPoints(translateMatrix(50, 50), curPoints);
	glLoadIdentity();
	glTranslatef(550.0, 350.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 由一系列变换生成的复合 指定点定向缩放
	curPoints = originalPoints;
	auto compound = translateMatrix(50, 50) * dirScaleMatrix(1, 2, 45 * PI / 180) * translateMatrix(-50, -50);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(150.0, 150.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 计算所得的复合 指定点定向缩放
	curPoints = originalPoints;
	transformPoints(dirScaleByPointMatrix(50, 50, 1, 2, 45 * PI / 180), curPoints);
	glLoadIdentity();
	glTranslatef(550.0, 150.0, 0.0);
	drawCoordinate();
	square(curPoints);
	
	glFlush();
}
void code_7_exercise_3()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_4
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void square(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix translateMatrix(float tx, float ty)
{
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix rotateByPointMatrix(Point p, float theta)
{
	// 基于指定点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[0][2] = p.x * (1 - std::cos(theta)) + p.y * std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	ret[1][2] = p.y * (1 - std::cos(theta)) - p.x * std::sin(theta);
	return ret;
}
Matrix scaleMatrix(float sx, float sy)
{
	// 基于原点缩放
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[1][1] = sy;
	return ret;
}
Matrix scaleByPointMatrix(Point p, float sx, float sy)
{
	// 基于指定点缩放
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
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 0, 0 },{ 100, 0 },{ 100, 100 },{ 0, 100 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);
	
	// 平移
	// 复合平移
	curPoints = originalPoints;
	compound = translateMatrix(0, 30) * translateMatrix(30, 0);
	transformPoints(compound, curPoints);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(250.0, 400.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 复合平移，交换次序
	curPoints = originalPoints;
	compound = translateMatrix(30, 0) * translateMatrix(0, 30);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(450.0, 400.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 旋转
	// 基于原点复合旋转
	curPoints = originalPoints;
	compound = rotateMatrix(20 * PI / 180) * rotateMatrix(10 * PI / 180);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(50.0, 250.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 基于原点复合旋转, 交换次序 
	curPoints = originalPoints;
	compound = rotateMatrix(10 * PI / 180) * rotateMatrix(20 * PI / 180);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(250.0, 250.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 基于指定点复合旋转
	curPoints = originalPoints;
	compound = rotateByPointMatrix({50, 50}, 20 * PI / 180) * rotateByPointMatrix({ 50, 50 }, 10 * PI / 180);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(450.0, 250.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 基于指定点复合旋转, 交换次序 
	curPoints = originalPoints;
	compound = rotateByPointMatrix({ 50, 50 }, 10 * PI / 180) * rotateByPointMatrix({ 50, 50 }, 20 * PI / 180);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(650.0, 250.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 缩放
	// 基于原点复合缩放
	curPoints = originalPoints;
	compound = scaleMatrix(0.8, 0.4) * scaleMatrix(0.5, 0.5);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(50.0, 100.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 基于原点复合缩放, 交换次序 
	curPoints = originalPoints;
	compound = scaleMatrix(0.5, 0.5) * scaleMatrix(0.8, 0.4);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(250.0, 100.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 基于指定点复合缩放
	curPoints = originalPoints;
	compound = scaleByPointMatrix({ 50, 50 }, 0.8, 0.4) * scaleByPointMatrix({ 50, 50 }, 0.5, 0.5);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(450.0, 100.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 基于指定点复合缩放, 交换次序 
	curPoints = originalPoints;
	compound = scaleByPointMatrix({ 50, 50 }, 0.5, 0.5) * scaleByPointMatrix({ 50, 50 }, 0.8, 0.4);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(650.0, 100.0, 0.0);
	drawCoordinate();
	square(curPoints);
	
	glFlush();
}

void code_7_exercise_4()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_5
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void square(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix rotateByPointMatrix(Point p, float theta)
{
	// 基于指定点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[0][2] = p.x * (1 - std::cos(theta)) + p.y * std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	ret[1][2] = p.y * (1 - std::cos(theta)) - p.x * std::sin(theta);
	return ret;
}
Matrix scaleMatrix(float sx, float sy)
{
	// 基于原点缩放
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[1][1] = sy;
	return ret;
}
Matrix scaleByPointMatrix(Point p, float sx, float sy)
{
	// 基于指定点缩放
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
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 0, 0 },{ 100, 0 },{ 100, 100 },{ 0, 100 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);

	// 基于原点
	// 一致缩放+旋转
	curPoints = originalPoints;
	compound = rotateMatrix(30 * PI / 180) * scaleMatrix(0.5, 0.5);
	transformPoints(compound, curPoints);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50.0, 400.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 旋转+一致缩放
	curPoints = originalPoints;
	compound = scaleMatrix(0.5, 0.5) * rotateMatrix(30 * PI / 180);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(250.0, 400.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 非一致缩放+旋转
	curPoints = originalPoints;
	compound = rotateMatrix(30 * PI / 180) * scaleMatrix(1, 0.5);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(450.0, 400.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 旋转+非一致缩放
	curPoints = originalPoints;
	compound = scaleMatrix(1, 0.5) * rotateMatrix(30 * PI / 180);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(650.0, 400.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 基于指定点
	// 一致缩放+旋转
	curPoints = originalPoints;
	compound = rotateByPointMatrix({ 50, 50 }, 30 * PI / 180) * scaleByPointMatrix({50, 50}, 0.5, 0.5);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(50.0, 150.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 旋转+一致缩放
	curPoints = originalPoints;
	compound = scaleByPointMatrix({ 50, 50 }, 0.5, 0.5) * rotateByPointMatrix({ 50, 50 }, 30 * PI / 180);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(250.0, 150.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 非一致缩放+旋转
	curPoints = originalPoints;
	compound = rotateByPointMatrix({ 50, 50 }, 30 * PI / 180) * scaleByPointMatrix({ 50, 50 }, 1, 0.5);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(450.0, 150.0, 0.0);
	drawCoordinate();
	square(curPoints);

	// 旋转+非一致缩放
	curPoints = originalPoints;
	compound = scaleByPointMatrix({ 50, 50 }, 1, 0.5) * rotateByPointMatrix({ 50, 50 }, 30 * PI / 180);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(650.0, 150.0, 0.0);
	drawCoordinate();
	square(curPoints);

	
	
	glFlush();
}

void code_7_exercise_5()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_6
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void square(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix translateMatrix(float tx, float ty)
{
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateByPointMatrix(Point p, float theta)
{
	// 基于指定点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[0][2] = p.x * (1 - std::cos(theta)) + p.y * std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	ret[1][2] = p.y * (1 - std::cos(theta)) - p.x * std::sin(theta);
	return ret;
}
Matrix scaleByPointMatrix(Point p, float sx, float sy)
{
	// 基于指定点缩放
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[0][2] = p.x * (1 - sx);
	ret[1][1] = sy;
	ret[1][2] = p.y * (1 - sy);
	return ret;
}
Matrix TRSMatrix(Point p, float tx, float ty, float theta, float sx, float sy)
{
	// 平移，旋转，缩放复合矩阵
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx * std::cos(theta);
	ret[0][1] = -sy * std::sin(theta);
	ret[0][2] = p.x * (1 - sx * std::cos(theta)) + p.y * sy * std::sin(theta) + tx;
	ret[1][0] = sx * std::sin(theta);
	ret[1][1] = sy * std::cos(theta);
	ret[1][2] = p.y * (1 - sy * std::cos(theta)) - p.x * sx * std::sin(theta) + ty;
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
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 0, 0 },{ 100, 0 },{ 100, 100 },{ 0, 100 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);

	curPoints = originalPoints;
	compound = translateMatrix(50, 100) * rotateByPointMatrix({ 50, 50 }, 45 * PI / 180) * scaleByPointMatrix({ 50, 50 }, 0.5, 0.5);
	transformPoints(compound, curPoints);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50.0, 250, 0.0);
	drawCoordinate();
	square(curPoints);

	curPoints = originalPoints;
	compound = TRSMatrix({ 50, 50 }, 50, 100, 45 * PI / 180, 0.5, 0.5);
	transformPoints(compound, curPoints);
	glLoadIdentity();
	glTranslatef(450.0, 250, 0.0);
	drawCoordinate();
	square(curPoints);

	glFlush();
}

void code_7_exercise_6()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_7
// 改为1:1等比例投影，方便查看变换效果，否则将产生拉伸形变
GLfloat xwcMin = 0.0, xwcMax = 225.0;
GLfloat ywcMin = 0.0, ywcMax = 225.0;
float sx = 0.f, sy = 0.f, theta = 0.f, tx = 0.f, ty = 0.f;
class wcPt2D
{
public:
	GLfloat x, y;
};
typedef GLfloat Matrix3x3[3][3];
Matrix3x3 matComposite;
void inputParam()
{
	printf("input scale parameter (sx,sy):");
	scanf_s("%f,%f", &sx, &sy);
	printf("input rotate parameter (theta):");
	scanf_s("%f", &theta);
	theta = theta * PI / 180;
	printf("input translate parameter (tx,ty):");
	scanf_s("%f,%f", &tx, &ty);
}
void matrix3x3SetIdentity(Matrix3x3& matIdent3x3)
{
	GLint row, col;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matIdent3x3[row][col] = (row == col);
}
void matrix3x3PreMultiply(Matrix3x3& m1, Matrix3x3& m2)
{
	GLint row, col;
	Matrix3x3 matTemp;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col];
		}

	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			m2[row][col] = matTemp[row][col];
}
void translate2D(GLfloat tx, GLfloat ty)
{
	Matrix3x3 matTransl;
	matrix3x3SetIdentity(matTransl);
	matTransl[0][2] = tx;
	matTransl[1][2] = ty;
	matrix3x3PreMultiply(matTransl, matComposite);
}
void rotate2D(wcPt2D pivotPt, GLfloat theta)
{
	Matrix3x3 matRot;
	matrix3x3SetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	matrix3x3PreMultiply(matRot, matComposite);
}
void scale2D(GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
	Matrix3x3 matScale;
	matrix3x3SetIdentity(matScale);
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx) * fixedPt.x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy) * fixedPt.y;
	matrix3x3PreMultiply(matScale, matComposite);
}
void transformVerts2D(GLint nVerts, wcPt2D* verts)
{
	GLint k;
	GLfloat temp;
	for (k = 0; k < nVerts; k++)
	{
		temp = matComposite[0][0] * verts[k].x + matComposite[0][1] * verts[k].y + matComposite[0][2];
		verts[k].y = matComposite[1][0] * verts[k].x + matComposite[1][1] * verts[k].y + matComposite[1][2];
		verts[k].x = temp;
	}
}
void triangle(wcPt2D* verts)
{
	GLint k;
	glBegin(GL_TRIANGLES);
	for (k = 0; k < 3; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void displayFcn(void)
{
	GLint nVerts = 3;
	wcPt2D verts[3] = { { 50.0, 25.0 },{ 150.0, 25.0 },{ 100.0, 100.0 } };
	wcPt2D centroidPt;
	GLint k, xSum = 0, ySum = 0;
	for (k = 0; k < nVerts; k++)
	{
		xSum += verts[k].x;
		ySum += verts[k].y;
	}
	centroidPt.x = GLfloat(xSum) / GLfloat(nVerts);
	centroidPt.y = GLfloat(ySum) / GLfloat(nVerts);
	wcPt2D pivPt, fixedPt;
	pivPt = centroidPt;
	fixedPt = centroidPt;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	matrix3x3SetIdentity(matComposite);
	scale2D(sx, sy, fixedPt);
	rotate2D(pivPt, theta);
	translate2D(tx, ty);
	transformVerts2D(nVerts, verts);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts);
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glClear(GL_COLOR_BUFFER_BIT);
}
void code_7_exercise_7()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);

	inputParam();
}
#endif

#ifdef CHAPTER_7_EXERCISE_8
// 改为1:1等比例投影，方便查看变换效果，否则将产生拉伸形变
GLfloat xwcMin = 0.0, xwcMax = 225.0;
GLfloat ywcMin = 0.0, ywcMax = 225.0;
class wcPt2D
{
public:
	GLfloat x, y;
};
typedef GLfloat Matrix3x3[3][3];
std::vector<wcPt2D> points;
Matrix3x3 matComposite;
void inputParam()
{
	std::string inputStr;
	while(1)
	{
		printf("input polygon points (x,y), input \"exit\" to end:");
		std::cin >> inputStr;
		if (inputStr == "exit")
			break;
		else
		{
			points.push_back({});
			sscanf_s(inputStr.c_str(), "%f, %f", &points.back().x, &points.back().y);
		}
	}
}
void matrix3x3SetIdentity(Matrix3x3& matIdent3x3)
{
	GLint row, col;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matIdent3x3[row][col] = (row == col);
}
void matrix3x3PreMultiply(Matrix3x3& m1, Matrix3x3& m2)
{
	GLint row, col;
	Matrix3x3 matTemp;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col];
		}

	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			m2[row][col] = matTemp[row][col];
}
void translate2D(GLfloat tx, GLfloat ty)
{
	Matrix3x3 matTransl;
	matrix3x3SetIdentity(matTransl);
	matTransl[0][2] = tx;
	matTransl[1][2] = ty;
	matrix3x3PreMultiply(matTransl, matComposite);
}
void rotate2D(wcPt2D pivotPt, GLfloat theta)
{
	Matrix3x3 matRot;
	matrix3x3SetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	matrix3x3PreMultiply(matRot, matComposite);
}
void scale2D(GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
	Matrix3x3 matScale;
	matrix3x3SetIdentity(matScale);
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx) * fixedPt.x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy) * fixedPt.y;
	matrix3x3PreMultiply(matScale, matComposite);
}
void transformVerts2D(std::vector<wcPt2D>& points)
{
	GLfloat temp;
	for (auto& p : points)
	{
		temp = matComposite[0][0] * p.x + matComposite[0][1] * p.y + matComposite[0][2];
		p.y = matComposite[1][0] * p.x + matComposite[1][1] * p.y + matComposite[1][2];
		p.x = temp;
	}
}
void polygon(const std::vector<wcPt2D>& points)
{
	glBegin(GL_POLYGON);
	for (auto& p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void displayFcn(void)
{
	wcPt2D centroidPt;
	GLint xSum = 0, ySum = 0;
	for (auto& p : points)
	{
		xSum += p.x;
		ySum += p.y;
	}
	centroidPt.x = GLfloat(xSum) / GLfloat(points.size());
	centroidPt.y = GLfloat(ySum) / GLfloat(points.size());
	wcPt2D pivPt, fixedPt;
	pivPt = centroidPt;
	fixedPt = centroidPt;
	GLfloat tx = 0.0, ty = 100.0;
	GLfloat sx = 0.5, sy = 0.5;
	GLdouble theta = PI / 2.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	polygon(points);
	matrix3x3SetIdentity(matComposite);
	scale2D(sx, sy, fixedPt);
	rotate2D(pivPt, theta);
	translate2D(tx, ty);
	transformVerts2D(points);
	glColor3f(1.0, 0.0, 0.0);
	polygon(points);
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glClear(GL_COLOR_BUFFER_BIT);
}
void code_7_exercise_8()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);

	inputParam();
}
#endif

#ifdef CHAPTER_7_EXERCISE_9
// 改为1:1等比例投影，方便查看变换效果，否则将产生拉伸形变
GLfloat xwcMin = 0.0, xwcMax = 225.0;
GLfloat ywcMin = 0.0, ywcMax = 225.0;
class wcPt2D
{
public:
	GLfloat x, y;
};
typedef GLfloat Matrix3x3[3][3];
std::string inputStr;
Matrix3x3 matComposite;
void inputParam()
{
	printf("input transform [(T)ranslate, (R)otate, (S)cale]:");
	std::cin >> inputStr;
}
void matrix3x3SetIdentity(Matrix3x3& matIdent3x3)
{
	GLint row, col;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matIdent3x3[row][col] = (row == col);
}
void matrix3x3PreMultiply(Matrix3x3& m1, Matrix3x3& m2)
{
	GLint row, col;
	Matrix3x3 matTemp;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col];
		}

	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			m2[row][col] = matTemp[row][col];
}
void translate2D(GLfloat tx, GLfloat ty)
{
	Matrix3x3 matTransl;
	matrix3x3SetIdentity(matTransl);
	matTransl[0][2] = tx;
	matTransl[1][2] = ty;
	matrix3x3PreMultiply(matTransl, matComposite);
}
void rotate2D(wcPt2D pivotPt, GLfloat theta)
{
	Matrix3x3 matRot;
	matrix3x3SetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	matrix3x3PreMultiply(matRot, matComposite);
}
void scale2D(GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
	Matrix3x3 matScale;
	matrix3x3SetIdentity(matScale);
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx) * fixedPt.x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy) * fixedPt.y;
	matrix3x3PreMultiply(matScale, matComposite);
}
void transformVerts2D(GLint nVerts, wcPt2D* verts)
{
	GLint k;
	GLfloat temp;
	for (k = 0; k < nVerts; k++)
	{
		temp = matComposite[0][0] * verts[k].x + matComposite[0][1] * verts[k].y + matComposite[0][2];
		verts[k].y = matComposite[1][0] * verts[k].x + matComposite[1][1] * verts[k].y + matComposite[1][2];
		verts[k].x = temp;
	}
}
void triangle(wcPt2D* verts)
{
	GLint k;
	glBegin(GL_TRIANGLES);
	for (k = 0; k < 3; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void displayFcn(void)
{
	GLint nVerts = 3;
	wcPt2D verts[3] = { { 50.0, 25.0 },{ 150.0, 25.0 },{ 100.0, 100.0 } };
	wcPt2D centroidPt;
	GLint k, xSum = 0, ySum = 0;
	for (k = 0; k < nVerts; k++)
	{
		xSum += verts[k].x;
		ySum += verts[k].y;
	}
	centroidPt.x = GLfloat(xSum) / GLfloat(nVerts);
	centroidPt.y = GLfloat(ySum) / GLfloat(nVerts);
	wcPt2D pivPt, fixedPt;
	pivPt = centroidPt;
	fixedPt = centroidPt;
	GLfloat tx = 0.0, ty = 100.0;
	GLfloat sx = 0.5, sy = 0.5;
	GLdouble theta = PI / 2.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	matrix3x3SetIdentity(matComposite);
	for (auto& c : inputStr)
	{
		if (c == 'T')
			translate2D(tx, ty);
		else if (c == 'R')
			rotate2D(pivPt, theta);
		else if (c == 'S')
			scale2D(sx, sy, fixedPt);
		else
			printf("invalid transform parameter : %c\n", c);
	}	
	transformVerts2D(nVerts, verts);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts);
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glClear(GL_COLOR_BUFFER_BIT);
}
void code_7_exercise_9()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);

	inputParam();
}
#endif

#ifdef CHAPTER_7_EXERCISE_10
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void polygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix reflectionXMatrix()
{
	// 基于x轴反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[1][1] = -1;
	return ret;
}
Matrix reflectionYXMatrix()
{
	// 基于y=x反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = 0;
	ret[0][1] = 1;
	ret[1][0] = 1;
	ret[1][1] = 0;
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
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glVertex2i(0, 0);
	glVertex2i(winWidth / 2, winWidth / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 140, 95 },{ 164, 22 },{ 216, 77 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);

	// 直接给出反射矩阵
	curPoints = originalPoints;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50.0, 250, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	transformPoints(reflectionYXMatrix(), curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 顺时针45 - x轴反射 - 逆时针45
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(300, 250, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = rotateMatrix(45 * PI / 180) * reflectionXMatrix() * rotateMatrix(-45 * PI / 180);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// x轴反射 - 逆时针90
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(550, 250, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = rotateMatrix(90 * PI / 180) * reflectionXMatrix();
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	glFlush();
}

void code_7_exercise_10()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_11
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void polygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix reflectionYMatrix()
{
	// 基于y轴反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = -1;
	return ret;
}
Matrix reflectionY_XMatrix()
{
	// 基于y=-x反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = 0;
	ret[0][1] = -1;
	ret[1][0] = -1;
	ret[1][1] = 0;
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
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glVertex2i(0, 0);
	glVertex2i(-winWidth / 2, winWidth / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { -75, 20 },{ -100, 80 },{ -150, 20 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);

	// 直接给出反射矩阵
	curPoints = originalPoints;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(250.0, 250, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	transformPoints(reflectionY_XMatrix(), curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 顺时针45 - y轴反射 - 逆时针45
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(500, 250, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = rotateMatrix(45 * PI / 180) * reflectionYMatrix() * rotateMatrix(-45 * PI / 180);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// y轴反射 - 逆时针90
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(750, 250, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = rotateMatrix(90 * PI / 180) * reflectionYMatrix();
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	glFlush();
}

void code_7_exercise_11()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_12
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void polygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix reflectionXMatrix()
{
	// 基于x轴反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[1][1] = -1;
	return ret;
}
Matrix reflectionYMatrix()
{
	// 基于y轴反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = -1;
	return ret;
}
Matrix reflectionOMatrix()
{
	// 基于原点反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = -1;
	ret[1][1] = -1;
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
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 30, 30 },{ 107, 64 },{ 70, 97 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);

	// 直接给出反射矩阵
	curPoints = originalPoints;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(200, 400, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	transformPoints(reflectionOMatrix(), curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// x轴反射 - y轴反射
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(550, 400, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = reflectionYMatrix() * reflectionXMatrix();
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// y轴反射 - x轴反射
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(200, 150, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = reflectionXMatrix() * reflectionYMatrix();
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 基于原点旋转180
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(550, 150, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	transformPoints(rotateMatrix(PI), curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	glFlush();
}

void code_7_exercise_12()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_13
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void polygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix translateMatrix(float tx, float ty)
{
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix reflectionXMatrix()
{
	// 基于x轴反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[1][1] = -1;
	return ret;
}
Matrix reflectionLineMatrix(float m, float b)
{
	// 基于y=mx+b直线的反射
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	float theta = std::atan(m);
	ret[0][0] = std::cos(2 * theta);
	ret[0][1] = std::sin(2 * theta);
	ret[0][2] = -b * std::sin(2 * theta);
	ret[1][0] = std::sin(2 * theta);
	ret[1][1] = -std::cos(2 * theta);
	ret[1][2] = b * (std::cos(2 * theta) + 1);
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
void drawCoordinate(float m, float b)
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glVertex2i(-100, - m * 100 + b);
	glVertex2i(210, m * 210 + b);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 127, 21 },{ 167, 43 },{ 116, 89 }};
	std::vector<Point> curPoints;
	Matrix compound(3, 3);

	float theta = 30 * PI / 180;
	float m = std::tan(theta);
	float b = 50;

	// 直接给出反射矩阵
	curPoints = originalPoints;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(100, 300, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(m, b);
	polygon(curPoints);
	transformPoints(reflectionLineMatrix(m, b), curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 平移-b, 旋转-theta, x轴反射, 旋转theta, 平移b
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(350, 300, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(m, b);
	polygon(curPoints);
	compound = translateMatrix(0, b) * rotateMatrix(theta) * reflectionXMatrix() * rotateMatrix(-theta) * translateMatrix(0, -b);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 平移-b, x轴反射, 旋转2theta, 平移b
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(600, 300, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(m, b);
	polygon(curPoints);
	compound = translateMatrix(0, b) * rotateMatrix(2 * theta) * reflectionXMatrix() * translateMatrix(0, -b);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	glFlush();
}

void code_7_exercise_13()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_14
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void polygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix translateMatrix(float tx, float ty)
{
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix reflectionOLineMatrix(float m)
{
	// 基于y=mx直线反射
	float theta = std::atan(m);
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(2 * theta);
	ret[0][1] = std::sin(2 * theta);
	ret[1][0] = std::sin(2 * theta);
	ret[1][1] = -std::cos(2 * theta);
	return ret;
}
Matrix reflection2LineMatrix(float m1, float m2)
{
	// 基于两条过原点直线的连续反射,先m1,然后m2
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	float theta = 2 * (std::atan(m2) - std::atan(m1));
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
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
void drawCoordinate(float m1, float m2)
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glVertex2i(0, 0);
	glVertex2i(200, m1 * 200);
	glVertex2i(0, 0);
	glVertex2i(200, m2 * 200);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 100, 49 },{ 91, 28 },{ 124, 40 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);

	float theta1 = 30 * PI / 180;
	float m1 = std::tan(theta1);
	float theta2 = 45 * PI / 180;
	float m2 = std::tan(theta2);

	// 直接给出反射矩阵
	curPoints = originalPoints;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(100, 300, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(m1, m2);
	polygon(curPoints);
	transformPoints(reflection2LineMatrix(m1, m2), curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 连续两次反射
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(350, 300, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(m1, m2);
	polygon(curPoints);
	transformPoints(reflectionOLineMatrix(m1), curPoints);
	glColor3f(0.0, 0.0, 1.0);
	polygon(curPoints);
	transformPoints(reflectionOLineMatrix(m2), curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 一次旋转
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(600, 300, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(m1, m2);
	polygon(curPoints);
	transformPoints(rotateMatrix(2 * (std::atan(m2) - std::atan(m1))), curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	glFlush();
}

void code_7_exercise_14()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_15
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void polygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix translateMatrix(float tx, float ty)
{
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix scaleMatrix(float sx, float sy)
{
	// 基于原点缩放
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[1][1] = sy;
	return ret;
}
Matrix shearXMatrix(float shx)
{
	// 基于原点的x方向错切
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][1] = shx;
	return ret;
}
Matrix shearXByParallelXLineMatrix(float shx, float y_ref)
{
	// 基于平行于x轴直线（即y=y_ref）的x方向错切
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][1] = shx;
	ret[0][2] = -shx * y_ref;
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
		temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void drawPoints(const std::vector<Point>& points)
{
	glBegin(GL_POINTS);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 0, 0 },{ 100, 0 },{ 100, 100 },{ 0, 100 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);
	
	// 基于原点的错切
	float shx = 2;

	// 直接给出错切矩阵
	curPoints = originalPoints;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(100, 400, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = shearXMatrix(shx);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);
	
	// 一系列基本变换:旋转theta,缩放(sx,sy),旋转-theta2,缩放(1 / (cos1 * cos2 * sx + sin1 * sin2 * sy), 1)
	float theta = 15 * PI / 180; // 由用户指定
	float cos1 = cos(theta);
	float sin1 = sin(theta);
	float tan1 = tan(theta);

	assert(tan1 <= 1 / shx && "tan theta must less than 1 / shx");
	
	float cos2 = sqrt(cos1 * cos1 - shx * cos1 * sin1);
	float sin2 = sqrt(1 - cos2 * cos2);

	float sx = sin1 / sin2;
	float sy = cos1 / cos2;

	float theta2 = acos(cos2);

	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(450, 400, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = scaleMatrix(1 / (cos1 * cos2 * sx + sin1 * sin2 * sy), 1) * rotateMatrix(-theta2) * scaleMatrix(sx, sy) * rotateMatrix(theta);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 基于平行于x轴参考线的错切
	shx = 0.5;
	float y_ref = -100;

	// 直接给出错切矩阵
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(100, 150, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = shearXByParallelXLineMatrix(shx, y_ref);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 一系列基本变换:平移-y_ref,旋转theta,缩放(sx,sy),旋转-theta2,缩放(1 / (cos1 * cos2 * sx + sin1 * sin2 * sy), 1),平移y_ref
	theta = 45 * PI / 180; // 由用户指定
	cos1 = cos(theta);
	sin1 = sin(theta);
	tan1 = tan(theta);

	assert(tan1 <= 1 / shx && "tan theta must less than 1 / shx");

	cos2 = sqrt(cos1 * cos1 - shx * cos1 * sin1);
	sin2 = sqrt(1 - cos2 * cos2);

	sx = sin1 / sin2;
	sy = cos1 / cos2;

	theta2 = acos(cos2);

	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(450, 150, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = translateMatrix(0, y_ref) * scaleMatrix(1 / (cos1 * cos2 * sx + sin1 * sin2 * sy), 1) * rotateMatrix(-theta2) * scaleMatrix(sx, sy) * rotateMatrix(theta) * translateMatrix(0, -y_ref);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	glFlush();
}

void code_7_exercise_15()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_16
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
void polygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
Matrix translateMatrix(float tx, float ty)
{
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix scaleMatrix(float sx, float sy)
{
	// 基于原点缩放
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[1][1] = sy;
	return ret;
}
Matrix shearYMatrix(float shy)
{
	// 基于原点的y方向错切
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[1][0] = shy;
	return ret;
}
Matrix shearYByParallelYLineMatrix(float shy, float x_ref)
{
	// 基于平行于y轴直线（即x=x_ref）的y方向错切
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[1][0] = shy;
	ret[1][2] = -shy * x_ref;
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
		temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> originalPoints = { { 0, 0 },{ 100, 0 },{ 100, 100 },{ 0, 100 } };
	std::vector<Point> curPoints;
	Matrix compound(3, 3);

	// 基于原点的错切
	float shy = 2;

	// 直接给出错切矩阵
	curPoints = originalPoints;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50, 200, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = shearYMatrix(shy);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 一系列基本变换:旋转-theta,缩放(sx,sy),旋转theta2,缩放(1, 1 / (sin1 * sin2 * sx + cos1 * cos2 * sy))
	float theta = 15 * PI / 180; // 由用户指定
	float cos1 = cos(theta);
	float sin1 = sin(theta);
	float tan1 = tan(theta);
	
	assert(tan1 <= 1 / shy && "tan theta must less than 1 / shy");
	
	float cos2 = sqrt(cos1 * cos1 - shy * cos1 * sin1);
	float sin2 = sqrt(1 - cos2 * cos2);
	
	float sx = cos1 / cos2;
	float sy = sin1 / sin2;
	
	float theta2 = acos(cos2);
	
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(250, 200, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = scaleMatrix(1, 1 / (sin1 * sin2 * sx + cos1 * cos2 * sy)) * rotateMatrix(theta2) * scaleMatrix(sx, sy) * rotateMatrix(-theta);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 基于平行于y轴参考线的错切
	shy = 0.5;
	float x_ref = -100;

	// 直接给出错切矩阵
	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(450, 200, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = shearYByParallelYLineMatrix(shy, x_ref);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	// 一系列基本变换:平移-x_ref,旋转-theta,缩放(sx,sy),旋转theta2,缩放(1, 1 / (sin1 * sin2 * sx + cos1 * cos2 * sy)),平移x_ref
	theta = 45 * PI / 180; // 由用户指定
	cos1 = cos(theta);
	sin1 = sin(theta);
	tan1 = tan(theta);

	assert(tan1 <= 1 / shy && "tan theta must less than 1 / shy");

	cos2 = sqrt(cos1 * cos1 - shy * cos1 * sin1);
	sin2 = sqrt(1 - cos2 * cos2);

	sx = cos1 / cos2;
	sy = sin1 / sin2;

	theta2 = acos(cos2);

	curPoints = originalPoints;
	glLoadIdentity();
	glTranslatef(650, 200, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate();
	polygon(curPoints);
	compound = translateMatrix(x_ref, 0) * scaleMatrix(1, 1 / (sin1 * sin2 * sx + cos1 * cos2 * sy)) * rotateMatrix(theta2) * scaleMatrix(sx, sy) * rotateMatrix(-theta) * translateMatrix(-x_ref, 0);
	transformPoints(compound, curPoints);
	glColor3f(1.0, 0.0, 0.0);
	polygon(curPoints);

	glFlush();
}

void code_7_exercise_16()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_17
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
Matrix shearXMatrix(float shx)
{
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][1] = shx;
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
		temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void drawChar(const std::vector<Point>& points)
{
	glBegin(GL_LINES);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> X = { { 5, 5 },{ 70, 100 },{ 5, 100 },{ 70, 5 } };
	std::vector<Point> Y = { { 5, 100 },{ 37, 50 },{ 70, 100 },{ 37, 50 },{ 37, 50 },{ 37, 5 } };
	std::vector<Point> Z = { { 5, 100 },{ 70, 100 },{ 70, 100 },{ 5, 5 },{ 5, 5 },{ 70, 5 } };
	std::vector<Point> curChar;

	glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(250, 400, 0.0);
	drawChar(X);
	glLoadIdentity();
	glTranslatef(320, 400, 0.0);
	drawChar(Y);
	glLoadIdentity();
	glTranslatef(390, 400, 0.0);
	drawChar(Z);

	Matrix m = shearXMatrix(0.35);
	curChar = X;
	glLoadIdentity();
	glTranslatef(250, 250, 0.0);
	transformPoints(m, curChar);
	drawChar(curChar);
	curChar = Y;
	glLoadIdentity();
	glTranslatef(320, 250, 0.0);
	transformPoints(m, curChar);
	drawChar(curChar);
	curChar = Z;
	glLoadIdentity();
	glTranslatef(390, 250, 0.0);
	transformPoints(m, curChar);
	drawChar(curChar);

	m = shearXMatrix(1);
	curChar = X;
	glLoadIdentity();
	glTranslatef(250, 100, 0.0);
	transformPoints(m, curChar);
	drawChar(curChar);
	curChar = Y;
	glLoadIdentity();
	glTranslatef(320, 100, 0.0);
	transformPoints(m, curChar);
	drawChar(curChar);
	curChar = Z;
	glLoadIdentity();
	glTranslatef(390, 100, 0.0);
	transformPoints(m, curChar);
	drawChar(curChar);
		
	glFlush();
}

void code_7_exercise_17()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_18
// 设点P与OX的夹角为theta1，与OX'的夹角为theta2，theta为坐标系旋转度数，即
// OX'与OX的夹角，则有theta2 = theta1 - theta，设OP = r，根据三角关系有：
// sin1 = y / r, cos1 = x / r, sin2 = y' / r, cos2 = x' / r,则x' = rcos2 =
// r(cos1cos + sin1sin) = rcos1cos + rsin1sin = xcos + ysin，而y' = rsin2
// = r(sin1cos - sincos1) = rsin1cos - rsincos1 = ycos - xsin

struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
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
		temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void drawPoints(const std::vector<Point>& points)
{
	glBegin(GL_POINTS);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> P = { { 100, 100 }};
	std::vector<Point> curP;
	float thetaD = 30;
	float thetaA = 30 * PI / 180;

	glMatrixMode(GL_MODELVIEW);
	
	// 顺时针旋转变换
	curP = P;
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(250, 250, 0.0);
	drawCoordinate();
	drawPoints(curP);
	glColor3f(1.0, 0.0, 0.0);
	transformPoints(rotateMatrix(-thetaA), curP);
	glRotatef(thetaD, 0.0, 0.0, 1.0);
	drawCoordinate();
	drawPoints(curP);

	// 根据计算所得
	curP = P;
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(550, 250, 0.0);
	drawCoordinate();
	drawPoints(curP);
	glColor3f(1.0, 0.0, 0.0);
	curP[0].x = P[0].x * cos(thetaA) + P[0].y * sin(thetaA);
	curP[0].y = P[0].y * cos(thetaA) - P[0].x * sin(thetaA);
	glRotatef(thetaD, 0.0, 0.0, 1.0);
	drawCoordinate();
	drawPoints(curP);

	glFlush();
}

void code_7_exercise_18()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_19
struct Point { float x; float y; };
struct Vector { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
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
		temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
void drawPoints(const std::vector<Point>& points)
{
	glBegin(GL_POINTS);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> P = { { 100, 100 } };
	std::vector<Point> curP;
	Matrix compound(3, 3);

	Point P0 = { 100, 50 };
	Vector V = { -70, 100 };

	float distanceV = sqrt(V.x * V.x + V.y * V.y);
	Vector v = { V.x / distanceV, V.y / distanceV };
	Vector u = { v.y, -v.x };

	glMatrixMode(GL_MODELVIEW);

	// 通过旋转矩阵得到变换矩阵

	float thetaA = atan(u.y / u.x);
	float thetaD = thetaA * 180 / PI;
	curP = P;
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(200, 250, 0.0);
	drawCoordinate();
	drawPoints(curP);
	glColor3f(1.0, 0.0, 0.0);
	compound = rotateMatrix(-thetaA) * translateMatrix(-P0.x, -P0.y);
	transformPoints(compound, curP);
	glTranslatef(P0.x, P0.y, 0.0);
	glRotatef(thetaD, 0.0, 0.0, 1.0);
	drawCoordinate();
	drawPoints(curP);

	// 通过旋转矩阵得到变换矩阵
	curP = P;
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(550, 250, 0.0);
	drawCoordinate();
	drawPoints(curP);
	glColor3f(1.0, 0.0, 0.0);
	Matrix r(3, 3);
	matrixSetIdentity(r);
	r[0][0] = u.x;
	r[0][1] = u.y;
	r[1][0] = v.x;
	r[1][1] = v.y;
	compound = r * translateMatrix(-P0.x, -P0.y);
	transformPoints(compound, curP);
	glTranslatef(P0.x, P0.y, 0.0);
	glRotatef(thetaD, 0.0, 0.0, 1.0);
	drawCoordinate();
	drawPoints(curP);
	
	glFlush();
}

void code_7_exercise_19()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_20
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	////glPixelStorei和glDrawPixels共同使用时要注意内存对齐问题
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glRasterPos2i(100, 100);
	//GLubyte colorShape[60] = { 0 };
	//for (int i = 0; i < 58; i++)
	//{
	//	colorShape[i] = 0xFF;
	//}
	//glDrawPixels(5, 4, GL_RGB, GL_UNSIGNED_BYTE, colorShape);

	//GLubyte colorShape1[80] = { 0 };
	//memset(colorShape1, 0xaa, 80);
	//glReadPixels(100, 100, 5, 4, GL_RGB, GL_UNSIGNED_BYTE, colorShape1);
	//glRasterPos2i(200, 100);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 这里若不为4字节对齐，将出错，因为glReadPixels是按照4字节对齐读取的
	//glDrawPixels(5, 4, GL_RGB, GL_UNSIGNED_BYTE, colorShape1);

	glBegin(GL_POLYGON);
	glVertex2f(100, 100);
	glVertex2f(200, 100);
	glVertex2f(200, 200);
	glVertex2f(100, 200);
	glEnd();

	// 读取-写入像素
	//GLubyte colorShape[100 * 100 * 3 + 100 * 3] = { 0 };
	//glReadPixels(100, 100, 100, 100, GL_RGB, GL_UNSIGNED_BYTE, colorShape);

	//glRasterPos2i(100, 300);
	//glDrawPixels(100, 100, GL_RGB, GL_UNSIGNED_BYTE, colorShape);

	// 直接拷贝像素
	glRasterPos2i(400, 300);
	glCopyPixels(100, 100, 100, 100, GL_COLOR);

	glFlush();
}

void code_7_exercise_20()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_21
struct Point { float x; float y; };
void drawPolygon(const std::vector<Point>& points, float r, float g, float b)
{
	glColor3f(r, g, b);

	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
GLubyte pixelsTemp[30 * 50 * 3 + 50 * 3] = { 0 };
void readAndDrawPixels(float readX, float readY, float readWidth, float readHeight, float drawX, float drawY, int logicOP)
{
	memset(pixelsTemp, 0, sizeof(pixelsTemp));
	glReadPixels(readX, readY, readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);
	
	glRasterPos2i(drawX, drawY);
	glLogicOp(logicOP);
	glDrawPixels(readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);
	glLogicOp(GL_COPY);

	// dubug
	//memset(pixelsTemp, 0, sizeof(pixelsTemp));
	//glReadPixels(drawX, drawY, readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);
}
void copyPixels(float readX, float readY, float readWidth, float readHeight, float drawX, float drawY, int logicOP)
{
	glRasterPos2i(drawX, drawY);

	glLogicOp(logicOP);
	glCopyPixels(readX, readY, readWidth, readHeight, GL_COLOR);
	glLogicOp(GL_COPY);

	// dubug
	//memset(pixelsTemp, 0, sizeof(pixelsTemp));
	//glReadPixels(drawX, drawY, readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_COLOR_LOGIC_OP);
	
	// AND操作
	drawPolygon({ { 30, 500 } ,{ 60, 500 },{ 60, 550 },{ 30, 550 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 70, 500 } ,{ 100, 500 },{ 100, 550 },{ 70, 550 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 110, 500 } ,{ 140, 500 },{ 140, 550 },{ 110, 550 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 150, 500 } ,{ 180, 500 },{ 180, 550 },{ 150, 550 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(30, 500, 30, 50, 110, 500, GL_AND);
	readAndDrawPixels(30, 500, 30, 50, 150, 500, GL_AND);
	readAndDrawPixels(30, 500, 30, 50, 150, 500, GL_AND);

	drawPolygon({ { 230, 500 } ,{ 260, 500 },{ 260, 550 },{ 230, 550 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 270, 500 } ,{ 300, 500 },{ 300, 550 },{ 270, 550 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 310, 500 } ,{ 340, 500 },{ 340, 550 },{ 310, 550 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 350, 500 } ,{ 380, 500 },{ 380, 550 },{ 350, 550 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(230, 500, 30, 50, 310, 500, GL_AND);
	readAndDrawPixels(230, 500, 30, 50, 350, 500, GL_AND);
	readAndDrawPixels(230, 500, 30, 50, 350, 500, GL_AND);

	drawPolygon({ { 430, 500 } ,{ 460, 500 },{ 460, 550 },{ 430, 550 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 470, 500 } ,{ 500, 500 },{ 500, 550 },{ 470, 550 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 510, 500 } ,{ 540, 500 },{ 540, 550 },{ 510, 550 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 550, 500 } ,{ 580, 500 },{ 580, 550 },{ 550, 550 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(430, 500, 30, 50, 510, 500, GL_AND);
	readAndDrawPixels(430, 500, 30, 50, 550, 500, GL_AND);
	readAndDrawPixels(430, 500, 30, 50, 550, 500, GL_AND);

	drawPolygon({ { 630, 500 } ,{ 660, 500 },{ 660, 550 },{ 630, 550 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 670, 500 } ,{ 700, 500 },{ 700, 550 },{ 670, 550 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 710, 500 } ,{ 740, 500 },{ 740, 550 },{ 710, 550 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 750, 500 } ,{ 780, 500 },{ 780, 550 },{ 750, 550 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(630, 500, 30, 50, 710, 500, GL_AND);
	readAndDrawPixels(630, 500, 30, 50, 750, 500, GL_AND);
	readAndDrawPixels(630, 500, 30, 50, 750, 500, GL_AND);

	drawPolygon({ { 30, 400 } ,{ 60, 400 },{ 60, 450 },{ 30, 450 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 70, 400 } ,{ 100, 400 },{ 100, 450 },{ 70, 450 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 110, 400 } ,{ 140, 400 },{ 140, 450 },{ 110, 450 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 150, 400 } ,{ 180, 400 },{ 180, 450 },{ 150, 450 } }, 0.0, 0.0, 0.0);
	copyPixels(30, 400, 30, 50, 110, 400, GL_AND);
	copyPixels(30, 400, 30, 50, 150, 400, GL_AND);
	copyPixels(30, 400, 30, 50, 150, 400, GL_AND);

	drawPolygon({ { 230, 400 } ,{ 260, 400 },{ 260, 450 },{ 230, 450 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 270, 400 } ,{ 300, 400 },{ 300, 450 },{ 270, 450 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 310, 400 } ,{ 340, 400 },{ 340, 450 },{ 310, 450 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 350, 400 } ,{ 380, 400 },{ 380, 450 },{ 350, 450 } }, 0.0, 0.0, 0.0);
	copyPixels(230, 400, 30, 50, 310, 400, GL_AND);
	copyPixels(230, 400, 30, 50, 350, 400, GL_AND);
	copyPixels(230, 400, 30, 50, 350, 400, GL_AND);

	drawPolygon({ { 430, 400 } ,{ 460, 400 },{ 460, 450 },{ 430, 450 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 470, 400 } ,{ 500, 400 },{ 500, 450 },{ 470, 450 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 510, 400 } ,{ 540, 400 },{ 540, 450 },{ 510, 450 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 550, 400 } ,{ 580, 400 },{ 580, 450 },{ 550, 450 } }, 1.0, 1.0, 1.0);
	copyPixels(430, 400, 30, 50, 510, 400, GL_COPY);
	copyPixels(430, 400, 30, 50, 550, 400, GL_COPY);
	copyPixels(430, 400, 30, 50, 550, 400, GL_COPY);

	drawPolygon({ { 630, 400 } ,{ 660, 400 },{ 660, 450 },{ 630, 450 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 670, 400 } ,{ 700, 400 },{ 700, 450 },{ 670, 450 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 710, 400 } ,{ 740, 400 },{ 740, 450 },{ 710, 450 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 750, 400 } ,{ 780, 400 },{ 780, 450 },{ 750, 450 } }, 1.0, 1.0, 1.0);
	copyPixels(630, 400, 30, 50, 710, 400, GL_AND);
	copyPixels(630, 400, 30, 50, 750, 400, GL_AND);
	copyPixels(630, 400, 30, 50, 750, 400, GL_AND);

	// OR操作
	drawPolygon({ { 30, 300 } ,{ 60, 300 },{ 60, 350 },{ 30, 350 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 70, 300 } ,{ 100, 300 },{ 100, 350 },{ 70, 350 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 110, 300 } ,{ 140, 300 },{ 140, 350 },{ 110, 350 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 150, 300 } ,{ 180, 300 },{ 180, 350 },{ 150, 350 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(30, 300, 30, 50, 110, 300, GL_OR);
	readAndDrawPixels(30, 300, 30, 50, 150, 300, GL_OR);
	readAndDrawPixels(30, 300, 30, 50, 150, 300, GL_OR);

	drawPolygon({ { 230, 300 } ,{ 260, 300 },{ 260, 350 },{ 230, 350 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 270, 300 } ,{ 300, 300 },{ 300, 350 },{ 270, 350 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 310, 300 } ,{ 340, 300 },{ 340, 350 },{ 310, 350 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 350, 300 } ,{ 380, 300 },{ 380, 350 },{ 350, 350 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(230, 300, 30, 50, 310, 300, GL_OR);
	readAndDrawPixels(230, 300, 30, 50, 350, 300, GL_OR);
	readAndDrawPixels(230, 300, 30, 50, 350, 300, GL_OR);

	drawPolygon({ { 430, 300 } ,{ 460, 300 },{ 460, 350 },{ 430, 350 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 470, 300 } ,{ 500, 300 },{ 500, 350 },{ 470, 350 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 510, 300 } ,{ 540, 300 },{ 540, 350 },{ 510, 350 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 550, 300 } ,{ 580, 300 },{ 580, 350 },{ 550, 350 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(430, 300, 30, 50, 510, 300, GL_OR);
	readAndDrawPixels(430, 300, 30, 50, 550, 300, GL_OR);
	readAndDrawPixels(430, 300, 30, 50, 550, 300, GL_OR);

	drawPolygon({ { 630, 300 } ,{ 660, 300 },{ 660, 350 },{ 630, 350 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 670, 300 } ,{ 700, 300 },{ 700, 350 },{ 670, 350 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 710, 300 } ,{ 740, 300 },{ 740, 350 },{ 710, 350 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 750, 300 } ,{ 780, 300 },{ 780, 350 },{ 750, 350 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(630, 300, 30, 50, 710, 300, GL_OR);
	readAndDrawPixels(630, 300, 30, 50, 750, 300, GL_OR);
	readAndDrawPixels(630, 300, 30, 50, 750, 300, GL_OR);

	drawPolygon({ { 30, 200 } ,{ 60, 200 },{ 60, 250 },{ 30, 250 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 70, 200 } ,{ 100, 200 },{ 100, 250 },{ 70, 250 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 110, 200 } ,{ 140, 200 },{ 140, 250 },{ 110, 250 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 150, 200 } ,{ 180, 200 },{ 180, 250 },{ 150, 250 } }, 0.0, 0.0, 0.0);
	copyPixels(30, 200, 30, 50, 110, 200, GL_OR);
	copyPixels(30, 200, 30, 50, 150, 200, GL_OR);
	copyPixels(30, 200, 30, 50, 150, 200, GL_OR);

	drawPolygon({ { 230, 200 } ,{ 260, 200 },{ 260, 250 },{ 230, 250 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 270, 200 } ,{ 300, 200 },{ 300, 250 },{ 270, 250 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 310, 200 } ,{ 340, 200 },{ 340, 250 },{ 310, 250 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 350, 200 } ,{ 380, 200 },{ 380, 250 },{ 350, 250 } }, 0.0, 0.0, 0.0);
	copyPixels(230, 200, 30, 50, 310, 200, GL_OR);
	copyPixels(230, 200, 30, 50, 350, 200, GL_OR);
	copyPixels(230, 200, 30, 50, 350, 200, GL_OR);

	drawPolygon({ { 430, 200 } ,{ 460, 200 },{ 460, 250 },{ 430, 250 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 470, 200 } ,{ 500, 200 },{ 500, 250 },{ 470, 250 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 510, 200 } ,{ 540, 200 },{ 540, 250 },{ 510, 250 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 550, 200 } ,{ 580, 200 },{ 580, 250 },{ 550, 250 } }, 1.0, 1.0, 1.0);
	copyPixels(430, 200, 30, 50, 510, 200, GL_OR);
	copyPixels(430, 200, 30, 50, 550, 200, GL_OR);
	copyPixels(430, 200, 30, 50, 550, 200, GL_OR);

	drawPolygon({ { 630, 200 } ,{ 660, 200 },{ 660, 250 },{ 630, 250 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 670, 200 } ,{ 700, 200 },{ 700, 250 },{ 670, 250 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 710, 200 } ,{ 740, 200 },{ 740, 250 },{ 710, 250 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 750, 200 } ,{ 780, 200 },{ 780, 250 },{ 750, 250 } }, 1.0, 1.0, 1.0);
	copyPixels(630, 200, 30, 50, 710, 200, GL_OR);
	copyPixels(630, 200, 30, 50, 750, 200, GL_OR);
	copyPixels(630, 200, 30, 50, 750, 200, GL_OR);

	// XOR操作
	drawPolygon({ { 30, 100 } ,{ 60, 100 },{ 60, 150 },{ 30, 150 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 70, 100 } ,{ 100, 100 },{ 100, 150 },{ 70, 150 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 110, 100 } ,{ 140, 100 },{ 140, 150 },{ 110, 150 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 150, 100 } ,{ 180, 100 },{ 180, 150 },{ 150, 150 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(30, 100, 30, 50, 110, 100, GL_XOR);
	readAndDrawPixels(30, 100, 30, 50, 150, 100, GL_XOR);
	readAndDrawPixels(30, 100, 30, 50, 150, 100, GL_XOR);

	drawPolygon({ { 230, 100 } ,{ 260, 100 },{ 260, 150 },{ 230, 150 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 270, 100 } ,{ 300, 100 },{ 300, 150 },{ 270, 150 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 310, 100 } ,{ 340, 100 },{ 340, 150 },{ 310, 150 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 350, 100 } ,{ 380, 100 },{ 380, 150 },{ 350, 150 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(230, 100, 30, 50, 310, 100, GL_XOR);
	readAndDrawPixels(230, 100, 30, 50, 350, 100, GL_XOR);
	readAndDrawPixels(230, 100, 30, 50, 350, 100, GL_XOR);

	drawPolygon({ { 430, 100 } ,{ 460, 100 },{ 460, 150 },{ 430, 150 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 470, 100 } ,{ 500, 100 },{ 500, 150 },{ 470, 150 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 510, 100 } ,{ 540, 100 },{ 540, 150 },{ 510, 150 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 550, 100 } ,{ 580, 100 },{ 580, 150 },{ 550, 150 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(430, 100, 30, 50, 510, 100, GL_XOR);
	readAndDrawPixels(430, 100, 30, 50, 550, 100, GL_XOR);
	readAndDrawPixels(430, 100, 30, 50, 550, 100, GL_XOR);

	drawPolygon({ { 630, 100 } ,{ 660, 100 },{ 660, 150 },{ 630, 150 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 670, 100 } ,{ 700, 100 },{ 700, 150 },{ 670, 150 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 710, 100 } ,{ 740, 100 },{ 740, 150 },{ 710, 150 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 750, 100 } ,{ 780, 100 },{ 780, 150 },{ 750, 150 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(630, 100, 30, 50, 710, 100, GL_XOR);
	readAndDrawPixels(630, 100, 30, 50, 750, 100, GL_XOR);
	readAndDrawPixels(630, 100, 30, 50, 750, 100, GL_XOR);

	drawPolygon({ { 30, 0 } ,{ 60, 0 },{ 60, 50 },{ 30, 50 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 70, 0 } ,{ 100, 0 },{ 100, 50 },{ 70, 50 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 110, 0 } ,{ 140, 0 },{ 140, 50 },{ 110, 50 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 150, 0 } ,{ 180, 0 },{ 180, 50 },{ 150, 50 } }, 0.0, 0.0, 0.0);
	copyPixels(30, 0, 30, 50, 110, 0, GL_XOR);
	copyPixels(30, 0, 30, 50, 150, 0, GL_XOR);
	copyPixels(30, 0, 30, 50, 150, 0, GL_XOR);

	drawPolygon({ { 230, 0 } ,{ 260, 0 },{ 260, 50 },{ 230, 50 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 270, 0 } ,{ 300, 0 },{ 300, 50 },{ 270, 50 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 310, 0 } ,{ 340, 0 },{ 340, 50 },{ 310, 50 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 350, 0 } ,{ 380, 0 },{ 380, 50 },{ 350, 50 } }, 0.0, 0.0, 0.0);
	copyPixels(230, 0, 30, 50, 310, 0, GL_XOR);
	copyPixels(230, 0, 30, 50, 350, 0, GL_XOR);
	copyPixels(230, 0, 30, 50, 350, 0, GL_XOR);

	drawPolygon({ { 430, 0 } ,{ 460, 0 },{ 460, 50 },{ 430, 50 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 470, 0 } ,{ 500, 0 },{ 500, 50 },{ 470, 50 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 510, 0 } ,{ 540, 0 },{ 540, 50 },{ 510, 50 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 550, 0 } ,{ 580, 0 },{ 580, 50 },{ 550, 50 } }, 1.0, 1.0, 1.0);
	copyPixels(430, 0, 30, 50, 510, 0, GL_XOR);
	copyPixels(430, 0, 30, 50, 550, 0, GL_XOR);
	copyPixels(430, 0, 30, 50, 550, 0, GL_XOR);

	drawPolygon({ { 630, 0 } ,{ 660, 0 },{ 660, 50 },{ 630, 50 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 670, 0 } ,{ 700, 0 },{ 700, 50 },{ 670, 50 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 710, 0 } ,{ 740, 0 },{ 740, 50 },{ 710, 50 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 750, 0 } ,{ 780, 0 },{ 780, 50 },{ 750, 50 } }, 1.0, 1.0, 1.0);
	copyPixels(630, 0, 30, 50, 710, 0, GL_XOR);
	copyPixels(630, 0, 30, 50, 750, 0, GL_XOR);
	copyPixels(630, 0, 30, 50, 750, 0, GL_XOR);
	
	glFlush();
}

void code_7_exercise_21()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_22
struct Point { float x; float y; };
void drawPolygon(const std::vector<Point>& points, float r, float g, float b)
{
	glColor3f(r, g, b);

	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
GLubyte pixelsTemp[30 * 30 * 3 + 30 * 3] = { 0 };
void readAndDrawPixels(float readX, float readY, float readWidth, float readHeight, float drawX, float drawY, int logicOP)
{
	memset(pixelsTemp, 0, sizeof(pixelsTemp));
	glReadPixels(readX, readY, readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);

	glRasterPos2i(drawX, drawY);

	int preLogicOP = 0;
	glGetIntegerv(GL_LOGIC_OP_MODE, &preLogicOP);
	glLogicOp(logicOP);
	glDrawPixels(readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);
	glLogicOp(preLogicOP);

	// dubug
	//memset(pixelsTemp, 0, sizeof(pixelsTemp));
	//glReadPixels(drawX, drawY, readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);
}
void logicTransform(float y, int logicOP)
{
	drawPolygon({ { 30, y } ,{ 60, y },{ 60, y + 30 },{ 30, y + 30 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 70, y } ,{ 100, y },{ 100, y + 30 },{ 70, y + 30 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 110, y } ,{ 140, y },{ 140, y + 30 },{ 110, y + 30 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 150, y } ,{ 180, y },{ 180, y + 30 },{ 150, y + 30 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(30, y, 30, 30, 110, y, logicOP);
	readAndDrawPixels(30, y, 30, 30, 150, y, logicOP);
	readAndDrawPixels(30, y, 30, 30, 150, y, logicOP);

	drawPolygon({ { 230, y } ,{ 260, y },{ 260, y + 30 },{ 230, y + 30 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 270, y } ,{ 300, y },{ 300, y + 30 },{ 270, y + 30 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 310, y } ,{ 340, y },{ 340, y + 30 },{ 310, y + 30 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 350, y } ,{ 380, y },{ 380, y + 30 },{ 350, y + 30 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(230, y, 30, 30, 310, y, logicOP);
	readAndDrawPixels(230, y, 30, 30, 350, y, logicOP);
	readAndDrawPixels(230, y, 30, 30, 350, y, logicOP);

	drawPolygon({ { 430, y } ,{ 460, y },{ 460, y + 30 },{ 430, y + 30 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 470, y } ,{ 500, y },{ 500, y + 30 },{ 470, y + 30 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 510, y } ,{ 540, y },{ 540, y + 30 },{ 510, y + 30 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 550, y } ,{ 580, y },{ 580, y + 30 },{ 550, y + 30 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(430, y, 30, 30, 510, y, logicOP);
	readAndDrawPixels(430, y, 30, 30, 550, y, logicOP);
	readAndDrawPixels(430, y, 30, 30, 550, y, logicOP);

	drawPolygon({ { 630, y } ,{ 660, y },{ 660, y + 30 },{ 630, y + 30 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 670, y } ,{ 700, y },{ 700, y + 30 },{ 670, y + 30 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 710, y } ,{ 740, y },{ 740, y + 30 },{ 710, y + 30 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 750, y } ,{ 780, y },{ 780, y + 30 },{ 750, y + 30 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(630, y, 30, 30, 710, y, logicOP);
	readAndDrawPixels(630, y, 30, 30, 750, y, logicOP);
	readAndDrawPixels(630, y, 30, 30, 750, y, logicOP);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_COLOR_LOGIC_OP);

	logicTransform(550, GL_AND);
	logicTransform(515, GL_OR);
	logicTransform(480, GL_XOR);
	logicTransform(445, GL_COPY_INVERTED);
	logicTransform(410, GL_INVERT);
	logicTransform(375, GL_CLEAR);
	logicTransform(340, GL_SET);
	logicTransform(305, GL_COPY);
	logicTransform(270, GL_NOOP);
	logicTransform(235, GL_NAND);
	logicTransform(200, GL_NOR);
	logicTransform(165, GL_EQUIV);
	logicTransform(130, GL_AND_REVERSE);
	logicTransform(95, GL_AND_INVERTED);
	logicTransform(60, GL_OR_REVERSE);
	logicTransform(25, GL_OR_INVERTED);

	glFlush();
}

void code_7_exercise_22()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_23
struct Point { float x; float y; };
void drawPolygon(const std::vector<Point>& points, float r, float g, float b)
{
	glColor3f(r, g, b);

	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
GLubyte pixelsTemp[60 * 30 * 3 + 30 * 3] = { 0 };
void readAndDrawPixels(float readX, float readY, float readWidth, float readHeight, float drawX, float drawY, int logicOP)
{
	memset(pixelsTemp, 0, sizeof(pixelsTemp));
	glReadPixels(readX, readY, readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);

	glRasterPos2i(drawX, drawY);

	int preLogicOP = 0;
	glGetIntegerv(GL_LOGIC_OP_MODE, &preLogicOP);
	glLogicOp(logicOP);
	glDrawPixels(readWidth, readHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);
	glLogicOp(preLogicOP);
}
void clearPixels(float x, float y, float width, float height)
{
	memset(pixelsTemp, 0, sizeof(pixelsTemp));
	glRasterPos2i(x, y);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixelsTemp);
}
void logicTransform(float y, int logicOP)
{
	drawPolygon({ { 20, y } ,{ 80, y },{ 80, y + 30 },{ 20, y + 30 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 120, y } ,{ 180, y },{ 180, y + 30 },{ 120, y + 30 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(20, y, 60, 30, 120, y, logicOP);
	clearPixels(20, y, 60, 30);

	drawPolygon({ { 220, y } ,{ 280, y },{ 280, y + 30 },{ 220, y + 30 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 320, y } ,{ 380, y },{ 380, y + 30 },{ 320, y + 30 } }, 0.0, 0.0, 0.0);
	readAndDrawPixels(220, y, 60, 30, 320, y, logicOP);
	clearPixels(220, y, 60, 30);

	drawPolygon({ { 420, y } ,{ 480, y },{ 480, y + 30 },{ 420, y + 30 } }, 0.0, 0.0, 0.0);
	drawPolygon({ { 520, y } ,{ 580, y },{ 580, y + 30 },{ 520, y + 30 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(420, y, 60, 30, 520, y, logicOP);
	clearPixels(420, y, 60, 30);

	drawPolygon({ { 620, y } ,{ 680, y },{ 680, y + 30 },{ 620, y + 30 } }, 1.0, 1.0, 1.0);
	drawPolygon({ { 720, y } ,{ 780, y },{ 780, y + 30 },{ 720, y + 30 } }, 1.0, 1.0, 1.0);
	readAndDrawPixels(620, y, 60, 30, 720, y, logicOP);
	clearPixels(620, y, 60, 30);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_COLOR_LOGIC_OP);

	logicTransform(550, GL_AND);
	logicTransform(515, GL_OR);
	logicTransform(480, GL_XOR);
	logicTransform(445, GL_COPY_INVERTED);
	logicTransform(410, GL_INVERT);
	logicTransform(375, GL_CLEAR);
	logicTransform(340, GL_SET);
	logicTransform(305, GL_COPY);
	logicTransform(270, GL_NOOP);
	logicTransform(235, GL_NAND);
	logicTransform(200, GL_NOR);
	logicTransform(165, GL_EQUIV);
	logicTransform(130, GL_AND_REVERSE);
	logicTransform(95, GL_AND_INVERTED);
	logicTransform(60, GL_OR_REVERSE);
	logicTransform(25, GL_OR_INVERTED);

	glFlush();
}

void code_7_exercise_23()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_24
struct Point { float x; float y; };
struct ColorElement
{
	ColorElement() {};
	ColorElement(GLubyte r, GLubyte g, GLubyte b) : _r(r), _g(g), _b(b) {};
	GLubyte _r = 0x00;
	GLubyte _g = 0x00;
	GLubyte _b = 0x00;
};
struct ColorArray
{
	ColorArray(int w, int h)
	{
		_w = w;
		_h = h;
		_data.assign(_h, std::vector<ColorElement>(_w, ColorElement()));
		_arrayData.assign(_w * _h * 3, 0x00);
	}
	std::vector<ColorElement>& operator [](int h)
	{
		return _data[h];
	}
	operator GLubyte* () 
	{
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w; j++)
			{
				int pos = (i * _w + j) * 3;
				_arrayData[pos] = _data[i][j]._r;
				_arrayData[pos + 1] = _data[i][j]._g;
				_arrayData[pos + 2] = _data[i][j]._b;
			}
		}
		return &_arrayData[0];
	}
	ColorArray& turnTranspose()
	{
		ColorArray temp(_h, _w);
		if (_w >= _h)
		{
			for (int i = _h - 1; i >= 0; i--)
			{
				for (int j = _h - 1 - i; j < _w; j++)
				{
					temp[temp._h - 1 - j][_h - 1 - i] = _data[i][j];
					if (j < _h)
						temp[temp._h - _h + i][j] = _data[_h - 1 - j][_h - 1 - i];
				}
			}
		}
		else
		{
			for (int j = 0; j < _w; j++)
			{
				for (int i = _h - 1 - j; i >= 0; i--)
				{
					temp[temp._h - 1 - j][_h - 1 - i] = _data[i][j];
					if (i > _h - 1 - _w)
						temp[temp._h - _h + i][j] = _data[_h - 1 - j][_h - 1 - i];
				}
			}
		}
		*this = temp;
		return *this;
	}
	ColorArray& turnLeftRight()
	{
		ColorElement temp;
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w / 2; j++)
			{
				temp = _data[i][j];
				_data[i][j] = _data[i][_w - 1 - j];
				_data[i][_w - 1 - j] = temp;
			}
		}
		return *this;
	}
	ColorArray& turnUpDown()
	{
		ColorElement temp;
		for (int j = 0; j < _w; j++)
		{
			for (int i = 0; i < _h / 2; i++)
			{
				temp = _data[i][j];
				_data[i][j] = _data[_h - 1 - i][j];
				_data[_h - 1 - i][j] = temp;
			}
		}
		return *this;
	}

	int _w;
	int _h;
	std::vector<std::vector<ColorElement>> _data;
	std::vector<GLubyte> _arrayData;
};
const int W = 150;
const int H = 100;
void drawPolygon(const std::vector<Point>& points, float r, float g, float b)
{
	glColor3f(r, g, b);

	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawPixels(float x, float y, ColorArray& colorArray)
{
	glRasterPos2i(x, y);
	glDrawPixels(colorArray._w, colorArray._h, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
}
void same(ColorArray& c1, ColorArray& c2)
{
	assert(c1._w == c2._w);
	assert(c1._h == c2._h);
	for (int i = 0; i < c1._h; i++)
	{
		for (int j = 0; j < c1._w; j++)
		{
			assert(c1[i][j]._r == c2[i][j]._r);
			assert(c1[i][j]._g == c2[i][j]._g);
			assert(c1[i][j]._b == c2[i][j]._b);
		}
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	ColorArray colorArray(150, 100);
	for (int i = 0; i < colorArray._h / 2; i++)
	{
		for (int j = 0; j < colorArray._w / 2; j++)
			colorArray[i][j] = ColorElement(0xFF, 0xFF, 0xFF);
	}
	for (int i = 0; i < colorArray._h / 2; i++)
	{
		for (int j = colorArray._w / 2; j < colorArray._w; j++)
			colorArray[i][j] = ColorElement(0xFF, 0x00, 0x00);
	}
	for (int i = colorArray._h / 2; i < colorArray._h; i++)
	{
		for (int j = 0; j < colorArray._w / 2; j++)
			colorArray[i][j] = ColorElement(0x00, 0xFF, 0x00);
	}
	for (int i = colorArray._h / 2; i < colorArray._h; i++)
	{
		for (int j = colorArray._w / 2; j < colorArray._w; j++)
			colorArray[i][j] = ColorElement(0x00, 0x00, 0xFF);
	}
	drawPixels(50, 250, colorArray);

	// 90
	auto ca = colorArray;
	drawPixels(300, 430, ca.turnTranspose().turnUpDown());

	ca = colorArray;
	drawPixels(600, 430, ca.turnLeftRight().turnTranspose());

	// 180
	ca = colorArray;
	drawPixels(300, 300, ca.turnLeftRight().turnUpDown());

	ca = colorArray;
	drawPixels(600, 300, ca.turnUpDown().turnLeftRight());

	// 270
	ca = colorArray;
	drawPixels(300, 130, ca.turnTranspose().turnLeftRight());

	ca = colorArray;
	drawPixels(600, 130, ca.turnUpDown().turnTranspose());

	// 360
	ca = colorArray;
	drawPixels(200, 20, 
		ca.turnTranspose().turnUpDown()
		.turnTranspose().turnUpDown()
		.turnTranspose().turnUpDown()
		.turnTranspose().turnUpDown());

	ca = colorArray;
	drawPixels(400, 20,
		ca.turnLeftRight().turnUpDown()
		.turnLeftRight().turnUpDown());

	ca = colorArray;
	drawPixels(600, 20,
		ca.turnTranspose().turnLeftRight()
		.turnTranspose().turnLeftRight()
		.turnTranspose().turnLeftRight()
		.turnTranspose().turnLeftRight());

	glFlush();
}

void code_7_exercise_24()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_24_1
struct Point { float x; float y; };
struct ColorElement
{
	ColorElement() {};
	ColorElement(GLubyte r, GLubyte g, GLubyte b) : _r(r), _g(g), _b(b) {};
	GLubyte _r = 0x00;
	GLubyte _g = 0x00;
	GLubyte _b = 0x00;
};
struct ColorArray
{
	ColorArray(int w, int h)
	{
		_w = w;
		_h = h;
		_data.assign(_h, std::vector<ColorElement>(_w, ColorElement()));
		_arrayData.assign(_w * _h * 3, 0x00);
	}
	std::vector<ColorElement>& operator [](int h)
	{
		return _data[h];
	}
	operator GLubyte* ()
	{
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w; j++)
			{
				int pos = (i * _w + j) * 3;
				_arrayData[pos] = _data[i][j]._r;
				_arrayData[pos + 1] = _data[i][j]._g;
				_arrayData[pos + 2] = _data[i][j]._b;
			}
		}
		return &_arrayData[0];
	}
	ColorArray& turnTranspose()
	{
		ColorArray temp(_h, _w);
		if (_w >= _h)
		{
			for (int i = _h - 1; i >= 0; i--)
			{
				for (int j = _h - 1 - i; j < _w; j++)
				{
					temp[temp._h - 1 - j][_h - 1 - i] = _data[i][j];
					if (j < _h)
						temp[temp._h - _h + i][j] = _data[_h - 1 - j][_h - 1 - i];
				}
			}
		}
		else
		{
			for (int j = 0; j < _w; j++)
			{
				for (int i = _h - 1 - j; i >= 0; i--)
				{
					temp[temp._h - 1 - j][_h - 1 - i] = _data[i][j];
					if (i > _h - 1 - _w)
						temp[temp._h - _h + i][j] = _data[_h - 1 - j][_h - 1 - i];
				}
			}
		}
		*this = temp;
		return *this;
	}
	ColorArray& turnLeftRight()
	{
		ColorElement temp;
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w / 2; j++)
			{
				temp = _data[i][j];
				_data[i][j] = _data[i][_w - 1 - j];
				_data[i][_w - 1 - j] = temp;
			}
		}
		return *this;
	}
	ColorArray& turnUpDown()
	{
		ColorElement temp;
		for (int j = 0; j < _w; j++)
		{
			for (int i = 0; i < _h / 2; i++)
			{
				temp = _data[i][j];
				_data[i][j] = _data[_h - 1 - i][j];
				_data[_h - 1 - i][j] = temp;
			}
		}
		return *this;
	}

	int _w;
	int _h;
	std::vector<std::vector<ColorElement>> _data;
	std::vector<GLubyte> _arrayData;
};
std::string inputStr;
void inputParam()
{
	printf("input transform [(T)ranspose, (U)pDown, (L)eftRight]:");
	std::cin >> inputStr;
}
void drawPixels(float x, float y, ColorArray& colorArray)
{
	glRasterPos2i(x, y);
	glDrawPixels(colorArray._w, colorArray._h, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
}
void same(ColorArray& c1, ColorArray& c2)
{
	assert(c1._w == c2._w);
	assert(c1._h == c2._h);
	for (int i = 0; i < c1._h; i++)
	{
		for (int j = 0; j < c1._w; j++)
		{
			assert(c1[i][j]._r == c2[i][j]._r);
			assert(c1[i][j]._g == c2[i][j]._g);
			assert(c1[i][j]._b == c2[i][j]._b);
		}
	}
}
void calcOperator(std::vector<int>& operators)
{
	for (auto it = operators.begin(); it != operators.end(); it++ )
	{
		int cur = *it;
		if (it + 1 != operators.end())
		{
			int next = *(it + 1);
			if (cur >= 0 && next >= 0)
			{
				it = operators.erase(it);
				it = operators.erase(it);
				if ((cur + next) % 360 > 0)
					operators.insert(it, (cur + next) % 360);
				calcOperator(operators);
				return;
			}
			else if (cur < 0 && next < 0)
			{
				it = operators.erase(it);
				it = operators.erase(it);

				if ((cur == -1 && next == -2) || (cur == -2 && next == -1))
				{
					operators.insert(it, 180);
				}
				calcOperator(operators);
				return;
			}
		}
	}
}
void drawOperator(float x, float y, ColorArray& colorArray, std::vector<int>& operators)
{
	for (auto & op : operators)
	{
		switch (op)
		{
		case 90:
			colorArray.turnTranspose().turnUpDown();
			break;
		case 180:
			colorArray.turnLeftRight().turnUpDown();
			break;
		case 270:
			colorArray.turnTranspose().turnLeftRight();
			break;
		case -1:
			colorArray.turnUpDown();
			break;
		case -2:
			colorArray.turnLeftRight();
			break;
		default:
			break;
		}
	}
	drawPixels(x, y, colorArray);
}
void normalByFor(std::vector<int>& operators)
{
	// 遍历算法	
	std::vector<int> temp; 
	for (auto& c : inputStr)
	{
		if (c == 'T')
		{
			if (operators.empty())
			{
				operators.push_back(90);
				operators.push_back(-1);
			}
			else
			{
				temp = operators;
				temp.push_back(90);
				temp.push_back(-1);
				calcOperator(temp);
				if (temp.size() > 2)
				{
					temp = operators;
					temp.push_back(-2);
					temp.push_back(90);
					calcOperator(temp);
				}
				operators = temp;
			}
		}
		else if (c == 'U')
		{
			operators.push_back(-1);
			calcOperator(operators);
		}
		else if (c == 'L')
		{
			operators.push_back(-2);
			calcOperator(operators);
		}
	}
	printf("normalByFor:");
	for (auto & op : operators)
	{
		printf("%d ", op);
	}
	printf("\n");
}
void normalByRecursion(std::vector<int> operators, int opIdx, std::vector<std::vector<int>>& operatorss)
{
	if (operators.size() > 2)
		return;

	char c = inputStr[opIdx];
	if (c == 'T')
	{
		auto temp = operators;
		temp.push_back(90);
		temp.push_back(-1);
		calcOperator(temp);
		normalByRecursion(temp, opIdx + 1, operatorss);

		temp = operators;
		temp.push_back(-2);
		temp.push_back(90);
		calcOperator(temp);
		normalByRecursion(temp, opIdx + 1, operatorss);
	}
	else if (c == 'U')
	{
		operators.push_back(-1);
		calcOperator(operators);
		normalByRecursion(operators, opIdx + 1, operatorss);
	}
	else if(c == 'L')
	{
		operators.push_back(-2);
		calcOperator(operators);
		normalByRecursion(operators, opIdx + 1, operatorss);
	}
	else
	{
		printf("normalByRecursion:");
		for (auto & op : operators)
		{
			printf("%d ", op);
		}
		printf("\n");
		operatorss.push_back(operators);
		return;
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	ColorArray colorArray(150, 100);
	for (int i = 0; i < colorArray._h / 2; i++)
	{
		for (int j = 0; j < colorArray._w / 2; j++)
			colorArray[i][j] = ColorElement(0xFF, 0xFF, 0xFF);
	}
	for (int i = 0; i < colorArray._h / 2; i++)
	{
		for (int j = colorArray._w / 2; j < colorArray._w; j++)
			colorArray[i][j] = ColorElement(0xFF, 0x00, 0x00);
	}
	for (int i = colorArray._h / 2; i < colorArray._h; i++)
	{
		for (int j = 0; j < colorArray._w / 2; j++)
			colorArray[i][j] = ColorElement(0x00, 0xFF, 0x00);
	}
	for (int i = colorArray._h / 2; i < colorArray._h; i++)
	{
		for (int j = colorArray._w / 2; j < colorArray._w; j++)
			colorArray[i][j] = ColorElement(0x00, 0x00, 0xFF);
	}
	drawPixels(50, 400, colorArray);

	// 按输入正常变换
	auto ca = colorArray;
	for (auto& c : inputStr)
	{
		switch (c)
		{
		case 'T':
			ca.turnTranspose();
			break;
		case 'U':
			ca.turnUpDown();
			break;
		case 'L':
			ca.turnLeftRight();
			break;
		default:
			break;
		}
	}
	drawPixels(50, 150, ca);

	// 循环化简
	ca = colorArray;
	std::vector<int> operators;
	normalByFor(operators);
	drawOperator(300, 400, ca, operators);
	
	// 递归化简
	std::vector<std::vector<int>> operatorss;
	normalByRecursion(std::vector<int>(), 0, operatorss);
	for (int i = 0; i < operatorss.size(); i++)
	{
		ca = colorArray;
		drawOperator(250 + i * 200, 150, ca, operatorss[i]);
	}

	glFlush();
}

void code_7_exercise_24_1()
{
	inputParam();
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_24_2
std::string inputStr;
void inputParam()
{
	printf("input majiangs:");
	std::cin >> inputStr;
}
bool getJiang(std::vector<int>& majiangs)
{
	auto it = majiangs.begin();
	if (it != majiangs.end() && it + 1 != majiangs.end() && *it == *(it + 1))
	{
		it = majiangs.erase(it);
		it = majiangs.erase(it);
		return true;
	}
	return false;
}
bool get3(std::vector<int>& majiangs)
{
	auto it = majiangs.begin();
	if (it != majiangs.end() && it + 1 != majiangs.end() && it + 2 != majiangs.end()
		&& *it == *(it + 1) && *it == *(it + 2))
	{
		it = majiangs.erase(it);
		it = majiangs.erase(it);
		it = majiangs.erase(it);
		return true;
	}
	return false;
}
bool getLian(std::vector<int>& majiangs)
{
	std::vector<int> temp = majiangs;
	auto it = temp.begin();
	if (it != temp.end())
	{
		int cur = *it;
		int count = 1;
		for (it = temp.erase(it); it != temp.end();)
		{
			if (cur + 1 == *it)
			{
				cur = *it;
				count++;
				it = temp.erase(it);
				if (count >= 3)
				{
					majiangs = temp;
					return true;
				}
			}
			else
			{
				it++;
			}
		}
	}
	return false;
}
void majiangRecursion(std::vector<int> majiangs, bool haveJiang)
{
	if (majiangs.size() == 0)
	{
		printf("OK\n");
		return;
	}		

	auto temp = majiangs;
	if (haveJiang)
	{
		if (getLian(temp))
		{
			majiangRecursion(temp, haveJiang);
			temp = majiangs;
		}
		
		if (get3(temp))
		{
			majiangRecursion(temp, haveJiang);
			temp = majiangs;
		}
	}
	else
	{
		if (getJiang(temp))
		{
			haveJiang = true;
			majiangRecursion(temp, haveJiang);
			temp = majiangs;
			haveJiang = false;
		}
						
		if (getLian(temp))
		{
			majiangRecursion(temp, haveJiang);
			temp = majiangs;
		}
				
		if (get3(temp))
		{
			majiangRecursion(temp, haveJiang);
			temp = majiangs;
		}
	}
}
void calcHu()
{
	std::vector<int> majiangs;
	for (auto& c : inputStr)
	{
		majiangs.push_back(c - 48);
	}
	std::sort(majiangs.begin(), majiangs.end(), [](auto a, auto b) { return a < b; });
	majiangRecursion(majiangs, false);
	cin.get();
	cin.get();
}
void code_7_exercise_24_2()
{
	inputParam();
	calcHu();
}
#endif

#ifdef CHAPTER_7_EXERCISE_25
struct Point { float x; float y; };
struct Comp
{
	bool operator()(const Point& p1, const Point& p2) const
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
};
struct ColorElement
{
	ColorElement() {};
	ColorElement(GLubyte r, GLubyte g, GLubyte b) : _r(r), _g(g), _b(b) {};
	GLubyte _r = 0x00;
	GLubyte _g = 0x00;
	GLubyte _b = 0x00;
};
struct ColorArray
{
	ColorArray(int w, int h)
	{
		_w = w;
		_h = h;
		_data.assign(_h, std::vector<ColorElement>(_w, ColorElement()));
		_arrayData.assign(_w * _h * 3, 0x00);
	}
	std::vector<ColorElement>& operator [](int h)
	{
		return _data[h];
	}
	operator GLubyte* ()
	{
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w; j++)
			{
				int pos = (i * _w + j) * 3;
				_arrayData[pos] = _data[i][j]._r;
				_arrayData[pos + 1] = _data[i][j]._g;
				_arrayData[pos + 2] = _data[i][j]._b;
			}
		}
		return &_arrayData[0];
	}
	int _w;
	int _h;
	std::vector<std::vector<ColorElement>> _data;
	std::vector<GLubyte> _arrayData;
};
void drawPixels(float x, float y, ColorArray& colorArray)
{
	glRasterPos2i(x, y);
	glDrawPixels(colorArray._w, colorArray._h, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
}
void drawPoint(Point p, ColorElement c)
{
	glRasterPos2i(p.x, p.y);
	static GLubyte a[3];
	a[0] = c._r;
	a[1] = c._g;
	a[2] = c._b;
	glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, a);

	//char aa[16] = {};
	//sprintf_s(aa, "%d, %d\n", (int)p.x, (int)p.y);
	//outPut(aa);
}
bool pointInRect(Point p, float x0, float y0, float w, float h)
{
	return p.x >= x0 && p.x <= x0 + w && p.y >= y0 && p.y <= y0 + h;
}
Point rotatePoint(Point p, Point pr, float theta)
{
	Point ret;
	ret.x = p.x * std::cos(theta) - p.y * std::sin(theta) + pr.x * (1 - std::cos(theta)) + pr.y * std::sin(theta);
	ret.y = p.x * std::sin(theta) + p.y * std::cos(theta) + pr.y * (1 - std::cos(theta)) - pr.x * std::sin(theta);
	return ret;
}
// 遍历附近9个像素测试
//void _rotateByDestCenter(Point p0, ColorArray& colorArray, Point pr, float theta)
//{
//	for (int i = 0; i < colorArray._h; i++)
//	{
//		for (int j = 0; j < colorArray._w; j++)
//		{
//			float x = p0.x + j;
//			float y = p0.y + i;
//			auto _p = rotatePoint({ x, y }, pr, theta);
//			for (int _i = -1; _i <= 1; _i++)
//			{
//				for (int _j = -1; _j <= 1; _j++)
//				{
//					Point p = { std::round(_p.x) + _j, std::round(_p.y) + _i };
//					Point testP = rotatePoint(p, pr, -theta);
//					if (pointInRect(testP, x - 0.5f, y - 0.5f, 1, 1))
//					{
//						drawPoint(p, colorArray[i][j]);
//					}
//				}
//			}
//		}
//	}
//}
// 目标像素中心点在源像素区域中
void rotateByDestCenter(Point p0, ColorArray& colorArray, Point pr, float theta)
{
	float thetaA = theta * 180 / PI;
	float d = std::sqrt(2) / 2 * sin((45 + fmod(std::fabs(thetaA), 90)) * PI / 180);
	for (int i = 0; i < colorArray._h; i++)
	{
		for (int j = 0; j < colorArray._w; j++)
		{
			float x = p0.x + j;
			float y = p0.y + i;
			auto _p = rotatePoint({ x, y }, pr, theta);
			for (int _i = std::ceil(_p.y - d); _i <= std::floor(_p.y + d); _i++)
			{
				for (int _j = std::ceil(_p.x - d); _j <= std::floor(_p.x + d); _j++)
				{
					Point testP = rotatePoint({(float)_j, (float)_i}, pr, -theta);
					if (pointInRect(testP, x - 0.5f, y - 0.5f, 1, 1))
					{
						drawPoint({ (float)_j, (float)_i }, colorArray[i][j]);
					}
				}
			}
		}
	}
}
// 源像素中心点在目标像素区域中
void rotateBySourceCenter(Point p0, ColorArray& colorArray, Point pr, float theta)
{
	for (int i = 0; i < colorArray._h; i++)
	{
		for (int j = 0; j < colorArray._w; j++)
		{
			float x = p0.x + j;
			float y = p0.y + i;
			auto _p = rotatePoint({ x, y }, pr, theta);
			drawPoint({ std::round(_p.x), std::round(_p.y) }, colorArray[i][j]);
		}
	}
}
// 超采样 源像素中心点在目标像素区域中
void rotateSSBySC(Point p0, ColorArray& colorArray, Point pr, float theta, int SSLevel)
{
	std::map<Point, std::map<Point, ColorElement, Comp>, Comp> pointInfo;

	Point subP0 = { p0.x * SSLevel , p0.y * SSLevel };
	Point subPr = { pr.x * SSLevel , pr.y * SSLevel };
	for (int i = 0; i < colorArray._h * SSLevel; i++)
	{
		for (int j = 0; j < colorArray._w * SSLevel; j++)
		{
			float x = subP0.x + j;
			float y = subP0.y + i;
			auto _p = rotatePoint({ x, y }, subPr, theta);
			pointInfo[{(float)(int)(std::round(_p.x) / SSLevel), (float)(int)(std::round(_p.y) / SSLevel) }][{std::round(_p.x), std::round(_p.y)}] = colorArray[i / SSLevel][j / SSLevel];
		}
	}

	int squareSS = SSLevel * SSLevel;
	int sumR = 0;
	int sumG = 0;
	int sumB = 0;
	for (auto& p : pointInfo)
	{
		sumR = 0;
		sumG = 0;
		sumB = 0;
		for (auto& _p : p.second)
		{
			sumR += _p.second._r;
			sumG += _p.second._g;
			sumB += _p.second._b;
		}
		drawPoint(p.first, ColorElement(sumR / squareSS, sumG / squareSS , sumB / squareSS));
	}
}
// 超采样 目标像素中心点在源像素区域中
void rotateSSByDC(Point p0, ColorArray& colorArray, Point pr, float theta, int SSLevel)
{
	float thetaA = theta * 180 / PI;
	float d = std::sqrt(2) / 2 * sin((45 + fmod(std::fabs(thetaA), 90)) * PI / 180);

	std::map<Point, std::map<Point, ColorElement, Comp>, Comp> pointInfo;

	Point subP0 = { p0.x * SSLevel , p0.y * SSLevel };
	Point subPr = { pr.x * SSLevel , pr.y * SSLevel };
	for (int i = 0; i < colorArray._h * SSLevel; i++)
	{
		for (int j = 0; j < colorArray._w * SSLevel; j++)
		{
			float x = subP0.x + j;
			float y = subP0.y + i;
			auto _p = rotatePoint({ x, y }, subPr, theta);
			for (int _i = std::ceil(_p.y - d); _i <= std::floor(_p.y + d); _i++)
			{
				for (int _j = std::ceil(_p.x - d); _j <= std::floor(_p.x + d); _j++)
				{
					Point testP = rotatePoint({ (float)_j, (float)_i }, subPr, -theta);
					if (pointInRect(testP, x - 0.5f, y - 0.5f, 1, 1))
					{
						pointInfo[{(float)(_j / SSLevel), (float)(_i / SSLevel) }][{(float)_j, (float)_i}] = colorArray[i / SSLevel][j / SSLevel];
					}
				}
			}
		}
	}

	int squareSS = SSLevel * SSLevel;
	int sumR = 0;
	int sumG = 0;
	int sumB = 0;
	for (auto& p : pointInfo)
	{
		sumR = 0;
		sumG = 0;
		sumB = 0;
		for (auto& _p : p.second)
		{
			sumR += _p.second._r;
			sumG += _p.second._g;
			sumB += _p.second._b;
		}
		drawPoint(p.first, ColorElement(sumR / squareSS, sumG / squareSS, sumB / squareSS));
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glColor3f(1.0, 1.0, 1.0);

	ColorArray colorArray(100, 80);
	for (int i = 0; i < colorArray._h / 2; i++)
	{
		for (int j = 0; j < colorArray._w / 2; j++)
			colorArray[i][j] = ColorElement(0xFF, 0xFF, 0xFF);
	}
	for (int i = 0; i < colorArray._h / 2; i++)
	{
		for (int j = colorArray._w / 2; j < colorArray._w; j++)
			colorArray[i][j] = ColorElement(0xFF, 0x00, 0x00);
	}
	for (int i = colorArray._h / 2; i < colorArray._h; i++)
	{
		for (int j = 0; j < colorArray._w / 2; j++)
			colorArray[i][j] = ColorElement(0x00, 0xFF, 0x00);
	}
	for (int i = colorArray._h / 2; i < colorArray._h; i++)
	{
		for (int j = colorArray._w / 2; j < colorArray._w; j++)
			colorArray[i][j] = ColorElement(0x00, 0x00, 0xFF);
	}
	drawPixels(50, 300, colorArray);

	rotateByDestCenter({ 230, 470 }, colorArray, { 200, 470 }, 0 * PI / 180);
	rotateByDestCenter({ 490, 450 }, colorArray, { 450, 450 }, 90 * PI / 180);
	rotateByDestCenter({ 530, 450 }, colorArray, { 500, 450 }, 30 * PI / 180);
	rotateByDestCenter({ 830, 500 }, colorArray, { 800, 500 }, 135 * PI / 180);

	rotateBySourceCenter({ 230, 320 }, colorArray, { 200, 320 }, 0 * PI / 180);
	rotateBySourceCenter({ 490, 300 }, colorArray, { 450, 300 }, 90 * PI / 180);
	rotateBySourceCenter({ 530, 300 }, colorArray, { 500, 300 }, 30 * PI / 180);
	rotateBySourceCenter({ 830, 350 }, colorArray, { 800, 350 }, 135 * PI / 180);

	rotateSSBySC({ 230, 170 }, colorArray, { 200, 170 }, 0 * PI / 180 , 4);
	rotateSSBySC({ 490, 150 }, colorArray, { 450, 150 }, 90 * PI / 180, 4);
	rotateSSBySC({ 530, 150 }, colorArray, { 500, 150 }, 30 * PI / 180, 4);
	rotateSSBySC({ 830, 200 }, colorArray, { 800, 200 }, 135 * PI / 180, 4);

	rotateSSByDC({ 230, 20 }, colorArray, { 200, 20 }, 0 * PI / 180, 4);
	rotateSSByDC({ 490, 0 }, colorArray, { 450, 0 }, 90 * PI / 180, 4);
	rotateSSByDC({ 530, 0 }, colorArray, { 500, 0 }, 30 * PI / 180, 4);
	rotateSSByDC({ 830, 50 }, colorArray, { 800, 50 }, 135 * PI / 180, 4);

	glFlush();
}
void code_7_exercise_25()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_26
struct Point { float x; float y; };
// 判断浮点数相等
bool Equal(float f1, float f2) { return std::abs(f1 - f2) < 0.0001; }
struct Comp
{
	bool operator()(const Point& p1, const Point& p2) const 
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
};
struct ColorElement
{
	ColorElement() {};
	ColorElement(GLubyte r, GLubyte g, GLubyte b) : _r(r), _g(g), _b(b) {};
	GLubyte _r = 0x00;
	GLubyte _g = 0x00;
	GLubyte _b = 0x00;
};
struct ColorArray
{
	ColorArray(int w, int h)
	{
		_w = w;
		_h = h;
		_data.assign(_h, std::vector<ColorElement>(_w, ColorElement()));
		_arrayData.assign(_w * _h * 3, 0x00);
	}
	std::vector<ColorElement>& operator [](int h)
	{
		return _data[h];
	}
	operator GLubyte* ()
	{
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w; j++)
			{
				int pos = (i * _w + j) * 3;
				_arrayData[pos] = _data[i][j]._r;
				_arrayData[pos + 1] = _data[i][j]._g;
				_arrayData[pos + 2] = _data[i][j]._b;
			}
		}
		return &_arrayData[0];
	}
	int _w;
	int _h;
	std::vector<std::vector<ColorElement>> _data;
	std::vector<GLubyte> _arrayData;
};
struct CompColor
{
	bool operator()(const ColorElement& c1, const ColorElement& c2) const
	{
		if (c1._r == c2._r)
		{
			if (c1._g == c2._g)
			{
				return c1._b < c2._b;
			}
			else
			{
				return c1._g < c2._g;
			}
		}
		else
		{
			return c1._r < c2._r;
		}
	}
};
void drawPixels(float x, float y, ColorArray& colorArray)
{
	glRasterPos2i(x, y);
	glDrawPixels(colorArray._w, colorArray._h, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
}
void drawPoint(Point p, ColorElement c)
{
	glRasterPos2i(p.x, p.y);
	static GLubyte a[3];
	a[0] = c._r;
	a[1] = c._g;
	a[2] = c._b;
	glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, a);

	//static char aa[64] = {};
	//sprintf_s(aa, "%d, %d	%d, %d, %d\n", (int)p.x, (int)p.y, (int)a[0], (int)a[1], (int)a[2]);
	//outPut(aa);
}
bool pointInRect(Point p, float x0, float y0, float w, float h)
{
	return p.x >= x0 && p.x <= x0 + w && p.y >= y0 && p.y <= y0 + h;
}
Point rotatePoint(Point p, Point pr, float theta)
{
	Point ret;
	ret.x = p.x * std::cos(theta) - p.y * std::sin(theta) + pr.x * (1 - std::cos(theta)) + pr.y * std::sin(theta);
	ret.y = p.x * std::sin(theta) + p.y * std::cos(theta) + pr.y * (1 - std::cos(theta)) - pr.x * std::sin(theta);
	return ret;
}
void scaleRealStencil(Point p0, ColorArray& colorArray, Point pr, float sx, float sy);
void scale(Point p0, ColorArray& colorArray, Point pr, float sx, float sy)
{
	if (std::abs(sx) >= 1 && std::abs(sy) >= 1)
	{
		// 目标像素中心点在源像素区域中
		Point sp0;
		sp0.x = sx * p0.x + pr.x * (1 - sx);
		sp0.y = sy * p0.y + pr.y * (1 - sy);
		float nextX = sp0.x - sx / 2;
		float nextY = sp0.y - sy / 2;
		float curX, curY;
		int beginX, endX, beginY, endY;
		for (int i = 0; i < colorArray._h; i++)
		{
			curY = nextY;
			nextY = curY + sy;
			beginY = sy > 0 ? std::ceil(curY) : std::ceil(nextY);
			endY = sy > 0 ? std::floor(nextY) : std::floor(curY);
			nextX = sp0.x - sx / 2;
			for (int j = 0; j < colorArray._w; j++)
			{
				curX = nextX;				
				nextX = curX + sx;
				beginX = sx > 0 ? std::ceil(curX) : std::ceil(nextX);
				endX = sx > 0 ? std::floor(nextX) : std::floor(curX);
				for (int _i = beginY; _i <= endY; _i++)
				{
					for (int _j = beginX; _j <= endX; _j++)
					{
						drawPoint({ (float)_j, (float)_i }, colorArray[i][j]);
					}
				}
			}
		}
	}
	else if (std::abs(sx) <= 1 && std::abs(sy) <= 1)
	{
		// 源像素中心点在目标像素区域中
		Point sp0;
		sp0.x = sx * p0.x + pr.x * (1 - sx);
		sp0.y = sy * p0.y + pr.y * (1 - sy);
		float curX = sp0.x;
		float curY = sp0.y;
		std::map<Point, std::map<Point, ColorElement, Comp>, Comp> pointInfo;
		for (int i = 0; i < colorArray._h; i++)
		{
			curX = sp0.x;
			for (int j = 0; j < colorArray._w; j++)
			{
				pointInfo[{std::round(curX), std::round(curY)}][{curX, curY}] = colorArray[i][j];
				curX += sx;
			}
			curY += sy;
		}

		int count = 0;
		int sumR = 0;
		int sumG = 0;
		int sumB = 0;
		for (auto& p : pointInfo)
		{
			count = p.second.size();
			sumR = 0;
			sumG = 0;
			sumB = 0;
			for (auto& _p : p.second)
			{
				sumR += _p.second._r;
				sumG += _p.second._g;
				sumB += _p.second._b;
			}
			drawPoint(p.first, ColorElement(sumR / count, sumG / count, sumB / count));
		}
	}
	else
	{
		scaleRealStencil(p0, colorArray, pr, sx, sy);
	}
}
// 超采样
void scaleSS(Point p0, ColorArray& colorArray, Point pr, float sx, float sy, int SSLevel)
{
	float minPer = max(std::abs(1 / sx), std::abs(1 / sy));
	if (SSLevel < minPer)
		SSLevel = std::ceil(minPer);

	std::map<Point, std::map<Point, ColorElement, Comp>, Comp> pointInfo;

	Point subP0 = { p0.x * SSLevel , p0.y * SSLevel };
	Point subPr = { pr.x * SSLevel , pr.y * SSLevel };

	Point sp0;
	sp0.x = sx * subP0.x + subPr.x * (1 - sx);
	sp0.y = sy * subP0.y + subPr.y * (1 - sy);
	float nextX = sp0.x - sx / 2 * SSLevel;
	float nextY = sp0.y - sy / 2 * SSLevel;
	float curX, curY;
	int beginX, endX, beginY, endY;
	for (int i = 0; i < colorArray._h; i++)
	{
		curY = nextY;
		nextY = curY + sy * SSLevel;
		beginY = sy > 0 ? std::ceil(curY) : std::ceil(nextY);
		endY = sy > 0 ? std::floor(nextY) : std::floor(curY);
		nextX = sp0.x - sx / 2 * SSLevel;
		for (int j = 0; j < colorArray._w; j++)
		{
			curX = nextX;
			nextX = curX + sx * SSLevel;
			beginX = sx > 0 ? std::ceil(curX) : std::ceil(nextX);
			endX = sx > 0 ? std::floor(nextX) : std::floor(curX);
			for (int _i = beginY; _i <= endY; _i++)
			{
				for (int _j = beginX; _j <= endX; _j++)
				{
					pointInfo[{floor((float)_j / SSLevel), floor((float)_i / SSLevel)}][{(float)_j, (float)_i}] = colorArray[i][j];
				}
			}
		}
	}

	int count = SSLevel * SSLevel;
	int sumR = 0;
	int sumG = 0;
	int sumB = 0;
	for (auto& p : pointInfo)
	{
		sumR = 0;
		sumG = 0;
		sumB = 0;
		for (auto& _p : p.second)
		{
			sumR += _p.second._r;
			sumG += _p.second._g;
			sumB += _p.second._b;
		}
		drawPoint(p.first, ColorElement(sumR / count, sumG / count, sumB / count));
	}
}
float rectAndArea(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	auto w = min(x1 + w1, x2 + w2) - max(x2, x1);
	if (w <= 0) return 0;
	auto h = min(y1 + h1, y2 + h2) - max(y2, y1);
	if (h <= 0) return 0;
	return w * h;
}
// 真实宽高计算
void scaleReal(Point p0, ColorArray& colorArray, Point pr, float sx, float sy)
{
	Point sp0;
	sp0.x = sx * p0.x + pr.x * (1 - sx);
	sp0.y = sy * p0.y + pr.y * (1 - sy);
	float nextX = sp0.x - sx / 2;
	float nextY = sp0.y - sy / 2;
	float curX, curY;

	std::map<Point, std::map<ColorElement, float, CompColor>, Comp> pointInfo;

	float w, h;
	int beginX, endX, beginY, endY;
	for (int i = 0; i < colorArray._h; i++)
	{
		curY = nextY;
		nextY = curY + sy;
		beginY = sy > 0 ? std::round(curY) : std::round(nextY);
		endY = sy > 0 ? std::round(nextY) : std::round(curY);
		for (int _i = beginY; _i <= endY; _i++)
		{
			if (beginY == endY)
			{
				h = std::abs(sy);
			}
			else if (_i == beginY)
			{
				if (sy > 0)
					h = beginY + 0.5 - curY;
				else
					h = beginY + 0.5 - nextY;
			}
			else if (_i == endY)
			{
				if (sy > 0)
					h = nextY - (endY - 0.5);
				else
					h = curY - (endY - 0.5);
			}
			else
			{
				h = 1;
			}
			nextX = sp0.x - sx / 2;
			for (int j = 0; j < colorArray._w; j++)
			{
				curX = nextX;
				nextX = curX + sx;
				beginX = sx > 0 ? std::round(curX) : std::round(nextX);
				endX = sx > 0 ? std::round(nextX) : std::round(curX);
				for (int _j = beginX; _j <= endX; _j++)
				{
					if (beginX == endX)
					{
						w = std::abs(sx);
					}
					else if (_j == beginX)
					{
						if (sx > 0)
							w = beginX + 0.5 - curX;
						else
							w = beginX + 0.5 - nextX;
					}
					else if (_j == endX)
					{
						if (sx > 0)
							w = nextX - (endX - 0.5);
						else
							w = curX - (endX - 0.5);
					}
					else
					{
						w = 1;
					}
					pointInfo[{(float)_j, (float)_i}][colorArray[i][j]] += w * h;
				}
			}
		}
	}

	float sumR = 0;
	float sumG = 0;
	float sumB = 0;
	for (auto& p : pointInfo)
	{
		sumR = 0;
		sumG = 0;
		sumB = 0;
		for (auto& _c : p.second)
		{
			sumR += _c.first._r * _c.second;
			sumG += _c.first._g * _c.second;
			sumB += _c.first._b * _c.second;
		}
		drawPoint(p.first, ColorElement(std::round(sumR), std::round(sumG), std::round(sumB)));
	}
}
// 真实模板计算
void scaleRealStencil(Point p0, ColorArray& colorArray, Point pr, float sx, float sy)
{
	Point sp0;
	sp0.x = sx * p0.x + pr.x * (1 - sx);
	sp0.y = sy * p0.y + pr.y * (1 - sy);
	float nextX = sp0.x - sx / 2;
	float nextY = sp0.y - sy / 2;
	float curX, curY;

	std::map<Point, std::map<Point, float, Comp>, Comp> pointStencil;
	int stencilX, stencilY;
	float stencilRealX, stencilRealY;
	std::map<Point, std::map<ColorElement, float, CompColor>, Comp> pointInfo;

	float w, h;
	int beginX, endX, beginY, endY;
	for (int i = 0; i < colorArray._h; i++)
	{
		stencilY = i + 1;
		curY = nextY;
		nextY = curY + sy;
		stencilRealY = nextY - (sp0.y - sy / 2);
		beginY = sy > 0 ? std::round(curY) : std::round(nextY);
		endY = sy > 0 ? std::round(nextY) : std::round(curY);
		for (int _i = beginY; _i <= endY; _i++)
		{
			if (beginY == endY)
			{
				h = std::abs(sy);
			}
			else if (_i == beginY)
			{
				if (sy > 0)
					h = beginY + 0.5 - curY;
				else
					h = beginY + 0.5 - nextY;
			}
			else if (_i == endY)
			{
				if (sy > 0)
					h = nextY - (endY - 0.5);
				else
					h = curY - (endY - 0.5);
			}
			else
			{
				h = 1;
			}
			nextX = sp0.x - sx / 2;
			for (int j = 0; j < colorArray._w; j++)
			{
				stencilX = j + 1;
				curX = nextX;
				nextX = curX + sx;
				stencilRealX = nextX - (sp0.x - sx / 2);
				beginX = sx > 0 ? std::round(curX) : std::round(nextX);
				endX = sx > 0 ? std::round(nextX) : std::round(curX);
				for (int _j = beginX; _j <= endX; _j++)
				{
					if (beginX == endX)
					{
						w = std::abs(sx);
					}
					else if (_j == beginX)
					{
						if (sx > 0)
							w = beginX + 0.5 - curX;
						else
							w = beginX + 0.5 - nextX;
					}
					else if (_j == endX)
					{
						if (sx > 0)
							w = nextX - (endX - 0.5);
						else
							w = curX - (endX - 0.5);
					}
					else
					{
						w = 1;
					}
					pointStencil[{(float)j, (float)i}][{(float)_j, (float)_i}] = w * h;
				}
				if (Equal(stencilRealX - std::round(stencilRealX), 0))
				{
					break;
				}
			}
		}
		if (Equal(stencilRealY - std::round(stencilRealY), 0))
		{			
			break;
		}
	}
	stencilRealX = std::round(stencilRealX);
	stencilRealY = std::round(stencilRealY);

	for (int i = 0; i < colorArray._h; i++)
	{
		for (int j = 0; j < colorArray._w; j++)
		{
			Point p = { j % stencilX, i % stencilY };
			int nX = j / stencilX;
			int nY = i / stencilY;
			for (auto & realP : pointStencil[p])
			{
				pointInfo[{realP.first.x + nX * stencilRealX, realP.first.y + nY * stencilRealY}][colorArray[i][j]] += realP.second;
			}
		}
	}

	float sumR = 0;
	float sumG = 0;
	float sumB = 0;
	for (auto& p : pointInfo)
	{
		sumR = 0;
		sumG = 0;
		sumB = 0;
		for (auto& _c : p.second)
		{
			sumR += _c.first._r * _c.second;
			sumG += _c.first._g * _c.second;
			sumB += _c.first._b * _c.second;
		}
		drawPoint(p.first, ColorElement(std::round(sumR), std::round(sumG), std::round(sumB)));
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glColor3f(1.0, 1.0, 1.0);

	ColorArray colorArray(100, 60);
	for (int i = 0; i < colorArray._h / 2; i++)
	{
		for (int j = 0; j < colorArray._w / 2; j++)
			colorArray[i][j] = ColorElement(
				j % 3 == 0 ? 0xFF : 0x00,
				j % 3 == 1 ? 0xFF : 0x00,
				j % 3 == 2 ? 0xFF : 0x00);
	}
	for (int i = 0; i < colorArray._h / 2; i++)
	{
		for (int j = colorArray._w / 2; j < colorArray._w; j++)
			colorArray[i][j] = ColorElement(0xFF, 0x00, 0x00);
	}
	for (int i = colorArray._h / 2; i < colorArray._h; i++)
	{
		for (int j = 0; j < colorArray._w / 2; j++)
			colorArray[i][j] = ColorElement(0x00, 0xFF, 0x00);
	}
	for (int i = colorArray._h / 2; i < colorArray._h; i++)
	{
		for (int j = colorArray._w / 2; j < colorArray._w; j++)
			colorArray[i][j] = ColorElement(0x00, 0x00, 0xFF);
	}
	drawPixels(20, 300, colorArray);
	glPixelZoom(0.5, 0.5);
	drawPixels(20, 250, colorArray);
	glPixelZoom(1.0, 1.0);

	scale({ 130, 430 }, colorArray, { 110, 400 }, 2.f, 2.f);
	scale({ 380, 430 }, colorArray, { 360, 400 }, 0.73f, 1.85f);
	scale({ 660, 630 }, colorArray, { 640, 600 }, -1.5f, -1.5f);
	scale({ 820, 520 }, colorArray, { 810, 500 }, -1.8f, 0.5f);

	scaleSS({ 130, 280 }, colorArray, { 110, 250 }, 2.f, 2.f, 4);
	scaleSS({ 380, 280 }, colorArray, { 360, 250 }, 0.73f, 1.85f, 4);
	scaleSS({ 660, 480 }, colorArray, { 640, 450 }, -1.5f, -1.5f, 4);
	scaleSS({ 820, 370 }, colorArray, { 810, 350 }, -1.8f, 0.5f, 4);

	auto startTime = GetTickCount();
	scaleReal({ 130, 130 }, colorArray, { 110, 100 }, 2.f, 2.f);
	scaleReal({ 380, 130 }, colorArray, { 360, 100 }, 0.73f, 1.85f);
	scaleReal({ 660, 330 }, colorArray, { 640, 300 }, -1.5f, -1.5f);
	scaleReal({ 820, 220 }, colorArray, { 810, 200 }, -1.8f, 0.5f);
	auto endTime = GetTickCount();
	printf("%d ms\n", endTime - startTime);

	startTime = GetTickCount();
	scaleRealStencil({ 130, -20 }, colorArray, { 110, -50 }, 2.f, 2.f);
	scaleRealStencil({ 380, -20 }, colorArray, { 360, -50 }, 0.73f, 1.85f);
	scaleRealStencil({ 660, 180 }, colorArray, { 640, 150 }, -1.5f, -1.5f);
	scaleRealStencil({ 820, 70 }, colorArray, { 810, 50 }, -1.8f, 0.5f);
	endTime = GetTickCount();
	printf("%d ms\n", endTime - startTime);



	glFlush();
}

void code_7_exercise_26()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_27
struct Point { float x; float y; };
std::vector<Point> rect1 = { { 90, -10 },{ 110, -10 },{ 110, 10 },{ 90, 10 } };
std::vector<Point> rect2 = { { -10, -10 },{ 10, -10 },{ 10, 10 },{ -10, 10 } };
int curAngle = 0;
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0.0, 0.0, 0.0);

	// 旋转-平移
	glLoadIdentity();
	glTranslatef(150.f, 300.f, 0.f);
	drawCoordinate();
	glRotated(curAngle, 0, 0, 1);
	drawPolygon(rect1);

	auto x = std::cos(curAngle * PI / 180) * 100;
	auto y = std::sin(curAngle * PI / 180) * 100;
	// 旋转-基于正方形中心的旋转（正方向）-平移
	glLoadIdentity();
	glTranslatef(400.f, 300.f, 0.f);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glRotated(-curAngle, 0, 0, 1);
	glTranslatef(-x, -y, 0.f);
	glRotated(curAngle, 0, 0, 1);
	drawPolygon(rect1);

	// 直接按圆路径平移
	glLoadIdentity();
	glTranslatef(650.f, 300.f, 0.f);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	drawPolygon(rect2);

	glFlush();
}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	curAngle -= 360 / 60;
	if (curAngle <= 0)
		curAngle = 360;
	displayFcn();
}
void code_7_exercise_27()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	SetTimer(NULL, NULL, 1000, onTimer);

	curAngle = 90;
	
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_28
struct Point { float x; float y; };
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
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
std::vector<Point> rect1 = { { 90, -10 },{ 110, -10 },{ 110, 10 },{ 90, 10 } };
std::vector<Point> rect2 = { { -10, -10 },{ 10, -10 },{ 10, 10 },{ -10, 10 } };
int curAngle = 0;
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void translate(float tx, float ty)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][3] = tx;
	m[1][3] = ty;
	glMultMatrixf(m);
}
void rotate(Point pr, float theta)
{
	theta = theta * PI / 180;
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][0] = std::cos(theta);
	m[0][1] = -std::sin(theta);
	m[0][3] = pr.x * (1 - std::cos(theta)) + pr.y * std::sin(theta);
	m[1][0] = std::sin(theta);
	m[1][1] = std::cos(theta);
	m[1][3] = pr.y * (1 - std::cos(theta)) - pr.x * std::sin(theta);
	glMultMatrixf(m);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0.0, 0.0, 0.0);

	// 旋转-平移
	glLoadIdentity();
	translate(150.f, 300.f);
	drawCoordinate();
	glRotated(curAngle, 0, 0, 1);
	drawPolygon(rect1);

	float x = std::cos(curAngle * PI / 180) * 100;
	float y = std::sin(curAngle * PI / 180) * 100;
	// 旋转-基于正方形中心的旋转（正方向）-平移
	glLoadIdentity();
	translate(400.f, 300.f);
	drawCoordinate();
	rotate({x, y}, -curAngle);
	rotate({0.f, 0.f}, curAngle);
	drawPolygon(rect1);

	// 直接按圆路径平移
	glLoadIdentity();
	translate(650.f, 300.f);
	drawCoordinate();
	translate(x, y);
	drawPolygon(rect2);

	glFlush();
}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	curAngle -= 360 / 60;
	if (curAngle <= 0)
		curAngle = 360;
	displayFcn();
}
void code_7_exercise_28()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	SetTimer(NULL, NULL, 1000, onTimer);

	curAngle = 90;

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_29
struct Point { float x; float y; };
std::vector<Point> rect1_1 = { { 90, -10 },{ 100, -10 },{ 100, 0 },{ 90, 0 } };
std::vector<Point> rect1_2 = { { 100, -10 },{ 110, -10 },{ 110, 0 },{ 100, 0 } };
std::vector<Point> rect1_3 = { { 100, 0 },{ 110, 0 },{ 110, 10 },{ 100, 10 } };
std::vector<Point> rect1_4 = { { 90, 0 },{ 100, 0 },{ 100, 10 },{ 90, 10 } };
std::vector<Point> rect2_1 = { { -10, -10 },{ 0, -10 },{ 0, 0 },{ -10, 0 } };
std::vector<Point> rect2_2 = { { 0, -10 },{ 10, -10 },{ 10, 0 },{ 0, 0 } };
std::vector<Point> rect2_3 = { { 0, 0 },{ 10, 0 },{ 10, 10 },{ 0, 10 } };
std::vector<Point> rect2_4 = { { -10, 0 },{ 0, 0 },{ 0, 10 },{ -10, 10 } };
int curAngle = 0;
float selfAngle = 0;
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}	
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	auto x = std::cos(curAngle * PI / 180) * 100;
	auto y = std::sin(curAngle * PI / 180) * 100;

	// 旋转-基于正方形中心的顺时针自旋转-平移
	selfAngle = (float)(curAngle % 90) / 90 * 360;
	glLoadIdentity();
	glTranslatef(200.f, 450.f, 0.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glRotated(selfAngle - curAngle, 0, 0, 1);
	glTranslatef(-x, -y, 0.f);
	glRotated(curAngle, 0, 0, 1);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect1_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect1_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect1_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect1_4);

	// 旋转-基于正方形中心的逆时针自旋转-平移
	selfAngle = 360 - ((float)(curAngle % 90) / 90 * 360);
	glLoadIdentity();
	glTranslatef(600.f, 450.f, 0.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glRotated(selfAngle - curAngle, 0, 0, 1);
	glTranslatef(-x, -y, 0.f);
	glRotated(curAngle, 0, 0, 1);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect1_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect1_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect1_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect1_4);

	// 基于正方形中心的顺时针自旋转-直接按圆路径平移
	selfAngle = (float)(curAngle % 90) / 90 * 360;
	glLoadIdentity();
	glTranslatef(200.f, 150.f, 0.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glRotated(selfAngle, 0, 0, 1);
	
	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect2_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect2_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect2_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect2_4);

	// 基于正方形中心的逆时针自旋转-直接按圆路径平移
	selfAngle = 360 - ((float)(curAngle % 90) / 90 * 360);
	glLoadIdentity();
	glTranslatef(600.f, 150.f, 0.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glRotated(selfAngle, 0, 0, 1);
	
	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect2_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect2_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect2_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect2_4);

	glFlush();
}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	curAngle -= 360 / 60;
	if (curAngle <= 0)
		curAngle = 360;
	displayFcn();
}
void code_7_exercise_29()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	SetTimer(NULL, NULL, 1000, onTimer);

	curAngle = 90;

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_30
struct Point { float x; float y; };
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
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
std::vector<Point> rect1_1 = { { 90, -10 },{ 100, -10 },{ 100, 0 },{ 90, 0 } };
std::vector<Point> rect1_2 = { { 100, -10 },{ 110, -10 },{ 110, 0 },{ 100, 0 } };
std::vector<Point> rect1_3 = { { 100, 0 },{ 110, 0 },{ 110, 10 },{ 100, 10 } };
std::vector<Point> rect1_4 = { { 90, 0 },{ 100, 0 },{ 100, 10 },{ 90, 10 } };
std::vector<Point> rect2_1 = { { -10, -10 },{ 0, -10 },{ 0, 0 },{ -10, 0 } };
std::vector<Point> rect2_2 = { { 0, -10 },{ 10, -10 },{ 10, 0 },{ 0, 0 } };
std::vector<Point> rect2_3 = { { 0, 0 },{ 10, 0 },{ 10, 10 },{ 0, 10 } };
std::vector<Point> rect2_4 = { { -10, 0 },{ 0, 0 },{ 0, 10 },{ -10, 10 } };
int curAngle = 0;
float selfAngle = 0;
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void translate(float tx, float ty)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][3] = tx;
	m[1][3] = ty;
	glMultMatrixf(m);
}
void rotate(Point pr, float theta)
{
	theta = theta * PI / 180;
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][0] = std::cos(theta);
	m[0][1] = -std::sin(theta);
	m[0][3] = pr.x * (1 - std::cos(theta)) + pr.y * std::sin(theta);
	m[1][0] = std::sin(theta);
	m[1][1] = std::cos(theta);
	m[1][3] = pr.y * (1 - std::cos(theta)) - pr.x * std::sin(theta);
	glMultMatrixf(m);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	float x = std::cos(curAngle * PI / 180) * 100;
	float y = std::sin(curAngle * PI / 180) * 100;

	// 旋转-基于正方形中心的顺时针自旋转-平移
	selfAngle = (float)(curAngle % 90) / 90 * 360;
	glLoadIdentity();
	translate(200.f, 450.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	rotate({ x, y }, selfAngle - curAngle);
	rotate({ 0.f, 0.f }, curAngle);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect1_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect1_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect1_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect1_4);

	// 旋转-基于正方形中心的逆时针自旋转-平移
	selfAngle = 360 - ((float)(curAngle % 90) / 90 * 360);
	glLoadIdentity();
	translate(600.f, 450.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	rotate({ x, y }, selfAngle - curAngle);
	rotate({ 0.f, 0.f }, curAngle);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect1_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect1_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect1_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect1_4);

	// 基于正方形中心的顺时针自旋转-直接按圆路径平移
	selfAngle = (float)(curAngle % 90) / 90 * 360;
	glLoadIdentity();
	translate(200.f, 150.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	translate(x, y);
	rotate({ 0.f, 0.f }, selfAngle);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect2_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect2_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect2_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect2_4);

	// 基于正方形中心的逆时针自旋转-直接按圆路径平移
	selfAngle = 360 - ((float)(curAngle % 90) / 90 * 360);
	glLoadIdentity();
	translate(600.f, 150.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	translate(x, y);
	rotate({ 0.f, 0.f }, selfAngle);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect2_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect2_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect2_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect2_4);

	glFlush();
}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	curAngle -= 360 / 60;
	if (curAngle <= 0)
		curAngle = 360;
	displayFcn();
}
void code_7_exercise_30()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	SetTimer(NULL, NULL, 1000, onTimer);

	curAngle = 90;

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_31
struct Point { float x; float y; };
std::vector<Point> rect1_1 = { { 90, -10 },{ 100, -10 },{ 100, 0 },{ 90, 0 } };
std::vector<Point> rect1_2 = { { 100, -10 },{ 110, -10 },{ 110, 0 },{ 100, 0 } };
std::vector<Point> rect1_3 = { { 100, 0 },{ 110, 0 },{ 110, 10 },{ 100, 10 } };
std::vector<Point> rect1_4 = { { 90, 0 },{ 100, 0 },{ 100, 10 },{ 90, 10 } };
std::vector<Point> rect2_1 = { { -10, -10 },{ 0, -10 },{ 0, 0 },{ -10, 0 } };
std::vector<Point> rect2_2 = { { 0, -10 },{ 10, -10 },{ 10, 0 },{ 0, 0 } };
std::vector<Point> rect2_3 = { { 0, 0 },{ 10, 0 },{ 10, 10 },{ 0, 10 } };
std::vector<Point> rect2_4 = { { -10, 0 },{ 0, 0 },{ 0, 10 },{ -10, 10 } };
int curAngle = 0;
float selfAngle = 0;
float selfScale = 0;
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	auto x = std::cos(curAngle * PI / 180) * 100;
	auto y = std::sin(curAngle * PI / 180) * 100;

	selfScale = ((float)((curAngle - 1) % 90 + 1) / 90  + 1) / 2;

	// 旋转-基于正方形中心的顺时针自旋转-基于正方形中心的缩放-平移
	selfAngle = (float)(curAngle % 90) / 90 * 360;
	glLoadIdentity();
	glTranslatef(200.f, 450.f, 0.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glScaled(selfScale, selfScale, 1.0);
	glRotated(selfAngle - curAngle, 0, 0, 1);
	glTranslatef(-x, -y, 0.f);
	glRotated(curAngle, 0, 0, 1);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect1_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect1_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect1_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect1_4);

	// 旋转-基于正方形中心的逆时针自旋转-基于正方形中心的缩放-平移
	selfAngle = 360 - ((float)(curAngle % 90) / 90 * 360);
	glLoadIdentity();
	glTranslatef(600.f, 450.f, 0.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glScaled(selfScale, selfScale, 1.0);
	glRotated(selfAngle - curAngle, 0, 0, 1);
	glTranslatef(-x, -y, 0.f);
	glRotated(curAngle, 0, 0, 1);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect1_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect1_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect1_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect1_4);

	// 基于正方形中心的顺时针自旋转-基于正方形中心的缩放-按圆路径平移
	selfAngle = (float)(curAngle % 90) / 90 * 360;
	glLoadIdentity();
	glTranslatef(200.f, 150.f, 0.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glScaled(selfScale, selfScale, 1.0);
	glRotated(selfAngle, 0, 0, 1);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect2_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect2_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect2_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect2_4);

	// 基于正方形中心的逆时针自旋转-基于正方形中心的缩放-按圆路径平移
	selfAngle = 360 - ((float)(curAngle % 90) / 90 * 360);
	glLoadIdentity();
	glTranslatef(600.f, 150.f, 0.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	glTranslatef(x, y, 0.f);
	glScaled(selfScale, selfScale, 1.0);
	glRotated(selfAngle, 0, 0, 1);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect2_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect2_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect2_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect2_4);

	glFlush();
}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	curAngle -= 360 / 60;
	if (curAngle <= 0)
		curAngle = 360;
	displayFcn();
}
void code_7_exercise_31()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	SetTimer(NULL, NULL, 1000, onTimer);

	curAngle = 90;

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_32
struct Point { float x; float y; };
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
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
std::vector<Point> rect1_1 = { { 90, -10 },{ 100, -10 },{ 100, 0 },{ 90, 0 } };
std::vector<Point> rect1_2 = { { 100, -10 },{ 110, -10 },{ 110, 0 },{ 100, 0 } };
std::vector<Point> rect1_3 = { { 100, 0 },{ 110, 0 },{ 110, 10 },{ 100, 10 } };
std::vector<Point> rect1_4 = { { 90, 0 },{ 100, 0 },{ 100, 10 },{ 90, 10 } };
std::vector<Point> rect2_1 = { { -10, -10 },{ 0, -10 },{ 0, 0 },{ -10, 0 } };
std::vector<Point> rect2_2 = { { 0, -10 },{ 10, -10 },{ 10, 0 },{ 0, 0 } };
std::vector<Point> rect2_3 = { { 0, 0 },{ 10, 0 },{ 10, 10 },{ 0, 10 } };
std::vector<Point> rect2_4 = { { -10, 0 },{ 0, 0 },{ 0, 10 },{ -10, 10 } };
int curAngle = 0;
float selfAngle = 0;
float selfScale = 0;
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex2i(-winWidth / 2, 0);
	glVertex2i(winWidth / 2, 0);
	glVertex2i(0, -winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}
void translate(float tx, float ty)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][3] = tx;
	m[1][3] = ty;
	glMultMatrixf(m);
}
void rotate(Point pr, float theta)
{
	theta = theta * PI / 180;
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][0] = std::cos(theta);
	m[0][1] = -std::sin(theta);
	m[0][3] = pr.x * (1 - std::cos(theta)) + pr.y * std::sin(theta);
	m[1][0] = std::sin(theta);
	m[1][1] = std::cos(theta);
	m[1][3] = pr.y * (1 - std::cos(theta)) - pr.x * std::sin(theta);
	glMultMatrixf(m);
}
void scale(Point pr, float sx, float sy)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][0] = sx;
	m[0][3] = pr.x * (1 - sx);
	m[1][1] = sy;
	m[1][3] = pr.y * (1 - sy);
	glMultMatrixf(m);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	float x = std::cos(curAngle * PI / 180) * 100;
	float y = std::sin(curAngle * PI / 180) * 100;

	selfScale = ((float)((curAngle - 1) % 90 + 1) / 90 + 1) / 2;

	// 旋转-基于正方形中心的顺时针自旋转-基于正方形中心的缩放-平移
	selfAngle = (float)(curAngle % 90) / 90 * 360;
	glLoadIdentity();
	translate(200.f, 450.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	scale({ x, y }, selfScale, selfScale);
	rotate({ x, y }, selfAngle - curAngle);
	rotate({ 0.f, 0.f }, curAngle);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect1_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect1_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect1_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect1_4);

	// 旋转-基于正方形中心的逆时针自旋转-基于正方形中心的缩放-平移
	selfAngle = 360 - ((float)(curAngle % 90) / 90 * 360);
	glLoadIdentity();
	translate(600.f, 450.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	scale({ x, y }, selfScale, selfScale);
	rotate({ x, y }, selfAngle - curAngle);
	rotate({ 0.f, 0.f }, curAngle);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect1_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect1_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect1_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect1_4);

	// 基于正方形中心的顺时针自旋转-基于正方形中心的缩放-按圆路径平移
	selfAngle = (float)(curAngle % 90) / 90 * 360;
	glLoadIdentity();
	translate(200.f, 150.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	translate(x, y);
	scale({ 0.f, 0.f }, selfScale, selfScale);
	rotate({ 0.f, 0.f }, selfAngle);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect2_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect2_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect2_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect2_4);

	// 基于正方形中心的逆时针自旋转-基于正方形中心的缩放-按圆路径平移
	selfAngle = 360 - ((float)(curAngle % 90) / 90 * 360);
	glLoadIdentity();
	translate(600.f, 150.f);
	glColor3f(0.0, 0.0, 0.0);
	drawCoordinate();
	translate(x, y);
	scale({ 0.f, 0.f }, selfScale, selfScale);
	rotate({ 0.f, 0.f }, selfAngle);

	glColor3f(0.0, 0.0, 0.0);
	drawPolygon(rect2_1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(rect2_2);
	glColor3f(0.0, 1.0, 0.0);
	drawPolygon(rect2_3);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(rect2_4);

	glFlush();

}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	curAngle -= 360 / 60;
	if (curAngle <= 0)
		curAngle = 360;
	displayFcn();
}
void code_7_exercise_32()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	SetTimer(NULL, NULL, 100, onTimer);

	curAngle = 90;

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_ADD_1
struct Point { float x; float y; };
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
	std::vector<std::vector<float>> _data;
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
struct Path
{
	struct PathLine
	{
		Point begin;
		Point end;
		float sin;
		float cos;
		float length;
	};
	std::vector<PathLine> _pathLines;
	float curLength;
	int curPathIdx;
	Path(const std::vector<Point>& points)
	{
		curLength = 0.f;
		curPathIdx = 0;
		for (int i = 0 ; i < points.size() - 1; i ++)
		{
			float dx = points[i + 1].x - points[i].x;
			float dy = points[i + 1].y - points[i].y;
			float length = std::sqrt(dx * dx + dy * dy);
			_pathLines.push_back({ points[i], points[i + 1], dy / length, dx / length, length });
		}
	}
	Point move(float distance)
	{
		curLength += distance;
		float dMaxLength = curLength - _pathLines[curPathIdx].length;
		float dMinLength = curLength - 0;
		if (dMaxLength > 0)
		{
			curPathIdx = (curPathIdx + 1) % _pathLines.size();
			curLength = 0;
			return move(dMaxLength);
		}
		else if (dMinLength < 0)
		{
			curPathIdx = (curPathIdx - 1 + _pathLines.size()) % _pathLines.size();
			curLength = _pathLines[curPathIdx].length;
			return move(dMinLength);
		}
		return{ _pathLines[curPathIdx].begin.x + curLength * _pathLines[curPathIdx].cos, _pathLines[curPathIdx].begin.y + curLength * _pathLines[curPathIdx].sin };
	}
};
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
Matrix translateMatrix(float tx, float ty)
{
	// 平移
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][2] = tx;
	ret[1][2] = ty;
	return ret;
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	return ret;
}
Matrix rotateByPointMatrix(Point p, float theta)
{
	// 基于指定点旋转
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[0][2] = p.x * (1 - std::cos(theta)) + p.y * std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	ret[1][2] = p.y * (1 - std::cos(theta)) - p.x * std::sin(theta);
	return ret;
}
Matrix scaleMatrix(float sx, float sy)
{
	// 基于原点缩放
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[1][1] = sy;
	return ret;
}
Matrix scaleByPointMatrix(Point p, float sx, float sy)
{
	// 基于指定点缩放
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[0][2] = p.x * (1 - sx);
	ret[1][1] = sy;
	ret[1][2] = p.y * (1 - sy);
	return ret;
}
Matrix shearXMatrix(float shx)
{
	// 基于原点的x方向错切
	Matrix ret(3, 3);
	matrixSetIdentity(ret);
	ret[0][1] = shx;
	return ret;
}
void transformPoint(Matrix& m, Point& point)
{
	Matrix _point(3, 1);
	Matrix temp(3, 1);
	_point[0][0] = point.x;
	_point[1][0] = point.y;
	_point[2][0] = 1;
	temp = m * _point;
	point.x = temp[0][0];
	point.y = temp[1][0];
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
		temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
	}
}
std::vector<Point> road = { { -400, 0 }, { 0, 0 },{ 1000, 0 },{ 1200, 150 },{ 1600, 150 },{ 2000, 0 },{ 2600, 0 }, { 2600, 200 },{ 2800, 200 } ,{ 2800, 0 } ,{ 3200, 0 },{ 3600, 0 } };
std::vector<Point> car = { {0, 0}, {-70, 0}, {-70, -12},{30, -12},{30, -2},{15, 26},{0, 26} };
std::vector<Point> goods = { { -30, -20 },{ 30, -20},{ 30, 20 },{ -30, 20 }};
Point wheelPoint = { 0, 0 };
std::vector<Point> wheelHolder;

std::vector<Point> curCar;
std::vector<Point> curGoods;
Point curWheelPoint1;
Point curWheelPoint2;
std::vector<Point> curWheel1;
std::vector<Point> curWheel2;
std::vector<Point> curWheelRoundHolder;
std::vector<Point> curWheelHolder1;
std::vector<Point> curWheelHolder2;

float FPS = 60;
int deltaFPS = 5;
float speed = 100;
float deltaSpeed = 10;
float scaleX = 1;
float deltaScaleX = 2;
float scaleY = 1;
float deltaScaleY = 2;
float shx = 0;
float tansShx = 0;
float deltaShear = 1;

int lastTick = 0;
float delta = 0.0;
Point curPosition;
float curDirection;
float wheelRadius = 0;
Path* path = nullptr;
void initPath()
{
	auto temp = road;
	temp.erase(temp.begin());
	temp.erase(temp.end() - 1);
	path = new Path(temp);
}
void drawPoint(Point p)
{
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}
void drawPoints(const std::vector<Point>& points)
{
	glBegin(GL_POINTS);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawStrip(const std::vector<Point>& points)
{
	glBegin(GL_LINE_STRIP);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawLoop(const std::vector<Point>& points)
{
	glBegin(GL_LINE_LOOP);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void circlePlot(Point p0, Point p, std::vector<Point>& points)
{
	points.push_back({p0.x + p.x, p0.y + p.y});
	points.push_back({p0.x - p.x, p0.y + p.y});
	points.push_back({p0.x + p.x, p0.y - p.y});
	points.push_back({p0.x - p.x, p0.y - p.y});
	points.push_back({p0.x + p.y, p0.y + p.x});
	points.push_back({p0.x - p.y, p0.y + p.x});
	points.push_back({p0.x + p.y, p0.y - p.x});
	points.push_back({p0.x - p.y, p0.y - p.x});
}									 
void circle(Point p0, float r, std::vector<Point>& points)
{
	points.clear();
	float x = r;
	float y = 0;
	int d2x = 2 * r;
	int d2y = 0;
	int p = 1 - r;
	circlePlot(p0, { x, y }, points);
	while (x > y)
	{
		y++;
		d2y += 2;
		if (p < 0)
		{
			p += d2y + 1;
		}
		else
		{
			x--;
			d2x -= 2;
			p += d2y + 1 - d2x;
		}
		circlePlot(p0, { x, y }, points);
	}
}
inline int64_t Round(const double a)
{
	if (a >= 0)
		return int64_t(a + 0.5);
	else
		return int64_t(a - 0.5);
}
void ellipsePlot(Point p0, int x, int y, std::vector<Point>& points)
{
	points.push_back({ p0.x + x, p0.y + y });
	points.push_back({ p0.x - x, p0.y + y });
	points.push_back({ p0.x + x, p0.y - y });
	points.push_back({ p0.x - x, p0.y - y });
}
void ellipse(Point p0, int Rx, int Ry, std::vector<Point>& points)
{
	points.clear();
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int64_t p;
	int x = 0;
	int y = Ry;
	int64_t px = 0;
	int64_t py = twoRx2*y;
	ellipsePlot(p0, x, y, points);
	/*Region 1*/
	//p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	p = Round(Ry2 - (int64_t)(Rx2*Ry) + (0.25*Rx2));
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
		ellipsePlot(p0, x, y, points);
	}
	/*Region 2*/
	//p = Round(Ry2*(x + 0.5)*(x + 0.5) + Rx2*(y - 1)*(y - 1) - Rx2*Ry2);
	p = Round((int64_t)Ry2*(x + 0.5)*(x + 0.5) + (int64_t)Rx2*(y - 1)*(y - 1) - (int64_t)Rx2*Ry2);
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
		ellipsePlot(p0, x, y, points);
	}
}
void drawRoad()
{
	drawStrip(road);
}
void drawCar()
{
	drawLoop(curCar);
}
void drawGoods()
{
	drawLoop(curGoods);
}
void drawWheel()
{
	drawPoints(curWheel1);
	for (auto & p : curWheelHolder1)
	{
		drawStrip({ curWheelPoint1, p});
	}
	drawPoints(curWheel2);
	for (auto & p : curWheelHolder2)
	{
		drawStrip({ curWheelPoint2, p });
	}
}
void scale(float sx, float sy)
{
	scaleX *= sx;
	scaleY *= sy;
	auto m = rotateByPointMatrix(curPosition, curDirection) * scaleByPointMatrix(curPosition, sx, sy) * rotateByPointMatrix(curPosition, -curDirection);
	transformPoints(m, curCar);
	transformPoints(m, curGoods);
	transformPoint(m, curWheelPoint1);
	transformPoint(m, curWheelPoint2);
	transformPoints(m, curWheelHolder1);
	transformPoints(m, curWheelHolder2);

	ellipse({ 0, 0 }, std::abs(scaleX) * wheelRadius, std::abs(scaleY) * wheelRadius, curWheel1);
	// 先斜切再缩放，斜切参数将变为原始的sx/sy倍，可以对比先斜切再缩放和先缩放再斜切的矩阵得出此结论
	tansShx *= (sx / sy);
	transformPoints(rotateByPointMatrix({0, 0}, curDirection) * shearXMatrix(tansShx), curWheel1);
	curWheel2 = curWheel1;
	transformPoints(translateMatrix(curWheelPoint1.x, curWheelPoint1.y), curWheel1);
	transformPoints(translateMatrix(curWheelPoint2.x, curWheelPoint2.y), curWheel2);
}
void shear(float sh)
{
	shx += sh;
	tansShx += sh;
	auto m = translateMatrix(curPosition.x, curPosition.y) * rotateMatrix(curDirection) * shearXMatrix(sh) * rotateMatrix(-curDirection) * translateMatrix(-curPosition.x, -curPosition.y);
	transformPoints(m, curCar);
	transformPoints(m, curGoods);
	transformPoint(m, curWheelPoint1);
	transformPoint(m, curWheelPoint2);
	transformPoints(m, curWheelHolder1);
	transformPoints(m, curWheelHolder2);
	transformPoints(m, curWheel1);
	transformPoints(m, curWheel2);
}
void reset()
{
	FPS = 60;
	deltaFPS = 5;
	speed = 100;
	deltaSpeed = 10;
	scale(1 / scaleX, 1 / scaleY);
	deltaScaleX = 2;
	deltaScaleY = 2;
	shear(-tansShx);
	deltaShear = 1;
}
void initWheel()
{
	for (int i = 0; i <= 4; i++)
	{
		float angle = (90 + i * 360 / 5) * PI / 180;
		wheelHolder.push_back({ std::cos(angle) * wheelRadius, std::sin(angle) * wheelRadius });
	}
}
void initCarData()
{
	initPath();
	lastTick = GetTickCount();
	curPosition = { 0.f, 0.f };
	curDirection = 0.f;
	wheelRadius = 10;
	initWheel();
	curCar = car;
	curGoods = goods;
	curWheelPoint1 = wheelPoint;
	curWheelPoint2 = wheelPoint;
	curWheelHolder1 = wheelHolder;
	curWheelHolder2 = wheelHolder;
	curWheelRoundHolder = wheelHolder;
	transformPoints(translateMatrix(20, 22), curCar);
	transformPoints(translateMatrix(-15, 47), curGoods);
	transformPoint(translateMatrix(32, 10), curWheelPoint1);
	transformPoint(translateMatrix(-32, 10), curWheelPoint2);
	transformPoints(translateMatrix(32, 10), curWheelHolder1);
	transformPoints(translateMatrix(-32, 10), curWheelHolder2);
	circle({ 32, 10 }, wheelRadius, curWheel1);
	circle({ -32, 10 }, wheelRadius, curWheel2);
}
float distance(const Point& p1, const Point& p2)
{
	auto dx = p2.x - p1.x;
	auto dy = p2.y - p1.y;
	return std::sqrt(dx * dx + dy * dy);
}
void updateWindowPosition()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(curPosition.x - winWidth / 2, curPosition.x + winWidth / 2, -150, -150 + winHeight);
}
void updateDirection(float diretion)
{
	auto d = diretion - curDirection;
	transformPoints(rotateByPointMatrix(curPosition, d), curCar);
	transformPoints(rotateByPointMatrix(curPosition, d), curGoods);
	transformPoint(rotateByPointMatrix(curPosition, d), curWheelPoint1);
	transformPoint(rotateByPointMatrix(curPosition, d), curWheelPoint2);
	transformPoints(rotateByPointMatrix(curPosition, d), curWheel1);
	transformPoints(rotateByPointMatrix(curPosition, d), curWheel2);

	transformPoints(translateMatrix(curWheelPoint1.x, curWheelPoint1.y), curWheelHolder1);
	transformPoints(translateMatrix(curWheelPoint2.x, curWheelPoint2.y), curWheelHolder2);
	transformPoints(rotateByPointMatrix(curWheelPoint1, diretion), curWheelHolder1);
	transformPoints(rotateByPointMatrix(curWheelPoint2, diretion), curWheelHolder2);
}
void updateMove(Point p)
{
	auto dx = p.x - curPosition.x;
	auto dy = p.y - curPosition.y;
	transformPoints(translateMatrix(dx, dy), curCar);
	transformPoints(translateMatrix(dx, dy), curGoods);
	transformPoint(translateMatrix(dx, dy), curWheelPoint1);
	transformPoint(translateMatrix(dx, dy), curWheelPoint2);
	transformPoints(translateMatrix(dx, dy), curWheel1);
	transformPoints(translateMatrix(dx, dy), curWheel2);
}
void updateTransform()
{
	auto l = std::sqrt(tansShx * tansShx + 1) * wheelRadius * std::abs(scaleY);
	
	float deltaA = delta  * speed / l;
	transformPoints(rotateByPointMatrix({0, 0}, -deltaA), curWheelRoundHolder);
	curWheelHolder1 = curWheelRoundHolder;
	transformPoints(shearXMatrix(tansShx) * scaleByPointMatrix({ 0, 0 }, scaleX, scaleY), curWheelHolder1);
	curWheelHolder2 = curWheelHolder1;
}
void update()
{
	//printf("update: %f\n", delta);

	Point nextP = path->move(delta * speed);
	auto dx = nextP.x - curPosition.x;
	auto dy = nextP.y - curPosition.y;
	float dir = 0;
	if (dx)
	{
		dir = std::atan(dy / dx);
	}
	else
	{
		if (dy)
		{
			auto signDy = dy > 0 ? 1 : -1;
			auto signSpeed = speed > 0 ? 1 : -1;
			dir = signDy * signSpeed * PI / 2;
		}
		else
		{
			dir = curDirection;
		}
	}		

	updateTransform();
	updateMove(nextP);
	curPosition = nextP;
	updateDirection(dir);
	curDirection = dir;
	
	updateWindowPosition();
}
bool isNumber(string s)
{
	std::stringstream sin(s);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}
bool setFPS(int value)
{
	if (value <= 0)
	{
		return false;
	}
	else
	{
		FPS = value;
		return true;
	}
}
void dealFPSCommand()
{
	printf("input fps value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("fps must be a number!!!\n");
		dealFPSCommand();
		return;
	}
	int value = std::stoi(str);
	if (!setFPS(value))
	{
		printf("fps must be a positive number!!!\n");
		dealFPSCommand();
	}
}
void dealDeltaFPSCommand()
{
	printf("input fps delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("fps delta must be a number!!!\n");
		dealDeltaFPSCommand();
		return;
	}
	int value = std::stoi(str);
	deltaFPS = value;
}
void dealSpeedCommand()
{
	printf("input speed value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("speed must be a number!!!\n");
		dealSpeedCommand();
		return;
	}
	float value = std::stof(str);
	speed = value;
}
void dealDeltaSpeedCommand()
{
	printf("input speed delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("speed delta must be a number!!!\n");
		dealDeltaSpeedCommand();
		return;
	}
	float value = std::stof(str);
	deltaSpeed = value;
}
void dealScaleXCommand()
{
	printf("input scale x value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("scale x must be a number!!!\n");
		dealScaleXCommand();
		return;
	}
	float value = std::stof(str);
	scale(value / scaleX, 1);
}
void dealDeltaScaleXCommand()
{
	printf("input scale x delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("scale x delta must be a number!!!\n");
		dealDeltaScaleXCommand();
		return;
	}
	float value = std::stof(str);
	deltaScaleX = value;
}
void dealScaleYCommand()
{
	printf("input scale y value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("scale y must be a number!!!\n");
		dealScaleYCommand();
		return;
	}
	float value = std::stof(str);
	scale(1, value / scaleY);
}
void dealDeltaScaleYCommand()
{
	printf("input scale y delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("scale y delta must be a number!!!\n");
		dealDeltaScaleYCommand();
		return;
	}
	float value = std::stof(str);
	deltaScaleY = value;
}
void dealShearCommand()
{
	printf("input shear value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("shear must be a number!!!\n");
		dealShearCommand();
		return;
	}
	float value = std::stof(str);
	shear(value - tansShx);
}
void dealDeltaShearCommand()
{
	printf("input shear delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("shear delta must be a number!!!\n");
		dealDeltaShearCommand();
		return;
	}
	float value = std::stof(str);
	deltaShear = value;
}

void showOperatorNotice();
bool _dealCommand(string commandString)
{
	if (commandString == "fps" || commandString == "FPS")
	{
		dealFPSCommand();
		return true;
	}
	else if (commandString == "dfps" || commandString == "DFPS")
	{
		dealDeltaFPSCommand();
		return true;
	}
	else if (commandString == "speed" || commandString == "SPEED")
	{
		dealSpeedCommand();
		return true;
	}
	else if (commandString == "dspeed" || commandString == "DSPEED")
	{
		dealDeltaSpeedCommand();
		return true;
	}
	else if (commandString == "scalex" || commandString == "SCALEX")
	{
		dealScaleXCommand();
		return true;
	}
	else if (commandString == "dscalex" || commandString == "DSCALEX")
	{
		dealDeltaScaleXCommand();
		return true;
	}
	else if (commandString == "scaley" || commandString == "SCALEY")
	{
		dealScaleYCommand();
		return true;
	}
	else if (commandString == "dscaley" || commandString == "DSCALEY")
	{
		dealDeltaScaleYCommand();
		return true;
	}
	else if (commandString == "shear" || commandString == "SHEAR")
	{
		dealShearCommand();
		return true;
	}
	else if (commandString == "dshear" || commandString == "DSHEAR")
	{
		dealDeltaShearCommand();
		return true;
	}
	else if (commandString == "reset" || commandString == "RESET")
	{
		reset();
		return true;
	}
	else if (commandString == "exit" || commandString == "EXIT")
	{
		return true;
	}
	else
	{
		return false;
	}
}
void dealCommand()
{
	printf("Input Command: ");
	std::string commandString;
	std::cin >> commandString;
	if (!_dealCommand(commandString))
	{
		printf("error command!!!\n");
		dealCommand();
		return;
	}
	showOperatorNotice();
	lastTick = GetTickCount();
}
void drawString(Point point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void showState()
{
	float leftX = curPosition.x - winWidth / 2 + 10;
	float topY = -150 + winHeight;
	int space = 20;
	char s[128] = {};
		
	int realFPS = 0;
	if (delta)
		realFPS = 1 / delta;
	sprintf_s(s, "FPS: %d", realFPS);	
	drawString({ leftX, topY - space * 1 }, s);

	sprintf_s(s, "position: %.02f, %.02f", curPosition.x, curPosition.y);
	drawString({ leftX, topY - space * 2 }, s);

	sprintf_s(s, "speed: %.02f pixel/s", speed);
	drawString({ leftX, topY - space * 3 }, s);

	float angle = curDirection * 180 / PI;
	sprintf_s(s, "angle: %.02f", angle);
	drawString({ leftX, topY - space * 4 }, s);

	sprintf_s(s, "scale x: %.02f", scaleX);
	drawString({ leftX, topY - space * 5 }, s);

	sprintf_s(s, "scale y: %.02f", scaleY);
	drawString({ leftX, topY - space * 6 }, s);

	sprintf_s(s, "shear: %.02f", tansShx);
	drawString({ leftX, topY - space * 7 }, s);
}
void showOperatorNotice()
{
	system("cls");
	printf("Operator: \n");
	printf("W: add FPS.\n");
	printf("S: decrease FPS.\n");
	printf("D: add speed.\n");
	printf("A: decrease speed.\n");
	printf("→: add scale x.\n");
	printf("←: decrease scale x.\n");
	printf("↑: add scale y.\n");
	printf("↓: decrease scale y.\n");
	printf("CTRL →: reflect to +x.\n");
	printf("CTRL ←: reflect to -x.\n");
	printf("CTRL ↑: reflect to +y.\n");
	printf("CTRL ↓: reflect to -y.\n");
	printf("SHIFT →: add shear.\n");
	printf("SHIFT ←: decrease shear.\n");
	printf("R: reset state\n");
	printf("ESC: command mode.\n");
}
void showCommandNotice()
{	
	system("cls");
	printf("Command Mode: \n");
	printf("fps: set FPS.\n");
	printf("dfps: set FPS delta.\n");
	printf("speed: set speed.\n");
	printf("dspeed: set speed delta.\n");
	printf("scalex: set scale x.\n");
	printf("dscalex: set scale x delta.\n");
	printf("scaley: set scale y.\n");
	printf("dscaley: set scale y delta.\n");
	printf("shear: set shear.\n");
	printf("dshear: set shear delta.\n");
	printf("reset: reset state.\n");
	printf("exit: exit command mode.\n");
	dealCommand();
}
void normalKeyFcn(unsigned char key, int x, int y)
{
	//printf("normalKeyFcn %d, %d, %d\n", key, x, y);
	switch (key)
	{
	case 27:
		showCommandNotice();
		break;
	case 'r':
	case 'R':
		reset();
		break;
	case 'w':
	case 'W':
		setFPS(FPS + deltaFPS);
		break;
	case 's':
	case 'S':
		setFPS(FPS - deltaFPS);
		break;
	case 'a':
	case 'A':
		speed -= deltaSpeed;
		break;
	case 'd':
	case 'D':
		speed += deltaSpeed;
		break;
	default:
		break;
	}
	
}
void specialKeyFcn(int key, int x, int y)
{
	//printf("specialKeyFcn %d, %d, %d\n", key, x, y);
	int mod = glutGetModifiers();
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if (mod == 0)
		{
			scale(deltaScaleX, 1);
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (scaleX < 0)
				scale(-1, 1);
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			shear(deltaShear);
		}
		break;
	case GLUT_KEY_LEFT:
		if (mod == 0)
		{
			scale(1 / deltaScaleX, 1);
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (scaleX > 0)
				scale(-1, 1);
		}			
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			shear(-deltaShear);
		}
		break;
	case GLUT_KEY_UP:
		if (mod == 0)
		{
			scale(1, deltaScaleY);
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (scaleY < 0)
				scale(1, -1);
		}
		break;
	case GLUT_KEY_DOWN:
		if (mod == 0)
		{
			scale(1, 1 / deltaScaleY);
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (scaleY > 0)
				scale(1, -1);
		}
		break;
	default:
		break;
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	update();

	drawRoad();
	drawCar();
	drawGoods();
	drawWheel();
	showState();

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
void code_7_exercise_add_1()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);

	initCarData();
	showOperatorNotice();

	glutDisplayFunc(displayFcn);
	glutKeyboardFunc(normalKeyFcn);
	glutSpecialFunc(specialKeyFcn);

	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
#endif

#ifdef CHAPTER_7_EXERCISE_ADD_2
struct Point { float x; float y; };
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
struct Path
{
	struct PathLine
	{
		Point begin;
		Point end;
		float sin;
		float cos;
		float length;
	};
	std::vector<PathLine> _pathLines;
	float curLength;
	int curPathIdx;
	Path(const std::vector<Point>& points)
	{
		curLength = 0.f;
		curPathIdx = 0;
		for (int i = 0; i < points.size() - 1; i++)
		{
			float dx = points[i + 1].x - points[i].x;
			float dy = points[i + 1].y - points[i].y;
			float length = std::sqrt(dx * dx + dy * dy);
			_pathLines.push_back({ points[i], points[i + 1], dy / length, dx / length, length });
		}
	}
	Point move(float distance)
	{
		curLength += distance;
		float dMaxLength = curLength - _pathLines[curPathIdx].length;
		float dMinLength = curLength - 0;
		if (dMaxLength > 0)
		{
			curPathIdx = (curPathIdx + 1) % _pathLines.size();
			curLength = 0;
			return move(dMaxLength);
		}
		else if (dMinLength < 0)
		{
			curPathIdx = (curPathIdx - 1 + _pathLines.size()) % _pathLines.size();
			curLength = _pathLines[curPathIdx].length;
			return move(dMinLength);
		}
		return{ _pathLines[curPathIdx].begin.x + curLength * _pathLines[curPathIdx].cos, _pathLines[curPathIdx].begin.y + curLength * _pathLines[curPathIdx].sin };
	}
};
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
Matrix scaleMatrix(float sx, float sy)
{
	// 基于原点缩放
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[1][1] = sy;
	return ret;
}
Matrix shearXMatrix(float shx)
{
	// 基于原点的x方向错切
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][1] = shx;
	return ret;
}
std::vector<Point> road = { { -400, 0 },{ 0, 0 },{ 1000, 0 },{ 1200, 150 },{ 1600, 150 },{ 2000, 0 },{ 2600, 0 },{ 2600, 200 },{ 2800, 200 } ,{ 2800, 0 } ,{ 3200, 0 },{ 3600, 0 } };
std::vector<Point> car = { { 0, 0 },{ -70, 0 },{ -70, -12 },{ 30, -12 },{ 30, -2 },{ 15, 26 },{ 0, 26 } };
std::vector<Point> goods = { { -30, -20 },{ 30, -20 },{ 30, 20 },{ -30, 20 } };
std::vector<Point> wheelHolder;

float FPS = 60;
int deltaFPS = 5;
float speed = 100;
float deltaSpeed = 10;
float scaleX = 1;
float deltaScaleX = 2;
float scaleY = 1;
float deltaScaleY = 2;
float shx = 0;
float tansShx = 0;
float deltaShear = 1;

int lastTick = 0;
float delta = 0.0;
Point curPosition;
float curDirection;
float wheelRadius = 0;
Path* path = nullptr;
float wheelRadian = 0.f;
Matrix curTransform(4, 4);
void initPath()
{
	auto temp = road;
	temp.erase(temp.begin());
	temp.erase(temp.end() - 1);
	path = new Path(temp);
}
void drawPoint(Point p)
{
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}
void drawPoints(const std::vector<Point>& points)
{
	glBegin(GL_POINTS);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawStrip(const std::vector<Point>& points)
{
	glBegin(GL_LINE_STRIP);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawLoop(const std::vector<Point>& points)
{
	glBegin(GL_LINE_LOOP);
	for (auto & p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
inline int64_t Round(const double a)
{
	if (a >= 0)
		return int64_t(a + 0.5);
	else
		return int64_t(a - 0.5);
}
void ellipsePlot(Point p0, int x, int y)
{
	drawPoint({ p0.x + x, p0.y + y });
	drawPoint({ p0.x - x, p0.y + y });
	drawPoint({ p0.x + x, p0.y - y });
	drawPoint({ p0.x - x, p0.y - y });
}
void ellipse(Point p0, int Rx, int Ry)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int64_t p;
	int x = 0;
	int y = Ry;
	int64_t px = 0;
	int64_t py = twoRx2*y;
	ellipsePlot(p0, x, y);
	/*Region 1*/
	//p = Round(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	p = Round(Ry2 - (int64_t)(Rx2*Ry) + (0.25*Rx2));
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
		ellipsePlot(p0, x, y);
	}
	/*Region 2*/
	//p = Round(Ry2*(x + 0.5)*(x + 0.5) + Rx2*(y - 1)*(y - 1) - Rx2*Ry2);
	p = Round((int64_t)Ry2*(x + 0.5)*(x + 0.5) + (int64_t)Rx2*(y - 1)*(y - 1) - (int64_t)Rx2*Ry2);
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
		ellipsePlot(p0, x, y);
	}
}
void drawRoad()
{
	drawStrip(road);
}
void drawCar()
{
	drawLoop(car);
}
void drawGoods()
{
	drawLoop(goods);
}
void drawWheel()
{
	ellipse({ 0, 0 }, std::abs(scaleX) * wheelRadius, std::abs(scaleY) * wheelRadius);
}
void drawWheelHolder()
{
	for (auto & p : wheelHolder)
	{
		drawStrip({ {0, 0}, p });
	}
}
void scale(float sx, float sy)
{
	scaleX *= sx;
	scaleY *= sy;
	
	// 先斜切再缩放，斜切参数将变为原始的sx/sy倍，可以对比先斜切再缩放和先缩放再斜切的矩阵得出此结论
	tansShx *= (sx / sy);
	curTransform = scaleMatrix(sx, sy) * curTransform;
}
void shear(float sh)
{
	shx += sh;
	tansShx += sh;
	curTransform = shearXMatrix(sh) * curTransform;
}
void reset()
{
	FPS = 60;
	deltaFPS = 5;
	speed = 100;
	deltaSpeed = 10;
	scaleX = 1;
	scaleY = 1;
	deltaScaleX = 2;
	deltaScaleY = 2;
	shx = 0;
	tansShx = 0;
	deltaShear = 1;
	matrixSetIdentity(curTransform);
}
void initWheel()
{
	for (int i = 0; i <= 4; i++)
	{
		float angle = (90 + i * 360 / 5) * PI / 180;
		wheelHolder.push_back({ std::cos(angle) * wheelRadius, std::sin(angle) * wheelRadius });
	}
}
void initCarData()
{
	initPath();
	lastTick = GetTickCount();
	curPosition = { 0.f, 0.f };
	curDirection = 0.f;
	wheelRadius = 10;
	wheelRadian = 0.f;
	initWheel();
	matrixSetIdentity(curTransform);
}
void updateWindowPosition()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(curPosition.x - winWidth / 2, curPosition.x + winWidth / 2, -150, -150 + winHeight);
}
void update()
{
	//printf("update: %f\n", delta);

	Point nextP = path->move(delta * speed);
	auto dx = nextP.x - curPosition.x;
	auto dy = nextP.y - curPosition.y;
	float dir = 0;
	if (dx)
	{
		dir = std::atan(dy / dx);
	}
	else
	{
		if (dy)
		{
			auto signDy = dy > 0 ? 1 : -1;
			auto signSpeed = speed > 0 ? 1 : -1;
			dir = signDy * signSpeed * PI / 2;
		}
		else
		{
			dir = curDirection;
		}
	}
	curPosition = nextP;
	curDirection = dir;

	// gluOrtho2D之后的绘制才会使用新的视口，否则若在gluOrtho2D之前绘制，绘制使用之前的视口，下次新视口才生效
	// 因此第一次绘制的效果是绘制图形移动，但视口不动，即绘制图形超前视口一个delta*speed距离
	// 下次绘制时，会首先生效上次的视口设置，即视口中心为绘制图形中心，但是图形此时又会移动，即绘制图形又会超前视口一个delta*speed距离
	// 这样每帧绘制都会超前视口一个delta*speed距离，因为delta是变量，所以每帧位置都有小偏差，造成视觉残影，绘制抖动
	updateWindowPosition();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawRoad();

	glLoadIdentity();
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(curTransform);
	glTranslated(20, 22, 0);
	drawCar();

	glLoadIdentity();
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(curTransform);
	glTranslated(-15, 47, 0);
	drawGoods();

	auto l = std::sqrt(tansShx * tansShx + 1) * wheelRadius * std::abs(scaleY);
	wheelRadian -= delta * speed / l * 180 / PI;
	wheelRadian = fmod(wheelRadian, 360);

	glLoadIdentity();
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(curTransform);
	glTranslated(32, 10, 0);
	glRotatef(wheelRadian, 0.0, 0.0, 1.0);
	drawWheelHolder();

	glLoadIdentity();
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(curTransform);
	glTranslated(-32, 10, 0);
	glRotatef(wheelRadian, 0.0, 0.0, 1.0);
	drawWheelHolder();
	
	glLoadIdentity();
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(shearXMatrix(tansShx));
	glTranslated(32 * scaleX, 10 * scaleY, 0);
	drawWheel();

	glLoadIdentity();
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(shearXMatrix(tansShx));
	glTranslated(-32 * scaleX, 10 * scaleY, 0);
	drawWheel();
}
bool isNumber(string s)
{
	std::stringstream sin(s);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}
bool setFPS(int value)
{
	if (value <= 0)
	{
		return false;
	}
	else
	{
		FPS = value;
		return true;
	}
}
void dealFPSCommand()
{
	printf("input fps value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("fps must be a number!!!\n");
		dealFPSCommand();
		return;
	}
	int value = std::stoi(str);
	if (!setFPS(value))
	{
		printf("fps must be a positive number!!!\n");
		dealFPSCommand();
	}
}
void dealDeltaFPSCommand()
{
	printf("input fps delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("fps delta must be a number!!!\n");
		dealDeltaFPSCommand();
		return;
	}
	int value = std::stoi(str);
	deltaFPS = value;
}
void dealSpeedCommand()
{
	printf("input speed value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("speed must be a number!!!\n");
		dealSpeedCommand();
		return;
	}
	float value = std::stof(str);
	speed = value;
}
void dealDeltaSpeedCommand()
{
	printf("input speed delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("speed delta must be a number!!!\n");
		dealDeltaSpeedCommand();
		return;
	}
	float value = std::stof(str);
	deltaSpeed = value;
}
void dealScaleXCommand()
{
	printf("input scale x value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("scale x must be a number!!!\n");
		dealScaleXCommand();
		return;
	}
	float value = std::stof(str);
	scale(value / scaleX, 1);
}
void dealDeltaScaleXCommand()
{
	printf("input scale x delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("scale x delta must be a number!!!\n");
		dealDeltaScaleXCommand();
		return;
	}
	float value = std::stof(str);
	deltaScaleX = value;
}
void dealScaleYCommand()
{
	printf("input scale y value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("scale y must be a number!!!\n");
		dealScaleYCommand();
		return;
	}
	float value = std::stof(str);
	scale(1, value / scaleY);
}
void dealDeltaScaleYCommand()
{
	printf("input scale y delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("scale y delta must be a number!!!\n");
		dealDeltaScaleYCommand();
		return;
	}
	float value = std::stof(str);
	deltaScaleY = value;
}
void dealShearCommand()
{
	printf("input shear value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("shear must be a number!!!\n");
		dealShearCommand();
		return;
	}
	float value = std::stof(str);
	shear(value - tansShx);
}
void dealDeltaShearCommand()
{
	printf("input shear delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("shear delta must be a number!!!\n");
		dealDeltaShearCommand();
		return;
	}
	float value = std::stof(str);
	deltaShear = value;
}

void showOperatorNotice();
bool _dealCommand(string commandString)
{
	if (commandString == "fps" || commandString == "FPS")
	{
		dealFPSCommand();
		return true;
	}
	else if (commandString == "dfps" || commandString == "DFPS")
	{
		dealDeltaFPSCommand();
		return true;
	}
	else if (commandString == "speed" || commandString == "SPEED")
	{
		dealSpeedCommand();
		return true;
	}
	else if (commandString == "dspeed" || commandString == "DSPEED")
	{
		dealDeltaSpeedCommand();
		return true;
	}
	else if (commandString == "scalex" || commandString == "SCALEX")
	{
		dealScaleXCommand();
		return true;
	}
	else if (commandString == "dscalex" || commandString == "DSCALEX")
	{
		dealDeltaScaleXCommand();
		return true;
	}
	else if (commandString == "scaley" || commandString == "SCALEY")
	{
		dealScaleYCommand();
		return true;
	}
	else if (commandString == "dscaley" || commandString == "DSCALEY")
	{
		dealDeltaScaleYCommand();
		return true;
	}
	else if (commandString == "shear" || commandString == "SHEAR")
	{
		dealShearCommand();
		return true;
	}
	else if (commandString == "dshear" || commandString == "DSHEAR")
	{
		dealDeltaShearCommand();
		return true;
	}
	else if (commandString == "reset" || commandString == "RESET")
	{
		reset();
		return true;
	}
	else if (commandString == "exit" || commandString == "EXIT")
	{
		return true;
	}
	else
	{
		return false;
	}
}
void dealCommand()
{
	printf("Input Command: ");
	std::string commandString;
	std::cin >> commandString;
	if (!_dealCommand(commandString))
	{
		printf("error command!!!\n");
		dealCommand();
		return;
	}
	showOperatorNotice();
	lastTick = GetTickCount();
}
void drawString(Point point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void showState()
{
	glLoadIdentity();

	float leftX = curPosition.x - winWidth / 2 + 10;
	float topY = -150 + winHeight;
	int space = 20;
	char s[128] = {};

	int realFPS = 0;
	if (delta)
		realFPS = 1 / delta;
	sprintf_s(s, "FPS: %d", realFPS);
	drawString({ leftX, topY - space * 1 }, s);

	sprintf_s(s, "position: %.02f, %.02f", curPosition.x, curPosition.y);
	drawString({ leftX, topY - space * 2 }, s);

	sprintf_s(s, "speed: %.02f pixel/s", speed);
	drawString({ leftX, topY - space * 3 }, s);

	float angle = curDirection * 180 / PI;
	sprintf_s(s, "angle: %.02f", angle);
	drawString({ leftX, topY - space * 4 }, s);

	sprintf_s(s, "scale x: %.02f", scaleX);
	drawString({ leftX, topY - space * 5 }, s);

	sprintf_s(s, "scale y: %.02f", scaleY);
	drawString({ leftX, topY - space * 6 }, s);

	sprintf_s(s, "shear: %.02f", tansShx);
	drawString({ leftX, topY - space * 7 }, s);
}
void showOperatorNotice()
{
	system("cls");
	printf("Operator: \n");
	printf("W: add FPS.\n");
	printf("S: decrease FPS.\n");
	printf("D: add speed.\n");
	printf("A: decrease speed.\n");
	printf("→: add scale x.\n");
	printf("←: decrease scale x.\n");
	printf("↑: add scale y.\n");
	printf("↓: decrease scale y.\n");
	printf("CTRL →: reflect to +x.\n");
	printf("CTRL ←: reflect to -x.\n");
	printf("CTRL ↑: reflect to +y.\n");
	printf("CTRL ↓: reflect to -y.\n");
	printf("SHIFT →: add shear.\n");
	printf("SHIFT ←: decrease shear.\n");
	printf("R: reset state\n");
	printf("ESC: command mode.\n");
}
void showCommandNotice()
{
	system("cls");
	printf("Command Mode: \n");
	printf("fps: set FPS.\n");
	printf("dfps: set FPS delta.\n");
	printf("speed: set speed.\n");
	printf("dspeed: set speed delta.\n");
	printf("scalex: set scale x.\n");
	printf("dscalex: set scale x delta.\n");
	printf("scaley: set scale y.\n");
	printf("dscaley: set scale y delta.\n");
	printf("shear: set shear.\n");
	printf("dshear: set shear delta.\n");
	printf("reset: reset state.\n");
	printf("exit: exit command mode.\n");
	dealCommand();
}
void normalKeyFcn(unsigned char key, int x, int y)
{
	//printf("normalKeyFcn %d, %d, %d\n", key, x, y);
	switch (key)
	{
	case 27:
		showCommandNotice();
		break;
	case 'r':
	case 'R':
		reset();
		break;
	case 'w':
	case 'W':
		setFPS(FPS + deltaFPS);
		break;
	case 's':
	case 'S':CHAPTER_7_EXERCISE_ADD_2
		setFPS(FPS - deltaFPS);
		break;
	case 'a':
	case 'A':
		speed -= deltaSpeed;
		break;
	case 'd':
	case 'D':
		speed += deltaSpeed;
		break;
	default:
		break;
	}

}
void specialKeyFcn(int key, int x, int y)
{
	//printf("specialKeyFcn %d, %d, %d\n", key, x, y);
	int mod = glutGetModifiers();
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if (mod == 0)
		{
			scale(deltaScaleX, 1);
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (scaleX < 0)
				scale(-1, 1);
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			shear(deltaShear);
		}
		break;
	case GLUT_KEY_LEFT:
		if (mod == 0)
		{
			scale(1 / deltaScaleX, 1);
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (scaleX > 0)
				scale(-1, 1);
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			shear(-deltaShear);
		}
		break;
	case GLUT_KEY_UP:
		if (mod == 0)
		{
			scale(1, deltaScaleY);
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (scaleY < 0)
				scale(1, -1);
		}
		break;
	case GLUT_KEY_DOWN:
		if (mod == 0)
		{
			scale(1, 1 / deltaScaleY);
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (scaleY > 0)
				scale(1, -1);
		}
		break;
	default:
		break;
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	update();
	
	showState();

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
void code_7_exercise_add_2()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);

	initCarData();
	showOperatorNotice();

	glutDisplayFunc(displayFcn);
	glutKeyboardFunc(normalKeyFcn);
	glutSpecialFunc(specialKeyFcn);

	glutTimerFunc((unsigned)(1000 / FPS), onTimer, 0);
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_7_COMMON

#ifdef CHAPTER_7_COMMON
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
	int glewCode = glewInit();
	if (glewCode != GLEW_OK)
	{
		printf("glewInit err:%d\n", glewCode);
		return;
	}

	init();

#ifdef CHAPTER_7_1_1
	code_7_1_1();
#endif

#ifdef CHAPTER_7_1_2
	code_7_1_2();
#endif

#ifdef CHAPTER_7_1_3
	code_7_1_3();
#endif

#ifdef CHAPTER_7_4_11
	code_7_4_11();
#endif

#ifdef CHAPTER_7_10
	code_7_10();
#endif

#ifdef CHAPTER_7_EXERCISE_1
	code_7_exercise_1();
#endif

#ifdef CHAPTER_7_EXERCISE_2
	code_7_exercise_2();
#endif

#ifdef CHAPTER_7_EXERCISE_3
	code_7_exercise_3();
#endif

#ifdef CHAPTER_7_EXERCISE_4
	code_7_exercise_4();
#endif

#ifdef CHAPTER_7_EXERCISE_5
	code_7_exercise_5();
#endif

#ifdef CHAPTER_7_EXERCISE_6
	code_7_exercise_6();
#endif

#ifdef CHAPTER_7_EXERCISE_7
	code_7_exercise_7();
#endif

#ifdef CHAPTER_7_EXERCISE_8
	code_7_exercise_8();
#endif

#ifdef CHAPTER_7_EXERCISE_9
	code_7_exercise_9();
#endif

#ifdef CHAPTER_7_EXERCISE_10
	code_7_exercise_10();
#endif

#ifdef CHAPTER_7_EXERCISE_11
	code_7_exercise_11();
#endif

#ifdef CHAPTER_7_EXERCISE_12
	code_7_exercise_12();
#endif

#ifdef CHAPTER_7_EXERCISE_13
	code_7_exercise_13();
#endif

#ifdef CHAPTER_7_EXERCISE_14
	code_7_exercise_14();
#endif

#ifdef CHAPTER_7_EXERCISE_15
	code_7_exercise_15();
#endif

#ifdef CHAPTER_7_EXERCISE_16
	code_7_exercise_16();
#endif

#ifdef CHAPTER_7_EXERCISE_17
	code_7_exercise_17();
#endif

#ifdef CHAPTER_7_EXERCISE_18
	code_7_exercise_18();
#endif

#ifdef CHAPTER_7_EXERCISE_19
	code_7_exercise_19();
#endif

#ifdef CHAPTER_7_EXERCISE_20
	code_7_exercise_20();
#endif

#ifdef CHAPTER_7_EXERCISE_21
	code_7_exercise_21();
#endif

#ifdef CHAPTER_7_EXERCISE_22
	code_7_exercise_22();
#endif

#ifdef CHAPTER_7_EXERCISE_23
	code_7_exercise_23();
#endif

#ifdef CHAPTER_7_EXERCISE_24
	code_7_exercise_24();
#endif

#ifdef CHAPTER_7_EXERCISE_24_1
	code_7_exercise_24_1();
#endif

#ifdef CHAPTER_7_EXERCISE_24_2
	code_7_exercise_24_2();
#endif

#ifdef CHAPTER_7_EXERCISE_25
	code_7_exercise_25();
#endif

#ifdef CHAPTER_7_EXERCISE_26
	code_7_exercise_26();
#endif

#ifdef CHAPTER_7_EXERCISE_27
	code_7_exercise_27();
#endif

#ifdef CHAPTER_7_EXERCISE_28
	code_7_exercise_28();
#endif

#ifdef CHAPTER_7_EXERCISE_29
	code_7_exercise_29();
#endif

#ifdef CHAPTER_7_EXERCISE_30
	code_7_exercise_30();
#endif

#ifdef CHAPTER_7_EXERCISE_31
	code_7_exercise_31();
#endif
	
#ifdef CHAPTER_7_EXERCISE_32
	code_7_exercise_32();
#endif

#ifdef CHAPTER_7_EXERCISE_ADD_1
	code_7_exercise_add_1();
#endif

#ifdef CHAPTER_7_EXERCISE_ADD_2
	code_7_exercise_add_2();
#endif


	glutMainLoop();
}
#endif
#endif 


