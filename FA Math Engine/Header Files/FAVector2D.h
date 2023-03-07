#pragma once

#include "FAMathUtility.h"

/** @file FAVector2D.h
*	@brief File has a 2D Vector class under the namespace FAMath.
*/


/** @namespace FAMath
*	@brief The name space has utility functions, Vector2D, Vector3D, Vector4D, Matrix4x4, and Quaternion classes.
*/
namespace FAMath
{
	/** @class Vector2D ""
		*	@brief A vector class used for 2D vectors/points and their manipulations.
		*
		*	The datatype for the components is float.
		*/
	class Vector2D
	{
	public:


		/**@brief Default Constructor.
		*
		*	Creates a new 2D vector/point with the components initialized to 0.0.
		*/
		Vector2D();

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 2D vector/point with the components initialized to the arguments.
		*/
		Vector2D(float x, float y);

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 2D vector/point with the components initialized to the arguments.
		*/
		Vector2D(Vector3D v);

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 2D vector/point with the components initialized to the arguments.
		*/
		Vector2D(Vector4D v);

		/**@brief Returns a reference to the x component.
		*/
		float& x();

		/**@brief Returns a reference to the y component.
		*/
		float& y();

		/**@brief Returns a constant reference to the x component.
		*/
		const float& x() const;

		/**@brief Returns a constant reference to the y component.
		*/
		const float& y() const;

		/**@brief 2D vector addition through overloading operator +=.
		*/
		Vector2D& operator+=(const Vector2D& b);

		/**@brief 2D vector subtraction through overloading operator -=.
		*/
		Vector2D& operator-=(const Vector2D& b);

		/**@brief 2D vector scalar multiplication through overloading operator *=.
		*/
		Vector2D& operator*=(const float& k);

		/**@brief 2D vector scalar division through overloading operator /=.
		*
		* If k is zero, the vector is unchanged.
		*/
		Vector2D& operator/=(const float& k);

	private:
		float m_x;
		float m_y;
	};

	/**@brief Returns true if a is the zero vector.
	*/
	bool zeroVector(const Vector2D& a);

	/**@brief 2D vector addition.
	*/
	Vector2D operator+(const Vector2D& a, const Vector2D& b);

	/**@brief 2D vector negation.
	*/
	Vector2D operator-(const Vector2D& v);

	/**@brief 2D vector subtraction.
	*/
	Vector2D operator-(const Vector2D& a, const Vector2D& b);

	/**@brief 2D vector scalar multiplication.
	* Returns a * k, where a is a vector and k is a scalar(float)
	*/
	Vector2D operator*(const Vector2D& a, const float& k);

	/**@brief 2D vector scalar multiplication.
	* Returns k * a,  where a is a vector and k is a scalar(float)
	*/
	Vector2D operator*(const float& k, const Vector2D& a);

	/**@brief 2D vector scalar division.
	* Returns a / k,  where a is a vector and k is a scalar(float)
	* If k = 0 the returned vector is the zero vector.
	*/
	Vector2D operator/(const Vector2D& a, const float& k);

	/**@brief Returns the dot product between two 2D vectors.
	*
	*/
	float dotProduct(const Vector2D& a, const Vector2D& b);

	/**@brief Returns the length(magnitude) of the 2D vector v.
	*/
	float length(const Vector2D& v);

	/**@brief Normalizes the 2D vector v.
	*
	* If the 2D vector is the zero vector v is returned.
	*/
	Vector2D norm(const Vector2D& v);

	/**@brief Converts the 2D vector v from polar coordinates to cartesian coordinates.
	* v should = (r, theta(degrees))
	* The returned 2D vector = (x, y)
	*/
	Vector2D PolarToCartesian(const Vector2D& v);

	/**@brief Converts the 2D vector v from cartesian coordinates to polar coordinates.
	* v should = (x, y, z)
	* If vx is zero then no conversion happens and v is returned.\n
	* The returned 2D vector = (r, theta(degrees)).
	*/
	Vector2D CartesianToPolar(const Vector2D& v);

	/**@brief Returns a 2D vector that is the projection of a onto b.
	* If b is the zero vector a is returned.
	*/
	Vector2D Projection(const Vector2D& a, const Vector2D& b);

#if defined(_DEBUG)
	void print(const Vector2D& v);
#endif
}