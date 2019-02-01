#ifdef TEST
//#include "stdafx.h"
#include <stdio.h>
#include "glut.h"
#include<gl/glu.h>
#include<gl/gl.h>
//#include <gl\GLAUX.h>

#include "testcamera.h"
#include "testbmp.h"

/** �����Դ������ֵ */
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };    /**< ��������� */
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };    /**< �������� */
GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   /**< �������� */
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };   /**< ��Դλ�� */

Camera m_Camera;
CBMPLoader m_Texture;

/** ����λͼ����(ľ����ͼ) */
bool LoadTexture()
{
	if (!m_Texture.LoadBitmap("image.bmp"))                                         /**< ����λͼ�ļ� */
	{
		MessageBox(NULL, (LPCSTR)"loadTexture error", (LPCSTR)"error", MB_OK);    /**< �������ʧ���򵯳��Ի��� */
		return false;
	}

	glGenTextures(1, &m_Texture.ID);                                                /**< ����һ������������� */
	glBindTexture(GL_TEXTURE_2D, m_Texture.ID);                                     /**< ����������� */

																					/** �����˲� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/** �������� */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_Texture.imageWidth,
		m_Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		m_Texture.image);

	return true;
}

/** ����������� */
void DrawGrid()
{
	/** ��ó�����һЩ״̬  */
	GLboolean  lp, tp;
	glGetBooleanv(GL_LIGHTING, &lp);
	glGetBooleanv(GL_TEXTURE_2D, &tp);

	/** �ر�����͹��� */
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	/** ���ƹ��� */
	glPushAttrib(GL_CURRENT_BIT);   /**< ���浱ǰ���� */
	glPushMatrix();                 /**< ѹ���ջ */
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);    /**< ������ɫ */

									/** ��X,Zƽ���ϻ������� */
	for (float i = -50; i <= 50; i += 1)
	{
		/** ������ */
		glBegin(GL_LINES);

		/** X�᷽�� */
		glVertex3f(-50, 0, i);
		glVertex3f(50, 0, i);

		/** Z�᷽�� */
		glVertex3f(i, 0, -50);
		glVertex3f(i, 0, 50);

		glEnd();
	}
	glPopMatrix();
	glPopAttrib();

	/** �ָ�����״̬ */
	if (tp)
		glEnable(GL_TEXTURE_2D);
	if (lp)
		glEnable(GL_LIGHTING);
}

/** �������� */
void DrawSphere()
{
	/** ���ò������� */
	GLfloat mat_ambient[] = { 0.9f, 0.5f, 0.8f, 1.0f };
	GLfloat mat_diffuse[] = { 0.9f, 0.5f, 0.8f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	/** �����������״̬ */
	GLboolean tp;
	glGetBooleanv(GL_TEXTURE_2D, &tp);
	glDisable(GL_TEXTURE_2D);                   /**< �ر����� */

												/** ���ƹ��� */
	glPushMatrix();
	glTranslatef(-5.0f, 2.0f, -10.0f);
	GLUquadricObj * sphere = gluNewQuadric();
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, 2.0, 50, 50);
	gluDeleteQuadric(sphere);
	glPopMatrix();

	/** �ָ�״̬ */
	if (tp)
		glEnable(GL_TEXTURE_2D);
}

/** ����ľ�� */
void DrawBox()
{
	/** ���ò������� */
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glPushMatrix();
	glTranslatef(5.0f, 2.0f, -10.0f);
	glScalef(2.0f, 2.0f, 2.0f);
	/** ѡ������ */
	glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
	/** ��ʼ�����ı��� */
	glBegin(GL_QUADS);
	/// ǰ����
	glNormal3f(0.0f, 0.0f, 1.0f);                               /**< ָ������ָ��۲��� */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	/// �����
	glNormal3f(0.0f, 0.0f, -1.0f);                              /**< ָ�����߱���۲��� */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	/// ����
	glNormal3f(0.0f, 1.0f, 0.0f);                               /**< ָ���������� */
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	/// ����
	glNormal3f(0.0f, -1.0f, 0.0f);                              /**< ָ�����߳��� */
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	/// �Ҳ���
	glNormal3f(1.0f, 0.0f, 0.0f);                               /**< ָ�����߳��� */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	/// �����
	glNormal3f(-1.0f, 0.0f, 0.0f);                              /**< ָ�����߳��� */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	glPopMatrix();
}

void init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/** ���ù�Դ������ֵ */
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     /**< ���û����� */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     /**< ��������� */
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);   /**< ��������� */

														/** ���ù�Դ������ */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_TEXTURE_2D);

	/** �������� */
	if (!LoadTexture())
		MessageBox(NULL, (LPCSTR)"��������ʧ��!", (LPCSTR)"����", MB_OK);

	/** ��������� */
	m_Camera.setCamera(0.0f, 1.5f, 6.0f, 0.0f, 1.5f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void display(void)
{
	/** �û��Զ���Ļ��ƹ��� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	/** ��������� */
	m_Camera.setLook();
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   /**< ���ù�Դλ�� */

														/**< ���ƹ��� */
	DrawGrid();
	DrawSphere();
	DrawBox();

	glFlush();
}

void ChangeSize(int width, int height)
{
	glViewport(0, 0, width, height);                                    /**< ���������ӿ� */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void motion(int x, int y)
{
	m_Camera.setViewByMouse();

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 49:
		m_Camera.setSpeed(0.2f);
		break;
	case 50:
		m_Camera.setSpeed(0.6f);
		break;
	case 119:
		m_Camera.moveCamera(m_Camera.getSpeed());
		break;
	case 115:
		m_Camera.moveCamera(-m_Camera.getSpeed());
		break;
	case 97:
		m_Camera.yawCamera(-m_Camera.getSpeed());
		break;
	case 100:
		m_Camera.yawCamera(m_Camera.getSpeed());
		break;
	}

	glutPostRedisplay();
	printf("========%d", key);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) >> 1) - 400, (GetSystemMetrics(SM_CYSCREEN) >> 1) - 300);
	glutCreateWindow("���������");
	init();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(display);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
#endif