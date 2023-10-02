#include "GameObject.h"
#include "../MathPhysicEngine/Particle.h"
#include <VisualGameObject.h>


/*GameObject::GameObject(MathPhysicsEngine& _physicEngine, VulkanHandler& v, std::string modelePath, std::string _name) :
	particle({}),
	physicEngine(_physicEngine),
	vkHandler(v),
	visual(VisualGameObject::CreateGameObject()),
	name(_name)
{
	std::shared_ptr<Model> lveModel = Model::CreateModelFromFile(v.GetGraphicDevice(), modelePath);
	visual.model = lveModel;
	visual.transform.translation = { 0.0f, .0f, 0.0f };
	visual.transform.scale = { 1.f, 1.f, 1.f };
	vkHandler.AddGameObject2(&visual);
	physicEngine.AddParticle(&particle);

	particle.gravity = 0;
}*/

GameObject::GameObject(std::string _name): name(_name)
{
}

GameObject::~GameObject()
{
	for (Component* comp : components) {
		comp->Shutdown();
		delete comp;
	}
	//physicEngine.RemoveParticle(&particle);
	//vkHandler.RemoveGameObject2(&visual);
}

/*void GameObject::UpdateVisual()
{
	visual.transform.translation.x = particle.position.x;
	visual.transform.translation.y = -particle.position.y;
	visual.transform.translation.z = particle.position.z;
}*/

void GameObject::Update()
{
	for(Component* comp: components)
	{
		comp->Update();
	}
	//TODO Link graphics and physics
	Particle* p = GetComponentOfType<Particle>();
	VisualGameObject * v = GetComponentOfType<VisualGameObject>();

	if (p != nullptr && v != nullptr) {
		v->transform.translation.x = p->position.x;
		v->transform.translation.y = -p->position.y;
		v->transform.translation.z = p->position.z;
	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
	component->Start();
}
