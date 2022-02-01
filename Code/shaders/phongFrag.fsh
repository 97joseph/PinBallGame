#version 330

//take in position and normal
smooth in vec3 normal; 
smooth in vec3 viewDir;
in vec2 texCoordMod;

smooth in vec3 lightDir[16];
smooth in vec3 reflectDir[16];
in float attenuation[16];
uniform vec3 lCol[16];
flat in int lights;
flat in vec3 o_atmosphere;

uniform sampler2D texture;
uniform sampler2D normalMap;

out vec4 frag_color;

float calculateDiff(int index, vec3 N);
float calculateSpecular(int index, vec3 V);

void main(void)
{	
	vec3 N = normalize(normal);
	vec3 V = normalize(viewDir);
	
	vec3 finalDiff = vec3(0,0,0) + o_atmosphere;
	float finalSpec = 0;
	float finalAtten = 0;
	for (int i = 0; i < lights; i++)
	{
		finalDiff += lCol[i] * calculateDiff(i, N);
		finalSpec += calculateSpecular(i, V);
		finalAtten = min(finalAtten + attenuation[i], 1);
	}
	
	
	
	vec3 albedo = texture2D(texture, texCoordMod).rgb;
	frag_color = vec4((albedo * finalDiff + finalSpec) * finalAtten, 1.0f);
}

float calculateDiff(int index, vec3 N)
{
	vec3 L = normalize(lightDir[index]);
	return clamp(dot(L, N), 0, 1);
}

float calculateSpecular(int index, vec3 V)
{
	vec3 R = normalize(reflectDir[index]);
	return pow(max(dot(V, R), 0), 15.0f);
}
