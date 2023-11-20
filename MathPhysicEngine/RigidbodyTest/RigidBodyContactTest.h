#pragma once

#include "../Rigidbody/RigidBody.h"

class RigidBodyContactTest
{
	public:
		// 1 could be nullptr
		RigidBody* rigidbody[2];

		//value between 0 - 1
		//define the elasticness of the collision
		// 1 = bouncy
		// 0 = full stop on collision
		float restitution;

		// distance of penetration of the contact
		float penetration;

		Vector3D contactNormal;

		//Solve Velocity and interpenetration
		void Resolve(float duration);

		float CalculateSeparatingVelocity();

	private:
		// Handle impulse for collision
		void ResolveVelocity(float duration);

		//Handle interpenetration for collision
		void ResolveInterpenetration();

	public:

		RigidBodyContactTest(RigidBody* rigidBodyA = nullptr, RigidBody* rigidBodyB = nullptr, float restitution = 0, float penetration = 0, Vector3D contactNormal = Vector3D())
			: rigidbody{ rigidBodyA, rigidBodyB }, restitution(restitution), penetration(penetration), contactNormal(contactNormal)
		{
		}
};

