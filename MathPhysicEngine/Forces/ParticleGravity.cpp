#include "ParticleGravity.h"

void ParticleGravity::UpdateForce(Particle* particle, float duration)
{
	//F = particle -> get mass * m_gravity;
	Vector3D gravityForce = m_gravity * 1/particle->GetInverseMass() * particle->gravity;

	particle->AddForce(gravityForce);
}


ParticleGravity::ParticleGravity(const Vector3D m_gravity)
	: m_gravity(m_gravity)
{
}
