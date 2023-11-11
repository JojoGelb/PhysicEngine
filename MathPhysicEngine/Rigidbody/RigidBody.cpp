#include "RigidBody.h"

void RigidBody::SetInversedTensorAsACube(float mass, float dx, float dy, float dz)
{
	inverseInertiaTensor = { 0.0f};

	float xAxe = 1.0f / 12.0f * mass * (dy * dy + dz * dz);
	float yAxe = 1.0f / 12.0f * mass * (dx * dx + dz * dz);
	float zAxe = 1.0f / 12.0f * mass * (dx * dx + dy * dy);

	inverseInertiaTensor.SetDiagonal(xAxe, yAxe, zAxe);
	inverseInertiaTensor = inverseInertiaTensor.Inverse();
}

void RigidBody::Integrate(double time, double deltaTime)
{
	// 1 Mettre � jour la position
	position = position + (velocity * deltaTime);

	// 2 Mettre � jour l�orientation
	orientation.UpdateByAngularVelocity(rotation, deltaTime);
	// 3 Calculer les valeurs d�riv�es
	CalculateDerivedData();

	// 4 Calculer l�acc�l�ration lin�aire
	acceleration = forceAccum * inversedMass;

	// 5 Calculer l�acc�l�ration angulaire
	Vector3D angularAcceleration = inverseInertiaTensorWorld * torqueAccum;

	// 6 Mettre � jour la v�locit� lin�aire
	velocity = velocity * linearDamping + (acceleration * deltaTime);

	// 7 Mettre � jour la v�locit� angulaire
	orientation.UpdateByAngularVelocity(angularAcceleration, deltaTime);

	// 8 Remettre � z�ro les accumulateurs
	ClearAccumulator();
}

void RigidBody::CalculateDerivedData()
{
	// Calculate the transform matrix for the body.
	
	CalculateTransformMatrix(this->transformMatrix, this->position, this->orientation);
	TransformInertiaTensorInWorld(this->orientation);
}

void RigidBody::CalculateTransformMatrix(Matrix34 &transformMatrix, const Vector3D &position,const Quaternion &orientation)
{
	transformMatrix.SetOrientationAndPosition(orientation, position);
}

void RigidBody::TransformInertiaTensorInWorld(const Quaternion& orientation)
{
	Matrix33 rotationMatrix;
	rotationMatrix.SetOrientation(orientation);
	inverseInertiaTensorWorld = rotationMatrix * inverseInertiaTensor * rotationMatrix.Inverse();
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

Vector3D RigidBody::GetPointInLocalSpace(const Vector3D worldPoint)
{
	return Vector3D();
}

Vector3D RigidBody::GetPointInWorldSpace(const Vector3D localPoint)
{
	return Vector3D();
}

float RigidBody::GetMass()
{
	return 1.0f/inversedMass;
}
