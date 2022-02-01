#include "physObject.h"
//
std::map <btCollisionObject*, PhysObject*> PhysObject::CollisionObjectMap;

PhysObject::PhysObject()
{
	isStatic = false;
	physics = NULL;
	shape = new btSphereShape(1);
	mass = 0.0f;
	friction = 0.3f;
	restitution = 0.2;
	isRenderable = true;
	layer = Layer_All;
	layersThatCanBeHit = Layer_All;
}

PhysObject::PhysObject(int layer, int layersThatCanBeHit)
{
	isStatic = false;
	physics = NULL;
	shape = new btSphereShape(1);
	mass = 0.0f;
	friction = 0.3f;
	restitution = 0.2;
	isRenderable = true;
	this -> layer = layer;
	this -> layersThatCanBeHit = layersThatCanBeHit;
}

PhysObject::~PhysObject()
{
	listener.getWorld()->removeCollisionObject(physics);
	delete physics;
	delete shape;
	physics = NULL;
	shape = NULL;
	mass = 0.0f;
}

void PhysObject::Update(unsigned int dt)
{
	  //update keyboard
	  for (int i = 0; i < listener.getSize(); i++)
	  {
		  keyboard(listener.getEvent(i));
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

int PhysObject::GetColliderLayer () {

		return layer;
}

int PhysObject::GetTangibleLayers () {

		return layersThatCanBeHit;
}

void PhysObject::SetColliderMask (int layer, int layersThatCanBeHit) {


	this -> layer = layer;
	this -> layersThatCanBeHit = layersThatCanBeHit;
}

void PhysObject::setConstraint(float low, float high)
{
	initPhysics();
	btVector3 axis(0,1,0);
	btVector3 pivot(0,1,0);
	btHingeConstraint *constraint = new btHingeConstraint(*physics, axis, pivot);
	constraint->setLimit(low, high, 0.0, 0.3);
	listener.getWorld()->addConstraint(constraint);
}

void PhysObject::OnCollisionDetected (PhysObject* hit) {

	//TODO: add functionality here
	//cout << hit << " is touching " << this << endl;


}

void PhysObject::Begin()
{
	initPhysics();
	//start children
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Begin();
	}
}

void PhysObject::setRenderable(bool set)
{
	isRenderable = set;
}

void setDefaultCollider(int)
{
	//TODO implement
}

void PhysObject::setBoxCollider(Physics_Mesh_Shape box, glm::vec3 size)
{
	delete shape;
	if (box == PHYS_BOX)
	{
		shape = new btBoxShape(glmToBt(size));
	}
	else if (box == PHYS_CYLINDER)
	{
		shape = new btCylinderShape(glmToBt(size));
	}
	else {

		cout << "INCORRECT PHYS INITIALIZATION" << endl;
		cout << "Defaulting to sphere" << endl;
		shape = new btSphereShape(1.0f);
	}
}

void PhysObject::setSphereCollider(Physics_Mesh_Shape sphere, float radius)
{
	delete shape;
	if (sphere == PHYS_SPHERE)
		shape = new btSphereShape(radius);
	else
	{
		cout << "INCORRECT PHYS INITIALIZATION" << endl;
		cout << "Defaulting to sphere" << endl;
		shape = new btSphereShape(1.0f);
	}
}

void PhysObject::setCapsuleCollider(Physics_Mesh_Shape capCylCone, float radius, float height)
{
	delete shape;
	if (capCylCone == PHYS_CAPSULE)
	{
		shape = new btCapsuleShape(radius, height);
	}
	else if (capCylCone == PHYS_CONE)
	{
		shape = new btConeShape(radius, height);
	}
	else
	{
		cout << "INCORRECT PHYS INITIALIZATION" << endl;
		cout << "Defaulting to sphere" << endl;
		shape = new btSphereShape(1.0f);
	}
}

void PhysObject::setProperties(float m, float f, float r)
{
	mass = m;
	if (mass == 0)
		isStatic = true;
	friction = f;
	restitution = r;
}

void PhysObject::setMeshCollider(Physics_Mesh_Shape mesh, string filename)
{
	loader readIn;
	obj final;


	if (readIn.loadObject(rootDir+filename,final))
	{
		if (mesh == PHYS_HULL)
		{
			//load in collision mesh
			glm::vec3 *vert = NULL;
			btConvexHullShape *meshShape = new btConvexHullShape();
			for (int i = 1; i < final.getVerts().size(); i++)
			{

				vert = &final.getVerts()[i].vertex;
				btVector3 v1(vert->x, vert->y, vert->z);
				meshShape->addPoint(v1, true);
			}
			if (shape != NULL)
				delete shape;
			shape = meshShape;
		}
		else if (mesh == PHYS_S_MESH)
		{
			glm::vec3 vert;
			btTriangleMesh *mesh = new btTriangleMesh();
			btVector3 *vertex;
			physMesh.clear();
			for (int i = 0; i < final.getIndices().size(); i++)
			{
				//load in points
				vert = final.getVerts()[final.getIndices()[i]].vertex;
				vertex = new btVector3(vert.x, vert.y, vert.z);
				physMesh.push_back(*vertex);
				delete vertex;
			}

			for (int i = 0; i < physMesh.size(); i += 3)
			{
				//send in triangle
				mesh->addTriangle(physMesh[i],physMesh[i+1],physMesh[i+2]);
			}

			delete shape;
			shape = new btBvhTriangleMeshShape(mesh, true);
			isStatic = true;
		}
		else
		{
			cout << "INCORRECT PHYS INITIALIZATION" << endl;
			cout << "Defaulting to sphere" << endl;
			shape = new btSphereShape(1.0f);
		}
	}
}

void PhysObject::applyForce(glm::vec3 direction)
{
	physics->activate(true);
	physics->applyTorque(glmToBt(direction));
}

void PhysObject::initPhysics()
{
	//if physics has already been initialized, skip
	if (physics == NULL)
	{
		//base initial position on model matrix
		transform.setIdentity();

		//apply glm world position to simulated world
		transform.setOrigin(glmToBt(glm::vec3(glm::vec4(0, 0, 0, 1) * glm::transpose(model))));
		glm::quat qt = glm::quat_cast(model);
		transform.setRotation(btQuaternion(qt.x,qt.y,qt.z,qt.w));

		//determine if static
		if (isStatic)
			mass = 0.0f;
		else
			mass = 1.0f;

		btVector3 inertia(0,0,0);
		shape->calculateLocalInertia(mass, inertia);

		btDefaultMotionState* objMotionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo objCI(0, NULL, NULL, inertia);
		objCI.m_localInertia = inertia;
		objCI.m_collisionShape = shape;
		objCI.m_motionState = objMotionState;
		objCI.m_mass = mass;
		objCI.m_restitution = restitution;
		objCI.m_friction = friction;

		physics = new btRigidBody(objCI);

		//Rigidbody, collider layer, what it colliders with
		listener.getWorld()->addRigidBody(physics, layer, layersThatCanBeHit);

		ContactSensorCallback cb (*physics, *this);
		listener.getWorld () -> contactTest (physics, cb); //Add callback function to struct

		CollisionObjectMap.insert(std::pair<btCollisionObject*, PhysObject*> (physics, this));
	}
	else
	{
		cout << "PHYSICS ALREADY INITIALZIED FOR THIS OBJECT" << endl;
	}

/*
// USAGE:
btRigidBody* tgtBody ;
YourContext foo;
ContactSensorCallback callback(*tgtBody, foo);
world->contactTest(tgtBody,callback);
*/
}

glm::vec3 PhysObject::btToGlm(btVector3 input)
{
	return glm::vec3(input.getX(), input.getY(), input.getZ());
}

glm::mat4 PhysObject::btToGlm(btTransform matrix)
{
	float *extract;
	matrix.getOpenGLMatrix(extract);
	glm::mat4 final = glm::make_mat4(extract);
	return final;
}

btVector3 PhysObject::glmToBt(glm::vec3 input)
{
	return btVector3(input.x, input.y, input.z);
}

PhysObject* PhysObject::btToPhysObject (const btCollisionObject* obj) {

	std::map<btCollisionObject*,PhysObject*>::iterator it;

  it = CollisionObjectMap.find((btCollisionObject*)obj);
  if (it != CollisionObjectMap.end())
    return it -> second;

  return NULL;
}

void PhysObject::Render()
{
	if (isRenderable)
	{
		  Object::Render();
	}
}

