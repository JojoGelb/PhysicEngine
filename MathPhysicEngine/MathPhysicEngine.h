#pragma once
#include "../Sources/ObjectData.h"
#include "Vecteur3D.h"
class MathPhysicsEngine {

public:

	void Init();
	void Update(double t, float frameTime);
	void Shutdown();



	MathPhysicsEngine();
	void AddParticle(Particle* particule) { particles.push_back(particule); }

private:
	std::vector<Particle*> particles;
};
