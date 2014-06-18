#pragma once
#include "gameobject.h"
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <LinearMath\btDefaultMotionState.h>
class DynamicObject :
	public GameObject
{
public:
	DynamicObject(void);
	DynamicObject(float a);
	~DynamicObject(void);

	void bpRegister(btDiscreteDynamicsWorld* dynamicsWorld);
	void bpUnregister(btDiscreteDynamicsWorld* dynamicsWorld);

	btRigidBody* fallRigidBody;
private:
	btCollisionShape* fallShape;
	btDefaultMotionState* fallMotionState;
	
};

