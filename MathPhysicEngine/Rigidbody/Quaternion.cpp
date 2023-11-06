#include "Quaternion.h"
#include "../Vecteur3D.h"
#include <math.h>

void Quaternion::Normalized()
{
	float mag = 0;
	for (int i = 0; i < 4; i++) {
		mag += value[i] * value[i];
	}
	
	//No rotation case
	if (mag == 0) {
		value[0] = 1;
		return;
	}

	mag = (1.0f) / sqrtf(mag);

	for (int i = 0; i < 4; i++) {
		value[i] *= mag;
	}
}

Quaternion Quaternion::operator*(const Quaternion& other)
{
	Quaternion q;
	q.value[0] = this->value[0] * other.value[0] - 
				this->value[1] * other.value[1] -
				this->value[2] * other.value[2] - this->value[3] * other.value[3];
	q.value[1] = this->value[0] * other.value[1] + this->value[1] * other.value[0] +
		this->value[2] * other.value[3] - this->value[3] * other.value[2];

	q.value[2] = this->value[0] * other.value[2] + this->value[2] * other.value[0] +
		this->value[3] * other.value[1] - this->value[1] * other.value[3];
	q.value[3] = this->value[0] * other.value[3] + this->value[3] * other.value[0] +
		this->value[1] * other.value[2] - this->value[2] * other.value[1];

	return q;
}

void Quaternion::operator*=(const Quaternion& other) {
	this->value[0] = 
		this->value[0] * other.value[0] -
		this->value[1] * other.value[1] -
		this->value[2] * other.value[2] - 
		this->value[3] * other.value[3];

	this->value[1] = 
		this->value[0] * other.value[1] + 
		this->value[1] * other.value[0] +
		this->value[2] * other.value[3] - 
		this->value[3] * other.value[2];

	this->value[2] = 
		this->value[0] * other.value[2] + 
		this->value[2] * other.value[0] +
		this->value[3] * other.value[1] - 
		this->value[1] * other.value[3];

	this->value[3] = 
		this->value[0] * other.value[3] + 
		this->value[3] * other.value[0] +
		this->value[1] * other.value[2] - 
		this->value[2] * other.value[1];
}

void Quaternion::RotateByVector(const Vector3D& vector)
{
	Quaternion q(0, vector.x, vector.y,vector.z);
	(*this) = *this * q;

}

void Quaternion::UpdateByAngularVelocity(const Vector3D& rotation, float duration)
{
	Quaternion q(0,
		rotation.x * duration,
		rotation.y * duration,
		rotation.z * duration);

	q *= *this;

	value[0] += q.value[0] * (0.5f);
	value[1] += q.value[1] * (0.5f);
	value[2] += q.value[2] * (0.5f);
	value[3] += q.value[3] * (0.5f);
}
