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
	Vector3D u2 = GetSpeedAtContactPoint(rigidbody[1]);

	// Calculate vector between center of mass and point of contact
	Vector3D r1 = contactPoint - rigidbody[0]->transform->position;
	Vector3D r2 = contactPoint - rigidbody[1]->transform->position;

	Vector3D urel = u1 - u2;

	// Calculate impulse
	Vector3D k = ((restitution + 1) * urel.DotProduct(contactNormal)) /
				 ((rigidbody[0]->GetInversedMass() + rigidbody[1]->GetInversedMass()) * contactNormal +
				  (rigidbody[0]->GetInverseInertiaTensorWorld() * r1.CrossProduct(contactNormal)).CrossProduct(r1) +
				  ((rigidbody[1]->GetInverseInertiaTensorWorld() * r2.CrossProduct(contactNormal)).CrossProduct(r2)))
					 .DotProduct(contactNormal);

	// Calculate new velocity
	rigidbody[0]->velocity = rigidbody[0]->velocity - k.DotProduct(contactNormal) / rigidbody[0]->GetMass();
	rigidbody[1]->velocity = rigidbody[1]->velocity + k.DotProduct(contactNormal) / rigidbody[1]->GetMass();

	// Calculate new angularVelocity
	rigidbody[0]->angularVelocity = rigidbody[0]->angularVelocity - (rigidbody[0]->GetInverseInertiaTensorWorld() * (r1.CrossProduct(k.CrossProduct(contactNormal))));
	rigidbody[1]->angularVelocity = rigidbody[1]->angularVelocity + (rigidbody[1]->GetInverseInertiaTensorWorld() * (r2.CrossProduct(k.CrossProduct(contactNormal))));

	/*
	//Velocity in direction of contact
	float seperatingVelocity = CalculateSeparatingVelocity();

	if (seperatingVelocity > 0) {
		//stationary contact or separating
		return;
	}

	float newSepVelocity = -seperatingVelocity * restitution;

	// ## Resting contact resolution

	//check velocity buildUp due to linearAcceleration
	Vector3D accCausedVelocity = rigidbody[0]->GetLinearAcceleration();
	if (rigidbody[1]) { accCausedVelocity -= rigidbody[1]->GetLinearAcceleration(); }
	float accCausedSepVelocity = accCausedVelocity.DotProduct(contactNormal) * duration;

	//closing velocity due to linearAcceleration buildup
	//need to remove it
	if (accCausedSepVelocity < 0) {

		newSepVelocity += restitution * accCausedSepVelocity;

		if (newSepVelocity < 0) newSepVelocity = 0;
	}
	// ##

	float deltaVelocity = newSepVelocity - seperatingVelocity;


	float totalInverseMass = rigidbody[0]->GetInversedMass();
	if (rigidbody[1]) {
		totalInverseMass += rigidbody[1]->GetInversedMass();
	}

	//all particle has infinite mass
	if (totalInverseMass <= 0) return;

	float impulse = deltaVelocity / totalInverseMass;

	Vector3D impulsePerIMass = contactNormal * impulse;

	rigidbody[0]->velocity = rigidbody[0]->velocity + impulsePerIMass * rigidbody[0]->GetInversedMass();

	if (rigidbody[1]) {
		rigidbody[1]->velocity = rigidbody[1]->velocity + impulsePerIMass * -rigidbody[1]->GetInversedMass();
	}
	*/
}

void RigidBodyContact::ResolveInterpenetration()
{

	if (penetration <= 0)
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
	}

	/*Same book, different edition
	Vector3D movePerIMass = contactNormal * (-penetration / totalInversMass);
	rigidbody[0]->position += movePerIMass * rigidbody[0]->GetInversedMass();

	if (rigidbody[1]) {
		rigidbody[1]->position += movePerIMass * rigidbody[1]->GetInversedMass();
	}*/

	// PATCH: this line prevent the ResolveInterpenetration to be called several times during a frame
	penetration = 0;
}