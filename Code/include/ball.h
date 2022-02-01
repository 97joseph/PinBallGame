
#ifndef BALL_H
#define BALL_H

#include "physObject.h"
#include "world.h"

class Ball : public PhysObject {

public:
	Ball (glm::vec3);
	void keyboard(eventType);
	void Update(unsigned int dt);

private:
	Light* light;

	static int activeBalls;
};



#endif
