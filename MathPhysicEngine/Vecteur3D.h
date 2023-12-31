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

	Vector3D operator%(const Vector3D& other) const;

	bool operator==(const Vector3D& other) const;

	//Hash function
	size_t operator()(const Vector3D& v) const {
		return std::hash<double>()(v.x) ^ std::hash<double>()(v.y) ^ std::hash<double>()(v.z);
	}

	void Display();

	bool Equal(const Vector3D& other, double epsilon = 1e-3) const;

	double DotProduct(const Vector3D& other) const;

	Vector3D CrossProduct(const Vector3D& other);

	//return magnitude/length of a vector
	double GetNorm() const;
	double GetSquareNorm() const;
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


	void SetAtIndex(unsigned index, double value) {
		switch (index)
		{
		case 0:
			x = value;
			break;
		case 1:
			y = value;
			break;
		case 2:
			z = value;
			break;
		default:
			break;
		}
	}


	double GetAtIndex(unsigned index) const {
		switch (index)
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			return 0.0f;
			break;
		}
	}

};

