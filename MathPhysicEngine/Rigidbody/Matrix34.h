#pragma once

class Vector3D;
class Quaternion;

class Matrix34 {
private:
	//double values[12];

	double GetDeterminant();
public:
	double values[12];

	Matrix34(
		double a = 0, double b = 0, double c = 0, double x1 = 0,
		double d = 0, double e = 0, double f = 0, double x2 = 0,
		double g = 0, double h = 0, double i = 0, double x3 = 0) :values{ a,b,c,x1,d,e,f,x2,g,h,i,x3 } {};

	Matrix34 operator*(const Matrix34& other)const;

	Vector3D operator*(const Vector3D& vector)const;

	Matrix34 operator*(const double& other)const;

	Matrix34 operator+(const Matrix34& other)const;
	
	Matrix34 Inverse();

	void SetOrientationAndPosition(const Quaternion& q, const Vector3D& p);

	Vector3D TransformPosition(const Vector3D& vector) const;
	Vector3D transformInverse(const Vector3D& vector) const;

	Vector3D TransformDirection(const Vector3D& vector);

	
	Vector3D GetPosition() const;

};