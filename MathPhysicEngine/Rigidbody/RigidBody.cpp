#include "RigidBody.h"

void RigidBody::Integrate(double time, double deltaTime)
{
	// 1 Mettre � jour la position
	// 2 Mettre � jour l�orientation
	// 3 Calculer les valeurs d�riv�es
	// 4 Calculer l�acc�l�ration lin�aire
	// 5 Calculer l�acc�l�ration angulaire
	// 6 Mettre � jour la v�locit� lin�aire
	// 7 Mettre � jour la v�locit� angulaire
	// 8 Remettre � z�ro les accumulateurs
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
