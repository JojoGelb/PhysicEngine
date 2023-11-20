#include "ParticleForceRegistry.h"
#include <iostream>

void ParticleForceRegistry::UpdateForce(float duration)
{
	for (ParticleForceEntry particleForceEntry : m_registry)
	{
		particleForceEntry.forceGenerator->UpdateForce(particleForceEntry.particle, duration);
	}
}

void ParticleForceRegistry::AddForce(Particle* particle, ParticuleForceGenerator* forceGenerator)
{
	m_registry.push_back(ParticleForceRegistry::ParticleForceEntry(particle, forceGenerator));
}

void ParticleForceRegistry::DeleteForce(Particle* particle, ParticuleForceGenerator* forceGenerator)
{
	for (int i = 0; i < m_registry.size(); i++) {
		if(m_registry.at(i).particle == particle) {
			if (m_registry.at(i).forceGenerator == forceGenerator) {

				m_registry.erase(m_registry.begin() + i);
			}

		}
	}
}

void ParticleForceRegistry::RemoveParticle(Particle* particle)
{
	for (int i = 0; i < m_registry.size(); i++) {
	
		if (m_registry.at(i).particle == particle) {
			
			auto iterator = m_registry.begin() + i;
			m_registry.erase(iterator);
			i--;
			std::cout << "";
		}
	
	}
}

ParticleForceRegistry::ParticleForceEntry::ParticleForceEntry(Particle* particle, ParticuleForceGenerator* forceGenerator) : particle(particle), forceGenerator(forceGenerator){}
