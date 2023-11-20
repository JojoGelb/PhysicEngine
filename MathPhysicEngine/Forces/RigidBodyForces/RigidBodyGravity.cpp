#include "RigidBodyGravity.h"

void RigidBodyGravity::UpdateForce(RigidBody* rigidBody, float duration)
{
	//F = particle -> get mass * m_gravity;
	Vector3D gravityForce = m_gravity * rigidBody->GetMass() * rigidBody->GetGravity();

	rigidBody->AddForce(gravityForce);
}


RigidBodyGravity::RigidBodyGravity(const Vector3D m_gravity)
	: m_gravity(m_gravity)
{
}
