#include "Particle.h";
#include "MathPhysicEngine.h"
#include <iostream>

Particle::Particle(const Vector3D& _position, const Vector3D& _velocity, const Vector3D& _acceleration, float _inversedMass, float _damping,float _gravity)
	: position(_position), velocity(_velocity), acceleration(_acceleration), inversedMass(_inversedMass), damping(_damping), gravity(_gravity)
{
	gravityForce = { 0,-10,0 };
}

float Particle::GetInverseMass()
{
	return this->inversedMass;
}

void Particle::SetInverseMass(float inversedMass)
{
	this->inversedMass = inversedMass;
}

void Particle::Start()
{
	MathPhysicsEngine::GetInstance()->AddParticle(this);
}

void Particle::Update()
{
}

void Particle::Shutdown()
{
	MathPhysicsEngine::GetInstance()->RemoveParticle(this);
}

void Particle::SetTest() {
	SetInverseMass(1);
	force = Vector3D(10.0, 10.0, 10.0);
	velocity = 0;
	//position = 0;
	damping = 1;
}

void Particle::AddProjectile()
{
	gravity = 0;
	damping =1;
	force = Vector3D(0.0, 0.0, 0.0f);
	position = { 0.0,0.0,0.0 };
}

void Particle::SemiImpliciteEulerIntegration(double t, double dt)
{

	velocity = velocity * damping + (acceleration * dt);

	acceleration = (force * inversedMass) + (gravityForce * gravity);

	position = position + (velocity * dt);


	if (printParticleOnTerminal) {
		std::cout << "time: " << t << " - Position: " << position << " Velocity: " << velocity << "\n";
	}

	if (impulse) {
		force = { 0.0,0.0,0.0 };
	}
}


