#pragma once
#include "../Rigidbody/RigidBody.h"
#include <vector>

#include "RigidBodyForces/RigidBodyForceGenerator.h"

class RigidBodyForceRegistry
{
private:
	struct RigidbodyEntry
	{
		RigidBody* rigidBody;
		RigidBodyForceGenerator* forceGenerator;
		RigidbodyEntry(RigidBody* rigidBody, RigidBodyForceGenerator* forceGenerator);
	};

	using Registry = std::vector<RigidbodyEntry>;
	Registry m_registry;


public:

	// Reigstry acccesor;
	void UpdateForce(float duration);
	void AddForce(RigidBody* rigidBody, RigidBodyForceGenerator* forceGenerator);
	void DeleteForce(RigidBody* rigidBody, RigidBodyForceGenerator* forceGenerator);
	void RemoveRigidBody(RigidBody* rigidBody);

};

