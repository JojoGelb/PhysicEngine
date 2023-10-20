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

void ParticleForceRegistry::RemoveParticle(Particle* particle)
{
	for (int i = 0; i < m_registry.size(); i++) {
	
		if (m_registry.at(i).particle == particle) {
			
			auto iterator = m_registry.begin() + i;
			m_registry.erase(iterator);

		}
	
	}
}

ParticleForceRegistry::ParticleForceEntry::ParticleForceEntry(Particle* particle, ParticleForceGenerator* forceGenerator) : particle(particle), forceGenerator(forceGenerator){}
