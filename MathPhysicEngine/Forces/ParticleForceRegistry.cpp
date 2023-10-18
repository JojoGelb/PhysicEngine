#include "ParticleForceRegistry.h"

void ParticleForceRegistry::UpdateForce(float duration)
{
	for (ParticleForceEntry particleForceEntry : m_registry)
	{
		particleForceEntry.forceGenerator->UpdateForce(particleForceEntry.particle, duration);
	}
}

void ParticleForceRegistry::AddForce(Particle* particle, ParticleForceGenerator* forceGenerator)
{
	m_registry.push_back(ParticleForceRegistry::ParticleForceEntry(particle, forceGenerator));
}

ParticleForceRegistry::ParticleForceEntry::ParticleForceEntry(Particle* particle, ParticleForceGenerator* forceGenerator) : particle(particle), forceGenerator(forceGenerator){}
