#include "common.h"

#ifdef CHAPTER_9
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

// 绘制
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}

#endif

#ifdef CHAPTER_9_1
struct Point
{
	GLfloat x, y, z;
};
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
void drawCoordinate()
{
	glBegin(GL_LINES);
		glVertex3f(-winWidth, 0, 0);
		glVertex3f(winWidth, 0, 0);
		glVertex3f(0, -winHeight, 0);
		glVertex3f(0, winHeight, 0);
	glEnd();
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
void drawCoordinate()
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

	drawCoordinate();
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

	glutMainLoop();
}
#endif
#endif 


