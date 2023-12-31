#pragma once

#include"Vecteur3D.h"
#include "../Headers/Component.h"
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

class Particle : public Component
{
public:

	Particle(const Vector3D& _position = { 0.0f }, const Vector3D& _velocity = { 0.0f }, const Vector3D& _acceleration = { 0.0f }, float _inversedMass = 1.0f, float _damping = 0.999f, float _gravity = 1.0f);
	~Particle();

	Vector3D inputValues = { 0.f,0.f,0.f };

	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;

	Vector3D gravityForce;

	//States
	State previousState;
	State currentState;
	State finalState;

	float damping;
	float gravity;
	float inversedMass;

	void Start() override;
	void Update() override;
	void Shutdown() override;

	float GetInverseMass();
	float GetMass() { return 1 / inversedMass; };

	void SemiImpliciteEulerIntegration(double t, double dt);

	void AddForce(Vector3D forceToAdd);

	void ClearSumForce();

private:

	Vector3D sumForce;
	void SetInverseMass(float mass);

	// Hérité via Component

};

