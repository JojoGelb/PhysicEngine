#pragma once

#include"Vecteur3D.h"

class Particule
{
public:
	Particule(const Vector3D& _position = Vector3D(0.0f, 0.0f, 0.0f), const Vector3D& _velocity = Vector3D(0.0f, 0.0f, 0.0f), const Vector3D& _acceleration = Vector3D(0.0f, 0.0f, 0.0f), float _mass = 1.0f, float _damping = 0.999999999f);

	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;

	Vector3D force;


	float damping;

	float GetMass();
	void SetMass(float mass);

	float GetInverseMass();

	void SetTest();

	void SemiImpliciteEulerIntegration(double t, double dt);



private:

	float mass;
	float inversedMass;

	void SetInverseMass(float mass);
};

