#include "ParticleSpring.h"

void ParticleSpring::UpdateForce(Particle* particle, float duration)
{
	Vector3D force = particle->position;
	force -= other->position;

	float norm = force.GetNorm();
	norm = norm - m_restLength;
	norm *= m_springConstant;

	force.Normalize();
	force *= -norm;

	particle->AddForce(force);
}
