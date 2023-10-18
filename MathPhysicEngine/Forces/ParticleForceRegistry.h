#pragma once
#include "../Particle.h"
#include "ParticleForceGenerator.h"
#include <vector>
class ParticleForceRegistry
{
private:
	struct ParticleForceEntry
	{
		Particle* particle;
		ParticleForceGenerator* forceGenerator;
		ParticleForceEntry(Particle* particle, ParticleForceGenerator* forceGenerator);
	};

	using Registry = std::vector<ParticleForceEntry>;
	Registry m_registry;

public:

	// Reigstry acccesor;
	
	void UpdateForce(float duration);
	void AddForce(Particle* particle, ParticleForceGenerator* forceGenerator);
};

