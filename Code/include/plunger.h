
#ifndef INCLUDE_PLUNGER_H
#define INCLUDE_PLUNGER_H

#include "physObject.h"

#define PLUNGER_FORCE 5000

class Plunger : public PhysObject{
public:
	Plunger();
	Plunger(char key);
	~Plunger();
private:
	void keyboard(eventType);
	void Update(unsigned int dt);
	bool isActive;
	char key;

	bool offset; //Prevents plunger from sticking
};



#endif /* INCLUDE_PLUNGER_H */
