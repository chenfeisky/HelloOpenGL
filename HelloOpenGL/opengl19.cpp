#include "common.h"

#if CHAPTER==19
#include "opengl19h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_19_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;
#endif

#ifdef CHAPTER_19_7_3
class rgbSpace { public: float r = 0, g = 0, b = 0; };
class hsvSpace { public: float h = 0, s = 0, v = 0; };
const float noHue = -1.0;
inline float min(float a, float b) { return (a < b) ? a : b; }
inline float max(float a, float b) { return (a > b) ? a : b; }
void rgbTOhsv(rgbSpace& rgb, hsvSpace& hsv)
{
	float minRGB = min(rgb.r, min(rgb.g, rgb.b)), maxRGB = max(rgb.r, max(rgb.g, rgb.b));
	float deltaRGB = maxRGB - minRGB;
	
	hsv.v = maxRGB;
	
	if (maxRGB != 0.0)
		hsv.s = deltaRGB / maxRGB;
	else
		hsv.s = 0.0;

	if (hsv.s <= 0.0)
		hsv.h = noHue;
	else
	{
		if (rgb.r == maxRGB)
			hsv.h = (rgb.g - rgb.b) / deltaRGB;
		else
		{
			if (rgb.g == maxRGB)
				hsv.h = 2.0 + (rgb.b - rgb.r) / deltaRGB;
			else
			{
				if (rgb.b == maxRGB)
					hsv.h = 4.0 + (rgb.r - rgb.g) / deltaRGB;
			}
		}

		hsv.h *= 60.0;
		if (hsv.h < 0.0)
			hsv.h += 360.0;
		hsv.h /= 360.0;
	}
}

void hsvTOrgb(hsvSpace& hsv, rgbSpace& rgb)
{
	int k;
	float aa, bb, cc, f;
	if (hsv.s <= 0.0)
		rgb.r = rgb.g = rgb.b = hsv.v;
	else
	{
		if (hsv.h == 1.0)
			hsv.h = 0.0;
		hsv.h *= 6.0;
		k = floor(hsv.h);
		f = hsv.h - k;
		aa = hsv.v * (1.0 - hsv.s);
		bb = hsv.v * (1.0 - (hsv.s * f));
		cc = hsv.v * (1.0 - (hsv.s * (1.0 - f)));
		switch (k)
		{
			case 0: rgb.r = hsv.v; rgb.g = cc; rgb.b = aa; break;
			case 1: rgb.r = bb; rgb.g = hsv.v; rgb.b = aa; break;
			case 2: rgb.r = aa; rgb.g = hsv.v; rgb.b = cc; break;
			case 3: rgb.r = aa; rgb.g = bb; rgb.b = hsv.v; break;
			case 4: rgb.r = cc; rgb.g = aa; rgb.b = hsv.v; break;
			case 5: rgb.r = hsv.v; rgb.g = aa; rgb.b = bb; break;
		}
	}
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	{
		rgbSpace rgb{ 1, 1, 0 };
		glColor3f(rgb.r, rgb.g, rgb.b);
		glBegin(GL_POLYGON);
		glVertex3f(-30, -5, 0);
		glVertex3f(-20, -5, 0);
		glVertex3f(-20, 5, 0);
		glVertex3f(-30, 5, 0);
		glEnd();

		hsvSpace hsv;
		rgbTOhsv(rgb, hsv);
		rgbSpace rgb1;
		hsvTOrgb(hsv, rgb1);
		glColor3f(rgb1.r, rgb1.g, rgb1.b);
		glBegin(GL_POLYGON);
		glVertex3f(-5, -5, 0);
		glVertex3f(5, -5, 0);
		glVertex3f(5, 5, 0);
		glVertex3f(-5, 5, 0);
		glEnd();
	}

	{
		hsvSpace hsv{ 4.f / 6, 1, 1 };
		rgbSpace rgb;
		hsvTOrgb(hsv, rgb);
		glColor3f(rgb.r, rgb.g, rgb.b);
		glBegin(GL_POLYGON);
		glVertex3f(20, -5, 0);
		glVertex3f(30, -5, 0);
		glVertex3f(30, 5, 0);
		glVertex3f(20, 5, 0);
		glEnd();
	}

	glFlush();
}

void code_19_7_3()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
}
#endif


//////////////////////////////////////////////////////////////////////////
// CHAPTER_19_COMMON

#ifdef CHAPTER_19_COMMON
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // 黑色背景色
	glColor3f(1.0, 1.0, 1.0); // 白色绘制
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(widthMin, widthMax, heightMin, heightMax);
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
	int glewCode = glewInit();
	if (glewCode != GLEW_OK)
	{
		printf("glewInit err:%d\n", glewCode);
		return;
	}

	init();

#ifdef CHAPTER_19_7_3
	code_19_7_3();
#endif

	glutMainLoop();
}
#endif


#endif 


