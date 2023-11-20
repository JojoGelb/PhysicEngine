#pragma once

#include "RigidbodyContactGenerator.h"

class NaiveRigidBodyContactGenerator: public RigidbodyContactGeneratorTest
{
public:
	float radius;

	//particles that could collide together
	std::vector<RigidBody*>* rigidbodies;




	// H�rit� via ParticleContactGenerator
	unsigned int AddContact(std::vector<RigidBodyContactTest*>& contacts, unsigned int limit) const override;

	NaiveRigidBodyContactGenerator(float radius, std::vector<RigidBody*>* rb)
		: radius(radius), rigidbodies(rb) {};
};