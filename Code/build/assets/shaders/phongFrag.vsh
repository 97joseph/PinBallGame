#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;
         
smooth out vec3 normal; 
smooth out vec3 viewDir;
out vec2 texCoordMod;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform int lSize;
uniform vec3 lPos[16];
//uniform float lRad[16];

smooth out vec3 lightDir[16];
smooth out vec3 reflectDir[16];
out float attenuation[16];
flat out int lights;

void calculateLight(int index, float radius, vec3 fragPos);

void main(void) 
{ 
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
    calculateLight(i, radius, fragPos);
  }

}

void calculateLight(int index, float radius, vec3 fragPos)
{
  lightDir[index] = lPos[index] - fragPos;
  reflectDir[index] = reflect(-lightDir[index], normal);
  attenuation[index] = pow(smoothstep(radius*2, 0 , length(lPos[index] - fragPos)), 2);
}