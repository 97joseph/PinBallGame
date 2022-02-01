#include "ball.h"

int Ball::activeBalls = 0;

Ball::Ball (glm::vec3 position) : PhysObject(Layer_All, Layer_All) {

	loadModel ("models/planet.obj");
	setSphereCollider(Physics_Mesh_Sphere, 1);
	translate (position);
	setProperties (0.3f, 0.5f, 0.5f);

	light = new Light ();
	
	light->translate(glm::vec3 (position.x, position.y + 0.5, position.z));
	light->setColor(glm::vec3 (0.25, 0.25, 0.25));

	World::GetInstance () -> addLight (light);

	initPhysics ();

	activeBalls++;
}
//
void Ball::keyboard(eventType) { }

void Ball::Update(unsigned int dt) { 

		//update keyboard
		for (int i = 0; i < listener.getSize(); i++)
		{
				keyboard(listener.getEvent(i));
		}

		if (light != NULL)
			light -> translate (btToGlm(transform.getOrigin ()));

		//Disable physics once ball is out of bounds
		if (transform.getOrigin ().getX () > 12) {

			if (physics != NULL) {
				activeBalls--;

				if (activeBalls == 0) {
					World::GetInstance () -> ActivateGameOverState ();
				}
			}

			physics = NULL;
			World::GetInstance () -> removeLight (light);

			delete light;
			light = NULL;

		
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



