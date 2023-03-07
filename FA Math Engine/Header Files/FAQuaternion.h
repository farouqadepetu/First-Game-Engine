#pragma once

#include "FAMathUtility.h"
#include "FAMatrix4x4.h"

/** @file FAMatrix4x4.h
*	@brief File has a 4x4 matrix class under the namespace FAMath.
*/


/** @namespace FAMath
*	@brief The name space has utility functions, Vector2D, Vector3D, Vector4D, Matrix4x4, and Quaternion classes.
*/
namespace FAMath
{
	/** @class Quaternion ""
	*
	*	The datatype for the components is float.\n
	*
	*/
	class Quaternion
	{
	public:

		/**@brief Default Constructor.
		*
		*	Constructs an identity quaternion.
		*/
		Quaternion();

		/**@brief Overloaded Constructor.
		*
		*	Constructs a quaternion with the given values.
		*/
		Quaternion(float scalar, float x, float y, float z);

		/**@brief Overloaded Constructor.
		*
		*	Constructs a quaternion with the given values.
		*/
		Quaternion(float scalar, const Vector3D& v);

		/**@brief Overloaded Constructor.
		*
		*	Constructs a quaternion with the given values in the 4D vector.\n
		*	The x value in the 4D vector should be the scalar.
		*	The y, z and w value in the 4D vector should be the axis.
		*/
		Quaternion(const Vector4D& v);

		/**@brief Returns a reference to the scalar component of the quaternion.
		*/
		float& scalar();

		/**@brief Returns a const reference to the scalar component of the quaternion.
		*/
		const float& scalar() const;

		/**@brief Returns a reference to the x value of the vector component in the quaternion.
		*/
		float& x();

		/**@brief Returns a const reference to the x value of the vector component in the quaternion.
		*/
		const float& x() const;

		/**@brief Returns a reference to the y value of the vector component in the quaternion.
		*/
		float& y();

		/**@brief Returns a const reference to the y value of the vector component in the quaternion.
		*/
		const float& y() const;

		/**@brief Returns a reference to the z value of the vector component in the quaternion.
		*/
		float& z();

		/**@brief Returns a const reference to the z value of the vector component in the quaternion.
		*/
		const float& z() const;

		/**@brief Returns the vector component of the quaternion.
		*/
		Vector3D vector();

		/**@brief Adds this quaternion to quaterion q and stores the result in this quaternion.
		*/
		Quaternion& operator+=(const Quaternion& q);

		/**@brief Subtracts this quaternion by quaterion q and stores the result in this quaternion.
		*/
		Quaternion& operator-=(const Quaternion& q);

		/**@brief Multiplies this quaternion by flaot k and stores the result in this quaternion.
		*/
		Quaternion& operator*=(float k);

		/**@brief Multiplies this quaternion by quaterion q and stores the result in this quaternion.
		*/
		Quaternion& operator*=(const Quaternion& q);


	private:

		float m_scalar;
		float m_x;
		float m_y;
		float m_z;
	};

	/**@brief Returns a quaternion that has the result of q1 + q2.
	*/
	Quaternion operator+(const Quaternion& q1, const Quaternion& q2);

	/**@brief Returns a quaternion that has the result of -q.
	*/
	Quaternion operator-(const Quaternion& q);

	/**@brief Returns a quaternion that has the result of q1 - q2.
	*/
	Quaternion operator-(const Quaternion& q1, const Quaternion& q2);

	/**@brief Returns a quaternion that has the result of k * q.
	*/
	Quaternion operator*(float k, const Quaternion& q);

	/**@brief Returns a quaternion that has the result of q * k.
	*/
	Quaternion operator*(const Quaternion& q, float k);

	/**@brief Returns a quaternion that has the result of q1 * q2.
	*/
	Quaternion operator*(const Quaternion& q1, const Quaternion& q2);


	/**@brief Returns true if quaternion q is a zero quaternion, false otherwise.
	*/
	bool isZeroQuaternion(const Quaternion& q);

	/**@brief Returns true if quaternion q is an identity quaternion, false otherwise.
	*/
	bool isIdentity(const Quaternion& q);

	/**@brief Returns the conjugate of quaternion q.
	*/
	Quaternion conjugate(const Quaternion& q);

	/**@brief Returns the length of quaternion q.
	*/
	float length(const Quaternion& q);

	/**@brief Normalizes quaternion q and returns the normalized quaternion.
	* If q is the zero quaternion then q is returned.
	*/
	Quaternion normalize(const Quaternion& q);

	/**@brief Returns the invese of quaternion q.
	*  If q is the zero quaternion then q is returned.
	*/
	Quaternion inverse(const Quaternion& q);

	/**@brief Returns a quaternion from the axis-angle rotation representation.
	*  The angle should be given in degrees.
	*/
	Quaternion rotationQuaternion(float angle, float x, float y, float z);

	/**@brief Returns a quaternion from the axis-angle rotation representation.
	* The angle should be given in degrees.
	*/
	Quaternion rotationQuaternion(float angle, const Vector3D& axis);

	/**@brief Returns a quaternion from the axis-angle rotation representation.
	* The x value in the 4D vector should be the angle(in degrees).\n
	* The y, z and w value in the 4D vector should be the axis.
	*/
	Quaternion rotationQuaternion(const Vector4D& angAxis);

	/**@brief Returns a matrix from the given quaterion for column vector-matrix multiplication.
	* Quaternion q should be a unit quaternion.
	*/
	Matrix4x4 quaternionRotationMatrixCol(const Quaternion& q);

	/**@brief Returns a matrix from the given quaterion for row vector-matrix multiplication.
	* Quaternion q should be a unit quaternion.
	*/
	Matrix4x4 quaternionRotationMatrixRow(const Quaternion& q);

#if defined(_DEBUG)
	void print(const Quaternion& q);
#endif

}
