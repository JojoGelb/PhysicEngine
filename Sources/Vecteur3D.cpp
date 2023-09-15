#include "Vecteur3D.h"

#include <iostream>

Vecteur3D::Vecteur3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

Vecteur3D::Vecteur3D(const Vecteur3D& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vecteur3D::Display()
{
	std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;
}

Vecteur3D Vecteur3D::operator+(const Vecteur3D& other) const {
	return Vecteur3D(x + other.x, y + other.y, z + other.z);
}

Vecteur3D Vecteur3D::operator*(float scalar) const
{
	return Vecteur3D(x*scalar,y*scalar,z*scalar);
}

bool Vecteur3D::operator==(const Vecteur3D& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}
