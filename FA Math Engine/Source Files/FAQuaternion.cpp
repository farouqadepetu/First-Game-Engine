#include "FAQuaternion.h"
#include "FAVector3D.h"
#include "FAVector4D.h"

namespace FAMath
{
	//-------------------------------------------------------------------------------------
	Quaternion::Quaternion() : m_scalar{ 1.0f }, m_x{ 0.0f }, m_y{ 0.0f }, m_z{ 0.0f }
	{
	}

	Quaternion::Quaternion(float scalar, float x, float y, float z) : m_scalar{ scalar }, m_x{ x }, m_y{ y }, m_z{ z }
	{
	}

	Quaternion::Quaternion(float scalar, const Vector3D& v) : m_scalar{ scalar }, m_x{ v.x() }, m_y{ v.y() }, m_z{ v.z() }
	{
	}

	Quaternion::Quaternion(const Vector4D& v) : m_scalar{ v.x() }, m_x{ v.y() }, m_y{ v.z() }, m_z{ v.w() }
	{
	}

	float& Quaternion::scalar()
	{
		return m_scalar;
	}

	const float& Quaternion::scalar() const
	{
		return m_scalar;
	}

	float& Quaternion::x()
	{
		return m_x;
	}

	const float& Quaternion::x() const
	{
		return m_x;
	}

	float& Quaternion::y()
	{
		return m_y;
	}

	const float& Quaternion::y() const
	{
		return m_y;
	}

	float& Quaternion::z()
	{
		return m_z;
	}

	const float& Quaternion::z() const
	{
		return m_z;
	}

	Vector3D Quaternion::vector()
	{
		return Vector3D(m_x, m_y, m_z);
	}

	Quaternion& Quaternion::operator+=(const Quaternion& q)
	{
		this->m_scalar += (double)q.m_scalar;
		this->m_x += (double)q.m_x;
		this->m_y += (double)q.m_y;
		this->m_z += (double)q.m_z;

		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& q)
	{
		this->m_scalar -= (double)q.m_scalar;
		this->m_x -= (double)q.m_x;
		this->m_y -= (double)q.m_y;
		this->m_z -= (double)q.m_z;

		return *this;
	}

	Quaternion& Quaternion::operator*=(float k)
	{
		this->m_scalar *= (double)k;
		this->m_x *= (double)k;
		this->m_y *= (double)k;
		this->m_z *= (double)k;

		return *this;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q)
	{
		Vector3D thisVector(this->m_x, this->m_y, this->m_z);
		Vector3D qVector(q.m_x, q.m_y, q.m_z);

		double s{ (double)this->m_scalar * q.m_scalar };
		double dP{ dotProduct(thisVector, qVector) };
		double resultScalar{ s - dP };

		Vector3D a(this->m_scalar * qVector);
		Vector3D b(q.m_scalar * thisVector);
		Vector3D cP(crossProduct(thisVector, qVector));
		Vector3D resultVector(a + b + cP);

		this->m_scalar = resultScalar;
		this->m_x = resultVector.x();
		this->m_y = resultVector.y();
		this->m_z = resultVector.z();

		return *this;
	}

	Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion((double)q1.scalar() + q2.scalar(), (double)q1.x() + q2.x(), (double)q1.y() + q2.y(), (double)q1.z() + q2.z());
	}

	Quaternion operator-(const Quaternion& q)
	{
		return Quaternion(-q.scalar(), -q.x(), -q.y(), -q.z());
	}

	Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion((double)q1.scalar() - q2.scalar(), (double)q1.x() - q2.x(), (double)q1.y() - q2.y(), (double)q1.z() - q2.z());
	}

	Quaternion operator*(float k, const Quaternion& q)
	{
		return Quaternion((double)k * q.scalar(), (double)k * q.x(), (double)k * q.y(), (double)k * q.z());
	}

	Quaternion operator*(const Quaternion& q, float k)
	{
		return Quaternion(q.scalar() * (double)k, q.x() * (double)k, q.y() * (double)k, q.z() * (double)k);
	}

	//scalar part = q1scalar * q2scalar - q1Vector dot q2Vector
	//vector part = q1Scalar * q2Vector + q2Scalar * q1Vector + q1Vector cross q2Vector
	Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
	{
		Vector3D q1Vector(q1.x(), q1.y(), q1.z());
		Vector3D q2Vector(q2.x(), q2.y(), q2.z());

		double s{ (double)q1.scalar() * q2.scalar() };
		double dP{ dotProduct(q1Vector, q2Vector) };
		double resultScalar{ s - dP };

		Vector3D a(q1.scalar() * q2Vector);
		Vector3D b(q2.scalar() * q1Vector);
		Vector3D cP(crossProduct(q1Vector, q2Vector));
		Vector3D resultVector(a + b + cP);

		return Quaternion(resultScalar, resultVector);
	}

	//zero quaternion = (0, 0, 0, 0)
	bool isZeroQuaternion(const Quaternion& q)
	{
		return compareFloats(q.scalar(), 0.0f, EPSILON) && compareFloats(q.x(), 0.0f, EPSILON) &&
			compareFloats(q.y(), 0.0f, EPSILON) && compareFloats(q.z(), 0.0f, EPSILON);
	}

	//identity quaternion = (1, 0, 0, 0)
	bool isIdentity(const Quaternion& q)
	{
		return compareFloats(q.scalar(), 1.0f, EPSILON) && compareFloats(q.x(), 0.0f, EPSILON) &&
			compareFloats(q.y(), 0.0f, EPSILON) && compareFloats(q.z(), 0.0f, EPSILON);
	}

	//conjugate of a quaternion is the quaternion with its vector part negated
	Quaternion conjugate(const Quaternion& q)
	{
		return Quaternion(q.scalar(), -q.x(), -q.y(), -q.z());
	}

	//length of a quaternion = sqrt(scalar^2 + x^2 + y^2 + z^2)
	float length(const Quaternion& q)
	{
		return sqrt((double)q.scalar() * q.scalar() + (double)q.x() * q.x() + (double)q.y() * q.y() + (double)q.z() * q.z());
	}

	//to normalize a quaternion you do q / |q|
	Quaternion normalize(const Quaternion& q)
	{
		if (isZeroQuaternion(q))
			return q;

		double d{ length(q) };

		return Quaternion(q.scalar() / d, q.x() / d, q.y() / d, q.z() / d);
	}

	//inverse = conjugate of q / |q|^2
	Quaternion inverse(const Quaternion& q)
	{
		if (isZeroQuaternion(q))
			return q;

		Quaternion conjugateOfQ(conjugate(q));

		double d{ length(q) };
		d *= d;

		return Quaternion(conjugateOfQ.scalar() / d, conjugateOfQ.x() / d, conjugateOfQ.y() / d, conjugateOfQ.z() / d);
	}

	//A roatation quaternion is a quaternion where the
	//scalar part = cos(theta / 2)
	//vector part = sin(theta / 2) * axis
	//the axis needs to be normalized
	Quaternion rotationQuaternion(float angle, float x, float y, float z)
	{
		double ang{ angle / 2.0 };
		double c{ cos(ang * PI / 180.0) };
		double s{ sin(ang * PI / 180.0) };

		Vector3D axis(x, y, z);
		axis = norm(axis);

		return Quaternion(c, s * axis.x(), s * axis.y(), s * axis.z());
	}

	//A roatation quaternion is a quaternion where the
	//scalar part = cos(theta / 2)
	//vector part = sin(theta / 2) * axis
	//the axis needs to be normalized
	Quaternion rotationQuaternion(float angle, const Vector3D& axis)
	{
		double ang{ angle / 2.0 };
		double c{ cos(ang * PI / 180.0) };
		double s{ sin(ang * PI / 180.0) };

		Vector3D axisN(norm(axis));

		return Quaternion(c, s * axisN.x(), s * axisN.y(), s * axisN.z());
	}

	//A roatation quaternion is a quaternion where the
	//scalar part = cos(theta / 2)
	//vector part = sin(theta / 2) * axis
	//the axis needs to be normalized
	Quaternion rotationQuaternion(const Vector4D& angAxis)
	{
		double angle{ angAxis.x() / 2.0 };
		double c{ cos(angle * PI / 180.0) };
		double s{ sin(angle * PI / 180.0) };

		Vector3D axis(angAxis.y(), angAxis.z(), angAxis.w());
		axis = norm(axis);

		return Quaternion(c, s * axis.x(), s * axis.y(), s * axis.z());
	}

	//1 - 2q3^2 - 2q4^2		2q2q3 - 2q1q4		2q2q4 + 2q1q3		0
	//2q2q3 + 2q1q4			1 - 2q2^2 - 2q4^2	2q3q4 - 2q1q2		0
	//2q2q4 - 2q1q3			2q3q4 + 2q1q2		1 - 2q2^2 - 2q3^2	0
	//0						0					0					1
	//q1 = scalar
	//q2 = x
	//q3 = y
	//q4 = z
	Matrix4x4 quaternionRotationMatrixCol(const Quaternion& q)
	{
		float colMat[4][4] = {};

		colMat[0][0] = 1.0 - 2.0 * q.y() * q.y() - 2.0 * q.z() * q.z();
		colMat[0][1] = 2.0 * q.x() * q.y() - 2.0 * q.scalar() * q.z();
		colMat[0][2] = 2.0 * q.x() * q.z() + 2.0 * q.scalar() * q.y();
		colMat[0][3] = 0.0f;

		colMat[1][0] = 2.0 * q.x() * q.y() + 2.0 * q.scalar() * q.z();
		colMat[1][1] = 1.0 - 2.0 * q.x() * q.x() - 2.0 * q.z() * q.z();
		colMat[1][2] = 2.0 * q.y() * q.z() - 2.0 * q.scalar() * q.x();
		colMat[1][3] = 0.0f;

		colMat[2][0] = 2.0 * q.x() * q.z() - 2.0 * q.scalar() * q.y();
		colMat[2][1] = 2.0 * q.y() * q.z() + 2.0 * q.scalar() * q.x();
		colMat[2][2] = 1.0 - 2.0 * q.x() * q.x() - 2.0 * q.y() * q.y();
		colMat[2][3] = 0.0f;

		colMat[3][0] = 0.0f;
		colMat[3][1] = 0.0f;
		colMat[3][2] = 0.0f;
		colMat[3][3] = 1.0f;

		return Matrix4x4(colMat);
	}


	//1 - 2q3^2 - 2q4^2		2q2q3 + 2q1q4		2q2q4 - 2q1q3		0
	//2q2q3 - 2q1q4			1 - 2q2^2 - 2q4^2	2q3q4 + 2q1q2		0
	//2q2q4 + 2q1q3			2q3q4 - 2q1q2		1 - 2q2^2 - 2q3^2	0
	//0						0					0					1
	//q1 = scalar
	//q2 = x
	//q3 = y
	//q4 = z
	Matrix4x4 quaternionRotationMatrixRow(const Quaternion& q)
	{
		float rowMat[4][4] = {};

		rowMat[0][0] = 1.0 - 2.0 * q.y() * q.y() - 2.0 * q.z() * q.z();
		rowMat[0][1] = 2.0 * q.x() * q.y() + 2.0 * q.scalar() * q.z();
		rowMat[0][2] = 2.0 * q.x() * q.z() - 2.0 * q.scalar() * q.y();
		rowMat[0][3] = 0.0f;

		rowMat[1][0] = 2.0 * q.x() * q.y() - 2.0 * q.scalar() * q.z();
		rowMat[1][1] = 1.0 - 2.0 * q.x() * q.x() - 2.0 * q.z() * q.z();
		rowMat[1][2] = 2.0 * q.y() * q.z() + 2.0 * q.scalar() * q.x();
		rowMat[1][3] = 0.0f;

		rowMat[2][0] = 2.0 * q.x() * q.z() + 2.0 * q.scalar() * q.y();
		rowMat[2][1] = 2.0 * q.y() * q.z() - 2.0 * q.scalar() * q.x();
		rowMat[2][2] = 1.0 - 2.0 * q.x() * q.x() - 2.0 * q.y() * q.y();
		rowMat[2][3] = 0.0f;

		rowMat[3][0] = 0.0f;
		rowMat[3][1] = 0.0f;
		rowMat[3][2] = 0.0f;
		rowMat[3][3] = 1.0f;

		return Matrix4x4(rowMat);
	}

#if defined(_DEBUG)
	void print(const Quaternion& q)
	{
		std::cout << "(" << q.scalar() << ", " << q.x() << ", " << q.y() << ", " << q.z();
	}
#endif
	//-------------------------------------------------------------------------------------
}