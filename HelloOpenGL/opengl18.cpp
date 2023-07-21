#include "common.h"

#if CHAPTER==18
#include "opengl18h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_18_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;
#endif

#ifdef CHAPTER_18_5_1
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLint k;
	GLubyte texLine[16];
	for (k = 0; k <= 2; k += 2)
	{
		texLine[4 * k] = 0;
		texLine[4 * k + 1] = 255;
		texLine[4 * k + 2] = 0;
		texLine[4 * k + 3] = 255;
	}

	for (k = 1; k <= 3; k += 2)
	{
		texLine[4 * k] = 255;
		texLine[4 * k + 1] = 0;
		texLine[4 * k + 2] = 0;
		texLine[4 * k + 3] = 255;
	}

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, texLine);

	glEnable(GL_TEXTURE_1D);

	glBegin(GL_LINES);
	glTexCoord1f(0.0);
	GLfloat endPt1[] = { -20, 0, 0 };
	glVertex3fv(endPt1);
	glTexCoord1f(1.0);
	GLfloat endPt2[] = { 20, 0, 0 };
	glVertex3fv(endPt2);
	glEnd();

	glDisable(GL_TEXTURE_1D);


	glFlush();
}

void code_18_5_1()
{
	camera = new Camera(Point(0, 0, 100), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f));
}
#endif

#ifdef CHAPTER_18_5_2
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLubyte texArray[32][32][4];
	for (int y = 0; y < 32; y ++)
	{
		for (int x = 0; x < 32; x++)
		{
			if (y == 0 || y == 10 || y == 20)
			{
				texArray[y][x][0] = 255;
				texArray[y][x][1] = 0;
				texArray[y][x][2] = 0;
				texArray[y][x][3] = 255;
			}
			else
			{
				texArray[y][x][0] = 255;
				texArray[y][x][1] = 255;
				texArray[y][x][2] = 255;
				texArray[y][x][3] = 255;
			}			
		}
	}

	GLfloat vertex1[] = { -20, 0, 0 };
	GLfloat vertex2[] = { 20, 0, 0 };
	GLfloat vertex3[] = { 20, 30, 0 };
	GLfloat vertex4[] = { -20, 30, 0 };

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, texArray);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3fv(vertex1);
		glTexCoord2f(1.0, 0.0); glVertex3fv(vertex2);
		glTexCoord2f(1.0, 1.0); glVertex3fv(vertex3);
		glTexCoord2f(0.0, 1.0); glVertex3fv(vertex4);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glFlush();
}

void code_18_5_2()
{
	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_18_Test
static GLint ImageHeight;
static GLint ImageWidth;
static GLubyte* pixelData;
static GLint ImageColorFormat;
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat vertex1[] = { -ImageWidth / 2, -ImageHeight / 2, 0 };
	GLfloat vertex2[] = { ImageWidth / 2, -ImageHeight / 2, 0 };
	GLfloat vertex3[] = { ImageWidth / 2, ImageHeight / 2, 0 };
	GLfloat vertex4[] = { -ImageWidth / 2, ImageHeight / 2, 0 };

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3fv(vertex1);
	glTexCoord2f(1.0, 0.0); glVertex3fv(vertex2);
	glTexCoord2f(1.0, 1.0); glVertex3fv(vertex3);
	glTexCoord2f(0.0, 1.0); glVertex3fv(vertex4);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
	glFlush();
}

//void readBmpFile(const char* filePath)
//{
//	FILE* pFile;
//	fopen_s(&pFile, filePath, "r+b");
//	
//	if (pFile == 0)  
//		exit(0);
//
//	fseek(pFile, 0, SEEK_END);
//	int totalLength = ftell(pFile);
//	rewind(pFile);
//
//	fseek(pFile, 0x0012, SEEK_SET);
//	fread(&ImageWidth, 4, 1, pFile);
//	fseek(pFile, 0x0016, SEEK_SET);
//	fread(&ImageHeight, 4, 1, pFile);
//
//	//PixelLength = ImageWidth * 3;
//	//while (PixelLength % 4 != 0)
//	//	++PixelLength;
//	//PixelLength *= ImageHeight;
//	//PixelLength += 54; // 头信息长度
//
//	pixelData = (GLubyte*)malloc(totalLength);
//	if (pixelData == 0)  
//		exit(0);
//	fseek(pFile, 54, SEEK_SET); // 头信息长度
//	fread(pixelData, totalLength-54, 1, pFile);
//
//	// 将BGR转为RGB格式
//	for (int y = 0; y < ImageHeight; y++)
//	{
//		for (int x = 0; x < ImageWidth; x++)
//		{
//			GLubyte* p = pixelData + (y * ImageWidth + x) * 3;
//			GLubyte b = *p;
//			GLubyte g = *(p + 1);
//			GLubyte r = *(p + 2);
//			*p = r;
//			*(p + 1) = g;
//			*(p + 2) = b;
//		}
//	}
//}

void readBmpFile(const char* filePath)
{
	int iw, ih, n;
	stbi_set_flip_vertically_on_load(1);
	// 加载图片获取宽、高、颜色通道信息
	unsigned char* idata = stbi_load(filePath, &ImageWidth, &ImageHeight, &n, 0);

	if (n == 3)
		ImageColorFormat = GL_RGB;
	else if (n == 4)
		ImageColorFormat = GL_RGBA;
	else
		ImageColorFormat = GL_RGB;

	pixelData = idata;
}

void code_18_Test()
{
	camera = new Camera(Point(0, 0, 1000), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f), 1000);

	readBmpFile("Texture/TestBmp.bmp");

	glTexImage2D(GL_TEXTURE_2D, 0, ImageColorFormat, ImageWidth, ImageHeight, 0, ImageColorFormat, GL_UNSIGNED_BYTE, pixelData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(pixelData);
}
#endif

#ifdef CHAPTER_18_Test_2
struct TextureInfo
{
	GLint width;
	GLint height;
	GLint channels;
	GLint colorFormat;
	GLubyte* pixelData;
};
GLuint texName[6];
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// 0,1,2,3为立方体前面4个点，4,5,6,7为立方体后面4个点
	GLfloat vertex[8][3] = { {-100,-100,100}, {100,-100,100}, {100,100,100}, {-100,100,100},
	{-100,-100,-100}, {100,-100,-100}, {100,100,-100}, {-100,100,-100} };

	int face[6][4] = {
		{0, 1, 2, 3}, // 前
		{1, 5, 6, 2}, // 右
		{3, 2, 6, 7}, // 上
		{4, 0, 3, 7}, // 左
		{4, 5, 1, 0}, // 下
		{5, 4, 7, 6}, // 后
	};

	// 映射纹理
	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3fv(vertex[face[i][0]]);
		glTexCoord2f(1.0, 0.0); glVertex3fv(vertex[face[i][1]]);
		glTexCoord2f(1.0, 1.0); glVertex3fv(vertex[face[i][2]]);
		glTexCoord2f(0.0, 1.0); glVertex3fv(vertex[face[i][3]]);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
	glFlush();
}

//TextureInfo readBmpFile(const char* filePath)
//{
//	FILE* pFile;
//	fopen_s(&pFile, filePath, "r+b");
//
//	if (pFile == 0)
//		exit(0);
//
//	TextureInfo info;
//
//	fseek(pFile, 0, SEEK_END);
//	int totalLength = ftell(pFile);
//	rewind(pFile);
//
//	fseek(pFile, 0x0012, SEEK_SET);
//	fread(&info.width, 4, 1, pFile);
//	fseek(pFile, 0x0016, SEEK_SET);
//	fread(&info.height, 4, 1, pFile);
//
//	//int PixelLength = info.width * 3;
//	//while (PixelLength % 4 != 0)
//	//	++PixelLength;
//	//PixelLength *= info.height;
//	//PixelLength += 54; // 头信息长度
//
//	info.pixelData = (GLubyte*)malloc(totalLength);
//	if (info.pixelData == 0)
//		exit(0);
//	fseek(pFile, 54, SEEK_SET); // 头信息长度
//	fread(info.pixelData, totalLength - 54, 1, pFile);
//
//	// 将BGR转为RGB格式
//	for (int y = 0; y < info.height; y++)
//	{
//		for (int x = 0; x < info.width; x++)
//		{
//			GLubyte* p = info.pixelData + (y * info.width + x) * 3;
//			GLubyte b = *p;
//			GLubyte g = *(p + 1);
//			GLubyte r = *(p + 2);
//			*p = r;
//			*(p + 1) = g;
//			*(p + 2) = b;
//		}
//	}
//
//	return info;
//}

TextureInfo readBmpFile(const char* filePath)
{
	int iw, ih, n;
	TextureInfo info;
	stbi_set_flip_vertically_on_load(1);
	// 加载图片获取宽、高、颜色通道信息
	info.pixelData = stbi_load(filePath, &info.width, &info.height, &info.channels, 0);
	
	if (info.channels == 3)
		info.colorFormat = GL_RGB;
	else if(info.channels == 4)
		info.colorFormat = GL_RGBA;
	else
		info.colorFormat = GL_RGB;

	return info;
}

void code_18_Test_2()
{
	camera = new Camera(Point(0, 0, 1000), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f), 1000);

	TextureInfo info[6];
	info[0] = readBmpFile("Texture/1.bmp");
	info[1] = readBmpFile("Texture/2.bmp");
	info[2] = readBmpFile("Texture/3.bmp");
	info[3] = readBmpFile("Texture/4.bmp");
	info[4] = readBmpFile("Texture/5.bmp");
	info[5] = readBmpFile("Texture/6.bmp");

	glGenTextures(6, texName); // 创建6个纹理对象，并将名字赋值给texName数组
	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]); // 激活纹理对象texName[i]，并将GL_TEXTURE_2D通道与该纹理对象绑定
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, info[i].colorFormat, info[i].width, info[i].height, 0, info[i].colorFormat, GL_UNSIGNED_BYTE, info[i].pixelData);
		stbi_image_free(info[i].pixelData);
	}
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_18_COMMON

#ifdef CHAPTER_18_COMMON
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

#ifdef CHAPTER_18_5_2
	widthMin = -400;
	widthMax = 400;
	heightMin = -300;
	heightMax = 300;
#endif

#ifdef CHAPTER_18_Test
	widthMin = -400;
	widthMax = 400;
	heightMin = -300;
	heightMax = 300;
#endif

#ifdef CHAPTER_18_Test_2
	widthMin = -400;
	widthMax = 400;
	heightMin = -300;
	heightMax = 300;
#endif

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

#ifdef CHAPTER_18_5_1
	code_18_5_1();
#endif

#ifdef CHAPTER_18_5_2
	code_18_5_2();
#endif

#ifdef CHAPTER_18_Test
	code_18_Test();
#endif

#ifdef CHAPTER_18_Test_2
	code_18_Test_2();
#endif

	glutMainLoop();
}
#endif


#endif 


