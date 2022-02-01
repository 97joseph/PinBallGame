#include "plunger.h"
//
Plunger::Plunger()
{
	isActive = false;
	key = 'p';
}

Plunger::Plunger(char k) : PhysObject(Layer_Plunger, Layer_Ball)
{
	  loadModel("models/plunger.obj");
	  loadTexture("textures/paddle.png");
	  setProperties(100.0, 0.5, 1.1);
	  setMeshCollider(Physics_Mesh_Hull, "models/plunger.obj");
	  isActive = false;
	  key = k - 'a';
	  translate(glm::vec3(0.1, 1.1, -20.9));

		initPhysics();

		btTransform t = btTransform::getIdentity();

		btSliderConstraint *constraint = new btSliderConstraint
		(
			*physics, 
			t, 
			true
		);

		constraint -> setLowerLinLimit (btScalar(-1.0));
		constraint -> setUpperLinLimit (btScalar(4.0));

		listener.getWorld()->addConstraint(constraint);
		
}

Plunger::~Plunger()
{

}

void Plunger::keyboard(eventType event)
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

void Plunger::Update(unsigned int dt)
{

	  //update keyboard
		for (int i = 0; i < listener.getSize(); i++)
		{
				keyboard(listener.getEvent(i));
		}

		if (isActive) {

			physics -> forceActivationState (true);
			physics -> applyCentralImpulse (btVector3(-45, 0, 0));
		}
		else if (!isActive) {
			physics -> forceActivationState (true);
			physics -> applyCentralImpulse (btVector3(15, 0, 0));
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
