#version 440 core

// ******************************************************
//		Done with the help of Professor Duarte Duque
//				 All Rights Reserved - 2019
// ******************************************************

in vec2 UV;
in vec3 Position_world;
in vec3 Normal_cam;
in vec3 vPositionEyeSpace;

out vec4 fColor;

uniform vec4 random;
uniform sampler2D textureSampler;
uniform mat4 V;
uniform int ambLightUni;
uniform int dirLightUni;
uniform int pLightUni;
uniform int spotLightUni;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;	// Componente de luz ambiente global
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global

// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Direção da luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};

uniform DirectionalLight directionalLight; // Fonte de luz direcional

// Estrutura de uma fonte de luz pontual
struct PointLight	{
	vec3 position;		// Posição do ponto de luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenuação constante
	float linear;		// Coeficiente de atenuação linear
	float quadratic;	// Coeficiente de atenuação quadrática
};

uniform PointLight pointLight[2]; // Duas fontes de luz pontual

// Estrutura de uma fonte de luz cónica
struct SpotLight {
	vec3 position;		// Posição do foco de luz, espaço do mundo
	vec3 direction;		// Direção da luz, espaço do mundo
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenuação constante
	float linear;		// Coeficiente de atenuação linear
	float quadratic;	// Coeficiente de atenuação quadrática

	float cutOff;
	float outerCutOff;
};

uniform SpotLight spotLight; // Fonte de luz cónica

struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

uniform Material material;

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
vec4 calcSpotLight(SpotLight light);

void main(){
	// Cálculo da componente emissiva do material.
	vec4 emissive = vec4(material.emissive, 1.0);

	// Cálculo do efeito da iluminação no fragmento.
	vec4 light[5];

	// Contribuição da fonte de luz ambiente
	if(ambLightUni == 1)
		light[0] = calcAmbientLight(ambientLight);
	else
		light[0] = vec4(0.0);

	// Contribuição da fonte de luz direcional
	if(dirLightUni == 1)
		light[1] = calcDirectionalLight(directionalLight);
	else
		light[1] = vec4(0.0);

	// Contribuição de cada fonte de luz Pontual
	for(int i=0; i<2; i++) {
		if(pLightUni == 1)
			light[i+2] = calcPointLight(pointLight[i]);
		else
			light[i+2] = vec4(0.0);
	}

	// Contribuição da fonte de luz cónica
	if(spotLightUni == 1)
		light[4] = calcSpotLight(spotLight);
	else
		light[4] = vec4(0.0);

	// Cálculo da cor final do fragmento.
	fColor = (emissive + light[0] + light[1] + light[2] + light[3] + light[4]) * (texture( textureSampler, UV ).rgba * random);
}

vec4 calcAmbientLight(AmbientLight light) {
	// Cálculo da contribuição da fonte de luz ambiente global, para a cor do objeto.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	// Cálculo da reflexão da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// Cálculo da reflexão da componente da luz difusa.
	vec3 lightDirectionEyeSpace = (V * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Direção inversa à da direção luz.
	vec3 N = normalize(Normal_cam);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
	
	// Cálculo da reflexão da componente da luz especular.
	// Como os cálculos estão a ser realizados nas coordenadas do olho, então a câmara está na posição (0,0,0).
	vec3 V = normalize(-vPositionEyeSpace);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	// Cálculo da contribuição da fonte de luz direcional para a cor final do fragmento.
	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	// Cálculo da reflexão da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// Cálculo da reflexão da componente da luz difusa.
	vec3 lightPositionEyeSpace = (V * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(Normal_cam);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	// Cálculo da reflexão da componente da luz especular.
	// Como os cálculos estão a ser realizados nas coordenadas do olho, então a câmara está na posição (0,0,0).
	vec3 V = normalize(-vPositionEyeSpace);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	// attenuation
	float dist = length(light.position - Position_world);	// Cálculo da distância entre o ponto de luz e o vértice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// Cálculo da contribuição da fonte de luz pontual para a cor final do fragmento.
	return (attenuation * (ambient + diffuse + specular));
}

vec4 calcSpotLight(SpotLight light) {
	// Cálculo da reflexão da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	vec3 lightPositionEyeSpace = (V * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);

	//https://www.youtube.com/watch?v=tmCOMzAA4rc
	float theta = dot(L, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * intensity;
	vec4 specular = vec4(material.specular * light.specular, 1.0) * intensity;

	// attenuation
	float dist = length(light.position - Position_world);	// Cálculo da distância entre o ponto de luz e o vértice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// Cálculo da contribuição da fonte de luz pontual para a cor final do fragmento.
	return (attenuation * (ambient + diffuse + specular));
}