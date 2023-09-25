#include "GameObject.h"


GameObject::GameObject(MathPhysicsEngine& _physicEngine, VulkanHandler& v, std::string modelePath, std::string _name):
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
}

GameObject::~GameObject()
{
	physicEngine.RemoveParticle(&particle);
	vkHandler.RemoveGameObject2(&visual);
}

void GameObject::UpdateVisual()
{
	visual.transform.translation.x = particle.position.x;
	visual.transform.translation.y = -particle.position.y;
	visual.transform.translation.z = particle.position.z;
}
