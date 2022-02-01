#ifndef INCLUDE_BUMPER_H
#define INCLUDE_BUMPER_H

#include "physObject.h"
#include "ball.h"
#include "gui.h"
#include "world.h"

#define FORCE_MULT 1000

enum BumperType { BumperCylinder = 2, BumperTriangle = 1 };

class Bumper : public PhysObject {
public:
	Bumper();
	Bumper(glm::vec3 position, glm::vec3 lColor, BumperType type);
	~Bumper();
protected:
	void OnCollisionDetected (PhysObject*);

private:
	void keyboard(eventType);
	void Update(unsigned int dt);

	BumperType type;
	glm::vec3 pos;
	Light* light;

	int lightCooldownTimer;
	bool active;
};



#endif /* INCLUDE_PADDLE_H_ */
