#include "common.h"

#if CHAPTER==22
#include "opengl22h.h"

#ifdef OPENGL_CAMERA
#include "openglCamera.h"
Camera* camera = nullptr;
#endif

#ifdef CHAPTER_22_COMMON
float winWidth = 800, winHeight = 600;
float widthMin = 0, widthMax = winWidth, heightMin = 0, heightMax = winHeight;


//////////////////////////////////////////////////////////////////////////
// 判断浮点数相等
float floatEqualValue = 0.0001f;
inline bool Equal(float f1, float f2) { return std::abs(f1 - f2) < floatEqualValue; }

//////////////////////////////////////////////////////////////////////////
// 矩阵
struct Matrix
{
	Matrix(int row, int col)
	{
		_data.assign(row, std::vector<float>(col, 0));
		_row = row;
		_col = col;
	}
	std::vector<float>& operator [](int row)
	{
		return _data[row];
	}
	operator GLfloat* ()
	{
		_elementData.clear();
		for (int j = 0; j < _col; j++)
		{
			for (int i = 0; i < _row; i++)
			{
				_elementData.push_back(_data[i][j]);
			}
		}
		return &_elementData[0];
	}
	std::vector<std::vector<float>> _data;
	std::vector<float> _elementData;
	int _row;
	int _col;
};
Matrix operator *(Matrix& m1, Matrix& m2)
{
	assert(m1._col == m2._row);

	Matrix ret(m1._row, m2._col);
	for (int row = 0; row < m1._row; row++)
	{
		for (int col = 0; col < m2._col; col++)
		{
			ret[row][col] = 0;
			for (int i = 0; i < m1._col; i++)
			{
				ret[row][col] += m1[row][i] * m2[i][col];
			}
		}
	}
	return ret;
}
void matrixSetIdentity(Matrix& m)
{
	for (int row = 0; row < m._row; row++)
		for (int col = 0; col < m._col; col++)
			m[row][col] = (row == col);
}
bool operator ==(Matrix& m1, Matrix& m2)
{
	if (!(m1._row == m2._row && m1._col == m2._col))
		return false;

	for (int row = 0; row < m1._row; row++)
	{
		for (int col = 0; col < m1._col; col++)
		{
			if (!Equal(m1[row][col], m2[row][col]))
			{
				return false;
			}
		}
	}
	return true;
}
void transformPoints(Matrix& m, std::vector<Point>& points)
{
	Matrix point(4, 1);
	Matrix temp(4, 1);
	for (auto& p : points)
	{
		point[0][0] = p.x;
		point[1][0] = p.y;
		point[2][0] = p.z;
		point[3][0] = p.h;
		auto temp = m * point;
		p.x = temp[0][0];
		p.y = temp[1][0];
		p.z = temp[2][0];
		p.h = temp[3][0];
	}
}
void transformPoint(Matrix& m, Point& point)
{
	Matrix _point(4, 1);
	Matrix temp(4, 1);
	_point[0][0] = point.x;
	_point[1][0] = point.y;
	_point[2][0] = point.z;
	_point[3][0] = point.h;
	temp = m * _point;
	point.x = temp[0][0];
	point.y = temp[1][0];
	point.z = temp[2][0];
	point.h = temp[3][0];
}
//////////////////////////////////////////////////////////////////////////
// 三维几何变换
// 平移
Matrix translateMatrix(float tx, float ty, float tz)
{
	Matrix m(4, 4);
	matrixSetIdentity(m);
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	return m;
}
// 任意点缩放
Matrix scaleMatrix(Point p0, float sx, float sy, float sz)
{
	// 基于指定点缩放
	Matrix ret(4, 4);
	matrixSetIdentity(ret);
	ret[0][0] = sx;
	ret[0][3] = p0.x * (1 - sx);
	ret[1][1] = sy;
	ret[1][3] = p0.y * (1 - sy);
	ret[2][2] = sz;
	ret[2][3] = p0.z * (1 - sz);
	return ret;
}

#endif

#ifdef CHAPTER_22_Test
class Shader;
Shader* cubeShader = nullptr;
Shader* lightShader = nullptr;
unsigned int VAO, lightVAO;
static GLfloat angle = 0.0f;
float lightX = 0.f, lightY = 0.f, lightZ = 0.f;
static clock_t lastTick = 0.0;
static float vertices_cubeHaveNormal[] = 
{
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
GLchar* ReadTextFile(const char* name)
{
	FILE* fp = nullptr;
	GLchar* content = NULL;
	int count = 0;

	if (name == NULL) return NULL;

	fopen_s(&fp, name, "rt");
	if (fp == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	if (count > 0)
	{
		content = (GLchar*)malloc(sizeof(char) * (count + 1));
		if (content != NULL)
		{
			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}
	}
	fclose(fp);
	return content;
}
class Shader
{
public:
	// 程序ID
	unsigned int ID;

	// 构造器读取并构建着色器
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		GLchar *vertSource, *fragSource;
		vertSource = ReadTextFile(vertexPath);
		fragSource = ReadTextFile(fragmentPath);

		//创建顶点着色器
		unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, (const GLchar**)&vertSource, nullptr);
		free(vertSource);
		glCompileShader(vertShader);
		GLint status;
		char infoLog[512];
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
			std::cout << "ERROR::VERTEX_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//创建片段着色器
		unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, (const GLchar**)&fragSource, nullptr);
		free(fragSource);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
			std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//创建Program
		GLuint program;
		program = glCreateProgram();
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status != GL_TRUE)
		{
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADERPROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		ID = program;
	}
	// 使用/激活程序
	void use()
	{
		glUseProgram(ID);
	}
	// uniform工具函数
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void set3Float(const std::string& name, float f1, float f2, float f3) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3);
	}
	void set4Float(const std::string& name, float f1, float f2, float f3, float f4) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3, f4);
	}
	void set4x4Matrix(const std::string& name, float matrix[4][4]) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, (const float*)matrix);
	}
};


void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	auto curTick = (float)clock() / CLOCKS_PER_SEC;

	float distance = 2.0f;
	Point lightPos;
	lightPos.y = sin(curTick) * distance;
	lightPos.z = -(cos(curTick) * distance);
	lightPos.x = sin(curTick) * distance;
	printf("%f, %f, %f\n", lightPos.x, lightPos.y, lightPos.z);

	cubeShader->use();

	Matrix modelMatrix(4, 4);
	matrixSetIdentity(modelMatrix);

	float viewMatrix[4][4];
	camera->GetModelViewMatrix((float*)viewMatrix);

	float projectionMatrix[4][4];
	camera->GetProjectionMatrix((float*)projectionMatrix);
	
	cubeShader->set4x4Matrix("modelMat", (float (*)[4])(float*)modelMatrix);
	cubeShader->set4x4Matrix("viewMat", viewMatrix);
	cubeShader->set4x4Matrix("projectionMat", projectionMatrix);
	//设置cube颜色
	cubeShader->set3Float("cubeColor", 1.0f, 0.5f, 0.31f);
	//设置光源颜色
	cubeShader->set3Float("lightColor", 1.0f, 1.0f, 1.0f);
	//设置光源世界坐标
	cubeShader->set3Float("lightPos", lightPos.x, lightPos.y, lightPos.z);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//设置观察者(摄像机)坐标
	cubeShader->set3Float("viewPos", camera->_viewP0.x, camera->_viewP0.y, camera->_viewP0.z);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	//光源shader
	lightShader->use();

	Matrix lightModelMatrix(4, 4);
	matrixSetIdentity(lightModelMatrix);
	lightModelMatrix = translateMatrix(lightPos.x, lightPos.y, lightPos.z) * scaleMatrix({ 0, 0, 0 }, 0.2, 0.2, 0.2) * lightModelMatrix;

	float lightViewMatrix[4][4];
	camera->GetModelViewMatrix((float*)lightViewMatrix);

	float lightProjectionMatrix[4][4];
	camera->GetProjectionMatrix((float*)lightProjectionMatrix);

	lightShader->set4x4Matrix("modelMat", (float(*)[4])(float*)lightModelMatrix);
	lightShader->set4x4Matrix("viewMat", lightViewMatrix);
	lightShader->set4x4Matrix("projectionMat", lightProjectionMatrix);

	//设置光源颜色
	lightShader->set3Float("lightColor", 1.0f, 1.0f, 1.0f);
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glutSwapBuffers();
	glFlush();
}

void update(void)
{
	glutPostRedisplay();
}

void code_22_Test()
{
	camera = new Camera(Point(0, 0, 10), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f), 10.f);

	glutIdleFunc(update);

	//创建顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//绑定缓冲对象，设定缓冲类型
	//将顶点数据复制到缓冲的内存中，并指定显卡如何管理数据，这里指定为GL_STATIC_DRAW
	//使用带法向量的顶点数组（为每个顶点手动配置法向量）
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cubeHaveNormal), vertices_cubeHaveNormal, GL_STATIC_DRAW);

	//创建VAO顶点数组对象
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	//法向量
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		//以顶点属性位置作为参数,启用顶点属性

	//创建light的VAO
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(
		float), (void*)0);
	glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//绑定缓冲对象，设定缓冲类型

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cubeShader = new Shader("Shader/cubew1.vert", "Shader/cubew1.frag");
	lightShader = new Shader("Shader/cubew1.vert", "Shader/lightw1.frag");
	
	glEnable(GL_DEPTH_TEST);
}
#endif

#ifdef CHAPTER_22_4_1
unsigned int VAO, lightVAO;
GLuint platShaderProgramID, lightShaderProgramID;
Point lightPosition(0.f, 3.f, 0.f, 1.f);
static float plat_VerticesAndNormal[] =
{
	-2.f, -0.5f, -2.f,  0.0f,  0.0f, -1.0f,
	 2.f, -0.5f, -2.f,  0.0f,  0.0f, -1.0f,
	 2.f,  0.5f, -2.f,  0.0f,  0.0f, -1.0f,
	 2.f,  0.5f, -2.f,  0.0f,  0.0f, -1.0f,
	-2.f,  0.5f, -2.f,  0.0f,  0.0f, -1.0f,
	-2.f, -0.5f, -2.f,  0.0f,  0.0f, -1.0f,

	-2.f, -0.5f,  2.f,  0.0f,  0.0f, 1.0f,
	 2.f, -0.5f,  2.f,  0.0f,  0.0f, 1.0f,
	 2.f,  0.5f,  2.f,  0.0f,  0.0f, 1.0f,
	 2.f,  0.5f,  2.f,  0.0f,  0.0f, 1.0f,
	-2.f,  0.5f,  2.f,  0.0f,  0.0f, 1.0f,
	-2.f, -0.5f,  2.f,  0.0f,  0.0f, 1.0f,

	-2.f,  0.5f,  2.f, -1.0f,  0.0f,  0.0f,
	-2.f,  0.5f, -2.f, -1.0f,  0.0f,  0.0f,
	-2.f, -0.5f, -2.f, -1.0f,  0.0f,  0.0f,
	-2.f, -0.5f, -2.f, -1.0f,  0.0f,  0.0f,
	-2.f, -0.5f,  2.f, -1.0f,  0.0f,  0.0f,
	-2.f,  0.5f,  2.f, -1.0f,  0.0f,  0.0f,

	 2.f,  0.5f,  2.f,  1.0f,  0.0f,  0.0f,
	 2.f,  0.5f, -2.f,  1.0f,  0.0f,  0.0f,
	 2.f, -0.5f, -2.f,  1.0f,  0.0f,  0.0f,
	 2.f, -0.5f, -2.f,  1.0f,  0.0f,  0.0f,
	 2.f, -0.5f,  2.f,  1.0f,  0.0f,  0.0f,
	 2.f,  0.5f,  2.f,  1.0f,  0.0f,  0.0f,

	-2.f, -0.5f, -2.f,  0.0f, -1.0f,  0.0f,
	 2.f, -0.5f, -2.f,  0.0f, -1.0f,  0.0f,
	 2.f, -0.5f,  2.f,  0.0f, -1.0f,  0.0f,
	 2.f, -0.5f,  2.f,  0.0f, -1.0f,  0.0f,
	-2.f, -0.5f,  2.f,  0.0f, -1.0f,  0.0f,
	-2.f, -0.5f, -2.f,  0.0f, -1.0f,  0.0f,

	-2.f,  0.5f, -2.f,  0.0f,  1.0f,  0.0f,
	 2.f,  0.5f, -2.f,  0.0f,  1.0f,  0.0f,
	 2.f,  0.5f,  2.f,  0.0f,  1.0f,  0.0f,
	 2.f,  0.5f,  2.f,  0.0f,  1.0f,  0.0f,
	-2.f,  0.5f,  2.f,  0.0f,  1.0f,  0.0f,
	-2.f,  0.5f, -2.f,  0.0f,  1.0f,  0.0f
};
GLchar* ReadTextFile(const char* name)
{
	FILE* fp = nullptr;
	GLchar* content = NULL;
	int count = 0;

	if (name == NULL) return NULL;

	fopen_s(&fp, name, "rt");
	if (fp == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	if (count > 0)
	{
		content = (GLchar*)malloc(sizeof(char) * (count + 1));
		if (content != NULL)
		{
			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}
	}
	fclose(fp);
	return content;
}
//void displayFcn(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glColor3f(1.0, 0.0, 0.0);
//	glEnable(GL_LIGHTING);// 打开光照处理功能
//
//	{
//		// 设置光源
//		GLfloat light_position[] = { lightPosition.x, lightPosition.y, lightPosition.z, 1.0f };
//		GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_AMBIENT 属性的值 R,G,B,A 
//		GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_DIFFUSE 属性的值 R,G,B,A
//		GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_SPECULAR 属性的值 R,G,B,A
//		glLightfv(GL_LIGHT0, GL_POSITION, light_position); // 设置 GL_POSITION (光源位置)属性 
//		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // 设置 GL_AMBIENT( 光源发出的光，经过非常多次反射，遗留在整个光照环境中的强度)
//		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);// 设置 GL_DIFFUSE(光源发出的光，照射到粗糙表面时，经过漫反射，所得到的光强度)
//		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);// 设置 GL_SPECULAR(光源发出的光，照射到光滑表面时，经过镜面反射，所得到的光强度) 
//
//		glEnable(GL_LIGHT0);// 开启光源
//	}
//
//	{
//		// 绘制灯光球体
//		// 定义球体材质并绘制球体（不与场景中光照产生交互，只有一个白色的自发光）
//		/* GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR 属性。这三个属性与光源的三个对应属性类似，每一属性都由四个值组成*/
//		GLfloat sphere_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//		GLfloat sphere_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//		GLfloat sphere_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//		GLfloat sphere_mat_emission[] = { 1.f, 1.0f, 1.0f, 1.0f };// 材质发光颜色 值
//		GLfloat sphere_mat_shininess = 0.0f;//  镜面指数 0 - 128 
//		glMaterialfv(GL_FRONT, GL_AMBIENT, sphere_mat_ambient); // 设置 GL_AMBIENT (光线照射到该材质上，经过很多次反射后最终遗留在环境中的光线强度)
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_mat_diffuse); // 设置 GL_DIFFUSE (光线照射到该材质上，经过漫反射后形成的光线强度)
//		glMaterialfv(GL_FRONT, GL_SPECULAR, sphere_mat_specular);// 设置 GL_SPECULAR (光线照射到该材质上，经过镜面反射后形成的光线强度)
//		glMaterialfv(GL_FRONT, GL_EMISSION, sphere_mat_emission); // 设置 GL_EMISSION (该属性由四个值组成，表示一种颜色。OpenGL 认为该材质本身就微微的向外发射光线，以至于眼睛感觉到它有这样的颜色，但这光线又比较微弱，以至于不会影响到其它物体的颜色)
//		glMaterialf(GL_FRONT, GL_SHININESS, sphere_mat_shininess); // 设置 GL_SHININESS  (该属性只有一个值，称为“镜面指数”，取值范围是 0 到 128。该值越小，表示材质越粗糙，点光源发射的光线照射到上面，也可以产生较大的亮点。该值越大，表示材质越类似于镜面，光源照射到上面后，产生较小的亮点。)
//
//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
//		glTranslatef(lightPosition.x, lightPosition.y, lightPosition.z);
//		glutSolidSphere(0.3, 40, 32);
//		glPopMatrix();
//	}
//
//	{
//		// 绘制立方体（作为默认光照效果参考）
//		GLfloat mat_ambient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
//		GLfloat mat_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
//		GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//		GLfloat mat_shininess = 30.0f;
//		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); 
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
//		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
//
//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
//		glTranslatef(5, 0, 0);
//		glutSolidCube(2);
//		glPopMatrix();
//	}
//
//	{
//		// 绘制地面shader
//		glUseProgram(platShaderProgramID);
//		glBindVertexArray(VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//		glUseProgram(0);
//		//glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindVertexArray(0);
//	}
//
//	glutSwapBuffers();
//	glFlush();
//}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glEnable(GL_LIGHTING);// 打开光照处理功能

	GLfloat light_position[] = { lightPosition.x, lightPosition.y, lightPosition.z, 1.0f };
	GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_AMBIENT 属性的值 R,G,B,A 
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_DIFFUSE 属性的值 R,G,B,A
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_SPECULAR 属性的值 R,G,B,A

	{
		// 设置光源	
		glLightfv(GL_LIGHT0, GL_POSITION, light_position); // 设置 GL_POSITION (光源位置)属性 
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // 设置 GL_AMBIENT( 光源发出的光，经过非常多次反射，遗留在整个光照环境中的强度)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);// 设置 GL_DIFFUSE(光源发出的光，照射到粗糙表面时，经过漫反射，所得到的光强度)
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);// 设置 GL_SPECULAR(光源发出的光，照射到光滑表面时，经过镜面反射，所得到的光强度) 

		glEnable(GL_LIGHT0);// 开启光源
	}

	{
		// 绘制灯光球体
		// 定义球体材质并绘制球体（不与场景中光照产生交互，只有一个白色的自发光）
		/* GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR 属性。这三个属性与光源的三个对应属性类似，每一属性都由四个值组成*/
		GLfloat sphere_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sphere_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sphere_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sphere_mat_emission[] = { 1.f, 1.0f, 1.0f, 1.0f };// 材质发光颜色 值
		GLfloat sphere_mat_shininess = 0.0f;//  镜面指数 0 - 128 
		glMaterialfv(GL_FRONT, GL_AMBIENT, sphere_mat_ambient); // 设置 GL_AMBIENT (光线照射到该材质上，经过很多次反射后最终遗留在环境中的光线强度)
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_mat_diffuse); // 设置 GL_DIFFUSE (光线照射到该材质上，经过漫反射后形成的光线强度)
		glMaterialfv(GL_FRONT, GL_SPECULAR, sphere_mat_specular);// 设置 GL_SPECULAR (光线照射到该材质上，经过镜面反射后形成的光线强度)
		glMaterialfv(GL_FRONT, GL_EMISSION, sphere_mat_emission); // 设置 GL_EMISSION (该属性由四个值组成，表示一种颜色。OpenGL 认为该材质本身就微微的向外发射光线，以至于眼睛感觉到它有这样的颜色，但这光线又比较微弱，以至于不会影响到其它物体的颜色)
		glMaterialf(GL_FRONT, GL_SHININESS, sphere_mat_shininess); // 设置 GL_SHININESS  (该属性只有一个值，称为“镜面指数”，取值范围是 0 到 128。该值越小，表示材质越粗糙，点光源发射的光线照射到上面，也可以产生较大的亮点。该值越大，表示材质越类似于镜面，光源照射到上面后，产生较小的亮点。)

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(lightPosition.x, lightPosition.y, lightPosition.z);
		glutSolidSphere(0.3, 40, 32);
		glPopMatrix();
	}

	GLfloat mat_ambient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_shininess = 30.0f;

	{
		// 绘制立方体（作为默认光照效果参考）
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(5, 0, 0);
		glutSolidCube(2);
		glPopMatrix();
	}

	{
		// 绘制地面shader
		glUseProgram(platShaderProgramID);
		Matrix modelMatrix(4, 4);
		matrixSetIdentity(modelMatrix);

		float viewMatrix[4][4];
		camera->GetModelViewMatrix((float*)viewMatrix);

		float projectionMatrix[4][4];
		camera->GetProjectionMatrix((float*)projectionMatrix);

		glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "modelMat"), 1, GL_FALSE, (const float*)modelMatrix);
		glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "viewMat"), 1, GL_FALSE, (const float*)viewMatrix);
		glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "projectionMat"), 1, GL_FALSE, (const float*)projectionMatrix);

		glUniform3f(glGetUniformLocation(platShaderProgramID, "light_ambient"), light_ambient[0], light_ambient[1], light_ambient[2]);
		glUniform3f(glGetUniformLocation(platShaderProgramID, "light_diffuse"), light_diffuse[0], light_diffuse[1], light_diffuse[2]);
		glUniform3f(glGetUniformLocation(platShaderProgramID, "light_specular"), light_specular[0], light_specular[1], light_specular[2]);

		glUniform3f(glGetUniformLocation(platShaderProgramID, "mat_ambient"), mat_ambient[0], mat_ambient[1], mat_ambient[2]);
		glUniform3f(glGetUniformLocation(platShaderProgramID, "mat_diffuse"), mat_diffuse[0], mat_diffuse[1], mat_diffuse[2]);
		glUniform3f(glGetUniformLocation(platShaderProgramID, "mat_specular"), mat_specular[0], mat_specular[1], mat_specular[2]);
		glUniform1f(glGetUniformLocation(platShaderProgramID, "mat_shininess"), mat_shininess);

		glUniform3f(glGetUniformLocation(platShaderProgramID, "lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform3f(glGetUniformLocation(platShaderProgramID, "viewPos"), camera->_viewP0.x, camera->_viewP0.y, camera->_viewP0.z);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUseProgram(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
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
			lightPosition.z += 0.1;
		else
			lightPosition.y += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if (mod == GLUT_ACTIVE_SHIFT)
			lightPosition.z -= 0.1;
		else
			lightPosition.y -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		lightPosition.x -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		lightPosition.x += 0.1;
		break;
	default:
		break;
	}
	displayFcn();
}

GLint CreateShader(const char* vertPath, const char* fragPath)
{
	GLuint vertShader, fragShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLchar* vertSource, * fragSource;
	vertSource = ReadTextFile(vertPath);
	if (vertSource == nullptr)
	{
		fputs("Failed to read vertex shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	fragSource = ReadTextFile(fragPath);
	if (fragSource == nullptr)
	{
		fputs("Failed to read fragment shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	glShaderSource(vertShader, 1, (const GLchar**)&vertSource, nullptr);
	glShaderSource(fragShader, 1, (const GLchar**)&fragSource, nullptr);
	free(vertSource);
	free(fragSource);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in vertex shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in fragment shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint programID = glCreateProgram();
	glAttachShader(programID, vertShader);
	glAttachShader(programID, fragShader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error when linking shader program\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint length;
	GLsizei num;
	char* log;
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetShaderInfoLog(vertShader, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetProgramInfoLog(programID, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}

	//glUseProgram(programID);
	return programID;
}

void code_22_4_1()
{
	camera = new Camera(Point(0, 10, 10), Rotator(-45, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f), 10.f);

	//创建顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//绑定缓冲对象，设定缓冲类型
	//将顶点数据复制到缓冲的内存中，并指定显卡如何管理数据，这里指定为GL_STATIC_DRAW
	//使用带法向量的顶点数组（为每个顶点手动配置法向量）
	glBufferData(GL_ARRAY_BUFFER, sizeof(plat_VerticesAndNormal), plat_VerticesAndNormal, GL_STATIC_DRAW);

	//创建VAO顶点数组对象
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	//法向量
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		//以顶点属性位置作为参数,启用顶点属性

	//创建light的VAO
	//glGenVertexArrays(1, &lightVAO);
	//glBindVertexArray(lightVAO);
	////顶点
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);		//绑定缓冲对象，设定缓冲类型

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	platShaderProgramID = CreateShader("Shader/PhongCustomVertexShader.vert", "Shader/PhongCustomFragmentShader.frag");
	//platShaderProgramID = CreateShader("Shader/PhongVertexShader.vert", "Shader/PhongFragmentShader.frag");

	glutSpecialFunc(&SpecialKey);
}
#endif


#ifdef CHAPTER_22_4_2
static unsigned int VAO;
static GLuint platShaderProgramID;
static GLint ImageHeight;
static GLint ImageWidth;
static GLubyte* pixelData;
static GLint ImageColorFormat;
static GLuint texture;

GLchar* ReadTextFile(const char* name)
{
	FILE* fp = nullptr;
	GLchar* content = NULL;
	int count = 0;

	if (name == NULL) return NULL;

	fopen_s(&fp, name, "rt");
	if (fp == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	if (count > 0)
	{
		content = (GLchar*)malloc(sizeof(char) * (count + 1));
		if (content != NULL)
		{
			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}
	}
	fclose(fp);
	return content;
}

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


void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	// 绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(glGetUniformLocation(platShaderProgramID, "ourTexture"), 0); // 纹理单元0是默认激活的，所以这里不用显示复制

	glUseProgram(platShaderProgramID);
	Matrix modelMatrix(4, 4);
	matrixSetIdentity(modelMatrix);

	float viewMatrix[4][4];
	camera->GetModelViewMatrix((float*)viewMatrix);

	float projectionMatrix[4][4];
	camera->GetProjectionMatrix((float*)projectionMatrix);

	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "modelMat"), 1, GL_FALSE, (const float*)modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "viewMat"), 1, GL_FALSE, (const float*)viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "projectionMat"), 1, GL_FALSE, (const float*)projectionMatrix);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glutSwapBuffers();
	glFlush();
}

GLint CreateShader(const char* vertPath, const char* fragPath)
{
	GLuint vertShader, fragShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLchar* vertSource, * fragSource;
	vertSource = ReadTextFile(vertPath);
	if (vertSource == nullptr)
	{
		fputs("Failed to read vertex shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	fragSource = ReadTextFile(fragPath);
	if (fragSource == nullptr)
	{
		fputs("Failed to read fragment shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	glShaderSource(vertShader, 1, (const GLchar**)&vertSource, nullptr);
	glShaderSource(fragShader, 1, (const GLchar**)&fragSource, nullptr);
	free(vertSource);
	free(fragSource);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in vertex shader compilation\n", stderr);
		exit(EXIT_FAILURE);}
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in fragment shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint programID = glCreateProgram();
	glAttachShader(programID, vertShader);
	glAttachShader(programID, fragShader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error when linking shader program\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint length;
	GLsizei num;
	char* log;
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetShaderInfoLog(vertShader, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetProgramInfoLog(programID, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}

	//glUseProgram(programID);
	return programID;
}



void code_22_4_2()
{
	camera = new Camera(Point(0, 0, 1000), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f), 1000);

	// 加载纹理
	glGenTextures(1, &texture);
	// 将纹理对象texture绑定到GL_TEXTURE_2D上，往后对GL_TEXTURE_2D的操作都将对texture执行
	glBindTexture(GL_TEXTURE_2D, texture);
	// 设置纹理在不同轴的环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理的纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载纹理图片
	readBmpFile("Texture/TestBmp.bmp");
	// 将纹理图片保存到绑定GL_TEXTURE_2D的对象上，以字符数组形式
	glTexImage2D(GL_TEXTURE_2D, 0, ImageColorFormat, ImageWidth, ImageHeight, 0, ImageColorFormat, GL_UNSIGNED_BYTE, pixelData);
	// 生成GL_TEXTURE_2D绑定对象的多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	// 释放纹理图片内存，解绑纹理texture
	stbi_image_free(pixelData);
	glBindTexture(GL_TEXTURE_2D, 0);

	//创建顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//绑定缓冲对象，设定缓冲类型
	//将顶点数据复制到缓冲的内存中，并指定显卡如何管理数据，这里指定为GL_STATIC_DRAW
	float vertices[] =
	{
	   // 顶点位置							  // 顶点颜色         // 纹理坐标
		ImageWidth/2,  ImageHeight/2, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
	   -ImageWidth/2,  ImageHeight/2, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top Left 
		ImageWidth/2, -ImageHeight/2, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right

	   -ImageWidth/2,  ImageHeight/2, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top Left 
	   -ImageWidth/2, -ImageHeight/2, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		ImageWidth/2, -ImageHeight/2, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right

	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建VAO顶点数组对象
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	//颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		//以顶点属性位置作为参数,启用顶点属性
	//纹理坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);		//以顶点属性位置作为参数,启用顶点属性

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glEnable(GL_DEPTH_TEST);

	platShaderProgramID = CreateShader("Shader/TextureMappingVertexShader.vert", "Shader/TextureMappingFragmentShader.frag");
}
#endif

#ifdef CHAPTER_22_4_2_Test1
static unsigned int VAO;
static GLuint platShaderProgramID;

struct TextureInfo
{
	GLint width;
	GLint height;
	GLint channels;
	GLint colorFormat;
	GLubyte* pixelData;
};
GLuint texName[6];

GLchar* ReadTextFile(const char* name)
{
	FILE* fp = nullptr;
	GLchar* content = NULL;
	int count = 0;

	if (name == NULL) return NULL;

	fopen_s(&fp, name, "rt");
	if (fp == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	if (count > 0)
	{
		content = (GLchar*)malloc(sizeof(char) * (count + 1));
		if (content != NULL)
		{
			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}
	}
	fclose(fp);
	return content;
}

TextureInfo readBmpFile(const char* filePath)
{
	int iw, ih, n;
	TextureInfo info;
	stbi_set_flip_vertically_on_load(1);
	// 加载图片获取宽、高、颜色通道信息
	info.pixelData = stbi_load(filePath, &info.width, &info.height, &info.channels, 0);

	if (info.channels == 3)
		info.colorFormat = GL_RGB;
	else if (info.channels == 4)
		info.colorFormat = GL_RGBA;
	else
		info.colorFormat = GL_RGB;

	return info;
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	glUseProgram(platShaderProgramID);
	Matrix modelMatrix(4, 4);
	matrixSetIdentity(modelMatrix);

	float viewMatrix[4][4];
	camera->GetModelViewMatrix((float*)viewMatrix);

	float projectionMatrix[4][4];
	camera->GetProjectionMatrix((float*)projectionMatrix);

	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "modelMat"), 1, GL_FALSE, (const float*)modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "viewMat"), 1, GL_FALSE, (const float*)viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "projectionMat"), 1, GL_FALSE, (const float*)projectionMatrix);

	glBindVertexArray(VAO);
	//glUniform1i(glGetUniformLocation(platShaderProgramID, "ourTexture"), 0); // 纹理单元0是默认激活的，所以这里不用显示复制
	for (int i = 0; i < 6; i++)
	{
		// 绑定纹理
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glDrawArrays(GL_TRIANGLES, i * 6, 6);
	}
	
	glUseProgram(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glutSwapBuffers();
	glFlush();
}

GLint CreateShader(const char* vertPath, const char* fragPath)
{
	GLuint vertShader, fragShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLchar* vertSource, * fragSource;
	vertSource = ReadTextFile(vertPath);
	if (vertSource == nullptr)
	{
		fputs("Failed to read vertex shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	fragSource = ReadTextFile(fragPath);
	if (fragSource == nullptr)
	{
		fputs("Failed to read fragment shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	glShaderSource(vertShader, 1, (const GLchar**)&vertSource, nullptr);
	glShaderSource(fragShader, 1, (const GLchar**)&fragSource, nullptr);
	free(vertSource);
	free(fragSource);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in vertex shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in fragment shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint programID = glCreateProgram();
	glAttachShader(programID, vertShader);
	glAttachShader(programID, fragShader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error when linking shader program\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint length;
	GLsizei num;
	char* log;
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetShaderInfoLog(vertShader, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetProgramInfoLog(programID, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}

	//glUseProgram(programID);
	return programID;
}



void code_22_4_2_Test1()
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, info[i].colorFormat, info[i].width, info[i].height, 0, info[i].colorFormat, GL_UNSIGNED_BYTE, info[i].pixelData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(info[i].pixelData);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// 0,1,2,3为立方体前面4个点，4,5,6,7为立方体后面4个点
	GLfloat vertex[8][3] = { {-100,-100,100}, {100,-100,100}, {100,100,100}, {-100,100,100},
	{-100,-100,-100}, {100,-100,-100}, {100,100,-100}, {-100,100,-100} };

	//创建顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//绑定缓冲对象，设定缓冲类型
	//将顶点数据复制到缓冲的内存中，并指定显卡如何管理数据，这里指定为GL_STATIC_DRAW

	float vertices[] =
	{
		// 顶点位置							      // 纹理坐标
/*前*/	vertex[2][0], vertex[2][1], vertex[2][2], 1.0f, 1.0f,
		vertex[3][0], vertex[3][1], vertex[3][2], 0.0f, 1.0f,
		vertex[1][0], vertex[1][1], vertex[1][2], 1.0f, 0.0f,
		vertex[3][0], vertex[3][1], vertex[3][2], 0.0f, 1.0f,
		vertex[0][0], vertex[0][1], vertex[0][2], 0.0f, 0.0f,
		vertex[1][0], vertex[1][1], vertex[1][2], 1.0f, 0.0f,

/*右*/	vertex[6][0], vertex[6][1], vertex[6][2], 1.0f, 1.0f,
		vertex[2][0], vertex[2][1], vertex[2][2], 0.0f, 1.0f,
		vertex[5][0], vertex[5][1], vertex[5][2], 1.0f, 0.0f,
		vertex[2][0], vertex[2][1], vertex[2][2], 0.0f, 1.0f,
		vertex[1][0], vertex[1][1], vertex[1][2], 0.0f, 0.0f,
		vertex[5][0], vertex[5][1], vertex[5][2], 1.0f, 0.0f,

/*上*/	vertex[6][0], vertex[6][1], vertex[6][2], 1.0f, 1.0f,
		vertex[7][0], vertex[7][1], vertex[7][2], 0.0f, 1.0f,
		vertex[2][0], vertex[2][1], vertex[2][2], 1.0f, 0.0f,
		vertex[7][0], vertex[7][1], vertex[7][2], 0.0f, 1.0f,
		vertex[3][0], vertex[3][1], vertex[3][2], 0.0f, 0.0f,
		vertex[2][0], vertex[2][1], vertex[2][2], 1.0f, 0.0f,

/*左*/	vertex[3][0], vertex[3][1], vertex[3][2], 1.0f, 1.0f,
		vertex[7][0], vertex[7][1], vertex[7][2], 0.0f, 1.0f,
		vertex[0][0], vertex[0][1], vertex[0][2], 1.0f, 0.0f,
		vertex[7][0], vertex[7][1], vertex[7][2], 0.0f, 1.0f,
		vertex[4][0], vertex[4][1], vertex[4][2], 0.0f, 0.0f,
		vertex[0][0], vertex[0][1], vertex[0][2], 1.0f, 0.0f,

/*下*/	vertex[1][0], vertex[1][1], vertex[1][2], 1.0f, 1.0f,
		vertex[0][0], vertex[0][1], vertex[0][2], 0.0f, 1.0f,
		vertex[5][0], vertex[5][1], vertex[5][2], 1.0f, 0.0f,
		vertex[0][0], vertex[0][1], vertex[0][2], 0.0f, 1.0f,
		vertex[4][0], vertex[4][1], vertex[4][2], 0.0f, 0.0f,
		vertex[5][0], vertex[5][1], vertex[5][2], 1.0f, 0.0f,

/*后*/	vertex[7][0], vertex[7][1], vertex[7][2], 1.0f, 1.0f,
		vertex[6][0], vertex[6][1], vertex[6][2], 0.0f, 1.0f,
		vertex[4][0], vertex[4][1], vertex[4][2], 1.0f, 0.0f,
		vertex[6][0], vertex[6][1], vertex[6][2], 0.0f, 1.0f,
		vertex[5][0], vertex[5][1], vertex[5][2], 0.0f, 0.0f,
		vertex[4][0], vertex[4][1], vertex[4][2], 1.0f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建VAO顶点数组对象
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	//纹理坐标
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		//以顶点属性位置作为参数,启用顶点属性

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	platShaderProgramID = CreateShader("Shader/CubeTextureMappingVertexShader.vert", "Shader/CubeTextureMappingFragmentShader.frag");
}
#endif

#ifdef CHAPTER_22_4_2_Test2
static unsigned int VAO;
static GLuint platShaderProgramID;

struct TextureInfo
{
	GLint width;
	GLint height;
	GLint channels;
	GLint colorFormat;
	GLubyte* pixelData;
};
GLuint texName[2];

GLchar* ReadTextFile(const char* name)
{
	FILE* fp = nullptr;
	GLchar* content = NULL;
	int count = 0;

	if (name == NULL) return NULL;

	fopen_s(&fp, name, "rt");
	if (fp == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	if (count > 0)
	{
		content = (GLchar*)malloc(sizeof(char) * (count + 1));
		if (content != NULL)
		{
			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}
	}
	fclose(fp);
	return content;
}

TextureInfo readBmpFile(const char* filePath)
{
	int iw, ih, n;
	TextureInfo info;
	stbi_set_flip_vertically_on_load(1);
	// 加载图片获取宽、高、颜色通道信息
	info.pixelData = stbi_load(filePath, &info.width, &info.height, &info.channels, 0);

	if (info.channels == 3)
		info.colorFormat = GL_RGB;
	else if (info.channels == 4)
		info.colorFormat = GL_RGBA;
	else
		info.colorFormat = GL_RGB;

	return info;
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	glUseProgram(platShaderProgramID);
	Matrix modelMatrix(4, 4);
	matrixSetIdentity(modelMatrix);

	float viewMatrix[4][4];
	camera->GetModelViewMatrix((float*)viewMatrix);

	float projectionMatrix[4][4];
	camera->GetProjectionMatrix((float*)projectionMatrix);

	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "modelMat"), 1, GL_FALSE, (const float*)modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "viewMat"), 1, GL_FALSE, (const float*)viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "projectionMat"), 1, GL_FALSE, (const float*)projectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glUniform1i(glGetUniformLocation(platShaderProgramID, "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glUniform1i(glGetUniformLocation(platShaderProgramID, "ourTexture2"), 1);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glUseProgram(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glutSwapBuffers();
	glFlush();
}

GLint CreateShader(const char* vertPath, const char* fragPath)
{
	GLuint vertShader, fragShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLchar* vertSource, * fragSource;
	vertSource = ReadTextFile(vertPath);
	if (vertSource == nullptr)
	{
		fputs("Failed to read vertex shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	fragSource = ReadTextFile(fragPath);
	if (fragSource == nullptr)
	{
		fputs("Failed to read fragment shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	glShaderSource(vertShader, 1, (const GLchar**)&vertSource, nullptr);
	glShaderSource(fragShader, 1, (const GLchar**)&fragSource, nullptr);
	free(vertSource);
	free(fragSource);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in vertex shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in fragment shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint programID = glCreateProgram();
	glAttachShader(programID, vertShader);
	glAttachShader(programID, fragShader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error when linking shader program\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint length;
	GLsizei num;
	char* log;
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetShaderInfoLog(vertShader, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetProgramInfoLog(programID, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}

	//glUseProgram(programID);
	return programID;
}



void code_22_4_2_Test2()
{
	camera = new Camera(Point(0, 0, 1000), Rotator(0, 0, 0));
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f), 1000);

	TextureInfo info[2];
	info[0] = readBmpFile("Texture/TestBmp.bmp");
	info[1] = readBmpFile("Texture/bg.png");

	glGenTextures(2, texName); // 创建6个纹理对象，并将名字赋值给texName数组
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]); // 激活纹理对象texName[i]，并将GL_TEXTURE_2D通道与该纹理对象绑定
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, info[i].colorFormat, info[i].width, info[i].height, 0, info[i].colorFormat, GL_UNSIGNED_BYTE, info[i].pixelData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(info[i].pixelData);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//创建顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//绑定缓冲对象，设定缓冲类型
	//将顶点数据复制到缓冲的内存中，并指定显卡如何管理数据，这里指定为GL_STATIC_DRAW

	float vertices[] =
	{
		// 顶点位置										 // 顶点颜色         // 纹理坐标
		 info[0].width / 2,  info[0].height / 2, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		-info[0].width / 2,  info[0].height / 2, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top Left 
		 info[0].width / 2, -info[0].height / 2, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-info[0].width / 2,  info[0].height / 2, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top Left 
		-info[0].width / 2, -info[0].height / 2, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		 info[0].width / 2, -info[0].height / 2, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right

	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建VAO顶点数组对象
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	//颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		//以顶点属性位置作为参数,启用顶点属性
	//纹理坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);		//以顶点属性位置作为参数,启用顶点属性

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	platShaderProgramID = CreateShader("Shader/MultiTextureMappingVertexShader.vert", "Shader/MultiTextureMappingFragmentShader.frag");
}
#endif


#ifdef CHAPTER_22_4_3
static unsigned int VAO;
static GLuint platShaderProgramID;
static GLint ImageHeight;
static GLint ImageWidth;
static GLubyte* pixelData;
static GLint ImageColorFormat;
static GLuint texture;
static float plat_VerticesAndNormal[] =
{
	-200.f, -300.f, -2.f,  0.0f,  0.0f, -1.0f,
	 200.f, -300.f, -2.f,  0.0f,  0.0f, -1.0f,
	 200.f,  300.f, -2.f,  0.0f,  0.0f, -1.0f,
	 200.f,  300.f, -2.f,  0.0f,  0.0f, -1.0f,
	-200.f,  300.f, -2.f,  0.0f,  0.0f, -1.0f,
	-200.f, -300.f, -2.f,  0.0f,  0.0f, -1.0f,
			
	-200.f, -300.f,  2.f,  0.0f,  0.0f, 1.0f,
	 200.f, -300.f,  2.f,  0.0f,  0.0f, 1.0f,
	 200.f,  300.f,  2.f,  0.0f,  0.0f, 1.0f,
	 200.f,  300.f,  2.f,  0.0f,  0.0f, 1.0f,
	-200.f,  300.f,  2.f,  0.0f,  0.0f, 1.0f,
	-200.f, -300.f,  2.f,  0.0f,  0.0f, 1.0f,
			
	-200.f,  300.f,  2.f, -1.0f,  0.0f,  0.0f,
	-200.f,  300.f, -2.f, -1.0f,  0.0f,  0.0f,
	-200.f, -300.f, -2.f, -1.0f,  0.0f,  0.0f,
	-200.f, -300.f, -2.f, -1.0f,  0.0f,  0.0f,
	-200.f, -300.f,  2.f, -1.0f,  0.0f,  0.0f,
	-200.f,  300.f,  2.f, -1.0f,  0.0f,  0.0f,
			
	 2.f,  300.f,  2.f,  1.0f,  0.0f,  0.0f,
	 2.f,  300.f, -2.f,  1.0f,  0.0f,  0.0f,
	 2.f, -300.f, -2.f,  1.0f,  0.0f,  0.0f,
	 2.f, -300.f, -2.f,  1.0f,  0.0f,  0.0f,
	 2.f, -300.f,  2.f,  1.0f,  0.0f,  0.0f,
	 2.f,  300.f,  2.f,  1.0f,  0.0f,  0.0f,
			
	-200.f, -300.f, -2.f,  0.0f, -1.0f,  0.0f,
	 200.f, -300.f, -2.f,  0.0f, -1.0f,  0.0f,
	 200.f, -300.f,  2.f,  0.0f, -1.0f,  0.0f,
	 200.f, -300.f,  2.f,  0.0f, -1.0f,  0.0f,
	-200.f, -300.f,  2.f,  0.0f, -1.0f,  0.0f,
	-200.f, -300.f, -2.f,  0.0f, -1.0f,  0.0f,
			
	-200.f,  300.f, -2.f,  0.0f,  1.0f,  0.0f,
	 200.f,  300.f, -2.f,  0.0f,  1.0f,  0.0f,
	 200.f,  300.f,  2.f,  0.0f,  1.0f,  0.0f,
	 200.f,  300.f,  2.f,  0.0f,  1.0f,  0.0f,
	-200.f,  300.f,  2.f,  0.0f,  1.0f,  0.0f,
	-200.f,  300.f, -2.f,  0.0f,  1.0f,  0.0f
};
GLchar* ReadTextFile(const char* name)
{
	FILE* fp = nullptr;
	GLchar* content = NULL;
	int count = 0;

	if (name == NULL) return NULL;

	fopen_s(&fp, name, "rt");
	if (fp == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	if (count > 0)
	{
		content = (GLchar*)malloc(sizeof(char) * (count + 1));
		if (content != NULL)
		{
			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}
	}
	fclose(fp);
	return content;
}

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


void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	glEnable(GL_LIGHTING);// 打开光照处理功能

	Point lightPosition(10.f, 0.f, 10.f, 1.f);

	GLfloat light_position[] = { lightPosition.x, lightPosition.y, lightPosition.z, 1.0f };
	GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_AMBIENT 属性的值 R,G,B,A 
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_DIFFUSE 属性的值 R,G,B,A
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // GL_SPECULAR 属性的值 R,G,B,A

	GLfloat mat_ambient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_shininess = 30.0f;

	{
		// 设置光源	
		glLightfv(GL_LIGHT0, GL_POSITION, light_position); // 设置 GL_POSITION (光源位置)属性 
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // 设置 GL_AMBIENT( 光源发出的光，经过非常多次反射，遗留在整个光照环境中的强度)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);// 设置 GL_DIFFUSE(光源发出的光，照射到粗糙表面时，经过漫反射，所得到的光强度)
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);// 设置 GL_SPECULAR(光源发出的光，照射到光滑表面时，经过镜面反射，所得到的光强度) 

		glEnable(GL_LIGHT0);// 开启光源

	}

	// 绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(glGetUniformLocation(platShaderProgramID, "ourTexture"), 0); // 纹理单元0是默认激活的，所以这里不用显示复制

	glUseProgram(platShaderProgramID);
	Matrix modelMatrix(4, 4);
	matrixSetIdentity(modelMatrix);

	float viewMatrix[4][4];
	camera->GetModelViewMatrix((float*)viewMatrix);

	float projectionMatrix[4][4];
	camera->GetProjectionMatrix((float*)projectionMatrix);

	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "modelMat"), 1, GL_FALSE, (const float*)modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "viewMat"), 1, GL_FALSE, (const float*)viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(platShaderProgramID, "projectionMat"), 1, GL_FALSE, (const float*)projectionMatrix);

	glUniform3f(glGetUniformLocation(platShaderProgramID, "light_ambient"), light_ambient[0], light_ambient[1], light_ambient[2]);
	glUniform3f(glGetUniformLocation(platShaderProgramID, "light_diffuse"), light_diffuse[0], light_diffuse[1], light_diffuse[2]);
	glUniform3f(glGetUniformLocation(platShaderProgramID, "light_specular"), light_specular[0], light_specular[1], light_specular[2]);

	glUniform3f(glGetUniformLocation(platShaderProgramID, "mat_ambient"), mat_ambient[0], mat_ambient[1], mat_ambient[2]);
	glUniform3f(glGetUniformLocation(platShaderProgramID, "mat_diffuse"), mat_diffuse[0], mat_diffuse[1], mat_diffuse[2]);
	glUniform3f(glGetUniformLocation(platShaderProgramID, "mat_specular"), mat_specular[0], mat_specular[1], mat_specular[2]);
	glUniform1f(glGetUniformLocation(platShaderProgramID, "mat_shininess"), mat_shininess);


	glUniform3f(glGetUniformLocation(platShaderProgramID, "lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(glGetUniformLocation(platShaderProgramID, "viewPos"), camera->_viewP0.x, camera->_viewP0.y, camera->_viewP0.z);


	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	glUseProgram(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glutSwapBuffers();
	glFlush();
}

GLint CreateShader(const char* vertPath, const char* fragPath)
{
	GLuint vertShader, fragShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLchar* vertSource, * fragSource;
	vertSource = ReadTextFile(vertPath);
	if (vertSource == nullptr)
	{
		fputs("Failed to read vertex shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	fragSource = ReadTextFile(fragPath);
	if (fragSource == nullptr)
	{
		fputs("Failed to read fragment shader\n", stderr);
		exit(EXIT_FAILURE);
	}
	glShaderSource(vertShader, 1, (const GLchar**)&vertSource, nullptr);
	glShaderSource(fragShader, 1, (const GLchar**)&fragSource, nullptr);
	free(vertSource);
	free(fragSource);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in vertex shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error in fragment shader compilation\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint programID = glCreateProgram();
	glAttachShader(programID, vertShader);
	glAttachShader(programID, fragShader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		fputs("Error when linking shader program\n", stderr);
		exit(EXIT_FAILURE);
	}

	GLint length;
	GLsizei num;
	char* log;
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetShaderInfoLog(vertShader, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		log = (char*)malloc(sizeof(char) * length);
		glGetProgramInfoLog(programID, length, &num, log);
		fprintf(stderr, "%s\n", log);
	}

	//glUseProgram(programID);
	return programID;
}



void code_22_4_3()
{
	camera = new Camera(Point(0, 0, 1000), Rotator(0, 0, 0),2);
	cameraDisplayFunc(displayFcn, camera, winWidth, winHeight);
	camera->setLookPoint(Point(0.f, 0.f, 0.f), 1000);

	// 加载纹理
	glGenTextures(1, &texture);
	// 将纹理对象texture绑定到GL_TEXTURE_2D上，往后对GL_TEXTURE_2D的操作都将对texture执行
	glBindTexture(GL_TEXTURE_2D, texture);
	// 设置纹理在不同轴的环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理的纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载纹理图片
	//readBmpFile("Texture/TestBmp.bmp");
	readBmpFile("Texture/T_Brick_Clay_New_D.PNG");
	
	// 将纹理图片保存到绑定GL_TEXTURE_2D的对象上，以字符数组形式
	glTexImage2D(GL_TEXTURE_2D, 0, ImageColorFormat, ImageWidth, ImageHeight, 0, ImageColorFormat, GL_UNSIGNED_BYTE, pixelData);
	// 生成GL_TEXTURE_2D绑定对象的多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	// 释放纹理图片内存，解绑纹理texture
	stbi_image_free(pixelData);
	glBindTexture(GL_TEXTURE_2D, 0);

	//创建顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//绑定缓冲对象，设定缓冲类型
	//将顶点数据复制到缓冲的内存中，并指定显卡如何管理数据，这里指定为GL_STATIC_DRAW
	ImageWidth = 500;
	ImageHeight = 500;
	float vertices[] =
	{
		// 顶点位置								   // 法线			   // 纹理坐标
		 ImageWidth / 2,  ImageHeight / 2, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top Right
		-ImageWidth / 2,  ImageHeight / 2, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // Top Left 
		 ImageWidth / 2, -ImageHeight / 2, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom Right

		-ImageWidth / 2,  ImageHeight / 2, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // Top Left 
		-ImageWidth / 2, -ImageHeight / 2, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		 ImageWidth / 2, -ImageHeight / 2, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom Right

	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建VAO顶点数组对象
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	//法线
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		//以顶点属性位置作为参数,启用顶点属性
	//纹理坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);		//以顶点属性位置作为参数,启用顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//// =========================================
	//glBufferData(GL_ARRAY_BUFFER, sizeof(plat_VerticesAndNormal), plat_VerticesAndNormal, GL_STATIC_DRAW);

	////创建VAO顶点数组对象
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	////顶点
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);		//以顶点属性位置作为参数,启用顶点属性
	////法向量
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	//glEnableVertexAttribArray(1);		//以顶点属性位置作为参数,启用顶点属性

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	//// =============================================

	glEnable(GL_DEPTH_TEST);

	platShaderProgramID = CreateShader("Shader/BumpMappingVertexShader.vert", "Shader/BumpMappingFragmentShader.frag");
	//platShaderProgramID = CreateShader("Shader/PhongCustomVertexShader.vert", "Shader/PhongCustomFragmentShader.frag");
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_22_COMMON

#ifdef CHAPTER_22_COMMON
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
	int glewCode = glewInit();
	if (glewCode != GLEW_OK)
	{
		printf("glewInit err:%d\n", glewCode);
		return;
	}
	init();

#ifdef CHAPTER_22_Test
	code_22_Test();
#endif

#ifdef CHAPTER_22_4_1
	code_22_4_1();
#endif

#ifdef CHAPTER_22_4_2
	code_22_4_2();
#endif

#ifdef CHAPTER_22_4_2_Test1
	code_22_4_2_Test1();
#endif

#ifdef CHAPTER_22_4_2_Test2
	code_22_4_2_Test2();
#endif

#ifdef CHAPTER_22_4_3
	code_22_4_3();
#endif

#ifdef CHAPTER_22_4_3_Test
	code_22_4_3_Test();
#endif

	glutMainLoop();
}
#endif


#endif 


