#version 330 core

// ����Ϊ������ɫ����������
in vec3 ourColor;
in vec2 TexCoord;

// ���Ϊ��ɫ
out vec4 color;

// ������
uniform sampler2D ourTexture;

void main()
{
	// ʹ��texture����������������ɫ�����������������꣩
    color = texture(ourTexture, TexCoord);
}