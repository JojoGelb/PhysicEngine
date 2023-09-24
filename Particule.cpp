#include "Particule.h";
#include <iostream>

Particule::Particule(const Vector3D& _position, const Vector3D& _velocity, const Vector3D& _acceleration, float _mass, float _damping)
	: position(_position), velocity(_velocity), acceleration(_acceleration), mass(_mass), inversedMass(1 / _mass), damping(_damping)
{
}

float Particule::GetInverseMass()
{
	return this->inversedMass;
}

void Particule::SetInverseMass(float mass)
{
	this->inversedMass = 1 / mass;
}

float Particule::GetMass()
{
	return mass;
}

void Particule::SetMass(float mass)
{
	SetInverseMass(mass);
	this->mass = mass;
}

void Particule::SetTest() {
	SetMass(1);
	force = Vector3D(10.0, 10.0, 10.0);
	velocity = 0;
	//position = 0;
	damping = 1;
}

void Particule::SemiImpliciteEulerIntegration(double t, double dt)
{

	velocity = velocity * damping + (acceleration * dt);

	acceleration = force * inversedMass;

	position = position + (velocity * dt);


	std::cout << "time: " << t << " - Position: " << position << " Velocity: " << velocity << "\n";

}


