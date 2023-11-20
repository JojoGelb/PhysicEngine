#pragma once

#include "RigidBodyLink.h"

class RigidBodyRod :public RigidBodyLink {

public:

	float length;

	// Hérité via ParticleLink
	unsigned int AddContact(std::vector<RigidBodyContactTest*>& contacts, unsigned int limit) const override;
	RigidBodyRod(RigidBody* rigidBodyA, RigidBody* rigidBodyB, float length)
		:RigidBodyLink(rigidBodyA, rigidBodyB), length(length) {};
};