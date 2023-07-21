#version 330 core

// 输入为顶点颜色、纹理坐标
in vec3 ourColor;
in vec2 TexCoord;

// 输出为颜色
out vec4 color;

// 采样器
uniform sampler2D ourTexture;

void main()
{
	// 使用texture函数来采样纹理颜色（采样器，纹理坐标）
    color = texture(ourTexture, TexCoord);
}