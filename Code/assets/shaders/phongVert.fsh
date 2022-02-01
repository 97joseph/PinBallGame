#version 330

uniform sampler2D texture;
uniform sampler2D normalMap;

in vec3 lightColor;
in float specColor;
in float attenuationColor;
in vec2 texCoordMod;

out vec4 frag_color;

void main(void)
{	
	
	vec3 albedo = texture2D(texture, texCoordMod).rgb;
	frag_color = vec4((albedo * lightColor + specColor) * attenuationColor, 1.0f);
}

