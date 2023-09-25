#include "GameObject.h"


GameObject::GameObject(Particle _particle, VulkanHandler& v, std::string modelePath, std::string _name):
	particle(std::move(_particle)),
	vk(v),
	visual(VisualGameObject::CreateGameObject()),
	name(_name)
{
	std::shared_ptr<Model> lveModel = Model::CreateModelFromFile(v.GetGraphicDevice(), modelePath);
	visual.model = lveModel;
	visual.transform.translation = { 0.0f, .0f, 0.0f };
	visual.transform.scale = { 1.f, 1.f, 1.f };
	vk.AddGameObject2(&visual);
}

GameObject::~GameObject()
{
	//vk.RemoveGameObject2(&visual);
}

void GameObject::UpdateVisual()
{
	visual.transform.translation.x = particle.position.x;
	visual.transform.translation.y = -particle.position.y;
	visual.transform.translation.z = particle.position.z;
}
