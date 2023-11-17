#pragma once
#include "../../Rigidbody/RigidBody.h"

class RigidBodyForceGenerator
{
public:
	virtual void UpdateForce(RigidBody* rigidBody, float duration) = 0;
};

