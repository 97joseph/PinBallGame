#version 330

uniform sampler2D texture;
uniform sampler2D normalMap;

in vec3 lightColor;
in float specColor;
in float attenuationColor;
in vec2 texCoordMod;
smooth in vec3 o_atmosphere;

out vec4 frag_color;

void main(void)
{	
	
	vec3 albedo = texture2D(texture, texCoordMod).rgb;
	vec3 col = (albedo * lightColor + specColor) * attenuationColor;

	if (col.x > o_atmosphere.x)
		col.x = o_atmosphere.x;
	if (col.y > o_atmosphere.y)
		col.y = o_atmosphere.y;
	if (col.z > o_atmosphere.z)
		col.z = o_atmosphere.z;

	frag_color = vec4(col, 1.0f);
}

