#pragma once
#include <string>
#include "Component.h"
#include <vector>

class GameObject {

public:

	GameObject(std::string name = "No name");
	~GameObject();
	
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

	const std::string & GetName() const { return name; }

	bool shouldDelete = false;

private:
	std::string name;

	std::vector<Component*> components;

};