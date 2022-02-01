
#include "bumper.h"

Bumper::Bumper() { }

Bumper::Bumper(glm::vec3 position, glm::vec3 lColor, BumperType type) : PhysObject(Layer_Plunger, Layer_Ball)
{
		lightCooldownTimer = 0;
		
		this -> type = type;

		if (type == BumperCylinder) {
			loadModel("models/cylBumper.obj");
			loadTexture("textures/cylBumper.png");
			
			setBoxCollider(Physics_Mesh_Cylinder, glm::vec3(1.5, 2, 1.5));
		 	setProperties(0,0,2);
			translate(position);

			light = new Light ();
	  	light->translate(position);
	  	light->setColor(lColor);
		}

		if (type == BumperTriangle) {

			//FIXME: BUMPER TRIANGLE NOT SENDING COLLISONS TO CALLBACK
	  	loadModel("models/bumper.obj");
	  	loadTexture("textures/bumper.png");
	  	setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_bumpers.obj");
	  	setProperties(0.0,0.5,3.0);
		}

		pos = position;

		initPhysics();
}

Bumper::~Bumper()
{

}

void Bumper::keyboard(eventType event)
{

}

void Bumper::OnCollisionDetected (PhysObject* hit) {

		GUI::getInstance () -> ChangeScore (25 * (int)type);

		if (active || type == BumperTriangle)
			return;

		lightCooldownTimer = 400;
		World::GetInstance () -> addLight (light);
		active = true;
}

void Bumper::Update(unsigned int dt)
{
	  //update keyboard
		for (int i = 0; i < listener.getSize(); i++)
		{
				keyboard(listener.getEvent(i));
		}
		if (lightCooldownTimer > 0) {
	
			lightCooldownTimer -= dt;
		}

		if (lightCooldownTimer <= 0) {
			lightCooldownTimer = 400;
			World::GetInstance () -> removeLight (light);
			active = false;
		}


	  //update physics object
	  if (physics != NULL)
	  {
		  physics->getMotionState()->getWorldTransform(transform);
		  model = btToGlm(transform);
		  //update scale
		  model *= mscale;
	  }
//
	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}
