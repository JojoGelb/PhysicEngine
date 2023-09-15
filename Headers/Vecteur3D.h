#pragma once
#include <ostream>
class Vecteur3D
{

public:

	Vecteur3D(float x_ = 0, float y_ = 0, float z_ = 0);

	Vecteur3D(const Vecteur3D& v);

	void Display();

	Vecteur3D operator+(const Vecteur3D& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Vecteur3D& v) {
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	//Source : www.exelib.net
private:
	float x;
	float y;
	float z;
};

