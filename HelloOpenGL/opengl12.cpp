#include "common.h"

#if CHAPTER==12
#include "opengl12h.h"

#ifdef CHAPTER_12_COMMON
float winWidth = 800, winHeight = 600;
#endif

#ifdef CHAPTER_12_10
const double TWO_PI = 6.2831853;
GLsizei winWidth = 500, winHeight = 500;
GLuint regHex;
static GLfloat rotTheta = 0.0;
static clock_t lastTick = 0.0;
class scrPt
{
	public :
		GLint x, y;
};

static void init(void)
{
	scrPt hexVertex;
	GLdouble hexTheta;
	GLint k;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	regHex = glGenLists(1);
	glNewList(regHex, GL_COMPILE);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (k = 0; k < 6; k++)
	{
		hexTheta = TWO_PI * k / 6;
		hexVertex.x = 150 + 100 * cos(hexTheta);
		hexVertex.y = 150 + 100 * sin(hexTheta);
		glVertex2i(hexVertex.x, hexVertex.y);
	}
	glEnd();
	glEndList();
}

void displayHex(void)
{
	auto curTick = clock();
	float dt = (float)(curTick - lastTick) / CLOCKS_PER_SEC;
	printf("%f\n", dt);

	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glRotatef(-rotTheta, 0.0, 0.0, 1.0);
	glCallList(regHex);
	glPopMatrix();

	glutSwapBuffers();

	glFlush();

	lastTick = curTick;
}

void rotateHex(void)
{
	rotTheta += 3.0;
	if (rotTheta > 360.0)
		rotTheta -= 360.0;

	glutPostRedisplay();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-320.0, 320.0, -320.0, 320.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClear(GL_COLOR_BUFFER_BIT);
}

void mouseFcn(GLint button, GLint action, GLint x, GLint y)
{
	switch (button)
	{
	case GLUT_MIDDLE_BUTTON:
		if (action == GLUT_DOWN)
			glutIdleFunc(rotateHex);
		break;
	case GLUT_RIGHT_BUTTON:
		if (action == GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void main(int argc, char** argv)
{
	lastTick = clock();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Animation Example");

	init();
	glutDisplayFunc(displayHex);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mouseFcn);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_12_1_2
static clock_t lastTick = 0.0;
bool run = false;
class Point
{
public:
	GLfloat x, y;
};
typedef Point Vector;
void update(void);
float runTime = 0.0;
Point origPoint{ 130, 220 };
Vector speed{ 100, 0 };
float allTime = 5;
Point curPoint = origPoint;
GLubyte colorShape[30 * 50 * 3] = { 0 };
//GLubyte colorShape[30 * 50 * 3 + 50 * 10] = { 0 };
void begin()
{
	run = true;
	runTime = 0;
	lastTick = clock();
	glutPostRedisplay();
	glutIdleFunc(update);
}
void finish()
{
	run = false;
	lastTick = 0;
	glutIdleFunc(NULL);
}
void pause()
{
	run = false;
	glutIdleFunc(NULL);
}
void resume()
{
	run = true;
	lastTick = clock();
	glutIdleFunc(update);
}
bool test = false;
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT); 
	// 注意这里必须清除全部内容，而不能只清除上一次的绘制，因为glutSwapBuffers将上一次的前台内容交换到了
	// 现在后台，故形成了残留，必须全部清除

	curPoint.x = origPoint.x + runTime * speed.x;
	curPoint.y = origPoint.y + runTime * speed.y;
	
	glRasterPos2f(curPoint.x, curPoint.y);
	memset(colorShape, 0xFF, sizeof(colorShape));
	glDrawPixels(30, 50, GL_RGB, GL_UNSIGNED_BYTE, colorShape);

	glutSwapBuffers();

	glFlush();

}

void update(void)
{
	auto curTick = clock();
	float dt = (float)(curTick - lastTick) / CLOCKS_PER_SEC;
	//printf("%f\n", dt);

	runTime += dt;

	glutPostRedisplay();

	if (runTime >= allTime)
	{
		runTime = allTime;
		finish();
	}

	lastTick = curTick;
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

	glClear(GL_COLOR_BUFFER_BIT);
}

void mouseFcn(GLint button, GLint action, GLint x, GLint y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (action == GLUT_UP)
		{
			begin();
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (action == GLUT_UP)
		{
			if (run)
				pause();
			else
				resume();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (action == GLUT_UP)
		{
			finish();
		}
		break;
	default:
		break;
	}
}

void code_12_1_2()
{
	// 这里用1字节对齐，确保colorShape中每一行的数据刚好都能被正确读取。
	// 若采用的默认值（4），则colorShape必须设为大于30 * 50 * 3 + 50 * 2，这样每行多2个字节，才能确保被正确读取。
	// 因为原本每行30 * 3个字节，90 / 4除不尽，将错误的读取下一行的数据。而现在设为每行92个自己，将刚好被4整除，故能正确的读取。
	// 当然每行大于92个字节也没问题。
	// 特别注意：本例中没有用glReadPixels，故采用哪种都无所谓。但若使用glReadPixels，则由于glReadPixels使用的
	// 是4字节对齐，故必须glPixelStorei(GL_UNPACK_ALIGNMENT, 4)，相应的colorShape必须设为大于30 * 50 * 3 + 50 * 2
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 
	
	glClearColor(0.0, 0.0, 0.0, 0.0); // 黑色背景色

	lastTick = clock();
	
	glutDisplayFunc(displayFcn);
	//glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mouseFcn);
}
#endif

#ifdef CHAPTER_12_6_1
const double TWO_PI = 6.2831853;
GLuint regHex;
static GLfloat rotTheta = 0.0;
static clock_t lastTick = 0.0;
int curKeyFrameIdx = 0;
bool run = false;
class Point
{
public:
	GLfloat x, y;
};
typedef Point Vector;
class Bone
{
public:
	std::vector<Point> points;
};
class Frame
{
public:
	std::vector<Bone> bones;
};
class keyFrame : public Frame
{
public:
	float time;
};
class Animation
{
public:
	std::vector<keyFrame> keyFrames;
};
void update(void);
float runTime = 0.0;
bool preDeal = false;
class scrPt
{
public:
	GLint x, y;
};
void begin()
{
	run = true;
	preDeal = false;
	runTime = 0;
	curKeyFrameIdx = 0;
	lastTick = clock();
	glutIdleFunc(update);
}
void finish()
{
	run = false;
	lastTick = 0;
	glutIdleFunc(NULL);
}
void pause()
{
	run = false;
	glutIdleFunc(NULL);
}
void resume()
{
	run = true;
	lastTick = clock();
	glutIdleFunc(update);
}
void drawFrame(const Frame& frame)
{
	for (int i = 0; i < frame.bones.size(); i++)
	{
		auto const & bone = frame.bones[i];
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < bone.points.size(); j++)
		{
			glVertex2i(bone.points[j].x, bone.points[j].y);
		}
		glEnd();
	}
}
void preDealFrame(Frame* frame1, Frame* frame2)
{
	int boneNum1 = frame1->bones.size();
	int boneNum2 = frame2->bones.size();
	if (boneNum1 == boneNum2)
		return;

	Frame* frameMax = frame1;
	Frame* frameMin = frame2;
	if (boneNum1 < boneNum2)
	{
		frameMax = frame2;
		frameMin = frame1;
	}
	int boneNumMax = frameMax->bones.size();
	int boneNumMin = frameMin->bones.size();

	int Ne = boneNumMax % boneNumMin;
	int Ns = boneNumMax / boneNumMin;

	int j = 0;
	std::vector<Bone> newBones;
	for (int i = 0; i < boneNumMin; i++)
	{
		if (i < Ne) // 前Ne条边
		{
			auto& bone = frameMin->bones[i];
			if (Ne + 1 <= 1)
			{
				newBones.push_back(bone);
			}
			else
			{
				std::vector<float> weight;
				float sum = 0;
				for (int k = j; k < j + Ns + 1; k++)
				{
					float l = sqrt(pow(frameMax->bones[k].points[1].x - frameMax->bones[k].points[0].x, 2)
						+ pow(frameMax->bones[k].points[1].y - frameMax->bones[k].points[0].y, 2));
					weight.push_back(l);
					sum += l;
				}
				j += Ns + 1;

				float dx = bone.points[1].x - bone.points[0].x;
				float dy = bone.points[1].y - bone.points[0].y;
				float curPercent = 0;
				for (int m = 0; m < weight.size(); m++)
				{
					float nextPercent = curPercent + weight[m] / sum;
					newBones.push_back({ {
						{bone.points[0].x + dx * curPercent, bone.points[0].y + dy * curPercent},
						{bone.points[0].x + dx * nextPercent, bone.points[0].y + dy * nextPercent}} });
					curPercent = nextPercent;
				}
			}			
		}
		else
		{
			auto& bone = frameMin->bones[i];
			if (Ns <= 1)
			{
				newBones.push_back(bone);
			}
			else
			{
				std::vector<float> weight;
				float sum = 0;
				for (int k = j; k < j + Ns; k++)
				{
					float l = sqrt(pow(frameMax->bones[k].points[1].x - frameMax->bones[k].points[0].x, 2)
						+ pow(frameMax->bones[k].points[1].y - frameMax->bones[k].points[0].y, 2));
					weight.push_back(l);
					sum += l;
				}
				j += Ns;

				float dx = bone.points[1].x - bone.points[0].x;
				float dy = bone.points[1].y - bone.points[0].y;
				float curPercent = 0;
				for (int m = 0; m < weight.size(); m++)
				{
					float nextPercent = curPercent + weight[m] / sum;
					newBones.push_back({ {
						{bone.points[0].x + dx * curPercent, bone.points[0].y + dy * curPercent},
						{bone.points[0].x + dx * nextPercent, bone.points[0].y + dy * nextPercent}} });
					curPercent = nextPercent;
				}
			}
		}
	}

	frameMin->bones = newBones;
}
Animation walkAnimation1()
{
	keyFrame kf1;
	kf1.bones = {
		{{ {42, 363},{42, 300} }},
		{{ {41, 341},{19, 312} }},
		{{ {41, 341},{64, 316} }},
		{{ {42, 300},{31, 259} }},
		{{ {42, 300},{55, 259} }},
	};
	kf1.time = 0;

	keyFrame kf2;
	kf2.bones = {
		{{ {50, 363},{50, 300} }},
		{{ {50, 344},{70, 323} }},
		{{ {50, 344},{39, 314} }},
		{{ {50, 300},{63, 259} }},
		{{ {50, 300},{39, 259} }},
	};
	kf2.time = 0.5;

	keyFrame kf3;
	kf3.bones = {
		{{ {58, 363},{58, 300} }},
		{{ {58, 344},{47, 314} }},
		{{ {58, 344},{78, 323} }},
		{{ {58, 300},{47, 259} }},
		{{ {58, 300},{71, 259} }},
	};
	kf3.time = 1;

	keyFrame kf4;
	kf4.bones = {
		{{ {64, 363},{64, 300} }},
		{{ {64, 344},{86, 323} }},
		{{ {64, 344},{55, 314} }},
		{{ {64, 300},{79, 259} }},
		{{ {64, 300},{55, 259} }},
	};
	kf4.time = 1.5;

	keyFrame kf5;
	kf5.bones = {
		{{ {72, 363},{72, 300} }},
		{{ {72, 344},{63, 314} }},
		{{ {72, 344},{94, 323} }},
		{{ {72, 300},{63, 259} }},
		{{ {72, 300},{87, 259} }},
	};
	kf5.time = 2;

	keyFrame kf6;
	kf6.bones = {
		{{ {80, 363},{80, 300} }},
		{{ {80, 344},{102, 323} }},
		{{ {80, 344},{71, 314} }},
		{{ {80, 300},{95, 259} }},
		{{ {80, 300},{71, 259} }},
	};
	kf6.time = 2.5;

	keyFrame kf7;
	kf7.bones = {
		{{ {88, 363},{88, 300} }},
		{{ {88, 344},{79, 314} }},
		{{ {88, 344},{110, 323} }},
		{{ {88, 300},{79, 259} }},
		{{ {88, 300},{103, 259} }},
	};
	kf7.time = 3;

	keyFrame kf8;
	kf8.bones = {
		{{ {96, 363},{96, 300} }},
		{{ {96, 344},{118, 323} }},
		{{ {96, 344},{87, 314} }},
		{{ {96, 300},{111, 259} }},
		{{ {96, 300},{87, 259} }},
	};
	kf8.time = 3.5;

	keyFrame kf9;
	kf9.bones = {
		{{ {104, 363},{104, 300} }},
		{{ {104, 344},{95, 314} }},
		{{ {104, 344},{126, 323} }},
		{{ {104, 300},{95, 259} }},
		{{ {104, 300},{119, 259} }},
	};
	kf9.time = 4;

	keyFrame kf10;
	kf10.bones = {
		{{ {112, 363},{112, 300} }},
		{{ {112, 344},{134, 323} }},
		{{ {112, 344},{103, 314} }},
		{{ {112, 300},{127, 259} }},
		{{ {112, 300},{103, 259} }},
	};
	kf10.time = 4.5;

	Animation ani;
	ani.keyFrames = { kf1, kf2, kf3, kf4, kf5, kf6, kf7, kf8, kf9, kf10 };
	return ani;
}

Animation walkAnimation2()
{
	float sp = 20;
	keyFrame kf1;
	kf1.bones = {
		{{ {65, 450},{65, 308} }}, // 躯干

		{{ {65, 405},{40, 333} }}, // 手1
		{{ {65, 405},{123, 346} }}, // 手2

		{{ {65, 307},{53, 224} }}, // 脚1
		{{ {53, 224},{33, 167} }}, // 脚1

		{{ {65, 307},{98, 229} }}, // 脚2
		{{ {98, 229},{123, 167} }}, // 脚2
	};
	kf1.time = 0;

	keyFrame kf2;
	kf2.bones = {
		{{ {65 + 1 * sp, 450},{65 + 1 * sp, 308} }},

		{{ {65 + 1 * sp, 405},{54 + 1 * sp, 328} }},
		{{ {65 + 1 * sp, 405},{71 + 1 * sp, 329} }},

		{{ {65 + 1 * sp, 307},{65 + 1 * sp, 228} }},
		{{ {65 + 1 * sp, 228},{27 + 1 * sp, 185} }},

		{{ {65 + 1 * sp, 307},{65 + 1 * sp, 228} }},
		{{ {65 + 1 * sp, 228},{65 + 1 * sp, 167} }},
	};
	kf2.time = 0.5;

	keyFrame kf3;
	kf3.bones = {
		{{ {65 + 2 * sp, 450},{65 + 2 * sp, 308} }},

		{{ {65 + 2 * sp, 405},{123 + 2 * sp, 346} }},
		{{ {65 + 2 * sp, 405},{19 + 2 * sp, 339} }},

		{{ {65 + 2 * sp, 307},{98 + 2 * sp, 229} }},
		{{ {98 + 2 * sp, 229},{123 + 2 * sp, 167} }},

		{{ {65 + 2 * sp, 307},{41 + 2 * sp, 224} }},
		{{ {41 + 2 * sp, 224},{30 + 2 * sp, 167} }},
	};
	kf3.time = 1;

	keyFrame kf4;
	kf4.bones = {
		{{ {65 + 3 * sp, 450},{65 + 3 * sp, 308} }},

		{{ {65 + 3 * sp, 405},{65 + 3 * sp, 321} }},
		{{ {65 + 3 * sp, 405},{65 + 3 * sp, 321} }},

		{{ {65 + 3 * sp, 307},{65 + 3 * sp, 228} }},
		{{ {65 + 3 * sp, 228},{65 + 3 * sp, 167} }},

		{{ {65 + 3 * sp, 307},{88 + 3 * sp, 229} }},
		{{ {88 + 3 * sp, 229},{50 + 3 * sp, 175} }},
	};
	kf4.time = 1.5;

	keyFrame kf5;
	kf5.bones = {
		{{ {65 + 4 * sp, 450},{65 + 4 * sp, 308} }},

		{{ {65 + 4 * sp, 405},{40 + 4 * sp, 333} }},
		{{ {65 + 4 * sp, 405},{123 + 4 * sp, 346} }},

		{{ {65 + 4 * sp, 307},{53 + 4 * sp, 224} }},
		{{ {53 + 4 * sp, 224},{33 + 4 * sp, 167} }},

		{{ {65 + 4 * sp, 307},{98 + 4 * sp, 229} }},
		{{ {98 + 4 * sp, 229},{123 + 4 * sp, 167} }},
	};
	kf5.time = 2;

	keyFrame kf6;
	kf6.bones = {
		{{ {65 + 5 * sp, 450},{65 + 5 * sp, 308} }},

		{ { {65 + 5 * sp, 405},{54 + 5 * sp, 328} }},
		{{ {65 + 5 * sp, 405},{71 + 5 * sp, 329} }},

		{ { {65 + 5 * sp, 307},{65 + 5 * sp, 228} }},
		{{ {65 + 5 * sp, 228},{27 + 5 * sp, 185} }},

		{ { {65 + 5 * sp, 307},{65 + 5 * sp, 228} }},
		{{ {65 + 5 * sp, 228},{65 + 5 * sp, 167} }},
	};
	kf6.time = 2.5;

	keyFrame kf7;
	kf7.bones = {
		{{ {65 + 6 * sp, 450},{65 + 6 * sp, 308} }},

		{{ {65 + 6 * sp, 405},{123 + 6 * sp, 346} }},
		{{ {65 + 6 * sp, 405},{19 + 6 * sp, 339} }},

		{{ {65 + 6 * sp, 307},{98 + 6 * sp, 229} }},
		{{ {98 + 6 * sp, 229},{123 + 6 * sp, 167} }},

		{{ {65 + 6 * sp, 307},{41 + 6 * sp, 224} }},
		{{ {41 + 6 * sp, 224},{30 + 6 * sp, 167} }},
	};
	kf7.time = 3;

	keyFrame kf8;
	kf8.bones = {
		{{ {65 + 7 * sp, 450},{65 + 7 * sp, 308} }},

		{{ {65 + 7 * sp, 405},{65 + 7 * sp, 321} }},
		{{ {65 + 7 * sp, 405},{65 + 7 * sp, 321} }},

		{{ {65 + 7 * sp, 307},{65 + 7 * sp, 228} }},
		{{ {65 + 7 * sp, 228},{65 + 7 * sp, 167} }},

		{{ {65 + 7 * sp, 307},{88 + 7 * sp, 229} }},
		{{ {88 + 7 * sp, 229},{50 + 7 * sp, 175} }},
	};
	kf8.time = 3.5;

	keyFrame kf9;
	kf9.bones = {
		{{ {65 + 8 * sp, 450},{65 + 8 * sp, 308} }},

		{{ {65 + 8 * sp, 405},{40 + 8 * sp, 333} }},
		{{ {65 + 8 * sp, 405},{123 + 8 * sp, 346} }},

		{{ {65 + 8 * sp, 307},{53 + 8 * sp, 224} }},
		{{ {53 + 8 * sp, 224},{33 + 8 * sp, 167} }},

		{{ {65 + 8 * sp, 307},{98 + 8 * sp, 229} }},
		{{ {98 + 8 * sp, 229},{123 + 8 * sp, 167} }},
	};
	kf9.time = 4;

	Animation ani;
	ani.keyFrames = { kf1, kf2, kf3, kf4, kf5, kf6, kf7, kf8, kf9 };
	return ani;
}
Animation changeShapeAnimation()
{
	keyFrame kf1;
	kf1.bones = {
		{{ {86, 240},{143, 377} }},
		{{ {143, 377},{173,145 } }}, 
		{{ {173,145},{86, 240} }},
	};
	kf1.time = 0;

	keyFrame kf2;
	kf2.bones = {
		{{ {268, 363},{293, 494} }},
		{{ {293, 494},{471, 434} }},
		{{ {471, 434},{379, 339} }},
		{{ {379, 339},{268, 363} }},
	};
	kf2.time = 1;

	keyFrame kf3;
	kf3.bones = {
		{{ {227, 151},{330, 248} }},
		{{ {330, 248},{387, 226} }},
		{{ {387, 226},{378, 184} }},
		{{ {378, 184},{505, 111} }},
		{{ {505, 111},{377, 34} }},
		{{ {377, 34},{276, 50} }},
		{{ {276, 50},{299, 97} }},
		{{ {299, 97},{227, 151} }},
	};
	kf3.time = 2;

	keyFrame kf4;
	kf4.bones = {
		{{ {510, 342},{732, 375} }},
	};
	kf4.time = 3;

	keyFrame kf5;
	kf5.bones = {
		{{ {622, 177}, {622, 177}}},
	};
	kf5.time = 4;

	Animation ani;
	ani.keyFrames = { kf1, kf2, kf3, kf4, kf5};
	return ani;
}

keyFrame _kf1, _kf2;
Animation ani;
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// 初始位置
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < ani.keyFrames.size(); i++)
	{
		//drawFrame(ani.keyFrames[i]);
	}
		   
	glColor3f(1.0, 1.0, 1.0);

	if (curKeyFrameIdx < ani.keyFrames.size() - 1)
	{
		if (!preDeal)
		{
			_kf1 = ani.keyFrames[curKeyFrameIdx];
			_kf2 = ani.keyFrames[curKeyFrameIdx + 1];
			preDealFrame(&_kf1, &_kf2);
			preDeal = true;
		}			

		if (runTime < _kf2.time)
		{
			Frame f;

			float t = _kf2.time - _kf1.time;
			for (int j = 0; j < _kf1.bones.size(); j++)
			{
				auto& b1 = _kf1.bones[j];
				assert(j < _kf2.bones.size());
				auto& b2 = _kf2.bones[j];

				f.bones.push_back({});
				for (int k = 0; k < b1.points.size(); k++)
				{
					auto& p1 = b1.points[k];
					assert(k < b2.points.size());
					auto& p2 = b2.points[k];

					Vector v = { p2.x - p1.x, p2.y - p1.y };
					float speedx = v.x / t;
					float speedy = v.y / t;

					float _t = runTime - _kf1.time;
					f.bones.back().points.push_back({ p1.x + speedx * _t, p1.y + speedy * _t });
				}
			}
			drawFrame(f);
		}
		else
		{
			drawFrame(_kf2);
			runTime = _kf2.time;

			curKeyFrameIdx++;
			preDeal = false;
		}
	}
	else
	{
		curKeyFrameIdx = ani.keyFrames.size() - 1;
		auto& kf = ani.keyFrames[curKeyFrameIdx];
		drawFrame(kf);
		runTime = kf.time;
		finish();
	}
	glutSwapBuffers();

	glFlush();

}
void update(void)
{
	auto curTick = clock();
	float dt = (float)(curTick - lastTick) / CLOCKS_PER_SEC;
	//printf("%f\n", dt);

	runTime += dt;

	glutPostRedisplay();

	lastTick = curTick;
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

	glClear(GL_COLOR_BUFFER_BIT);
}

void mouseFcn(GLint button, GLint action, GLint x, GLint y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (action == GLUT_UP)
		{
			begin();
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (action == GLUT_UP)
		{
			if (run)
				pause();
			else
				resume();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (action == GLUT_UP)
		{
			finish();
		}			
		break;
	default:
		break;
	}
}

void code_12_6_1()
{
	lastTick = clock();

	// 简单行走动画
	// ani = walkAnimation1();

	// 骨骼行走动画
	ani = walkAnimation2();

	// 变形动画
	// ani = changeShapeAnimation();

	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mouseFcn);
}

#endif

#ifdef CHAPTER_12_6_2
const double TWO_PI = 6.2831853;
GLuint regHex;
static GLfloat rotTheta = 0.0;
static clock_t lastTick = 0.0;
bool run = false;
class Point
{
public:
	GLfloat x, y;
};
typedef Point Vector;
class Bone
{
public:
	std::vector<Point> points;
};
class Frame
{
public:
	std::vector<Bone> bones;
};
class keyFrame : public Frame
{
public:
	float time;
};
class Animation
{
public:
	std::vector<keyFrame> keyFrames;
};
void update(void);
float runTime = 0.0;
int frameCount = 30;
int curframeNum = 1;
class scrPt
{
public:
	GLint x, y;
};
void begin()
{
	run = true;
	runTime = 0;
	curframeNum = 1;
	lastTick = clock();
	glutPostRedisplay();
	glutIdleFunc(update);
}
void finish()
{
	run = false;
	lastTick = 0;
	glutIdleFunc(NULL);
}
void pause()
{
	run = false;
	glutIdleFunc(NULL);
}
void resume()
{
	run = true;
	lastTick = clock();
	glutIdleFunc(update);
}
void drawFrame(const Frame& frame)
{
	for (int i = 0; i < frame.bones.size(); i++)
	{
		auto const & bone = frame.bones[i];
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < bone.points.size(); j++)
		{
			glVertex2i(bone.points[j].x, bone.points[j].y);
		}
		glEnd();
	}
}
Animation moveAnimation()
{
	keyFrame kf1;
	kf1.bones = {
		{{ {60, 250},{160, 250} }},
		{{ {160, 250},{160,320 } }},
		{{ {160,320},{60, 320} }},
		{{ {60,320},{60, 250} }},
	};
	kf1.time = 0;

	keyFrame kf2;
	kf2.bones = {
		{{ {650, 250},{750, 250} }},
		{{ {750, 250},{750,320 } }},
		{{ {750,320},{650, 320} }},
		{{ {650,320},{650, 250} }},
	};
	kf2.time = 3;

	Animation ani;
	ani.keyFrames = { kf1, kf2};
	return ani;
}

Animation ani;
keyFrame _kf1, _kf2;
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// 初始位置
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < ani.keyFrames.size(); i++)
	{
		//drawFrame(ani.keyFrames[i]);
	}

	glColor3f(1.0, 1.0, 1.0);

	_kf1 = ani.keyFrames[0];
	_kf2 = ani.keyFrames[1];

	Frame f;

	float t = _kf2.time - _kf1.time;
	for (int j = 0; j < _kf1.bones.size(); j++)
	{
		auto& b1 = _kf1.bones[j];
		assert(j < _kf2.bones.size());
		auto& b2 = _kf2.bones[j];

		f.bones.push_back({});
		for (int k = 0; k < b1.points.size(); k++)
		{
			auto& p1 = b1.points[k];
			assert(k < b2.points.size());
			auto& p2 = b2.points[k];

			Vector v = { p2.x - p1.x, p2.y - p1.y };
			float speedx = v.x / t;
			float speedy = v.y / t;

			float _t = runTime - _kf1.time;
			f.bones.back().points.push_back({ p1.x + speedx * _t, p1.y + speedy * _t });
		}
	}
	drawFrame(f);
		
	glutSwapBuffers();

	glFlush();

}

float getNextFrameTimeByLiner()
{
	float frame_dt = (ani.keyFrames[1].time - ani.keyFrames[0].time) / (frameCount + 1);
	float t = ani.keyFrames[0].time + curframeNum * frame_dt;
	return t;
}
float getNextFrameTimeBySpeedUp()
{
	float all_dt = ani.keyFrames[1].time - ani.keyFrames[0].time;
	float t = ani.keyFrames[0].time + all_dt * (1 - cos((curframeNum * PI) / (2 * (frameCount + 1))));
	return t;
}
float getNextFrameTimeBySlowDown()
{
	float all_dt = ani.keyFrames[1].time - ani.keyFrames[0].time;
	float t = ani.keyFrames[0].time + all_dt * sin((curframeNum * PI) / (2 * (frameCount + 1)));
	return t;
}
float getNextFrameTimeByBoth()
{
	float all_dt = ani.keyFrames[1].time - ani.keyFrames[0].time;
	float t = ani.keyFrames[0].time + all_dt * 0.5 * (1 - cos((curframeNum * PI) / (frameCount + 1)));
	return t;
}
void update(void)
{
	auto curTick = clock();
	float dt = (float)(curTick - lastTick) / CLOCKS_PER_SEC;
	//printf("%f\n", dt);

	runTime += dt;

	//auto t = getNextFrameTimeByLiner();   // 线性插值（匀速）
	//auto t = getNextFrameTimeBySpeedUp(); // 加速插值
	//auto t = getNextFrameTimeBySlowDown();// 减速插值
	auto t = getNextFrameTimeByBoth();      // 加速和减速插值

	if (runTime >= t)
	{
		glutPostRedisplay();
		curframeNum++;
	}	
	if (runTime >= ani.keyFrames[1].time)
	{
		auto& kf = ani.keyFrames[1];
		drawFrame(kf);
		runTime = kf.time;
		finish();
	}

	lastTick = curTick;
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

	glClear(GL_COLOR_BUFFER_BIT);
}

void mouseFcn(GLint button, GLint action, GLint x, GLint y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (action == GLUT_UP)
		{
			begin();
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (action == GLUT_UP)
		{
			if (run)
				pause();
			else
				resume();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (action == GLUT_UP)
		{
			finish();
		}
		break;
	default:
		break;
	}
}

void code_12_6_2()
{
	lastTick = clock();
		
	ani = moveAnimation();

	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mouseFcn);
}

#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_12_COMMON

#ifdef CHAPTER_12_COMMON
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

#ifdef CHAPTER_12_1_2
	code_12_1_2();
#endif

#ifdef CHAPTER_12_6_1
	code_12_6_1();
#endif

#ifdef CHAPTER_12_6_2
	code_12_6_2();
#endif

	glutMainLoop();
}
#endif
#endif 


