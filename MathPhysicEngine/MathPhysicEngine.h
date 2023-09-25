#pragma once
#include "../Sources/ObjectData.h"
#include "Vecteur3D.h"
class MathPhysicsEngine {

public:

	ObjectData* objectData;

	void Init();
	void Update(double t, float frameTime);
	void Shutdown();



	MathPhysicsEngine(ObjectData* _objectData);
};
