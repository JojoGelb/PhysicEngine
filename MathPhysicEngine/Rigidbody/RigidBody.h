#pragma once
#include "../Vecteur3D.h"
#include "Component.h"
#include "Matrix34.h"
#include "Matrix33.h"
#include "Quaternion.h"

struct RigidBodyState
{
	//Vector3D position;
	//Vector3D velocity;
	//Vector3D acceleration;

	Matrix34 transformMatrix;
	RigidBodyState();
	RigidBodyState(Matrix34 _transformMatrix);
	RigidBodyState operator*(const double scalar) const;
	RigidBodyState operator+(const RigidBodyState& other) const;


};
class RigidBody : public Component {

private:
	//Same as Particle :

	Vector3D position;
	Vector3D velocity;
	Vector3D linearAcceleration;

	Quaternion orientation;
	Matrix33 inverseInertiaTensor;
	Matrix33 inverseInertiaTensorWorld;

	//Matrix34 transformMatrix;

	float linearDamping;
	float gravity;
	
	float inversedMass;

	Vector3D angularAcceleration;
	//Linear damping but for rotation
	float angularDamping;

	Vector3D forceAccum;

	Vector3D torqueAccum;

public:

	//States
	RigidBodyState previousState;
	RigidBodyState currentState;
	RigidBodyState finalState;
	
	Vector3D rotation;

	Matrix34 transformMatrix;

	RigidBody(const Vector3D& _position = { 0.0f }, const Vector3D& _velocity = { 0.0f }, const Vector3D& _linearAcceleration = { 0.0f }, const Vector3D& _rotation = { 0.0f }, const Quaternion& _orientation = { 1.0f,0.0f,0.0f,0.0f }, const Matrix33& _inverseInertiaTensor = { 0.0f }, float _linearDamping = 0.999f, float _gravity = 1.0f, float _inversedMass = 1.0f, float _angularDamping = 0.999f);
	~RigidBody();

	//Inherited from component
	void Start() override;
	void Update() override;
	void Shutdown() override;

	void SetGravityScale(float gravityScale) { gravity = gravityScale; };
	void SetInversedTensorAsACube(float mass, float dx, float dy, float dz);
	void SetOrientation(float w, float i, float j, float k) { orientation = { w,i,j,k }; };
	Vector3D GetPosition() { return position; };
	Vector3D GetVelocity() { return velocity; };
	Vector3D GetLinearAcceleration() { return linearAcceleration; };
	float GetLinearDamping() { return linearDamping; };
	float GetMass();
	float GetGravity() { return gravity; };
	float GetInversedMass() { return inversedMass; };
	Quaternion GetOrientation() { return orientation; };
	Vector3D GetRotation() { return rotation; };
	Vector3D GetAngularAcceleration() { return angularAcceleration; };
	Matrix33 GetInverseInertiaTensor() { return inverseInertiaTensor; };
	Matrix33 GetInverseInertiaTensorWorld() { return inverseInertiaTensorWorld; };
	float GetAngularDamping() { return angularDamping; };

	void Integrate(double time, double deltaTime);

	//add force on the center of mass, no torque generated
	void AddForce(const Vector3D force);
private:
	


	void CalculateDerivedData();

	void CalculateTransformMatrix(Matrix34& transformMatrix, const Vector3D& position, const Quaternion& orientation);

	void TransformInertiaTensorInWorld(const Quaternion& orientation);



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