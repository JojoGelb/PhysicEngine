#include "Matrix34.h"
#include "Quaternion.h"
#include "../Vecteur3D.h"
#include <math.h>

Matrix34 Matrix34::operator*(const Matrix34& other) const
{
	return Matrix34(
		values[0] * other.values[0] + values[1] * other.values[4] + values[2] * other.values[8],
		values[0] * other.values[1] + values[1] * other.values[5] + values[2] * other.values[9],
		values[0] * other.values[2] + values[1] * other.values[6] + values[2] * other.values[10],
		values[0] * other.values[3] + values[1] * other.values[7] + values[2] * other.values[11] + values[3],

		values[4] * other.values[0] + values[5] * other.values[4] + values[6] * other.values[8],
		values[4] * other.values[1] + values[5] * other.values[5] + values[6] * other.values[9],
		values[4] * other.values[2] + values[5] * other.values[6] + values[6] * other.values[10],
		values[4] * other.values[3] + values[5] * other.values[7] + values[6] * other.values[11] + values[7],

		values[8] * other.values[0] + values[9] * other.values[4] + values[10] * other.values[8],
		values[8] * other.values[1] + values[9] * other.values[5] + values[10] * other.values[9],
		values[8] * other.values[2] + values[9] * other.values[6] + values[10] * other.values[10],
		values[8] * other.values[3] + values[9] * other.values[7] + values[10] * other.values[11] + values[11]
	);
}

Vector3D Matrix34::operator*(const Vector3D& vector) const
{
	return Vector3D(
		vector.x * values[0] +
		vector.y * values[1] +
		vector.z * values[2] + values[3],

		vector.x * values[4] +
		vector.y * values[5] +
		vector.z * values[6] + values[7],

		vector.x * values[8] +
		vector.y * values[9] +
		vector.z * values[10] + values[11]
	);
}

Matrix34 Matrix34::operator*(const double& other) const
{
	return Matrix34(
		values[0] * other,
		values[1] * other,
		values[2] * other,
		values[3] * other,

		values[4] * other,
		values[5] * other,
		values[6] * other,
		values[7] * other,

		values[8] * other,
		values[9] * other,
		values[10] * other,
		values[11] * other
	);
}

Matrix34 Matrix34::operator+(const Matrix34& other) const
{
	return Matrix34(
	values[0] + other.values[0],
	values[1] + other.values[1],
	values[2] + other.values[2],
	values[3] + other.values[3],
	values[4] + other.values[4],
	values[5] + other.values[5],
	values[6] + other.values[6],
	values[7] + other.values[7],
	values[8] + other.values[8],
	values[9] + other.values[9],
	values[10] + other.values[10],
	values[11] + other.values[11]
	);
}

double Matrix34::GetDeterminant()
{
	return values[8] * values[5] * values[2] +
		values[4] * values[9] * values[2] +
		values[8] * values[1] * values[6] -
		values[0] * values[9] * values[6] -
		values[4] * values[1] * values[10] +
		values[0] * values[5] * values[10];
}

Matrix34 Matrix34::Inverse()
{
	double det = GetDeterminant();
	if (det == 0) return (*this);
	det = ((double)1.0) / det;

	double t0 = values[0];
	double t1 = values[1];
	double t2 = values[2];
	double t3 = values[3];
	double t4 = values[4];
	double t5 = values[5];
	double t6 = values[6];
	double t7 = values[7];
	double t8 = values[8];
	double t9 = values[9];
	double t10 = values[10];
	double t11 = values[11];

	values[0] = (-t9 * t6 + t5 * t10) * det;
	values[4] = (t8 * t6 - t4 * t10) * det;
	values[8] = (-t8 * t5 + t4 * t9 * 1) * det;
	
	values[1] = (t9 * t2 - t1 * t10) * det;
	values[5] = (-t8 * t2 + t0 * t10) * det;
	values[9] = (t8 * t1 - t0 * t9 * 1) * det;

	values[2] = (-t5 * t2 + t1 * t6 * 1) * det;
	values[6] = (t4 * t2 - t0 * t6 * 1) * det;
	values[10] = (-t4 * t1 + t0 * t5 * 1) * det;

	values[3] = (t9 * t6 * t3
		- t5 * t10 * t3
		- t9 * t2 * t7
		+ t1 * t10 * t7
		+ t5 * t2 * t11
		- t1 * t6 * t11) * det;
	values[7] = (-t8 * t6 * t3
		+ t4 * t10 * t3
		+ t8 * t2 * t7
		- t0 * t10 * t7
		- t4 * t2 * t11
		+ t0 * t6 * t11) * det;
	values[11] = (t8 * t5 * t3
		- t4 * t9 * t3
		- t8 * t1 * t7
		+ t0 * t9 * t7
		+ t4 * t1 * t11
		- t0 * t5 * t11) * det;
}

void Matrix34::SetOrientationAndPosition(const Quaternion& q, const Vector3D& p)
{
	// old version by the book
	values[0] = 1 - (2 * q.j() * q.j() + 2 * q.k() * q.k());
	values[1] = 2 * q.i() * q.j() + 2 * q.k() * q.w();
	values[2] = 2 * q.i() * q.k() - 2 * q.j() * q.w();
	values[3] = p.x;
	values[4] = 2 * q.i() * q.j() - 2 * q.k() * q.w();
	values[5] = 1 - (2 * q.i() * q.i() + 2 * q.k() * q.k());
	values[6] = 2 * q.j() * q.k() + 2 * q.i() * q.w();
	values[7] = p.y;
	values[8] = 2 * q.i() * q.k() + 2 * q.j() * q.w();
	values[9] = 2 * q.j() * q.k() - 2 * q.i() * q.w();
	values[10] = 1 - (2 * q.i() * q.i() + 2 * q.j() * q.j());
	values[11] = p.z;

	// other version by the book
	/*values[0] = 1 - (2 * q.j() * q.j() - 2 * q.k() * q.k());
	values[1] = 2 * q.i() * q.j() - 2 * q.k() * q.w();
	values[2] = 2 * q.i() * q.k() + 2 * q.j() * q.w();
	values[3] = p.x;
	values[4] = 2 * q.i() * q.j() + 2 * q.k() * q.w();
	values[5] = 1 - (2 * q.i() * q.i() - 2 * q.k() * q.k());
	values[6] = 2 * q.j() * q.k() - 2 * q.i() * q.w();
	values[7] = p.y;
	values[8] = 2 * q.i() * q.k() - 2 * q.j() * q.w();
	values[9] = 2 * q.j() * q.k() + 2 * q.i() * q.w();
	values[10] = 1 - (2 * q.i() * q.i() - 2 * q.j() * q.j());
	values[11] = p.z;*/
}

Vector3D Matrix34::TransformPosition(const Vector3D& vector) const
{
	return Vector3D(
		vector.x * values[0] +
		vector.y * values[1] +
		vector.z * values[2] +
		values[3],
		vector.x * values[4] +
		vector.y * values[5] +
		vector.z * values[6]+
		values[7],
		vector.x * values[8] +
		vector.y * values[9] +
		vector.z * values[10]+
		values[11]
	);
}

Vector3D Matrix34::transformInverse(const Vector3D &vector) const
{
	Vector3D tmp = vector;
	tmp.x -= values[3];
	tmp.y -= values[7];
	tmp.z -= values[11];
	return Vector3D(
		tmp.x * values[0] +
		tmp.y * values[4] +
		tmp.z * values[8],

		tmp.x * values[1] +
		tmp.y * values[5] +
		tmp.z * values[9],

		tmp.x * values[2] +
		tmp.y * values[6] +
		tmp.z * values[10]
	);
}


Vector3D Matrix34::GetPosition() const
{
	return Vector3D(values[3], values[7], values[11]);
}
