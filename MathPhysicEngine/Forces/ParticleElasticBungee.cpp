#include "ParticleElasticBungee.h"

void ParticleElasticBungee::UpdateForce(Particle* particle, float duration)
{
	Vector3D force = particle->position;
	force -= other->position;

	//Check if bungee is compressed
	float norm = force.GetNorm();
	if (norm <= m_restLength) return;

	norm = norm - m_restLength;
	norm *= m_springConstant;

	force = force.Normalize();
	force *= -norm;

	particle->AddForce(force);
}


ParticleElasticBungee::ParticleElasticBungee(Particle* other, float m_springConstant, float m_restLength)
	: other(other), m_springConstant(m_springConstant), m_restLength(m_restLength)
{
}
