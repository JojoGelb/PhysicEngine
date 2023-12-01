#include "RigidBody.h"
#include "../MathPhysicEngine.h"

RigidBodyState::RigidBodyState()
{
	
}

RigidBodyState::RigidBodyState(Matrix34 _transformMatrix)
	:transformMatrix(_transformMatrix)
{
}

RigidBodyState RigidBodyState::operator*(const double scalar) const
{
	return {this->transformMatrix * scalar};
}

RigidBodyState RigidBodyState::operator+(const RigidBodyState& other) const
{
	return {this->transformMatrix + other.transformMatrix};
}

RigidBody::RigidBody(const std::string _inertiaTensorSelection, const Vector3D& _position, const Vector3D& _velocity, const Vector3D& _linearAcceleration, const Vector3D& _rotation, const Quaternion& _orientation, const Matrix33& _inverseInertiaTensor, float _linearDamping, float _gravity, float _inversedMass, float _angularDamping, BoundingSphere _boundingSphere)
	: inertiaTensorSelection(_inertiaTensorSelection), position(_position), velocity(_velocity), linearAcceleration(_linearAcceleration), rotation(_rotation), orientation(_orientation), inverseInertiaTensor(_inverseInertiaTensor), linearDamping(_linearDamping), gravity(_gravity), inversedMass(_inversedMass), angularDamping(_angularDamping), boundingSphere(_boundingSphere)
{
	previousState = { 0.0f };
	currentState = { 0.0f };
	finalState = { 0.0f };
	
	forceAccum = { 0,0,0 };
	torqueAccum = { 0,0,0 };
	
	transformMatrix = { 0.0f };
	CalculateTransformMatrix(transformMatrix, position, orientation);
	angularAcceleration = { 0,0,0 };

	if(inertiaTensorSelection == "sphere")
	{
		SetInversedTensorAsASphere(GetMass(), 1.0f);
	}
	else
	{
		SetInversedTensorAsACube(GetMass(), 1.0f, 1.0f, 1.0f);
	}
}

RigidBody::~RigidBody()
{
	MathPhysicsEngine::GetInstance()->RemoveRigidBody(this);
}


void RigidBody::Start()
{
	MathPhysicsEngine::GetInstance()->AddRigidBody(this);
}

void RigidBody::Update()
{
}

void RigidBody::Shutdown()
{
	MathPhysicsEngine::GetInstance()->RemoveRigidBody(this);
}

void RigidBody::SetInversedTensorAsACube(float mass, float dx, float dy, float dz)
{
	inverseInertiaTensor = { 0.0f};

	float xAxe = 1.0f / 12.0f * mass * (dy * dy + dz * dz);
	float yAxe = 1.0f / 12.0f * mass * (dx * dx + dz * dz);
	float zAxe = 1.0f / 12.0f * mass * (dx * dx + dy * dy);

	inverseInertiaTensor.SetDiagonal(xAxe, yAxe, zAxe);
	inverseInertiaTensor = inverseInertiaTensor.Inverse();

	
}

void RigidBody::SetInversedTensorAsASphere(float mass, float radius)
{
	inverseInertiaTensor = { 0.0f};

	float axe = 2.0f / 5.0f * mass * radius * radius;

	inverseInertiaTensor.SetDiagonal(axe, axe, axe);
	inverseInertiaTensor = inverseInertiaTensor.Inverse();
}

void RigidBody::Integrate(double time, double deltaTime)
{

	previousState = { transformMatrix };
	
	// 1 Mettre à jour la position
	position = position + (velocity * deltaTime);

	// 2 Mettre à jour l’orientation
	orientation.UpdateByAngularVelocity(rotation, deltaTime);
	orientation.Normalized();
	
	// 3 Calculer les valeurs dérivées
	CalculateDerivedData();

	// 4 Calculer l’accélération linéaire
	linearAcceleration = forceAccum * inversedMass;

	// 5 Calculer l’accélération angulaire
	angularAcceleration = inverseInertiaTensorWorld * torqueAccum;

	// 6 Mettre à jour la vélocité linéaire
	velocity = velocity * linearDamping + (linearAcceleration * deltaTime);

	// 7 Mettre à jour la vélocité angulaire
	rotation = rotation * angularDamping + (angularAcceleration * deltaTime);

	// 8 Remettre à zéro les accumulateurs
	ClearAccumulator();

	currentState = { transformMatrix };
}

void RigidBody::CalculateDerivedData()
{
	// Calculate the transform matrix for the body.
	
	CalculateTransformMatrix(this->transformMatrix, this->position, this->orientation);
	TransformInertiaTensorInWorld(this->orientation);
}

void RigidBody::CalculateTransformMatrix(Matrix34 &transformMatrix, const Vector3D &position,const Quaternion &orientation)
{
	transformMatrix.SetOrientationAndPosition(orientation, position);
}

void RigidBody::TransformInertiaTensorInWorld(const Quaternion& orientation)
{
	Matrix33 rotationMatrix;
	rotationMatrix.SetOrientation(orientation);
	inverseInertiaTensorWorld = rotationMatrix * inverseInertiaTensor * rotationMatrix.Inverse();

	
	inverseInertiaTensorWorld.DeleteMinusZero();
}

void RigidBody::AddForce(const Vector3D force)
{
	forceAccum += force;
}

void RigidBody::AddForceAtPoint(const Vector3D force, const Vector3D worldPoint)
{
	//set point where force is applied
	Vector3D point = worldPoint - position;
	forceAccum += force;
	torqueAccum += point.CrossProduct(force);
}

void RigidBody::AddForceAtBodyPoint(const Vector3D force, const Vector3D localPoint)
{
	Vector3D worldPoint = GetPointInWorldSpace(localPoint);
	AddForceAtPoint(force, worldPoint);
}

void RigidBody::ClearAccumulator()
{
	forceAccum = { 0.0f, 0.0f, 0.0f };
	torqueAccum = { 0.0f, 0.0f, 0.0f };
}

Vector3D RigidBody::GetPointInLocalSpace(const Vector3D worldPoint)
{
	return Vector3D();
}

Vector3D RigidBody::GetPointInWorldSpace(const Vector3D localPoint)
{
	return transformMatrix.TransformPosition(localPoint);
}

float RigidBody::GetMass()
{
	return 1.0f/inversedMass;
}



