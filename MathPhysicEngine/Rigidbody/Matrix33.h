#pragma once

class Quaternion;
class Vector3D;

class Matrix33 {
private:
	double values[9];

public:

	Matrix33(
		double a = 0, double b = 0, double c = 0, 
		double d = 0, double e = 0, double f = 0, 
		double g = 0, double h = 0, double i = 0) :values{ a,b,c,d,e,f,g,h,i } {};

	Matrix33 operator*(const Matrix33& other)const;

	Vector3D operator*(const Vector3D& vector)const;

	Matrix33 Inverse();

	Matrix33 Transpose();

	void SetDiagonal(double x, double y, double z);

	void SetOrientation(const Quaternion& q);

	double A() { return values[0]; };
	double B() { return values[1]; };
	double C() { return values[2]; };
	double D() { return values[3]; };
	double E() { return values[4]; };
	double F() { return values[5]; };
	double G() { return values[6]; };
	double H() { return values[7]; };
	double I() { return values[8]; };





};