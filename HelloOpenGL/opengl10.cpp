#include "common.h"

#ifdef CHAPTER_10
#include "opengl10h.h"

#ifdef CHAPTER_10_COMMON
float winWidth = 800, winHeight = 600;

//////////////////////////////////////////////////////////////////////////
// 判断浮点数相等
inline bool Equal(float f1, float f2) { return std::abs(f1 - f2) < 0.0001; }
inline bool Greater(float f1, float f2) { return Equal(f1, f2) ? false : (f1 > f2); }
inline bool Less(float f1, float f2) { return Equal(f1, f2) ? false : (f1 < f2); }
inline bool GreaterQ(float f1, float f2) { return Greater(f1, f2) || Equal(f1, f2); }
inline bool LessQ(float f1, float f2) { return Less(f1, f2) || Equal(f1, f2); }

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// 绘制
// 二维多边形
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_POLYGON);
	for (auto & p : points)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
}
// 二维坐标轴
void drawCoordinate(float xStart = -winWidth, float xEnd = winWidth, float yStart = -winHeight, float yEnd = winHeight)
{
	glBegin(GL_LINES);
	glVertex3f(xStart, 0, 0);
	glVertex3f(xEnd, 0, 0);
	glVertex3f(0, yStart, 0);
	glVertex3f(0, yEnd, 0);
	glEnd();
}
// 三维多面体
void drawPolyhedron(const std::vector<Point>& points, const std::vector<std::vector<int>>& indexs)
{
	for (auto& index : indexs)
	{
		glBegin(GL_LINE_LOOP);
		for (auto& i : index)
		{
			glVertex2f(points[i].x, points[i].y);
		}
		glEnd();
	}
}

//////////////////////////////////////////////////////////////////////////
// 三维几何变换
// 错切（参考z轴）
Matrix shearZrefMatrix(float shzx, float shzy, float zref)
{
	Matrix Mzshear(4, 4);
	matrixSetIdentity(Mzshear);
	Mzshear[0][2] = shzx;
	Mzshear[0][3] = -shzx * zref;
	Mzshear[1][2] = shzy;
	Mzshear[1][3] = -shzy * zref;
	return Mzshear;
}

//////////////////////////////////////////////////////////////////////////
// 三维观察矩阵
// 建模变换
Matrix modelMatrix(Point o, Vec3 X, Vec3 Y)
{
	Vec3 x = X;
	normal(x);
	Vec3 z = cross(x, Y);
	normal(z);
	Vec3 y = cross(z, x);

	Matrix T(4, 4);
	matrixSetIdentity(T);
	T[0][3] = -o.x;
	T[1][3] = -o.y;
	T[2][3] = -o.z;

	Matrix R(4, 4);
	matrixSetIdentity(R);
	R[0][0] = x.x;
	R[0][1] = x.y;
	R[0][2] = x.z;
	R[1][0] = y.x;
	R[1][1] = y.y;
	R[1][2] = y.z;
	R[2][0] = z.x;
	R[2][1] = z.y;
	R[2][2] = z.z;

	return R * T;
}
// 观察变换
Matrix viewMatrix(Point p0, Vec3 N, Vec3 V)
{
	Vec3 n = N;
	normal(n);
	Vec3 u = cross(V, n);
	normal(u);
	Vec3 v = cross(n, u);

	Matrix T(4, 4);
	matrixSetIdentity(T);
	T[0][3] = -p0.x;
	T[1][3] = -p0.y;
	T[2][3] = -p0.z;

	Matrix R(4, 4);
	matrixSetIdentity(R);
	R[0][0] = u.x;
	R[0][1] = u.y;
	R[0][2] = u.z;
	R[1][0] = v.x;
	R[1][1] = v.y;
	R[1][2] = v.z;
	R[2][0] = n.x;
	R[2][1] = n.y;
	R[2][2] = n.z;

	return R * T;
}
// 正投影变换
Matrix orthogonalProjectionMatrix()
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
	return m;
}
// 平行投影变换
Matrix parallelProjectionMatrix(Vec3 Vp, float zvp)
{
	Matrix M_oblique(4, 4);
	matrixSetIdentity(M_oblique);
	M_oblique[0][2] = -Vp.x / Vp.z;
	M_oblique[0][3] = zvp * Vp.x / Vp.z;
	M_oblique[1][2] = -Vp.y / Vp.z;
	M_oblique[1][3] = zvp * Vp.y / Vp.z;

	return M_oblique;
}
// 规范化变换
Matrix normalMatrix(float xwmin, float xwmax, float ywmin, float ywmax, float znear, float zfar)
{
	Matrix M_ortho_norm(4, 4);
	matrixSetIdentity(M_ortho_norm);
	M_ortho_norm[0][0] = 2 / (xwmax - xwmin);
	M_ortho_norm[0][3] = -(xwmax + xwmin) / (xwmax - xwmin);
	M_ortho_norm[1][1] = 2 / (ywmax - ywmin);
	M_ortho_norm[1][3] = -(ywmax + ywmin) / (ywmax - ywmin);
	M_ortho_norm[2][2] = -2 / (znear - zfar);
	M_ortho_norm[2][3] = (znear + zfar) / (znear - zfar);

	return M_ortho_norm;
}
// 视口变换
Matrix viewportMatrix(float xvmin, float xvmax, float yvmin, float yvmax)
{
	Matrix M_normviewvol_3Dscreen(4, 4);
	matrixSetIdentity(M_normviewvol_3Dscreen);
	M_normviewvol_3Dscreen[0][0] = (xvmax - xvmin) / 2;
	M_normviewvol_3Dscreen[0][3] = (xvmax + xvmin) / 2;
	M_normviewvol_3Dscreen[1][1] = (yvmax - yvmin) / 2;
	M_normviewvol_3Dscreen[1][3] = (yvmax + yvmin) / 2;
	M_normviewvol_3Dscreen[2][2] = 0.5;
	M_normviewvol_3Dscreen[2][3] = 0.5;

	return M_normviewvol_3Dscreen;
}


#endif

#ifdef CHAPTER_10_10_6
GLint winWidth = 600, winHeight = 600;
GLfloat x0 = 100.0, y_0 = 50.0, z0 = 50.0;
GLfloat xref = 50.0, yref = 50.0, zref = 0.0;
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;
GLfloat xwMin = -40.0, ywMin = -60.0, xwMax = 40.0, ywMax = 60.0;
GLfloat dnear = 25.0, dfar = 125.0;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(x0, y_0, z0, xref, yref, zref, Vx, Vy, Vz);
	GLfloat test[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, test);

	/*glLoadIdentity();
	Vec3 n = { x0 - xref, y_0 - yref, z0 - zref };
	normal(n);
	Vec3 v = { Vx, Vy, Vz };
	normal(v);
	Vec3 u = cross(v, n);
	normal(u);
	Matrix r(4, 4);
	matrixSetIdentity(r);
	r[0][0] = u.x;
	r[0][1] = u.y;
	r[0][2] = u.z;
	r[1][0] = v.x;
	r[1][1] = v.y;
	r[1][2] = v.z;
	r[2][0] = n.x;
	r[2][1] = n.y;
	r[2][2] = n.z;
	glMultMatrixf(r);

	Matrix t(4, 4);
	matrixSetIdentity(t);
	t[0][3] = -x0;
	t[1][3] = -y_0;
	t[2][3] = -z0;

	glMultMatrixf(t);
	GLfloat test[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, test);*/
	
	glMatrixMode(GL_PROJECTION);
	glFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	
	glBegin(GL_QUADS);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	glVertex3f(100.0, 100.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	glEnd();

	glFlush();
}
void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Perspective View of A Square");

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(reshapeFcn);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_10_EXERCISE_1
Matrix worldToView1(Point p0, Vec3 N, Vec3 V)
{
	Vec3 n = N;
	normal(n);
	Vec3 u = cross(V, n);
	normal(u);
	Vec3 v = cross(n, u);

	Matrix T(4, 4);
	matrixSetIdentity(T);
	T[0][3] = -p0.x;
	T[1][3] = -p0.y;
	T[2][3] = -p0.z;

	Matrix R(4, 4);
	matrixSetIdentity(R);
	R[0][0] = u.x;
	R[0][1] = u.y;
	R[0][2] = u.z;
	R[1][0] = v.x;
	R[1][1] = v.y;
	R[1][2] = v.z;
	R[2][0] = n.x;
	R[2][1] = n.y;
	R[2][2] = n.z;

	return R * T;
}
Matrix worldToView2(Point p0, Vec3 N, Vec3 V)
{
	Vec3 n = N;
	normal(n);
	Vec3 u = cross(V, n);
	normal(u);
	Vec3 v = cross(n, u);

	Matrix m(4, 4);
	matrixSetIdentity(m);

	m[0][0] = u.x;
	m[0][1] = u.y;
	m[0][2] = u.z;
	m[1][0] = v.x;
	m[1][1] = v.y;
	m[1][2] = v.z;
	m[2][0] = n.x;
	m[2][1] = n.y;
	m[2][2] = n.z;

	m[0][3] = -dot(u, p0);
	m[1][3] = -dot(v, p0);
	m[2][3] = -dot(n, p0);

	return m;
}
void printMatrix(Matrix& m)
{
	for (int r = 0; r < m._row; r++)
	{
		for (int c = 0; c < m._col; c++)
		{
			printf("%.4f	", m[r][c]);
		}
		printf("\n");
	}
}
void code_10_exercise_1()
{
	Point p0 = { 1, 1, 1 };
	Vec3 N = { 0, 0, 1 };
	Vec3 V = { 1, 1, 0 };
	Matrix m1 = worldToView1(p0, N, V);
	Matrix m2 = worldToView2(p0, N, V);
	printf("P0=(%f,%f,%f),N=(%f,%f,%f),V=(%f,%f,%f)\n", p0.x, p0.y, p0.z, N.x, N.y, N.z, V.x, V.y, V.z);
	printf(m1 == m2 ? "true" : "false");
	printf("\n");
	printMatrix(m1);

	printf("\n");
	V = { 1, 1, 1 };
	Matrix m3 = worldToView1(p0, N, V);
	Matrix m4 = worldToView2(p0, N, V);
	printf("P0=(%f,%f,%f),N=(%f,%f,%f),V=(%f,%f,%f)\n", p0.x, p0.y, p0.z, N.x, N.y, N.z, V.x, V.y, V.z);
	printf(m3 == m4 ? "true" : "false");
	printf("\n");
	printMatrix(m3);

	cin.get();
	cin.get();
}
#endif

#ifdef CHAPTER_10_EXERCISE_2
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> cube = { { -50.f, -50.f, 50.f }, { 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f}, {-50.f, 50.f, 50.f},
	{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } };
	std::vector<std::vector<int>> indexs = { { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } };

	// 正投影
	auto temp = cube;
	transformPoints(viewportMatrix(0, winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	// 正投影(平行投影)
	temp = cube;
	transformPoints(viewportMatrix(winWidth / 3, 2 * winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		parallelProjectionMatrix({0, 0, 1}, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	// 斜平行投影
	temp = cube;
	transformPoints(viewportMatrix(2 * winWidth / 3, winWidth, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		parallelProjectionMatrix({ 1.f, 1.f, 2 * 1.414f }, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	glFlush();
}

void code_10_exercise_2()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_3
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> cube = { { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
	{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } };
	std::vector<std::vector<int>> indexs = { { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } };

	// 正投影
	auto temp = cube;
	transformPoints(viewportMatrix(0, winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	// 正投影(平行投影)
	temp = cube;
	transformPoints(viewportMatrix(winWidth / 3, 2 * winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		shearZrefMatrix(0, 0, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	// 斜平行投影
	temp = cube;
	Vec3 Vp = { 1.f, 1.f, 2 * 1.414f };
	transformPoints(viewportMatrix(2 * winWidth / 3, winWidth, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		shearZrefMatrix(-Vp.x / Vp.z, -Vp.y / Vp.z, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	glFlush();
}

void code_10_exercise_3()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_4
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	std::vector<Point> cube = { { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
	{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } };
	std::vector<std::vector<int>> indexs = { { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } };

	// 正投影
	auto temp = cube;
	transformPoints(viewportMatrix(0, winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	// 正投影(平行投影)
	temp = cube;
	transformPoints(viewportMatrix(winWidth / 3, 2 * winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		shearZrefMatrix(0, 0, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	// 斜平行投影
	temp = cube;
	Vec3 Vp = { 1.f, 1.f, 2 * 1.414f };
	transformPoints(viewportMatrix(2 * winWidth / 3, winWidth, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		shearZrefMatrix(-Vp.x / Vp.z, -Vp.y / Vp.z, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp);
	drawPolyhedron(temp, indexs);

	glFlush();
}

void code_10_exercise_4()
{
	glutDisplayFunc(displayFcn);
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_10_COMMON

#ifdef CHAPTER_10_COMMON
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // 黑色背景色
	//glColor3f(1.0, 1.0, 1.0); // 白色绘制
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

#ifdef CHAPTER_10_EXERCISE_1
	code_10_exercise_1();
#endif

#ifdef CHAPTER_10_EXERCISE_2
	code_10_exercise_2();
#endif

#ifdef CHAPTER_10_EXERCISE_3
	code_10_exercise_3();
#endif

#ifdef CHAPTER_10_EXERCISE_4
	code_10_exercise_4();
#endif

	glutMainLoop();
}
#endif
#endif 


