// 三维相机代码
#pragma once

#ifdef OPENGL_CAMERA

class Vec3;
class Point
{
public:
	GLfloat x, y, z, h;
	Point(float x = 0.f, float y = 0.f, float z = 0.f, float h = 1.f) :x(x), y(y), z(z), h(h) {}
	operator Vec3();
};

class Vec3
{
public:
	GLfloat x, y, z;
	operator Point()
	{
		return Point(x,y,z);
	}
};

class Rotator
{
public:
	float Pitch;
	float Yaw;
	float Roll;

public:
	Rotator(float Pitch, float Yaw, float Roll) : Pitch(Pitch), Yaw(Yaw), Roll(Roll) {};
	
	Vec3 GetForwardVector();
	Vec3 GetUpVector();
	Vec3 GetRightVector();
	static Rotator MakeRotator(Vec3 forward, Vec3 up);

private:
	Vec3 RotateVector(const Vec3& V);
};

class Camera
{
public:
	Camera(Point position = Point(0.f, 0.f, 0.f), Rotator rotation = Rotator(0.f, 0.f, 0.f), float moveSpeed = 1.f, float wheelSpeed = 1.f, float middleDrageSpeed = 1.f, float leftDrageSpeed = 1.f, float rightDrageSpeed = 1.f);
	Camera(Point viewP0, Vec3 N, Vec3 V, float moveSpeed = 1.f, float wheelSpeed = 1.f, float middleDrageSpeed = 1.f, float leftDrageSpeed = 1.f, float rightDrageSpeed = 1.f);
	void setLookPoint(Point lookPoint, float lookDistance = 100.f);
	void update();

public:
	Point _viewP0;
	Vec3 _u, _v, _n;

private:
	int _curPressedButton = -1;
	int _lastX, _lastY;
	float _focus = 100.f;
	float _lastTick = 0.f;
	Point _lookPoint = Point(0.f, 0.f, 0.f);
	float _lookDistance = 0.f;

	float _moveSpeed = 1.f;
	float _wheelSpeed = 1.f;
	float _middleDrageSpeed = 1.f;
	float _leftDrageSpeed = 1.f;
	float _rightDrageSpeed = 1.f;
	
	bool moving = false;
	float moveCurTime = 0.f;
	float moveTotalTime = 0.f;
	Point moveStartPoint = Point(0.f, 0.f, 0.f);
	Point moveTargetPoint = Point(0.f, 0.f, 0.f);
	void (*moveCallback)() = nullptr;

	void init(Point viewP0, Vec3 N, Vec3 V, float moveSpeed, float wheelSpeed, float middleDrageSpeed, float leftDrageSpeed, float rightDrageSpeed);
	void initInputEvent();

	Rotator GetCameraRotator();
	void SetCameraRotator(Rotator r);

	void onLeftMouseButtonDown(int x, int y);
	void onLeftMouseButtonUp(int x, int y);
	void onLeftMouseButtonDrage(int dx, int dy);
	void onRightMouseButtonDown(int x, int y);
	void onRightMouseButtonUp(int x, int y);
	void onRightMouseButtonDrage(int dx, int dy);
	void onMiddleMouseButtonDown(int x, int y);
	void onMiddleMouseButtonUp(int x, int y);
	void onMiddleMouseButtonDrage(int dx, int dy);
	void onMouseWheelDown();
	void onMouseWheelUp();

	void on_W_Axis();
	void on_A_Axis();
	void on_S_Axis();
	void on_D_Axis();
	void on_Q_Axis();
	void on_E_Axis();
	void move_dir(Vec3 dir);
	void lookAtPoint(Point p, float focus = 0.f, float time = -1.f, void (*callback)() = nullptr);
	void moveToPoint(Point p, float time, void (*callback)());
	void tick(float dt);
};

void cameraDisplayFunc(void (*displayFcn)(void), Camera* camera, float width, float height);

#endif
//////////////////////////////////////////////////////////////////////////