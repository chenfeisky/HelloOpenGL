#version 330 core
in vec3 normal;
in vec4 fragPos;						//Ƭ����������

out vec4 fragColor;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 lightPos;			//������������
uniform vec3 viewPos;			//�۲�����������

void main(){
	//���㻷������
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;		//��������

	//�������������
	//תΪ��λ����
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - vec3(fragPos));
	float rad = max(dot(lightDir, norm), 0.0);			//���շ�����Ƭ�η������н�[0,PI/2]
	vec3 diffuse = rad * lightColor;							//���������

	//���㾵�����
	vec3 viewDir = normalize(viewPos - vec3(fragPos));			//�۲��߷���
	vec3 reflectDir = reflect(-lightDir, norm);							//���վ����������䷽��
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);	//�����,����ȡ��˵�32����
	float specularStrength = 0.5;												//����ǿ��
	vec3 specular = spec * specularStrength * lightColor;		//�������
	
	//����Ӱ�����ǿ��,��ʱ�����Լ����
	float enableDistance = 3.0f;	//��Ч���վ���
	vec3 light2fragVec = lightPos - vec3(fragPos);
	float distan = min(sqrt(light2fragVec.x * light2fragVec.x + light2fragVec.y * light2fragVec.y + light2fragVec.z * light2fragVec.z), enableDistance);
	float strength = 1 - distan / enableDistance;

	//����Զ���Ȳ�Ӱ�컷��������
	vec3 result = ambient * cubeColor + (diffuse + specular)  * cubeColor * strength;
	fragColor = vec4(result, 1.0);
}
