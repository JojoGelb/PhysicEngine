#pragma once
#include "../Vecteur3D.h"

class Quaternion
{
private:

	//w, i,j,k
	float value[4];

public:

	Quaternion(float w = 0, float i = 0, float j = 0, float k = 0) :value{w,i,j,k} {};

	// normalize by multipling the quaternion by the inverse of its magnitude
	void Normalized();

	//Quaternion multiplication
	Quaternion operator*(const Quaternion& other);
	void operator*=(const Quaternion& other);

	// Rotate the quaternion by a vector - multiply this by q = (0, dx, dy, dz)
	void RotateByVector(const Vector3D& vector);

	// Apply the quaternion update by the angular velocity
	void UpdateByAngularVelocity(const Vector3D& rotation, float duration);
};