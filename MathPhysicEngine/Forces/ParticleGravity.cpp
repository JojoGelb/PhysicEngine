#include "ParticleGravity.h"

void ParticleGravity::UpdateForce(Particle* particle, float duration)
{
	//F = particle -> get mass * m_gravity;
	Vector3D gravityForce = m_gravity * particle->GetInverseMass();

	particle->AddForce(gravityForce);
}


ParticleGravity::ParticleGravity(const Vector3D m_gravity)
	: m_gravity(m_gravity)
{
}
