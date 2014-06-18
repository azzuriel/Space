#include "DynamicObject.h"
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <BulletCollision\CollisionShapes\btSphereShape.h>

DynamicObject::DynamicObject(void)
{
	fallShape = new btSphereShape(1);//

	fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(rand()%2+4,rand()%10+10,rand()%2+4)));
	btScalar mass = 1;
	btVector3 fallInertia(0,0,0);
	fallShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);

}

DynamicObject::DynamicObject(float a)
{
	fallShape = new btSphereShape(a);//

	fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(rand()%2+4,rand()%10+10,rand()%2+4)));
	btScalar mass = (4/3)*(a*a*a*3.1415926);
	btVector3 fallInertia(0,0,0);
	fallShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);

}


DynamicObject::~DynamicObject(void)
{
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;
	delete fallShape;
}

void DynamicObject::bpUnregister(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->removeRigidBody(fallRigidBody);
}

void DynamicObject::bpRegister(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->addRigidBody(fallRigidBody);
}
