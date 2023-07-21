#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;

//���ռ���ͳһ���뵽Ƭ����ɫ���м���
out vec3 normal;
out vec4 fragPos;		//Ƭ����������

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main(){
	gl_Position = projectionMat * viewMat * modelMat * vec4(pos, 1.0f);
	normal = nor;
	fragPos = modelMat * vec4(pos, 1.0);		//�����������ģ�;���תΪ��������
}