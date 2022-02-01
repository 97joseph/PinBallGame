#version 330

//take in position and normal
smooth in vec3 normal; 
smooth in vec3 viewDir;
in vec2 texCoordMod;

smooth in vec3 lightDir;
smooth in vec3 reflectDir;
in float attenuation;
flat in int lights;

uniform sampler2D normalMap;

out vec4 frag_color;

void main(void)
{	
	vec3 N = normalize(normal);
	vec3 V = normalize(viewDir);
	vec3 L = normalize(lightDir);
	vec3 R = normalize(reflectDir);
	
	float finalDiff = clamp(dot(L, N), 0, 1);
	float finalSpec = pow(max(dot(V, R), 0), 15.0f);
	//+ finalSpec
	frag_color = vec4((vec3(1.0f) * finalDiff) * attenuation, 1.0f);
}