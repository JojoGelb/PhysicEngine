#pragma once
#include "../MathPhysicEngine/Particle.h"
#include <VisualGameObject.h>
#include <string>
#include <VulkanHandler.h>

class GameObject {

public:

	GameObject(Particle particle, VulkanHandler& v,std::string modelePathName, std::string name = "No name");
	~GameObject();
	
	void UpdateVisual();

private:
	std::string name;
	Particle particle; //Physic handler
	VisualGameObject visual;
	VulkanHandler& vk;
};