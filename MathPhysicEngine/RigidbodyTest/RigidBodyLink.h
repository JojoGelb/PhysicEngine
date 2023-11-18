#include "RigidbodyContactGenerator.h"
#include "RigidBodyContactTest.h"

#pragma once
class RigidBodyLink : public RigidbodyContactGeneratorTest
{
public:
	RigidBody* rigidBody[2];

	//length of the link
	float CurrentLength() const;

	unsigned int AddContact(std::vector<RigidBodyContactTest*>& contacts, unsigned int limit) const = 0;

	RigidBodyLink(RigidBody* rbA, RigidBody* rbB)
		: rigidBody{ rbA,rbB } {};
};
