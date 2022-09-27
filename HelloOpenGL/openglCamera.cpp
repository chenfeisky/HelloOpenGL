#include "common.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"

#define GLUT_WHEEL_UP   0x0003
#define GLUT_WHEEL_DOWN 0x0004
#define INV_PI			(0.31830988618f)
#define HALF_PI			(1.57079632679f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)

#define ROTATE_X_FACTOR (0.1f)
#define ROTATE_Y_FACTOR (0.1f)
#define REVOLVE_X_FACTOR (0.25f)
#define REVOLVE_Y_FACTOR (0.25f)
#define MOVE_FACTOR (0.5f)
#define MOUSE_WHEEL_FACTOR (1.f)
#define MOUSE_MIDDLE_FACTOR (0.08f)

#define MAX_PITCH (89.f)
#define MIN_PITCH (-89.f)

static float winWidth, winHeight;
static void (*displayFcn)(void) = nullptr;
static GLfloat dnear = 1, dfar = 2000;
static Camera* camera = nullptr;
static int fps = 60;

static class Matrix
{
public:
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
	operator GLfloat* ()
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

static class InputManager
{
public:
	enum MouseEvent
	{
		MouseDown,
		MouseUp,
		MouseDrage,
	};
	enum KeyEvent
	{
		KeyDown,
		KeyUp,
		KeyInPress,
	};
public:
	InputManager() {};
	static InputManager* GetInstance();

	void Init();
	void AddMouseEventListen(void (*func)(int button, MouseEvent mouseEvent, int x, int y));
	void AddKeyEventListen(void (*func)(unsigned char key, KeyEvent keyEvent));

	static void onTimer(int);

private:
	void onMouseFunc(int button, int state, int x, int y);
	void onMouseMoveFunc(int x, int y);
	void onKeyDownFunc(unsigned char key, int x, int y);
	void onKeyUpFunc(unsigned char key, int x, int y);
	void update();
	float realY(float y);

private:
	static InputManager* InputManagerInstance;
	clock_t lastTick = 0.0;
	int lastMouseX, lastMouseY;
	int _curPressedButton = -1;

	void (*mouseEventFunc)(int button, MouseEvent mouseEvent, int x, int y) = nullptr;
	void (*keyEventFunc)(unsigned char key, KeyEvent keyEvent) = nullptr;
	std::map<unsigned char, int> keyState;
};

InputManager* InputManager::InputManagerInstance = nullptr;

InputManager* InputManager::GetInstance()
{
	if (!InputManagerInstance)
	{
		InputManagerInstance = new InputManager();
		InputManagerInstance->Init();
	}
	return InputManagerInstance;
}

void InputManager::onMouseFunc(int button, int state, int x, int y)
{
	y = realY(y);
	if (state == GLUT_DOWN)
	{
		lastMouseX = x;
		lastMouseY = y;
		_curPressedButton = button;

		if (mouseEventFunc)
			mouseEventFunc(button, MouseEvent::MouseDown, x, y);
	}
	else if (state == GLUT_UP)
	{
		lastMouseX = x;
		lastMouseY = y;
		_curPressedButton = -1;

		if (mouseEventFunc)
			mouseEventFunc(button, MouseEvent::MouseUp, x, y);
	}
}

void InputManager::onMouseMoveFunc(int x, int y)
{
	if (_curPressedButton != -1)
	{
		y = realY(y);
		if (mouseEventFunc)
			mouseEventFunc(_curPressedButton, MouseEvent::MouseDrage, x - lastMouseX, y - lastMouseY);
		lastMouseX = x;
		lastMouseY = y;
	}
}

void InputManager::onKeyDownFunc(unsigned char key, int x, int y)
{
	if (keyState[key] == 0)
	{
		keyState[key] = 1;

		if (keyEventFunc)
			keyEventFunc(key, KeyEvent::KeyDown);
	}
}

void InputManager::onKeyUpFunc(unsigned char key, int x, int y)
{
	if (keyState[key] == 1)
	{
		keyState[key] = 0;

		if (keyEventFunc)
			keyEventFunc(key, KeyEvent::KeyUp);
	}
}

float InputManager::realY(float y)
{
	return winHeight - y;
}

void InputManager::update()
{
	auto curTick = clock();
	float dt = (float)(curTick - lastTick) / CLOCKS_PER_SEC;

	for (auto it = keyState.begin(); it != keyState.end(); it++)
	{
		if (it->second == 1)
		{
			if (keyEventFunc)
				keyEventFunc(it->first, KeyEvent::KeyInPress);
		}
	}

	lastTick = curTick;
}

void InputManager::onTimer(int)
{
	InputManager::GetInstance()->update();
	camera->update();
	glutTimerFunc((unsigned)(1000.f / fps), onTimer, 0);
}

void InputManager::Init()
{
	glutMouseFunc([](int button, int state, int x, int y)
		{
			InputManager::GetInstance()->onMouseFunc(button, state, x, y);
		});
	glutMotionFunc([](int x, int y)
		{
			InputManager::GetInstance()->onMouseMoveFunc(x, y);
		});
	glutKeyboardFunc([](unsigned char key, int x, int y)
		{
			InputManager::GetInstance()->onKeyDownFunc(key, x, y);
		});
	glutKeyboardUpFunc([](unsigned char key, int x, int y)
		{
			InputManager::GetInstance()->onKeyUpFunc(key, x, y);
		});

	lastTick = clock();

	glutTimerFunc((unsigned)(1000.f / fps), onTimer, 0);
}

void InputManager::AddMouseEventListen(void (*func)(int button, MouseEvent mouseEvent, int x, int y))
{
	mouseEventFunc = func;
}

void InputManager::AddKeyEventListen(void (*func)(unsigned char key, KeyEvent keyEvent))
{
	keyEventFunc = func;
}

static bool IsNearlyEqual(float A, float B, float ErrorTolerance = SMALL_NUMBER)
{
	return abs(A - B) <= ErrorTolerance;
}

Point::operator Vec3()
{
	if (!IsNearlyEqual(h, 0.f))
		return { x / h, y / h, z / h };
	else
		return { x, y, z };
}

static float length(const Vec3& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static void normal(Vec3& v)
{
	float len = length(v);
	v.x = v.x / len;
	v.y = v.y / len;
	v.z = v.z / len;
}

static Vec3 operator +(const Vec3& v1, const Vec3& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

static Vec3 operator -(const Vec3& v1, const Vec3& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

static Vec3 operator *(float i, const Vec3& v)
{
	return { i * v.x, i * v.y,  i * v.z };
}

static Vec3 operator *(const Vec3& v, float i)
{
	return i * v;
}

static Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	return{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

static float dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static Matrix operator *(Matrix& m1, Matrix& m2)
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

static void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
static Matrix rotateMatrix(Point p0, Vec3 u, float theta)
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

static void transformPoint(Matrix& m, Point& point)
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

typedef Point Vec4;
static void transformVec4(Vec4& vec, Matrix& m)
{
	Matrix _vec(1, 4);
	Matrix temp(1, 4);
	_vec[0][0] = vec.x;
	_vec[0][1] = vec.y;
	_vec[0][2] = vec.z;
	_vec[0][3] = vec.h;
	temp = _vec * m;
	vec.x = temp[0][0];
	vec.y = temp[0][1];
	vec.z = temp[0][2];
	vec.h = temp[0][3];
}

static float Atan2(float Y, float X)
{
	//return atan2f(Y,X);
	// atan2f occasionally returns NaN with perfectly valid input (possibly due to a compiler or library bug).
	// We are replacing it with a minimax approximation with a max relative error of 7.15255737e-007 compared to the C library function.
	// On PC this has been measured to be 2x faster than the std C version.

	const float absX = abs(X);
	const float absY = abs(Y);
	const bool yAbsBigger = (absY > absX);
	float t0 = yAbsBigger ? absY : absX; // Max(absY, absX)
	float t1 = yAbsBigger ? absX : absY; // Min(absX, absY)

	if (t0 == 0.f)
		return 0.f;

	float t3 = t1 / t0;
	float t4 = t3 * t3;

	static const float c[7] = {
		+7.2128853633444123e-03f,
		-3.5059680836411644e-02f,
		+8.1675882859940430e-02f,
		-1.3374657325451267e-01f,
		+1.9856563505717162e-01f,
		-3.3324998579202170e-01f,
		+1.0f
	};

	t0 = c[0];
	t0 = t0 * t4 + c[1];
	t0 = t0 * t4 + c[2];
	t0 = t0 * t4 + c[3];
	t0 = t0 * t4 + c[4];
	t0 = t0 * t4 + c[5];
	t0 = t0 * t4 + c[6];
	t3 = t0 * t3;

	t3 = yAbsBigger ? (0.5f * PI) - t3 : t3;
	t3 = (X < 0.0f) ? PI - t3 : t3;
	t3 = (Y < 0.0f) ? -t3 : t3;

	return t3;
}

static void SinCos(float* ScalarSin, float* ScalarCos, float  Value)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = (INV_PI * 0.5f) * Value;
	if (Value >= 0.0f)
	{
		quotient = (float)((int)(quotient + 0.5f));
	}
	else
	{
		quotient = (float)((int)(quotient - 0.5f));
	}
	float y = Value - (2.0f * PI) * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	float sign;
	if (y > HALF_PI)
	{
		y = PI - y;
		sign = -1.0f;
	}
	else if (y < -HALF_PI)
	{
		y = -PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float y2 = y * y;

	// 11-degree minimax approximation
	*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*ScalarCos = sign * p;
}

static float DegreesToRadians(float value)
{
	return value * (PI / 180.f);
}

static Matrix MakeFromXZ(Vec3 const& XAxis, Vec3 const& ZAxis)
{
	Vec3 NewX = XAxis, Norm = ZAxis;

	normal(NewX);
	normal(Norm);

	// if they're almost same, we need to find arbitrary vector
	if (IsNearlyEqual(abs(dot(NewX, Norm)), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (abs(NewX.z) < (1.f - KINDA_SMALL_NUMBER)) ? Vec3{0, 0, 1.f} : Vec3{1.f, 0, 0};
	}

	Vec3 NewY = cross(Norm, NewX);
	normal(NewY);

	Vec3 NewZ = cross(NewX, NewY);

	Matrix M(4, 4);
	M[0][0] = NewX.x, M[0][1] = NewX.y, M[0][2] = NewX.z, M[0][3] = 0.f;
	M[1][0] = NewY.x, M[1][1] = NewY.y, M[1][2] = NewY.z, M[1][3] = 0.f;
	M[2][0] = NewZ.x, M[2][1] = NewZ.y, M[2][2] = NewZ.z, M[2][3] = 0.f;
	M[3][0] = 0.f,    M[3][1] = 0.f,    M[3][2] = 0.f,    M[3][3] = 1.f;

	return M;
}

static Matrix MakeFromRotator(Rotator r)
{
	float SP, SY, SR;
	float CP, CY, CR;
	SinCos(&SP, &CP, DegreesToRadians(r.Pitch));
	SinCos(&SY, &CY, DegreesToRadians(r.Yaw));
	SinCos(&SR, &CR, DegreesToRadians(r.Roll));

	Matrix M(4, 4);
	M[0][0] = CP * CY;
	M[0][1] = CP * SY;
	M[0][2] = SP;
	M[0][3] = 0.f;

	M[1][0] = SR * SP * CY - CR * SY;
	M[1][1] = SR * SP * SY + CR * CY;
	M[1][2] = -SR * CP;
	M[1][3] = 0.f;

	M[2][0] = -(CR * SP * CY + SR * SY);
	M[2][1] = CY * SR - CR * SP * SY;
	M[2][2] = CR * CP;
	M[2][3] = 0.f;

	M[3][0] = 0.f;
	M[3][1] = 0.f;
	M[3][2] = 0.f;
	M[3][3] = 1.f;

	return M;
}

static Rotator GetRotatorFromMatrix(Matrix& M)
{
	const Vec3 XAxis = Vec3{ M[0][0], M[0][1], M[0][2] };
	const Vec3 YAxis = Vec3{ M[1][0], M[1][1], M[1][2] };
	const Vec3 ZAxis = Vec3{ M[2][0], M[2][1], M[2][2] };

	Rotator	rotator = Rotator(
		Atan2(XAxis.z, sqrt(XAxis.x * XAxis.x + XAxis.y * XAxis.y)) * 180.f / PI,
		Atan2(XAxis.y, XAxis.x) * 180.f / PI,
		0
	);

	Matrix Mx = MakeFromRotator(rotator);
	Vec3 SYAxis = Vec3{ Mx[1][0], Mx[1][1], Mx[1][2] };

	rotator.Roll = Atan2(dot(ZAxis, SYAxis), dot(YAxis, SYAxis)) * 180.f / PI;

	return rotator;
}

Vec3 Rotator::GetForwardVector()
{
	return RotateVector({ 1, 0, 0 });
}

Vec3 Rotator::GetUpVector()
{
	return RotateVector({ 0, 0, 1 });
}

Vec3 Rotator::GetRightVector()
{
	return RotateVector({ 0, 1, 0 });
}

Rotator Rotator::MakeRotator(Vec3 forward, Vec3 up)
{
	Matrix M = MakeFromXZ(forward, up);
	return GetRotatorFromMatrix(M);
}

Vec3 Rotator::RotateVector(const Vec3& V)
{
	Matrix M = MakeFromRotator(*this);

	Point p = Point(V.x, V.y, V.z, 0.f);
	transformVec4(p, M);

	Vec3 ret = p;

	return ret;
}

// 观察坐标系转旋转坐标系（UE4）
static Vec3 view2RotateVec(const Vec3& v)
{
	Vec3 ret;
	ret.x = -1 * v.z;
	ret.y = v.x;
	ret.z = v.y;
	return ret;
}

// 旋转坐标系（UE4）转观察坐标系
static Vec3 rotate2ViewVec(const Vec3& v)
{
	Vec3 ret;
	ret.x = v.y;
	ret.y = v.z;
	ret.z = -1 * v.x;
	return ret;
}

Camera::Camera(Point position, Rotator rotation, float moveSpeed, float wheelSpeed, float middleDrageSpeed, float leftDrageSpeed, float rightDrageSpeed)
{
	Vec3 N = rotate2ViewVec(-1 * (rotation.GetForwardVector()));
	Vec3 V = rotate2ViewVec(rotation.GetUpVector());
	init(position, N, V, moveSpeed, wheelSpeed, middleDrageSpeed, leftDrageSpeed, rightDrageSpeed);
}

Camera::Camera(Point viewP0, Vec3 N, Vec3 V, float moveSpeed, float wheelSpeed, float middleDrageSpeed, float leftDrageSpeed, float rightDrageSpeed)
{
	init(viewP0, N, V, moveSpeed, wheelSpeed, middleDrageSpeed, leftDrageSpeed, rightDrageSpeed);
}

void Camera::setLookPoint(Point lookPoint, float lookDistance)
{
	_lookPoint = lookPoint;
	_lookDistance = lookDistance;
}

void Camera::init(Point viewP0, Vec3 N, Vec3 V, float moveSpeed, float wheelSpeed, float middleDrageSpeed, float leftDrageSpeed, float rightDrageSpeed)
{
	_viewP0 = viewP0;

	_n = N;
	normal(_n);
	_u = cross(V, _n);
	normal(_u);
	_v = cross(_n, _u);

	_moveSpeed = moveSpeed;
	_wheelSpeed = wheelSpeed;
	_middleDrageSpeed = middleDrageSpeed;
	_leftDrageSpeed = leftDrageSpeed;
	_rightDrageSpeed = rightDrageSpeed;

	initInputEvent();

}

void Camera::initInputEvent()
{
	InputManager::GetInstance()->AddMouseEventListen([](int button, InputManager::MouseEvent mouseEvent, int x, int y)
		{
			switch (button)
			{
			case GLUT_LEFT_BUTTON:
				if (mouseEvent == InputManager::MouseEvent::MouseDown)
				{
					camera->onLeftMouseButtonDown(x, y);
				}
				else if (mouseEvent == InputManager::MouseEvent::MouseUp)
				{
					camera->onLeftMouseButtonUp(x, y);
				}
				else if (mouseEvent == InputManager::MouseEvent::MouseDrage)
				{
					camera->onLeftMouseButtonDrage(x, y);
				}
				break;
			case GLUT_RIGHT_BUTTON:
				if (mouseEvent == InputManager::MouseEvent::MouseDown)
				{
					camera->onRightMouseButtonDown(x, y);
				}
				else if (mouseEvent == InputManager::MouseEvent::MouseUp)
				{
					camera->onRightMouseButtonUp(x, y);
				}
				else if (mouseEvent == InputManager::MouseEvent::MouseDrage)
				{
					camera->onRightMouseButtonDrage(x, y);
				}
				break;
			case GLUT_MIDDLE_BUTTON:
				if (mouseEvent == InputManager::MouseEvent::MouseDown)
				{
					camera->onMiddleMouseButtonDown(x, y);
				}
				else if (mouseEvent == InputManager::MouseEvent::MouseUp)
				{
					camera->onMiddleMouseButtonUp(x, y);
				}
				else if (mouseEvent == InputManager::MouseEvent::MouseDrage)
				{
					camera->onMiddleMouseButtonDrage(x, y);
				}
				break;
			case GLUT_WHEEL_UP:
				camera->onMouseWheelUp();
				break;
			case GLUT_WHEEL_DOWN:
				camera->onMouseWheelDown();
				break;
			default:
				break;
			}
		});

	InputManager::GetInstance()->AddKeyEventListen([](unsigned char key, InputManager::KeyEvent keyEvent)
		{
			if (keyEvent == InputManager::KeyInPress)
			{
				switch (key)
				{
				case 'w':
				case 'W':
					camera->on_W_Axis();
					break;
				case 'a':
				case 'A':
					camera->on_A_Axis();
					break;
				case 's':
				case 'S':
					camera->on_S_Axis();
					break;
				case 'd':
				case 'D':
					camera->on_D_Axis();
					break;
				case 'q':
				case 'Q':
					camera->on_Q_Axis();
					break;
				case 'e':
				case 'E':
					camera->on_E_Axis();
					break;
				default:
					break;
				}
			}
			if ((key == 'f' || key == 'F') && keyEvent == InputManager::KeyUp)
			{
				camera->lookAtPoint(camera->_lookPoint, camera->_lookDistance);
			}
		});
}

Rotator Camera::GetCameraRotator()
{
	Vec3 forward = view2RotateVec(-1 * _n);
	Vec3 up = view2RotateVec(_v);

	Rotator r = Rotator::MakeRotator(forward, up);
	return r;
}

void Camera::SetCameraRotator(Rotator r)
{
	_n = rotate2ViewVec(-1 * (r.GetForwardVector()));
	_v = rotate2ViewVec(r.GetUpVector());
	_u = rotate2ViewVec(r.GetRightVector());
}

void Camera::onLeftMouseButtonUp(int x, int y)
{

}

void Camera::onLeftMouseButtonDown(int x, int y)
{

}

void Camera::onLeftMouseButtonDrage(int dx, int dy)
{
	Point focusPoint = _viewP0 - _n * _focus;
	//printf("focusPoint %f %f %f\n", focusPoint.x, focusPoint.y, focusPoint.z);

	Rotator r = GetCameraRotator();
	r.Yaw += dx * _leftDrageSpeed * REVOLVE_X_FACTOR;
	r.Pitch += dy * _leftDrageSpeed * REVOLVE_X_FACTOR;
	r.Pitch = std::max(MIN_PITCH, r.Pitch);
	r.Pitch = std::min(MAX_PITCH, r.Pitch);
	r.Roll = 0;
	SetCameraRotator(r);

	Point p = focusPoint + _n * _focus;
	_viewP0 = p;

	glutPostRedisplay();
}

void Camera::onRightMouseButtonUp(int x, int y)
{

}

void Camera::onRightMouseButtonDown(int x, int y)
{

}

void Camera::onRightMouseButtonDrage(int dx, int dy)
{
	Rotator r = GetCameraRotator();
	r.Yaw += dx * _rightDrageSpeed * ROTATE_X_FACTOR;
	r.Pitch += dy * _rightDrageSpeed * ROTATE_Y_FACTOR;
	r.Pitch = std::max(MIN_PITCH, r.Pitch);
	r.Pitch = std::min(MAX_PITCH, r.Pitch);
	r.Roll = 0;

	SetCameraRotator(r);
	glutPostRedisplay();
}

void Camera::onMiddleMouseButtonUp(int x, int y)
{

}

void Camera::onMiddleMouseButtonDown(int x, int y)
{

}

void Camera::onMiddleMouseButtonDrage(int dx, int dy)
{
	move_dir({ 0, -(float)dy * _middleDrageSpeed * MOUSE_MIDDLE_FACTOR, 0 });
	move_dir({ -(float)dx * _middleDrageSpeed * MOUSE_MIDDLE_FACTOR, 0, 0 });
}

void Camera::onMouseWheelDown()
{
	move_dir({ 0, 0, 1 * _wheelSpeed * MOUSE_WHEEL_FACTOR });
}

void Camera::onMouseWheelUp()
{
	move_dir({ 0, 0, -1 * _wheelSpeed * MOUSE_WHEEL_FACTOR });
}

void Camera::on_W_Axis()
{
	move_dir({0, 0, -1 * _moveSpeed * MOVE_FACTOR });
}

void Camera::on_A_Axis()
{
	move_dir({ -1 * _moveSpeed * MOVE_FACTOR, 0, 0 });
}

void Camera::on_S_Axis()
{
	move_dir({ 0, 0, 1 * _moveSpeed * MOVE_FACTOR });
}

void Camera::on_D_Axis()
{
	move_dir({ 1 * _moveSpeed * MOVE_FACTOR, 0, 0 });
}

void Camera::on_Q_Axis()
{
	move_dir({ 0, -1 * _moveSpeed * MOVE_FACTOR, 0 });
}

void Camera::on_E_Axis()
{
	move_dir({ 0, 1 * _moveSpeed * MOVE_FACTOR, 0 });
}

void Camera::move_dir(Vec3 moveValue)
{
	Vec3 forward = _n * moveValue.z;
	Vec3 up = _v * moveValue.y;
	Vec3 right = _u * moveValue.x;

	_viewP0 = _viewP0 + forward + up + right;

	glutPostRedisplay();
}

void Camera::lookAtPoint(Point p, float focus /*= 0.f*/, float time /*= -1.f*/, void (*callback)() /*= nullptr*/)
{
	if (focus > 0)
		_focus = focus;

	if (time < 0)
		time = 0.5;

	Point mp = p + _focus * _n;

	moveToPoint(mp, time, callback);
}

void Camera::moveToPoint(Point p, float time, void (*callback)())
{
	moveCurTime = 0.f;
	moveTotalTime = time;
	moveStartPoint = _viewP0;
	moveTargetPoint = p;
	moveCallback = callback;
	moving = true;
}

void Camera::update()
{
	auto curTick = clock();
	float dt = (float)(curTick - _lastTick) / CLOCKS_PER_SEC;
	tick(dt);
	_lastTick = curTick;
}

void Camera::tick(float dt)
{
	if (!moving)
		return;

	moveCurTime = moveCurTime + dt;
	if (moveCurTime >= moveTotalTime)
	{
		_viewP0 = moveTargetPoint;
		glutPostRedisplay();

		if (moveCallback)
			moveCallback();

		moveCurTime = 0.f;
		moveTotalTime = 0.f;
		moveStartPoint = Point(0.f, 0.f, 0.f);
		moveTargetPoint = Point(0.f, 0.f, 0.f);
		moveCallback = nullptr;
		moving = false;
		return;
	}

	float percent = moveCurTime / moveTotalTime;
	Point p = moveStartPoint + percent * (moveTargetPoint - moveStartPoint);
	_viewP0 = p;
	glutPostRedisplay();
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
	float clipWinWidth = winWidth / winHeight * clipWinHeight;

	glFrustum(-clipWinWidth / 2, clipWinWidth / 2, -clipWinHeight / 2, clipWinHeight / 2, dnear, dfar);
	//glFrustum(-winWidth / 4, winWidth / 4, -winHeight / 2, winHeight / 2, winHeight / 2 / tan(45.f / 2 * PI / 180), dfar);
	//gluPerspective(45.f, (GLfloat)winWidth / 2 / (GLfloat)winHeight, dnear, dfar);
	glViewport(0, 0, winWidth, winHeight);
}

void _displayFcn(void)
{
	viewOpenGL();
	if (displayFcn)
	{
		displayFcn();
	}
}

void cameraDisplayFunc(void (*fcn)(void), Camera* _camera, float width, float height)
{
	winWidth = width;
	winHeight = height;
	displayFcn = fcn;
	glutDisplayFunc(_displayFcn);
	camera = _camera;
}

#endif

