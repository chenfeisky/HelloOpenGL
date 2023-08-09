#version 330 core

// ����Ϊ������ɫ����������
in vec2 TexCoord;
in vec3 lightdir;
in vec3 view;

// ���Ϊ��ɫ
out vec4 fragColor;

uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;

// ������
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	vec4 base = texture(ourTexture1, TexCoord);
	vec3 norm = normalize(vec3(texture(ourTexture2, TexCoord)));
	//vec3 norm = vec3(0,0,1);
	vec3 color = light_ambient * vec3(base);

	float NdotL = max(dot(norm, lightdir), 0.0);
	vec3 aaa = NdotL * (light_diffuse * vec3(base));
	color += NdotL * (light_diffuse * vec3(base));

	//���㾵�����
	vec3 reflection = reflect(-lightdir, norm);		
	float RdotV = max(dot(reflection, view), 0.0);
	vec3 bbb = vec3(base) * light_specular * pow(RdotV, 0.5f);
	color += vec3(base) * light_specular * pow(RdotV, 0.5f);

    fragColor = vec4(color, 1.0f);
}