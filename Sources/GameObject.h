#pragma once
#include "../MathPhysicEngine/Particle.h"
#include <VisualGameObject.h>
#include <string>
#include <VulkanHandler.h>
#include "../MathPhysicEngine/MathPhysicEngine.h"

class GameObject {

public:

	GameObject(MathPhysicsEngine& physicEngine, VulkanHandler& vulkanHandler,std::string modelePathName, std::string name = "No name");
	~GameObject();
	
	void UpdateVisual();

private:
	std::string name;
	Particle particle; //Physic handler
	VisualGameObject visual;

	VulkanHandler& vkHandler;
	MathPhysicsEngine& physicEngine;
};