#version 330 core

// 输入为顶点颜色、纹理坐标
in vec2 TexCoord;
in vec3 lightdir;
in vec3 view;

// 输出为颜色
out vec4 fragColor;

uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;

// 采样器
uniform sampler2D textureID;

float height(vec3 color)
{
	float avg = (color.r + color.g) / 2.0;
	return mix(avg, 0.5, 0.985);
}

vec3 modNormal(vec2 point)
{
	vec2 d0 = vec2(0, 0.001);
	vec2 d1 = vec2(-0.000866, -0.0005);
	vec2 d2 = vec2(0.000866, -0.0005);

	vec2 p0 = point + d0;
	vec2 p1 = point + d1;
	vec2 p2 = point + d2;

	float h0 = height(vec3(texture(textureID, p0)));
	float h1 = height(vec3(texture(textureID, p1)));
	float h2 = height(vec3(texture(textureID, p2)));

	vec3 v0 = vec3(d0, h0);
	vec3 v1 = vec3(d1, h1);
	vec3 v2 = vec3(d2, h2);

	return normalize(vec3(cross(v1 - v0, v2 - v0)));
}


void main()
{
	vec4 base = texture(textureID, TexCoord);
	vec3 bump = modNormal(TexCoord);
	vec3 color = light_ambient * vec3(base);

	float NdotL = max(dot(bump, lightdir), 0.0);
	vec3 aaa = NdotL * (light_diffuse * vec3(base));
	color += NdotL * (light_diffuse * vec3(base));

	//计算镜面光照
	vec3 reflection = reflect(-lightdir, bump);		
	float RdotV = max(dot(reflection, view), 0.0);
	vec3 bbb = vec3(base) * light_specular * pow(RdotV, 0.5f);
	color += vec3(base) * light_specular * pow(RdotV, 0.5f);

    fragColor = vec4(color, 1.0f);
}