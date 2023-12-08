#pragma once

#include "../MathPhysicEngine/Vecteur3D.h"
#include "../MathPhysicEngine/Rigidbody/Matrix34.h"

class Transform {

public:
    Vector3D position;
    Vector3D rotation;

    Matrix34 transformMatrix; //Only for Rigidbody

    Transform(Vector3D position = Vector3D(), Vector3D rotation = Vector3D()) {};

    /*
    void Start() override{};
	void Update() override{};
	void Shutdown() override{};
    */
};