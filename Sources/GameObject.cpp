#include "GameObject.h"
#include "../MathPhysicEngine/Particle.h"
#include <VisualGameObject.h>

GameObject::GameObject(std::string _name): name(_name)
{
}

GameObject::~GameObject()
{
	for (Component* comp : components) {
		comp->Shutdown();
		delete comp;
	}
}


void GameObject::Update()
{
	for(Component* comp: components)
	{
		comp->Update();
	}
	//TODO Do something better like a transform component mandatory
	Particle* p = GetComponentOfType<Particle>();
	VisualGameObject * v = GetComponentOfType<VisualGameObject>();

	if (p != nullptr && v != nullptr) {
		v->transform.translation.x = p->finalState.position.x;
		v->transform.translation.y = -p->finalState.position.y;
		v->transform.translation.z = p->finalState.position.z;
	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
	component->Start();
}
