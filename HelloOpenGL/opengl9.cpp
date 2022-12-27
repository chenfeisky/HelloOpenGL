#include "common.h"

#if CHAPTER==9
#include "opengl9h.h"

#ifdef CHAPTER_9_COMMON
GLsizei winWidth = 800, winHeight = 600;

// 判断浮点数相等
inline bool Equal(float f1, float f2) { return std::abs(f1 - f2) < 0.0001; }
inline bool Greater(float f1, float f2) { return Equal(f1, f2) ? false : (f1 > f2); }
inline bool Less(float f1, float f2) { return Equal(f1, f2) ? false : (f1 < f2); }
inline bool GreaterQ(float f1, float f2) { return Greater(f1, f2) || Equal(f1, f2); }
inline bool LessQ(float f1, float f2) { return Less(f1, f2) || Equal(f1, f2); }

// 点，向量
class Point
{
public:
	GLfloat x, y, z;
};
bool operator ==(const Point& p1, const Point& p2)
{
	return Equal(p1.x, p2.x) && Equal(p1.y, p2.y) && Equal(p1.z, p2.z);
}
bool operator!=(const Point& p1, const Point& p2)
{
	return !(p1 == p2);
}

// 矩阵
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
bool operator ==(Matrix& m1, Matrix& m2)
{
	if (!(m1._row == m2._row && m1._col == m2._col))
		return false;

	for (int row = 0; row < m1._row; row++)
	{
		for (int col = 0; col < m1._col; col++)
		{
			if (!Equal(m1[row][col], m2[row][col]))
			{
				return false;
			}
		}
	}
	return true;
}
void transformPoints(Matrix& m, std::vector<Point>& points)
{
	Matrix point(4, 1);
	Matrix temp(4, 1);
	for (auto& p : points)
	{
		point[0][0] = p.x;
		point[1][0] = p.y;
		point[2][0] = p.z;
		point[3][0] = 1;
		auto temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
		p.z = temp[2][0];
	}
}
void transformPoint(Matrix& m, Point& point)
{
	Matrix _point(4, 1);
	Matrix temp(4, 1);
	_point[0][0] = point.x;
	_point[1][0] = point.y;
	_point[2][0] = point.z;
	_point[3][0] = 1;
	temp = m * _point;
	point.x = temp[0][0];
	point.y = temp[1][0];
	point.z = temp[2][0];
}

// 向量
typedef Point Vec3;
Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	return{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}
float dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vec3 operator *(float f, const Vec3& v)
{
	Vec3 ret;
	ret.x = f * v.x;
	ret.y = f * v.y;
	ret.z = f * v.z;
	return ret;
}
Vec3 operator +(const Vec3& v1, const Vec3& v2)
{
	Vec3 ret;
	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y;
	ret.z = v1.z + v2.z;
	return ret;
}
Vec3 operator -(const Vec3& v1, const Vec3& v2)
{
	Vec3 ret;
	ret.x = v1.x - v2.x;
	ret.y = v1.y - v2.y;
	ret.z = v1.z - v2.z;
	return ret;
}
float length(const Vec3& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
void normal(Vec3& v)
{
	float len = length(v);
	v.x = v.x / len;
	v.y = v.y / len;
	v.z = v.z / len;
}

// 计算
Point centerPoint(const std::vector<Point>& points)
{
	Point center;
	int size = points.size();
	float xSum = 0, ySum = 0, zSum = 0;
	for (int k = 0; k < size; k++)
	{
		xSum += points[k].x;
		ySum += points[k].y;
		zSum += points[k].z;
	}
	center.x = xSum / size;
	center.y = ySum / size;
	center.z = zSum / size;
	return center;
}
// 绘制
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}
// 坐标轴
void drawCoordinate(float xStart = -winWidth, float xEnd = winWidth, float yStart = -winHeight, float yEnd = winHeight)
{
	glBegin(GL_LINES);
	glVertex3f(xStart, 0, 0);
	glVertex3f(xEnd, 0, 0);
	glVertex3f(0, yStart, 0);
	glVertex3f(0, yEnd, 0);
	glEnd();
}

#endif

#ifdef CHAPTER_9_1
typedef GLfloat Matrix4x4[4][4];
void matrix4x4SetIdentity(Matrix4x4& matIdent4x4)
{
	GLint row, col;
	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			matIdent4x4[row][col] = (row == col);
}
void translate3D(GLfloat tx, GLfloat ty, GLfloat tz, Matrix4x4& matTransl3D)
{
	matrix4x4SetIdentity(matTransl3D);

	matTransl3D[0][3] = tx;
	matTransl3D[1][3] = ty;
	matTransl3D[2][3] = tz;
}
void transformPoints(Matrix4x4& m, std::vector<Point>& points)
{
	Point temp;
	for (auto& p : points)
	{
		temp.x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3] * 1;
		temp.y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3] * 1;
		temp.z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3] * 1;
		p = temp;
	}
}
void drawSquare(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto& p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> square = { {0, 0, 0}, {100, 0, 0}, {100, 100, 0}, {0, 100, 0} };
	drawSquare(square);

	Matrix4x4 m;
	translate3D(400, 300, 0, m);
	transformPoints(m, square);

	drawSquare(square);

	glFlush();
}
void code_9_1()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_2_3
class wcPt3D
{
public:
	GLfloat x, y, z;
};
typedef float Matrix4x4[4][4];

Matrix4x4 matRot;

void matrix4x4SetIdentity(Matrix4x4& matIdent4x4)
{
	GLint row, col;
	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			matIdent4x4[row][col] = (row == col);
}
void matrix4x4PreMultiply(Matrix4x4& m1, Matrix4x4& m2)
{
	GLint row, col;
	Matrix4x4 matTemp;
	
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col]
				+ m1[row][1] * m2[1][col]
				+ m1[row][2] * m2[2][col]
				+ m1[row][3] * m2[3][col];

		}
	}

	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			m2[row][col] = matTemp[row][col];
		}
	}
}
void translate3D(GLfloat tx, GLfloat ty, GLfloat tz)
{
	Matrix4x4 matTransl3D;

	matrix4x4SetIdentity(matTransl3D);

	matTransl3D[0][3] = tx;
	matTransl3D[1][3] = ty;
	matTransl3D[2][3] = tz;

	matrix4x4PreMultiply(matTransl3D, matRot);
}
void rotate3D(wcPt3D p1, wcPt3D p2, GLfloat radianAngle)
{
	Matrix4x4 matQuaternionRot;
	GLfloat axisVectLength = sqrt((p2.x - p1.x) * (p2.x - p1.x)
		+ (p2.y - p1.y) * (p2.y - p1.y)
		+ (p2.z - p1.z) * (p2.z - p1.z));
	GLfloat cosA = cos(radianAngle);
	GLfloat oneC = 1 - cosA;
	GLfloat sinA = sin(radianAngle);
	GLfloat ux = (p2.x - p1.x) / axisVectLength;
	GLfloat uy = (p2.y - p1.y) / axisVectLength;
	GLfloat uz = (p2.z - p1.z) / axisVectLength;

	translate3D(-p1.x, -p1.y, -p1.z);

	matrix4x4SetIdentity(matQuaternionRot);

	matQuaternionRot[0][0] = ux * ux * oneC + cosA;
	matQuaternionRot[0][1] = ux * uy * oneC - uz * sinA;
	matQuaternionRot[0][2] = ux * uz * oneC + uy * sinA;
	matQuaternionRot[1][0] = uy * ux * oneC + uz * sinA;
	matQuaternionRot[1][1] = uy * uy * oneC + cosA;
	matQuaternionRot[1][2] = uy * uz * oneC - ux * sinA;
	matQuaternionRot[2][0] = uz * ux * oneC - uy * sinA;
	matQuaternionRot[2][1] = uz * uy * oneC + ux * sinA;
	matQuaternionRot[2][2] = uz * uz * oneC + cosA;

	matrix4x4PreMultiply(matQuaternionRot, matRot);

	translate3D(p1.x, p1.y, p1.z);

}
void transformPoints(Matrix4x4& m, std::vector<wcPt3D>& points)
{
	wcPt3D temp;
	for (auto& p : points)
	{
		temp.x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3] * 1;
		temp.y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3] * 1;
		temp.z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3] * 1;
		p = temp;
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<wcPt3D> pt = { { 0, 0, -100 },{ 100, 0, -100 },{100, 100, -100 },{0, 100, -100 }, { 0, 0, 0 },{ 100, 0, 0 },{ 100, 100, 0 },{ 0, 100, 0 } };

	matrix4x4SetIdentity(matRot);
	rotate3D({ 100, 0, -100 }, { 100, 0, 0 }, -PI / 4);
	transformPoints(matRot, pt);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &pt[0]);

	std::vector<wcPt3D> color = { { 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0},{ 1, 0, 0 }, { 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 } };
	glColorPointer(3, GL_FLOAT, 0, &color[0]);
	std::vector<GLubyte> vertIndex = { 4,5,6,7 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 }, { 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 } };
	vertIndex = { 5,1,2,6 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 }, { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
	vertIndex = { 0,4,7,3 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 } };
	vertIndex = { 0,3,2,1 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 1, 1 },{ 1, 1,1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 } };
	vertIndex = { 6,2,3,7 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 } };
	vertIndex = { 0,1,5,4 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);
	
	glFlush();
}
void code_9_2_3()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_3
class wcPt3D
{
private:
	GLfloat x, y, z;
public:
	wcPt3D()
	{
		x = y = z = 0.0;
	}
	void setCoords(GLfloat xCoord, GLfloat yCoord, GLfloat zCoord)
	{
		x = xCoord;
		y = yCoord;
		z = zCoord;
	}
	GLfloat getx() const
	{
		return x;
	}
	GLfloat gety() const
	{
		return y;
	}
	GLfloat getz() const
	{
		return z;
	}
};
typedef float Matrix4x4[4][4];

Matrix4x4 matScale;

void matrix4x4SetIdentity(Matrix4x4& matIdent4x4)
{
	GLint row, col;
	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			matIdent4x4[row][col] = (row == col);
}
void matrix4x4PreMultiply(Matrix4x4& m1, Matrix4x4& m2)
{
	GLint row, col;
	Matrix4x4 matTemp;

	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col]
				+ m1[row][1] * m2[1][col]
				+ m1[row][2] * m2[2][col]
				+ m1[row][3] * m2[3][col];

		}
	}

	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			m2[row][col] = matTemp[row][col];
		}
	}
}
void scale3D(GLfloat sx, GLfloat sy, GLfloat sz, wcPt3D fixedPt)
{
	Matrix4x4 matScale3D;

	matrix4x4SetIdentity(matScale3D);

	matScale3D[0][0] = sx;
	matScale3D[0][3] = (1 - sx) * fixedPt.getx();
	matScale3D[1][1] = sy;
	matScale3D[1][3] = (1 - sy) * fixedPt.gety();
	matScale3D[2][2] = sz;
	matScale3D[2][3] = (1 - sz) * fixedPt.getz();

	matrix4x4PreMultiply(matScale3D, matScale);
}
void transformPoints(Matrix4x4& m, std::vector<wcPt3D>& points)
{
	wcPt3D temp;
	for (auto& p : points)
	{
		p.setCoords(m[0][0] * p.getx() + m[0][1] * p.gety() + m[0][2] * p.getz() + m[0][3] * 1,
			m[1][0] * p.getx() + m[1][1] * p.gety() + m[1][2] * p.getz() + m[1][3] * 1,
			m[2][0] * p.getx() + m[2][1] * p.gety() + m[2][2] * p.getz() + m[2][3] * 1);
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<wcPt3D> pt;
	wcPt3D p;
	p.setCoords(100, 100, -100);
	pt.push_back(p);
	p.setCoords(200, 100, -100);
	pt.push_back(p);
	p.setCoords(200, 200, -100);
	pt.push_back(p);
	p.setCoords(100, 200, -100);
	pt.push_back(p);
	p.setCoords(100, 100, 0);
	pt.push_back(p);
	p.setCoords(200, 100, 0);
	pt.push_back(p);
	p.setCoords(200, 200, 0);
	pt.push_back(p);
	p.setCoords(100, 200, 0);
	pt.push_back(p);

	matrix4x4SetIdentity(matScale);
	p.setCoords(100, 100, 0);
	scale3D(2, 2, 2, p);
	transformPoints(matScale, pt);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &pt[0]);

	struct Point
	{
		GLfloat x, y, z;
	};
	std::vector<Point> color = { { 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 } };
	glColorPointer(3, GL_FLOAT, 0, &color[0]);
	std::vector<GLubyte> vertIndex = { 4,5,6,7 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 } };
	vertIndex = { 5,1,2,6 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
	vertIndex = { 0,4,7,3 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 } };
	vertIndex = { 0,3,2,1 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 1, 1 },{ 1, 1,1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 } };
	vertIndex = { 6,2,3,7 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 } };
	vertIndex = { 0,1,5,4 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	glFlush();
}
void code_9_3()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_4
class wcPt3D
{
public:
	GLfloat x, y, z;
};
typedef float Matrix4x4[4][4];

Matrix4x4 matComposite;

void matrix4x4SetIdentity(Matrix4x4& matIdent4x4)
{
	GLint row, col;
	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			matIdent4x4[row][col] = (row == col);
}
void matrix4x4PreMultiply(Matrix4x4& m1, Matrix4x4& m2)
{
	GLint row, col;
	Matrix4x4 matTemp;

	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col]
				+ m1[row][1] * m2[1][col]
				+ m1[row][2] * m2[2][col]
				+ m1[row][3] * m2[3][col];

		}
	}

	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			m2[row][col] = matTemp[row][col];
		}
	}
}
void translate3D(GLfloat tx, GLfloat ty, GLfloat tz)
{
	Matrix4x4 matTransl3D;

	matrix4x4SetIdentity(matTransl3D);

	matTransl3D[0][3] = tx;
	matTransl3D[1][3] = ty;
	matTransl3D[2][3] = tz;

	matrix4x4PreMultiply(matTransl3D, matComposite);
}
void rotate3D(wcPt3D p1, wcPt3D p2, GLfloat radianAngle)
{
	Matrix4x4 matQuatRot;
	GLfloat axisVectLength = sqrt((p2.x - p1.x) * (p2.x - p1.x)
		+ (p2.y - p1.y) * (p2.y - p1.y)
		+ (p2.z - p1.z) * (p2.z - p1.z));
	GLfloat cosA = cosf(radianAngle);
	GLfloat oneC = 1 - cosA;
	GLfloat sinA = sinf(radianAngle);
	GLfloat ux = (p2.x - p1.x) / axisVectLength;
	GLfloat uy = (p2.y - p1.y) / axisVectLength;
	GLfloat uz = (p2.z - p1.z) / axisVectLength;

	translate3D(-p1.x, -p1.y, -p1.z);

	matrix4x4SetIdentity(matQuatRot);

	matQuatRot[0][0] = ux * ux * oneC + cosA;
	matQuatRot[0][1] = ux * uy * oneC - uz * sinA;
	matQuatRot[0][2] = ux * uz * oneC + uy * sinA;
	matQuatRot[1][0] = uy * ux * oneC + uz * sinA;
	matQuatRot[1][1] = uy * uy * oneC + cosA;
	matQuatRot[1][2] = uy * uz * oneC - ux * sinA;
	matQuatRot[2][0] = uz * ux * oneC - uy * sinA;
	matQuatRot[2][1] = uz * uy * oneC + ux * sinA;
	matQuatRot[2][2] = uz * uz * oneC + cosA;

	matrix4x4PreMultiply(matQuatRot, matComposite);

	translate3D(p1.x, p1.y, p1.z);
}
void scale3D(GLfloat sx, GLfloat sy, GLfloat sz, wcPt3D fixedPt)
{
	Matrix4x4 matScale3D;

	matrix4x4SetIdentity(matScale3D);

	matScale3D[0][0] = sx;
	matScale3D[0][3] = (1 - sx) * fixedPt.x;
	matScale3D[1][1] = sy;
	matScale3D[1][3] = (1 - sy) * fixedPt.y;
	matScale3D[2][2] = sz;
	matScale3D[2][3] = (1 - sz) * fixedPt.z;

	matrix4x4PreMultiply(matScale3D, matComposite);
}
void transformPoints(Matrix4x4& m, std::vector<wcPt3D>& points)
{
	wcPt3D temp;
	for (auto& p : points)
	{
		temp.x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3] * 1;
		temp.y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3] * 1;
		temp.z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3] * 1;
		p = temp;
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<wcPt3D> pt = { { 0, 0, -100 },{ 100, 0, -100 },{ 100, 100, -100 },{ 0, 100, -100 },{ 0, 0, 0 },{ 100, 0, 0 },{ 100, 100, 0 },{ 0, 100, 0 } };

	matrix4x4SetIdentity(matComposite);

	rotate3D({ 50, 50, -100 }, { 50, 50, 0 }, PI / 4);
	scale3D(2, 2, 1, { 50, 50, 0 });
	translate3D(350, 250, 0);

	transformPoints(matComposite, pt);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &pt[0]);

	std::vector<wcPt3D> color = { { 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 } };
	glColorPointer(3, GL_FLOAT, 0, &color[0]);
	std::vector<GLubyte> vertIndex = { 4,5,6,7 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 } };
	vertIndex = { 5,1,2,6 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
	vertIndex = { 0,4,7,3 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 } };
	vertIndex = { 0,3,2,1 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 1, 1 },{ 1, 1,1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 } };
	vertIndex = { 6,2,3,7 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 } };
	vertIndex = { 0,1,5,4 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	glFlush();
}
void code_9_4()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_9_1
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(0.0, 0.0, 1.0);
	glRecti(50, 100, 200, 150);
	
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);

	glTranslatef(-200.0, -50.0, 0.0);
	glRecti(50, 100, 200, 150);

	glPopMatrix();
	glPushMatrix();

	glRotatef(90.0, 0.0, 0.0, 1.0);
	glRecti(50, 100, 200, 150);

	glPopMatrix();
	glScalef(-0.5, 1.0, 1.0);
	glRecti(50, 100, 200, 150);

	glFlush();
}
void code_9_1_1()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-400, 400, -300, 300);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_9_2
class wcPt3D
{
public:
	GLfloat x, y, z;
};
void rotate3D(wcPt3D p1, wcPt3D p2, GLfloat thetaDegrees)
{
	float vx = (p2.x - p1.x);
	float vy = (p2.y - p1.y);
	float vz = (p2.z - p1.z);

	glTranslatef(p1.x, p1.y, p1.z);
	glRotatef(thetaDegrees, vx, vy, vz);
	glTranslatef(-p1.x, -p1.y, -p1.z);
}
void scale3D(GLfloat sx, GLfloat sy, GLfloat sz, wcPt3D fixedPt)
{
	glTranslatef(fixedPt.x, fixedPt.y, fixedPt.z);
	glScalef(sx, sy, sz);
	glTranslatef(-fixedPt.x, -fixedPt.y, -fixedPt.z);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(350, 250, 0);
	scale3D(2, 2, 1, { 50, 50, 0 });
	rotate3D({ 50, 50, -100 }, { 50, 50, 0 }, 45);

	std::vector<wcPt3D> pt = { { 0, 0, -100 },{ 100, 0, -100 },{ 100, 100, -100 },{ 0, 100, -100 },{ 0, 0, 0 },{ 100, 0, 0 },{ 100, 100, 0 },{ 0, 100, 0 } };

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &pt[0]);

	std::vector<wcPt3D> color = { { 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 },{ 1, 0, 0 } };
	glColorPointer(3, GL_FLOAT, 0, &color[0]);
	std::vector<GLubyte> vertIndex = { 4,5,6,7 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 },{ 0, 1, 0 } };
	vertIndex = { 5,1,2,6 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
	vertIndex = { 0,4,7,3 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 },{ 1, 1, 0 } };
	vertIndex = { 0,3,2,1 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 1, 1 },{ 1, 1,1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 } };
	vertIndex = { 6,2,3,7 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	color = { { 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 },{ 1, 0, 1 } };
	vertIndex = { 0,1,5,4 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, &vertIndex[0]);

	glFlush();
}
void code_9_1_2()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_1
Matrix RotateMatrix1(Vec3 u)
{
	Matrix ret(4, 4);
	matrixSetIdentity(ret);

	normal(u);
	Vec3 ux = { 1, 0, 0 };
	Vec3 uy = { 0, 1, 0 };
	Vec3 uz = { 0, 0, 1 };
	
	Vec3 uz_ = u;
	Vec3 uy_ = cross(u, ux);
	normal(uy_);
	Vec3 ux_ = cross(uy_, uz_);

	ret[0][0] = ux_.x;
	ret[0][1] = ux_.y;
	ret[0][2] = ux_.z;

	ret[1][0] = uy_.x;
	ret[1][1] = uy_.y;
	ret[1][2] = uy_.z;

	ret[2][0] = uz_.x;
	ret[2][1] = uz_.y;
	ret[2][2] = uz_.z;

	return ret;
}
Matrix RotateMatrix2(Vec3 u)
{
	Matrix ret(4, 4);
	matrixSetIdentity(ret);

	normal(u);
	
	float a = u.x;
	float b = u.y;
	float c = u.z;
	float d = std::sqrt(b * b + c * c);

	Matrix Rxa(4, 4);
	matrixSetIdentity(Rxa);
	Rxa[1][1] = c / d;
	Rxa[1][2] = -b / d;
	Rxa[2][1] = b / d;
	Rxa[2][2] = c / d;

	Matrix Ryb(4, 4);
	matrixSetIdentity(Ryb);
	Ryb[0][0] = d;
	Ryb[0][2] = -a;
	Ryb[2][0] = a;
	Ryb[2][2] = d;

	ret = Ryb * Rxa;
	return ret;
}
void test()
{
	Vec3 u = {1, 1.5, 0.7};
	Matrix m1 = RotateMatrix1(u);
	Matrix m2 = RotateMatrix2(u);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	u = { 0, 0, 1 };
	m1 = RotateMatrix1(u);
	m2 = RotateMatrix2(u);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	u = { 0, 1, 0};
	m1 = RotateMatrix1(u);
	m2 = RotateMatrix2(u);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");
	
	// 注意这里不能用u=(1,0,0)，因为RotateMatrix2计算d=0,RotateMatrix1计算uy_模长也是0
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	test();

	glFlush();
}
void code_9_exercise_1()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_2
struct Quaternion
{
	float s, i, j, k;
};
Quaternion operator *(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion ret;
	ret.s = q1.s * q2.s - q1.i * q2.i - q1.j * q2.j - q1.k * q2.k;
	ret.i = q1.i * q2.s + q1.s * q2.i + q1.j * q2.k - q1.k * q2.j;
	ret.j = q1.j * q2.s + q1.s * q2.j + q1.k * q2.i - q1.i * q2.k;
	ret.k = q1.k * q2.s + q1.s * q2.k + q1.i * q2.j - q1.j * q2.i;
	return ret;
}
Quaternion operator +(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion ret;
	ret.s = q1.s + q2.s;
	ret.i = q1.i + q2.i;
	ret.j = q1.j + q2.j;
	ret.k = q1.k + q2.k;
	return ret;
}
float length(const Quaternion& q)
{
	return std::sqrt(q.s * q.s + q.i * q.i + q.j * q.j + q.k * q.k);
}
Quaternion inverse(const Quaternion& q)
{
	float l = length(q);
	float f = 1 / (l * l);
	Quaternion ret;
	ret.s = f * q.s;
	ret.i = f * (-q.i);
	ret.j = f * (-q.j);
	ret.k = f * (-q.k);
	return ret;
}
Point rotatePoint1(float s, float a, float b, float c, Point p)
{
	Vec3 v = {a, b, c};
	Vec3 p_ = s * s * p + dot(p, v) * v + 2 * s * cross(v, p) + cross(v, cross(v, p));
	return p_;
}
Point rotatePoint2(float s, float a, float b, float c, Point ponit)
{
	Quaternion ret;

	Quaternion q = { s, a, b, c };
	Quaternion p = { 0, ponit.x, ponit.y, ponit.z };

	Quaternion q_inverse = inverse(q);
	ret = q * p * q_inverse;

	assert(Equal(ret.s, 0));
	return{ ret.i, ret.j, ret.k };
}
Point rotatePoint3(float s, float a, float b, float c, Point ponit)
{
	Matrix m(3, 3);
	matrixSetIdentity(m);
	m[0][0] = 1 - 2 * b * b - 2 * c * c;
	m[0][1] = 2 * a * b - 2 * s * c;
	m[0][2] = 2 * a * c + 2 * s * b;
	m[1][0] = 2 * a * b + 2 * s * c;
	m[1][1] = 1 - 2 * a * a - 2 * c * c;
	m[1][2] = 2 * b * c - 2 * s * a;
	m[2][0] = 2 * a * c - 2 * s * b;
	m[2][1] = 2 * b * c + 2 * s * a;
	m[2][2] = 1 - 2 * a * a - 2 * b * b;

	Matrix p(3, 1);
	p[0][0] = ponit.x;
	p[1][0] = ponit.y;
	p[2][0] = ponit.z;
	Matrix ret = m * p;
	return{ ret[0][0], ret[1][0], ret[2][0] };
}

void test()
{
	Point p = { 2,3,4 };

	float theta = 30 * PI / 180;
	Vec3 u = { 1, 1.3, 0.7 };
	normal(u);
	float s = cos(theta / 2);
	float a = u.x * sin(theta / 2);
	float b = u.y * sin(theta / 2);
	float c = u.z * sin(theta / 2);
	Point p1 = rotatePoint1(s, a, b, c, p);
	Point p2 = rotatePoint2(s, a, b, c, p);
	Point p3 = rotatePoint3(s, a, b, c, p);
	printf("theta = %f, u = (%f, %f, %f) same = %s\n", theta, u.x, u.y, u.z, p1 == p2 && p2 == p3 ? "true" : "false");

	u = { 1, 0, 0 };
	normal(u);
	a = u.x * sin(theta / 2);
	b = u.y * sin(theta / 2);
	c = u.z * sin(theta / 2);
	p1 = rotatePoint1(s, a, b, c, p);
	p2 = rotatePoint2(s, a, b, c, p);
	p3 = rotatePoint3(s, a, b, c, p);
	printf("theta = %f, u = (%f, %f, %f) same = %s\n", theta, u.x, u.y, u.z, p1 == p2 && p2 == p3 ? "true" : "false");

	u = { 0, 1, 0 };
	normal(u);
	a = u.x * sin(theta / 2);
	b = u.y * sin(theta / 2);
	c = u.z * sin(theta / 2);
	p1 = rotatePoint1(s, a, b, c, p);
	p2 = rotatePoint2(s, a, b, c, p);
	p3 = rotatePoint3(s, a, b, c, p);
	printf("theta = %f, u = (%f, %f, %f) same = %s\n", theta, u.x, u.y, u.z, p1 == p2 && p2 == p3 ? "true" : "false");

	u = { 0, 0, 1 };
	normal(u);
	a = u.x * sin(theta / 2);
	b = u.y * sin(theta / 2);
	c = u.z * sin(theta / 2);
	p1 = rotatePoint1(s, a, b, c, p);
	p2 = rotatePoint2(s, a, b, c, p);
	p3 = rotatePoint3(s, a, b, c, p);
	printf("theta = %f, u = (%f, %f, %f) same = %s\n", theta, u.x, u.y, u.z, p1 == p2 && p2 == p3 ? "true" : "false");


}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	test();

	glFlush();
}
void code_9_exercise_2()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_3
Matrix RotateZMatrix1(float theta)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
		
	m[0][0] = cos(theta);
	m[0][1] = -sin(theta);
	m[1][0] = sin(theta);
	m[1][1] = cos(theta);

	return m;
}
Matrix RotateZMatrix2(float theta)
{
	float s = cos(theta / 2);
	float a = 0;
	float b = 0;
	float c = sin(theta / 2);

	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][0] = 1 - 2 * b * b - 2 * c * c;
	m[0][1] = 2 * a * b - 2 * s * c;
	m[0][2] = 2 * a * c + 2 * s * b;
	m[1][0] = 2 * a * b + 2 * s * c;
	m[1][1] = 1 - 2 * a * a - 2 * c * c;
	m[1][2] = 2 * b * c - 2 * s * a;
	m[2][0] = 2 * a * c - 2 * s * b;
	m[2][1] = 2 * b * c + 2 * s * a;
	m[2][2] = 1 - 2 * a * a - 2 * b * b;

	return m;
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	Point p = { 2,3,4 };

	float theta = 45 * PI / 180;
		
	Matrix m1 = RotateZMatrix1(theta);
	Matrix m2 = RotateZMatrix1(theta);
	printf("same = %s\n", m1 == m2 ? "true" : "false");

	std::vector<Point> rect = { {-50, -50, 0}, {50, -50, 0},{50, 50, 0},{-50, 50, 0} };

	auto rect1 = rect;
	glViewport(0, 0, winWidth / 2, winHeight);
	drawCoordinate();
	transformPoints(m1, rect1);
	drawPolygon(rect1);

	auto rect2 = rect;
	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	drawCoordinate();
	transformPoints(m1, rect2);
	drawPolygon(rect2);

	glFlush();
}
void code_9_exercise_3()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_4
Matrix rotateMatrix1(float tx, float ty, float tz, Vec3 u, float theta)
{
	normal(u);

	float a = u.x;
	float b = u.y;
	float c = u.z;
	float d = std::sqrt(b * b + c * c);

	Matrix T_inverse(4, 4);
	matrixSetIdentity(T_inverse);
	T_inverse[0][3] = tx;
	T_inverse[1][3] = ty;
	T_inverse[2][3] = tz;

	Matrix Rxa_inverse(4, 4);
	matrixSetIdentity(Rxa_inverse);
	Rxa_inverse[1][1] = c / d;
	Rxa_inverse[1][2] = b / d;
	Rxa_inverse[2][1] = -b / d;
	Rxa_inverse[2][2] = c / d;

	Matrix Ryb_inverse(4, 4);
	matrixSetIdentity(Ryb_inverse);
	Ryb_inverse[0][0] = d;
	Ryb_inverse[0][2] = a;
	Ryb_inverse[2][0] = -a;
	Ryb_inverse[2][2] = d;

	Matrix Rz(4, 4);
	matrixSetIdentity(Rz);
	Rz[0][0] = cos(theta);
	Rz[0][1] = -sin(theta);
	Rz[1][0] = sin(theta);
	Rz[1][1] = cos(theta);

	Matrix Ryb(4, 4);
	matrixSetIdentity(Ryb);
	Ryb[0][0] = d;
	Ryb[0][2] = -a;
	Ryb[2][0] = a;
	Ryb[2][2] = d;

	Matrix Rxa(4, 4);
	matrixSetIdentity(Rxa);
	Rxa[1][1] = c / d;
	Rxa[1][2] = -b / d;
	Rxa[2][1] = b / d;
	Rxa[2][2] = c / d;
	
	Matrix T(4, 4);
	matrixSetIdentity(T);
	T[0][3] = -tx;
	T[1][3] = -ty;
	T[2][3] = -tz;

	return T_inverse * Rxa_inverse * Ryb_inverse * Rz * Ryb * Rxa * T;
}
Matrix rotateMatrix2(float tx, float ty, float tz, Vec3 u, float theta)
{
	normal(u);

	Matrix T_inverse(4, 4);
	matrixSetIdentity(T_inverse);
	T_inverse[0][3] = tx;
	T_inverse[1][3] = ty;
	T_inverse[2][3] = tz;

	Matrix MR(4, 4);
	matrixSetIdentity(MR);
	MR[0][0] = u.x * u.x * (1 - cos(theta)) + cos(theta);
	MR[0][1] = u.x * u.y * (1 - cos(theta)) - u.z * sin(theta);
	MR[0][2] = u.x * u.z * (1 - cos(theta)) + u.y * sin(theta);
	
	MR[1][0] = u.y * u.x * (1 - cos(theta)) + u.z * sin(theta);
	MR[1][1] = u.y * u.y * (1 - cos(theta)) + cos(theta);
	MR[1][2] = u.y * u.z * (1 - cos(theta)) - u.x * sin(theta);

	MR[2][0] = u.z * u.x * (1 - cos(theta)) - u.y * sin(theta);
	MR[2][1] = u.z * u.y * (1 - cos(theta)) + u.x * sin(theta);
	MR[2][2] = u.z * u.z * (1 - cos(theta)) + cos(theta);

	Matrix T(4, 4);
	matrixSetIdentity(T);
	T[0][3] = -tx;
	T[1][3] = -ty;
	T[2][3] = -tz;

	return T_inverse * MR * T;
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	float tx = 3, ty = 4, tz = 5;
	Vec3 u = { 1, 1.5, 0.7 };
	float theta = 30 * PI / 180;

	Matrix m1 = rotateMatrix1(tx, ty, tz, u, theta);
	Matrix m2 = rotateMatrix2(tx, ty, tz, u, theta);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	u = { 0, 0, 1 };
	m1 = rotateMatrix1(tx, ty, tz, u, theta);
	m2 = rotateMatrix2(tx, ty, tz, u, theta);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	u = { 0, 1, 0 };
	m1 = rotateMatrix1(tx, ty, tz, u, theta);
	m2 = rotateMatrix2(tx, ty, tz, u, theta);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	// 注意这里不能用u=(1,0,0)，因为rotateMatrix1计算分母d=0。但化简后可消去d，即（9.39）式
	// 故化简后（在rotateMatrix2中）可使用u=(1,0,0)
	//u = { 1, 0, 0 };
	//m1 = rotateMatrix1(tx, ty, tz, u, theta);
	//m2 = rotateMatrix2(tx, ty, tz, u, theta);
	//printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	glFlush();
}
void code_9_exercise_4()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_5
Matrix rotateMatrix1(Vec3 u, float theta)
{
	normal(u);

	float s = cos(theta / 2);
	float a = u.x * sin(theta / 2);
	float b = u.y * sin(theta / 2);
	float c = u.z * sin(theta / 2);
	
	Matrix MR(4, 4);
	matrixSetIdentity(MR);
	MR[0][0] = 1 - 2 * b * b - 2 * c * c;
	MR[0][1] = 2 * a * b - 2 * s * c;
	MR[0][2] = 2 * a * c + 2 * s * b;

	MR[1][0] = 2 * a * b + 2 * s * c;
	MR[1][1] = 1 - 2 * a * a - 2 * c * c;
	MR[1][2] = 2 * b * c - 2 * s * a;

	MR[2][0] = 2 * a * c - 2 * s * b;
	MR[2][1] = 2 * b * c + 2 * s * a;
	MR[2][2] = 1 - 2 * a * a - 2 * b * b;

	return MR;
}
Matrix rotateMatrix2(Vec3 u, float theta)
{
	normal(u);

	Matrix MR(4, 4);
	matrixSetIdentity(MR);
	MR[0][0] = u.x * u.x * (1 - cos(theta)) + cos(theta);
	MR[0][1] = u.x * u.y * (1 - cos(theta)) - u.z * sin(theta);
	MR[0][2] = u.x * u.z * (1 - cos(theta)) + u.y * sin(theta);

	MR[1][0] = u.y * u.x * (1 - cos(theta)) + u.z * sin(theta);
	MR[1][1] = u.y * u.y * (1 - cos(theta)) + cos(theta);
	MR[1][2] = u.y * u.z * (1 - cos(theta)) - u.x * sin(theta);

	MR[2][0] = u.z * u.x * (1 - cos(theta)) - u.y * sin(theta);
	MR[2][1] = u.z * u.y * (1 - cos(theta)) + u.x * sin(theta);
	MR[2][2] = u.z * u.z * (1 - cos(theta)) + cos(theta);

	return MR;
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	Vec3 u = { 1, 1.5, 0.7 };
	float theta = 30 * PI / 180;

	Matrix m1 = rotateMatrix1(u, theta);
	Matrix m2 = rotateMatrix2(u, theta);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	u = { 0, 0, 1 };
	m1 = rotateMatrix1(u, theta);
	m2 = rotateMatrix2(u, theta);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	u = { 0, 1, 0 };
	m1 = rotateMatrix1(u, theta);
	m2 = rotateMatrix2(u, theta);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	u = { 1, 0, 0 };
	m1 = rotateMatrix1(u, theta);
	m2 = rotateMatrix2(u, theta);
	printf("u=(%f, %f, %f) same = %s\n", u.x, u.y, u.z, m1 == m2 ? "true" : "false");

	glFlush();
}
void code_9_exercise_5()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_6
float angle = 5 * PI / 180;
int loopCount = 0;
std::vector<Point> originalPoints = { { (float)winWidth / 2 + 150, (float)winHeight / 2 },
{ (float)winWidth / 2 + 200, (float)winHeight / 2 },
{ (float)winWidth / 2 + 175, (float)winHeight / 2 + 100 } };
std::vector<Point> curPoints;
Matrix r(4, 4);
namespace ApproximateTrigonometry
{
	float cos(float angle)
	{
		return 1.f;
		//return std::cos(angle);
	}
	float sin(float angle)
	{
		return angle;
		//return std::sin(angle);
	}
}
void _drawCoordinate()
{
	glBegin(GL_LINES);
	glVertex3f(0, winHeight / 2, 0);
	glVertex3f(winWidth, winHeight / 2, 0);
	glVertex3f(winWidth / 2, 0, 0);
	glVertex3f(winWidth / 2, winHeight, 0);
	glEnd();
}
Matrix rotateMatrix(Point p1, Point p2, float theta)
{
	using ApproximateTrigonometry::cos;
	using ApproximateTrigonometry::sin;

	Vec3 u = p2 - p1;
	normal(u);

	float tx = p1.x;
	float ty = p1.y;
	float tz = p1.z;

	Matrix T_inverse(4, 4);
	matrixSetIdentity(T_inverse);
	T_inverse[0][3] = tx;
	T_inverse[1][3] = ty;
	T_inverse[2][3] = tz;

	Matrix MR(4, 4);
	matrixSetIdentity(MR);
	MR[0][0] = u.x * u.x * (1 - cos(theta)) + cos(theta);
	MR[0][1] = u.x * u.y * (1 - cos(theta)) - u.z * sin(theta);
	MR[0][2] = u.x * u.z * (1 - cos(theta)) + u.y * sin(theta);

	MR[1][0] = u.y * u.x * (1 - cos(theta)) + u.z * sin(theta);
	MR[1][1] = u.y * u.y * (1 - cos(theta)) + cos(theta);
	MR[1][2] = u.y * u.z * (1 - cos(theta)) - u.x * sin(theta);

	MR[2][0] = u.z * u.x * (1 - cos(theta)) - u.y * sin(theta);
	MR[2][1] = u.z * u.y * (1 - cos(theta)) + u.x * sin(theta);
	MR[2][2] = u.z * u.z * (1 - cos(theta)) + cos(theta);

	Matrix T(4, 4);
	matrixSetIdentity(T);
	T[0][3] = -tx;
	T[1][3] = -ty;
	T[2][3] = -tz;

	return T_inverse * MR * T;
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	_drawCoordinate();
	drawPolygon(curPoints);

	glFlush();
}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//printf("onTimer: %d\n", dwTime);
	loopCount++;
	if (loopCount >= 2 * PI / angle)
	{
		curPoints = originalPoints;
		loopCount = 0;
	}
	else
	{
		transformPoints(r, curPoints);
	}

	glutPostRedisplay();
	//displayFcn();
}
void code_9_exercise_6()
{
	SetTimer(NULL, NULL, 100, onTimer);

	r = rotateMatrix({ (float)winWidth / 2, (float)winHeight / 2, 0 }, { (float)winWidth / 2, (float)winHeight / 2, 100 }, angle);

	curPoints = originalPoints;

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_7
Matrix scaleMatrix1(Vec3 a, Vec3 b, Vec3 c, float sx, float sy, float sz)
{
	normal(a);
	normal(b);
	normal(c);

	Matrix r(4, 4);
	matrixSetIdentity(r);

	r[0][0] = a.x;
	r[0][1] = a.y;
	r[0][2] = a.z;
	r[1][0] = b.x;
	r[1][1] = b.y;
	r[1][2] = b.z;
	r[2][0] = c.x;
	r[2][1] = c.y;
	r[2][2] = c.z;

	Matrix s(4, 4);
	matrixSetIdentity(s);

	s[0][0] = sx;
	s[1][1] = sy;
	s[2][2] = sz;

	Matrix r_inverse(4, 4);
	matrixSetIdentity(r_inverse);

	r_inverse[0][0] = a.x;
	r_inverse[0][1] = b.x;
	r_inverse[0][2] = c.x;
	r_inverse[1][0] = a.y;
	r_inverse[1][1] = b.y;
	r_inverse[1][2] = c.y;
	r_inverse[2][0] = a.z;
	r_inverse[2][1] = b.z;
	r_inverse[2][2] = c.z;

	return r_inverse * s * r;
}
Matrix scaleMatrix2(Vec3 a, Vec3 b, Vec3 c, float sx, float sy, float sz)
{
	normal(a);
	normal(b);
	normal(c);

	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][0] = sx * a.x * a.x + sy * b.x * b.x + sz * c.x * c.x;
	m[0][1] = sx * a.x * a.y + sy * b.x * b.y + sz * c.x * c.y;
	m[0][2] = sx * a.x * a.z + sy * b.x * b.z + sz * c.x * c.z;
	m[1][0] = sx * a.y * a.x + sy * b.y * b.x + sz * c.y * c.x;
	m[1][1] = sx * a.y * a.y + sy * b.y * b.y + sz * c.y * c.y;
	m[1][2] = sx * a.y * a.z + sy * b.y * b.z + sz * c.y * c.z;
	m[2][0] = sx * a.z * a.x + sy * b.z * b.x + sz * c.z * c.x;
	m[2][1] = sx * a.z * a.y + sy * b.z * b.y + sz * c.z * c.y;
	m[2][2] = sx * a.z * a.z + sy * b.z * b.z + sz * c.z * c.z;

	return m;
}
Matrix translateMatrix(float tx, float ty, float tz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	return m;
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	Vec3 a = { 1, 0, 0 };
	Vec3 b = { 0, 1, 0 };
	Vec3 c = { 0, 0, 1 };
	float sx = 1, sy = 2, sz = 1;

	Matrix m1 = scaleMatrix1(a, b, c, sx, sy, sz);
	Matrix m2 = scaleMatrix2(a, b, c, sx, sy, sz);
	printf("same = %s\n", m1 == m2 ? "true" : "false");

	a = { 1, -1, 0 };
	b = { 1, 1, 0 };
	c = { 0, 0, 1 };
	sx = 1, sy = 2, sz = 1;

	m1 = scaleMatrix1(a, b, c, sx, sy, sz);
	m2 = scaleMatrix2(a, b, c, sx, sy, sz);
	printf("same = %s\n", m1 == m2 ? "true" : "false");

	Point pr = { (float)winWidth / 4, (float)winHeight / 2 };
	std::vector<Point> rect = { { (float)winWidth / 4 - 50, (float)winHeight / 2 - 50, 0 },
	{ (float)winWidth / 4 + 50, (float)winHeight / 2 - 50, 0 },
	{ (float)winWidth / 4 + 50, (float)winHeight / 2 + 50, 0 },
	{ (float)winWidth / 4 - 50, (float)winHeight / 2 + 50, 0 } };

	auto rect1 = rect;
	glViewport(0, 0, winWidth / 2, winHeight);
	transformPoints(translateMatrix(pr.x, pr.y, pr.z) * m1 * translateMatrix(-pr.x, -pr.y, -pr.z), rect1);
	drawPolygon(rect1);

	auto rect2 = rect;
	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	transformPoints(translateMatrix(pr.x, pr.y, pr.z) * m2 * translateMatrix(-pr.x, -pr.y, -pr.z), rect2);
	drawPolygon(rect2);

	glFlush();
}
void code_9_exercise_7()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth / 2, 0, winHeight);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_8
Matrix rotateMatrix(Vec3 u, float theta)
{
	normal(u);

	Matrix MR(4, 4);
	matrixSetIdentity(MR);
	MR[0][0] = u.x * u.x * (1 - cos(theta)) + cos(theta);
	MR[0][1] = u.x * u.y * (1 - cos(theta)) - u.z * sin(theta);
	MR[0][2] = u.x * u.z * (1 - cos(theta)) + u.y * sin(theta);

	MR[1][0] = u.y * u.x * (1 - cos(theta)) + u.z * sin(theta);
	MR[1][1] = u.y * u.y * (1 - cos(theta)) + cos(theta);
	MR[1][2] = u.y * u.z * (1 - cos(theta)) - u.x * sin(theta);

	MR[2][0] = u.z * u.x * (1 - cos(theta)) - u.y * sin(theta);
	MR[2][1] = u.z * u.y * (1 - cos(theta)) + u.x * sin(theta);
	MR[2][2] = u.z * u.z * (1 - cos(theta)) + cos(theta);

	return MR;
}
Matrix translateMatrix(float tx, float ty, float tz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	return m;
}
Matrix reflectionYMatrix()
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[1][1] = -1;

	return m;
}
Matrix reflectionMatrix(float A, float B, float C, float D)
{
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	
	float cos = (A * 0 + B * 1 + C * 0) / (sqrt(A * A + B * B + C * C) * sqrt(0 * 0 + 1 * 1 + 0 * 0));
	float theta = acos(cos);
	if (theta != 0)
	{
		float tx = 0, tz = 0;
		if (A != 0)
		{
			tz = 0;
			tx = -D / A;
		}
		else
		{
			tx = 0;
			tz = D / C;
		}
		Vec3 u = cross({ A, B, C }, { 0, 1, 0 });
		ret = translateMatrix(tx, 0, tz) * rotateMatrix(u, -theta) * reflectionYMatrix() * rotateMatrix(u, theta) * translateMatrix(-tx, 0, -tz);
	}
	else
	{
		ret = translateMatrix(0, -D / B, 0) * reflectionYMatrix() * translateMatrix(0, D / B, 0);
	}
	return ret;
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> rect = { { 50, 50, 0 }, { 150, 50, 0 }, {150, 100, 0 }, {50, 100, 0 } };

	glViewport(0, winHeight / 2, winWidth / 2, winHeight / 2);
	drawCoordinate(-180, 180, -140, 140);
	auto rect1 = rect;
	Matrix m1 = reflectionMatrix(0, 1, 0, 0);
	transformPoints(m1, rect1);
	drawPolygon(rect1);

	glViewport(winWidth / 2, winHeight / 2, winWidth / 2, winHeight / 2);
	drawCoordinate(-180, 180, -140, 140);
	auto rect2 = rect;
	Matrix m2 = reflectionMatrix(1, 0, 0, -50);
	transformPoints(m2, rect2);
	drawPolygon(rect2);

	glViewport(0, 0, winWidth / 2, winHeight / 2);
	drawCoordinate(-180, 180, -140, 140);
	auto rect3 = rect;
	Matrix m3 = reflectionMatrix(1, 1, 0, -50);
	transformPoints(m3, rect3);
	drawPolygon(rect3);

	glViewport(winWidth / 2, 0, winWidth / 2, winHeight / 2);
	drawCoordinate(-180, 180, -140, 140);
	auto rect4 = rect;
	Matrix m4 = reflectionMatrix(-1, -1, 0, 50);
	transformPoints(m4, rect4);
	drawPolygon(rect4);

	glFlush();
}
void code_9_exercise_8()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-winWidth / 4, winWidth / 4, -winHeight / 4, winHeight / 4);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_9
Matrix shareZXMatrix(float shzx)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][2] = shzx;
	
	return m;
}
Matrix shareZYMatrix(float shzy)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[1][2] = shzy;

	return m;
}
Matrix shareXYMatrix(float shxy)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[1][0] = shxy;

	return m;
}
Matrix shareXZMatrix(float shxz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[2][0] = shxz;

	return m;
}
Matrix shareYXMatrix(float shyx)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][1] = shyx;

	return m;
}
Matrix shareYZMatrix(float shyz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[2][1] = shyz;

	return m;
}
Matrix translateMatrix(float tx, float ty, float tz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	return m;
}
Matrix shareLineYMatrix(float tx, float ty, float tz, Vec3 u, float shyz)
{
	normal(u);

	Vec3 ux = { 1, 0, 0 };
	Vec3 uy = { 0, 1, 0 };
	Vec3 uz = { 0, 0, 1 };
	Vec3 uz_ = u;
	Vec3 uy_ = cross(u, ux);
	normal(uy_);
	Vec3 ux_ = cross(uy_, uz_);

	Matrix Rz(4, 4);
	matrixSetIdentity(Rz);
	Rz[0][0] = ux_.x;
	Rz[0][1] = ux_.y;
	Rz[0][2] = ux_.z;
	Rz[1][0] = uy_.x;
	Rz[1][1] = uy_.y;
	Rz[1][2] = uy_.z;
	Rz[2][0] = uz_.x;
	Rz[2][1] = uz_.y;
	Rz[2][2] = uz_.z;

	Matrix Rz_inverse(4, 4);
	matrixSetIdentity(Rz_inverse);
	Rz_inverse[0][0] = ux_.x;
	Rz_inverse[0][1] = uy_.x;
	Rz_inverse[0][2] = uz_.x;
	Rz_inverse[1][0] = ux_.y;
	Rz_inverse[1][1] = uy_.y;
	Rz_inverse[1][2] = uz_.y;
	Rz_inverse[2][0] = ux_.z;
	Rz_inverse[2][1] = uy_.z;
	Rz_inverse[2][2] = uz_.z;

	return translateMatrix(tx, ty, tz) * Rz_inverse * shareYZMatrix(shyz) * Rz * translateMatrix(-tx, -ty, -tz);
}
Matrix shareLineXMatrix(float tx, float ty, float tz, Vec3 u, float shxz)
{
	normal(u);

	Vec3 ux = { 1, 0, 0 };
	Vec3 uy = { 0, 1, 0 };
	Vec3 uz = { 0, 0, 1 };
	Vec3 uz_ = u;
	Vec3 uy_ = cross(u, ux);
	normal(uy_);
	Vec3 ux_ = cross(uy_, uz_);

	Matrix Rz(4, 4);
	matrixSetIdentity(Rz);
	Rz[0][0] = ux_.x;
	Rz[0][1] = ux_.y;
	Rz[0][2] = ux_.z;
	Rz[1][0] = uy_.x;
	Rz[1][1] = uy_.y;
	Rz[1][2] = uy_.z;
	Rz[2][0] = uz_.x;
	Rz[2][1] = uz_.y;
	Rz[2][2] = uz_.z;

	Matrix Rz_inverse(4, 4);
	matrixSetIdentity(Rz_inverse);
	Rz_inverse[0][0] = ux_.x;
	Rz_inverse[0][1] = uy_.x;
	Rz_inverse[0][2] = uz_.x;
	Rz_inverse[1][0] = ux_.y;
	Rz_inverse[1][1] = uy_.y;
	Rz_inverse[1][2] = uz_.y;
	Rz_inverse[2][0] = ux_.z;
	Rz_inverse[2][1] = uy_.z;
	Rz_inverse[2][2] = uz_.z;

	return translateMatrix(tx, ty, tz) * Rz_inverse * shareXZMatrix(shxz) * Rz * translateMatrix(-tx, -ty, -tz);
}


void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> rect = { {0, 0, 0}, {50, 0, 0},{ 50, 50, 0 },{ 0, 50, 0 } };

	auto rect1 = rect;
	glViewport(0, winHeight / 2, winWidth / 3, winHeight / 2);
	drawCoordinate(-50, 200, -50, 230);
	transformPoints(shareZXMatrix(1), rect1);
	drawPolygon(rect1);
	
	auto rect2 = rect;
	glViewport(0, 0, winWidth / 3, winHeight / 2);
	drawCoordinate(-50, 200, -50, 230);
	transformPoints(shareYXMatrix(1), rect2);
	drawPolygon(rect2);

	auto rect3 = rect;
	glViewport(winWidth / 3, winHeight / 2, winWidth / 3, winHeight / 2);
	drawCoordinate(-50, 200, -50, 230);
	transformPoints(shareZYMatrix(1), rect3);
	drawPolygon(rect3);

	auto rect4 = rect;
	glViewport(winWidth / 3, 0, winWidth / 3, winHeight / 2);
	drawCoordinate(-50, 200, -50, 230);
	transformPoints(shareXYMatrix(1), rect4);
	drawPolygon(rect4);

	rect = { { 50, 50, 0 },{ 100, 50, 0 },{ 100, 100, 0 },{ 50, 100, 0 } };
	auto rect5 = rect;
	glViewport(2 * winWidth / 3, winHeight / 2, winWidth / 3, winHeight / 2);
	drawCoordinate(-50, 200, -50, 230);
	transformPoints(shareLineYMatrix(50, 50, 0, {1, 1, 0}, 1), rect5);
	drawPolygon(rect5);

	auto rect6 = rect;
	glViewport(2 * winWidth / 3, 0, winWidth / 3, winHeight / 2);
	drawCoordinate(-50, 200, -50, 230);
	transformPoints(shareLineXMatrix(50, 50, 0, { 1, 1, 0 }, 1), rect6);
	drawPolygon(rect6);

	glFlush();
}
void code_9_exercise_9()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-50, winWidth / 3 - 50, -50, winHeight / 2 - 50);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_10
Matrix translateMatrix(float tx, float ty, float tz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	return m;
}
Matrix transferMatrix(Point o, Vec3 ux, Vec3 uy, Vec3 uz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	normal(ux);
	normal(uy);
	normal(uz);

	m[0][0] = ux.x;
	m[0][1] = ux.y;
	m[0][2] = ux.z;
	m[1][0] = uy.x;
	m[1][1] = uy.y;
	m[1][2] = uy.z;
	m[2][0] = uz.x;
	m[2][1] = uz.y;
	m[2][2] = uz.z;

	return m * translateMatrix(-o.x, -o.y, -o.z);
}
void _drawCoordinate(Point xStart, Point xEnd, Point yStart, Point yEnd)
{
	glBegin(GL_LINES);
	glVertex3f(xStart.x, xStart.y, 0);
	glVertex3f(xEnd.x, xEnd.y, 0);
	glVertex3f(yStart.x, yStart.y, 0);
	glVertex3f(yEnd.x, yEnd.y, 0);
	glEnd();
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> rect = { { 120, 100, 0 },
	{ float(120 + 50), float(100 - 50), 0 },
	{ float(120 + 100), 100, 0 },
	{ float(120 + 50), float(100 + 50), 0 } };

	Point o = { 100, 100, 0 };
	Vec3 ux = { 100, -100, 0 };
	Vec3 uy = { 100, 100, 0 };
	Vec3 uz = { 0, 0, 100 };

	glViewport(0, 0, winWidth / 2, winHeight);
	_drawCoordinate({-50, 0, 0}, {320, 0, 0}, {0, -50, 0 }, { 0, 520, 0 });
	_drawCoordinate(o, o + ux, o, o + uy);
	drawPolygon(rect);

	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	_drawCoordinate({ -50, 0, 0 }, { 320, 0, 0 }, { 0, -50, 0 }, { 0, 520, 0 });
	transformPoints(transferMatrix(o, ux, uy, uz), rect);
	drawPolygon(rect);

	glFlush();
}
void code_9_exercise_10()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-60, winWidth / 2 - 60, -60, winHeight - 60);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_11
void rotate(Point p0, Vec3 u, float theta)
{
	normal(u);
	glTranslatef(p0.x, p0.y, p0.z);
	glRotatef(theta, u.x, u.y, u.z);
	glTranslatef(-p0.x, -p0.y, -p0.z);
}
void scale(Point p0, float sx, float sy, float sz)
{
	glTranslatef(p0.x, p0.y, p0.z);
	glScalef(sx, sy, sz);
	glTranslatef(-p0.x, -p0.y, -p0.z);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<Point> verts = { { 50, 25, 0 }, { 150, 25, 0 }, {100, 100, 0 }};
	Point centroidPt = centerPoint(verts);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glViewport(0, 0, winWidth / 2, winHeight);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(verts);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(-50, 320, -50, 520);

	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	glTranslatef(0, 100, 0);
	rotate(centroidPt, { 0, 0, 1 }, 90);
	scale(centroidPt, 0.5, 0.5, 1);
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(verts);

	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(-50, 320, -50, 520);

	glFlush();
}
void code_9_exercise_11()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-60, winWidth / 2 - 60, -60, winHeight - 60);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_12
std::vector<Point> inputPoints()
{
	std::vector<Point> points;
	std::string inputStr;
	while (1)
	{
		printf("input points x, y, z. \"exit\" to end:");
		std::cin >> inputStr;
		if (inputStr == "exit")
			break;
		else
		{
			points.push_back({});
			sscanf_s(inputStr.c_str(), "%f, %f, %f", &points.back().x, &points.back().y, &points.back().z);
		}
	}
	return points;
}
struct Transform
{
	std::string type;
};
struct Translate : public Transform
{
	float tx, ty, tz;
};
struct Rotate : public Transform
{
	Point p0;
	Vec3 u;
	float theta;
};
struct Scale : public Transform
{
	Point p0;
	float sx, sy, sz;
};
std::vector<Transform*> inputTransform()
{
	std::vector<Transform*> transforms;
	std::string inputStr;
	while (1)
	{
		printf("input transform [(T)ranslate, (R)otate, (S)cale]. \"exit\" to end:");
		std::cin >> inputStr;
		if (inputStr == "exit")
			break;
		else
		{
			Transform * transform = nullptr;
			if (inputStr == "T")
			{
				transform = new Translate();
			}
			else if (inputStr == "R")
			{
				transform = new Rotate();
			}
			else if (inputStr == "S")
			{
				transform = new Scale();
			}
			else
			{
				assert(0 && "error type!");
			}
			transform->type = inputStr;

			if (inputStr == "T")
			{
				printf("input translate parameters: tx, ty, tz. \"exit\" to end:");
				std::cin >> inputStr;
				if (inputStr == "exit")
					break;
				sscanf_s(inputStr.c_str(), "%f, %f, %f", 
					&((Translate*)transform)->tx, 
					&((Translate*)transform)->ty,
					&((Translate*)transform)->tz);
			}
			else if (inputStr == "R")
			{
				printf("input rotate parameters: reference point: x, y, z; rotate axis: u.x, u.y, u.z; angle: theta. \"exit\" to end:");
				std::cin >> inputStr;
				if (inputStr == "exit")
					break;
				sscanf_s(inputStr.c_str(), "%f, %f, %f, %f, %f, %f, %f",
					&((Rotate*)transform)->p0.x,
					&((Rotate*)transform)->p0.y,
					&((Rotate*)transform)->p0.z,
					&((Rotate*)transform)->u.x,
					&((Rotate*)transform)->u.y, 
					&((Rotate*)transform)->u.z,
					&((Rotate*)transform)->theta);
			}
			else if (inputStr == "S")
			{
				printf("input scale parameters: reference point: x, y, z; sx, sy, sz. \"exit\" to end:");
				std::cin >> inputStr;
				if (inputStr == "exit")
					break;
				sscanf_s(inputStr.c_str(), "%f, %f, %f, %f, %f, %f",
					&((Scale*)transform)->p0.x,
					&((Scale*)transform)->p0.y,
					&((Scale*)transform)->p0.z,
					&((Scale*)transform)->sx,
					&((Scale*)transform)->sy,
					&((Scale*)transform)->sz);
			}
			transforms.push_back(transform);
		}
	}
	return transforms;
}
void rotate(Point p0, Vec3 u, float theta)
{
	normal(u);
	glTranslatef(p0.x, p0.y, p0.z);
	glRotatef(theta, u.x, u.y, u.z);
	glTranslatef(-p0.x, -p0.y, -p0.z);
}
void scale(Point p0, float sx, float sy, float sz)
{
	glTranslatef(p0.x, p0.y, p0.z);
	glScalef(sx, sy, sz);
	glTranslatef(-p0.x, -p0.y, -p0.z);
}
std::vector<Point> points;
std::vector<Transform*> transforms;
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glViewport(0, 0, winWidth / 2, winHeight);
	glColor3f(0.0, 0.0, 1.0);
	drawPolygon(points);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(-50, 320, -50, 520);

	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	
	for (auto it = transforms.rbegin(); it != transforms.rend(); it++)
	{
		auto& transform = *it;
		if (transform->type == "T")
		{
			Translate* tran = (Translate*)transform;
			glTranslatef(tran->tx, tran->ty, tran->tz);
		}
		else if(transform->type == "R")
		{
			Rotate* tran = (Rotate*)transform;
			rotate(tran->p0, tran->u, tran->theta);
		}
		else if (transform->type == "S")
		{
			Scale* tran = (Scale*)transform;
			scale(tran->p0, tran->sx, tran->sy, tran->sz);
		}
	}
	glColor3f(1.0, 0.0, 0.0);
	drawPolygon(points);

	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinate(-50, 320, -50, 520);

	glFlush();
}
void code_9_exercise_12()
{
	points = inputPoints();
	transforms = inputTransform();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-60, winWidth / 2 - 60, -60, winHeight - 60);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_15
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> points = { {100, 100, 0}, {150, 100, 0},{150, 150, 0},{100, 150, 0} };

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glViewport(0, winHeight / 2, winWidth / 2, winHeight / 2);
	drawCoordinate(-50, 320, -50, 220);
	drawPolygon(points);

	glViewport(winWidth / 2, winHeight / 2, winWidth / 2, winHeight / 2);
	drawCoordinate(-50, 320, -50, 220);
	glTranslatef(125, 125, 0);
	glScalef(2.0, 1.0, 1.0);
	glTranslatef(-125, -125, 0);
	drawPolygon(points);

	glPopMatrix();
	glPushMatrix();
	glViewport(0, 0, winWidth / 2, winHeight / 2);
	drawCoordinate(-50, 320, -50, 220);
	glTranslatef(125, 125, 0);
	glScalef(1.0, 2.0, 1.0);
	glTranslatef(-125, -125, 0);
	drawPolygon(points);

	glPopMatrix();
	glViewport(winWidth / 2, 0, winWidth / 2, winHeight / 2);
	drawCoordinate(-50, 320, -50, 220);
	glTranslatef(125, 125, 0);
	glScalef(1.0, 1.0, 2.0);
	glTranslatef(-125, -125, 0);
	drawPolygon(points);

	glFlush();
}
void code_9_exercise_15()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-60, winWidth / 2 - 60, -60, winHeight / 2 - 60);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_15_1
Matrix translate(float tx, float ty, float tz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	return m;
}
Matrix scale(float sx, float sy, float sz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;

	return m;
}
Matrix directionScale(Point p0, Vec3 ux, Vec3 uy, Vec3 uz, float sx, float sy, float sz)
{
	normal(ux);
	normal(uy);
	normal(uz);

	Matrix R(4, 4);
	matrixSetIdentity(R);
	R[0][0] = ux.x;
	R[0][1] = ux.y;
	R[0][2] = ux.z;
	R[1][0] = uy.x;
	R[1][1] = uy.y;
	R[1][2] = uy.z;
	R[2][0] = uz.x;
	R[2][1] = uz.y;
	R[2][2] = uz.z;

	Matrix R_inverse(4, 4);
	matrixSetIdentity(R_inverse);
	R_inverse[0][0] = ux.x;
	R_inverse[0][1] = uy.x;
	R_inverse[0][2] = uz.x;
	R_inverse[1][0] = ux.y;
	R_inverse[1][1] = uy.y;
	R_inverse[1][2] = uz.y;
	R_inverse[2][0] = ux.z;
	R_inverse[2][1] = uy.z;
	R_inverse[2][2] = uz.z;

	return translate(p0.x, p0.y, p0.z) * R_inverse * scale(sx, sy, sz) * R * translate(-p0.x, -p0.y, -p0.z);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> points = { { 100, 100, 0 },{ 150, 100, 0 },{ 150, 150, 0 },{ 100, 150, 0 } };

	glViewport(0, 0, winWidth / 3, winHeight);
	drawCoordinate(-20, 220, -50, 520);
	drawPolygon(points);

	glViewport(winWidth / 3, 0, winWidth / 3, winHeight);
	drawCoordinate(-20, 220, -50, 520);
	auto points1 = points;
	transformPoints(directionScale({ 125, 125, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, 2, 1, 1), points1);
	drawPolygon(points1);

	glViewport(2 * winWidth / 3, 0, winWidth / 3, winHeight);
	drawCoordinate(-20, 220, -50, 520);
	auto points2 = points;
	transformPoints(directionScale({ 125, 125, 0 }, { 1, 1, 0 }, { -1, 1, 0 }, { 0, 0, 1 }, 2, 1, 1), points2);
	drawPolygon(points2);

	glFlush();
}
void code_9_exercise_15_1()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-30, winWidth / 3 - 30, -60, winHeight - 60);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_16
void reflectX()
{
	glRotatef(180, 1, 0, 0);
}
void shearX(float shx)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][1] = shx;
	glMultMatrixf(m);
}
void rotate(Point p0, float theta)
{
	glTranslatef(p0.x, p0.y, p0.z);
	glRotatef(theta, 0, 0, 1);
	glTranslatef(-p0.x, -p0.y, -p0.z);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> points = { { -35, -35, 0 },{ -5, -35, 0 },{ -5, -5, 0 },{ -35, -5, 0 } };
	Point p0 = {-20, -20, 0};

	glMatrixMode(GL_MODELVIEW);

	glViewport(0, 0, winWidth / 2, winHeight);
	drawCoordinate(-190, 190, -290, 290);
	drawPolygon(points);

	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	drawCoordinate(-190, 190, -290, 290);
	
	shearX(2);
	rotate({ -20, 20, 0 }, -45);
	reflectX();

	drawPolygon(points);

	glFlush();
}

void code_9_exercise_16()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_17
struct Transform;
std::vector<Point> points;
std::vector<Transform*> transforms;
Matrix translate(float tx, float ty, float tz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	return m;
}
Matrix rotateMatrix(Point p0, Vec3 u, float theta)
{
	normal(u);

	Matrix T_inverse(4, 4);
	matrixSetIdentity(T_inverse);
	T_inverse[0][3] = p0.x;
	T_inverse[1][3] = p0.y;
	T_inverse[2][3] = p0.z;

	Matrix MR(4, 4);
	matrixSetIdentity(MR);
	MR[0][0] = u.x * u.x * (1 - cos(theta)) + cos(theta);
	MR[0][1] = u.x * u.y * (1 - cos(theta)) - u.z * sin(theta);
	MR[0][2] = u.x * u.z * (1 - cos(theta)) + u.y * sin(theta);

	MR[1][0] = u.y * u.x * (1 - cos(theta)) + u.z * sin(theta);
	MR[1][1] = u.y * u.y * (1 - cos(theta)) + cos(theta);
	MR[1][2] = u.y * u.z * (1 - cos(theta)) - u.x * sin(theta);

	MR[2][0] = u.z * u.x * (1 - cos(theta)) - u.y * sin(theta);
	MR[2][1] = u.z * u.y * (1 - cos(theta)) + u.x * sin(theta);
	MR[2][2] = u.z * u.z * (1 - cos(theta)) + cos(theta);

	Matrix T(4, 4);
	matrixSetIdentity(T);
	T[0][3] = -p0.x;
	T[1][3] = -p0.y;
	T[2][3] = -p0.z;

	return T_inverse * MR * T;
}
Matrix rotate(Point p0, Vec3 u, float theta)
{
	glTranslatef(p0.x, p0.y, p0.z);
	glRotatef(theta, u.x, u.y, u.z);
	glTranslatef(-p0.x, -p0.y, -p0.z);

	return rotateMatrix(p0, u, theta * PI / 180);
}
Matrix reflect(Point p0, Vec3 u)
{
	return rotate(p0, u, 180);
}
Matrix _shear(char reference, float refValue, char shear1, char shear2, float sh1, float sh2)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);

	if (reference == 'z')
	{
		if (shear1 == 'x')
		{
			m[0][2] = sh1;
			m[0][3] = -sh1 * refValue;
		}
		if (shear2 == 'x')
		{
			m[0][2] = sh2;
			m[0][3] = -sh2 * refValue;
		}
		if (shear1 == 'y')
		{
			m[1][2] = sh1;
			m[1][3] = -sh1 * refValue;
		}
		if (shear2 == 'y')
		{
			m[1][2] = sh2;
			m[1][3] = -sh2 * refValue;
		}
	}

	if (reference == 'x')
	{
		if (shear1 == 'y')
		{
			m[1][0] = sh1;
			m[1][3] = -sh1 * refValue;
		}
		if (shear2 == 'y')
		{
			m[1][0] = sh2;
			m[1][3] = -sh2 * refValue;
		}
		if (shear1 == 'z')
		{
			m[2][0] = sh1;
			m[2][3] = -sh1 * refValue;
		}
		if (shear2 == 'z')
		{
			m[2][0] = sh2;
			m[2][3] = -sh2 * refValue;
		}
	}

	if (reference == 'y')
	{
		if (shear1 == 'x')
		{
			m[0][1] = sh1;
			m[0][3] = -sh1 * refValue;
		}
		if (shear2 == 'x')
		{
			m[0][1] = sh2;
			m[0][3] = -sh2 * refValue;
		}
		if (shear1 == 'z')
		{
			m[2][1] = sh1;
			m[2][3] = -sh1 * refValue;
		}
		if (shear2 == 'z')
		{
			m[2][1] = sh2;
			m[2][3] = -sh2 * refValue;
		}
	}

	return m;
}
Matrix shear(Point p0, Vec3 ux, Vec3 uy, Vec3 uz, char reference, float refValue, char shear1, char shear2, float sh1, float sh2)
{
	normal(ux);
	normal(uy);
	normal(uz);

	Matrix R(4, 4);
	matrixSetIdentity(R);
	R[0][0] = ux.x;
	R[0][1] = ux.y;
	R[0][2] = ux.z;
	R[1][0] = uy.x;
	R[1][1] = uy.y;
	R[1][2] = uy.z;
	R[2][0] = uz.x;
	R[2][1] = uz.y;
	R[2][2] = uz.z;

	Matrix R_inverse(4, 4);
	matrixSetIdentity(R_inverse);
	R_inverse[0][0] = ux.x;
	R_inverse[0][1] = uy.x;
	R_inverse[0][2] = uz.x;
	R_inverse[1][0] = ux.y;
	R_inverse[1][1] = uy.y;
	R_inverse[1][2] = uz.y;
	R_inverse[2][0] = ux.z;
	R_inverse[2][1] = uy.z;
	R_inverse[2][2] = uz.z;

	auto m = translate(p0.x, p0.y, p0.z) * R_inverse * _shear(reference, refValue, shear1, shear2, sh1, sh2) * R * translate(-p0.x, -p0.y, -p0.z);

	glMultMatrixf(m);

	return m;
}
std::vector<Point> inputPoints()
{
	std::vector<Point> points;
	std::string inputStr;
	while (1)
	{
		printf("input points x, y, z. \"exit\" to end:");
		std::cin >> inputStr;
		if (inputStr == "exit")
			break;
		else
		{
			points.push_back({});
			sscanf_s(inputStr.c_str(), "%f, %f, %f", &points.back().x, &points.back().y, &points.back().z);
		}
	}
	return points;
}
struct Transform
{
	std::string type;
};
struct Rotate : public Transform
{
	Point p0;
	Vec3 u;
	float theta;
};
struct Reflect : public Transform
{
	Point p0;
	Vec3 u;
};
struct Shear : public Transform
{
	Point p0;
	Vec3 ux, uy, uz;
	char reference;
	float refValue;
	char shear1, shear2;
	float sh1, sh2;
};
std::vector<Transform*> inputTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::vector<Transform*> transforms;
	std::string inputStr;

	Matrix m(4, 4);
	matrixSetIdentity(m);
	
	while (1)
	{
		printf("input transform [(Re)flect, (R)otate, (S)hear]. \"exit\" to end:");
		std::cin >> inputStr;
		if (inputStr == "exit")
			break;
		else
		{
			Transform * transform = nullptr;
			if (inputStr == "Re")
			{
				transform = new Reflect();
			}
			else if (inputStr == "R")
			{
				transform = new Rotate();
			}
			else if (inputStr == "S")
			{
				transform = new Shear();
			}
			else
			{
				assert(0 && "error type!");
			}
			transform->type = inputStr;

			if (inputStr == "Re")
			{
				Reflect* tran = (Reflect*)transform;
				printf("input reflect parameters: point: x, y, z; reflect axis: u.x, u.y, u.z. \"exit\" to end:");
				std::cin >> inputStr;
				if (inputStr == "exit")
					break;
				sscanf_s(inputStr.c_str(), "%f, %f, %f, %f, %f, %f",
					&tran->p0.x,
					&tran->p0.y,
					&tran->p0.z,
					&tran->u.x,
					&tran->u.y,
					&tran->u.z);

				m = reflect(tran->p0, tran->u) * m;
			}
			else if (inputStr == "R")
			{
				Rotate* tran = (Rotate*)transform;
				printf("input rotate parameters: reference point: x, y, z(\"center\" to center point); rotate axis: u.x, u.y, u.z; angle: theta. \"exit\" to end:");
				std::cin >> inputStr;
				if (inputStr == "exit")
					break;
				if (inputStr.substr(0, 6) == "center")
				{
					Point center = centerPoint(points);
					transformPoint(m, center);
					tran->p0.x = center.x;
					tran->p0.y = center.y;
					tran->p0.z = center.z;
					inputStr = inputStr.substr(6);
					sscanf_s(inputStr.c_str(), ", %f, %f, %f, %f",
						&tran->u.x,
						&tran->u.y,
						&tran->u.z,
						&tran->theta);
				}
				else
				{
					sscanf_s(inputStr.c_str(), "%f, %f, %f, %f, %f, %f, %f",
						&tran->p0.x,
						&tran->p0.y,
						&tran->p0.z,
						&tran->u.x,
						&tran->u.y,
						&tran->u.z,
						&tran->theta);
				}

				m = rotate(tran->p0, tran->u, tran->theta) * m;
			}
			else if (inputStr == "S")
			{
				Shear* tran = (Shear*)transform;
				printf("input shear parameters: point: x, y, z; x axis: ux.x, ux.y, ux.z; \
y axis: uy.x, uy.y, uy.z; z axis: uz.x, uz.y, uz.z; refrence axis name: reference; refrence value: refValue; \
shear axis1 name(\"n\" to skip): shear1; shear axis1 value: sh1; shear axis2 name(\"n\" to skip): shear2; shear axis2 value: sh2. \"exit\" to end:");
				std::cin >> inputStr;
				if (inputStr == "exit")
					break;
				sscanf_s(inputStr.c_str(), "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %c, %f, %c, %f, %c, %f",
					&tran->p0.x,
					&tran->p0.y,
					&tran->p0.z,
					&tran->ux.x,
					&tran->ux.y,
					&tran->ux.z,
					&tran->uy.x,
					&tran->uy.y,
					&tran->uy.z,
					&tran->uz.x,
					&tran->uz.y,
					&tran->uz.z,
					&tran->reference, 1,
					&tran->refValue,
					&tran->shear1, 1,
					&tran->sh1,
					&tran->shear2, 1,
					&tran->sh2);

				m = shear(tran->p0, tran->ux, tran->uy, tran->uz, tran->reference, tran->refValue, tran->shear1, tran->shear2, tran->sh1, tran->sh2) * m;
			}
			transforms.push_back(transform);
		}
	}
	return transforms;
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, winWidth / 2, winHeight);
	drawCoordinate(-190, 190, -290, 290);
	drawPolygon(points);

	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
	drawCoordinate(-190, 190, -290, 290);

	for (auto it = transforms.rbegin(); it != transforms.rend(); it++)
	{
		auto& transform = *it;
		if (transform->type == "Re")
		{
			Reflect* tran = (Reflect*)transform;
			reflect(tran->p0, tran->u);
		}
		else if (transform->type == "R")
		{
			Rotate* tran = (Rotate*)transform;
			rotate(tran->p0, tran->u, tran->theta);
		}
		else if (transform->type == "S")
		{
			Shear* tran = (Shear*)transform;
			shear(tran->p0, tran->ux, tran->uy, tran->uz, tran->reference, tran->refValue, tran->shear1, tran->shear2, tran->sh1, tran->sh2);
		}
	}
	drawPolygon(points);

	glFlush();
}

void code_9_exercise_17()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2);

	points = inputPoints();
	transforms = inputTransform();

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_9_EXERCISE_ADD_1
// 打开两者算法的其中之一
//#define SUTHERLAN_HODGMAN
#define WEILER_ATHERTON
struct Rect
{
	float x;
	float y;
	float z;
	float width;
	float height;
};
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
		return{ _pathLines[curPathIdx].begin.x + curLength * _pathLines[curPathIdx].cos, _pathLines[curPathIdx].begin.y + curLength * _pathLines[curPathIdx].sin, 0};
	}
};
Matrix translateMatrix(float tx, float ty, float tz)
{
	// 平移
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][3] = tx;
	ret[1][3] = ty;
	ret[2][3] = tz;
	return ret;
}
Matrix rotateMatrix(float theta)
{
	// 基于原点旋转
	Matrix ret(4, 4);
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
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][0] = std::cos(theta);
	ret[0][1] = -std::sin(theta);
	ret[0][3] = p.x * (1 - std::cos(theta)) + p.y * std::sin(theta);
	ret[1][0] = std::sin(theta);
	ret[1][1] = std::cos(theta);
	ret[1][3] = p.y * (1 - std::cos(theta)) - p.x * std::sin(theta);
	return ret;
}
Matrix scaleMatrix(float sx, float sy, float sz)
{
	// 基于原点缩放
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[1][1] = sy;
	ret[2][2] = sz;
	return ret;
}
Matrix scaleByPointMatrix(Point p, float sx, float sy, float sz)
{
	// 基于指定点缩放
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[0][3] = p.x * (1 - sx);
	ret[1][1] = sy;
	ret[1][3] = p.y * (1 - sy);
	ret[2][2] = sz;
	ret[2][3] = p.z * (1 - sz);
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

//////////////////////////////////////////////////////////////////////////
// 裁剪
#ifdef SUTHERLAN_HODGMAN
//////////////////////////////////////////////////////////////////////////
// Sutherlan-Hodgman多边形裁剪算法(凸多边形)
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
typedef enum { None = -1, Left, Right, Bottom, Top } Boundary;
enum class AntiClockBoundary
{
	None = -1, Left, Bottom, Right, Top
};
const GLint nClip = 4;
vector<Point> sPoint[nClip];
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
GLint cross(Point p1, Point p2, Boundary winEdge, Point wMin, Point wMax)
{
	if (inside(p1, winEdge, wMin, wMax) == inside(p2, winEdge, wMin, wMax))
		return (false);
	else
		return (true);
}
Point intersect2(Point p1, Point p2, Boundary winEdge, Point wMin, Point wMax, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo)
{
	Point iPt = {0, 0, 0};
	GLfloat m;

	if (p1.x != p2.x)
		m = (p1.y - p2.y) / (p1.x - p2.x);
	switch (winEdge)
	{
	case Boundary::Left:
		iPt.x = wMin.x;
		iPt.y = p2.y + (wMin.x - p2.x) * m;

		if (iPt.y >= wMin.y && iPt.y <= wMax.y)
			_boundaryOrderInfo[AntiClockBoundary::Left].push_back(iPt);

		break;
	case Boundary::Right:
		iPt.x = wMax.x;
		iPt.y = p2.y + (wMax.x - p2.x) * m;

		if (iPt.y >= wMin.y && iPt.y <= wMax.y)
			_boundaryOrderInfo[AntiClockBoundary::Right].push_back(iPt);

		break;
	case Boundary::Bottom:
		iPt.y = wMin.y;
		if (p1.x != p2.x)
			iPt.x = p2.x + (wMin.y - p2.y) / m;
		else
			iPt.x = p2.x;

		if (iPt.x >= wMin.x && iPt.x <= wMax.x)
			_boundaryOrderInfo[AntiClockBoundary::Bottom].push_back(iPt);

		break;
	case Boundary::Top:
		iPt.y = wMax.y;
		if (p1.x != p2.x)
			iPt.x = p2.x + (wMax.y - p2.y) / m;
		else
			iPt.x = p2.x;

		if (iPt.x >= wMin.x && iPt.x <= wMax.x)
			_boundaryOrderInfo[AntiClockBoundary::Top].push_back(iPt);

		break;
	default:
		break;
	}

	return (iPt);
}
void clipPoint2(Point p, Boundary winEdge, Point wMin, Point wMax, Point* pOut, int* cnt, Point* first[], Point* s, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo)
{
	Point iPt = {0, 0, 0};
	if (!first[winEdge])
		first[winEdge] = new Point{ p.x, p.y, 0 };
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
void closeClip2(Point wMin, Point wMax, Point* pOut, GLint* cnt, Point* first[], Point* s, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo)
{
	Point pt = {0, 0, 0};
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
GLint polygonClipSuthHodg2(Point wMin, Point wMax, GLint n, Point* pIn, Point* pOut, std::map<AntiClockBoundary, std::vector<Point>>& _boundaryOrderInfo)
{
	for (int i = 0; i < 4; i++)
	{
		sPoint[i].clear();
	}
	Point* first[nClip] = { 0, 0, 0, 0 }, s[nClip];
	GLint k, cnt = 0;
	for (k = 0; k < n; k++)
		clipPoint2(pIn[k], Boundary::Left, wMin, wMax, pOut, &cnt, first, s, _boundaryOrderInfo);
	closeClip2(wMin, wMax, pOut, &cnt, first, s, _boundaryOrderInfo);

	/*printf("==================================================\n");
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
	}*/
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

// 分割边界上的多组顶点 算法
void polygonClipSutherlanHodgman(const std::vector<Point>& polygon, Point wMin, Point wMax, std::vector<std::vector<Point>>& result)
{
	std::map<AntiClockBoundary, std::vector<Point>> _boundaryOrderInfo;

	std::vector<Point> points(32, { 0, 0, 0 });
	int n = polygonClipSuthHodg2(wMin, wMax, polygon.size(), (Point*)&polygon[0], &points[0], _boundaryOrderInfo);
	if (n)
	{
		points.resize(n);
		cutResult(points, _boundaryOrderInfo, result);
	}
}
//////////////////////////////////////////////////////////////////////////
#endif

#ifdef WEILER_ATHERTON
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
enum class PointType
{
	None,    // 无类型
	Polygon, // 多边形点
	ClipWindow,	 // 裁剪窗口点
				 //SAME,    // 共同点
				 CrossIn, // 裁剪窗口进交点
				 CrossOut, //裁剪窗口出交点
};
struct CrossPointInfo
{
	Point point = { 0, 0, 0 };
	PointType type = PointType::None;
	float u1 = 0.0f;	// 裁剪窗口边 直线参数方程u
	float u2 = 0.0f;	// 多边形边	直线参数方程u
	int lineIdx1 = 0; // 裁剪窗口边索引
	int lineIdx2 = 0; // 多边形边索引
};
struct LineWithCross
{
	Point begin;
	Point end;
	std::map<float, std::vector<CrossPointInfo>> crossPoints;
};
struct PointInfo
{
	Point point = { 0, 0, 0 };
	PointType type = PointType::None; // 类型
	int idx1 = -1;   // 裁剪窗口顶点 数组索引
	int idx2 = -1;	// 多边形顶点 数组索引
	bool dealed = false; // 是否已处理
};
//////////////////////////////////////////////////////////////////////////
// 检查内外
bool sign(float f)
{
	return f > 0;
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
		Point end = { p.x + length * cos(theta), p.y + length * sin(theta), 0 };
		if (checkRay(p, end, polygon))
		{
			float u1 = 0, u2 = 0;
			int count = 0;
			for (int i = 0; i < polygon.size(); i++)
			{
				int next = i + 1 < polygon.size() ? i + 1 : 0;
				if (crossPoint(p, end, polygon[i], polygon[next], u1, u2))
				{
					if (cross({ end.x - p.x, end.y - p.y, 0 },
					{ polygon[next].x - polygon[i].x, polygon[next].y - polygon[i].y, 0 }).z > 0)
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
bool crossPoint(LineWithCross& line1, LineWithCross& line2, float& u1, float& u2)
{
	float dx1 = line1.end.x - line1.begin.x;
	float dy1 = line1.end.y - line1.begin.y;
	float dx2 = line2.end.x - line2.begin.x;
	float dy2 = line2.end.y - line2.begin.y;

	if (dx1 == 0 && dx2 == 0)
		return false;

	if (dy1 / dx1 == dy2 / dx2)
		return false;

	float x01 = line1.begin.x;
	float y01 = line1.begin.y;
	float x02 = line2.begin.x;
	float y02 = line2.begin.y;
	u1 = (dy2 * (x02 - x01) + dx2 * (y01 - y02)) / (dy2 * dx1 - dy1 * dx2);
	u2 = (dy1 * (x01 - x02) + dx1 * (y02 - y01)) / (dy1 * dx2 - dy2 * dx1);
	if ((u1 < 0 || u1 > 1) || (u2 < 0 || u2 > 1))
		return false;

	return true;
}
Point calcCrossPointByU(const Point& begin, const Point& end, float u)
{
	return{ begin.x + (end.x - begin.x) * u, begin.y + (end.y - begin.y) * u, 0 };
}
void calcLines(std::vector<Point>& points, std::vector<LineWithCross>& lines)
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
bool calcCrossPoint(LineWithCross& clipWindowLine, LineWithCross& polygonLine, CrossPointInfo& crossPointInfo)
{
	if (!crossPoint(clipWindowLine, polygonLine, crossPointInfo.u1, crossPointInfo.u2))
		return false;

	float dx1 = clipWindowLine.end.x - clipWindowLine.begin.x;
	float dy1 = clipWindowLine.end.y - clipWindowLine.begin.y;
	float dx2 = polygonLine.end.x - polygonLine.begin.x;
	float dy2 = polygonLine.end.y - polygonLine.begin.y;
	float x01 = clipWindowLine.begin.x;
	float y01 = clipWindowLine.begin.y;

	if (dx1 * dy2 - dy1 * dx2 > 0)
	{
		crossPointInfo.type = PointType::CrossIn;
	}
	else
	{
		crossPointInfo.type = PointType::CrossOut;
	}
	return true;
}
PointType combinePointType(PointType type1, PointType type2)
{
	if (type1 == PointType::CrossIn)
	{
		if (type2 == PointType::CrossIn)
			return PointType::CrossIn;
		else if (type2 == PointType::CrossOut)
			return PointType::None;
	}
	else if (type1 == PointType::CrossOut)
	{
		if (type2 == PointType::CrossIn)
			return PointType::None;
		else if (type2 == PointType::CrossOut)
			return PointType::CrossOut;
	}
}
bool sameEdge(LineWithCross& e1, LineWithCross& e2)
{
	bool ret = false;
	float dx1 = e1.end.x - e1.begin.x;
	float dy1 = e1.end.y - e1.begin.y;
	float dx2 = e2.end.x - e2.begin.x;
	float dy2 = e2.end.y - e2.begin.y;

	if (dx1 * dy2 - dy1 * dx2 == 0)
	{
		if (dx1 * dx2 >= 0 && dy1 * dy2 >= 0)
			ret = e1.begin == e2.begin || e1.end == e2.end;
		else
			ret = e1.begin == e2.end || e1.end == e2.begin;
	}
	return ret;
}
PointType calcEdgeType(LineWithCross& e, float crossU, std::vector<Point>& clipWindow)
{
	if (crossU == 1.f)
	{
		float u = 0.f;
		for (auto it = e.crossPoints.begin(); it != e.crossPoints.end(); it++)
		{
			if (it->first < crossU && it->first > u)
				u = it->first;
		}
		u = (crossU + u) / 2;
		Point testP = { e.begin.x + u * (e.end.x - e.begin.x), e.begin.y + u * (e.end.y - e.begin.y), 0 };
		return checkIn(testP, clipWindow) ? PointType::CrossOut : PointType::CrossIn;
	}
	else if (crossU == 0.f)
	{
		float u = 1.f;
		for (auto it = e.crossPoints.begin(); it != e.crossPoints.end(); it++)
		{
			if (it->first > crossU && it->first < u)
				u = it->first;
		}
		u = (crossU + u) / 2;
		Point testP = { e.begin.x + u * (e.end.x - e.begin.x), e.begin.y + u * (e.end.y - e.begin.y), 0 };
		return checkIn(testP, clipWindow) ? PointType::CrossIn : PointType::CrossOut;
	}
}
void calcPointInfo(std::vector<Point>& clipWindow, std::vector<Point>& polygon, std::vector<PointInfo>& clipWindowPointInfos, std::vector<PointInfo>& polygonPointInfos)
{
	std::vector<LineWithCross> clipWindowLines;
	std::vector<LineWithCross> polygonLines;
	calcLines(clipWindow, clipWindowLines);
	calcLines(polygon, polygonLines);
	std::vector<Point> spNonePoints;

	CrossPointInfo crossPointInfo;
	for (int i = 0; i < clipWindowLines.size(); i++)
	{
		for (int j = 0; j < polygonLines.size(); j++)
		{
			if (calcCrossPoint(clipWindowLines[i], polygonLines[j], crossPointInfo))
			{
				crossPointInfo.point = calcCrossPointByU(polygonLines[j].begin, polygonLines[j].end, crossPointInfo.u2);
				crossPointInfo.lineIdx1 = i;
				crossPointInfo.lineIdx2 = j;
				polygonLines[j].crossPoints[crossPointInfo.u2].push_back(crossPointInfo);
			}
		}
	}

	for (int i = 0; i < polygonLines.size(); i++)
	{
		for (auto& it : polygonLines[i].crossPoints)
		{

			if (it.first == 0.f || it.first == 1.f)
			{
				if (it.second.size() == 1 &&
					(it.second[0].u1 != 0 && it.second[0].u1 != 1))
				{
					int nexti = -1;
					float nextu2 = -1.f;
					if (it.first == 0.f)
					{
						nexti = (i - 1 + polygonLines.size()) % polygonLines.size();
						nextu2 = 1.f;
					}
					else if (it.first == 1.f)
					{
						nexti = (i + 1) % polygonLines.size();
						nextu2 = 0.f;
					}

					PointType type;
					if (polygonLines[nexti].crossPoints.find(nextu2) != polygonLines[nexti].crossPoints.end() &&
						polygonLines[nexti].crossPoints[nextu2][0].lineIdx1 == it.second[0].lineIdx1)
					{
						type = combinePointType(it.second[0].type, polygonLines[nexti].crossPoints[nextu2][0].type);
						polygonLines[nexti].crossPoints.erase(nextu2);
					}
					else
					{
						type = it.second[0].type;
					}

					auto cp = it.second[0];
					it.second.clear();

					PointType type1, type2;
					if (type == PointType::None)
					{
						type2 = PointType::Polygon;
						type1 = PointType::ClipWindow;

						spNonePoints.push_back(cp.point);
					}
					else
					{
						type2 = type;
						type1 = type;
					}
					cp.type = type2;
					it.second.push_back(cp);

					cp.type = type1;
					clipWindowLines[cp.lineIdx1].crossPoints[cp.u1].push_back(cp);
				}
				else if (it.second.size() > 0)
				{
					int nexti = -1;
					float nextu2 = -1.f;
					if (it.first == 0.f)
					{
						nexti = (i - 1 + polygonLines.size()) % polygonLines.size();
						nextu2 = 1.f;
					}
					else if (it.first == 1.f)
					{
						nexti = (i + 1) % polygonLines.size();
						nextu2 = 0.f;
					}

					PointType type;
					PointType type1;
					PointType type2;

					bool same1 = false;
					assert(polygonLines[nexti].crossPoints[nextu2].size() > 0);
					for (auto& cp : polygonLines[nexti].crossPoints[nextu2])
					{
						if (sameEdge(polygonLines[i], clipWindowLines[cp.lineIdx1]))
						{
							same1 = true;
							break;
						}
					}
					if (!same1)
					{
						if (it.second.size() == 1)
						{
							type1 = it.second[0].type;
						}
						else
						{
							type1 = combinePointType(it.second[0].type, it.second[1].type);
							if (type1 == PointType::None)
							{
								type1 = calcEdgeType(polygonLines[i], it.first, clipWindow);
							}
						}
					}

					bool same2 = false;
					for (auto& cp : it.second)
					{
						if (sameEdge(polygonLines[nexti], clipWindowLines[cp.lineIdx1]))
						{
							same2 = true;
							break;
						}
					}
					if (!same2)
					{
						if (polygonLines[nexti].crossPoints[nextu2].size() == 1)
						{
							type2 = polygonLines[nexti].crossPoints[nextu2][0].type;
						}
						else
						{
							type2 = combinePointType(polygonLines[nexti].crossPoints[nextu2][0].type, polygonLines[nexti].crossPoints[nextu2][1].type);
							if (type2 == PointType::None)
							{
								type2 = calcEdgeType(polygonLines[nexti], nextu2, clipWindow);
							}
						}
					}

					if (same1 && same2)
					{
						//type = PointType::SAME;
						type = PointType::CrossIn;
					}
					else if (!same1 && !same2)
					{
						type = combinePointType(type1, type2);
					}
					else if (same1 && !same2)
					{
						type = type2;
					}
					else if (!same1 && same2)
					{
						type = type1;
					}

					if (type == PointType::None)
					{
						type2 = PointType::Polygon;
						type1 = PointType::ClipWindow;

						spNonePoints.push_back(it.second[0].point);
					}
					else
					{
						type2 = type;
						type1 = type;
					}
					auto cp = it.second[0];
					it.second.clear();
					polygonLines[nexti].crossPoints.erase(nextu2);
					cp.type = type2;
					it.second.push_back(cp);

					cp.type = type1;
					clipWindowLines[cp.lineIdx1].crossPoints[cp.u1].push_back(cp);
				}
			}
			else if (it.second.size() == 2)
			{
				PointType type = combinePointType(it.second[0].type, it.second[1].type);
				PointType type1, type2;
				if (type == PointType::None)
				{
					type2 = PointType::Polygon;
					type1 = PointType::ClipWindow;

					spNonePoints.push_back(it.second[0].point);
				}
				else
				{
					type2 = type;
					type1 = type;
				}
				auto cp = it.second[0];
				it.second.clear();
				cp.type = type2;
				it.second.push_back(cp);

				cp.type = type1;
				clipWindowLines[cp.lineIdx1].crossPoints[cp.u1].push_back(cp);

			}
			else if (it.second.size() == 1)
			{
				auto cp = it.second[0];
				clipWindowLines[cp.lineIdx1].crossPoints[cp.u1].push_back(cp);
			}
			else
			{
				//assert(0);
			}
		}
	}

	for (int i = 0; i < clipWindowLines.size(); i++)
	{
		bool find = false;
		int nexti = (i - 1 + clipWindowLines.size()) % clipWindowLines.size();
		auto it = clipWindowLines[i].crossPoints.find(0.f);
		if (it != clipWindowLines[i].crossPoints.end())
		{
			find = true;
		}
		else
		{
			it = clipWindowLines[nexti].crossPoints.find(1.f);
			find = it != clipWindowLines[nexti].crossPoints.end();
		}

		PointInfo pi;
		if (find)
		{
			//assert(it->second.size() == 1);
			pi.type = it->second[0].type;
		}
		else
		{
			pi.type = PointType::ClipWindow;
		}
		pi.point = clipWindowLines[i].begin;
		clipWindowPointInfos.push_back(pi);

		for (auto& it : clipWindowLines[i].crossPoints)
		{
			if (it.first != 0.f && it.first != 1.f)
			{
				//assert(it.second.size() == 1);

				pi.point = it.second[0].point;
				pi.type = it.second[0].type;
				clipWindowPointInfos.push_back(pi);
			}
		}
	}

	for (int i = 0; i < polygonLines.size(); i++)
	{
		bool find = false;
		int nexti = (i - 1 + polygonLines.size()) % polygonLines.size();
		auto it = polygonLines[i].crossPoints.find(0.f);
		if (it != polygonLines[i].crossPoints.end())
		{
			find = true;
		}
		else
		{
			it = polygonLines[nexti].crossPoints.find(1.f);
			find = it != polygonLines[nexti].crossPoints.end();
		}

		PointInfo pi;
		if (find)
		{
			//assert(it->second.size() == 1);
			pi.type = it->second[0].type;
		}
		else
		{
			pi.type = PointType::Polygon;
		}
		pi.point = polygonLines[i].begin;
		polygonPointInfos.push_back(pi);

		for (auto& it : polygonLines[i].crossPoints)
		{
			if (it.first != 0.f && it.first != 1.f)
			{
				//assert(it.second.size() == 1);

				pi.point = it.second[0].point;
				pi.type = it.second[0].type;
				polygonPointInfos.push_back(pi);
			}
		}
	}

	int size = polygonPointInfos.size();
	for (int i = 0; i < size; i++)
	{
		bool findFirstSp = false;
		for (auto & sp : spNonePoints)
		{
			if (polygonPointInfos[0].point == sp)
			{
				findFirstSp = true;
				polygonPointInfos.push_back(polygonPointInfos[0]);
				polygonPointInfos.erase(polygonPointInfos.begin());
				break;
			}
		}

		if (!findFirstSp)
			break;
	}

	for (int i = 0; i < clipWindowPointInfos.size(); i++)
	{
		for (int j = 0; j < polygonPointInfos.size(); j++)
		{
			if (clipWindowPointInfos[i].point == polygonPointInfos[j].point &&
				(clipWindowPointInfos[i].type == PointType::CrossIn ||
					clipWindowPointInfos[i].type == PointType::CrossOut/* ||
																	   clipWindowPointInfos[i].type == PointType::SAME*/))
			{
				clipWindowPointInfos[i].idx1 = i;
				clipWindowPointInfos[i].idx2 = j;
				polygonPointInfos[j].idx1 = i;
				polygonPointInfos[j].idx2 = j;
			}
		}
	}
}
void genResultPolygon(Point closePoint, std::vector<std::vector<Point>>& reslutPolygon, std::map<Point, int>& resultPointIdx)
{
	auto it = resultPointIdx.find(closePoint);
	auto& result = reslutPolygon.back();
	if (result.size() > 0 && it != resultPointIdx.end())
	{
		if ((int)result.size() - it->second >= 3)
		{
			if (it->second > 0)
			{
				result.erase(result.begin(), result.begin() + it->second);
			}
		}
		else
		{
			result.clear();
		}
	}

	if (result.size() > 0 && result.size() < 3)
	{
		result.clear();
	}
	resultPointIdx.clear();

	if (!result.empty())
		reslutPolygon.push_back(std::vector<Point>());
}
void addResultPoint(Point p, std::vector<std::vector<Point>>& reslutPolygon, std::map<Point, int>& resultPointIdx)
{
	auto it = resultPointIdx.find(p);
	auto& result = reslutPolygon.back();
	result.push_back(p);
	if (it == resultPointIdx.end())
	{
		resultPointIdx[p] = (int)result.size() - 1;
	}
	else
	{
		if ((int)result.size() - 1 - it->second >= 3)
		{
			reslutPolygon.insert(reslutPolygon.end() - 1, std::vector<Point>(result.begin() + it->second + 1, result.end()));
			assert(reslutPolygon.at(reslutPolygon.size() - 2).size() >= 3);
		}
		auto& result = reslutPolygon.back();
		for (int i = it->second + 1; i < (int)result.size() - 1; i++)
		{
			if (resultPointIdx.find(result[i]) != resultPointIdx.end())
				resultPointIdx.erase(result[i]);
		}
		result.erase(result.begin() + it->second + 1, result.end());
	}

}
void walkClipWindow(int idx, std::vector<PointInfo>& clipWindowPointInfos, std::vector<PointInfo>& polygonPointInfos, std::vector<std::vector<Point>>& reslutPolygon, std::map<Point, int>& resultPointIdx);
void walkPolygon(int idx, bool record, std::vector<PointInfo>& clipWindowPointInfos, std::vector<PointInfo>& polygonPointInfos, std::vector<std::vector<Point>>& reslutPolygon, std::map<Point, int>& resultPointIdx)
{
	auto& pointInfo = polygonPointInfos[idx];
	if (pointInfo.type == PointType::CrossIn)
	{
		record = true;
	}
	else if (pointInfo.type == PointType::CrossOut)
	{
		record = true;
	}
	else if (pointInfo.type == PointType::Polygon)
	{

	}

	if (pointInfo.dealed)
	{
		genResultPolygon(pointInfo.point, reslutPolygon, resultPointIdx);
		return;
	}

	pointInfo.dealed = true;
	if (pointInfo.idx1 >= 0)
		clipWindowPointInfos[pointInfo.idx1].dealed = true;

	if (record)
		addResultPoint(pointInfo.point, reslutPolygon, resultPointIdx);

	idx = idx + 1 >= polygonPointInfos.size() ? 0 : idx + 1;
	if (pointInfo.type == PointType::CrossIn)
	{
		walkPolygon(idx, true, clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
	}
	else if (pointInfo.type == PointType::CrossOut)
	{
		int nextCidx = pointInfo.idx1 + 1 >= clipWindowPointInfos.size() ? 0 : pointInfo.idx1 + 1;
		walkClipWindow(nextCidx, clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
		walkPolygon(idx, false, clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
	}
	else if (pointInfo.type == PointType::Polygon)
	{
		walkPolygon(idx, record, clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
	}
	else
	{
		assert(0);
	}
}
void walkClipWindow(int idx, std::vector<PointInfo>& clipWindowPointInfos, std::vector<PointInfo>& polygonPointInfos, std::vector<std::vector<Point>>& reslutPolygon, std::map<Point, int>& resultPointIdx)
{
	auto& pointInfo = clipWindowPointInfos[idx];

	if (pointInfo.dealed)
	{
		genResultPolygon(pointInfo.point, reslutPolygon, resultPointIdx);
		return;
	}

	pointInfo.dealed = true;
	if (pointInfo.idx2 >= 0)
		polygonPointInfos[pointInfo.idx2].dealed = true;

	addResultPoint(pointInfo.point, reslutPolygon, resultPointIdx);

	idx = idx + 1 >= clipWindowPointInfos.size() ? 0 : idx + 1;
	if (pointInfo.type == PointType::CrossIn)
	{
		int nextPidx = pointInfo.idx2 + 1 >= polygonPointInfos.size() ? 0 : pointInfo.idx2 + 1;
		walkPolygon(nextPidx, true, clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
	}
	else if (pointInfo.type == PointType::CrossOut)
	{
		walkClipWindow(idx, clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
		int nextPidx = pointInfo.idx2 + 1 >= polygonPointInfos.size() ? 0 : pointInfo.idx2 + 1;
		walkPolygon(nextPidx, false, clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
	}
	else if (pointInfo.type == PointType::ClipWindow)
	{
		walkClipWindow(idx, clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
	}
	else
	{
		assert(0);
	}
}
void polygonClipWeilerAtherton(std::vector<Point>& clipWindow, std::vector<Point>& polygon, std::vector<std::vector<Point>>& reslutPolygon)
{
	std::vector<PointInfo> clipWindowPointInfos;
	std::vector<PointInfo> polygonPointInfos;
	std::map<Point, int> resultPointIdx;

	calcPointInfo(clipWindow, polygon, clipWindowPointInfos, polygonPointInfos);

	reslutPolygon.clear();
	reslutPolygon.push_back(std::vector<Point>());
	walkPolygon(0, checkIn(polygonPointInfos[0].point, clipWindow), clipWindowPointInfos, polygonPointInfos, reslutPolygon, resultPointIdx);
	if (reslutPolygon.back().empty())
		reslutPolygon.erase(reslutPolygon.end() - 1);
}
#endif

//////////////////////////////////////////////////////////////////////////
// 线段裁剪算法
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
bool lineClipCohSuth(Point winMin, Point winMax, Point& p1, Point& p2)
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
	return plotLine;
}
//////////////////////////////////////////////////////////////////////////
std::vector<Point> road = { { -400, 0, 0 },{ 0, 0, 0 },{ 1000, 0, 0 },{ 1200, 150, 0 },{ 1600, 150, 0 },{ 2000, 0, 0 },{ 2600, 0, 0 },{ 2600, 200, 0 },{ 2800, 200, 0 } ,{ 2800, 0, 0 } ,{ 3200, 0, 0 },{ 3600, 0, 0 } };
std::vector<Point> car = { { 0, 0, 0 },{ -70, 0, 0 },{ -70, -12, 0 },{ 30, -12, 0 },{ 30, -2, 0 },{ 15, 26, 0 },{ 0, 26, 0 } };
std::vector<Point> goods = { { -30, -20, 0 },{ 30, -20, 0 },{ 30, 20, 0 },{ -30, 20, 0 } };
Point wheelPoint = { 0, 0, 0 };
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

Rect window = {};
Rect viewPort = {};

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
Point deltaPosition;
float curDirection;
float wheelRadius = 0;
Path* path = nullptr;

// 裁剪窗口
float clipwinRotate = 0;
float deltaClipwinRotate = 5 * PI / 180;
Point clipwinPos = { 0, 0, 0 }; // 中心点
float deltaClipwinPos = 5;
float clipwinWidth = 200;
float clipwinHeight = 150;
float deltaClipwinSize = 10;
bool clipwinFollow = true;
bool clipwinMode = false;

// 视口
float xvmin = 570, yvmin = 420, xvmax = 770, yvmax = 570;
float xnvmin = xvmin / winWidth;
float xnvmax = xvmax / winWidth;
float ynvmin = yvmin / winHeight;
float ynvmax = yvmax / winHeight;
bool viewTransformDirty = true;

std::vector<std::vector<Point>> viewCar;
std::vector<std::vector<Point>> viewGoods;
std::vector<std::vector<Point>> viewWheelHolder;
std::vector<Point> viewWheels1;
std::vector<Point> viewWheels2;
std::vector<std::vector<Point>> viewRoad;

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
	glVertex3f(p.x, p.y, p.z);
	glEnd();
}
void drawPoints(const std::vector<Point>& points)
{
	glBegin(GL_POINTS);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}
void drawStrip(const std::vector<Point>& points)
{
	glBegin(GL_LINE_STRIP);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}
void drawLoop(const std::vector<Point>& points)
{
	glBegin(GL_LINE_LOOP);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}
void circlePlot(Point p0, Point p, std::vector<Point>& points)
{
	points.push_back({ p0.x + p.x, p0.y + p.y, 0 });
	points.push_back({ p0.x - p.x, p0.y + p.y, 0 });
	points.push_back({ p0.x + p.x, p0.y - p.y, 0 });
	points.push_back({ p0.x - p.x, p0.y - p.y, 0 });
	points.push_back({ p0.x + p.y, p0.y + p.x, 0 });
	points.push_back({ p0.x - p.y, p0.y + p.x, 0 });
	points.push_back({ p0.x + p.y, p0.y - p.x, 0 });
	points.push_back({ p0.x - p.y, p0.y - p.x, 0 });
}
void circle(Point p0, float r, std::vector<Point>& points)
{
	points.clear();
	float x = r;
	float y = 0;
	int d2x = 2 * r;
	int d2y = 0;
	int p = 1 - r;
	circlePlot(p0, { x, y, 0 }, points);
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
		circlePlot(p0, { x, y, 0 }, points);
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
	points.push_back({ p0.x + x, p0.y + y, 0 });
	points.push_back({ p0.x - x, p0.y + y, 0 });
	points.push_back({ p0.x + x, p0.y - y, 0 });
	points.push_back({ p0.x - x, p0.y - y, 0 });
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
		drawStrip({ curWheelPoint1, p });
	}
	drawPoints(curWheel2);
	for (auto & p : curWheelHolder2)
	{
		drawStrip({ curWheelPoint2, p });
	}
}
Matrix updateViewCoord()
{
	// 这里将观察坐标系原点默认为裁剪窗口中心点。其实观察坐标系的原点影响的就是点在观察坐标系中
	// 的绝对坐标值，但这个绝对坐标值的数值并不重要，重要的是点和裁剪窗口的相对位置关系，因为最终视
	// 口中显示的是裁剪后的内容，而裁剪后的内容就是这个相对位置关系决定的，跟两者的绝对坐标值无关。
	// 而由于是刚体变换（旋转，平移），相对位置关系都不会变，所以，为了方便后续计算(不然裁剪窗口位
	// 置还要计算)，这里选观察坐标系原点为裁剪窗口中心点（裁剪窗口变换后在观察坐标系的原点）

	return rotateMatrix(-clipwinRotate) * translateMatrix(-clipwinPos.x, -clipwinPos.y, 0);
}
Matrix updateNormalView()
{
	float xwmin = -clipwinWidth / 2;
	float xwmax = clipwinWidth / 2;
	float ywmin = -clipwinHeight / 2;
	float ywmax = clipwinHeight / 2;

	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][0] = (xnvmax - xnvmin) / (xwmax - xwmin);
	ret[1][1] = (ynvmax - ynvmin) / (ywmax - ywmin);
	ret[0][3] = (xwmax * xnvmin - xwmin * xnvmax) / (xwmax - xwmin);
	ret[1][3] = (ywmax * ynvmin - ywmin * ynvmax) / (ywmax - ywmin);
	return ret;
}
Matrix updateViewport()
{
	static auto ret = scaleMatrix(winWidth, winHeight, 1);

	return ret;
}
void clipPolygon(std::vector<Point>& in, std::vector<std::vector<Point>>& out)
{
	out.clear();

	if (in.size())
	{
#ifdef SUTHERLAN_HODGMAN
		polygonClipSutherlanHodgman(in, { xnvmin , ynvmin, 0 }, { xnvmax, ynvmax, 0 }, out);
#endif

#ifdef WEILER_ATHERTON
		polygonClipWeilerAtherton(std::vector<Point>({ { xnvmin , ynvmin, 0 } ,
		{ xnvmax , ynvmin, 0 },
		{ xnvmax, ynvmax, 0 } ,
		{ xnvmin, ynvmax, 0 } }), in, out);
#endif
	}
}
void clipLine(const Point& p1, const Point& p2, std::vector<Point>& out)
{
	out.clear();
	auto _p1 = p1;
	auto _p2 = p2;
	if (lineClipCohSuth({ xnvmin , ynvmin, 0 }, { xnvmax, ynvmax, 0 }, _p1, _p2))
	{
		out.push_back(_p1);
		out.push_back(_p2);
	}
}
void clipPoint(std::vector<Point>& points)
{
	for (auto it = points.begin(); it < points.end();)
	{
		if (it->x >= xvmin && it->x <= xvmax && it->y >= yvmin && it->y <= yvmax)
			it++;
		else
			it = points.erase(it);

	}
}
void updateView()
{
	static Matrix viewTransform(4, 4);
	if (viewTransformDirty)
	{
		matrixSetIdentity(viewTransform);

		viewTransform = updateNormalView() * updateViewCoord();
	}

	static std::vector<Point> temp;
	static Point tempp1;
	static Point tempp2;
	bool turnOrder = (clipwinWidth > 0 && clipwinHeight < 0 || clipwinWidth < 0 && clipwinHeight > 0);
	//////////////////////////////////////////////////////////////////////////
	temp = curCar;
	transformPoints(viewTransform, temp);
	if (turnOrder)
	{
		std::reverse(temp.begin(), temp.end());
	}
	clipPolygon(temp, viewCar);
	for (auto & c : viewCar)
		transformPoints(updateViewport(), c);

	//////////////////////////////////////////////////////////////////////////
	temp = curGoods;
	transformPoints(viewTransform, temp);
	if (turnOrder)
	{
		std::reverse(temp.begin(), temp.end());
	}
	clipPolygon(temp, viewGoods);
	for (auto & c : viewGoods)
		transformPoints(updateViewport(), c);

	//////////////////////////////////////////////////////////////////////////
	tempp1 = curWheelPoint1;
	transformPoint(viewTransform, tempp1);

	tempp2 = curWheelPoint2;
	transformPoint(viewTransform, tempp2);

	//////////////////////////////////////////////////////////////////////////
	viewWheelHolder.clear();
	temp = curWheelHolder1;
	transformPoints(viewTransform, temp);
	for (auto& wheel : temp)
	{
		static std::vector<Point> tempLine;
		clipLine(tempp1, wheel, tempLine);
		if (tempLine.size())
		{
			transformPoints(updateViewport(), tempLine);
			viewWheelHolder.push_back(tempLine);
		}
	}

	temp = curWheelHolder2;
	transformPoints(viewTransform, temp);
	for (auto& wheel : temp)
	{
		static std::vector<Point> tempLine;
		clipLine(tempp2, wheel, tempLine);
		if (tempLine.size())
		{
			transformPoints(updateViewport(), tempLine);
			viewWheelHolder.push_back(tempLine);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 直接对点进行处理
	//temp = curWheel1;
	//transformPoints(viewTransform, temp);
	//transformPoints(updateViewport(), temp);
	//clipPoint(temp);
	//viewWheels1 = temp;

	//temp = curWheel2;
	//transformPoints(viewTransform, temp);
	//transformPoints(updateViewport(), temp);
	//clipPoint(temp);
	//viewWheels2 = temp;

	// 根据长轴计算
	/*transformPoint(updateViewport(), tempp1);
	transformPoint(updateViewport(), tempp2);
	auto sviewX = (xvmax - xvmin) / clipwinWidth;
	auto sviewY = (yvmax - yvmin) / clipwinHeight;
	auto m = scaleMatrix(sviewX, sviewY, 1) * rotateMatrix(curDirection - clipwinRotate) * shearXMatrix(tansShx) * scaleMatrix(scaleX, scaleY, 1);
	Point ellipseY = { 0, wheelRadius, 0 };
	transformPoint(m, ellipseY);

	float l = std::sqrt(ellipseY.x * ellipseY.x + ellipseY.y * ellipseY.y);
	Point v = { ellipseY.x / l, ellipseY.y / l, 0 };
	Point u = { v.y, -v.x, 0 };
	Matrix tranToXY(4, 4);
	matrixSetIdentity(tranToXY);
	tranToXY[0][0] = u.x;
	tranToXY[0][1] = u.y;
	tranToXY[1][0] = v.x;
	tranToXY[1][1] = v.y;

	transformPoint(tranToXY, ellipseY);

	Point ellipseX = { wheelRadius, 0, 0 };
	transformPoint(tranToXY * m, ellipseX);

	ellipse({ 0, 0, 0 }, std::abs(ellipseX.x), std::abs(ellipseY.y), viewWheels1);
	tranToXY[0][1] = -tranToXY[0][1];
	tranToXY[1][0] = -tranToXY[1][0];

	transformPoints(tranToXY, viewWheels1);

	viewWheels2 = viewWheels1;
	transformPoints(translateMatrix(tempp1.x, tempp1.y), viewWheels1);
	transformPoints(translateMatrix(tempp2.x, tempp2.y), viewWheels2);
	clipPoint(viewWheels1);
	clipPoint(viewWheels2);*/

	// 轮子绘制为椭圆，是为了保证像素的连贯性，否则缩放后会出现像素间的间隔
	// 观察变换过程为 斜切->旋转->XY方向缩放，因椭圆旋转后再缩放无法得知椭圆的参数，
	// 所以将变换过程改为 XY方向缩放（椭圆轴向）->斜切->旋转
	// 方法为 计算两个矩阵并令其相等
	transformPoint(updateViewport(), tempp1);
	transformPoint(updateViewport(), tempp2);
	auto sx = (xvmax - xvmin) / clipwinWidth;
	auto sy = (yvmax - yvmin) / clipwinHeight;
	auto cos = std::cos(curDirection - clipwinRotate);
	auto sin = std::sin(curDirection - clipwinRotate);

	auto sx_ = std::sqrt(sx * sx * cos * cos + sy * sy * sin * sin);
	auto sy_ = sx * sy / sx_;
	auto cos_ = sx * cos / sx_;
	auto sin_ = sy * sin / sx_;

	auto shx_ = tansShx;
	if (!Equal(sy_ * cos_, 0.f))
	{
		shx_ = (sx * cos * tansShx - sx * sin + sin_ * sy_) / (sy_ * cos_);
	}
	else
	{
		shx_ = (sy * sin * tansShx + sy * cos - cos_ * sy_) / (sy_ * sin_);
	}

	ellipse({ 0, 0, 0 }, std::abs(sx_ * scaleX * wheelRadius), std::abs(sy_ * scaleY * wheelRadius), viewWheels1);

	Matrix r(4, 4);
	matrixSetIdentity(r);
	r[0][0] = cos_;
	r[0][1] = -sin_;
	r[1][0] = sin_;
	r[1][1] = cos_;
	auto m = r * shearXMatrix(shx_);
	transformPoints(m, viewWheels1);

	viewWheels2 = viewWheels1;
	transformPoints(translateMatrix(tempp1.x, tempp1.y, 0), viewWheels1);
	transformPoints(translateMatrix(tempp2.x, tempp2.y, 0), viewWheels2);
	clipPoint(viewWheels1);
	clipPoint(viewWheels2);


	//////////////////////////////////////////////////////////////////////////
	temp = road;
	transformPoints(viewTransform, temp);
	static std::vector<Point> tempLine;
	viewRoad.clear();
	for (int i = 0; i < temp.size() - 1; i++)
	{
		clipLine(temp[i], temp[i + 1], tempLine);
		if (tempLine.size())
			viewRoad.push_back(tempLine);
	}
	for (auto & c : viewRoad)
		transformPoints(updateViewport(), c);
}

void drawWindow()
{
	static std::vector<Point> window(4);

	if (viewTransformDirty)
	{
		window[0] = { -clipwinWidth / 2, -clipwinHeight / 2, 0 };
		window[1] = { clipwinWidth / 2, -clipwinHeight / 2, 0 };
		window[2] = { clipwinWidth / 2, clipwinHeight / 2, 0 };
		window[3] = { -clipwinWidth / 2, clipwinHeight / 2, 0 };

		transformPoints(translateMatrix(clipwinPos.x, clipwinPos.y, 0) * rotateMatrix(clipwinRotate), window);
	}

	drawLoop(window);
}
void drawViewport()
{
	drawLoop({
		{ xvmin, yvmin, 0 },
		{ xvmax, yvmin, 0 },
		{ xvmax, yvmax, 0 },
		{ xvmin, yvmax, 0 } });
}
void drawViewCar()
{
	for (auto & c : viewCar)
	{
		drawLoop(c);
	}
}
void drawViewGoods()
{
	for (auto & c : viewGoods)
	{
		drawLoop(c);
	}
}
void drawViewWheel()
{
	for (auto & c : viewWheelHolder)
	{
		drawLoop(c);
	}
	drawPoints(viewWheels1);
	drawPoints(viewWheels2);
}
void drawViewRoad()
{
	for (auto & c : viewRoad)
	{
		drawLoop(c);
	}
}
void scale(float sx, float sy)
{
	scaleX *= sx;
	scaleY *= sy;
	//auto m = rotateByPointMatrix(curPosition, curDirection) * scaleByPointMatrix(curPosition, sx, sy) * rotateByPointMatrix(curPosition, -curDirection);
	auto m = translateMatrix(curPosition.x, curPosition.y, 0) * rotateMatrix(curDirection) * scaleMatrix(sx, sy, 1) * rotateMatrix(-curDirection) * translateMatrix(-curPosition.x, -curPosition.y, 0);

	transformPoints(m, curCar);
	transformPoints(m, curGoods);
	bool turnOrder = (sx > 0 && sy < 0) || (sx < 0 && sy > 0);
	if (turnOrder)
	{
		std::reverse(curCar.begin(), curCar.end());
		std::reverse(curGoods.begin(), curGoods.end());
	}

	transformPoint(m, curWheelPoint1);
	transformPoint(m, curWheelPoint2);
	transformPoints(m, curWheelHolder1);
	transformPoints(m, curWheelHolder2);

	ellipse({ 0, 0, 0 }, std::abs(scaleX) * wheelRadius, std::abs(scaleY) * wheelRadius, curWheel1);
	// 先斜切再缩放，斜切参数将变为原始的sx/sy倍，可以对比先斜切再缩放和先缩放再斜切的矩阵得出此结论
	tansShx *= (sx / sy);
	transformPoints(rotateByPointMatrix({ 0, 0, 0 }, curDirection) * shearXMatrix(tansShx), curWheel1);
	curWheel2 = curWheel1;
	transformPoints(translateMatrix(curWheelPoint1.x, curWheelPoint1.y, 0), curWheel1);
	transformPoints(translateMatrix(curWheelPoint2.x, curWheelPoint2.y, 0), curWheel2);
}
void shear(float sh)
{
	shx += sh;
	tansShx += sh;
	auto m = translateMatrix(curPosition.x, curPosition.y, 0) * rotateMatrix(curDirection) * shearXMatrix(sh) * rotateMatrix(-curDirection) * translateMatrix(-curPosition.x, -curPosition.y, 0);
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
void clipwinReset()
{
	clipwinRotate = 0;
	deltaClipwinRotate = 5 * PI / 180;
	clipwinPos = curPosition;
	deltaClipwinPos = 5;
	clipwinWidth = 200;
	clipwinHeight = 150;
	deltaClipwinSize = 10;
	clipwinFollow = true;
	viewTransformDirty = true;
}
void initWheel()
{
	for (int i = 0; i <= 4; i++)
	{
		float angle = (90 + i * 360 / 5) * PI / 180;
		wheelHolder.push_back({ std::cos(angle) * wheelRadius, std::sin(angle) * wheelRadius, 0 });
	}
}
void initCarData()
{
	initPath();
	lastTick = GetTickCount();
	curPosition = { 0.f, 0.f, 0.f };
	deltaPosition = { 0.f, 0.f, 0.f };
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
	transformPoints(translateMatrix(20, 22, 0), curCar);
	transformPoints(translateMatrix(-15, 47, 0), curGoods);
	transformPoint(translateMatrix(32, 10, 0), curWheelPoint1);
	transformPoint(translateMatrix(-32, 10, 0), curWheelPoint2);
	transformPoints(translateMatrix(32, 10, 0), curWheelHolder1);
	transformPoints(translateMatrix(-32, 10, 0), curWheelHolder2);
	circle({ 32, 10, 0 }, wheelRadius, curWheel1);
	circle({ -32, 10, 0 }, wheelRadius, curWheel2);
}
void updateWindowPosition()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(curPosition.x - winWidth / 2, curPosition.x + winWidth / 2, -150, -150 + winHeight);
}
void updateClipwinPos()
{
	if (clipwinFollow)
	{
		clipwinPos.x += deltaPosition.x;
		clipwinPos.y += deltaPosition.y;
	}
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

	transformPoints(translateMatrix(curWheelPoint1.x, curWheelPoint1.y, 0), curWheelHolder1);
	transformPoints(translateMatrix(curWheelPoint2.x, curWheelPoint2.y, 0), curWheelHolder2);
	transformPoints(rotateByPointMatrix(curWheelPoint1, diretion), curWheelHolder1);
	transformPoints(rotateByPointMatrix(curWheelPoint2, diretion), curWheelHolder2);
}
void updateMove(Point p)
{
	auto dx = p.x - curPosition.x;
	auto dy = p.y - curPosition.y;
	transformPoints(translateMatrix(dx, dy, 0), curCar);
	transformPoints(translateMatrix(dx, dy, 0), curGoods);
	transformPoint(translateMatrix(dx, dy, 0), curWheelPoint1);
	transformPoint(translateMatrix(dx, dy, 0), curWheelPoint2);
	transformPoints(translateMatrix(dx, dy, 0), curWheel1);
	transformPoints(translateMatrix(dx, dy, 0), curWheel2);
}
void updateTransform()
{
	auto l = std::sqrt(tansShx * tansShx + 1) * wheelRadius * std::abs(scaleY);

	float deltaA = delta * speed / l;
	transformPoints(rotateByPointMatrix({ 0, 0, 0 }, -deltaA), curWheelRoundHolder);
	curWheelHolder1 = curWheelRoundHolder;
	transformPoints(shearXMatrix(tansShx) * scaleByPointMatrix({ 0, 0, 0 }, scaleX, scaleY, 1), curWheelHolder1);
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
	deltaPosition = { nextP.x - curPosition.x, nextP.y - curPosition.y, 0 };
	curPosition = nextP;
	updateDirection(dir);
	curDirection = dir;

	updateWindowPosition();

	updateClipwinPos();
	updateView();
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
void dealClipWindowPositionCommand()
{
	printf("input clip window position value: ");
	float x, y;
	if (scanf_s("%f,%f", &x, &y) != 2)
	{
		printf("clip window position must be two numbers!!!\n");
		dealClipWindowPositionCommand();
		return;
	}
	clipwinPos = { x, y };
}
void dealDeltaClipWindowPositionCommand()
{
	printf("input clip window position delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window position delta must be a number!!!\n");
		dealDeltaClipWindowPositionCommand();
		return;
	}
	float value = std::stof(str);
	deltaClipwinPos = value;
}
void dealClipWindowRotateCommand()
{
	printf("input clip window rotate value(angle): ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window rotate must be a number!!!\n");
		dealClipWindowRotateCommand();
		return;
	}
	float value = std::stof(str);
	clipwinRotate = value * PI / 180;
}
void dealDeltaClipWindowRotateCommand()
{
	printf("input clip window rotate delta value(angle): ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window rotate delta must be a number!!!\n");
		dealDeltaClipWindowRotateCommand();
		return;
	}
	float value = std::stof(str);
	deltaClipwinRotate = value * PI / 180;
}
void dealClipWindowWidthCommand()
{
	printf("input clip window width value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window width must be a number!!!\n");
		dealClipWindowWidthCommand();
		return;
	}
	float value = std::stof(str);
	clipwinWidth = value;
}
void dealClipWindowHeightCommand()
{
	printf("input clip window height value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window width must be a number!!!\n");
		dealClipWindowHeightCommand();
		return;
	}
	float value = std::stof(str);
	clipwinHeight = value;
}
void dealDeltaClipWindowSizeCommand()
{
	printf("input clip window size delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window size delta must be a number!!!\n");
		dealDeltaClipWindowSizeCommand();
		return;
	}
	float value = std::stof(str);
	deltaClipwinSize = value;
}
void dealClipWindowFollowCommand()
{
	printf("input clip window follow value: ");
	std::string str;
	std::cin >> str;
	if (str == "true" || str == "TRUE")
	{
		clipwinFollow = true;
		clipwinPos = curPosition;
	}
	else if (str == "false" || str == "FALSE")
	{
		clipwinFollow = false;
	}
	else
	{
		printf("clip window follow must be bool value!!!\n");
		dealClipWindowFollowCommand();
		return;
	}
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
	else if (commandString == "cwp" || commandString == "CWP")
	{
		dealClipWindowPositionCommand();
		return true;
	}
	else if (commandString == "dcwp" || commandString == "DCWP")
	{
		dealDeltaClipWindowPositionCommand();
		return true;
	}
	else if (commandString == "cwr" || commandString == "CWR")
	{
		dealClipWindowRotateCommand();
		return true;
	}
	else if (commandString == "dcwr" || commandString == "DCWR")
	{
		dealDeltaClipWindowRotateCommand();
		return true;
	}
	else if (commandString == "cww" || commandString == "CWW")
	{
		dealClipWindowWidthCommand();
		return true;
	}
	else if (commandString == "cwh" || commandString == "CWH")
	{
		dealClipWindowHeightCommand();
		return true;
	}
	else if (commandString == "dcws" || commandString == "DCWS")
	{
		dealDeltaClipWindowSizeCommand();
		return true;
	}
	else if (commandString == "cwf" || commandString == "CWF")
	{
		dealClipWindowFollowCommand();
		return true;
	}
	else if (commandString == "cwreset" || commandString == "CWRESET")
	{
		clipwinReset();
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
	float leftX = 10;
	float topY = winHeight;
	int space = 20;
	char s[128] = {};

	int realFPS = 0;
	if (delta)
		realFPS = 1 / delta;
	sprintf_s(s, "FPS: %d", realFPS);
	drawString({ leftX, topY - space * 1, 0 }, s);

	sprintf_s(s, "position: %.02f, %.02f", curPosition.x, curPosition.y);
	drawString({ leftX, topY - space * 2, 0 }, s);

	sprintf_s(s, "speed: %.02f pixel/s", speed);
	drawString({ leftX, topY - space * 3, 0 }, s);

	float angle = curDirection * 180 / PI;
	sprintf_s(s, "angle: %.02f", angle);
	drawString({ leftX, topY - space * 4, 0 }, s);

	sprintf_s(s, "scale x: %.02f", scaleX);
	drawString({ leftX, topY - space * 5, 0 }, s);

	sprintf_s(s, "scale y: %.02f", scaleY);
	drawString({ leftX, topY - space * 6, 0 }, s);

	sprintf_s(s, "shear: %.02f", tansShx);
	drawString({ leftX, topY - space * 7, 0 }, s);

	sprintf_s(s, "clip window postion: %.02f, %.02f", clipwinPos.x, clipwinPos.y);
	drawString({ leftX, topY - space * 8, 0 }, s);

	sprintf_s(s, "clip window width: %.02f", clipwinWidth);
	drawString({ leftX, topY - space * 9, 0 }, s);

	sprintf_s(s, "clip window height: %.02f", clipwinHeight);
	drawString({ leftX, topY - space * 10, 0 }, s);

	sprintf_s(s, "clip window rotate: %.02f", clipwinRotate * 180 / PI);
	drawString({ leftX, topY - space * 11, 0 }, s);

	sprintf_s(s, "clip window follow: %s", clipwinFollow ? "true" : "false");
	drawString({ leftX, topY - space * 12, 0 }, s);
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
	printf("R: reset state. \n");
	printf("V: clip window mode. \n");
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
	printf("cwp: set clip window position.\n");
	printf("dcwp: set clip window position delta.\n");
	printf("cwr: set clip window rotate.\n");
	printf("dcwr: set clip window rotate delta.\n");
	printf("cww: set clip window width.\n");
	printf("cwh: set clip window height.\n");
	printf("dcws: set clip window size delta.\n");
	printf("cwf: set clip window follow.\n");
	printf("cwreset: set clip window reset.\n");
	printf("reset: reset state.\n");
	printf("exit: exit command mode.\n");
	dealCommand();
}
void showViewOperatorNotice()
{
	system("cls");
	printf("Clip Window Operator: \n");
	printf("F: fllow the car.\n");
	printf("→: move right.\n");
	printf("←: move left.\n");
	printf("↑: move up.\n");
	printf("↓: move down.\n");
	printf("CTRL →: add width.\n");
	printf("CTRL ←: decrease width.\n");
	printf("CTRL ↑: add height. \n");
	printf("CTRL ↓: decrease height. \n");
	printf("SHIFT ↑: add angle.\n");
	printf("SHIFT ↓: decrease angle.\n");
	printf("R: reset state\n");
	printf("ESC: back.\n");
}
void normalKeyFcn(unsigned char key, int x, int y)
{
	//printf("normalKeyFcn %d, %d, %d\n", key, x, y);
	switch (key)
	{
	case 27:
		if (!clipwinMode)
		{
			showCommandNotice();
		}
		else
		{
			clipwinMode = false;
			showOperatorNotice();
		}
		break;
	case 'r':
	case 'R':
		if (!clipwinMode)
		{
			reset();
		}
		else
		{
			clipwinReset();
		}
		break;
	case 'w':
	case 'W':
		if (!clipwinMode)
		{
			setFPS(FPS + deltaFPS);
		}
		break;
	case 's':
	case 'S':
		if (!clipwinMode)
		{
			setFPS(FPS - deltaFPS);
		}
		break;
	case 'a':
	case 'A':
		if (!clipwinMode)
		{
			speed -= deltaSpeed;
		}
		break;
	case 'd':
	case 'D':
		if (!clipwinMode)
		{
			speed += deltaSpeed;
		}
		break;
	case 'v':
	case 'V':
		if (!clipwinMode)
		{
			showViewOperatorNotice();
			clipwinMode = true;
		}
		break;
	case 'f':
	case 'F':
		if (clipwinMode)
		{
			clipwinFollow = !clipwinFollow;

			if (clipwinFollow)
				clipwinPos = curPosition;
		}
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
			if (!clipwinMode)
			{
				scale(deltaScaleX, 1);
			}
			else
			{
				clipwinPos.x += deltaClipwinPos;
				viewTransformDirty = true;
			}
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (!clipwinMode)
			{
				if (scaleX < 0)
					scale(-1, 1);
			}
			else
			{
				clipwinWidth += deltaClipwinSize;
				viewTransformDirty = true;
			}
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			if (!clipwinMode)
			{
				shear(deltaShear);
			}
		}
		break;
	case GLUT_KEY_LEFT:
		if (mod == 0)
		{
			if (!clipwinMode)
			{
				scale(1 / deltaScaleX, 1);
			}
			else
			{
				clipwinPos.x -= deltaClipwinPos;
				viewTransformDirty = true;
			}
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (!clipwinMode)
			{
				if (scaleX > 0)
					scale(-1, 1);
			}
			else
			{
				clipwinWidth -= deltaClipwinSize;
				viewTransformDirty = true;
			}
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			if (!clipwinMode)
			{
				shear(-deltaShear);
			}
		}
		break;
	case GLUT_KEY_UP:
		if (mod == 0)
		{
			if (!clipwinMode)
			{
				scale(1, deltaScaleY);
			}
			else
			{
				clipwinPos.y += deltaClipwinPos;
				viewTransformDirty = true;
			}
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (!clipwinMode)
			{
				if (scaleY < 0)
					scale(1, -1);
			}
			else
			{
				clipwinHeight += deltaClipwinSize;
				viewTransformDirty = true;
			}
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			if (clipwinMode)
			{
				clipwinRotate += deltaClipwinRotate;
				clipwinRotate = fmod(clipwinRotate, 2 * PI);
				viewTransformDirty = true;
			}
		}
		break;
	case GLUT_KEY_DOWN:
		if (mod == 0)
		{
			if (!clipwinMode)
			{
				scale(1, 1 / deltaScaleY);
			}
			else
			{
				clipwinPos.y -= deltaClipwinPos;
				viewTransformDirty = true;
			}
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (!clipwinMode)
			{
				if (scaleY > 0)
					scale(1, -1);
			}
			else
			{
				clipwinHeight -= deltaClipwinSize;
				viewTransformDirty = true;
			}
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			if (clipwinMode)
			{
				clipwinRotate -= deltaClipwinRotate;
				clipwinRotate = fmod(clipwinRotate, 2 * PI);
				viewTransformDirty = true;
			}
		}
		break;
	default:
		break;
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	update();
	glViewport(0, 0, winWidth, winHeight);

	drawRoad();
	drawCar();
	drawGoods();
	drawWheel();
	drawWindow();
	viewTransformDirty = clipwinFollow;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glViewport(0, 0, winWidth, winHeight);

	showState();
	drawViewport();
	drawViewCar();
	drawViewGoods();
	drawViewWheel();
	drawViewRoad();

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
void code_9_exercise_add_1()
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

#ifdef CHAPTER_9_EXERCISE_ADD_2
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
		return{ _pathLines[curPathIdx].begin.x + curLength * _pathLines[curPathIdx].cos, _pathLines[curPathIdx].begin.y + curLength * _pathLines[curPathIdx].sin, 0 };
	}
};
Matrix scaleMatrix4(float sx, float sy)
{
	// 基于原点缩放
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[1][1] = sy;
	return ret;
}
Matrix shearXMatrix4(float shx)
{
	// 基于原点的x方向错切
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][1] = shx;
	return ret;
}
std::vector<Point> road = { { -400, 0, 0 },{ 0, 0, 0 },{ 1000, 0, 0 },{ 1200, 150, 0 },{ 1600, 150, 0 },{ 2000, 0, 0 },{ 2600, 0, 0 },{ 2600, 200, 0 },{ 2800, 200, 0 } ,{ 2800, 0, 0 } ,{ 3200, 0, 0 },{ 3600, 0, 0 } };
std::vector<Point> car = { { 0, 0, 0 },{ -70, 0, 0 },{ -70, -12, 0 },{ 30, -12, 0 },{ 30, -2, 0 },{ 15, 26, 0 },{ 0, 26, 0 } };
std::vector<Point> goods = { { -30, -20, 0 },{ 30, -20, 0 },{ 30, 20, 0 },{ -30, 20, 0 } };
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
Point deltaPosition;
float curDirection;
float wheelRadius = 0;
Path* path = nullptr;
float wheelRadian = 0.f;
Matrix curTransform(4, 4);

// 裁剪窗口
float clipwinRotate = 0;
float deltaClipwinRotate = 5 * PI / 180;
Point clipwinPos = { 0, 0, 0 }; // 中心点
float deltaClipwinPos = 5;
float clipwinWidth = 200;
float clipwinHeight = 150;
float deltaClipwinSize = 10;
bool clipwinFollow = true;
bool clipwinMode = false;

// 视口
float xvmin = 570, yvmin = 420, xvmax = 770, yvmax = 570;

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
	glVertex3f(p.x, p.y, p.z);
	glEnd();
}
void drawPoints(const std::vector<Point>& points)
{
	glBegin(GL_POINTS);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}
void drawStrip(const std::vector<Point>& points)
{
	glBegin(GL_LINE_STRIP);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}
void drawLoop(const std::vector<Point>& points)
{
	glBegin(GL_LINE_LOOP);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
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
	drawPoint({ p0.x + x, p0.y + y, 0 });
	drawPoint({ p0.x - x, p0.y + y, 0 });
	drawPoint({ p0.x + x, p0.y - y, 0 });
	drawPoint({ p0.x - x, p0.y - y, 0 });
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
void ellipsePlot(Point p0, int x, int y, std::vector<Point>& points)
{
	points.push_back({ p0.x + x, p0.y + y, 0 });
	points.push_back({ p0.x - x, p0.y + y, 0 });
	points.push_back({ p0.x + x, p0.y - y, 0 });
	points.push_back({ p0.x - x, p0.y - y, 0 });
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
	drawLoop(car);
}
void drawGoods()
{
	drawLoop(goods);
}
void drawWheel()
{
	ellipse({ 0, 0, 0 }, std::abs(scaleX) * wheelRadius, std::abs(scaleY) * wheelRadius);
}
void drawWheelHolder()
{
	for (auto & p : wheelHolder)
	{
		drawStrip({ { 0, 0, 0 }, p });
	}
}
void drawWindow()
{
	drawLoop({
		{ -clipwinWidth / 2, -clipwinHeight / 2, 0 },
		{ clipwinWidth / 2, -clipwinHeight / 2, 0 },
		{ clipwinWidth / 2, clipwinHeight / 2, 0 },
		{ -clipwinWidth / 2, clipwinHeight / 2, 0 }, });
}
void drawViewport()
{
	drawLoop({
		{ xvmin, yvmin, 0 },
		{ xvmax, yvmin, 0 },
		{ xvmax, yvmax, 0 },
		{ xvmin, yvmax, 0 } });
}
void scale(float sx, float sy)
{
	scaleX *= sx;
	scaleY *= sy;

	// 先斜切再缩放，斜切参数将变为原始的sx/sy倍，可以对比先斜切再缩放和先缩放再斜切的矩阵得出此结论
	tansShx *= (sx / sy);
	curTransform = scaleMatrix4(sx, sy) * curTransform;
}
void shear(float sh)
{
	shx += sh;
	tansShx += sh;
	curTransform = shearXMatrix4(sh) * curTransform;
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
void clipwinReset()
{
	clipwinRotate = 0;
	deltaClipwinRotate = 5 * PI / 180;
	clipwinPos = curPosition;
	deltaClipwinPos = 5;
	clipwinWidth = 200;
	clipwinHeight = 150;
	deltaClipwinSize = 10;
	clipwinFollow = true;
}
void initWheel()
{
	for (int i = 0; i <= 4; i++)
	{
		float angle = (90 + i * 360 / 5) * PI / 180;
		wheelHolder.push_back({ std::cos(angle) * wheelRadius, std::sin(angle) * wheelRadius, 0 });
	}
}
void initCarData()
{
	initPath();
	lastTick = GetTickCount();
	curPosition = { 0.f, 0.f, 0.f };
	deltaPosition = { 0.f, 0.f, 0.f };
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
void updateClipwinPos()
{
	if (clipwinFollow)
	{
		clipwinPos.x += deltaPosition.x;
		clipwinPos.y += deltaPosition.y;
	}
}
void updateViewport()
{
	// 绘制视口1
	// 这里直接采用opengl投影变换到视口
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(clipwinPos.x - clipwinWidth / 2, clipwinPos.x + clipwinWidth / 2, clipwinPos.y - clipwinHeight / 2, clipwinPos.y + clipwinHeight / 2);
	glViewport(xvmin, yvmin, xvmax - xvmin, yvmax - yvmin);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(clipwinPos.x, clipwinPos.y, 0);
	glRotatef(-clipwinRotate * 180 / PI, 0.f, 0.f, 1.f);
	glTranslatef(-clipwinPos.x, -clipwinPos.y, 0);
	drawRoad();

	glLoadIdentity();
	glTranslatef(clipwinPos.x, clipwinPos.y, 0);
	glRotatef(-clipwinRotate * 180 / PI, 0.f, 0.f, 1.f);
	glTranslatef(-clipwinPos.x, -clipwinPos.y, 0);
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(curTransform);
	glTranslated(20, 22, 0);
	drawCar();

	glLoadIdentity();
	glTranslatef(clipwinPos.x, clipwinPos.y, 0);
	glRotatef(-clipwinRotate * 180 / PI, 0.f, 0.f, 1.f);
	glTranslatef(-clipwinPos.x, -clipwinPos.y, 0);
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(curTransform);
	glTranslated(-15, 47, 0);
	drawGoods();

	glLoadIdentity();
	glTranslatef(clipwinPos.x, clipwinPos.y, 0);
	glRotatef(-clipwinRotate * 180 / PI, 0.f, 0.f, 1.f);
	glTranslatef(-clipwinPos.x, -clipwinPos.y, 0);
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(curTransform);
	glTranslated(32, 10, 0);
	glRotatef(wheelRadian, 0.0, 0.0, 1.0);
	drawWheelHolder();

	glLoadIdentity();
	glTranslatef(clipwinPos.x, clipwinPos.y, 0);
	glRotatef(-clipwinRotate * 180 / PI, 0.f, 0.f, 1.f);
	glTranslatef(-clipwinPos.x, -clipwinPos.y, 0);
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(curTransform);
	glTranslated(-32, 10, 0);
	glRotatef(wheelRadian, 0.0, 0.0, 1.0);
	drawWheelHolder();

	glLoadIdentity();
	glTranslatef(clipwinPos.x, clipwinPos.y, 0);
	glRotatef(-clipwinRotate * 180 / PI, 0.f, 0.f, 1.f);
	glTranslatef(-clipwinPos.x, -clipwinPos.y, 0);
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(shearXMatrix4(tansShx));
	glTranslated(32 * scaleX, 10 * scaleY, 0);
	drawWheel();

	glLoadIdentity();
	glTranslatef(clipwinPos.x, clipwinPos.y, 0);
	glRotatef(-clipwinRotate * 180 / PI, 0.f, 0.f, 1.f);
	glTranslatef(-clipwinPos.x, -clipwinPos.y, 0);
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(shearXMatrix4(tansShx));
	glTranslated(-32 * scaleX, 10 * scaleY, 0);
	drawWheel();
}
void showState();
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

	deltaPosition = { nextP.x - curPosition.x, nextP.y - curPosition.y, 0 };
	curPosition = nextP;
	curDirection = dir;

	// gluOrtho2D之后的绘制才会使用新的视口，否则若在gluOrtho2D之前绘制，绘制使用之前的视口，下次新视口才生效
	// 因此第一次绘制的效果是绘制图形移动，但视口不动，即绘制图形超前视口一个delta*speed距离
	// 下次绘制时，会首先生效上次的视口设置，即视口中心为绘制图形中心，但是图形此时又会移动，即绘制图形又会超前视口一个delta*speed距离
	// 这样每帧绘制都会超前视口一个delta*speed距离，因为delta是变量，所以每帧位置都有小偏差，造成视觉残影，绘制抖动
	updateWindowPosition();
	updateClipwinPos();

	glViewport(0, 0, winWidth, winHeight);

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
	glMultMatrixf(shearXMatrix4(tansShx));
	glTranslated(32 * scaleX, 10 * scaleY, 0);
	drawWheel();

	glLoadIdentity();
	glTranslatef(curPosition.x, curPosition.y, 0.f);
	glRotatef(curDirection * 180 / PI, 0.0, 0.0, 1.0);
	glMultMatrixf(shearXMatrix4(tansShx));
	glTranslated(-32 * scaleX, 10 * scaleY, 0);
	drawWheel();

	// 裁剪窗口
	glLoadIdentity();
	glTranslatef(clipwinPos.x, clipwinPos.y, 0.f);
	glRotatef(clipwinRotate * 180 / PI, 0.0, 0.0, 1.0);
	drawWindow();

	// 绘制状态文字，视口
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glViewport(0, 0, winWidth, winHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	showState();
	drawViewport();

	updateViewport();
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
void dealClipWindowPositionCommand()
{
	printf("input clip window position value: ");
	float x, y;
	if (scanf_s("%f,%f", &x, &y) != 2)
	{
		printf("clip window position must be two numbers!!!\n");
		dealClipWindowPositionCommand();
		return;
	}
	clipwinPos = { x, y };
}
void dealDeltaClipWindowPositionCommand()
{
	printf("input clip window position delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window position delta must be a number!!!\n");
		dealDeltaClipWindowPositionCommand();
		return;
	}
	float value = std::stof(str);
	deltaClipwinPos = value;
}
void dealClipWindowRotateCommand()
{
	printf("input clip window rotate value(angle): ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window rotate must be a number!!!\n");
		dealClipWindowRotateCommand();
		return;
	}
	float value = std::stof(str);
	clipwinRotate = value * PI / 180;
}
void dealDeltaClipWindowRotateCommand()
{
	printf("input clip window rotate delta value(angle): ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window rotate delta must be a number!!!\n");
		dealDeltaClipWindowRotateCommand();
		return;
	}
	float value = std::stof(str);
	deltaClipwinRotate = value * PI / 180;
}
void dealClipWindowWidthCommand()
{
	printf("input clip window width value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window width must be a number!!!\n");
		dealClipWindowWidthCommand();
		return;
	}
	float value = std::stof(str);
	clipwinWidth = value;
}
void dealClipWindowHeightCommand()
{
	printf("input clip window height value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window width must be a number!!!\n");
		dealClipWindowHeightCommand();
		return;
	}
	float value = std::stof(str);
	clipwinHeight = value;
}
void dealDeltaClipWindowSizeCommand()
{
	printf("input clip window size delta value: ");
	std::string str;
	std::cin >> str;
	if (!isNumber(str))
	{
		printf("clip window size delta must be a number!!!\n");
		dealDeltaClipWindowSizeCommand();
		return;
	}
	float value = std::stof(str);
	deltaClipwinSize = value;
}
void dealClipWindowFollowCommand()
{
	printf("input clip window follow value: ");
	std::string str;
	std::cin >> str;
	if (str == "true" || str == "TRUE")
	{
		clipwinFollow = true;
		clipwinPos = curPosition;
	}
	else if (str == "false" || str == "FALSE")
	{
		clipwinFollow = false;
	}
	else
	{
		printf("clip window follow must be bool value!!!\n");
		dealClipWindowFollowCommand();
		return;
	}
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
	else if (commandString == "cwp" || commandString == "CWP")
	{
		dealClipWindowPositionCommand();
		return true;
	}
	else if (commandString == "dcwp" || commandString == "DCWP")
	{
		dealDeltaClipWindowPositionCommand();
		return true;
	}
	else if (commandString == "cwr" || commandString == "CWR")
	{
		dealClipWindowRotateCommand();
		return true;
	}
	else if (commandString == "dcwr" || commandString == "DCWR")
	{
		dealDeltaClipWindowRotateCommand();
		return true;
	}
	else if (commandString == "cww" || commandString == "CWW")
	{
		dealClipWindowWidthCommand();
		return true;
	}
	else if (commandString == "cwh" || commandString == "CWH")
	{
		dealClipWindowHeightCommand();
		return true;
	}
	else if (commandString == "dcws" || commandString == "DCWS")
	{
		dealDeltaClipWindowSizeCommand();
		return true;
	}
	else if (commandString == "cwf" || commandString == "CWF")
	{
		dealClipWindowFollowCommand();
		return true;
	}
	else if (commandString == "cwreset" || commandString == "CWRESET")
	{
		clipwinReset();
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
	float leftX = 10;
	float topY = winHeight;
	int space = 20;
	char s[128] = {};

	int realFPS = 0;
	if (delta)
		realFPS = 1 / delta;
	sprintf_s(s, "FPS: %d", realFPS);
	drawString({ leftX, topY - space * 1, 0 }, s);

	sprintf_s(s, "position: %.02f, %.02f", curPosition.x, curPosition.y);
	drawString({ leftX, topY - space * 2, 0 }, s);

	sprintf_s(s, "speed: %.02f pixel/s", speed);
	drawString({ leftX, topY - space * 3, 0 }, s);

	float angle = curDirection * 180 / PI;
	sprintf_s(s, "angle: %.02f", angle);
	drawString({ leftX, topY - space * 4, 0 }, s);

	sprintf_s(s, "scale x: %.02f", scaleX);
	drawString({ leftX, topY - space * 5, 0 }, s);

	sprintf_s(s, "scale y: %.02f", scaleY);
	drawString({ leftX, topY - space * 6, 0 }, s);

	sprintf_s(s, "shear: %.02f", tansShx);
	drawString({ leftX, topY - space * 7, 0 }, s);

	sprintf_s(s, "clip window postion: %.02f, %.02f", clipwinPos.x, clipwinPos.y);
	drawString({ leftX, topY - space * 8, 0 }, s);

	sprintf_s(s, "clip window width: %.02f", clipwinWidth);
	drawString({ leftX, topY - space * 9, 0 }, s);

	sprintf_s(s, "clip window height: %.02f", clipwinHeight);
	drawString({ leftX, topY - space * 10, 0 }, s);

	sprintf_s(s, "clip window rotate: %.02f", clipwinRotate * 180 / PI);
	drawString({ leftX, topY - space * 11, 0 }, s);

	sprintf_s(s, "clip window follow: %s", clipwinFollow ? "true" : "false");
	drawString({ leftX, topY - space * 12, 0 }, s);
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
	printf("R: reset state. \n");
	printf("V: clip window mode. \n");
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
	printf("cwp: set clip window position.\n");
	printf("dcwp: set clip window position delta.\n");
	printf("cwr: set clip window rotate.\n");
	printf("dcwr: set clip window rotate delta.\n");
	printf("cww: set clip window width.\n");
	printf("cwh: set clip window height.\n");
	printf("dcws: set clip window size delta.\n");
	printf("cwf: set clip window follow.\n");
	printf("cwreset: set clip window reset.\n");
	printf("reset: reset state.\n");
	printf("exit: exit command mode.\n");
	dealCommand();
}
void showViewOperatorNotice()
{
	system("cls");
	printf("Clip Window Operator: \n");
	printf("F: fllow the car.\n");
	printf("→: move right.\n");
	printf("←: move left.\n");
	printf("↑: move up.\n");
	printf("↓: move down.\n");
	printf("CTRL →: add width.\n");
	printf("CTRL ←: decrease width.\n");
	printf("CTRL ↑: add height. \n");
	printf("CTRL ↓: decrease height. \n");
	printf("SHIFT ↑: add angle.\n");
	printf("SHIFT ↓: decrease angle.\n");
	printf("R: reset state\n");
	printf("ESC: back.\n");
}
void normalKeyFcn(unsigned char key, int x, int y)
{
	//printf("normalKeyFcn %d, %d, %d\n", key, x, y);
	switch (key)
	{
	case 27:
		if (!clipwinMode)
		{
			showCommandNotice();
		}
		else
		{
			clipwinMode = false;
			showOperatorNotice();
		}
		break;
	case 'r':
	case 'R':
		if (!clipwinMode)
		{
			reset();
		}
		else
		{
			clipwinReset();
		}
		break;
	case 'w':
	case 'W':
		if (!clipwinMode)
		{
			setFPS(FPS + deltaFPS);
		}
		break;
	case 's':
	case 'S':
		if (!clipwinMode)
		{
			setFPS(FPS - deltaFPS);
		}
		break;
	case 'a':
	case 'A':
		if (!clipwinMode)
		{
			speed -= deltaSpeed;
		}
		break;
	case 'd':
	case 'D':
		if (!clipwinMode)
		{
			speed += deltaSpeed;
		}
		break;
	case 'v':
	case 'V':
		if (!clipwinMode)
		{
			showViewOperatorNotice();
			clipwinMode = true;
		}
		break;
	case 'f':
	case 'F':
		if (clipwinMode)
		{
			clipwinFollow = !clipwinFollow;

			if (clipwinFollow)
				clipwinPos = curPosition;
		}
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
			if (!clipwinMode)
			{
				scale(deltaScaleX, 1);
			}
			else
			{
				clipwinPos.x += deltaClipwinPos;
			}
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (!clipwinMode)
			{
				if (scaleX < 0)
					scale(-1, 1);
			}
			else
			{
				clipwinWidth += deltaClipwinSize;
			}
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			if (!clipwinMode)
			{
				shear(deltaShear);
			}
		}
		break;
	case GLUT_KEY_LEFT:
		if (mod == 0)
		{
			if (!clipwinMode)
			{
				scale(1 / deltaScaleX, 1);
			}
			else
			{
				clipwinPos.x -= deltaClipwinPos;
			}
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (!clipwinMode)
			{
				if (scaleX > 0)
					scale(-1, 1);
			}
			else
			{
				clipwinWidth -= deltaClipwinSize;
			}
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			if (!clipwinMode)
			{
				shear(-deltaShear);
			}
		}
		break;
	case GLUT_KEY_UP:
		if (mod == 0)
		{
			if (!clipwinMode)
			{
				scale(1, deltaScaleY);
			}
			else
			{
				clipwinPos.y += deltaClipwinPos;
			}
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (!clipwinMode)
			{
				if (scaleY < 0)
					scale(1, -1);
			}
			else
			{
				clipwinHeight += deltaClipwinSize;
			}
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			if (clipwinMode)
			{
				clipwinRotate += deltaClipwinRotate;
				clipwinRotate = fmod(clipwinRotate, 2 * PI);
			}
		}
		break;
	case GLUT_KEY_DOWN:
		if (mod == 0)
		{
			if (!clipwinMode)
			{
				scale(1, 1 / deltaScaleY);
			}
			else
			{
				clipwinPos.y -= deltaClipwinPos;
			}
		}
		else if (mod == GLUT_ACTIVE_CTRL)
		{
			if (!clipwinMode)
			{
				if (scaleY > 0)
					scale(1, -1);
			}
			else
			{
				clipwinHeight -= deltaClipwinSize;
			}
		}
		else if (mod == GLUT_ACTIVE_SHIFT)
		{
			if (clipwinMode)
			{
				clipwinRotate -= deltaClipwinRotate;
				clipwinRotate = fmod(clipwinRotate, 2 * PI);
			}
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
void code_9_exercise_add_2()
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
// CHAPTER_9_COMMON

#ifdef CHAPTER_9_COMMON
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

#ifdef CHAPTER_9_1
	code_9_1();
#endif

#ifdef CHAPTER_9_2_3
	code_9_2_3();
#endif

#ifdef CHAPTER_9_3
	code_9_3();
#endif

#ifdef CHAPTER_9_4
	code_9_4();
#endif

#ifdef CHAPTER_9_9_1
	code_9_1_1();
#endif

#ifdef CHAPTER_9_9_2
	code_9_1_2();
#endif

#ifdef CHAPTER_9_EXERCISE_1
	code_9_exercise_1();
#endif

#ifdef CHAPTER_9_EXERCISE_2
	code_9_exercise_2();
#endif

#ifdef CHAPTER_9_EXERCISE_3
	code_9_exercise_3();
#endif

#ifdef CHAPTER_9_EXERCISE_4
	code_9_exercise_4();
#endif

#ifdef CHAPTER_9_EXERCISE_5
	code_9_exercise_5();
#endif

#ifdef CHAPTER_9_EXERCISE_6
	code_9_exercise_6();
#endif

#ifdef CHAPTER_9_EXERCISE_7
	code_9_exercise_7();
#endif

#ifdef CHAPTER_9_EXERCISE_8
	code_9_exercise_8();
#endif

#ifdef CHAPTER_9_EXERCISE_9
	code_9_exercise_9();
#endif

#ifdef CHAPTER_9_EXERCISE_10
	code_9_exercise_10();
#endif

#ifdef CHAPTER_9_EXERCISE_11
	code_9_exercise_11();
#endif

#ifdef CHAPTER_9_EXERCISE_12
	code_9_exercise_12();
#endif

#ifdef CHAPTER_9_EXERCISE_15
	code_9_exercise_15();
#endif

#ifdef CHAPTER_9_EXERCISE_15_1
	code_9_exercise_15_1();
#endif

#ifdef CHAPTER_9_EXERCISE_16
	code_9_exercise_16();
#endif

#ifdef CHAPTER_9_EXERCISE_17
	code_9_exercise_17();
#endif

#ifdef CHAPTER_9_EXERCISE_ADD_1
	code_9_exercise_add_1();
#endif
	
#ifdef CHAPTER_9_EXERCISE_ADD_2
	code_9_exercise_add_2();
#endif

	glutMainLoop();
}
#endif
#endif 


