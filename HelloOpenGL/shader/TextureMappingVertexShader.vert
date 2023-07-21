#version 330 core

// 输入为顶点数据中的位置、颜色、纹理坐标
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

// 输出为顶点颜色、纹理坐标
out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0f);
    ourColor = color;
    TexCoord = texCoord;
}