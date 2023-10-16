#include "ParticleForceRegistry.h"

void ParticleForceRegistry::UpdateForce(float duration)
{
	for (auto particleForceEntry : m_registry)
	{
		particleForceEntry.forceGenerator->UpdateForce(particleForceEntry.particle, duration);
	}
}

void ParticleForceRegistry::ClearForces()
{
	for (auto particleForceEntry : m_registry)
	{
		particleForceEntry.particle->ClearSumForce();
	}
}
