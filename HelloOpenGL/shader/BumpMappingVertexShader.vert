#version 330 core

// ����Ϊ���������е�λ�á����ߡ���������
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;

// ���Ϊ��������
out vec2 TexCoord;
out vec3 lightdir;
out vec3 view;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

uniform vec3 lightPos;			//������������
uniform vec3 viewPos;			//�۲�����������

void main()
{
    vec4 _norm = modelMat * vec4(norm, 1.0f);
    vec3 normal = normalize(vec3(_norm));
    vec3 yAxis = vec3(0,1,0);
    vec3 zAxis = vec3(0,0,1);
    vec3 tangent;
    vec3 crossYAxis = cross(normal, yAxis);
    vec3 crossZAxis = cross(normal, zAxis);
    if (length(crossYAxis) > length(crossZAxis))
    {
        tangent = normalize(crossYAxis);
    }
    else
    {
        tangent = normalize(crossZAxis);
    }
    vec3 binorm = normalize(-cross(tangent, normal));
    lightdir = normalize(lightPos - position);
    view = normalize(viewPos - position);

    vec3 temp;
    temp.x = dot(lightdir, tangent);
    temp.y = dot(lightdir, binorm);
    temp.z = dot(lightdir, normal);
    lightdir = temp;

    temp.x = dot(view, tangent);
    temp.y = dot(view, binorm);
    temp.z = dot(view, normal);
    view = temp;
 
    gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0f);
    TexCoord = texCoord;
}