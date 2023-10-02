#pragma once
#include <string>
#include "Component.h"
#include <vector>

class GameObject {

public:

	//GameObject(MathPhysicsEngine& physicEngine, VulkanHandler& vulkanHandler,std::string modelePathName, std::string name = "No name");
	GameObject(std::string name = "No name");
	~GameObject();
	
	//void UpdateVisual();
	void Update();
	
	template <typename T>
	T* GetComponentOfType() {
		for (Component* component : components) {
			if (typeid(*component) == typeid(T)) {
				return dynamic_cast<T*>(component);
			}
		}
		return nullptr;
	}

	void AddComponent(Component* component);

private:
	std::string name;

	std::vector<Component*> components;

	//Particle* particle; //Physic handler
	//VisualGameObject* visual;

	//VulkanHandler& vkHandler;
	//MathPhysicsEngine& physicEngine;
};