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

	//add force at a point in world coordinate.
	void AddForceAtPoint(const Vector3D force, const Vector3D worldPoint);

	// add force a point in local coordinate.
	void AddForceAtBodyPoint(const Vector3D force, const Vector3D localPoint);

	//called each frame to clear forceAccum and torqueAccum
	void ClearAccumulator();
	
	//Get point in local Space with the transform matrix
	Vector3D GetPointInLocalSpace(const Vector3D worldPoint);

	//Get point in world Space with the transform matrix
	Vector3D GetPointInWorldSpace(const Vector3D localPoint);
};