#include "common.h"

#ifdef CHAPTER_7
#include "opengl7h.h"

#ifdef CHAPTER_7_COMMON
GLsizei winWidth = 800, winHeight = 600;
#endif

#ifdef CHAPTER_7_1_1
class wcPt2D
{
public:
	GLfloat x, y;
};
void translatePolygon(wcPt2D* verts, GLint nVerts, GLfloat tx, GLfloat ty)
{
	GLint k;
	for (k = 0; k < nVerts; k++)
	{
		verts[k].x = verts[k].x + tx;
		verts[k].y = verts[k].y + ty;
	}
	glBegin(GL_POLYGON);
	for (k = 0; k < nVerts; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	
	wcPt2D verts[] = { {120, 172}, {186, 104},{300, 150} ,{182, 224} };
	glBegin(GL_POLYGON);
	for (int k = 0; k < 4; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();

	translatePolygon(verts, 4, 400, 60);

	glFlush();
}
void code_7_1_1()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_7_1_2
class wcPt2D
{
public:
	GLfloat x, y;
};
void rotatePolygon(wcPt2D* verts, GLint nVerts, wcPt2D pivPt, GLdouble theta)
{
	wcPt2D* vertsRot = new wcPt2D[nVerts];
	GLint k;
	for (k = 0; k < nVerts; k++)
	{
		vertsRot[k].x = pivPt.x + (verts[k].x - pivPt.x) * std::cos(theta) - (verts[k].y - pivPt.y) * std::sin(theta);
		vertsRot[k].y = pivPt.y + (verts[k].x - pivPt.x) * std::sin(theta) + (verts[k].y - pivPt.y) * std::cos(theta);
	}
	glBegin(GL_POLYGON);
	for (k = 0; k < nVerts; k++)
		glVertex2f(vertsRot[k].x, vertsRot[k].y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D verts[] = { { 520, 232 },{ 586, 164 },{ 700, 210 } ,{ 582, 284 } };
	glBegin(GL_POLYGON);
	for (int k = 0; k < 4; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();

	rotatePolygon(verts, 4, {275, 242}, PI / 3);

	glFlush();
}
void code_7_1_2()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_7_1_3
class wcPt2D
{
public:
	GLfloat x, y;
};
void scalePolygon(wcPt2D* verts, GLint nVerts, wcPt2D fixedPt, GLfloat sx, GLfloat sy)
{
	wcPt2D* vertsNew = new wcPt2D[nVerts];
	GLint k;
	for (k = 0; k < nVerts; k++)
	{
		vertsNew[k].x = verts[k].x * sx + fixedPt.x * (1 - sx);
		vertsNew[k].y = verts[k].y * sy + fixedPt.y * (1 - sy);
	}
	glBegin(GL_POLYGON);
	for (k = 0; k < nVerts; k++)
		glVertex2f(vertsNew[k].x, vertsNew[k].y);
	glEnd();
}
void drawFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	wcPt2D verts[] = { { 120, 172 },{ 186, 104 },{ 300, 150 } ,{ 182, 224 } };
	glBegin(GL_POLYGON);
	for (int k = 0; k < 4; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();

	scalePolygon(verts, 4, { 100, 100 }, 3, 3);

	glFlush();
}
void code_7_1_3()
{
	glutDisplayFunc(drawFunc);
}
#endif

#ifdef CHAPTER_7_4_11
GLfloat xwcMin = 0.0, xwcMax = 225.0;
GLfloat ywcMin = 0.0, ywcMax = 225.0;
class wcPt2D
{
public:
	GLfloat x, y;
};
typedef GLfloat Matrix3x3[3][3];
Matrix3x3 matComposite;

void matrix3x3SetIdentity(Matrix3x3& matIdent3x3)
{
	GLint row, col;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matIdent3x3[row][col] = (row == col);
}
void matrix3x3PreMultiply(Matrix3x3& m1, Matrix3x3& m2)
{
	GLint row, col;
	Matrix3x3 matTemp;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col];
		}

	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			m2[row][col] = matTemp[row][col];
}
void translate2D(GLfloat tx, GLfloat ty)
{
	Matrix3x3 matTransl;
	matrix3x3SetIdentity(matTransl);
	matTransl[0][2] = tx;
	matTransl[1][2] = ty;
	matrix3x3PreMultiply(matTransl, matComposite);
}
void rotate2D(wcPt2D pivotPt, GLfloat theta)
{
	Matrix3x3 matRot;
	matrix3x3SetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
	matrix3x3PreMultiply(matRot, matComposite);
}
void scale2D(GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
	Matrix3x3 matScale;
	matrix3x3SetIdentity(matScale);
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx) * fixedPt.x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy) * fixedPt.y;
	matrix3x3PreMultiply(matScale, matComposite);
}
void transformVerts2D(GLint nVerts, wcPt2D* verts)
{
	GLint k;
	GLfloat temp;
	for (k = 0; k < nVerts; k++)
	{
		temp = matComposite[0][0] * verts[k].x + matComposite[0][1] * verts[k].y + matComposite[0][2];
		verts[k].y = matComposite[1][0] * verts[k].x + matComposite[1][1] * verts[k].y + matComposite[1][2];
		verts[k].x = temp;
	}
}
void triangle(wcPt2D* verts)
{
	GLint k;
	glBegin(GL_TRIANGLES);
	for (k = 0; k < 3; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
void displayFcn(void)
{
	GLint nVerts = 3;
	wcPt2D verts[3] = { {50.0, 25.0}, {150.0, 25.0}, {100.0, 100.0} };
	wcPt2D centroidPt;
	GLint k, xSum = 0, ySum = 0;
	for (k = 0; k < nVerts; k++)
	{
		xSum += verts[k].x;
		ySum += verts[k].y;
	}
	centroidPt.x = GLfloat(xSum) / GLfloat(nVerts);
	centroidPt.y = GLfloat(ySum) / GLfloat(nVerts);
	wcPt2D pivPt, fixedPt;
	pivPt = centroidPt;
	fixedPt = centroidPt;
	GLfloat tx = 0.0, ty = 100.0;
	GLfloat sx = 0.5, sy = 0.5;
	GLdouble theta = PI / 2.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	triangle(verts);
	matrix3x3SetIdentity(matComposite);
	scale2D(sx, sy, fixedPt);
	rotate2D(pivPt, theta);
	translate2D(tx, ty);
	transformVerts2D(nVerts, verts);
	glColor3f(1.0, 0.0, 0.0);
	triangle(verts);
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);
	glClear(GL_COLOR_BUFFER_BIT);
}
void code_7_4_11()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
}
#endif

#ifdef CHAPTER_7_10
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	
	glColor3f(0.0, 0.0, 1.0);
	glRecti(50, 100, 200, 150);

	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-200.0, -50.0, 0.0);
	glRecti(50, 100, 200, 150);

	glLoadIdentity();
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glRecti(50, 100, 200, 150);

	glLoadIdentity();
	glScalef(-0.5, 1.0, 1.0);
	glRecti(50, 100, 200, 150);

	glFlush();
}
void code_7_10()
{
	glLoadIdentity();
	gluOrtho2D(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2);
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_7_EXERCISE_1
float smallSin()
{
	return 0.f;
}
float smallCos()
{
	return 0.f;
}
void displayFcn(void)
{
	printf("xxxxx\n");
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//printf("onTimer: %d\n", dwTime);
	displayFcn();
}
void code_7_exercise_1()
{
	SetTimer(NULL, NULL, 1000, onTimer);

	glutDisplayFunc(displayFcn);

}
#endif



//////////////////////////////////////////////////////////////////////////
// CHAPTER_7_COMMON

#ifdef CHAPTER_7_COMMON
void init(void)
{
	// ºÚÉ«±³¾°É«
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

#ifdef CHAPTER_7_1_1
	code_7_1_1();
#endif

#ifdef CHAPTER_7_1_2
	code_7_1_2();
#endif

#ifdef CHAPTER_7_1_3
	code_7_1_3();
#endif

#ifdef CHAPTER_7_4_11
	code_7_4_11();
#endif

#ifdef CHAPTER_7_10
	code_7_10();
#endif

#ifdef CHAPTER_7_EXERCISE_1
	code_7_exercise_1();
#endif

	glutMainLoop();
}
#endif
#endif 


