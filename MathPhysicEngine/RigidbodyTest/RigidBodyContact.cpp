#include "RigidBodyContact.h"
#include <iostream>
void RigidBodyContact::Resolve(float duration)
{
	ResolveVelocity(duration);

	ResolveInterpenetration();
}

float RigidBodyContact::CalculateSeparatingVelocity()
{
	Vector3D relativeVelocity = rigidbody[0]->velocity;
	if (rigidbody[1])
	{
		relativeVelocity -= rigidbody[1]->velocity;
	}

	return relativeVelocity.DotProduct(contactNormal);
}

// Give proper impulse to solve contact
void RigidBodyContact::ResolveVelocity(float duration)
{

	Vector3D u1 = GetSpeedAtContactPoint(rigidbody[0]);

	// Calculate vector between center of mass and point of contact
	Vector3D r1 = contactPoint - rigidbody[0]->transform->position;
	Vector3D urel = u1;
	Vector3D rb2EquationPart = Vector3D();

	Vector3D r2;

	if (rigidbody[1])
	{
		Vector3D u2 = GetSpeedAtContactPoint(rigidbody[1]);
		r2 = contactPoint - rigidbody[1]->transform->position;
		urel -= u2;
		Vector3D rb2EquationPart = ((rigidbody[1]->GetInverseInertiaTensorWorld() * r2.CrossProduct(contactNormal)).CrossProduct(r2));
	}

	// Calculate impulse
	double k = ((restitution + 1) * urel.DotProduct(contactNormal)) /
				 ((rigidbody[0]->GetInversedMass() + rigidbody[1]->GetInversedMass()) * contactNormal +
				  (rigidbody[0]->GetInverseInertiaTensorWorld() * r1.CrossProduct(contactNormal)).CrossProduct(r1) +
				  rb2EquationPart).DotProduct(contactNormal);


	std::cout << "k: " << k << std::endl;

	// Calculate new velocity
	rigidbody[0]->velocity = rigidbody[0]->velocity - k *contactNormal / rigidbody[0]->GetMass();
	// Calculate new angularVelocity
	rigidbody[0]->angularVelocity = rigidbody[0]->angularVelocity - (rigidbody[0]->GetInverseInertiaTensorWorld() * (r1.CrossProduct(k*contactNormal)));

	if (rigidbody[1]){
		rigidbody[1]->angularVelocity = rigidbody[1]->angularVelocity + (rigidbody[1]->GetInverseInertiaTensorWorld() * (r2.CrossProduct(k*contactNormal)));
		rigidbody[1]->velocity = rigidbody[1]->velocity + k *contactNormal / rigidbody[1]->GetMass();
	}
}

void RigidBodyContact::ResolveInterpenetration()
{
	double angularInertia[2] = { 0,0 };
	double linearInertia[2] = { 0,0 };

	double totalInertia = 0;

	for (unsigned i = 0; i < 2; i++) {
		if (rigidbody[i]) {
			Matrix33 inverseInertiaTensor = rigidbody[i]->GetInverseInertiaTensorWorld();
			// Use the same procedure as for calculating frictionless
			// velocity change to work out the angular inertia.
			Vector3D r = contactPoint - rigidbody[i]->transform->position;
			Vector3D angularInertiaWorld = r % contactNormal;
			angularInertiaWorld = inverseInertiaTensor * (angularInertiaWorld);
			angularInertiaWorld = angularInertiaWorld % r;
			angularInertia[i] = angularInertiaWorld.DotProduct(contactNormal);
			// The linear component is simply the inverse mass.
			linearInertia[i] = rigidbody[i]->GetInversedMass();
			// Keep track of the total inertia from all components.
			totalInertia += linearInertia[i] + angularInertia[i];
		}
	}

	double linearMove[2];
	double angularMove[2];

	double inverseInertia = 1 / totalInertia;
	linearMove[0] = penetration * linearInertia[0] * inverseInertia;
	linearMove[1] = -penetration * linearInertia[1] * inverseInertia;
	angularMove[0] = penetration * angularInertia[0] * inverseInertia;
	angularMove[1] = -penetration * angularInertia[1] * inverseInertia;

	for (unsigned i = 0; i < 2; i++) {
		if (rigidbody[i]) {
			rigidbody[i]->transform->position += contactNormal * linearMove[i];

			Vector3D r = contactPoint - rigidbody[i]->transform->position;
			Matrix33 inverseInertiaTensor = rigidbody[i]->GetInverseInertiaTensorWorld();
			Vector3D impulsiveTorque = r % contactNormal;
			Vector3D impulsePerMove = inverseInertiaTensor * impulsiveTorque;
			Vector3D rotationPerMove = impulsePerMove;
			if (angularInertia[i] != 0) {
				rotationPerMove = impulsePerMove * 1 / angularInertia[i];
				//std::cout << "rotationpermove done \n";
			} 
			Vector3D rotation = rotationPerMove * angularMove[i];

			//std::cout << "Rotate amount : " << rotation << "\n";

			rigidbody[i]->orientation.RotateByVector(rotation);
		}
	}




	/*if (penetration <= 0)
		return;

	float totalInversMass = rigidbody[0]->GetInversedMass();
	if (rigidbody[1])
	{
		totalInversMass += rigidbody[1]->GetInversedMass();
	}

	if (totalInversMass <= 0)
		return;

	Vector3D movePerIMass = contactNormal * (penetration / totalInversMass);

	Vector3D particleAMovement = movePerIMass * rigidbody[0]->GetInversedMass();
	Vector3D particleBMovement;
	if (rigidbody[1])
	{
		particleBMovement = movePerIMass * -rigidbody[1]->GetInversedMass();
	}
	else
	{
		particleBMovement = Vector3D();
	}

	rigidbody[0]->transform->position += particleAMovement;

	if (rigidbody[1])
	{
		rigidbody[1]->transform->position += particleBMovement;
	}*/

	/*Same book, different edition
	Vector3D movePerIMass = contactNormal * (-penetration / totalInversMass);
	rigidbody[0]->position += movePerIMass * rigidbody[0]->GetInversedMass();

	if (rigidbody[1]) {
		rigidbody[1]->position += movePerIMass * rigidbody[1]->GetInversedMass();
	}*/

	// PATCH: this line prevent the ResolveInterpenetration to be called several times during a frame
	//penetration = 0;
}