#include "Vecteur3D.h"

#include <iostream>
#include<math.h>

Vector3D::Vector3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

Vector3D::Vector3D(const Vector3D& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vector3D::Display() {
	std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
	return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator+=(const Vector3D& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
	return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator-=(const Vector3D& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3D Vector3D::operator*(float scalar) const {
	return Vector3D(x*scalar,y*scalar,z*scalar);
}

Vector3D Vector3D::operator*=(float scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vector3D Vector3D::operator/(float scalar) const{
	if (scalar == 0) {
		throw std::runtime_error("Vector 3D Division by zero error.");
		return Vector3D();
	}
	return Vector3D(x/scalar,y/scalar,z/scalar);
}

Vector3D Vector3D::operator/=(float scalar){
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vector3D& Vector3D::operator=(const Vector3D& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

bool Vector3D::operator==(const Vector3D& other) const {
	return (x == other.x && y == other.y && z == other.z);
}

bool Vector3D::Equal(const Vector3D& other, float epsilon) const {
	return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon && std::abs(z - other.z) < epsilon;
}

float Vector3D::DotProduct(const Vector3D& other) {
	return x* other.x + other.y * y + other.z * z;
}

Vector3D Vector3D::CrossProduct(const Vector3D& other){
	float cx = y * other.z - z * other.y;
	float cy = z * other.x - x * other.z;
	float cz = x * other.y - y * other.x;
	return Vector3D(cx, cy, cz);
}

float Vector3D::GetNorm() const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::Normalize()
{
	if (GetNorm() == 0) {
		throw std::runtime_error("Trying to get the Normalized version of a vector with a norm of 0");
		return Vector3D();
	}
	*this /= GetNorm();
	return *this;
}
