#pragma once

#include "FAMathUtility.h"

/** @file FAVector3D.h
*	@brief File has a 3D Vector class under the namespace FAMath.
*/


/** @namespace FAMath
*	@brief The name space has utility functions, Vector2D, Vector3D, Vector4D, Matrix4x4, and Quaternion classes.
*/

namespace FAMath
{
	/** @class Vector3D ""
		*	@brief A vector class used for 3D vectors/points and their manipulations.
		*
		*	The datatype for the components is float
		*/
	class Vector3D
	{
	public:


		/**@brief Default Constructor.
		*
		*	Creates a new 3D vector/point with the components initialized to 0.0.
		*/
		Vector3D();

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 3D vector/point with the components initialized to the arguments.
		*/
		Vector3D(float x, float y, float z);

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 3D vector/point with the components initialized to the arguments.
		*/
		Vector3D(Vector2D v, float z = 0.0f);

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 3D vector/point with the components initialized to the arguments.
		*/
		Vector3D(Vector4D v);

		/**@brief Returns a reference to the x component.
		*/
		float& x();

		/**@brief Returns a reference to the y component.
		*/
		float& y();

		/**@brief Returns a reference to the z component.
		*/
		float& z();

		/**@brief Returns a constant reference to the x component.
		*/
		const float& x() const;

		/**@brief Returns a constant reference to the y component.
		*/
		const float& y() const;

		/**@brief Returns a constant reference to the z component.
		*/
		const float& z() const;

		/**@brief 3D vector addition through overloading operator +=.
		*/
		Vector3D& operator+=(const Vector3D& b);

		/**@brief 3D vector subtraction through overloading operator -=.
		*/
		Vector3D& operator-=(const Vector3D& b);

		/**@brief 3D vector scalar multiplication through overloading operator *=.
		*/
		Vector3D& operator*=(const float& k);

		/**@brief 3D vector scalar division through overloading operator /=.
		*
		* If k is zero, the vector is unchanged.
		*/
		Vector3D& operator/=(const float& k);

	private:
		float m_x;
		float m_y;
		float m_z;
	};

	/**@brief Returns true if a is the zero vector.
	*/
	bool zeroVector(const Vector3D& a);

	/**@brief 3D vector addition.
	*/
	Vector3D operator+(const Vector3D& a, const Vector3D& b);

	/**@brief 3D vector negeation.
	*/
	Vector3D operator-(const Vector3D& v);

	/**@brief 3D vector subtraction.
	*/
	Vector3D operator-(const Vector3D& a, const Vector3D& b);

	/**@brief 3D vector scalar multiplication.
	* Returns a * k, where a is a vector and k is a scalar(float)
	*/
	Vector3D operator*(const Vector3D& a, const float& k);

	/**@brief 3D vector scalar multiplication.
	* Returns k * a,  where a is a vector and k is a scalar(float)
	*/
	Vector3D operator*(const float& k, const Vector3D& a);

	/**@brief 3D vector scalar division.
	* Returns a / k,  where a is a vector and k is a scalar(float)
	* If k = 0 the returned vector is the zero vector.
	*/
	Vector3D operator/(const Vector3D& a, const float& k);

	/**@brief Returns the dot product between two 3D vectors.
	*/
	float dotProduct(const Vector3D& a, const Vector3D& b);

	/**@brief Returns the cross product between two 3D vectors.
	*/
	Vector3D crossProduct(const Vector3D& a, const Vector3D& b);

	/**@brief Returns the length(magnitude) of the 3D vector v.
	*/
	float length(const Vector3D& v);

	/**@brief Normalizes the 3D vector v.
	*
	* If the 3D vector is the zero vector v is returned.
	*/
	Vector3D norm(const Vector3D& v);

	/**@brief Converts the 3D vector v from cylindrical coordinates to cartesian coordinates.
	* v should = (r, theta(degrees), z).\n
	* The returned 3D vector = (x, y ,z).
	*/
	Vector3D CylindricalToCartesian(const Vector3D& v);

	/**@brief Converts the 3D vector v from cartesian coordinates to cylindrical coordinates.
	* v should = (x, y, z).\n
	* If vx is zero then no conversion happens and v is returned.\n
	* The returned 3D vector = (r, theta(degrees), z).
	*/
	Vector3D CartesianToCylindrical(const Vector3D& v);

	/**@brief Converts the 3D vector v from spherical coordinates to cartesian coordinates.
	* v should = (pho, phi(degrees), theta(degrees)).\n
	* The returned 3D vector = (x, y, z)
	*/
	Vector3D SphericalToCartesian(const Vector3D& v);

	/**@brief Converts the 3D vector v from cartesian coordinates to spherical coordinates.
	* If v is the zero vector or if vx is zero then no conversion happens and v is returned.\n
	* The returned 3D vector = (r, phi(degrees), theta(degrees)).
	*/
	Vector3D CartesianToSpherical(const Vector3D& v);

	/**@brief Returns a 3D vector that is the projection of a onto b.
	* If b is the zero vector a is returned.
	*/
	Vector3D Projection(const Vector3D& a, const Vector3D& b);


#if defined(_DEBUG)
	void print(const Vector3D& v);
#endif
}