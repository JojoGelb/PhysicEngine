#pragma once

#include"Vecteur3D.h"

struct State
{
	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;

	State();
	State(Vector3D _position, Vector3D _velocity, Vector3D _acceleration);
	State operator*(const double scalar) const;
	State operator+(const State& other) const;


};
class Particle
{
public:
	Particle(const Vector3D& _position = Vector3D(0.0f, 0.0f, 0.0f), const Vector3D& _velocity = Vector3D(0.0f, 0.0f, 0.0f), const Vector3D& _acceleration = Vector3D(0.0f, 0.0f, 0.0f), float _mass = 1.0f, float _damping = 0.999999999f,float _gravity = 10);


	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;

	State previousState;
	State currentState;
	State finalState;

	Vector3D force;
	Vector3D gravityForce;

	float damping;
	float gravity;

	float GetInverseMass();

	void SetTest();

	void AddProjectile();

	void SemiImpliciteEulerIntegration(double t, double dt);


	bool impulse = false;
	bool printParticleOnTerminal = false;
	float inversedMass;

private:


	void SetInverseMass(float mass);
};

