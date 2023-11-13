#pragma once

class Vector3D;
class Quaternion;

class Matrix34 {
private:
	//float values[12];

	float GetDeterminant();
public:
	float values[12];

	Matrix34(
		float a = 0, float b = 0, float c = 0, float x1 = 0,
		float d = 0, float e = 0, float f = 0, float x2 = 0,
		float g = 0, float h = 0, float i = 0, float x3 = 0) :values{ a,b,c,x1,d,e,f,x2,g,h,i,x3 } {};

	Matrix34 operator*(const Matrix34& other)const;

	Vector3D operator*(const Vector3D& vector)const;

	Matrix34 Inverse();

	void SetOrientationAndPosition(const Quaternion& q, const Vector3D& p);

	Vector3D TransformPosition(const Vector3D& vector);

	Vector3D TransformDirection(const Vector3D& vector);


};