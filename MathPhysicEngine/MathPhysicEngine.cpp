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
}

void MathPhysicsEngine::Update(double t,float frameTime)
{
	 //std::cout << frameTime << "\n";
	for (Particle * p : particles) {
		p->SemiImpliciteEulerIntegration(t, (double)frameTime);
	}
}

void MathPhysicsEngine::Shutdown()
{
	delete this->particleForceRegistry;
}

void MathPhysicsEngine::SetFinalStates(const double alpha)
{
	/*
	for (int i = 0; i < objectData->particles.size(); i++) {

		objectData->particles[i].finalState = objectData->particles[i].currentState * alpha +
			objectData->particles[i].previousState * (1.0 - alpha);
	}*/

	for (Particle* p : particles) {
		p->finalState = p->currentState * alpha + p->previousState * (1.0 - alpha);
	}
}

void MathPhysicsEngine::UpdateSumForces(float frameTime)
{
	/*
	for (auto particle : objectData->particles) 
	{
		particle.ClearSumForce();
	}*/

	for (Particle* p : particles) {
		p->ClearSumForce();
	}

	particleForceRegistry->UpdateForce(frameTime);

}
MathPhysicsEngine* MathPhysicsEngine::singleton_ = nullptr;
