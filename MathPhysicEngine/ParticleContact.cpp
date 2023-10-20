#include "ParticleContact.h"
#include <iostream>
void ParticleContact::Resolve(float duration)
{
	//std::cout << "before : " << CalculateSeparatingVelocity() << std::endl;
	ResolveVelocity(duration);

	ResolveInterpenetration();
	//std::cout << "after : " << CalculateSeparatingVelocity() << std::endl;
}

float ParticleContact::CalculateSeparatingVelocity()
{
	Vector3D relativeVelocity = particle[0]->velocity;
	if (particle[1]) {
		relativeVelocity -= particle[1]->velocity;
	}

	return relativeVelocity.DotProduct(contactNormal);
}

//Give proper impulse to solve contact
void ParticleContact::ResolveVelocity(float duration)
{
	//Velocity in direction of contact
	float seperatingVelocity = CalculateSeparatingVelocity();

	if (seperatingVelocity > 0) {
		//stationary contact or separating
		return;
	}

	float newSepVelocity = -seperatingVelocity * restitution;

	//check velocity buildUp due to acceleration
	Vector3D accCausedVelocity = particle[0]->acceleration;
	if (particle[1]) { accCausedVelocity -= particle[1]->acceleration; }
	float accCausedSepVelocity = accCausedVelocity.DotProduct(contactNormal) * duration;

	//closing velocity due to acceleration buildup
	//need to remove it
	if (accCausedSepVelocity < 0) {
	
		newSepVelocity += restitution * accCausedSepVelocity;

		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	float deltaVelocity = newSepVelocity - seperatingVelocity;

	
	float totalInverseMass = particle[0]->GetInverseMass();
	if (particle[1]) {
		totalInverseMass += particle[1]->GetInverseMass();
	}

	//all particle has infinite mass
	if (totalInverseMass <= 0) return;

	float impulse = deltaVelocity / totalInverseMass;

	Vector3D impulsePerIMass = contactNormal * impulse;

	particle[0]->velocity = particle[0]->velocity + impulsePerIMass * particle[0]->GetInverseMass();

	if (particle[1]) {
		particle[1]->velocity = particle[1]->velocity + impulsePerIMass * -particle[1]->GetInverseMass();
	}
}

void ParticleContact::ResolveInterpenetration()
{

	if (penetration <= 0) return;

	float totalInversMass = particle[0]->GetInverseMass();
	if (particle[1]) {
		totalInversMass += particle[1]->GetInverseMass();
	}

	if (totalInversMass <= 0) return;

	Vector3D movePerIMass = contactNormal * (penetration / totalInversMass);

	Vector3D particleAMovement = movePerIMass * particle[0]->GetInverseMass();
	Vector3D particleBMovement;
	if (particle[1]) {
		particleBMovement = movePerIMass * -particle[1]->GetInverseMass();
	}
	else {
		particleBMovement = Vector3D();
	}

	particle[0]->position += particleAMovement;

	if (particle[1]) {
		particle[1]->position += particleBMovement;
	}

	/*Same book, different edition
	Vector3D movePerIMass = contactNormal * (-penetration / totalInversMass);
	particle[0]->position += movePerIMass * particle[0]->GetInverseMass();

	if (particle[1]) {
		particle[1]->position += movePerIMass * particle[1]->GetInverseMass();
	}*/

	//PATCH: this line prevent the ResolveInterpenetration to be called several times during a frame
	penetration = 0;

}


//page 120