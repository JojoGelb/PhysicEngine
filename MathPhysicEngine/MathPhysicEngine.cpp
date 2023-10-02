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

MathPhysicsEngine* MathPhysicsEngine::singleton_ = nullptr;
