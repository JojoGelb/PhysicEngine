#pragma once

#include <ostream>
class Vector3D
{

public:

	Vector3D(double x_ = 0, double y_ = 0, double z_ = 0);
	Vector3D(const Vector3D& v);

	Vector3D operator+(const Vector3D& other) const;
	Vector3D operator+=(const Vector3D& other);
	Vector3D operator-(const Vector3D& other) const;
	Vector3D operator-=(const Vector3D& other);
	Vector3D operator*(double scalar) const;
	Vector3D operator*=(double scalar);
	Vector3D operator/(double scalar) const;
	Vector3D operator/=(double scalar);
	Vector3D& operator=(const Vector3D& other);

	bool operator==(const Vector3D& other) const;

	void Display();

	bool Equal(const Vector3D& other, double epsilon = 1e-3) const;

	double DotProduct(const Vector3D& other);

	Vector3D CrossProduct(const Vector3D& other);

	//return magnitude/length of a vector
	double GetNorm() const; 

	Vector3D Normalize();

	double DistanceTo(const Vector3D& other);


	friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	double x;
	double y;
	double z;

	friend Vector3D operator*(double scalar, const Vector3D& vector);

};

