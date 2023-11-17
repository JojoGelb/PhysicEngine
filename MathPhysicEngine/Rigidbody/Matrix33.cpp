#include "Matrix33.h"

#include "../Vecteur3D.h"
#include "Quaternion.h"

Matrix33 Matrix33::operator*(const Matrix33& other) const
{
	return Matrix33(
		values[0] * other.values[0] + values[1] * other.values[3] + values[2] * other.values[6],
		values[0] * other.values[1] + values[1] * other.values[4] + values[2] * other.values[7],
		values[0] * other.values[2] + values[1] * other.values[5] + values[2] * other.values[8],
		values[3] * other.values[0] + values[4] * other.values[3] + values[5] * other.values[6],
		values[3] * other.values[1] + values[4] * other.values[4] + values[5] * other.values[7],
		values[3] * other.values[2] + values[4] * other.values[5] + values[5] * other.values[8],
		values[6] * other.values[0] + values[7] * other.values[3] + values[8] * other.values[6],
		values[6] * other.values[1] + values[7] * other.values[4] + values[8] * other.values[7],
		values[6] * other.values[2] + values[7] * other.values[5] + values[8] * other.values[8]
	);
}

Vector3D Matrix33::operator*(const Vector3D& vector) const
{
	return Vector3D(
		vector.x * values[0] +
		vector.y * values[1] +
		vector.z * values[2],

		vector.x * values[3] +
		vector.y * values[4] +
		vector.z * values[5],

		vector.x * values[6] +
		vector.y * values[7] +
		vector.z * values[8]
	);
}

Matrix33 Matrix33::Inverse()
{
	double t4 = values[0] * values[4];
	double t6 = values[0] * values[5];
	double t8 = values[1] * values[3];
	double t10 = values[2] * values[3];
	double t12 = values[1] * values[6];
	double t14 = values[2] * values[6];
	// Calculate the determinant.
	double t16 = (t4 * values[8] - t6 * values[7] - t8 * values[8] +
		t10 * values[7] + t12 * values[5] - t14 * values[4]);
	// Make sure the determinant is non-zero.
	if (t16 == (double)0.0f) return (*this);
	double t17 = 1 / t16;

	Matrix33 m;

	m.values[0] = (values[4] * values[8] - values[5] * values[7]) * t17;
	m.values[1] = -(values[1] * values[8] - values[2] * values[7]) * t17;
	m.values[2] = (values[1] * values[5] - values[2] * values[4]) * t17;
	m.values[3] = -(values[3] * values[8] - values[5] * values[6]) * t17;
	m.values[4] = (values[0] * values[8] - t14) * t17;
	m.values[5] = -(t6 - t10) * t17;
	m.values[6] = (values[3] * values[7] - values[4] * values[6]) * t17;
	m.values[7] = -(values[0] * values[7] - t12) * t17;
	m.values[8] = (t4 - t8) * t17;

	return m;
}

Matrix33 Matrix33::Transpose()
{
	double t = values[1];
	values[1] = values[3];
	values[3] = t;

	t = values[2];
	values[2] = values[6];
	values[6] = t;

	t = values[5];
	values[5] = values[7];
	values[7] = t;

	return (*this);
}

void Matrix33::SetDiagonal(double x, double y, double z)
{
	values[0] = x;
	values[4] = y;
	values[8] = z;
}

void Matrix33::SetOrientation(const Quaternion& q)
{
	values[0] = 1 - (2 * q.j() * q.j() + 2 * q.k() * q.k());
	values[1] = 2 * q.i() * q.j() + 2 * q.k() * q.w();
	values[2] = 2 * q.i() * q.k() - 2 * q.j() * q.w();
	values[3] = 2 * q.i() * q.j() - 2 * q.k() * q.w();
	values[4] = 1 - (2 * q.i() * q.i() + 2 * q.k() * q.k());
	values[5] = 2 * q.j() * q.k() + 2 * q.i() * q.w();
	values[6] = 2 * q.i() * q.k() + 2 * q.j() * q.w();
	values[7] = 2 * q.j() * q.k() - 2 * q.i() * q.w();
	values[8] = 1 - (2 * q.i() * q.i() + 2 * q.j() * q.j());
}
