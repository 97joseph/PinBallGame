//
#include "Light.h"

Light::Light()
{
	light = LightData(5, 1, LIGHT_POINT, glm::vec3(1,1,1));
}

Light::~Light()
{

}

void Light::Update(unsigned int dt)
{
	light.pos = glm::vec3(model * glm::vec4(0,0,0,1));
}

void Light::Render()
{
	//ignore rendering
}

LightData* Light::getLight()
{
	return &light;
}

void Light::setColor(glm::vec3 lightColor)
{
	light.color = lightColor;
}
