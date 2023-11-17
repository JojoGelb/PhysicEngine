#include "ParticleAnchoredSpring.h"

void ParticleAnchoredSpring::UpdateForce(Particle* particle, float duration)
{
	Vector3D force = particle->position;
	force -= anchor;

	float norm = force.GetNorm();
	norm = norm - m_restLength;
	norm *= m_springConstant;

	force = force.Normalize();
	force *= -norm;

	particle->AddForce(force);
}


ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3D anchor, float m_springConstant, float m_restLength)
	: anchor(anchor), m_springConstant(m_springConstant), m_restLength(m_restLength)
{
}
