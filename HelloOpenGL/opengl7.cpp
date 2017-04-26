#include "common.h"

#ifdef CHAPTER_7
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

	glutMainLoop();
}
#endif
#endif 


