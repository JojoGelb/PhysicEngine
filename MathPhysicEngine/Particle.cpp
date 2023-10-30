#include "Particle.h"
#include "MathPhysicEngine.h"
#include <iostream>

Particle::Particle(const Vector3D& _position, const Vector3D& _velocity, const Vector3D& _acceleration, float _inversedMass, float _damping,float _gravity)
	: position(_position), velocity(_velocity), acceleration(_acceleration), inversedMass(_inversedMass), damping(_damping), gravity(_gravity)
{
	previousState = { 0.0,0.0 ,0.0 };
	currentState = { 0.0,0.0 ,0.0 };
	finalState = { 0.0,0.0 ,0.0 };
	sumForce = { 0,0,0 };
	gravityForce = { 0,-gravity,0 };
}

Particle::~Particle()
{
	MathPhysicsEngine::GetInstance()->RemoveParticle(this);
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

void Particle::SemiImpliciteEulerIntegration(double t, double dt)
{
	previousState = { position ,velocity ,acceleration };

	velocity = velocity * damping + (acceleration * dt);

	acceleration = sumForce * inversedMass;

	position = position + (velocity * dt);

	/*
	if (printParticleOnTerminal) {
		std::cout << "time: " << t << " - Position: " << currentState.position << " Velocity: " << currentState.velocity << currentState.acceleration << "\n";
	}*/

	currentState = { position ,velocity ,acceleration };

	
}

void Particle::AddForce(Vector3D forceToAdd)
{
	sumForce += forceToAdd;
}

void Particle::ClearSumForce()
{
	sumForce = { 0,0,0 };
}

State::State()
{
	position = 0.0f;
	velocity = 0.0f;
	acceleration = 0.0f;
}

State::State(Vector3D _position, Vector3D _velocity, Vector3D _acceleration)
{
	this->position = _position;
	this->velocity = _velocity;
	this->acceleration = _acceleration;
}

State State::operator*(const double scalar) const
{
	return State(this->position * scalar, this->velocity * scalar, this->acceleration * scalar);
}

State State::operator+(const State& other) const
{
	return State(this->position + other.position, this->velocity + other.velocity, this->acceleration + other.acceleration);
}
