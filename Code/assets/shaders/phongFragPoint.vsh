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

uniform vec3 lPos;
//uniform float lRad;

smooth out vec3 lightDir;
smooth out vec3 reflectDir;
out float attenuation;

void main(void) 
{ 
  //temp light and parameters
  float radius = 40.0f;
  
  
  //matrix calculations
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  texCoordMod = v_texcoord; 
  vec3 fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
  
  //vector calculation
  viewDir = (inverse(viewMatrix) * vec4(0.0f, 0.0f, 1.0f, 0.0f)).xyz;
  normal = vec3(modelMatrix * vec4(v_normal, 0.0f));
  lightDir = lPos - fragPos;
  reflectDir = reflect(-lightDir, normal);
  attenuation = pow(smoothstep(radius*2, 0 , length(lightDir)), 2);
  
}
