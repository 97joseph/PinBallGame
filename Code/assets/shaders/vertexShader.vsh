#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texcoord;
         
smooth out vec3 color; 
out vec3 fragPos;
out vec3 normal;
out vec3 planetNormal;
out vec3 viewDir;
out vec2 texCoordMod;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 lightPos;
        
void main(void) 
{ 
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
  //use temp color
  
  lightPos = vec3(80.0f, 500.0f, 0.0f);
  texCoordMod = v_texcoord;
  
  //did normal calculation in color array
  //modify normals to world space
  vec3 aNormal = v_color;
  
  //transform to proper coordinates
  normal = vec3(modelMatrix * vec4(aNormal, 0.0f));
  
  //get view position
  mat4 view = viewMatrix;
  viewDir = normalize((inverse(view) * vec4(0.0f, 0.0f, 1.0f, 0.0f))).xyz;
  
  //calculate planet normals from model origin
  vec4 calcNormal = (modelMatrix * vec4(0,0,0,1)) - vec4(fragPos.xyz, 0.0f);
  planetNormal = normalize(vec3(calcNormal)), 0 ,1;
}
          