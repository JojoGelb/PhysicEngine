#include "RigidBodySpring.h"

void RigidBodySpring::UpdateForce(RigidBody* rigidBody, float duration)
{
	Vector3D localWorldSpace = rigidBody->GetPointInWorldSpace(connectionPoint);
	Vector3D otherWorldSpace = other->GetPointInWorldSpace(otherConnectionPoint);
	
	Vector3D force = localWorldSpace - otherWorldSpace;
	
	//force -= other->GetPosition();

	float norm = force.GetNorm();
	norm = norm - m_restLength;
	norm *= m_springConstant;

	force = force.Normalize();
	force *= -norm;

	rigidBody->AddForceAtPoint(force,localWorldSpace);
}


RigidBodySpring::RigidBodySpring(RigidBody* other, Vector3D connectionPoint,Vector3D otherConnectionPoint, float m_springConstant, float m_restLength)
	: other(other), m_springConstant(m_springConstant), m_restLength(m_restLength), connectionPoint(connectionPoint), otherConnectionPoint(otherConnectionPoint)
{
}
