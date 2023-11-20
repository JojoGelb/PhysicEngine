#include "ParticleSpring.h"

void ParticleSpring::UpdateForce(Particle* particle, float duration)
{
	Vector3D force = particle->position;
	force -= other->position;

	float norm = force.GetNorm();
	norm = norm - m_restLength;
	norm *= m_springConstant;

	force = force.Normalize();
	force *= -norm;

	particle->AddForce(force);
}


ParticleSpring::ParticleSpring(Particle* other, float m_springConstant, float m_restLength)
	: other(other), m_springConstant(m_springConstant), m_restLength(m_restLength)
{
}
