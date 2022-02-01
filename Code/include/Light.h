
#ifndef LIGHT_H_
#define LIGHT_H_

#include "object.h"

#define LIGHT_POINT	0

struct LightData {
	int type;
	float radius;
	float power;
	glm::vec3 pos;
	glm::vec3 color;
	LightData(): radius(1), power(1), type(LIGHT_POINT) {}
	LightData(float r, float p, int t, glm::vec3 c):
		radius(r), power(p), type(t), color(c) {}
};

class Light : public Object {
public:
	Light();
	~Light();
	void Render();
	void Update(unsigned int dt);
	LightData* getLight();
	void setColor(glm::vec3);
protected:
private:
	glm::vec3 color;
	LightData light;
};



#endif /* LIGHT_H_ */
