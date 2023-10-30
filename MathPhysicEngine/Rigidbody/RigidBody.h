#pragma once
#include "../Vecteur3D.h"
#include "Component.h"

class RigidBody : public Component {

private:
	//Same as Particle :

	Vector3D position;
	Vector3D velocity;
	//Vector3D acceleration;

	Vector3D rotation;

	//Quaternion orientation;
	// 
	//Matrix34 transformMatrix;

	float linearDamping;
	float gravity;
	float inversedMass;


	//Linear damping but for rotation
	float angularDamping;

	Vector3D forceAccum;

	Vector3D torqueAccum;

public:

	//Inherited from component
	void Start() override;
	void Update() override;
	void Shutdown() override;

private:

	void Integrate(double time, double deltaTime);

	void CalculateDerivedData();

	//add force on the center of mass, no torque generated
	void AddForce(const Vector3D force);

	//add force in world coordinate.
	void AddForceAtPoint();

	// add force in local coordinate.
	void AddForceAtBodyForce();

	//called each frame to clear forceAccum and torqueAccum
	void ClearAccumulator();
};