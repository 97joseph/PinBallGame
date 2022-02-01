
#ifndef INCLUDE_PADDLE_H_
#define INCLUDE_PADDLE_H_

#include "physObject.h"

#define FORCE_MULT 1000

enum paddleType {
	pleft, pright
};

class Paddle : public PhysObject{
public:
	Paddle();
	Paddle(paddleType, char key);
	~Paddle();
private:
	void keyboard(eventType);
	void Update(unsigned int dt);
	paddleType paddle;
	bool isActive;
	char key;

};



#endif /* INCLUDE_PADDLE_H_ */
