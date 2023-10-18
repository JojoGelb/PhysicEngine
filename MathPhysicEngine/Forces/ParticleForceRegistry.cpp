#include "ParticleForceRegistry.h"

void ParticleForceRegistry::UpdateForce(float duration)
{
	for (auto particleForceEntry : m_registry)
	{
		particleForceEntry.forceGenerator->UpdateForce(particleForceEntry.particle, duration);
	}
}
