/*
 * physObject.h
 *
 *  Created on: Oct 11, 2017
 *      Author: nog
 */
#ifndef PHYSOBJECT_H_
#define PHYSOBJECT_H_

#include <bullet/btBulletDynamicsCommon.h>
#include <map>

#include "object.h"
#include "gui.h"

enum Collision_Layer_Mask { Layer_None = 0, Layer_Table = 1, Layer_Ball = 2, Layer_Paddle = 4,
Layer_Bumper = 8, Layer_Plunger = 16, Layer_All = 255 };

enum Physics_Mesh_Shape { Physics_Mesh_Box = 0, Physics_Mesh_Sphere = 1, Physics_Mesh_Capsule = 2,
Physics_Mesh_Cylinder = 3, Physics_Mesh_Cone = 4, Physics_Mesh_Hull = 5, Physics_Mesh_S_Mesh = 6 };

#define PHYS_BOX 		0
#define PHYS_SPHERE 	1
#define PHYS_CAPSULE 	2
#define PHYS_CYLINDER	3
#define PHYS_CONE		4
#define PHYS_HULL		5
#define PHYS_S_MESH		6

class PhysObject : public Object {
public:

	static std::map <btCollisionObject*, PhysObject*> CollisionObjectMap;

	PhysObject();
	PhysObject(int, int);
	virtual ~PhysObject();
	virtual void Begin();
	virtual void Update(unsigned int dt);
	virtual void Render();
    void initPhysics();
    void setDefaultCollider(Physics_Mesh_Shape base);
    void setBoxCollider(Physics_Mesh_Shape box, glm::vec3 size);
    void setSphereCollider(Physics_Mesh_Shape sphere, float radius);
    void setCapsuleCollider(Physics_Mesh_Shape capCylCone, float, float);
    void setMeshCollider(Physics_Mesh_Shape mesh, string filename);
    void setProperties(float mass, float friction, float restitution);
    void setConstraint(float low, float high);
	void setRenderable(bool);

    int GetColliderLayer ();
    int GetTangibleLayers ();
    void SetColliderMask (int, int);

	virtual void OnCollisionDetected (PhysObject* hit);

	void applyForce(glm::vec3);

	static PhysObject* btToPhysObject (const btCollisionObject* obj);



protected:
    glm::vec3 btToGlm(btVector3);
    glm::mat4 btToGlm(btTransform);
    btVector3 glmToBt(glm::vec3);
    btRigidBody *physics;
    btTransform transform;
private:

	//void OnCollisionDetected ();

    btCollisionShape *shape;
    vector<btVector3> physMesh;
    float mass;
    float friction;
    float restitution;
    bool isStatic;
    bool isRenderable;
	int layer;
	int layersThatCanBeHit;
};

struct ContactSensorCallback : public btCollisionWorld::ContactResultCallback {

    //! Constructor, pass whatever context you want to have available when processing contacts
    /*! You may also want to set m_collisionFilterGroup and m_collisionFilterMask
     *  (supplied by the superclass) for needsCollision() */
    ContactSensorCallback(btRigidBody& tgtBody , PhysObject& context)
        : btCollisionWorld::ContactResultCallback(), body(tgtBody), ctxt(context) { 

		}

    btRigidBody& body; //!< The body the sensor is monitoring
    PhysObject& ctxt; //!< External information for contact processing

    //! If you don't want to consider collisions where the bodies are joined by a constraint, override needsCollision:
    /*! However, if you use a btCollisionObject for #body instead of a btRigidBody,
     *  then this is unnecessary—checkCollideWithOverride isn't available */
    virtual bool needsCollision(btBroadphaseProxy* proxy) const {

        // superclass will check m_collisionFilterGroup and m_collisionFilterMask
        if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
            return false;

        // if passed filters, may also want to avoid contacts between constraints
        return body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
    }

    //! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
    virtual btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0,int partId0,int index0,
        const btCollisionObjectWrapper* colObj1,int partId1,int index1)
    {

        btVector3 pt; // will be set to point of collision relative to body
        if(colObj0->m_collisionObject==&body) {
            pt = cp.m_localPointA;

        } else {
            assert(colObj1->m_collisionObject==&body && "body does not match either collision object");
            pt = cp.m_localPointB;
        }

        // do stuff with the collision point
        return 0; // There was a planned purpose for the return value of addSingleResult, but it is not used so you can ignore it.
    }
};

#endif /* PHYSOBJECT_H_ */

