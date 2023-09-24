#include "MathPhysicEngine.h"


void MathPhysicsEngine::Init()
{

	
	for (int i = 0; i < particulesData->gameObjects.size(); i++) {

		Particule particule = Particule();
		particule.SetTest();
		particule.SetMass(200);

		particule.position = Vector3D(particulesData->gameObjects[i].transform.translation.x, particulesData->gameObjects[i].transform.translation.y, particulesData->gameObjects[i].transform.translation.z);

		particulesData->Particules.push_back(particule);
	}
}

void MathPhysicsEngine::Update(double t,float frameTime)
{
	
	 //std::cout << frameTime << "\n";
	for (int i = 0; i < particulesData->Particules.size(); i++) {

		//particulesData->Particules[i].position += Vector3D(0.0f,0.0f,0.01f);
		particulesData->Particules[i].SemiImpliciteEulerIntegration(t, (double)frameTime);
	}
}


MathPhysicsEngine::MathPhysicsEngine(ParticulesData* particulesData)
	: particulesData(particulesData)
{
}
