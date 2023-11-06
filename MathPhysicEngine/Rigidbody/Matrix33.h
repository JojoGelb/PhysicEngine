#pragma once

class Quaternion;
class Vector3D;

class Matrix33 {
private:
	float values[9];

public:

	Matrix33(
		float a = 0, float b = 0, float c = 0, 
		float d = 0, float e = 0, float f = 0, 
		float g = 0, float h = 0, float i = 0) :values{ a,b,c,d,e,f,g,h,i } {};

	Matrix33 operator*(const Matrix33& other)const;

	Vector3D operator*(const Vector3D& vector)const;

	Matrix33 Inverse();

	Matrix33 Transpose();

	void SetOrientation(const Quaternion& q);




};