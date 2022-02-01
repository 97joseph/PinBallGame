#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;
         
vec3 normal; 
vec3 viewDir;
out vec2 texCoordMod;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 atmosphere;

uniform int lSize;
uniform vec3 lPos[16];
uniform vec3 lCol[16];
//uniform float lRad[16];

out vec3 lightColor;
out float specColor;
out float attenuationColor;
smooth out vec3 o_atmosphere;

void main(void) 
{ 

	o_atmosphere = atmosphere;

  vec3 lightDir[16];
  vec3 reflectDir[16];
  float attenuation[16];
  int lights;
  
  //temp light and parameters
  float radius = 40.0f;
  
  //matrix calculations
  lights = lSize;
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  texCoordMod = v_texcoord; 
  vec3 fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
  
  //vector calculation
  viewDir = (inverse(viewMatrix) * vec4(0.0f, 0.0f, 1.0f, 0.0f)).xyz;
  normal = vec3(modelMatrix * vec4(v_normal, 0.0f));
  for (int i = 0; i < lSize; i++)
  {
    lightDir[i] = lPos[i] - fragPos;
    reflectDir[i] = reflect(-lightDir[i], normal);
    attenuation[i] = pow(smoothstep(radius*2, 0 , length(lPos[i] - fragPos)), 2);
  }
  
  vec3 N = normalize(normal);
	vec3 V = normalize(viewDir);
	
	vec3 finalDiff = vec3 (0, 0, 0);
	float finalSpec = 0;
	float finalAtten = 0;
	for (int i = 0; i < lights; i++)
	{
		finalDiff += lCol[i] * clamp(dot(normalize(lightDir[i]), N), 0, 1);
		finalSpec += pow(max(dot(V, normalize(reflectDir[i])), 0), 15.0f);
		finalAtten = min(finalAtten + attenuation[i], 1);
	}
	
	lightColor = finalDiff;
	specColor = finalSpec;
	attenuationColor = finalAtten;

}
