#pragma once
#include "../Sources/ObjectData.h"
#include "Vecteur3D.h"
class MathPhysicsEngine {

public:

	void Init();
	void Update(double t, float frameTime);
	void Shutdown();



	MathPhysicsEngine();
	void AddParticle(Particle* p) { particles.push_back(p); }
	void RemoveParticle(Particle* p) { particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end()); }

private:
	std::vector<Particle*> particles;
};
