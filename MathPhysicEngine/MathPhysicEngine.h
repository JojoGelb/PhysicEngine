#pragma once
#include "../Sources/ObjectData.h"
#include "Vecteur3D.h"
#include "Forces/ParticleForceRegistry.h"
class MathPhysicsEngine {

public:

	ObjectData* objectData;

	void Init();
	void Update(double t, float frameTime);
	void Shutdown();

	void SetFinalStates(const double alpha);


	MathPhysicsEngine(ObjectData* _objectData);

	ParticleForceRegistry* particleForceRegistry;

private:
	void UpdateSumForces(float frameTime);
};
