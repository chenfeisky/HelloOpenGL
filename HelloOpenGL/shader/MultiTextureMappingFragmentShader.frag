#version 330 core

// ����Ϊ������ɫ����������
in vec3 ourColor;
in vec2 TexCoord;

// ���Ϊ��ɫ
out vec4 color;

// ������
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	// ʹ��texture����������������ɫ�����������������꣩
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.7);
}