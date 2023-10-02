#include "ParticleGravity.h"

void ParticleGravity::UpdateForce(Particle* particle, float duration)
{
	//F = particle -> get mass * m_gravity;
	Vector3D f = m_gravity * particle->GetInverseMass();
}
