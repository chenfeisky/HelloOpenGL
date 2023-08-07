#version 330 core
in vec3 normal;
in vec4 fragPos;						//片段世界坐标

out vec4 fragColor;

uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;

uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
uniform vec3 mat_specular;
uniform float mat_shininess;

uniform vec3 lightPos;			//光照世界坐标
uniform vec3 viewPos;			//观察者世界坐标

vec3 color;

void main(){
	//计算环境光照
	color = light_ambient * mat_ambient;

	//计算漫反射光照
	vec3 norm = normalize(normal);
	vec3 lightdir = normalize(lightPos - vec3(fragPos));
	float NdotL = max(dot(norm, lightdir), 0.0);
	color += NdotL * (mat_diffuse * light_diffuse);

	//计算镜面光照
	vec3 view = normalize(viewPos - vec3(fragPos));			//观察者方向
	vec3 reflection = reflect(-lightdir, norm);		
	float RdotV = max(dot(reflection, view), 0.0);
	color += mat_specular * light_specular * pow(RdotV, mat_shininess);
	
	
	//距离影响光照强度,暂时是我自己想的
	float enableDistance = 3.0f;	//有效光照距离
	vec3 light2fragVec = lightPos - vec3(fragPos);
	float distan = min(sqrt(light2fragVec.x * light2fragVec.x + light2fragVec.y * light2fragVec.y + light2fragVec.z * light2fragVec.z), enableDistance);
	float strength = 1 - distan / enableDistance;

	//fragColor = vec4(color, 1.0);
	fragColor = vec4(vec3(lightdir), 1.0);
}
