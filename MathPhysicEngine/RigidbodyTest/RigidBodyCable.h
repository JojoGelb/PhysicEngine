#pragma once
#include "RigidBodyLink.h"
class RigidBodyCable : public RigidBodyLink
{
public:
	//create contact when distance between 2 particles > mexLength
	float maxLength;

	// bounciness of the cable
	float restitution;

	unsigned int AddContact(std::vector<RigidBodyContact*>& contacts, unsigned int limit) const override;

	RigidBodyCable(RigidBody* rigidbodyA, RigidBody* rigidbodyB, float length, float restitution)
		:RigidBodyLink(rigidbodyA, rigidbodyB), maxLength(length), restitution(restitution) {};
};
