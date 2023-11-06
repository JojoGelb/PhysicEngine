#include "RigidBody.h"

void RigidBody::Integrate(double time, double deltaTime)
{
	// 1 Mettre à jour la position
	// 2 Mettre à jour l’orientation
	// 3 Calculer les valeurs dérivées
	// 4 Calculer l’accélération linéaire
	// 5 Calculer l’accélération angulaire
	// 6 Mettre à jour la vélocité linéaire
	// 7 Mettre à jour la vélocité angulaire
	// 8 Remettre à zéro les accumulateurs
	ClearAccumulator();
}

void RigidBody::CalculateDerivedData()
{
	// Calculate the transform matrix for the body.
}

void RigidBody::AddForce(const Vector3D force)
{
	forceAccum += force;
}

void RigidBody::AddForceAtPoint(const Vector3D force, const Vector3D worldPoint)
{
	//set point where force is applied
	Vector3D point = worldPoint - position;
	forceAccum += force;
	torqueAccum += point.CrossProduct(force);
}

void RigidBody::AddForceAtBodyPoint(const Vector3D force, const Vector3D localPoint)
{
	Vector3D worldPoint = GetPointInWorldSpace(localPoint);
	AddForceAtPoint(force, worldPoint);
}

void RigidBody::ClearAccumulator()
{
	forceAccum = { 0.0f, 0.0f, 0.0f };
	torqueAccum = { 0.0f, 0.0f, 0.0f };
}
