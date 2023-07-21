#version 330 core
in vec3 normal;
in vec4 fragPos;						//片段世界坐标

out vec4 fragColor;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 lightPos;			//光照世界坐标
uniform vec3 viewPos;			//观察者世界坐标

void main(){
	//计算环境光照
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;		//环境光照

	//计算漫反射光照
	//转为单位向量
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - vec3(fragPos));
	float rad = max(dot(lightDir, norm), 0.0);			//光照方向与片段法向量夹角[0,PI/2]
	vec3 diffuse = rad * lightColor;							//漫反射光照

	//计算镜面光照
	vec3 viewDir = normalize(viewPos - vec3(fragPos));			//观察者方向
	vec3 reflectDir = reflect(-lightDir, norm);							//光照经法向量折射方向
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);	//反光度,这里取点乘的32次幂
	float specularStrength = 0.5;												//镜面强度
	vec3 specular = spec * specularStrength * lightColor;		//镜面光照
	
	//距离影响光照强度,暂时是我自己想的
	float enableDistance = 3.0f;	//有效光照距离
	vec3 light2fragVec = lightPos - vec3(fragPos);
	float distan = min(sqrt(light2fragVec.x * light2fragVec.x + light2fragVec.y * light2fragVec.y + light2fragVec.z * light2fragVec.z), enableDistance);
	float strength = 1 - distan / enableDistance;

	//光照远近先不影响环境光照了
	vec3 result = ambient * cubeColor + (diffuse + specular)  * cubeColor * strength;
	fragColor = vec4(result, 1.0);
}
