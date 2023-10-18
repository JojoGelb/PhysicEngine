#pragma once

#include"Vecteur3D.h"
#include "Component.h"
#include "ParticleCollider.h"

class Particle : public Component
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

	Particle(const Vector3D& _position = Vector3D(0.0f, 0.0f, 0.0f), const Vector3D& _velocity = Vector3D(0.0f, 0.0f, 0.0f), const Vector3D& _acceleration = Vector3D(0.0f, 0.0f, 0.0f), float _inversedMass = 1.0f, float _damping = 0.999999999f,float _gravity = 10);

	float GetInverseMass();
	float GetMass() { return 1 / inversedMass; };

	void SetTest();

	void AddProjectile();

	void SemiImpliciteEulerIntegration(double t, double dt);

	void Start() override;
	void Update() override;
	void Shutdown() override;

private:

	void SetInverseMass(float mass);

	// Hérité via Component

};

