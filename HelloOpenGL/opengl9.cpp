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

	glutMainLoop();
}
#endif
#endif 


