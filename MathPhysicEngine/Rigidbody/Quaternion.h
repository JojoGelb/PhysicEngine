#pragma once

class Vector3D;

class Quaternion
{
private:

	//w, i,j,k
	double value[4];

public:

	Quaternion(double w = 0, double i = 0, double j = 0, double k = 0) :value{w,i,j,k} {};

	// normalize by multipling the quaternion by the inverse of its magnitude
	void Normalized();

	//Quaternion multiplication
	Quaternion operator*(const Quaternion& other);
	void operator*=(const Quaternion& other);

	// Rotate the quaternion by a vector - multiply this by q = (0, dx, dy, dz)
	void RotateByVector(const Vector3D& vector);

	Quaternion Inverse(const Quaternion& q) {
		Quaternion qInverse = q;
		qInverse.value[1] *= -1;
		qInverse.value[2] *= -1;
		qInverse.value[3] *= -1;
		return qInverse;
	
	}

	// Apply the quaternion update by the angular velocity
	void UpdateByAngularVelocity(const Vector3D& rotation, double duration);

	double w() const { return value[0]; }
	double i() const { return value[1]; }
	double j() const { return value[2]; }
	double k() const { return value[3]; }
};