#include "common.h"

#if CHAPTER==10
#include "opengl10h.h"

#ifdef CHAPTER_10_COMMON
float winWidth = 800, winHeight = 600;
float floatEqualValue = 0.0001f;
//////////////////////////////////////////////////////////////////////////
// 判断浮点数相等
inline bool Equal(float f1, float f2) { return std::abs(f1 - f2) < floatEqualValue; }
inline bool Greater(float f1, float f2) { return Equal(f1, f2) ? false : (f1 > f2); }
inline bool Less(float f1, float f2) { return Equal(f1, f2) ? false : (f1 < f2); }
inline bool GreaterQ(float f1, float f2) { return Greater(f1, f2) || Equal(f1, f2); }
inline bool LessQ(float f1, float f2) { return Less(f1, f2) || Equal(f1, f2); }

//////////////////////////////////////////////////////////////////////////
// 点
class Point
{
public:
	GLfloat x, y, z, h;
	Point(float x = 0.f, float y = 0.f, float z = 0.f, float h = 1.f) :x(x), y(y), z(z), h(h) {}
};
bool operator ==(const Point& p1, const Point& p2)
{
	return Equal(p1.x, p2.x) && Equal(p1.y, p2.y) && Equal(p1.z, p2.z) && Equal(p1.h, p2.h);
}
bool operator!=(const Point& p1, const Point& p2)
{
	return !(p1 == p2);
}
bool operator < (const Point& p1, const Point& p2)
{
	if (Less(p1.x, p2.x))
	{
		return true;
	}
	else if (Less(p2.x, p1.x))
	{
		return false;
	}
	else if (Less(p1.y, p2.y))
	{
		return true;
	}
	else if (Less(p2.y, p1.y))
	{
		return false;
	}
	else if (Less(p1.z, p2.z))
	{
		return true;
	}
	else if (Less(p2.z, p1.z))
	{
		return false;
	}
	else
		return false;
}
void descartes(Point& p)
{
	p.x = p.x / p.h;
	p.y = p.y / p.h;
	p.z = p.z / p.h;
	p.h = 1.f;
}
void homogeneous(Point& p, float h)
{
	p.x = p.x * h;
	p.y = p.y * h;
	p.z = p.z * h;
	p.h = h;
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
		point[3][0] = p.h;
		auto temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
		p.z = temp[2][0];
		p.h = temp[3][0];
	}
}
void transformPoint(Matrix& m, Point& point)
{
	Matrix _point(4, 1);
	Matrix temp(4, 1);
	_point[0][0] = point.x;
	_point[1][0] = point.y;
	_point[2][0] = point.z;
	_point[3][0] = point.h;
	temp = m * _point;
	point.x = temp[0][0];
	point.y = temp[1][0];
	point.z = temp[2][0];
	point.h = temp[3][0];
}

//////////////////////////////////////////////////////////////////////////
// 向量
class Vec3
{
public:
	GLfloat x, y, z;
};
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
float sign(float f)
{
	if (f < 0)
		return -1;
	else if (f > 0)
		return 1;
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////////
// 多面体
class Polyhedron
{
public:
	std::vector<Point> points;
	std::vector<std::vector<int>> indexs;
};

//////////////////////////////////////////////////////////////////////////
// 绘制
// 直线
void drawLine(Point p1, Point p2)
{
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}
// 二维多边形
void drawPolygon(const std::vector<Point>& points)
{
	glBegin(GL_LINE_LOOP);
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
void drawPolyhedron(const Polyhedron& polyhedron)
{
	for (auto& index : polyhedron.indexs)
	{
		glBegin(GL_LINE_LOOP);
		for (auto& i : index)
		{
			glVertex2f(polyhedron.points[i].x, polyhedron.points[i].y);
		}
		glEnd();
	}
}
void drawPolyhedron3D(const Polyhedron& polyhedron)
{
	for (auto& index : polyhedron.indexs)
	{
		glBegin(GL_LINE_LOOP);
		for (auto& i : index)
		{
			glVertex3f(polyhedron.points[i].x, polyhedron.points[i].y, polyhedron.points[i].z);
		}
		glEnd();
	}
}


//////////////////////////////////////////////////////////////////////////
// 三维几何变换
// 平移
Matrix translateMatrix(float tx, float ty, float tz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	return m;
}
// 任意轴旋转
Matrix rotateMatrix(Point p0, Vec3 u, float theta)
{
	normal(u);

	Matrix T_(4, 4);
	matrixSetIdentity(T_);
	T_[0][3] = p0.x;
	T_[1][3] = p0.y;
	T_[2][3] = p0.z;

	Matrix MR(4, 4);
	matrixSetIdentity(MR);
	float cos = std::cos(theta);
	float sin = std::sin(theta);
	MR[0][0] = u.x * u.x * (1 - cos) + cos;
	MR[0][1] = u.x * u.y * (1 - cos) - u.z * sin;
	MR[0][2] = u.x * u.z * (1 - cos) + u.y * sin;
	MR[1][0] = u.y * u.x * (1 - cos) + u.z * sin;
	MR[1][1] = u.y * u.y * (1 - cos) + cos;
	MR[1][2] = u.y * u.z * (1 - cos) - u.x * sin;
	MR[2][0] = u.z * u.x * (1 - cos) - u.y * sin;
	MR[2][1] = u.z * u.y * (1 - cos) + u.x * sin;
	MR[2][2] = u.z * u.z * (1 - cos) + cos;

	Matrix T(4, 4);
	matrixSetIdentity(T);
	T[0][3] = -p0.x;
	T[1][3] = -p0.y;
	T[2][3] = -p0.z;

	return T_ * MR * T;
}
// 任意点缩放
Matrix scaleMatrix(Point p0, float sx, float sy, float sz)
{
	// 基于指定点缩放
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[0][3] = p0.x * (1 - sx);
	ret[1][1] = sy;
	ret[1][3] = p0.y * (1 - sy);
	ret[2][2] = sz;
	ret[2][3] = p0.z * (1 - sz);
	return ret;
}
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
// 透视投影和规范化矩阵
Matrix perspectiveProjectionAndNormalMatrix(float xwmin, float xwmax, float ywmin, float ywmax, float znear, float zfar)
{
	Matrix M_normpers(4, 4);
	matrixSetIdentity(M_normpers);
	M_normpers[0][0] = -2 * znear / (xwmax - xwmin);
	M_normpers[0][2] = (xwmax + xwmin) / (xwmax - xwmin);
	M_normpers[1][1] = -2 * znear / (ywmax - ywmin);
	M_normpers[1][2] = (ywmax + ywmin) / (ywmax - ywmin);
	M_normpers[2][2] = (znear + zfar) / (znear - zfar);
	M_normpers[2][3] = -2 * znear * zfar / (znear - zfar);
	M_normpers[3][2] = -1;
	M_normpers[3][3] = 0;

	return M_normpers;
}
// 透视投影
Matrix perspectiveProjectionMatrix(float xwmin, float xwmax, float ywmin, float ywmax, float znear, float zfar)
{
	Matrix M_obliquepers(4, 4);
	matrixSetIdentity(M_obliquepers);
	M_obliquepers[0][0] = -znear;
	M_obliquepers[0][2] = (xwmax + xwmin) / 2;
	M_obliquepers[1][1] = -znear;
	M_obliquepers[1][2] = (ywmax + ywmin) / 2;
	M_obliquepers[2][2] = -(znear + zfar);
	M_obliquepers[2][3] = znear * zfar;
	M_obliquepers[3][2] = -1;
	M_obliquepers[3][3] = 0;

	return M_obliquepers;
}
// 规范化变换（透视投影）
Matrix normalMatrix_perspective(float xwmin, float xwmax, float ywmin, float ywmax, float znear, float zfar)
{
	Matrix M_ortho_norm(4, 4);
	matrixSetIdentity(M_ortho_norm);
	M_ortho_norm[0][0] = 2 / (xwmax - xwmin);
	M_ortho_norm[1][1] = 2 / (ywmax - ywmin);
	M_ortho_norm[2][2] = -2 / (znear - zfar);
	M_ortho_norm[2][3] = (znear + zfar) / (znear - zfar);

	return M_ortho_norm;
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

class Camera
{
public:
#define GLUT_WHEEL_UP 0x0003
#define GLUT_WHEEL_DOWN 0x0004

	static Camera* _this;
	Point _viewP0;
	Vec3 _u, _v, _n;

	Camera(Point viewP0, Vec3 N, Vec3 V, float zViewDistance)
	{
		_this = this;
		_viewP0 = viewP0;
		init(N, V);
		_zViewDistance = zViewDistance;
		glutMouseFunc(&mouseFunc);
		glutMotionFunc(&mouseMoveFunc);
	}
	void static mouseFunc(int button, int state, int x, int y)
	{
		//printf("mouseFunc %d, %d, %d %d\n", button, state, x, y);
		y = _this->realY(y);
		_this->_mouseFunc(button, state, x, y);
	}
	void static mouseMoveFunc(int x, int y)
	{
		//printf("mouseMoveFunc %d, %d\n", x, y);
		y = _this->realY(y);
		_this->_mouseMoveFunc(x, y);
	}
private:
	float _zViewDistance;
	int _pressButton = 0;
	int _lastX, _lastY;
	void init(Vec3 N, Vec3 V)
	{
		_n = N;
		normal(_n);
		_u = cross(V, _n);
		normal(_u);
		_v = cross(_n, _u);
	}
	void _mouseFunc(int button, int state, int x, int y)
	{
		switch (button)
		{
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
			{
				_lastX = x;
				_lastY = y;
				_pressButton = GLUT_MIDDLE_BUTTON;
			}
			else if (state == GLUT_UP)
			{
				_pressButton = 0;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
			{
				_lastX = x;
				_lastY = y;
				_pressButton = GLUT_RIGHT_BUTTON;
			}
			else if (state == GLUT_UP)
			{
				_pressButton = 0;
			}
			break;
		case GLUT_WHEEL_UP:
			if (state == GLUT_UP)
			{
				moveAxis("z", -5);
			}
			break;
		case GLUT_WHEEL_DOWN:
			if (state == GLUT_UP)
			{
				moveAxis("z", 5);
			}
			break;
		default:
			break;
		}

	}
	void _mouseMoveFunc(int x, int y)
	{
		if (_pressButton == GLUT_MIDDLE_BUTTON)
		{
			float dx = x - _lastX;
			float dy = y - _lastY;
			_lastX = x;
			_lastY = y;
			moveAxis("x", -dx);
			moveAxis("y", -dy);
		}
		else if (_pressButton == GLUT_RIGHT_BUTTON)
		{
			float dx = x - _lastX;
			float dy = y - _lastY;
			_lastX = x;
			_lastY = y;

			float ry = -dx / _zViewDistance;
			auto Ry = rotateMatrix({ 0, 0, 0 }, { 0, 1, 0 }, ry);
			float rx = dy / _zViewDistance;
			auto Rx = rotateMatrix({ 0, 0, 0 }, _u, rx);

			auto m = Rx * Ry;
			Point pu = { _u.x, _u.y, _u.z };
			Point pv = { _v.x, _v.y, _v.z };
			Point pn = { _n.x, _n.y, _n.z };
			transformPoint(m, pu);
			transformPoint(m, pv);
			transformPoint(m, pn);
			_u.x = pu.x;
			_u.y = pu.y;
			_u.z = pu.z;
			_v.x = pv.x;
			_v.y = pv.y;
			_v.z = pv.z;
			_n.x = pn.x;
			_n.y = pn.y;
			_n.z = pn.z;
			normal(_u);
			normal(_n);
			normal(_v);

			glutPostRedisplay();
		}
	}
	void moveAxis(std::string axis, float d)
	{
		if (axis == "x")
		{
			float r = length(_u);
			_viewP0.x += _u.x / r * d;
			_viewP0.y += _u.y / r * d;
			_viewP0.z += _u.z / r * d;
		}
		if (axis == "y")
		{
			float r = length(_v);
			_viewP0.x += _v.x / r * d;
			_viewP0.y += _v.y / r * d;
			_viewP0.z += _v.z / r * d;
		}
		if (axis == "z")
		{
			float r = length(_n);
			_viewP0.x += _n.x / r * d;
			_viewP0.y += _n.y / r * d;
			_viewP0.z += _n.z / r * d;
		}
		glutPostRedisplay();
	}
	float realY(float y)
	{
		return winHeight - y;
	}
};
Camera* Camera::_this = nullptr;

#ifdef ClipPolyhedron
const int LeftClipPlane = 1;
const int RightClipPlane = 2;
const int BottomClipPlane = 3;
const int TopClipPlane = 4;
const int NearClipPlane = 5;
const int FarClipPlane = 6;
const std::vector<int> clipPlanes = { LeftClipPlane, RightClipPlane, BottomClipPlane, TopClipPlane, NearClipPlane, FarClipPlane };
void addCrossInfo(Point p1, Point p2, map<Point, map<Point, int>>& crossInfo)
{
	//auto it1 = crossInfo.find(p1);
	//if (it1 != crossInfo.end())
	//{
	//	it1->second[p2] = 1;
	//}
	//else
	//{
	//	crossInfo[p1][p2] = 1;
	//}
	crossInfo[p1][p2] = 1;
}
bool checkSamePoint(vector<Point>& points)
{
	map<Point, int> testSame;
	for (auto it = points.begin(); it != points.end();)
	{
		if (testSame.find(*it) == testSame.end())
		{
			testSame[*it] = 1;
			it++;
		}
		else
		{
			it = points.erase(it);
		}
	}

	return points.size() >= 3;
}
void addSurface(Polyhedron& polyhedron, vector<Point>& points, map<Point, int>& pointInfo)
{
	if (checkSamePoint(points))
	{
		polyhedron.indexs.push_back({});
		for (int i = 0; i < points.size(); i++)
		{
			auto it = pointInfo.find(points[i]);
			if (it != pointInfo.end())
			{
				polyhedron.indexs.back().push_back(it->second);
			}
			else
			{
				polyhedron.points.push_back(points[i]);
				int idx = polyhedron.points.size() - 1;
				polyhedron.indexs.back().push_back(idx);

				pointInfo[points[i]] = idx;
			}
		}
	}
}
vector<Point> createClipSuface(map<Point, map<Point, int>>& crossInfo)
{
	vector<Point> ret;

	if (crossInfo.size() < 3)
		return ret;

	auto beginPoint = crossInfo.begin()->first;
	ret.push_back(beginPoint);

	auto curPoint = beginPoint;
	while (true)
	{
		//assert(crossInfo.find(curPoint) != crossInfo.end());
		if (crossInfo.find(curPoint) == crossInfo.end())
		{
			ret.clear();
			return ret;
		}
		auto nextPoint = crossInfo[curPoint].begin()->first;
		assert(crossInfo[curPoint][nextPoint] == 1);

		//assert(crossInfo.find(nextPoint) != crossInfo.end());
		if (crossInfo.find(nextPoint) == crossInfo.end())
		{
			ret.clear();
			return ret;
		}
		if (crossInfo[nextPoint].find(curPoint) == crossInfo[nextPoint].end())
		{
			ret.clear();
			return ret;
		}
		assert(crossInfo[nextPoint][curPoint] == 1);

		crossInfo[curPoint].erase(nextPoint);
		if (crossInfo[curPoint].size() == 0)
		{
			crossInfo.erase(curPoint);
		}

		crossInfo[nextPoint].erase(curPoint);
		if (crossInfo[nextPoint].size() == 0)
		{
			crossInfo.erase(nextPoint);
		}

		if (nextPoint != beginPoint)
			ret.push_back(nextPoint);
		else
			break;
		curPoint = nextPoint;
	}

	return ret;
}

void antiClockPoints(vector<Point>& points, Vec3 normalVec)
{
	Vec3 a = { points[1].x - points[0].x, points[1].y - points[0].y, -(points[1].z - points[0].z) }; // z取负值是因为在左手坐标系和右手坐标系z反号
	Vec3 b = { points[2].x - points[1].x, points[2].y - points[1].y, -(points[2].z - points[1].z) };
	Vec3 crossVec = cross(a, b);
	bool antiClocked = crossVec.x * normalVec.x >= 0 && crossVec.y * normalVec.y >= 0 && crossVec.z * normalVec.z >= 0;
	if (!antiClocked)
	{
		std::reverse(points.begin(), points.end());
	}
}
Vec3 getClipPlaneNormalVec(int clipPlane)
{
	switch (clipPlane)
	{
	case LeftClipPlane:
		return{ -1, 0, 0 };
	case RightClipPlane:
		return{ 1, 0, 0 };
	case BottomClipPlane:
		return{ 0, -1, 0 };
	case TopClipPlane:
		return{ 0, 1, 0 };
	case NearClipPlane:
		return{ 0, 0, -1 };
	case FarClipPlane:
		return{ 0, 0, 1 };
	}
}

bool inside(Point p, int clipPlane)
{
	switch (clipPlane)
	{
	case LeftClipPlane:
		return p.x > -1;
	case RightClipPlane:
		return p.x < 1;
	case BottomClipPlane:
		return p.y > -1;
	case TopClipPlane:
		return p.y < 1;
	case NearClipPlane:
		return p.z > -1;
	case FarClipPlane:
		return p.z < 1;
	}
}
Point clacCrossPoint(Point p1, Point p2, int clipPlane)
{
	float u = 0.f;
	switch (clipPlane)
	{
	case LeftClipPlane:
		u = (p1.x + 1) / (p1.x - p2.x);
		break;
	case RightClipPlane:
		u = (p1.x - 1) / (p1.x - p2.x);
		break;
	case BottomClipPlane:
		u = (p1.y + 1) / (p1.y - p2.y);
		break;
	case TopClipPlane:
		u = (p1.y - 1) / (p1.y - p2.y);
		break;
	case NearClipPlane:
		u = (p1.z + 1) / (p1.z - p2.z);
		break;
	case FarClipPlane:
		u = (p1.z - 1) / (p1.z - p2.z);
		break;
	default:
		break;
	}

	return{ p1.x + (p2.x - p1.x) * u,
		p1.y += (p2.y - p1.y) * u ,
		p1.z += (p2.z - p1.z) * u };
}
vector<Point> clipSurface(const vector<Point>& points, int clipPlane, map<Point, map<Point, int>>& crossInfo)
{
	if (points.size() < 3)
	{
		printf("%d\n", points.size());
	}

	vector<Point> ret;
	vector<Point> cross;
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 < points.size() ? i + 1 : 0;

		if (!inside(points[i], clipPlane))
		{
			if (inside(points[next], clipPlane))
			{
				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
				ret.push_back(cp);
				ret.push_back(points[next]);

				cross.push_back(cp);
			}
		}
		else
		{
			if (inside(points[next], clipPlane))
			{
				ret.push_back(points[next]);
			}
			else
			{
				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
				ret.push_back(cp);
				cross.push_back(cp);
			}
		}
	}
	if (cross.size() > 1 && cross[0] != cross[1])
	{
		addCrossInfo(cross[0], cross[1], crossInfo);
		addCrossInfo(cross[1], cross[0], crossInfo);
	}

	return ret;
}
void clipClipPlane(Polyhedron& polyhedron, int clipPlane)
{
	map<Point, int> pointInfo;
	map<Point, map<Point, int>> crossInfo;
	Polyhedron newPolyhedron;

	vector<Point> surface;
	for (auto& index : polyhedron.indexs)
	{
		surface.clear();
		for (auto& idx : index)
			surface.push_back(polyhedron.points[idx]);

		auto points = clipSurface(surface, clipPlane, crossInfo);
		if (points.size() > 0)
			addSurface(newPolyhedron, points, pointInfo);
	}

	// 裁剪面上的面
	if (crossInfo.size() > 2)
	{
		auto clipSurfacePoints = createClipSuface(crossInfo);
		antiClockPoints(clipSurfacePoints, getClipPlaneNormalVec(clipPlane));
		addSurface(newPolyhedron, clipSurfacePoints, pointInfo);
	}

	polyhedron = newPolyhedron;
}
bool checkEntire(Polyhedron& polyhedron)
{
	bool allInside = true;
	for (auto& clipPlane : clipPlanes)
	{
		bool allOutside = true;
		for (auto& p : polyhedron.points)
		{
			if (inside(p, clipPlane))
				allOutside = false;
			else
				allInside = false;
		}

		if (allInside)
		{
			continue;
		}

		if (allOutside)
		{
			polyhedron.points = {};
			polyhedron.indexs = {};
			return true;
		}
	}

	return allInside;
}
void clip(Polyhedron& polyhedron)
{
	if (!checkEntire(polyhedron))
	{
		for (auto& clipPlane : clipPlanes)
		{
			clipClipPlane(polyhedron, clipPlane);
		}
	}
}
#endif

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

	// 测试opengl观察矩阵
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

// 测试摄像机
//Point viewP0 = { 0, 0, 100 };
//GLfloat dnear = 1, dfar = 2000;
//std::vector<Point> cube = { { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
//{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } };
//std::vector<std::vector<int>> indexs = { { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } };
//Camera *camera = nullptr;
//void viewLine1()
//{
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0, winWidth, 0, winHeight);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glViewport(0, 0, winWidth, winHeight);
//
//	auto temp = cube;
//
//	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
//	float clipWinWidth = winWidth / 2 / winHeight * clipWinHeight;
//	transformPoints(perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -dnear, -dfar) *
//		viewMatrix(camera->_viewP0, camera->_n, camera->_v), temp);
//
//	for (auto& p : temp)
//		descartes(p);
//	
//	transformPoints(viewportMatrix(winWidth / 2, winWidth, 0, winHeight), temp);
//
//	drawPolyhedron(temp, indexs);
//}
//void displayFcn(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glColor3f(1.0, 1.0, 1.0);
//	
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	Point pu = { camera->_viewP0.x - camera->_n.x,
//		camera->_viewP0.y - camera->_n.y,
//		camera->_viewP0.z - camera->_n.z, };
//	gluLookAt(camera->_viewP0.x, camera->_viewP0.y, camera->_viewP0.z, pu.x, pu.y, pu.z, camera->_v.x, camera->_v.y, camera->_v.z);
//	
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
//	float clipWinWidth = winWidth / 2 / winHeight * clipWinHeight;
//
//	glFrustum(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, dnear, dfar);
//	//glFrustum(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2, 724.264, dfar);
//	//gluPerspective(45.f, (GLfloat)winWidth / (GLfloat)winHeight, dnear, dfar);
//	glViewport(0, 0, winWidth / 2, winHeight);
//	drawPolyhedron3D(cube, indexs);
//
//	viewLine1();
//	
//	glFlush();
//}
//void ch_init(void)
//{
//	glutDisplayFunc(displayFcn);
//	camera = new Camera({ 200, 200, 1000 }, { 0, 0, 1 }, { 0, 1, 0 }, 1000);
//}
#endif

#ifdef CHAPTER_10_CAMERA
Point viewP0 = { 0, 0, 100 };
GLfloat dnear = 1, dfar = 2000;
Camera *camera = nullptr;
Polyhedron cube = {
	{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },

{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};
void view()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, winWidth, winHeight);

	auto temp = cube;

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / 2 / winHeight * clipWinHeight;
	transformPoints(perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -dnear, -dfar) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v), temp.points);

	for (auto& p : temp.points)
		descartes(p);
	
	transformPoints(viewportMatrix(winWidth / 2, winWidth, 0, winHeight), temp.points);

	drawPolyhedron(temp);
}
void viewOpenGL()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Point pu = { camera->_viewP0.x - camera->_n.x,
		camera->_viewP0.y - camera->_n.y,
		camera->_viewP0.z - camera->_n.z, };
	gluLookAt(camera->_viewP0.x, camera->_viewP0.y, camera->_viewP0.z, pu.x, pu.y, pu.z, camera->_v.x, camera->_v.y, camera->_v.z);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / 2 / winHeight * clipWinHeight;

	glFrustum(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, dnear, dfar);
	//glFrustum(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, winHeight / 2 / tan(45.f / 2 * PI / 180), dfar);
	//gluPerspective(45.f, (GLfloat)winWidth / 2 / (GLfloat)winHeight, dnear, dfar);
	glViewport(0, 0, winWidth / 2, winHeight);
	drawPolyhedron3D(cube);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	
	viewOpenGL();

	view();
	
	glFlush();
}
void code_10_camera(void)
{
	glutDisplayFunc(displayFcn);
	camera = new Camera({ 200, 200, 1000 }, { 0, 0, 1 }, { 0, 1, 0 }, 1000);
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

	m[0][3] = -dot(u, { p0.x, p0.y, p0.z });
	m[1][3] = -dot(v, { p0.x, p0.y, p0.z });
	m[2][3] = -dot(n, { p0.x, p0.y, p0.z });

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

	Polyhedron cube = {
		{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
		{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },

		{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
	};

	// 正投影
	auto temp = cube;
	transformPoints(viewportMatrix(0, winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	drawPolyhedron(temp);

	// 正投影(平行投影)
	temp = cube;
	transformPoints(viewportMatrix(winWidth / 3, 2 * winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		parallelProjectionMatrix({0, 0, 1}, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	drawPolyhedron(temp);

	// 斜平行投影
	temp = cube;
	transformPoints(viewportMatrix(2 * winWidth / 3, winWidth, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		parallelProjectionMatrix({ 1.f, 1.f, 2 * 1.414f }, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	drawPolyhedron(temp);

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

	Polyhedron cube = {
		{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
		{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },

		{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
	};

	// 正投影
	auto temp = cube;
	transformPoints(viewportMatrix(0, winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	drawPolyhedron(temp);

	// 正投影(平行投影)
	temp = cube;
	transformPoints(viewportMatrix(winWidth / 3, 2 * winWidth / 3, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		shearZrefMatrix(0, 0, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	drawPolyhedron(temp);

	// 斜平行投影
	temp = cube;
	Vec3 Vp = { 1.f, 1.f, 2 * 1.414f };
	transformPoints(viewportMatrix(2 * winWidth / 3, winWidth, 0, winHeight) *
		normalMatrix(-winWidth / 6, winWidth / 6, -winHeight / 2, winHeight / 2, -60.f, -200.f) *
		orthogonalProjectionMatrix() *
		shearZrefMatrix(-Vp.x / Vp.z, -Vp.y / Vp.z, -50.f) *
		viewMatrix({ winWidth / 6, winHeight / 2, 220.f }, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -300.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	drawPolyhedron(temp);

	glFlush();
}

void code_10_exercise_3()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_4
Point viewP0 = { 160.f, 170.f, 200.f };
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	Polyhedron cube = {
		{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
		{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },

		{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
	};
	
	// 直接使用复合投影+规范化矩阵
	auto temp = cube;
	transformPoints(viewMatrix(viewP0, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionAndNormalMatrix(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -40.f, -200.f), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(viewportMatrix(0, winWidth / 2, 0, winHeight), temp.points);
	drawPolyhedron(temp);

	// 单独的投影，规范化矩阵
	temp = cube;
	transformPoints(viewMatrix(viewP0, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionMatrix(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -40.f, -200.f), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(normalMatrix_perspective(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -40.f, -200.f), temp.points);
	transformPoints(viewportMatrix(winWidth / 2, winWidth, 0, winHeight), temp.points);
	drawPolyhedron(temp);

	glFlush();
}

void code_10_exercise_4()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_5
Point viewP0 = { 200.f, 100.f, 200.f };
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	Polyhedron cube = {
		{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
		{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },

		{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
	};

	// 直接使用复合投影+规范化矩阵
	auto temp = cube;
	transformPoints(viewMatrix(viewP0, { 1, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionAndNormalMatrix(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -60.f, -300.f), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(viewportMatrix(0, winWidth / 2, 0, winHeight), temp.points);
	drawPolyhedron(temp);

	// 单独的投影，规范化矩阵
	temp = cube;
	transformPoints(viewMatrix(viewP0, { 1, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionMatrix(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -60.f, -300.f), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(normalMatrix_perspective(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -60.f, -300.f), temp.points);
	transformPoints(viewportMatrix(winWidth / 2, winWidth, 0, winHeight), temp.points);
	drawPolyhedron(temp);

	glFlush();
}

void code_10_exercise_5()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_6
Point viewP0 = { 200.f, 200.f, 200.f};
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	Polyhedron cube = {
		{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
		{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },

		{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
	};
	
	// 直接使用复合投影+规范化矩阵
	auto temp = cube;
	transformPoints(viewMatrix(viewP0, { 1, 1, 1 }, { -100, 200, -100 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionAndNormalMatrix(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -75.f, -400.f), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(viewportMatrix(0, winWidth / 2, 0, winHeight), temp.points);
	drawPolyhedron(temp);

	// 单独的投影，规范化矩阵
	temp = cube;
	transformPoints(viewMatrix(viewP0, { 1, 1, 1 }, { -100, 200, -100 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionMatrix(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -75.f, -400.f), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(normalMatrix_perspective(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, -75.f, -400.f), temp.points);
	transformPoints(viewportMatrix(winWidth / 2, winWidth, 0, winHeight), temp.points);
	drawPolyhedron(temp);

	glFlush();
}

void code_10_exercise_6()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_7
Point viewP0 = { 160.f, 170.f, 200.f };
Vec3 view_u = { 0, 0, 1 };
Vec3 view_v = { 0, 1, 0 };
float znear = -40.f;
float zfar = -200.f;

Polyhedron cube = {
{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },
{{ 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};

auto _curCube = cube;
void normalKeyFcn(unsigned char key, int x, int y)
{
	//printf("normalKeyFcn %d, %d, %d\n", key, x, y);
	switch (key)
	{
	case 'a':
	case 'A':
		viewP0 = { 160.f, 170.f, 200.f };
		view_u = { 0, 0, 1 };
		view_v = { 0, 1, 0 };
		znear = -40.f;
		zfar = -200.f;
		break;
	case 's':
	case 'S':
		viewP0 = { 200.f, 100.f, 200.f };
		view_u = { 1, 0, 1 };
		view_v = { 0, 1, 0 };
		znear = -60.f;
		zfar = -300.f;
		break;
	case 'd':
	case 'D':
		viewP0 = { 200.f, 200.f, 200.f };
		view_u = { 1, 1, 1 };
		view_v = { -100, 200, -100 };
		znear = -75.f;
		zfar = -400.f;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void specialKeyFcn(int key, int x, int y)
{
	//printf("specialKeyFcn %d, %d, %d\n", key, x, y);
	switch (key)
	{
	case GLUT_KEY_LEFT:
		transformPoints(rotateMatrix({ 0, 0, 0 }, {0, 1, 0}, 5 * PI / 180), _curCube.points);
		break;
	case GLUT_KEY_RIGHT:
		transformPoints(rotateMatrix({ 0, 0, 0 }, { 0, 1, 0 }, -5 * PI / 180), _curCube.points);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	auto temp = _curCube;
	transformPoints(viewMatrix(viewP0, view_u, view_v) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionAndNormalMatrix(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2, znear, zfar), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(viewportMatrix(0, winWidth, 0, winHeight), temp.points);
	drawPolyhedron(temp);

	glFlush();
}

void code_10_exercise_7()
{
	glutDisplayFunc(displayFcn);
	glutKeyboardFunc(normalKeyFcn);
	glutSpecialFunc(specialKeyFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_8
Polyhedron cube = {
{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },
{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};
Point viewP0 = { 160.f, 170.f, 200.f };
void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	float xwmin = -300, xwmax = 100, ywmin = -winHeight / 4, ywmax = winHeight / 4;
	float znear = -40.f, zfar = -200.f;
	// 直接使用复合投影+规范化矩阵
	auto temp = cube;
	transformPoints(viewMatrix(viewP0, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionAndNormalMatrix(xwmin, xwmax, ywmin, ywmax, znear, zfar), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(viewportMatrix(0, winWidth / 2, winHeight / 2, winHeight), temp.points);
	drawPolyhedron(temp);

	// 单独的投影，规范化矩阵
	temp = cube;
	transformPoints(viewMatrix(viewP0, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(perspectiveProjectionMatrix(xwmin, xwmax, ywmin, ywmax, znear, zfar), temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(normalMatrix_perspective(xwmin, xwmax, ywmin, ywmax, znear, zfar), temp.points);
	transformPoints(viewportMatrix(winWidth / 2, winWidth, winHeight / 2, winHeight), temp.points);
	drawPolyhedron(temp);

	// 用对称透视投影(P294(10.33))+平移来代替斜透视投影(P294(10.34))，后续按书上的方法计算
	temp = cube;
	// P294(10.33)
	Matrix M_pers(4, 4);
	matrixSetIdentity(M_pers);
	M_pers[0][0] = -znear;
	M_pers[1][1] = -znear;
	M_pers[1][2] = (ywmax + ywmin) / 2;
	M_pers[2][2] = (znear + zfar) / (znear - zfar);
	M_pers[2][3] = -2 * znear * zfar / (znear - zfar);
	M_pers[3][2] = -1;
	M_pers[3][3] = 0;

	transformPoints(viewMatrix(viewP0, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(scaleMatrix({0, 0, 0}, 2 / (xwmax - xwmin), 2 / (ywmax - ywmin), 1) * 
		translateMatrix(-(xwmin + xwmax) / 2, -(ywmin + ywmax) / 2, 0) * 
		M_pers, temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(viewportMatrix(0, winWidth / 2, 0, winHeight / 2), temp.points);
	drawPolyhedron(temp);

	// 用单独的对称透视投影+平移来代替单独的斜透视投影，然后再规范化矩阵
	temp = cube;
	// 单独的对称透视投影
	Matrix M_perspective(4, 4);
	matrixSetIdentity(M_perspective);
	M_perspective[0][0] = -znear;
	M_perspective[0][2] = 0;
	M_perspective[1][1] = -znear;
	M_perspective[1][2] = 0;
	M_perspective[2][2] = -(znear + zfar);
	M_perspective[2][3] = znear * zfar;
	M_perspective[3][2] = -1;
	M_perspective[3][3] = 0;

	transformPoints(viewMatrix(viewP0, { 0, 0, 1 }, { 0, 1, 0 }) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 }), temp.points);
	transformPoints(translateMatrix(-(xwmin + xwmax) / 2, -(ywmin + ywmax) / 2, 0) *
		M_perspective, temp.points);
	for (auto& p : temp.points)
		descartes(p);
	transformPoints(normalMatrix_perspective(xwmin, xwmax, ywmin, ywmax, znear, zfar), temp.points);
	transformPoints(viewportMatrix(winWidth / 2, winWidth, 0, winHeight / 2), temp.points);
	drawPolyhedron(temp);

	glFlush();
}

void code_10_exercise_8()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_9

const GLint winLeftBitCode = 0x01;
const GLint winRightBitCode = 0x02;
const GLint winBottomBitCode = 0x04;
const GLint winTopBitCode = 0x08;
const GLint winNearBitCode = 0x10;
const GLint winFarBitCode = 0x20;
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
GLubyte encode(Point pt)
{
	GLubyte code = 0x00;
	if (pt.h > 0)
	{
		if (pt.h + pt.x < 0)
			code = code | winLeftBitCode;
		if (pt.h - pt.x < 0)
			code = code | winRightBitCode;
		if (pt.h + pt.y < 0)
			code = code | winBottomBitCode;
		if (pt.h - pt.y < 0)
			code = code | winTopBitCode;
		if (pt.h + pt.z < 0)
			code = code | winNearBitCode;
		if (pt.h - pt.z < 0)
			code = code | winFarBitCode;
	}
	else
	{
		if (pt.h + pt.x > 0)
			code = code | winLeftBitCode;
		if (pt.h - pt.x > 0)
			code = code | winRightBitCode;
		if (pt.h + pt.y > 0)
			code = code | winBottomBitCode;
		if (pt.h - pt.y > 0)
			code = code | winTopBitCode;
		if (pt.h + pt.z > 0)
			code = code | winNearBitCode;
		if (pt.h - pt.z > 0)
			code = code | winFarBitCode;
	}
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
void clacCrossPoint(Point& p1, const Point& p2, GLubyte planeCode)
{
	float u = 0.f;
	switch (planeCode)
	{
	case winLeftBitCode:
		u = (p1.x + p1.h) / ((p1.x + p1.h) - (p2.x + p2.h));
		break;
	case winRightBitCode:
		u = (p1.x - p1.h) / ((p1.x - p1.h) - (p2.x - p2.h));
		break;
	case winBottomBitCode:
		u = (p1.y + p1.h) / ((p1.y + p1.h) - (p2.y + p2.h));
		break;
	case winTopBitCode:
		u = (p1.y - p1.h) / ((p1.y - p1.h) - (p2.y - p2.h));
		break;
	case winNearBitCode:
		u = (p1.z + p1.h) / ((p1.z + p1.h) - (p2.z + p2.h));
		break;
	case winFarBitCode:
		u = (p1.z - p1.h) / ((p1.z - p1.h) - (p2.z - p2.h));
		break;
	default:
		break;
	}

	p1.x += (p2.x - p1.x) * u;
	p1.y += (p2.y - p1.y) * u;
	p1.z += (p2.z - p1.z) * u;
	p1.h += (p2.h - p1.h) * u;
}
bool lineClipCohSuth1(Point& p1, Point& p2)
{
	GLubyte code1, code2;
	GLint done = false, plotLine = false;
	while (!done)
	{
		code1 = encode(p1);
		code2 = encode(p2);
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
				clacCrossPoint(p1, p2, winLeftBitCode);
			}
			else if (code1 & winRightBitCode)
			{
				clacCrossPoint(p1, p2, winRightBitCode);
			}
			else if (code1 & winBottomBitCode)
			{
				clacCrossPoint(p1, p2, winBottomBitCode);
			}
			else if (code1 & winTopBitCode)
			{
				clacCrossPoint(p1, p2, winTopBitCode);
			}
			else if (code1 & winNearBitCode)
			{
				clacCrossPoint(p1, p2, winNearBitCode);
			}
			else if (code1 & winFarBitCode)
			{
				clacCrossPoint(p1, p2, winFarBitCode);
			}
		}
	}

	return plotLine;
}


GLubyte encode2(Point pt)
{
	GLubyte code = 0x00;
	if (pt.x < -1)
		code = code | winLeftBitCode;
	if (pt.x > 1)
		code = code | winRightBitCode;
	if (pt.y < -1)
		code = code | winBottomBitCode;
	if (pt.y > 1)
		code = code | winTopBitCode;
	if (pt.z < -1)
		code = code | winNearBitCode;
	if (pt.z > 1)
		code = code | winFarBitCode;
	return (code);
}
void clacCrossPoint2(Point& p1, const Point& p2, GLubyte planeCode)
{
	float u = 0.f;
	switch (planeCode)
	{
	case winLeftBitCode:
		u = (p1.x + 1) / (p1.x - p2.x);
		break;
	case winRightBitCode:
		u = (p1.x - 1) / (p1.x - p2.x);
		break;
	case winBottomBitCode:
		u = (p1.y + 1) / (p1.y - p2.y);
		break;
	case winTopBitCode:
		u = (p1.y - 1) / (p1.y - p2.y);
		break;
	case winNearBitCode:
		u = (p1.z + 1) / (p1.z - p2.z);
		break;
	case winFarBitCode:
		u = (p1.z - 1) / (p1.z - p2.z);
		break;
	default:
		break;
	}

	p1.x += (p2.x - p1.x) * u;
	p1.y += (p2.y - p1.y) * u;
	p1.z += (p2.z - p1.z) * u;
}
bool lineClipCohSuth2(Point& p1, Point& p2)
{
	GLubyte code1, code2;
	GLint done = false, plotLine = false;

	while (!done)
	{
		code1 = encode2(p1);
		code2 = encode2(p2);
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
				clacCrossPoint2(p1, p2, winLeftBitCode);
			}
			else if (code1 & winRightBitCode)
			{
				clacCrossPoint2(p1, p2, winRightBitCode);
			}
			else if (code1 & winBottomBitCode)
			{
				clacCrossPoint2(p1, p2, winBottomBitCode);
			}
			else if (code1 & winTopBitCode)
			{
				clacCrossPoint2(p1, p2, winTopBitCode);
			}
			else if (code1 & winNearBitCode)
			{
				clacCrossPoint2(p1, p2, winNearBitCode);
			}
			else if (code1 & winFarBitCode)
			{
				clacCrossPoint2(p1, p2, winFarBitCode);
			}
		}
	}

	return plotLine;
}
Camera *camera = nullptr;
Point p1 = { 80, -60, -60 }, p2 = { -35, 70, 40 };
void viewLine1()
{
	glColor3f(1.0, 1.0, 1.0);
	winWidth = 150, winHeight = 100;

	auto p1_temp = p1;
	auto p2_temp = p2;

	Point viewPortPoint = { 100.f, 100.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / 2 / winHeight * clipWinHeight;

	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -2000.f) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 });
			
	transformPoint(m, p1_temp);
	transformPoint(m, p2_temp);

	auto p1_temp_clip = p1_temp;
	auto p2_temp_clip = p2_temp;
	auto clip = lineClipCohSuth1(p1_temp_clip, p2_temp_clip);
	
	descartes(p1_temp);
	descartes(p2_temp);
	if (clip)
	{
		descartes(p1_temp_clip);
		descartes(p2_temp_clip);
	}

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);
	transformPoint(m1, p1_temp);
	transformPoint(m1, p2_temp);
	if (clip)
	{
		transformPoint(m1, p1_temp_clip);
		transformPoint(m1, p2_temp_clip);
	}
	drawLine(p1_temp, p2_temp);
	glColor3f(1.0, 0.0, 0.0);
	if (clip)
		drawLine(p1_temp_clip, p2_temp_clip);
}
void viewLine2()
{
	glColor3f(1.0, 1.0, 1.0);
	winWidth = 150, winHeight = 100;

	auto p1_temp = p1;
	auto p2_temp = p2;

	Point viewPortPoint = { 500, 100.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / 2 / winHeight * clipWinHeight;
	
	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -2000.f) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 });

	transformPoint(m, p1_temp);
	transformPoint(m, p2_temp);

	descartes(p1_temp);
	descartes(p2_temp);

	auto p1_temp_clip = p1_temp;
	auto p2_temp_clip = p2_temp;
	auto clip = lineClipCohSuth2(p1_temp_clip, p2_temp_clip);

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);

	transformPoint(m1, p1_temp);
	transformPoint(m1, p2_temp);
	if (clip)
	{
		transformPoint(m1, p1_temp_clip);
		transformPoint(m1, p2_temp_clip);
	}

	drawLine(p1_temp, p2_temp);
	glColor3f(1.0, 0.0, 0.0);
	if (clip)
		drawLine(p1_temp_clip, p2_temp_clip);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	viewLine1();
	viewLine2();
	
	glFlush();
}
void code_10_exercise_9()
{
	glutDisplayFunc(drawFunc);
	camera = new Camera({ 100.f, 100.f, 200.f }, { 0, 0, 1 }, { 0, 1, 0 }, 200);
}
#endif

#ifdef CHAPTER_10_EXERCISE_10

const GLint winLeftBitCode = 0x01;
const GLint winRightBitCode = 0x02;
const GLint winBottomBitCode = 0x04;
const GLint winTopBitCode = 0x08;
const GLint winNearBitCode = 0x10;
const GLint winFarBitCode = 0x20;
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
GLubyte encode(Point pt)
{
	GLubyte code = 0x00;
	if (pt.h > 0)
	{
		if (pt.h + pt.x < 0)
			code = code | winLeftBitCode;
		if (pt.h - pt.x < 0)
			code = code | winRightBitCode;
		if (pt.h + pt.y < 0)
			code = code | winBottomBitCode;
		if (pt.h - pt.y < 0)
			code = code | winTopBitCode;
		if (pt.h + pt.z < 0)
			code = code | winNearBitCode;
		if (pt.h - pt.z < 0)
			code = code | winFarBitCode;
	}
	else
	{
		if (pt.h + pt.x > 0)
			code = code | winLeftBitCode;
		if (pt.h - pt.x > 0)
			code = code | winRightBitCode;
		if (pt.h + pt.y > 0)
			code = code | winBottomBitCode;
		if (pt.h - pt.y > 0)
			code = code | winTopBitCode;
		if (pt.h + pt.z > 0)
			code = code | winNearBitCode;
		if (pt.h - pt.z > 0)
			code = code | winFarBitCode;
	}
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
void clacCrossPoint(Point& p1, const Point& p2, GLubyte planeCode)
{
	float u = 0.f;
	switch (planeCode)
	{
	case winLeftBitCode:
		u = (p1.x + p1.h) / ((p1.x + p1.h) - (p2.x + p2.h));
		break;
	case winRightBitCode:
		u = (p1.x - p1.h) / ((p1.x - p1.h) - (p2.x - p2.h));
		break;
	case winBottomBitCode:
		u = (p1.y + p1.h) / ((p1.y + p1.h) - (p2.y + p2.h));
		break;
	case winTopBitCode:
		u = (p1.y - p1.h) / ((p1.y - p1.h) - (p2.y - p2.h));
		break;
	case winNearBitCode:
		u = (p1.z + p1.h) / ((p1.z + p1.h) - (p2.z + p2.h));
		break;
	case winFarBitCode:
		u = (p1.z - p1.h) / ((p1.z - p1.h) - (p2.z - p2.h));
		break;
	default:
		break;
	}

	p1.x += (p2.x - p1.x) * u;
	p1.y += (p2.y - p1.y) * u;
	p1.z += (p2.z - p1.z) * u;
	p1.h += (p2.h - p1.h) * u;
}
bool lineClipCohSuth1(Point& p1, Point& p2)
{
	GLubyte code1, code2;
	GLint done = false, plotLine = false;
	while (!done)
	{
		code1 = encode(p1);
		code2 = encode(p2);
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
				clacCrossPoint(p1, p2, winLeftBitCode);
			}
			else if (code1 & winRightBitCode)
			{
				clacCrossPoint(p1, p2, winRightBitCode);
			}
			else if (code1 & winBottomBitCode)
			{
				clacCrossPoint(p1, p2, winBottomBitCode);
			}
			else if (code1 & winTopBitCode)
			{
				clacCrossPoint(p1, p2, winTopBitCode);
			}
			else if (code1 & winNearBitCode)
			{
				clacCrossPoint(p1, p2, winNearBitCode);
			}
			else if (code1 & winFarBitCode)
			{
				clacCrossPoint(p1, p2, winFarBitCode);
			}
		}
	}

	return plotLine;
}
Camera *camera = nullptr;
struct Line
{
	Point p1, p2;
};
Point p1 = { 80, -60, -60 }, p2 = { -35, 70, 40 };
std::vector<Line> lines;
float random(int a, int b)
{
	if (a > b)
	{
		int temp = a;
		a = b;
		b = temp;
	}
	return (rand() % (b - a + 1)) + a;
}
void genLines()
{
	Point p0 = { -250, -250, -600 };
	float xlen0 = 500, ylen0 = 500, zlen0 = 500;

	Point p1 = { -500, -500, -600 };
	float xlen1 = 1000, ylen1 = 1000, zlen1 = 1000;

	for (int i = 0; i < 10; i++)
	{
		lines.push_back({ { random(p0.x, p0.x + xlen0),
			random(p0.y, p0.y + ylen0),
			random(p0.z, p0.z - zlen0) },
			{ random(p1.x, p1.x + xlen1),
			random(p1.y, p1.y + ylen1),
			random(p1.z, p1.z - zlen1) } });
	}
}
void viewLine()
{
	glColor3f(1.0, 1.0, 1.0);
	winWidth = 400, winHeight = 400;

	auto tempLines = lines;

	Point viewPortPoint = { 200.f, 100.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / winHeight * clipWinHeight;

	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -1200.f) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v);

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);

	for (auto& line : tempLines)
	{
		transformPoint(m, line.p1);		
		transformPoint(m, line.p2);

		auto p1_clip = line.p1;
		auto p2_clip = line.p2;

		auto clip = lineClipCohSuth1(p1_clip, p2_clip);

		descartes(line.p1);
		descartes(line.p2);
		if (clip)
		{
			descartes(p1_clip);
			descartes(p2_clip);
		}

		transformPoint(m1, line.p1);
		transformPoint(m1, line.p2);
		if (clip)
		{
			transformPoint(m1, p1_clip);
			transformPoint(m1, p2_clip);
		}
		glColor3f(1.0, 1.0, 1.0);
		drawLine(line.p1, line.p2);
		glColor3f(1.0, 0.0, 0.0);
		if (clip)
			drawLine(p1_clip, p2_clip);
	}	
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	viewLine();

	glFlush();
}
void code_10_exercise_10()
{
	genLines();
	glutDisplayFunc(drawFunc);
	camera = new Camera({ 0, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 }, 500);
}
#endif

#ifdef CHAPTER_10_EXERCISE_11
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
bool lineClipLiangBarsk(Point& p1, Point& p2)
{
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy, dz;
	if (clipTest(-dx, p1.x - (-1), &u1, &u2))
		if (clipTest(dx, 1 - p1.x, &u1, &u2))
		{
			dy = p2.y - p1.y;
			if (clipTest(-dy, p1.y - (-1), &u1, &u2))
				if (clipTest(dy, 1 - p1.y, &u1, &u2))
				{
					dz = p2.z - p1.z;
					if (clipTest(-dz, p1.z - (-1), &u1, &u2))
						if (clipTest(dz, 1 - p1.z, &u1, &u2))
						{
							if (u2 < 1.0)
							{
								p2.x = p1.x + u2 * dx;
								p2.y = p1.y + u2 * dy;
								p2.z = p1.z + u2 * dz;
							}
							if (u1 > 0.0)
							{
								p1.x = p1.x + u1 * dx;
								p1.y = p1.y + u1 * dy;
								p1.z = p1.z + u1 * dz;
							}
							//lineBres(Round(p1.getx()), Round(p1.gety()), Round(p2.getx()), Round(p2.gety())); // 精确到浮点数绘图
							//drawLine(p1, p2);
							return true;
						}
				}
		}
	return false;
}
Camera *camera = nullptr;
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Point p1 = { 80, -60, -60 }, p2 = { -35, 70, 40 };

	glColor3f(1.0, 1.0, 1.0);

	winWidth = 150, winHeight = 100;

	Point viewPortPoint = { 500.f, 100.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });


	auto p1_temp = p1;
	auto p2_temp = p2;

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / 2 / winHeight * clipWinHeight;

	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -2000.f) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v) *
		modelMatrix({ -100.f, -100.f, -100.f }, { 1, 0, 0 }, { 0, 1, 0 });

	transformPoint(m, p1_temp);
	transformPoint(m, p2_temp);

	descartes(p1_temp);
	descartes(p2_temp);

	auto p1_temp_clip = p1_temp;
	auto p2_temp_clip = p2_temp;
	auto clip = lineClipLiangBarsk(p1_temp_clip, p2_temp_clip);

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);

	transformPoint(m1, p1_temp);
	transformPoint(m1, p2_temp);
	if (clip)
	{
		transformPoint(m1, p1_temp_clip);
		transformPoint(m1, p2_temp_clip);
	}

	drawLine(p1_temp, p2_temp);
	glColor3f(1.0, 0.0, 0.0);
	if (clip)
		drawLine(p1_temp_clip, p2_temp_clip);

	glFlush();
}
void code_10_exercise_11()
{
	glutDisplayFunc(drawFunc);

	camera = new Camera({ 100, 100, 200 }, { 0, 0, 1 }, { 0, 1, 0 }, 200);
}
#endif

#ifdef CHAPTER_10_EXERCISE_12
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
bool lineClipLiangBarsk(Point& p1, Point& p2)
{
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy, dz;
	if (clipTest(-dx, p1.x - (-1), &u1, &u2))
		if (clipTest(dx, 1 - p1.x, &u1, &u2))
		{
			dy = p2.y - p1.y;
			if (clipTest(-dy, p1.y - (-1), &u1, &u2))
				if (clipTest(dy, 1 - p1.y, &u1, &u2))
				{
					dz = p2.z - p1.z;
					if (clipTest(-dz, p1.z - (-1), &u1, &u2))
						if (clipTest(dz, 1 - p1.z, &u1, &u2))
						{
							if (u2 < 1.0)
							{
								p2.x = p1.x + u2 * dx;
								p2.y = p1.y + u2 * dy;
								p2.z = p1.z + u2 * dz;
							}
							if (u1 > 0.0)
							{
								p1.x = p1.x + u1 * dx;
								p1.y = p1.y + u1 * dy;
								p1.z = p1.z + u1 * dz;
							}
							//lineBres(Round(p1.getx()), Round(p1.gety()), Round(p2.getx()), Round(p2.gety())); // 精确到浮点数绘图
							//drawLine(p1, p2);
							return true;
						}
				}
		}
	return false;
}
Camera *camera = nullptr;
struct Line
{
	Point p1, p2;
};
std::vector<Line> lines;
float random(int a, int b)
{
	if (a > b)
	{
		int temp = a;
		a = b;
		b = temp;
	}
	return (rand() % (b - a + 1)) + a;
}
void genLines()
{
	Point p0 = { -250, -250, -600 };
	float xlen0 = 500, ylen0 = 500, zlen0 = 500;

	Point p1 = { -500, -500, -600 };
	float xlen1 = 1000, ylen1 = 1000, zlen1 = 1000;

	for (int i = 0; i < 10; i++)
	{
		lines.push_back({ { random(p0.x, p0.x + xlen0),
			random(p0.y, p0.y + ylen0),
			random(p0.z, p0.z - zlen0) },
			{ random(p1.x, p1.x + xlen1),
			random(p1.y, p1.y + ylen1),
			random(p1.z, p1.z - zlen1) } });
	}
}
void viewLine()
{
	glColor3f(1.0, 1.0, 1.0);
	winWidth = 400, winHeight = 400;

	auto tempLines = lines;

	Point viewPortPoint = { 200.f, 100.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / winHeight * clipWinHeight;

	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -1200.f) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v);

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);

	for (auto& line : tempLines)
	{
		transformPoint(m, line.p1);
		transformPoint(m, line.p2);

		descartes(line.p1);
		descartes(line.p2);

		auto p1_clip = line.p1;
		auto p2_clip = line.p2;
		auto clip = lineClipLiangBarsk(p1_clip, p2_clip);

		transformPoint(m1, line.p1);
		transformPoint(m1, line.p2);
		if (clip)
		{
			transformPoint(m1, p1_clip);
			transformPoint(m1, p2_clip);
		}
		glColor3f(1.0, 1.0, 1.0);
		drawLine(line.p1, line.p2);
		glColor3f(1.0, 0.0, 0.0);
		if (clip)
			drawLine(p1_clip, p2_clip);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	viewLine();

	glFlush();
}
void code_10_exercise_12()
{
	genLines();
	glutDisplayFunc(drawFunc);
	camera = new Camera({ 0, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 }, 500);
}
#endif

#ifdef CHAPTER_10_EXERCISE_13
Camera *camera = nullptr;
Polyhedron cube = {
	{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
	{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },
	{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};
void view()
{
	glColor3f(1.0, 1.0, 1.0);
	winWidth = 400, winHeight = 300;

	auto temp = cube;

	Point viewPortPoint = { 200.f, 150.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / winHeight * clipWinHeight;

	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -1200.f) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v);

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);


	transformPoints(m, temp.points);
	for (auto& p : temp.points)
		descartes(p);

	auto temp_clip = temp;
	clip(temp_clip);

	transformPoints(m1, temp.points);
	if (temp_clip.points.size() > 0)
		transformPoints(m1, temp_clip.points);

	glColor3f(1.0, 1.0, 1.0);
	drawPolyhedron(temp);
	glColor3f(1.0, 0.0, 0.0);
	if (temp_clip.points.size() > 0)
		drawPolyhedron(temp_clip);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	view();

	glFlush();
}
void code_10_exercise_13()
{
	camera = new Camera({ 100, 200, 500 }, { 0, 0, 1 }, { 0, 1, 0 }, 1000);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_10_EXERCISE_13_TEST
enum PointType
{
	None,
	Normal,
	CrossEnter,
	CrossExit,
};
struct PointInfo
{
	Point point;
	PointType type;
};
struct PolyhedronInfo
{
	Polyhedron polyhedron;
	map<Point, int> pointsInfo;
};
Camera *camera = nullptr;
const int LeftClipPlane = 1;
const int RightClipPlane = 2;
const int BottomClipPlane = 3;
const int TopClipPlane = 4;
const int NearClipPlane = 5;
const int FarClipPlane = 6;
const std::vector<int> clipPlanes = { LeftClipPlane, RightClipPlane, BottomClipPlane, TopClipPlane, NearClipPlane, FarClipPlane };

bool linesPoint(Point line1Begin, Point line1End, Point line2Begin, Point line2End, float& u1, float&u2)
{
	Vec3 line1Vec = { line1End.x - line1Begin.x, line1End.y - line1Begin.y, line1End.z - line1Begin.z };
	Vec3 line2Vec = { line2End.x - line2Begin.x, line2End.y - line2Begin.y, line2End.z - line2Begin.z };
	do {
		bool recorded = false;
		float record = 0.f;
		if (!Equal(line1Vec.x, 0) && !Equal(line2Vec.x, 0))
		{
			record = line1Vec.x / line2Vec.x;
			recorded = true;
		}
		else if(Equal(line1Vec.x, 0) && Equal(line2Vec.x, 0))
		{

		}
		else
		{
			break;
		}

		if (!Equal(line1Vec.y, 0) && !Equal(line2Vec.y, 0))
		{
			float r = line1Vec.y / line2Vec.y;
			if (recorded)
			{
				if (record != r)
					break;
			}
			else
			{
				record = r;
				recorded = true;
			}
		}
		else if (Equal(line1Vec.y, 0) && Equal(line2Vec.y, 0))
		{

		}
		else
		{
			break;
		}

		if (!Equal(line1Vec.z, 0) && !Equal(line2Vec.z, 0))
		{
			float r = line1Vec.z / line2Vec.z;
			if (recorded)
			{
				if (record != r)
					break;
			}
			else
			{
				record = r;
				recorded = true;
			}
		}
		else if (Equal(line1Vec.z, 0) && Equal(line2Vec.z, 0))
		{

		}
		else
		{
			break;
		}

		return false;
	} while (0);

	float f = line1Vec.x * line2Vec.y - line1Vec.y * line2Vec.x;
	if (!Equal(f, 0))
	{
		u2 = (line1Vec.y * (line2Begin.x - line1Begin.x) - line1Vec.x * (line2Begin.y - line1Begin.y)) / f;
	}
	else
	{
		float f = line1Vec.x * line2Vec.z - line1Vec.z * line2Vec.x;
		if (!Equal(f, 0))
		{
			u2 = (line1Vec.z * (line2Begin.x - line1Begin.x) - line1Vec.x * (line2Begin.z - line1Begin.z)) / f;
		}
		else
		{
			float f = line1Vec.y * line2Vec.z - line1Vec.z * line2Vec.y;
			if (!Equal(f, 0))
			{
				u2 = (line1Vec.z * (line2Begin.y - line1Begin.y) - line1Vec.y * (line2Begin.z - line1Begin.z)) / f;
			}
			else
			{
				assert(0);
			}
		}
	}

	if (!Equal(line1Vec.x, 0))
	{
		u1 = (line2Vec.x * u2 + line2Begin.x - line1Begin.x) / line1Vec.x;
	}
	else if (!Equal(line1Vec.y, 0))
	{
		u1 = (line2Vec.y * u2 + line2Begin.y - line1Begin.y) / line1Vec.y;
	}
	else if (!Equal(line1Vec.z, 0))
	{
		u1 = (line2Vec.z * u2 + line2Begin.z - line1Begin.z) / line1Vec.z;
	}
	else
	{
		assert(0);
	}

	return true;
}

void _cutPolygon(const vector<Point>& points, Vec3 normalVec, vector<vector<Point>>& ret)
{
	vector<Vec3> edges;
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 < points.size() ? i + 1 : 0;
		edges.push_back({ points[next].x - points[i].x,
			points[next].y - points[i].y,
			points[next].z - points[i].z,});
	}

	int cutIndex = -1;
	for (int i = 0; i < edges.size(); ++i)
	{
		int next = i + 1 < edges.size() ? i + 1 : 0;
		Vec3 curVec = cross(edges[i], edges[next]);
		if (i > 0)
		{
			if (curVec.x * normalVec.x < 0 || curVec.y * normalVec.y < 0 || curVec.z * normalVec.z < 0)
			{
				cutIndex = i;
				break;
			}
		}
	}
	if (cutIndex == -1)
	{
		ret.push_back(points);
		return;
	}

	Point cutStartP = points[cutIndex];
	Point cutEndP = points[cutIndex + 1 < points.size() ? cutIndex + 1 : 0];
	float A1 = cutEndP.y - cutStartP.y;
	float B1 = cutStartP.x - cutEndP.x;
	float C1 = cutEndP.x * cutStartP.y - cutStartP.x * cutEndP.y;

	Point cutPoint;
	float cutU1 = 0.f;
	float cutU2 = 0.f;
	int curIndex = 0;
	for (int i = 0; i < points.size(); i++)
	{
		int j = i + 1 < points.size() ? i + 1 : 0;
		Point point;
		float u1 = 0.f, u2 = 0.f;
		if (linesPoint(cutStartP, cutEndP, points[i], points[j], u1, u2))
		{
			if (u1 > 1.f && 0.f <= u2 && u2 <= 1.f)
			{
				Vec3 v = cross({ cutEndP.x - cutStartP.x, cutEndP.y - cutStartP.y, cutEndP.z - cutStartP.z },
				{ points[j].x - points[i].x, points[j].y - points[i].y, points[j].z - points[i].z });

				if (v.x * normalVec.x >= 0 && v.y * normalVec.y >= 0 && v.z * normalVec.z >= 0)
				{
					if (u1 < cutU1 || cutU1 == 0.f)
					{
						cutPoint = { cutStartP.x + u1 * (cutEndP.x  - cutStartP.x ),
							cutStartP.y + u1 * (cutEndP.y - cutStartP.y),
							cutStartP.z + u1 * (cutEndP.z - cutStartP.z) };
						curIndex = i;

						cutU1 = u1;
						cutU2 = u2;
					}
				}
			}
		}
	}
	vector<Point> newPloygon = points;
	if (0.f < cutU2 && cutU2 < 1.f)
	{
		newPloygon.insert(newPloygon.begin() + curIndex + 1, cutPoint);
	}

	vector<Point> newPloygon1;
	vector<Point> newPloygon2;

	bool f = false;
	for (int i = 0; i < newPloygon.size(); i++)
	{
		if (newPloygon[i] == cutPoint || newPloygon[i] == cutEndP)
		{
			if (!f)
			{
				newPloygon1.push_back(newPloygon[i]);
			}
			else
			{
				newPloygon2.push_back(newPloygon[i]);
			}
			f = !f;
		}

		if (!f)
		{
			newPloygon1.push_back(newPloygon[i]);
		}
		else
		{
			newPloygon2.push_back(newPloygon[i]);
		}
	}
	_cutPolygon(newPloygon1, normalVec, ret);
	_cutPolygon(newPloygon2, normalVec, ret);
}
vector<vector<Point>> cutPolygon(const vector<Point>& points, Vec3 normalVec)
{
	vector<vector<Point>> ret;
	_cutPolygon(points, normalVec, ret);
	return ret;
}
map<Point, Point> orderCorssLine(vector<PointInfo>& crossPoints)
{
	map<Point, Point> ret;

	if (crossPoints.size() > 2)
	{
		assert(crossPoints.size() % 2 == 0);

		if (!Equal(crossPoints[0].point.x, crossPoints[1].point.x) || !Equal(crossPoints[1].point.x, crossPoints[2].point.x))
		{
			std::sort(crossPoints.begin(), crossPoints.end(), [](auto& a, auto& b)
			{
				return a.point.x < b.point.x;
			});
		}
		else if (!Equal(crossPoints[0].point.y, crossPoints[1].point.y) || !Equal(crossPoints[1].point.y, crossPoints[2].point.y))
		{
			std::sort(crossPoints.begin(), crossPoints.end(), [](auto& a, auto& b)
			{
				return a.point.y < b.point.y;
			});
		}
		else if (!Equal(crossPoints[0].point.z, crossPoints[1].point.z) || !Equal(crossPoints[1].point.z, crossPoints[2].point.z))
		{
			std::sort(crossPoints.begin(), crossPoints.end(), [](auto& a, auto& b)
			{
				return a.point.z < b.point.z;
			});
		}
		else
		{
			assert(0);
		}
	}

	if (crossPoints.size() >= 2)
	{
		for (int i = 0; i < crossPoints.size(); i = i + 2)
		{
			int next = i + 1;
			if (crossPoints[i].type == CrossExit)
			{
				assert(ret.find(crossPoints[i].point) == ret.end());
				ret[crossPoints[i].point] = crossPoints[next].point;
			}
			else if (crossPoints[next].type == CrossExit)
			{
				assert(ret.find(crossPoints[next].point) == ret.end());
				ret[crossPoints[next].point] = crossPoints[i].point;
			}
			else
			{
				assert(0);
			}
		}
	}	

	return ret;
}
void addCrossInfo(Point p1, Point p2, map<Point, map<Point, int>>& crossInfo)
{
	//auto it1 = crossInfo.find(p1);
	//if (it1 != crossInfo.end())
	//{
	//	it1->second[p2] = 1;
	//}
	//else
	//{
	//	crossInfo[p1][p2] = 1;
	//}

	//if (crossInfo.find(p1) != crossInfo.end() && 
	//	crossInfo[p1].find(p2) != crossInfo[p1].end())
	//{
	//	printf("%f, %f, %f\n", p1.x, p1.y, p1.z);
	//	printf("%f, %f, %f\n", p2.x, p2.y, p2.z);
	//	printf("\n");
	//}
	crossInfo[p1][p2] = 1;
}
bool checkSamePoint(vector<Point>& points)
{
	map<Point, int> testSame;
	for (auto it = points.begin(); it != points.end();)
	{
		if (testSame.find(*it) == testSame.end())
		{
			testSame[*it] = 1;
			it++;
		}
		else
		{
			it = points.erase(it);
		}
	}

	return points.size() >= 3;
}
void addSurface(PolyhedronInfo& polyhedronInfo, vector<Point>& points)
{
	polyhedronInfo.polyhedron.indexs.push_back({});
	for (int i = 0; i < points.size(); i++)
	{
		auto it = polyhedronInfo.pointsInfo.find(points[i]);
		if (it != polyhedronInfo.pointsInfo.end())
		{
			polyhedronInfo.polyhedron.indexs.back().push_back(it->second);
		}
		else
		{
			polyhedronInfo.polyhedron.points.push_back(points[i]);
			int idx = polyhedronInfo.polyhedron.points.size() - 1;
			polyhedronInfo.polyhedron.indexs.back().push_back(idx);

			polyhedronInfo.pointsInfo[points[i]] = idx;
		}
	}
}
void addPolyhedron(PolyhedronInfo& mainPolyhedronInfo, PolyhedronInfo& otherPolyhedronInfo)
{
	for (auto& pointIdxs : otherPolyhedronInfo.polyhedron.indexs)
	{
		vector<Point> points;
		for (auto& pointIdx : pointIdxs)
		{
			points.push_back(otherPolyhedronInfo.polyhedron.points[pointIdx]);
		}
		addSurface(mainPolyhedronInfo, points);
	}
}
void addSurface(vector<PolyhedronInfo>& polyhedrons, vector<Point>& points)
{
	if (checkSamePoint(points))
	{
		PolyhedronInfo* mainPolyhedronInfo = nullptr;
		for (auto it = polyhedrons.begin(); it != polyhedrons.end();)
		{
			bool itAdded = false;
			for (auto& p : points)
			{
				if (it->pointsInfo.find(p) != it->pointsInfo.end())
				{
					if (!mainPolyhedronInfo)
					{
						addSurface(*it, points);
						mainPolyhedronInfo = &(*it);
					}
					else
					{
						addPolyhedron(*mainPolyhedronInfo, *it);
						it = polyhedrons.erase(it);
						itAdded = true;
					}
					break;
				}
			}
			if (!itAdded)
				it++;
		}
		if (!mainPolyhedronInfo)
		{
			PolyhedronInfo polyhedronInfo;
			polyhedronInfo.polyhedron.points = points;
			polyhedronInfo.polyhedron.indexs.push_back({});
			for (int i = 0; i < points.size(); i++)
			{
				polyhedronInfo.polyhedron.indexs.back().push_back(i);
				polyhedronInfo.pointsInfo[points[i]] = i;
			}
			polyhedrons.push_back(polyhedronInfo);
		}
	}
}
void _createClipSuface(map<Point, map<Point, int>>& crossInfo, vector<vector<Point>>& ret)
{
	if (crossInfo.size() < 3)
		return;

	auto beginPoint = crossInfo.begin()->first;
	ret.push_back({});
	ret.back().push_back(beginPoint);

	auto curPoint = beginPoint;
	while (true)
	{
		//assert(crossInfo.find(curPoint) != crossInfo.end());
		if (crossInfo.find(curPoint) == crossInfo.end())
		{
			ret.clear();
			return;
		}
		auto nextPoint = crossInfo[curPoint].begin()->first;
		assert(crossInfo[curPoint][nextPoint] == 1);

		//assert(crossInfo.find(nextPoint) != crossInfo.end());
		if (crossInfo.find(nextPoint) == crossInfo.end())
		{
			ret.clear();
			return;
		}
		if (crossInfo[nextPoint].find(curPoint) == crossInfo[nextPoint].end())
		{
			ret.clear();
			return;
		}
		assert(crossInfo[nextPoint][curPoint] == 1);

		crossInfo[curPoint].erase(nextPoint);
		if (crossInfo[curPoint].size() == 0)
		{
			crossInfo.erase(curPoint);
		}

		crossInfo[nextPoint].erase(curPoint);
		if (crossInfo[nextPoint].size() == 0)
		{
			crossInfo.erase(nextPoint);
		}

		if (nextPoint != beginPoint)
			ret.back().push_back(nextPoint);
		else
			return _createClipSuface(crossInfo, ret);
		curPoint = nextPoint;
	}
}
vector<vector<Point>> createClipSuface(map<Point, map<Point, int>>& crossInfo)
{
	vector<vector<Point>> ret;
	_createClipSuface(crossInfo, ret);
	return ret;
}

void antiClockPoints(vector<Point>& points, Vec3 normalVec)
{
	Vec3 a = { points[1].x - points[0].x, points[1].y - points[0].y, -(points[1].z - points[0].z) }; // z取负值是因为在左手坐标系和右手坐标系z反号
	Vec3 b = { points[2].x - points[1].x, points[2].y - points[1].y, -(points[2].z - points[1].z) };
	Vec3 crossVec = cross(a, b);
	bool antiClocked = crossVec.x * normalVec.x >= 0 && crossVec.y * normalVec.y >= 0 && crossVec.z * normalVec.z >= 0;
	if (!antiClocked)
	{
		std::reverse(points.begin(), points.end());
	}
}
Vec3 getClipPlaneNormalVec(int clipPlane)
{
	switch (clipPlane)
	{
	case LeftClipPlane:
		return{ -1, 0, 0 };
	case RightClipPlane:
		return{ 1, 0, 0 };
	case BottomClipPlane:
		return{ 0, -1, 0 };
	case TopClipPlane:
		return{ 0, 1, 0 };
	case NearClipPlane:
		return{ 0, 0, -1 };
	case FarClipPlane:
		return{ 0, 0, 1 };
	}
}

bool inside(Point p, int clipPlane)
{
	switch (clipPlane)
	{
	case LeftClipPlane:
		return p.x > -1;
	case RightClipPlane:
		return p.x < 1;
	case BottomClipPlane:
		return p.y > -1;
	case TopClipPlane:
		return p.y < 1;
	case NearClipPlane:
		return p.z > -1;
	case FarClipPlane:
		return p.z < 1;
	}
}
Point clacCrossPoint(Point p1, Point p2, int clipPlane)
{
	float u = 0.f;
	switch (clipPlane)
	{
	case LeftClipPlane:
		u = (p1.x + 1) / (p1.x - p2.x);
		break;
	case RightClipPlane:
		u = (p1.x - 1) / (p1.x - p2.x);
		break;
	case BottomClipPlane:
		u = (p1.y + 1) / (p1.y - p2.y);
		break;
	case TopClipPlane:
		u = (p1.y - 1) / (p1.y - p2.y);
		break;
	case NearClipPlane:
		u = (p1.z + 1) / (p1.z - p2.z);
		break;
	case FarClipPlane:
		u = (p1.z - 1) / (p1.z - p2.z);
		break;
	default:
		break;
	}

	return{ p1.x + (p2.x - p1.x) * u,
		p1.y += (p2.y - p1.y) * u ,
		p1.z += (p2.z - p1.z) * u };
}
//vector<vector<Point>> clipSurface(const vector<Point>& points, int clipPlane, map<Point, map<Point, int>>& crossInfo)
//{
//	vector<vector<Point>> ret;
//
//	assert(points.size() >= 3);
//	Vec3 normalVec = cross({ points[1].x - points[0].x, points[1].y - points[0].y, points[1].z - points[0].z },
//	{ points[2].x - points[1].x, points[2].y - points[1].y, points[2].z - points[1].z });
//	vector<vector<Point>> convexPolygons = cutPolygon(points, normalVec);
//	
//	vector<Point> cross;
//	for (int i = 0; i < points.size(); i++)
//	{
//		int next = i + 1 < points.size() ? i + 1 : 0;
//
//		if (!inside(points[i], clipPlane))
//		{
//			if (inside(points[next], clipPlane))
//			{
//				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
//				ret.push_back(cp);
//				ret.push_back(points[next]);
//
//				cross.push_back(cp);
//			}
//		}
//		else
//		{
//			if (inside(points[next], clipPlane))
//			{
//				ret.push_back(points[next]);
//			}
//			else
//			{
//				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
//				ret.push_back(cp);
//				cross.push_back(cp);
//			}
//		}
//	}
//	if (cross.size() > 1 && cross[0] != cross[1])
//	{
//		addCrossInfo(cross[0], cross[1], crossInfo);
//		addCrossInfo(cross[1], cross[0], crossInfo);
//	}
//
//	return ret;
//}
int findPointIdx(vector<PointInfo>& clipPoints, Point p, PointType type)
{
	for (int i = 0; i < clipPoints.size(); i++)
	{
		if (p == clipPoints[i].point && clipPoints[i].type == type)
		{
			return i;
		}
	}
}
void createClipedSurface(vector<PointInfo>& clipPoints, map<Point, Point>& orderLines, vector<vector<Point>>& out)
{
	if (clipPoints.size() < 3)
	{
		return;
	}

	PointInfo begin = clipPoints[0];
	vector<int> idxs;
	int i = -1;
	idxs.push_back(0);
	if (begin.type == CrossExit)
	{
		auto it = orderLines.find(begin.point);
		assert(it != orderLines.end());
		auto p = it->second;
		i = findPointIdx(clipPoints, p, CrossEnter);
	}
	else
	{
		i = 1;
	}
	while (true)
	{
		bool findprv = false;
		for (auto& idx : idxs)
		{
			if (idx == i)
			{
				findprv = true;
				break;
			}
		}
		if(findprv)
		{
			if (idxs.size() >= 3)
				out.push_back({});

			int offset = 0;
			for (auto idx : idxs)
			{
				auto it = clipPoints.begin() + (idx + offset);
				
				if (idxs.size() >= 3)
				{
					out.back().push_back(it->point);
				}

				clipPoints.erase(it);
				offset -= 1;
			}

			createClipedSurface(clipPoints, orderLines, out);
			return;
		}

		idxs.push_back(i);

		if (clipPoints[i].type == CrossExit)
		{
			auto it = orderLines.find(clipPoints[i].point);
			assert(it != orderLines.end());
			auto p = it->second;
			i = findPointIdx(clipPoints, p, CrossEnter);
		}
		else
		{
			i = i + 1 < clipPoints.size() ? i + 1 : 0;
		}
	}
}
vector<vector<Point>> clipSurface(const vector<Point>& points, int clipPlane, map<Point, map<Point, int>>& crossInfo)
{
	vector<vector<Point>> ret;

	if (points.size() < 3)
	{
		printf("%d\n", points.size());
		return ret;
	}
	//assert(points.size() >= 3);
	
	vector<PointInfo> clipPoints;
	vector<PointInfo> crossPoints;

	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 < points.size() ? i + 1 : 0;

		if (!inside(points[i], clipPlane))
		{
			if (inside(points[next], clipPlane))
			{
				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
				clipPoints.push_back({ cp, CrossEnter });
				clipPoints.push_back({ points[next], Normal });

				crossPoints.push_back({ cp, CrossEnter });
			}
		}
		else
		{
			if (inside(points[next], clipPlane))
			{
				clipPoints.push_back({ points[next], Normal });
			}
			else
			{
				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
				clipPoints.push_back({ cp, CrossExit });

				crossPoints.push_back({ cp, CrossExit });
			}
		}
	}	

	auto orderLines = orderCorssLine(crossPoints);
	for (auto it = orderLines.begin(); it != orderLines.end(); it++)
	{
		auto p1 = it->first;
		auto p2 = it->second;
		if (p1 != p2)
		{
			addCrossInfo(p1, p2, crossInfo);
			addCrossInfo(p2, p1, crossInfo);
		}
	}
	createClipedSurface(clipPoints, orderLines, ret);

	return ret;
}
void clipClipPlane(vector<Polyhedron>& polyhedrons, int clipPlane)
{
	map<Point, map<Point, int>> crossInfo;
	vector<PolyhedronInfo> newPolyhedronInfos;

	vector<Point> surface;
	for (auto& polyhedron : polyhedrons)
	{
		for (auto& index : polyhedron.indexs)
		{
			surface.clear();
			for (auto& idx : index)
				surface.push_back(polyhedron.points[idx]);

			auto surfaces = clipSurface(surface, clipPlane, crossInfo);
			for (auto&surface : surfaces)
			{
				addSurface(newPolyhedronInfos, surface);
			}
		}
	}
	
	// 裁剪面上的面
	if (crossInfo.size() > 2)
	{
		auto clipSurfaces = createClipSuface(crossInfo);
		for (auto&surface : clipSurfaces)
		{
			antiClockPoints(surface, getClipPlaneNormalVec(clipPlane));
			addSurface(newPolyhedronInfos, surface);
		}
	}

	polyhedrons.clear();
	for (auto& newPolyhedronInfo : newPolyhedronInfos)
	{
		polyhedrons.push_back(newPolyhedronInfo.polyhedron);
	}
}
bool checkEntire(Polyhedron& polyhedron)
{
	bool allInside = true;
	for (auto& clipPlane : clipPlanes)
	{
		bool allOutside = true;
		for (auto& p : polyhedron.points)
		{
			if (inside(p, clipPlane))
				allOutside = false;
			else
				allInside = false;
		}

		if (allInside)
		{
			continue;
		}

		if (allOutside)
		{
			polyhedron.points = {};
			polyhedron.indexs = {};
			return true;
		}
	}

	return allInside;
}
vector<Polyhedron> commonClip(Polyhedron& polyhedron)
{
	vector<Polyhedron> ret;

	if (!checkEntire(polyhedron))
	{
		ret = { polyhedron };
		for (auto& clipPlane : clipPlanes)
		{
			clipClipPlane(ret, clipPlane);
		}
	}
	else
	{
		ret = { polyhedron };
	}
	return ret;
}
Polyhedron cube = {
	{ { -50.f, -50.f, 50.f },{ 0.f, -50.f, 0.f },{ 0.f, 50.f, 0.f },{ -50.f, 50.f, 50.f },
	{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ 0.f, -50.f, -50.f },{ 0.f, 50.f, -50.f } },
	{ { 0, 1, 2, 3 },{ 1, 4, 5, 2 },{ 5, 4, 6, 7 },{ 3, 7, 6, 0 },{ 3, 2, 5, 7 },{ 0, 6, 4, 1 } }
};
//Polyhedron cube = {
//	{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
//	{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },
//	{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
//};
void view()
{
	glColor3f(1.0, 1.0, 1.0);
	winWidth = 400, winHeight = 300;

	auto temp = cube;

	Point viewPortPoint = { 200.f, 150.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / winHeight * clipWinHeight;

	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -1200.f) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v);

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);


	transformPoints(m, temp.points);
	for (auto& p : temp.points)
		descartes(p);

	auto temp_clip = temp;
	auto temp_clips = commonClip(temp_clip);

	transformPoints(m1, temp.points);
	for (auto& temp_clip : temp_clips)
	{
		if (temp_clip.points.size() > 0)
			transformPoints(m1, temp_clip.points);
	}

	glColor3f(1.0, 1.0, 1.0);
	drawPolyhedron(temp);
	glColor3f(1.0, 0.0, 0.0);
	for (auto& temp_clip : temp_clips)
	{
		if (temp_clip.points.size() > 0)
			drawPolyhedron(temp_clip);
	}
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	view();

	glFlush();
}
void code_10_exercise_13_test()
{
	vector<int> a = { 1 };
	auto it = a.begin() + 1;
	camera = new Camera({ 100, 200, 500 }, { 0, 0, 1 }, { 0, 1, 0 }, 1000);
	//camera = new Camera({ 278, -272, 500 }, { 0, 0, 1 }, { 0, 1, 0 }, 1000);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_10_EXERCISE_16
class Surface
{
public:
	Surface(const vector<Point>& points_) : A(0), B(0), C(0), D(0)
	{
		if (points_.size() >= 3)
		{
			points = points_;
			A = points[0].y * (points[1].z - points[2].z) + points[1].y * (points[2].z - points[0].z) + points[2].y * (points[0].z - points[1].z);
			B = points[0].z * (points[1].x - points[2].x) + points[1].z * (points[2].x - points[0].x) + points[2].z * (points[0].x - points[1].x);
			C = points[0].x * (points[1].y - points[2].y) + points[1].x * (points[2].y - points[0].y) + points[2].x * (points[0].y - points[1].y);
			D = -points[0].x * (points[1].y * points[2].z - points[2].y * points[1].z)
				- points[1].x * (points[2].y * points[0].z - points[0].y * points[2].z)
				- points[2].x * (points[0].y * points[1].z - points[1].y * points[0].z);
		}
	}
	vector<Point> points;
	float A, B, C, D;
};
Camera *camera = nullptr;
const int LeftClipPlane = 1;
const int RightClipPlane = 2;
const int BottomClipPlane = 3;
const int TopClipPlane = 4;
const int NearClipPlane = 5;
const int FarClipPlane = 6;
const std::vector<int> clipPlanes = { LeftClipPlane, RightClipPlane, BottomClipPlane, TopClipPlane, NearClipPlane, FarClipPlane };
void addCrossInfo(Point p1, Point p2, map<Point, map<Point, int>>& crossInfo)
{
	//auto it1 = crossInfo.find(p1);
	//if (it1 != crossInfo.end())
	//{
	//	it1->second[p2] = 1;
	//}
	//else
	//{
	//	crossInfo[p1][p2] = 1;
	//}
	crossInfo[p1][p2] = 1;
}
bool checkSamePoint(vector<Point>& points)
{
	map<Point, int> testSame;
	for (auto it = points.begin(); it != points.end();)
	{
		if (testSame.find(*it) == testSame.end())
		{
			testSame[*it] = 1;
			it++;
		}
		else
		{
			it = points.erase(it);
		}
	}

	return points.size() >= 3;
}
void addSurface(Polyhedron& polyhedron, vector<Point>& points, map<Point, int>& pointInfo)
{
	if (checkSamePoint(points))
	{
		polyhedron.indexs.push_back({});
		for (int i = 0; i < points.size(); i++)
		{
			auto it = pointInfo.find(points[i]);
			if (it != pointInfo.end())
			{
				polyhedron.indexs.back().push_back(it->second);
			}
			else
			{
				polyhedron.points.push_back(points[i]);
				int idx = polyhedron.points.size() - 1;
				polyhedron.indexs.back().push_back(idx);

				pointInfo[points[i]] = idx;
			}
		}
	}
}
vector<Point> createClipSuface(map<Point, map<Point, int>>& crossInfo)
{
	vector<Point> ret;

	if (crossInfo.size() < 3)
		return ret;

	auto beginPoint = crossInfo.begin()->first;
	ret.push_back(beginPoint);

	auto curPoint = beginPoint;
	while (true)
	{
		//assert(crossInfo.find(curPoint) != crossInfo.end());
		if (crossInfo.find(curPoint) == crossInfo.end())
		{
			ret.clear();
			return ret;
		}
		auto nextPoint = crossInfo[curPoint].begin()->first;
		assert(crossInfo[curPoint][nextPoint] == 1);

		//assert(crossInfo.find(nextPoint) != crossInfo.end());
		if (crossInfo.find(nextPoint) == crossInfo.end())
		{
			ret.clear();
			return ret;
		}
		if (crossInfo[nextPoint].find(curPoint) == crossInfo[nextPoint].end())
		{
			ret.clear();
			return ret;
		}
		assert(crossInfo[nextPoint][curPoint] == 1);

		crossInfo[curPoint].erase(nextPoint);
		if (crossInfo[curPoint].size() == 0)
		{
			crossInfo.erase(curPoint);
		}

		crossInfo[nextPoint].erase(curPoint);
		if (crossInfo[nextPoint].size() == 0)
		{
			crossInfo.erase(nextPoint);
		}

		if (nextPoint != beginPoint)
			ret.push_back(nextPoint);
		else
			break;
		curPoint = nextPoint;
	}
	return ret;
}

void antiClockPoints(vector<Point>& points, Vec3 normalVec)
{
	Vec3 a = { points[1].x - points[0].x, points[1].y - points[0].y, points[1].z - points[0].z };
	Vec3 b = { points[2].x - points[1].x, points[2].y - points[1].y, points[2].z - points[1].z };
	Vec3 crossVec = cross(a, b);
	bool antiClocked = crossVec.x * normalVec.x >= 0 && crossVec.y * normalVec.y >= 0 && crossVec.z * normalVec.z >= 0;
	if (!antiClocked)
	{
		std::reverse(points.begin(), points.end());
	}
}
bool inside(Point p, Surface& clipPlane)
{
	return clipPlane.A * p.x + clipPlane.B * p.y + clipPlane.C * p.z + clipPlane.D < 0;
}
Point clacCrossPoint(Point p1, Point p2, Surface& clipPlane)
{
	float u = 0.f;
	if (clipPlane.A * (p2.x - p1.x) + clipPlane.B * (p2.y - p1.y) + clipPlane.C * (p2.z - p1.z) != 0)
	{
		u = (-clipPlane.D - clipPlane.A * p1.x - clipPlane.B * p1.y - clipPlane.C * p1.z) /
			(clipPlane.A * (p2.x - p1.x) + clipPlane.B * (p2.y - p1.y) + clipPlane.C * (p2.z - p1.z));
	}
	return{ p1.x + (p2.x - p1.x) * u,
		p1.y += (p2.y - p1.y) * u ,
		p1.z += (p2.z - p1.z) * u };
}
vector<Point> clipSurface(const vector<Point>& points, Surface& clipPlane, map<Point, map<Point, int>>& crossInfo)
{
	if (points.size() < 3)
	{
		printf("%d\n", points.size());
	}

	vector<Point> ret;
	vector<Point> cross;
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 < points.size() ? i + 1 : 0;

		if (!inside(points[i], clipPlane))
		{
			if (inside(points[next], clipPlane))
			{
				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
				ret.push_back(cp);
				ret.push_back(points[next]);

				cross.push_back(cp);
			}
		}
		else
		{
			if (inside(points[next], clipPlane))
			{
				ret.push_back(points[next]);
			}
			else
			{
				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
				ret.push_back(cp);
				cross.push_back(cp);
			}
		}
	}
	if (cross.size() > 1 && cross[0] != cross[1])
	{
		addCrossInfo(cross[0], cross[1], crossInfo);
		addCrossInfo(cross[1], cross[0], crossInfo);
	}

	return ret;
}
void clipClipPlane(Polyhedron& polyhedron, Surface& clipPlane)
{
	map<Point, int> pointInfo;
	map<Point, map<Point, int>> crossInfo;
	Polyhedron newPolyhedron;

	vector<Point> surface;
	for (auto& index : polyhedron.indexs)
	{
		surface.clear();
		for (auto& idx : index)
			surface.push_back(polyhedron.points[idx]);

		auto points = clipSurface(surface, clipPlane, crossInfo);
		if (points.size() > 0)
			addSurface(newPolyhedron, points, pointInfo);
	}

	// 裁剪面上的面
	if (crossInfo.size() >= 3)
	{
		auto clipSurfacePoints = createClipSuface(crossInfo);
		if (clipSurfacePoints.size() >= 3)
		{
			antiClockPoints(clipSurfacePoints, { clipPlane.A, clipPlane.B, clipPlane.C });
			addSurface(newPolyhedron, clipSurfacePoints, pointInfo);
		}
	}

	polyhedron = newPolyhedron;
}
bool checkEntire(const Polyhedron& viewVolume, Polyhedron& polyhedron)
{
	vector<Point> temp;
	bool allInside = true;
	for (auto& index : viewVolume.indexs)
	{
		temp.clear();
		for (auto& idx : index)
			temp.push_back(viewVolume.points[idx]);
		Surface clipPlane(temp);
				
		bool allOutside = true;
		for (auto& p : polyhedron.points)
		{
			if (inside(p, clipPlane))
				allOutside = false;
			else
				allInside = false;
		}

		if (allInside)
		{
			continue;
		}

		if (allOutside)
		{
			polyhedron.points = {};
			polyhedron.indexs = {};
			return true;
		}
	}

	return allInside;
}
void clipCommonViewVolume(const Polyhedron& viewVolume, Polyhedron& polyhedron)
{
	if (!checkEntire(viewVolume, polyhedron))
	{
		vector<Point> temp;
		for (auto& index : viewVolume.indexs)
		{
			temp.clear();
			for (auto& idx : index)
				temp.push_back(viewVolume.points[idx]);
			Surface clipPlane(temp);
			
			clipClipPlane(polyhedron, clipPlane);
		}
	}
}
void clipFrustum(float xwmin, float xwmax, float ywmin, float ywmax, float znear, float zfar, Polyhedron& polyhedron)
{
	vector<Point> nearSurface = {
		{ xwmin , ywmin, znear },
		{ xwmax , ywmin, znear },
		{ xwmax , ywmax, znear },
		{ xwmin , ywmax, znear } };
	float fnRate = zfar / znear;
	vector<Point> farSurface = {
		{ xwmin * fnRate, ywmin * fnRate, zfar },
		{ xwmax * fnRate, ywmin * fnRate, zfar },
		{ xwmax * fnRate, ywmax * fnRate, zfar },
		{ xwmin * fnRate, ywmax * fnRate, zfar } };

	Polyhedron viewVolume;
	map<Point, int> pointInfo;
	addSurface(viewVolume, nearSurface, pointInfo);

	vector<Point> temp = { farSurface[0], farSurface[3], farSurface[2], farSurface[1] };
	addSurface(viewVolume, temp, pointInfo);

	temp = { farSurface[0], nearSurface[0], nearSurface[3], farSurface[3] };
	addSurface(viewVolume, temp, pointInfo);

	temp = { nearSurface[3], nearSurface[2], farSurface[2], farSurface[3] };
	addSurface(viewVolume, temp, pointInfo);

	temp = { farSurface[1], farSurface[2], nearSurface[2], nearSurface[1] };
	addSurface(viewVolume, temp, pointInfo);

	temp = { farSurface[0], farSurface[1], nearSurface[1], nearSurface[0] };
	addSurface(viewVolume, temp, pointInfo);

	clipCommonViewVolume(viewVolume, polyhedron);
}
Polyhedron cube = {
	{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
	{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },
	{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};
void view()
{
	glColor3f(1.0, 1.0, 1.0);
	winWidth = 400, winHeight = 300;

	auto temp = cube;

	Point viewPortPoint = { 200.f, 150.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });
	
	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / winHeight * clipWinHeight;

	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -1200.f);
	auto m1= viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);

	transformPoints(viewMatrix(camera->_viewP0, camera->_n, camera->_v), temp.points);

	auto temp_clip = temp;
	clipFrustum(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -1200.f, temp_clip);

	transformPoints(m, temp.points);
	for (auto& p : temp.points)
		descartes(p);
	if (temp_clip.points.size() > 0)
	{
		transformPoints(m, temp_clip.points);
		for (auto& p : temp_clip.points)
			descartes(p);
	}

	transformPoints(m1, temp.points);
	if (temp_clip.points.size() > 0)
		transformPoints(m1, temp_clip.points);

	glColor3f(1.0, 1.0, 1.0);
	drawPolyhedron(temp);
	glColor3f(1.0, 0.0, 0.0);
	if (temp_clip.points.size() > 0)
		drawPolyhedron(temp_clip);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	view();

	glFlush();
}
void code_10_exercise_16()
{
	camera = new Camera({ 100, 200, 500 }, { 0, 0, 1 }, { 0, 1, 0 }, 1000);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_10_EXERCISE_20
Camera *camera = nullptr;
const int LeftClipPlane = 1;
const int RightClipPlane = 2;
const int BottomClipPlane = 3;
const int TopClipPlane = 4;
const int NearClipPlane = 5;
const int FarClipPlane = 6;
const std::vector<int> clipPlanes = { LeftClipPlane, RightClipPlane, BottomClipPlane, TopClipPlane, NearClipPlane, FarClipPlane };
void addCrossInfo(Point p1, Point p2, map<Point, map<Point, int>>& crossInfo)
{
	//auto it1 = crossInfo.find(p1);
	//if (it1 != crossInfo.end())
	//{
	//	it1->second[p2] = 1;
	//}
	//else
	//{
	//	crossInfo[p1][p2] = 1;
	//}
	crossInfo[p1][p2] = 1;
}
bool checkSamePoint(vector<Point>& points)
{
	map<Point, int> testSame;
	for (auto it = points.begin(); it != points.end();)
	{
		if (testSame.find(*it) == testSame.end())
		{
			testSame[*it] = 1;
			it++;
		}
		else
		{
			it = points.erase(it);
		}
	}

	return points.size() >= 3;
}
void addSurface(Polyhedron& polyhedron, vector<Point>& points, map<Point, int>& pointInfo)
{
	if (checkSamePoint(points))
	{
		polyhedron.indexs.push_back({});
		for (int i = 0; i < points.size(); i++)
		{
			auto it = pointInfo.find(points[i]);
			if (it != pointInfo.end())
			{
				polyhedron.indexs.back().push_back(it->second);
			}
			else
			{
				polyhedron.points.push_back(points[i]);
				int idx = polyhedron.points.size() - 1;
				polyhedron.indexs.back().push_back(idx);

				pointInfo[points[i]] = idx;
			}
		}
	}
}
vector<Point> createClipSuface(map<Point, map<Point, int>>& crossInfo)
{
	vector<Point> ret;

	if (crossInfo.size() < 3)
		return ret;

	auto beginPoint = crossInfo.begin()->first;
	ret.push_back(beginPoint);

	auto curPoint = beginPoint;
	while (true)
	{
		//assert(crossInfo.find(curPoint) != crossInfo.end());
		if (crossInfo.find(curPoint) == crossInfo.end())
		{
			ret.clear();
			return ret;
		}
		auto nextPoint = crossInfo[curPoint].begin()->first;
		assert(crossInfo[curPoint][nextPoint] == 1);

		//assert(crossInfo.find(nextPoint) != crossInfo.end());
		if (crossInfo.find(nextPoint) == crossInfo.end())
		{
			ret.clear();
			return ret;
		}
		if (crossInfo[nextPoint].find(curPoint) == crossInfo[nextPoint].end())
		{
			ret.clear();
			return ret;
		}
		assert(crossInfo[nextPoint][curPoint] == 1);

		crossInfo[curPoint].erase(nextPoint);
		if (crossInfo[curPoint].size() == 0)
		{
			crossInfo.erase(curPoint);
		}

		crossInfo[nextPoint].erase(curPoint);
		if (crossInfo[nextPoint].size() == 0)
		{
			crossInfo.erase(nextPoint);
		}

		if (nextPoint != beginPoint)
			ret.push_back(nextPoint);
		else
			break;
		curPoint = nextPoint;
	}

	return ret;
}

void antiClockPoints(vector<Point>& points, Vec3 normalVec)
{
	Point p0 = points[0];
	descartes(p0);
	Point p1 = points[1];
	descartes(p1);
	Point p2 = points[2];
	descartes(p2);

	Vec3 a = { p1.x - p0.x, p1.y - p0.y, -(p1.z - p0.z) }; // z取负值是因为在左手坐标系和右手坐标系z反号
	Vec3 b = { p2.x - p1.x, p2.y - p1.y, -(p2.z - p1.z) };
	Vec3 crossVec = cross(a, b);
	bool antiClocked = crossVec.x * normalVec.x >= 0 && crossVec.y * normalVec.y >= 0 && crossVec.z * normalVec.z >= 0;
	if (!antiClocked)
	{
		std::reverse(points.begin(), points.end());
	}
}
Vec3 getClipPlaneNormalVec(int clipPlane)
{
	switch (clipPlane)
	{
	case LeftClipPlane:
		return{ -1, 0, 0 };
	case RightClipPlane:
		return{ 1, 0, 0 };
	case BottomClipPlane:
		return{ 0, -1, 0 };
	case TopClipPlane:
		return{ 0, 1, 0 };
	case NearClipPlane:
		return{ 0, 0, -1 };
	case FarClipPlane:
		return{ 0, 0, 1 };
	}
}

bool inside(Point p, int clipPlane)
{
	if (p.h != 0)
	{
		float sn = sign(p.h);
		switch (clipPlane)
		{
		case LeftClipPlane:
			return sn * (p.h + p.x) > 0;
		case RightClipPlane:
			return sn * (p.h - p.x) > 0;
		case BottomClipPlane:
			return sn * (p.h + p.y) > 0;
		case TopClipPlane:
			return sn * (p.h - p.y) > 0;
		case NearClipPlane:
			return sn * (p.h + p.z) > 0;
		case FarClipPlane:
			return sn * (p.h - p.z) > 0;
		}
	}
	return false;
}
Point clacCrossPoint(Point p1, Point p2, int clipPlane)
{
	float u = 0.f;
	switch (clipPlane)
	{
	case LeftClipPlane:
		u = (p1.x + p1.h) / ((p1.x + p1.h) - (p2.x + p2.h));
		break;
	case RightClipPlane:
		u = (p1.x - p1.h) / ((p1.x - p1.h) - (p2.x - p2.h));
		break;
	case BottomClipPlane:
		u = (p1.y + p1.h) / ((p1.y + p1.h) - (p2.y + p2.h));
		break;
	case TopClipPlane:
		u = (p1.y - p1.h) / ((p1.y - p1.h) - (p2.y - p2.h));
		break;
	case NearClipPlane:
		u = (p1.z + p1.h) / ((p1.z + p1.h) - (p2.z + p2.h));
		break;
	case FarClipPlane:
		u = (p1.z - p1.h) / ((p1.z - p1.h) - (p2.z - p2.h));
		break;
	default:
		break;
	}

	return{ p1.x + (p2.x - p1.x) * u,
		p1.y += (p2.y - p1.y) * u ,
		p1.z += (p2.z - p1.z) * u,
		p1.h += (p2.h - p1.h) * u };
}
vector<Point> clipSurface(const vector<Point>& points, int clipPlane, map<Point, map<Point, int>>& crossInfo)
{
	if (points.size() < 3)
	{
		printf("%d\n", points.size());
	}

	vector<Point> ret;
	vector<Point> cross;
	for (int i = 0; i < points.size(); i++)
	{
		int next = i + 1 < points.size() ? i + 1 : 0;

		if (!inside(points[i], clipPlane))
		{
			if (inside(points[next], clipPlane))
			{
				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
				ret.push_back(cp);
				ret.push_back(points[next]);

				cross.push_back(cp);
			}
		}
		else
		{
			if (inside(points[next], clipPlane))
			{
				ret.push_back(points[next]);
			}
			else
			{
				auto cp = clacCrossPoint(points[i], points[next], clipPlane);
				ret.push_back(cp);
				cross.push_back(cp);
			}
		}
	}
	if (cross.size() > 1 && cross[0] != cross[1])
	{
		addCrossInfo(cross[0], cross[1], crossInfo);
		addCrossInfo(cross[1], cross[0], crossInfo);
	}

	return ret;
}
void clipClipPlane(Polyhedron& polyhedron, int clipPlane)
{
	map<Point, int> pointInfo;
	map<Point, map<Point, int>> crossInfo;
	Polyhedron newPolyhedron;

	vector<Point> surface;
	for (auto& index : polyhedron.indexs)
	{
		surface.clear();
		for (auto& idx : index)
			surface.push_back(polyhedron.points[idx]);

		auto points = clipSurface(surface, clipPlane, crossInfo);
		if (points.size() > 0)
			addSurface(newPolyhedron, points, pointInfo);
	}

	// 裁剪面上的面
	if (crossInfo.size() > 2)
	{
		auto clipSurfacePoints = createClipSuface(crossInfo);
		antiClockPoints(clipSurfacePoints, getClipPlaneNormalVec(clipPlane));
		addSurface(newPolyhedron, clipSurfacePoints, pointInfo);
	}

	polyhedron = newPolyhedron;
}
bool checkEntire(Polyhedron& polyhedron)
{
	bool allInside = true;
	for (auto& clipPlane : clipPlanes)
	{
		bool allOutside = true;
		for (auto& p : polyhedron.points)
		{
			if (inside(p, clipPlane))
				allOutside = false;
			else
				allInside = false;
		}

		if (allInside)
		{
			continue;
		}

		if (allOutside)
		{
			polyhedron.points = {};
			polyhedron.indexs = {};
			return true;
		}
	}

	return allInside;
}
void clipHomogeneous(Polyhedron& polyhedron)
{
	if (!checkEntire(polyhedron))
	{
		for (auto& clipPlane : clipPlanes)
		{
			clipClipPlane(polyhedron, clipPlane);
		}
	}
}
Polyhedron cube = {
	{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
	{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },
	{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};
void view()
{
	glColor3f(1.0, 1.0, 1.0);
	winWidth = 400, winHeight = 300;

	auto temp = cube;

	Point viewPortPoint = { 200.f, 150.f };
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });

	float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
	float clipWinWidth = winWidth / winHeight * clipWinHeight;

	auto m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -1.f, -1200.f) *
		viewMatrix(camera->_viewP0, camera->_n, camera->_v);

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);

	transformPoints(m, temp.points);

	auto temp_clip = temp;
	clipHomogeneous(temp_clip);

	for (auto& p : temp.points)
		descartes(p);
	if (temp_clip.points.size() > 0)
	{
		for (auto& p : temp_clip.points)
			descartes(p);
	}

	transformPoints(m1, temp.points);
	if (temp_clip.points.size() > 0)
		transformPoints(m1, temp_clip.points);

	glColor3f(1.0, 1.0, 1.0);
	drawPolyhedron(temp);
	glColor3f(1.0, 0.0, 0.0);
	if (temp_clip.points.size() > 0)
		drawPolyhedron(temp_clip);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	view();

	glFlush();
}
void code_10_exercise_20()
{
	camera = new Camera({ 100, 200, 500 }, { 0, 0, 1 }, { 0, 1, 0 }, 1000);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_10_EXERCISE_21
GLint winWidth = 600, winHeight = 600;
GLfloat x0 = 100.0, y_0 = 50.0, z0 = -50.0;
GLfloat xref = 50.0, yref = 50.0, zref = 0.0;
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;
GLfloat xwMin = -40.0, ywMin = -60.0, xwMax = 40.0, ywMax = 60.0;
GLfloat dnear = 25.0, dfar = 125.0;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(x0, y_0, z0, xref, yref, zref, Vx, Vy, Vz);
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

#ifdef CHAPTER_10_EXERCISE_22
Camera *camera = nullptr;
GLfloat x0 = 100.0, y_0 = 50.0, z0 = 50.0;
GLfloat xref = 50.0, yref = 50.0, zref = 0.0;
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;
GLfloat xwMin = -80.0, ywMin = -60.0, xwMax = 80.0, ywMax = 60.0;
GLfloat dnear = 25.0, dfar = 125.0;
void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
}
void drawFunc()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Point pu = { camera->_viewP0.x - camera->_n.x,
		camera->_viewP0.y - camera->_n.y,
		camera->_viewP0.z - camera->_n.z, };
	gluLookAt(camera->_viewP0.x, camera->_viewP0.y, camera->_viewP0.z, 
		pu.x, pu.y, pu.z,
		camera->_v.x, camera->_v.y, camera->_v.z);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar);

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
void code_10_exercise_22()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	camera = new Camera({ x0, y_0, z0 }, { x0 - xref, y_0 - yref, z0 - zref }, { Vx, Vy, Vz }, 500);
	glutDisplayFunc(drawFunc);
	glutReshapeFunc(reshapeFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_23
Camera *camera = nullptr;
GLfloat x0 = 100.0, y_0 = 50.0, z0 = 500.0;
GLfloat xref = 50.0, yref = 50.0, zref = 0.0;
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;
GLfloat xwMin = -80.0, ywMin = -60.0, xwMax = 80.0, ywMax = 60.0;
GLfloat dnear = 145.0, dfar = 1250.0;
Polyhedron cube = {
	{ { 0.f, 0.f, 0.f },{ 100.f, 0.f, 0.f },{ 100.f, 100.f, 0.f },{ 0.f, 100.f, 0.f },
	{ 0.f, 0.f, -100.f },{ 100.f, 0.f, -100.f },{ 100.f, 100.f, -100.f },{ 0.f, 100.f, -100.f } },
	{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};
void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
}
void drawFunc()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Point pu = { camera->_viewP0.x - camera->_n.x,
		camera->_viewP0.y - camera->_n.y,
		camera->_viewP0.z - camera->_n.z, };
	gluLookAt(camera->_viewP0.x, camera->_viewP0.y, camera->_viewP0.z,
		pu.x, pu.y, pu.z,
		camera->_v.x, camera->_v.y, camera->_v.z);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar);

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	drawPolyhedron3D(cube);

	glFlush();
}
void code_10_exercise_23()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	camera = new Camera({ x0, y_0, z0 }, { x0 - xref, y_0 - yref, z0 - zref }, { Vx, Vy, Vz }, 500);
	glutDisplayFunc(drawFunc);
	glutReshapeFunc(reshapeFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_24
GLfloat x0 = 50.0, y_0 = 50.0, z0 = 50.0;
GLfloat xref = 50.0, yref = 50.0, zref = 0.0;
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;
GLfloat xwMin = -400.0, ywMin = -300.0, xwMax = 400.0, ywMax = 300.0;
GLfloat dnear = 25.0, dfar = 1250.0;
Polyhedron cube = {
	{ { 0.f, 0.f, 0.f },{ 100.f, 0.f, 0.f },{ 100.f, 100.f, 0.f },{ 0.f, 100.f, 0.f },
	{ 0.f, 0.f, -100.f },{ 100.f, 0.f, -100.f },{ 100.f, 100.f, -100.f },{ 0.f, 100.f, -100.f } },
	{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};
void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
}
void drawFunc()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x0, y_0, z0, xref, yref, zref, Vx, Vy, Vz);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xwMin, xwMax, ywMin, ywMax, dnear, dfar);

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	drawPolyhedron3D(cube);

	glFlush();
}
void code_10_exercise_24()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(drawFunc);
	glutReshapeFunc(reshapeFcn);
}
#endif

#ifdef CHAPTER_10_EXERCISE_25
GLfloat x0 = 50.0, y_0 = 50.0, z0 = 50.0;
GLfloat xref = 50.0, yref = 50.0, zref = 0.0;
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;
GLfloat xwMin = -200.0, ywMin = -300.0, xwMax = 200.0, ywMax = 300.0;
GLfloat dnear = 25.0, dfar = 1250.0;
Polyhedron cube = {
	{ { 0.f, 0.f, 0.f },{ 100.f, 0.f, 0.f },{ 100.f, 100.f, 0.f },{ 0.f, 100.f, 0.f },
	{ 0.f, 0.f, -100.f },{ 100.f, 0.f, -100.f },{ 100.f, 100.f, -100.f },{ 0.f, 100.f, -100.f } },
	{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Vec3 Vp = { 1.f, 1.f, 2 * 1.414f };

	// 设置斜投影矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x0, y_0, z0, xref, yref, zref, Vx, Vy, Vz);

	glMatrixMode(GL_PROJECTION);
	auto m = normalMatrix(xwMin, xwMax, ywMin, ywMax, -dnear, -dfar) *
		parallelProjectionMatrix(Vp, -dnear);
	glLoadIdentity();
	glMultMatrixf(m);

	glViewport(0, 0, 400, 600);

	//GLfloat test[16];
	//glGetFloatv(GL_PROJECTION_MATRIX, test);

	glColor3f(0.0, 1.0, 0.0);
	drawPolyhedron3D(cube);

	// 先斜切变换对象，再正投影
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(shearZrefMatrix(-Vp.x / Vp.z, -Vp.y / Vp.z, -dnear));
	gluLookAt(x0, y_0, z0, xref, yref, zref, Vx, Vy, Vz);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xwMin, xwMax, ywMin, ywMax, dnear, dfar);

	glViewport(400, 0, 400, 600);

	glColor3f(0.0, 1.0, 0.0);
	drawPolyhedron3D(cube);

	glFlush();
}
void code_10_exercise_25()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_10_EXERCISE_ADD_2
Camera *camera = nullptr;
enum ProjectionType
{
	Orthogonal, // 正交投影
	Parallel, // 平行投影
	Perspective, // 透视投影
};
ProjectionType projectionType = Perspective;
GLfloat dnear = 1.f, dfar = 1200.f;
Vec3 parallelVec = { 1.f, 1.f, 2 * 1.414f };;

void normalKeyFcn(unsigned char key, int x, int y)
{
	//printf("normalKeyFcn %d, %d, %d\n", key, x, y);
	switch (key)
	{
	case 'a':
	case 'A':
		projectionType = Orthogonal;
		break;
	case 's':
	case 'S':
		projectionType = Parallel;
		break;
	case 'd':
	case 'D':
		projectionType = Perspective;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
Polyhedron cube = {
	{ { -50.f, -50.f, 50.f },{ 50.f, -50.f, 50.f },{ 50.f, 50.f, 50.f },{ -50.f, 50.f, 50.f },
	{ -50.f, -50.f, -50.f },{ 50.f, -50.f, -50.f },{ 50.f, 50.f, -50.f },{ -50.f, 50.f, -50.f } },
	{ { 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 4, 7, 6, 5 },{ 0, 3, 7, 4 },{ 0, 4, 5, 1 },{ 2, 6, 7, 3 } }
};

void view1()
{
	winWidth = 260, winHeight = 200;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 400, 0, 600);

	glViewport(0, 0, 400, 600);
	
	auto temp = cube;

	Point viewPortPoint = { 70.f, 200.f };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });

	auto m = viewMatrix(camera->_viewP0, camera->_n, camera->_v);

	if (projectionType == Orthogonal)
	{
		m = normalMatrix(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2, -dnear, -dfar) *
			orthogonalProjectionMatrix() * m;
	}
	else if (projectionType == Parallel)
	{
		m = normalMatrix(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2, -dnear, -dfar) *
			parallelProjectionMatrix(parallelVec, -dnear) * m;
	}
	else if (projectionType == Perspective)
	{
		float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
		float clipWinWidth = winWidth / winHeight * clipWinHeight;
		m = perspectiveProjectionAndNormalMatrix(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, -dnear, -dfar) * m;
	}

	auto m1 = viewportMatrix(viewPortPoint.x, viewPortPoint.x + winWidth, viewPortPoint.y, viewPortPoint.y + winHeight);
	
	transformPoints(m, temp.points);
	for (auto& p : temp.points)
		descartes(p);

	auto temp_clip = temp;
	clip(temp_clip);

	transformPoints(m1, temp.points);
	if (temp_clip.points.size() > 0)
		transformPoints(m1, temp_clip.points);

	glColor3f(1.0, 1.0, 1.0);
	drawPolyhedron(temp);
	glColor3f(1.0, 0.0, 0.0);
	if (temp_clip.points.size() > 0)
		drawPolyhedron(temp_clip);
}
void view2()
{
	winWidth = 260, winHeight = 200;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(400, 800, 0, 600);

	glViewport(400, 0, 400, 600);

	Point viewPortPoint = { 470.f, 200.f };
	glColor3f(1.0, 1.0, 1.0);
	drawPolygon({ viewPortPoint,{ viewPortPoint.x + winWidth, viewPortPoint.y, 0 },{ viewPortPoint.x + winWidth,viewPortPoint.y + winHeight, 0 },{ viewPortPoint.x, viewPortPoint.y + winHeight, 0 } });


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Point pu = { camera->_viewP0.x - camera->_n.x,
		camera->_viewP0.y - camera->_n.y,
		camera->_viewP0.z - camera->_n.z, };
	gluLookAt(camera->_viewP0.x, camera->_viewP0.y, camera->_viewP0.z,
		pu.x, pu.y, pu.z,
		camera->_v.x, camera->_v.y, camera->_v.z);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (projectionType == Orthogonal)
	{
		glOrtho(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2, dnear, dfar);
	}
	else if (projectionType == Parallel)
	{
		auto m = normalMatrix(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2, -dnear, -dfar) *
			parallelProjectionMatrix(parallelVec, -dnear);
		glMultMatrixf(m);
	}
	else if (projectionType == Perspective)
	{
		float clipWinHeight = 2 * tan(45.f / 2 * PI / 180);
		float clipWinWidth = winWidth / winHeight * clipWinHeight;
		glFrustum(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, dnear, dfar);
	}

	glViewport(viewPortPoint.x, viewPortPoint.y, winWidth, winHeight);
	
	glColor3f(1.0, 0.0, 0.0);
	drawPolyhedron3D(cube);
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	view1();

	view2();

	glFlush();
}
void code_10_exercise_add_2()
{
	camera = new Camera({ 100, 200, 500 }, { 0, 0, 1 }, { 0, 1, 0 }, 1000);
	glutDisplayFunc(drawFunc);
	glutKeyboardFunc(normalKeyFcn);
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_10_COMMON

#ifdef CHAPTER_10_COMMON
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // 黑色背景色
	glColor3f(1.0, 1.0, 1.0); // 白色绘制
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth , 0, winHeight);
}

void main(int argc, char** argv)
{
	//srand(time(0));
	srand(100);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("An Example OpenGL Program");

	init();

#ifdef CHAPTER_10_CAMERA
	code_10_camera();
#endif

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

#ifdef CHAPTER_10_EXERCISE_5
	code_10_exercise_5();
#endif

#ifdef CHAPTER_10_EXERCISE_6
	code_10_exercise_6();
#endif

#ifdef CHAPTER_10_EXERCISE_7
	code_10_exercise_7();
#endif

#ifdef CHAPTER_10_EXERCISE_8
	code_10_exercise_8();
#endif

#ifdef CHAPTER_10_EXERCISE_9
	code_10_exercise_9();
#endif

#ifdef CHAPTER_10_EXERCISE_10
	code_10_exercise_10();
#endif

#ifdef CHAPTER_10_EXERCISE_11
	code_10_exercise_11();
#endif

#ifdef CHAPTER_10_EXERCISE_12
	code_10_exercise_12();
#endif

#ifdef CHAPTER_10_EXERCISE_13
	code_10_exercise_13();
#endif

#ifdef CHAPTER_10_EXERCISE_13_TEST
	code_10_exercise_13_test();
#endif

#ifdef CHAPTER_10_EXERCISE_16
	code_10_exercise_16();
#endif

#ifdef CHAPTER_10_EXERCISE_20
	code_10_exercise_20();
#endif

#ifdef CHAPTER_10_EXERCISE_22
	code_10_exercise_22();
#endif

#ifdef CHAPTER_10_EXERCISE_23
	code_10_exercise_23();
#endif

#ifdef CHAPTER_10_EXERCISE_24
	code_10_exercise_24();
#endif

#ifdef CHAPTER_10_EXERCISE_25
	code_10_exercise_25();
#endif

#ifdef CHAPTER_10_EXERCISE_ADD_2
	code_10_exercise_add_2();
#endif

	glutMainLoop();
}
#endif
#endif 


