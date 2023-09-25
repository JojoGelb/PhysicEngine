#pragma once
#include <ostream>
class Vector3D
{

public:

	Vector3D(float x_ = 0, float y_ = 0, float z_ = 0);
	Vector3D(const Vector3D& v);

	Vector3D operator+(const Vector3D& other) const;
	Vector3D operator+=(const Vector3D& other);
	Vector3D operator-(const Vector3D& other) const;
	Vector3D operator-=(const Vector3D& other);
	Vector3D operator*(float scalar) const;
	Vector3D operator*=(float scalar);
	Vector3D operator/(float scalar) const;
	Vector3D operator/=(float scalar);
	Vector3D& operator=(const Vector3D& other);

	bool operator==(const Vector3D& other) const;

	void Display();

	bool Equal(const Vector3D& other, float epsilon = 1e-3) const;

	float DotProduct(const Vector3D& other);

	Vector3D CrossProduct(const Vector3D& other);

	float GetNorm() const; 

	Vector3D Normalize();



	friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	float x;
	float y;
	float z;
};

