#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;

//光照计算统一传入到片段着色器中计算
out vec3 normal;
out vec4 fragPos;		//片段世界坐标

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main(){
	gl_Position = projectionMat * viewMat * modelMat * vec4(pos, 1.0f);
	normal = nor;
	fragPos = modelMat * vec4(pos, 1.0);		//顶点坐标乘以模型矩阵转为世界坐标
}