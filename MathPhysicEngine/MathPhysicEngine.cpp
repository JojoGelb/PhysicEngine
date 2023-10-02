#include "MathPhysicEngine.h"



MathPhysicsEngine::MathPhysicsEngine()
{
}

//Static methods should be defined outside the class.
MathPhysicsEngine* MathPhysicsEngine::GetInstance()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (singleton_ == nullptr) {
		singleton_ = new MathPhysicsEngine();
		singleton_->Init();
	}
	return singleton_;
}

void MathPhysicsEngine::Init()
{

	//Initialisation of pre-existing gameObjects
	/*for (int i = 0; i < objectData->gameObjects.size(); i++) {

		Particle particle = Particle();

		particle.position = Vector3D(objectData->gameObjects[i].transform.translation.x, objectData->gameObjects[i].transform.translation.y, objectData->gameObjects[i].transform.translation.z);
		particle.gravity = 0;
		particle.gravityForce = { 0,-10,0 };
		objectData->particles.push_back(particle);
	}*/
}

void MathPhysicsEngine::Update(double t,float frameTime)
{
	
	 //std::cout << frameTime << "\n";
	for (Particle * p : particles) {

		//objectData->particles[i].position += Vector3D(0.0f,0.0f,0.01f);
		p->SemiImpliciteEulerIntegration(t, (double)frameTime);
	}
}

MathPhysicsEngine* MathPhysicsEngine::singleton_ = nullptr;
