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

	glutMainLoop();
}
#endif
#endif 


