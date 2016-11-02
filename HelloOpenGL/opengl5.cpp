#include "common.h"

#ifdef CHAPTER_5
#include "opengl5h.h"

#ifdef CHAPTER_5_COMMON
GLsizei winWidth = 800, winHeight = 600;
#endif

#ifdef CHAPTER_5_7_2
typedef struct { float x, y; } wcPt2D;
wcPt2D dataPts[5];
void linePlot(wcPt2D dataPts[5])
{
	int k;
	glBegin(GL_LINE_STRIP);
		for (k = 0; k < 5; k++)
			glVertex2f(dataPts[k].x, dataPts[k].y);
	glEnd();
	glFlush();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glEnable(GL_LINE_STIPPLE);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 100,270 };
	dataPts[2] = { 287, 270 };
	dataPts[3] = { 287, 434 };
	dataPts[4] = { 512,517 };
	glLineStipple(1, 0x1C47);
	linePlot(dataPts);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 161, 166 };
	dataPts[2] = { 408, 171 };
	dataPts[3] = { 410, 397 };
	dataPts[4] = { 571, 451 };
	glLineStipple(1, 0x00FF);
	glLineWidth(2.0);
	linePlot(dataPts);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 206, 43 };
	dataPts[2] = { 553, 43 };
	dataPts[3] = { 532, 243 };
	dataPts[4] = { 668, 345 };
	glLineStipple(1, 0x0101);
	glLineWidth(3.0);
	linePlot(dataPts);

	glDisable(GL_LINE_STIPPLE);
}
void code_5_7_2()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_1
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glIndexi(0);
	glBegin(GL_LINE);
	glVertex2i(0, 0);
	glVertex2i(300, 300);
	glEnd();
	glFlush();
}
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);
}
void main(int argc, char** argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_INDEX);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("An Example OpenGL Program");

	init();

	glutSetColor(0, 1.0, 0.0, 0.0);
	glutSetColor(1, 0.0, 1.0, 0.0);
	glutSetColor(2, 0.0, 0.0, 1.0);
	glutSetColor(3, 0.0, 0.0, 0.0);
	glutSetColor(4, 1.0, 1.0, 1.0);
	glutSetColor(5, 0.5, 0.5, 0.5);

	glutDisplayFunc(drawFunction);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_5_EXERCISE_2
GLfloat hueAndPt[] = {  1, 0, 0 ,  221, 542, 0 ,  1, 0, 0 ,  166,486, 0 ,  0, 1, 0 , 188, 411, 0 , 0, 1, 0 ,  227, 446, 0 ,
						0, 0, 0, 311, 404, 0, 1, 1, 1, 513, 376, 0, 0.7, 0.5, 0.5, 513, 449, 0, 0.5, 0.5, 0.5, 434, 519, 0,
						0, 0, 1, 570, 451, 0, 1, 1, 0, 680, 381, 0, 0, 1, 0, 697, 485, 0, 1, 0, 0, 640, 510, 0,
						0, 0, 0, 209, 278, 0, 1, 1, 1, 83, 152, 0, 0.3, 0.6, 0.5, 394, 134, 0, 1, 0.3, 0.7, 354, 245, 0,
						1, 1, 0, 427, 282, 0, 1, 0, 0, 416, 202, 0, 1, 0, 0, 443, 134, 0, 0.9, 1, 0.1, 504, 125, 0, 0.5, 0.5, 0.5, 682, 319, 0, 1, 0, 1, 489, 309, 0,
};
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &hueAndPt[3]);
	//glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &hueAndPt[0]);
	glInterleavedArrays(GL_C3F_V3F, 0, hueAndPt);
	GLubyte vertIndex[] = { 0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
	21,16,17,18,
	21,18,19,20};
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, vertIndex);
	
	glFlush();
}
void code_5_exercise_2()
{
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	//glPolygonMode(GL_FRONT, GL_LINE);
	glutDisplayFunc(drawFunction);
}
#endif+

#ifdef CHAPTER_5_EXERCISE_4
GLfloat hueAndPt[] = { 0.2, 0.3, 0.4, 0, 0, 0, 
1, 0, 0, 100, 0, 0,
0, 1, 0, 100, 100, 0,
0, 0, 1, 0, 100, 0,
1, 1, 0, 0, 0, 100,
1, 0, 1, 100, 0, 100,
0, 1, 1, 100, 100, 100,
1, 0.3, 0.8, 0, 100, 100
};
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glInterleavedArrays(GL_C3F_V3F, 0, hueAndPt);
	GLubyte vertIndex[] = { 5,1,2,6,
		6, 2, 3, 7,
		3, 0, 4, 7,
		0, 1, 5, 4,
		4, 5, 6, 7,
		0, 3, 2, 1 };
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, vertIndex);

	glFlush();
}
void code_5_exercise_4()
{
	//glPolygonMode(GL_FRONT, GL_LINE);
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_5
GLubyte lightCloud[] =
{
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
};
GLubyte darkCloud[] =
{
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,

	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,

	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,

	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
};
void drawLightCloudPolygon()
{
	glBegin(GL_POLYGON);
	glVertex2f(253, 198);
	glVertex2f(311, 215);
	glVertex2f(393, 238);
	glVertex2f(473, 273);
	glVertex2f(546, 318);
	glVertex2f(499, 356);
	glVertex2f(368, 361);
	glVertex2f(200, 321);
	glVertex2f(150, 263);
	glVertex2f(141, 238);
	glVertex2f(153, 219);
	glVertex2f(219, 206);
	glEnd();
}
void drawDarkCloudPolygon()
{
	glBegin(GL_POLYGON);
	glVertex2f(328, 181);
	glVertex2f(414, 178);
	glVertex2f(461, 192);
	glVertex2f(512, 220);
	glVertex2f(542, 248);
	glVertex2f(546, 318);
	glVertex2f(473, 273);
	glVertex2f(393, 238);
	glVertex2f(311, 215);
	glVertex2f(253, 198);
	glEnd();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_POLYGON_STIPPLE);
	glColor3f(1.0, 1.0, 1.0);
	drawLightCloudPolygon();

	glPolygonStipple(lightCloud);
	glEnable(GL_POLYGON_STIPPLE);
	glColor3f(0.8, 0.8, 0.8);
	drawLightCloudPolygon();

	glDisable(GL_POLYGON_STIPPLE);
	glColor3f(1.0, 1.0, 1.0);
	drawDarkCloudPolygon();

	glPolygonStipple(darkCloud);
	glEnable(GL_POLYGON_STIPPLE);
	glColor3f(0.2, 0.2, 0.2);
	drawDarkCloudPolygon();

	glFlush();
}
void init(void)
{
	glClearColor(0.6, 0.84, 0.91, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);
}
void main(int argc, char** argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("An Example OpenGL Program");

	init();

	glPolygonMode(GL_FRONT, GL_FILL);
	glutDisplayFunc(drawFunction);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_5_EXERCISE_6
GLubyte lightCloud[] =
{
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x88, 0x88, 0x88, 0x88,
};
GLubyte darkCloud[] =
{
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,

	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,

	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,

	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
	0x00, 0x00, 0x00, 0x00,
	0xAA, 0xAA, 0xAA, 0xAA,
};
void drawLightCloudPolygon()
{
	glBegin(GL_POLYGON);
	glVertex2f(253, 198);
	glVertex2f(311, 215);
	glVertex2f(393, 238);
	glVertex2f(473, 273);
	glVertex2f(546, 318);
	glVertex2f(499, 356);
	glVertex2f(368, 361);
	glVertex2f(200, 321);
	glVertex2f(150, 263);
	glVertex2f(141, 238);
	glVertex2f(153, 219);
	glVertex2f(219, 206);
	glEnd();
}
void drawDarkCloudPolygon()
{
	glBegin(GL_POLYGON);
	glVertex2f(328, 181);
	glVertex2f(414, 178);
	glVertex2f(461, 192);
	glVertex2f(512, 220);
	glVertex2f(542, 248);
	glVertex2f(546, 318);
	glVertex2f(473, 273);
	glVertex2f(393, 238);
	glVertex2f(311, 215);
	glVertex2f(253, 198);
	glEnd();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_POLYGON_STIPPLE);
	glColor3f(0.88, 0.87, 0.6);
	drawLightCloudPolygon();

	glPolygonStipple(lightCloud);
	glEnable(GL_POLYGON_STIPPLE);
	glColor3f(0.8, 0.8, 0.8);
	drawLightCloudPolygon();

	glDisable(GL_POLYGON_STIPPLE);
	glColor3f(1.0, 0.5, 0.0);
	drawDarkCloudPolygon();

	glPolygonStipple(darkCloud);
	glEnable(GL_POLYGON_STIPPLE);
	glColor3f(0.2, 0.2, 0.2);
	drawDarkCloudPolygon();
	
	glFlush();
}
void init(void)
{
	glClearColor(0.6, 0.84, 0.91, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);
}
void main(int argc, char** argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("An Example OpenGL Program");

	init();

	glPolygonMode(GL_FRONT, GL_FILL);
	glutDisplayFunc(drawFunction);

	glutMainLoop();
}
#endif

#ifdef CHAPTER_5_EXERCISE_7
typedef struct { float x, y; } wcPt2D;
wcPt2D dataPts[5];
void linePlot(wcPt2D dataPts[5], GLfloat lineWidth)
{
	glLineWidth(lineWidth);
	int k;
	glBegin(GL_LINE_STRIP);
	for (k = 0; k < 5; k++)
		glVertex2f(dataPts[k].x, dataPts[k].y);
	glEnd();
	glFlush();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glEnable(GL_LINE_STIPPLE);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 100, 270 };
	dataPts[2] = { 287, 270 };
	dataPts[3] = { 287, 434 };
	dataPts[4] = { 512, 517 };
	glLineStipple(1, 0x1C47);
	linePlot(dataPts, 1.0);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 161, 166 };
	dataPts[2] = { 408, 171 };
	dataPts[3] = { 410, 397 };
	dataPts[4] = { 571, 451 };
	glLineStipple(1, 0x00FF);
	linePlot(dataPts, 2.0);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 206, 43 };
	dataPts[2] = { 553, 43 };
	dataPts[3] = { 532, 243 };
	dataPts[4] = { 668, 345 };
	glLineStipple(1, 0x0101);
	linePlot(dataPts, 3.0);

	glDisable(GL_LINE_STIPPLE);
}
void code_5_exercise_7()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_8
typedef struct { float x, y; } wcPt2D;
wcPt2D dataPts[5];
void linePlot(wcPt2D dataPts[5], GLushort pattern)
{
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, pattern);
	int k;
	glBegin(GL_LINE_STRIP);
	for (k = 0; k < 5; k++)
		glVertex2f(dataPts[k].x, dataPts[k].y);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glFlush();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 100, 270 };
	dataPts[2] = { 287, 270 };
	dataPts[3] = { 287, 434 };
	dataPts[4] = { 512, 517 };
	linePlot(dataPts, 0x1C47);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 161, 166 };
	dataPts[2] = { 408, 171 };
	dataPts[3] = { 410, 397 };
	dataPts[4] = { 571, 451 };
	glLineWidth(2.0);
	linePlot(dataPts, 0x00FF);

	dataPts[0] = { 0, 0 };
	dataPts[1] = { 206, 43 };
	dataPts[2] = { 553, 43 };
	dataPts[3] = { 532, 243 };
	dataPts[4] = { 668, 345 };
	glLineWidth(3.0);
	linePlot(dataPts, 0x0101);
}
void code_5_exercise_8()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_9
typedef struct { float x, y; } wcPt2D;
void linePlot(string fileName)
{
	ifstream dataFile(fileName);
	if (!dataFile)
	{
		printf("打开文件%s失败！\n", fileName);
		return;
	}
	char buf[1024];
	glBegin(GL_LINE_STRIP);
	while (dataFile.good() && !dataFile.eof())
	{
		memset(buf, 0, 1024);
		dataFile.getline(buf, 1024);
		wcPt2D point;
		sscanf_s(buf, "%f,%f", &point.x, &point.y);
		glVertex2f(point.x, point.y);
	}		
	glEnd();
	glFlush();
	dataFile.close();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glEnable(GL_LINE_STIPPLE);

	glLineStipple(1, 0x1C47);
	linePlot("line1.txt");

	glLineStipple(1, 0x00FF);
	glLineWidth(2.0);
	linePlot("line2.txt");

	glLineStipple(1, 0x0101);
	glLineWidth(3.0);
	linePlot("line3.txt");
	glDisable(GL_LINE_STIPPLE);
}
void code_5_exercise_9()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_10
typedef struct { float x, y; } wcPt2D;
void linePlot(string fileName)
{
	ifstream dataFile(fileName);
	if (!dataFile)
	{
		printf("打开文件%s失败！\n", fileName);
		return;
	}
	char buf[1024];
	glBegin(GL_LINE_STRIP);
	while (dataFile.good() && !dataFile.eof())
	{
		memset(buf, 0, 1024);
		dataFile.getline(buf, 1024);
		wcPt2D point;
		sscanf_s(buf, "%f,%f", &point.x, &point.y);
		glVertex2f(point.x, point.y);
	}
	glEnd();
	glFlush();
	dataFile.close();
}
void drawWord(wcPt2D point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void drawCoordinate()
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(1000, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 1000);
	glEnd();
	drawWord({ 0, 3 }, "0");
	for (int i = 0; i < 7; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ (float)(i + 1) * 100 - 15, (float)3 }, number);
	}
	drawWord({ 790, 3 }, "x");
	for (int i = 0; i < 5; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ (float)0, (float)(i + 1) * 100 - 5 }, number);
	}
	drawWord({ 0, 590 }, "y");
	glFlush();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);	
	glEnable(GL_LINE_STIPPLE);

	glColor3f(1.0, 0.0, 0.0);
	glLineStipple(1, 0x1C47);
	linePlot("line1.txt");

	glColor3f(0.0, 1.0, 0.0);
	glLineStipple(1, 0x00FF);
	glLineWidth(2.0);
	linePlot("line2.txt");

	glColor3f(0.0, 0.0, 1.0);
	glLineStipple(1, 0x0101);
	glLineWidth(3.0);
	linePlot("line3.txt");
	glDisable(GL_LINE_STIPPLE);

	glLineWidth(1.0);
	drawCoordinate();
}
void code_5_exercise_10()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_11
typedef struct { float x, y; } wcPt2D;
int width = 30;
GLubyte bitShape[20] = { 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00,
0xff, 0x80, 0x7f, 0x00, 0x3e, 0x00, 0x1c, 0x00, 0x08, 0x00 };
void drawWord(wcPt2D point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void drawCoordinate()
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(1000, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 1000);
	glEnd();
	drawWord({ 0, 3 }, "0");
	for (int i = 0; i < 7; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ (float)(i + 1) * 100 - 15, 3 }, number);
	}
	drawWord({ 790, 3 }, "x");
	for (int i = 0; i < 5; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ 0, (float)(i + 1) * 100 - 5 }, number);
	}
	drawWord({ 0, 590 }, "y");
	glFlush();
}
void drawRect(GLint coord, GLint value)
{
	glRecti(coord - width / 2, 0, coord + width / 2, value);
}
void drawRectColor(GLint coord, GLint value)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(coord - width / 2, 0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(coord + width / 2, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(coord + width / 2, value);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(coord - width / 2, value);
	glEnd();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawCoordinate();
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_LINE);
	drawRect(100, 100);
	drawWord({ 100 - 15, 100 + 3 }, "100");

	glColor3f(1.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	drawRect(200, 200);
	glColor3f(0.0, 0.0, 0.0);
	drawWord({ 200 - 15, 200 + 3 }, "200");

	drawRectColor(300, 300);
	glColor3f(0.0, 0.0, 0.0);
	drawWord({ 300 - 15, 300 + 3 }, "300");

	glPolygonStipple(bitShape);
	glEnable(GL_POLYGON_STIPPLE);
	glColor3f(0.0, 0.0, 0.0);
	drawRect(400, 400);
	glDisable(GL_POLYGON_STIPPLE);
	drawWord({ 400 - 15, 400 + 3 }, "400");

	glFlush();
}
void code_5_exercise_11()
{
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_12
typedef struct { float x, y; } wcPt2D;
struct Color3f
{
	float r, g, b;
};
int width = 30;
void drawRectH(GLint coord, GLint value, Color3f color)
{
	glColor3f(color.r, color.g, color.b);
	glRecti(0, coord - width / 2, value, coord + width / 2);
}
void drawRectV(GLint coord, GLint value, Color3f color)
{
	glColor3f(color.r, color.g, color.b);
	glRecti(coord - width / 2, 0, coord + width / 2, value);
}
void drawWord(wcPt2D point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void drawCoordinate()
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(1000, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 1000);
	glEnd();
	drawWord({ 0, 3 }, "0");
	for (int i = 0; i < 7; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ (float)(i + 1) * 100 - 15, 3 }, number);
	}
	drawWord({ 790, 3 }, "x");
	for (int i = 0; i < 5; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ 0, (float)(i + 1) * 100 - 5 }, number);
	}
	drawWord({ 0, 590 }, "y");
	glFlush();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawCoordinate();
	drawRectH(100, 300, { 1.0, 0.0, 0.0 });
	drawRectV(100, 300, { 1.0, 0.0, 0.0 });

	drawRectH(200, 200, { 0.0, 1.0, 0.0 });
	drawRectV(200, 200, { 0.0, 1.0, 0.0 });

	drawRectH(300, 100, { 0.0, 0.0, 1.0 });
	drawRectV(300, 100, { 0.0, 0.0, 1.0 });

	glFlush();
}
void code_5_exercise_12()
{
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_13
struct Color3f
{
	float r, g, b;
};
map<int, Color3f> colorTable;
void addColor(int index, Color3f color)
{
	colorTable[index] = color;
}
Color3f findColor(int index)
{
	if (colorTable.find(index) != colorTable.end())
		return colorTable[index];
	
	printf("没有找到颜色索引：%d\n", index);
	return { 0.0, 0.0, 0.0 };
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	glColor3f(findColor(1).r, findColor(1).g, findColor(1).b);
	glVertex2f(100, 100);
	glColor3f(findColor(2).r, findColor(2).g, findColor(2).b);
	glVertex2f(200, 200);
	glColor3f(findColor(3).r, findColor(3).g, findColor(3).b);
	glVertex2f(100, 300);
	glColor3f(findColor(4).r, findColor(4).g, findColor(4).b);
	glVertex2f(500, 100);
	glColor3f(findColor(10).r, findColor(10).g, findColor(10).b);
	glVertex2f(400, 100);
	glColor3f(findColor(11).r, findColor(11).g, findColor(11).b);
	glVertex2f(500, 600);
	glEnd();

	glFlush();
}
void code_5_exercise_13()
{
	addColor(1, { 1.0, 0.0, 0.0 });
	addColor(2, { 0.0, 1.0, 0.0 });
	addColor(3, { 0.0, 0.0, 1.0 });
	addColor(4, { 1.0, 1.0, 0.0 });
	addColor(5, { 1.0, 0.0, 1.0 });
	addColor(6, { 0.0, 1.0, 1.0 });
	addColor(7, { 1.0, 1.0, 1.0 });
	addColor(8, { 0.0, 0.0, 0.0 });
	addColor(9, { 0.1, 0.1, 0.1 });
	addColor(10, { 0.5, 0.5, 0.5 });
	addColor(11, { 0.9, 0.9, 0.9 });

	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_14
void drawFunction()
{

}
void code_5_exercise_14()
{
	printf("帧缓存最小尺寸是%d字节\n", 14 * 10 * 120);
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_15
void drawFunction()
{

}
void code_5_exercise_15()
{
	printf("共有%d种灰度等级\n", (int)pow(2, 8));
	printf("总共能显示%d种颜色\n", (int)pow(2, 24));
	printf("同时能显示%d种颜色\n", (int)pow(2,16));
	printf("总的存储容量是%d字节\n", 1024 * 768 * 16 / 8);
	printf("一种方案：使帧缓存中每像素位数减少至8位，增加颜色表的数量为 %d 张，这样帧缓存的存储容量为%d字节\n", (int)(pow(2,16) / pow(2, 8)), 1024 * 768 * 8 / 8);

	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_16
typedef struct { float x, y; } wcPt2D;
void drawWord(wcPt2D point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void drawCoordinate()
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(1000, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 1000);
	glEnd();
	drawWord({ 0, 3 }, "0");
	for (int i = 0; i < 7; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ (i + 1) * 100 - 15, 3 }, number);
	}
	drawWord({ 790, 3 }, "x");
	for (int i = 0; i < 5; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ 0, (i + 1) * 100 - 5 }, number);
	}
	drawWord({ 0, 590 }, "y");
	glFlush();
}
void drawPoints(vector<wcPt2D> points, GLubyte gray)
{
	glPointSize(5.0);
	glColor3f(gray / 256.f, gray / 256.f, gray / 256.f);
	glBegin(GL_POINTS);
	for (auto& p : points)
		glVertex2f(p.x, p.y);
	glEnd();
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawCoordinate();
	drawPoints({ { 100, 100 }, { 200, 100 }, { 300, 100 }, { 400, 100 } }, 50);
	drawPoints({ { 100, 300 }, { 200, 300 }, { 300, 300 }, { 400, 300 } }, 125);
	drawPoints({ { 100, 500 }, { 200, 500 }, { 300, 500 }, { 400, 500 } }, 200);
	glFlush();
}
void code_5_exercise_16()
{
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_EXERCISE_17
typedef struct { float x, y; } wcPt2D;
struct Color3f
{
	float r, g, b;
};
void drawWord(wcPt2D point, string word)
{
	glRasterPos2i(point.x, point.y);
	for (char c : word)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}
void drawCoordinate()
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(1000, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 1000);
	glEnd();
	drawWord({ 0, 3 }, "0");
	for (int i = 0; i < 7; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ (i + 1) * 100 - 15, 3 }, number);
	}
	drawWord({ 790, 3 }, "x");
	for (int i = 0; i < 5; i++)
	{
		char number[8] = {};
		sprintf_s(number, "%d", (i + 1) * 100);
		drawWord({ 0, (i + 1) * 100 - 5 }, number);
	}
	drawWord({ 0, 590 }, "y");
	glFlush();
}
void drawPoints(vector<wcPt2D> points, Color3f color, string title, wcPt2D titlePos)
{
	glPointSize(5.0);
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	for (auto& p : points)
		glVertex2f(p.x, p.y);
	glEnd();
	
	glBegin(GL_POINTS);
	glVertex2f(titlePos.x, titlePos.y + 3);
	glEnd();
	drawWord({ titlePos.x + 10, titlePos.y }, title);
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawCoordinate();
	drawPoints({ { 100, 100 }, { 200, 100 }, { 300, 100 }, { 400, 100 } }, { 1.0, 0.0, 0.0 }, "red", {200, 550});
	drawPoints({ { 100, 300 }, { 200, 300 }, { 300, 300 }, { 400, 300 } }, { 0.0, 1.0, 0.0 }, "green", { 400, 550 });
	drawPoints({ { 100, 500 }, { 200, 500 }, { 300, 500 }, { 400, 500 } }, { 0.0, 0.0, 1.0 }, "blue", { 600, 550 });
	glFlush();
}
void code_5_exercise_17()
{
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_ADD_EXERCISE_1
GLfloat points[] = { 221, 542, 0, 166, 486, 0, 188, 411, 0, 227, 446, 0};
GLboolean edgeFlags[] = { GL_TRUE, GL_TRUE, GL_FALSE, GL_TRUE };
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// 深度值位移
	float z = 1;
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.75, 1.0);

	glColor3f(1.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(100, 100,0);
	glVertex3f(200, 100,z);
	glVertex3f(200, 200,0);
	glVertex3f(100, 200,z);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_POLYGON);
	glVertex3f(100, 100, 0);
	glVertex3f(200, 100, z);
	glVertex3f(200, 200, 0);
	glVertex3f(100, 200, z);
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);

	// 多边形消除选定边
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	glVertex3f(300, 100, 0);
	glEdgeFlag(GL_FALSE);
	glVertex3f(400, 100, 0);
	glEdgeFlag(GL_TRUE);
	glVertex3f(350, 300, 0);
	glEnd();

	glVertexPointer(3, GL_FLOAT, 0, points);
	glEdgeFlagPointer(0, edgeFlags);
	GLubyte vertIndex[] = { 0, 1, 2, 3 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, vertIndex);

	// 反走样
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(500, 400, 0);
	glVertex3f(378, 534, 0);
	glEnd();

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);
	glVertex3f(600, 400, 0);
	glVertex3f(478, 534, 0);
	glEnd();
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);

	glFlush();
}
void code_5_add_exercise_1()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_EDGE_FLAG_ARRAY);
	glutDisplayFunc(drawFunction);
}
#endif

#ifdef CHAPTER_5_ADD_EXERCISE_2
struct Color3f
{
	float r, g, b;
};
map<int, Color3f> colorTable;
void addColor(int index, Color3f color)
{
	colorTable[index] = color;
}
Color3f findColor(int index)
{
	if (colorTable.find(index) != colorTable.end())
		return colorTable[index];

	printf("没有找到颜色索引：%d\n", index);
	return{ 0.0, 0.0, 0.0 };
}
void drawFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(findColor(1).r, findColor(1).g, findColor(1).b);
	glVertex3f(100, 100, 0);
	glColor3f(findColor(2).r, findColor(2).g, findColor(2).b);
	glVertex3f(200, 100, 0);
	glColor3f(findColor(3).r, findColor(3).g, findColor(3).b);
	glVertex3f(200, 200, 0);
	glColor3f(findColor(4).r, findColor(4).g, findColor(4).b);
	glVertex3f(100, 200, 0);
	glEnd();

	glColor3f(findColor(8).r, findColor(8).g, findColor(8).b);
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_POLYGON);
	glVertex3f(100, 100, 0);
	glVertex3f(200, 100, 0);
	glVertex3f(200, 200, 0);
	glVertex3f(100, 200, 0);
	glEnd();

	glFlush();
}
void code_5_add_exercise_2()
{
	addColor(1, { 1.0, 0.0, 0.0 });
	addColor(2, { 0.0, 1.0, 0.0 });
	addColor(3, { 0.0, 0.0, 1.0 });
	addColor(4, { 1.0, 1.0, 0.0 });
	addColor(5, { 1.0, 0.0, 1.0 });
	addColor(6, { 0.0, 1.0, 1.0 });
	addColor(7, { 1.0, 1.0, 1.0 });
	addColor(8, { 0.0, 0.0, 0.0 });
	addColor(9, { 0.1, 0.1, 0.1 });
	addColor(10, { 0.5, 0.5, 0.5 });
	addColor(11, { 0.9, 0.9, 0.9 });

	glutDisplayFunc(drawFunction);
	
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_5_COMMON

#ifdef CHAPTER_5_COMMON

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
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

#ifdef CHAPTER_5_7_2
	code_5_7_2();
#endif

#ifdef CHAPTER_5_EXERCISE_2
	code_5_exercise_2();
#endif

#ifdef CHAPTER_5_EXERCISE_4
	code_5_exercise_4();
#endif

#ifdef CHAPTER_5_EXERCISE_7
	code_5_exercise_7();
#endif

#ifdef CHAPTER_5_EXERCISE_8
	code_5_exercise_8();
#endif

#ifdef CHAPTER_5_EXERCISE_9
	code_5_exercise_9();
#endif

#ifdef CHAPTER_5_EXERCISE_10
	code_5_exercise_10();
#endif

#ifdef CHAPTER_5_EXERCISE_11
	code_5_exercise_11();
#endif

#ifdef CHAPTER_5_EXERCISE_12
	code_5_exercise_12();
#endif

#ifdef CHAPTER_5_EXERCISE_13
	code_5_exercise_13();
#endif

#ifdef CHAPTER_5_EXERCISE_14
	code_5_exercise_14();
#endif

#ifdef CHAPTER_5_EXERCISE_15
	code_5_exercise_15();
#endif

#ifdef CHAPTER_5_EXERCISE_16
	code_5_exercise_16();
#endif

#ifdef CHAPTER_5_EXERCISE_17
	code_5_exercise_17();
#endif

#ifdef CHAPTER_5_ADD_EXERCISE_1
	code_5_add_exercise_1();
#endif

#ifdef CHAPTER_5_ADD_EXERCISE_2
	code_5_add_exercise_2();
#endif

	glutMainLoop();
}
#endif
#endif 