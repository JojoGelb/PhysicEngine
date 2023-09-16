#include "Vecteur3D.h"

#include <iostream>

Vecteur3D::Vecteur3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

Vecteur3D::Vecteur3D(const Vecteur3D& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vecteur3D::Display() {
	std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;
}

Vecteur3D Vecteur3D::operator+(const Vecteur3D& other) const {
	return Vecteur3D(x + other.x, y + other.y, z + other.z);
}

Vecteur3D Vecteur3D::operator+=(const Vecteur3D& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vecteur3D Vecteur3D::operator-(const Vecteur3D& other) const {
	return Vecteur3D(x - other.x, y - other.y, z - other.z);
}

Vecteur3D Vecteur3D::operator-=(const Vecteur3D& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vecteur3D Vecteur3D::operator*(float scalar) const {
	return Vecteur3D(x*scalar,y*scalar,z*scalar);
}

Vecteur3D Vecteur3D::operator*=(float scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vecteur3D Vecteur3D::operator/(float scalar) const{
	if (scalar == 0) {
		throw std::runtime_error("Vector 3D Division by zero error.");
		return Vecteur3D();
	}
	return Vecteur3D(x/scalar,y/scalar,z/scalar);
}

Vecteur3D Vecteur3D::operator/=(float scalar){
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vecteur3D& Vecteur3D::operator=(const Vecteur3D& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

bool Vecteur3D::operator==(const Vecteur3D& other) const {
	return (x == other.x && y == other.y && z == other.z);
}

bool Vecteur3D::Equal(const Vecteur3D& other, float epsilon) const {
	return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon && std::abs(z - other.z) < epsilon;
}

float Vecteur3D::DotProduct(const Vecteur3D& other) {
	return x* other.x + other.y * y + other.z * z;
}

Vecteur3D Vecteur3D::CrossProduct(const Vecteur3D& other){
	float cx = y * other.z - z * other.y;
	float cy = z * other.x - x * other.z;
	float cz = x * other.y - y * other.x;
	return Vecteur3D(cx, cy, cz);
}
