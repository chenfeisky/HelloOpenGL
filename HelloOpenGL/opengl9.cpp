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

	glutMainLoop();
}
#endif
#endif 


