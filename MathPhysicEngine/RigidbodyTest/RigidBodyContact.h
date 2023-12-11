#pragma once

#include "../Rigidbody/RigidBody.h"

class RigidBodyContact
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

		Vector3D contactPoint;

		//Solve Velocity and interpenetration
		void Resolve(float duration);

		float CalculateSeparatingVelocity();

	private:
		// Handle impulse for collision
		void ResolveVelocity(float duration);

		//Handle interpenetration for collision
		void ResolveInterpenetration();


	// * being cross product
	//u = r + w * r : v linear velocity, w angular velocity, r vector from center of mass to point of contact
	Vector3D VitessePointContact(RigidBody* rb) {
		//maybe cross product
		Vector3D result = rb->velocity + rb->angularVelocity.CrossProduct((contactPoint - rb->transform->position));
	}

	//Calculate k 
	Vector3D GetImpulse() {

		Vector3D u1 = VitessePointContact(rigidbody[0]);
		Vector3D u2 = VitessePointContact(rigidbody[1]);

		Vector3D r1 = contactPoint - rigidbody[0]->transform->position;
		Vector3D r2 = contactPoint - rigidbody[1]->transform->position;

		Vector3D urel = u1 - u2;

		Vector3D result = ((restitution+1)* urel.DotProduct(contactNormal)) / 
			(
				(rigidbody[0]->GetInversedMass() + rigidbody[1]->GetInversedMass())*contactNormal + 
				(rigidbody[0]->GetInverseInertiaTensorWorld() * r1.CrossProduct(contactNormal)).CrossProduct(r1) +
				((rigidbody[1]->GetInverseInertiaTensorWorld() * r2.CrossProduct(contactNormal)).CrossProduct(r2))
			).DotProduct(contactNormal);
	}


	public:

		RigidBodyContact(RigidBody* rigidBodyA = nullptr, RigidBody* rigidBodyB = nullptr, float restitution = 0, float penetration = 0, Vector3D contactNormal = Vector3D())
			: rigidbody{ rigidBodyA, rigidBodyB }, restitution(restitution), penetration(penetration), contactNormal(contactNormal)
		{
		}
};

