#pragma once

#include"Vecteur3D.h"

class Particle
{
public:

	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;

	Vector3D force;
	Vector3D gravityForce;

	float damping;
	float gravity;

	bool impulse = false;
	bool printParticleOnTerminal = false;
	float inversedMass;

	Particle(const Vector3D& _position = Vector3D(0.0f, 0.0f, 0.0f), const Vector3D& _velocity = Vector3D(0.0f, 0.0f, 0.0f), const Vector3D& _acceleration = Vector3D(0.0f, 0.0f, 0.0f), float _mass = 1.0f, float _damping = 0.999999999f,float _gravity = 10);

	float GetInverseMass();

	void SetTest();

	void AddProjectile();

	void SemiImpliciteEulerIntegration(double t, double dt);


private:

	void SetInverseMass(float mass);
};

