#include "common.h"

#if CHAPTER==17
#include "opengl17h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_17_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;
#endif

#ifdef CHAPTER_17_Test
static GLfloat angle = 0.0f;
float lightX = 0.f, lightY = 0.f, lightZ = 0.f;

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	{
		//定义太阳光源，它是一种白色的光源

		// GL_POSITION 属性的值 (x,y,z,w)  w为零表示无限远 x/w,y/w,z/w 表示光源位置
		GLfloat sun_light_position[] = { lightX, lightY, lightZ, 1.0f };
		// GL_AMBIENT 属性的值 R,G,B,A 
		GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		// GL_DIFFUSE 属性的值 R,G,B,A 
		GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		// GL_SPECULAR 属性的值 R,G,B,A
		GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		// 设置 GL_POSITION (光源位置)属性 
		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
		// 设置 GL_AMBIENT( 光源发出的光，经过非常多次反射，遗留在整个光照环境中的强度)
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
		// 设置 GL_DIFFUSE(光源发出的光，照射到粗糙表面时，经过漫反射，所得到的光强度)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
		// 设置 GL_SPECULAR(光源发出的光，照射到光滑表面时，经过镜面反射，所得到的光强度) 
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

		// 开启光源
		glEnable(GL_LIGHT0);
		// 打开光照处理功能
		glEnable(GL_LIGHTING);
		// 打开景深测试
		glEnable(GL_DEPTH_TEST);
	}
	
	{
		glPushMatrix();
		//定义太阳的材质并绘制太阳（不与场景中光照产生交互，只有一个红色的自发光）

		/* GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR 属性。这三个属性与光源的三个对应属性类似，每一属性都由四个值组成*/
		GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		// 材质发光颜色 值
		GLfloat sun_mat_emission[] = { 0.8f, 0.0f, 0.0f, 1.0f };
		//  镜面指数 0 - 128 
		GLfloat sun_mat_shininess = 0.0f;

		// 设置 GL_AMBIENT (光线照射到该材质上，经过很多次反射后最终遗留在环境中的光线强度)
		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		// 设置 GL_DIFFUSE (光线照射到该材质上，经过漫反射后形成的光线强度)
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		// 设置 GL_SPECULAR (光线照射到该材质上，经过镜面反射后形成的光线强度)
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		// 设置 GL_EMISSION (该属性由四个值组成，表示一种颜色。OpenGL 认为该材质本身就微微的向外发射光线，以至于眼睛感觉到它有这样的颜色，但这光线又比较微弱，以至于不会影响到其它物体的颜色)
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		// 设置 GL_SHININESS  (该属性只有一个值，称为“镜面指数”，取值范围是 0 到 128。该值越小，表示材质越粗糙，点光源发射的光线照射到上面，也可以产生较大的亮点。该值越大，表示材质越类似于镜面，光源照射到上面后，产生较小的亮点。)
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
		// 绘制太阳
		glTranslatef(lightX, lightY, lightZ);
		glutSolidSphere(5.0, 40, 32);
		glPopMatrix();
	}
	
	{
		glPushMatrix();
		//定义地球的材质并绘制地球

		// 地球 光源 和 材质 同太阳类似
		GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		GLfloat earth_mat_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat earth_mat_shininess = 30.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);

		// 矩阵模型变换
		//glRotatef(angle, 0.0f, -1.0f, 0.0f);
		glTranslatef(20.0f, 0.0f, 0.0f);
		// 绘制地球
		glutSolidSphere(5.0, 40, 32);
		glPopMatrix();
	}
	 
	glutSwapBuffers();
	glFlush();
}

void SpecialKey(GLint key, GLint x, GLint y)
{
	int mod = glutGetModifiers();
	switch (key)
	{
	case GLUT_KEY_UP:
		if (mod == GLUT_ACTIVE_SHIFT)
			lightZ += 2;
		else
			lightY += 2;
		break;
	case GLUT_KEY_DOWN:
		if (mod == GLUT_ACTIVE_SHIFT)
			lightZ -= 2;
		else
			lightY -= 2;
		break;
	case GLUT_KEY_LEFT:
		lightX -= 2;
		break;
	case GLUT_KEY_RIGHT:
		lightX += 2;
		break;
	default:
		break;
	}
	displayFcn();
}

void code_17_Test()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));

	glutSpecialFunc(&SpecialKey);
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_17_COMMON

#ifdef CHAPTER_17_COMMON
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("An Example OpenGL Program");

	init();

#ifdef CHAPTER_17_Test
	code_17_Test();
#endif

	glutMainLoop();
}
#endif


#endif 


