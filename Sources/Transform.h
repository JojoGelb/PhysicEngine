#pragma once

#include "../MathPhysicEngine/Vecteur3D.h"

class Transform {

public:
    Vector3D position;
    Vector3D rotation;

    Transform(Vector3D position = Vector3D(), Vector3D rotation = Vector3D()) {};

    /*
    void Start() override{};
	void Update() override{};
	void Shutdown() override{};
    */
};