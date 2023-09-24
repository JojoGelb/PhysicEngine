#include "MathPhysicEngine.h"


void MathPhysicsEngine::Init()
{

	//Initialisation of pre-existing gameObjects
	for (int i = 0; i < objectData->gameObjects.size(); i++) {

		Particle particle = Particle();

		particle.position = Vector3D(objectData->gameObjects[i].transform.translation.x, objectData->gameObjects[i].transform.translation.y, objectData->gameObjects[i].transform.translation.z);

		objectData->particles.push_back(particle);
	}
}

void MathPhysicsEngine::Update(double t,float frameTime)
{
	
	 //std::cout << frameTime << "\n";
	for (int i = 0; i < objectData->particles.size(); i++) {

		//objectData->particles[i].position += Vector3D(0.0f,0.0f,0.01f);
		objectData->particles[i].SemiImpliciteEulerIntegration(t, (double)frameTime);
	}
}


MathPhysicsEngine::MathPhysicsEngine(ObjectData* _objectData)
	: objectData(_objectData)
{
}
