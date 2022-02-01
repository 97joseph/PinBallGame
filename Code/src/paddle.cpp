#include "paddle.h"
//
Paddle::Paddle()
{
	paddle = pleft;
	isActive = false;
	key = 'k';
}

Paddle::Paddle(paddleType type, char k) : PhysObject(Layer_Paddle, Layer_Ball)
{
	  loadModel("models/paddle.obj");
	  loadTexture("textures/paddle.png");
	  setProperties(0.1, 0.5, 0.0);
	  setMeshCollider(Physics_Mesh_Hull, "models/collision/c_paddle.obj");
	  paddle = type;
	  isActive = false;
	  key = k - 'a';


	  if (type == pleft)
	  {
		  translate(glm::vec3(-0.09729, 0, 8.46273));
		  rotate(45, glm::vec3(0,1,0));
		  setConstraint(0.785, 2.35);
	  }
	  else if (type == pright)
	  {
		  translate(glm::vec3(-0.09729, 0, -8.46273));
		  rotate(-45, glm::vec3(0,1,0));
		  setConstraint(-2.35, -0.785 );
	  }
}

Paddle::~Paddle()
{

}

void Paddle::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
		{
			if (event.key == SDLK_a + key) {
				isActive = true;
			}
		}
	else if (event.eventVer == SDL_KEYUP)
	{
		if (event.key == SDLK_a + key) {
			isActive = false;
		}
	}
}

void Paddle::Update(unsigned int dt)
{
	  //update keyboard
	for (int i = 0; i < listener.getSize(); i++)
	{
		  keyboard(listener.getEvent(i));
	}

	if (isActive)
	{
		if (paddle == pleft)
			applyForce(glm::vec3(0,FORCE_MULT,0));
		else
			applyForce(glm::vec3(0,-FORCE_MULT,0));
	}
	else
	{
		if (paddle == pleft)
			applyForce(glm::vec3(0,-FORCE_MULT,0));
		else
			applyForce(glm::vec3(0,FORCE_MULT,0));
	}

	  //update physics object
	  if (physics != NULL)
	  {
		  physics->getMotionState()->getWorldTransform(transform);
		  model = btToGlm(transform);
		  //update scale
		  model *= mscale;
	  }

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}
