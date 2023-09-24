#pragma once
#include "ParticulesData.h"
#include "Vecteur3D.h"
class MathPhysicsEngine {

public:

	ParticulesData* particulesData;

	void Init();
	void Update(double t, float frameTime);
	void Shutdown();



	MathPhysicsEngine(ParticulesData* particulesData);
};
