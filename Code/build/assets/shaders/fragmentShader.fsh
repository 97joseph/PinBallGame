#version 330

//take in position and normal
smooth in vec3 color;
in vec3 fragPos;
in vec3 normal;
in vec3 planetNormal;
in vec3 viewDir;
in vec2 texCoordMod;

uniform sampler2D texture;
uniform sampler2D normalMap;
uniform vec3 horizonColor;
uniform vec3 atmosphereColor;

out vec4 frag_color;

in vec3 lightPos;

void main(void)
{	
	//light color
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	//basic ambience
	float ambience = 0.0f;
	vec3 ambientColor = ambience * lightColor;
	
	//get norm and light direction;
	vec3 lightDir = normalize(lightPos - fragPos);

	//get diffuse strength
	vec3 newNormal = normalize(normal * (texture2D(normalMap, vec2(texCoordMod.x, -texCoordMod.y)).xyz));

	float diff = dot(newNormal, lightDir);
	diff = clamp(diff, 0, 1);
	
	vec3 diffuse = diff * lightColor;
	
	
	//atmosphere effects

	vec3 atmColor = atmosphereColor;
	vec3 atmHorizon = horizonColor;
	
	//get brightness based on sun direction
	float horizonBrightness = max(dot(normal, lightDir), 0);

	//get resulting color
	vec3 atmFinal = mix(atmHorizon, atmColor, pow(diff, 1.0f));

	//get brightness based on view direction
	float atmBrightness = pow(sin(acos(dot(planetNormal, viewDir))), 4.0f)*1.5;
	atmBrightness = clamp(atmBrightness, 0 ,1);

	//raw output for atmosphere effect and planet lighting
	vec4 atmOut = vec4(atmFinal * atmBrightness * horizonBrightness, 1.0f);
	vec4 base = (texture2D(texture, vec2(texCoordMod.x, -texCoordMod.y)) * vec4((ambientColor.rgb + diffuse.rgb), 1.0));

	//screen output mix	
	frag_color =  1- (1-base) * (1-atmOut);
}