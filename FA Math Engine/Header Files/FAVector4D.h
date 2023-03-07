#pragma once

#include "FAMathUtility.h"

/** @file FAVector4D.h
*	@brief File has a 4D Vector class under the namespace FAMath.
*/


/** @namespace FAMath
*	@brief The name space has utility functions, Vector2D, Vector3D, Vector4D, Matrix4x4, and Quaternion classes.
*/
namespace FAMath
{
	/** @class Vector4D ""
		*	@brief A vector class used for 4D vectors/points and their manipulations.
		*
		*	The datatype for the components is float
		*/
	class Vector4D
	{
	public:
		/**@brief Default Constructor.
		*
		*	Creates a new 4D vector/point with the components initialized to 0.0.
		*/
		Vector4D();

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 4D vector/point with the components initialized to the arguments.
		*/
		Vector4D(float x, float y, float z, float w);

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 4D vector/point with the components initialized to the arguments.
		*/
		Vector4D(Vector2D v, float z = 0.0f, float w = 0.0f);

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 4D vector/point with the components initialized to the arguments.
		*/
		Vector4D(Vector3D v, float w = 0.0f);

		/**@brief Returns a reference to the x component.
		*/
		float& x();

		/**@brief Returns a reference to the y component.
		*/
		float& y();

		/**@brief Returns a reference to the z component.
		*/
		float& z();

		/**@brief Returns a reference to the w component.
		*/
		float& w();

		/**@brief Returns a constant reference to the x component.
		*/
		const float& x() const;

		/**@brief Returns a constant reference to the y component.
		*/
		const float& y() const;

		/**@brief Returns a constant reference to the z component.
		*/
		const float& z() const;

		/**@brief Returns a constant reference to the w component.
		*/
		const float& w() const;

		/**@brief 4D vector addition through overloading operator +=.
		*/
		Vector4D& operator+=(const Vector4D& b);

		/**@brief 4D vector subtraction through overloading operator -=.
		*/
		Vector4D& operator-=(const Vector4D& b);

		/**@brief 4D vector scalar multiplication through overloading operator *=.
		*/
		Vector4D& operator*=(const float& k);

		/**@brief 4D vector scalar division through overloading operator /=.
		*
		* If k is zero, the vector is unchanged.
		*/
		Vector4D& operator/=(const float& k);

	private:
		float m_x;
		float m_y;
		float m_z;
		float m_w;
	};

	/**@brief Returns true if a is the zero vector.
	*/
	bool zeroVector(const Vector4D& a);

	/**@brief 4D vector addition.
	*/
	Vector4D operator+(const Vector4D& a, const Vector4D& b);

	/**@brief 4D vector negation.
	*/
	Vector4D operator-(const Vector4D& v);

	/**@brief 4D vector subtraction.
	*/
	Vector4D operator-(const Vector4D& a, const Vector4D& b);

	/**@brief 4D vector scalar multiplication.
	* Returns a * k, where a is a vector and k is a scalar(float)
	*/
	Vector4D operator*(const Vector4D& a, const float& k);

	/**@brief 4D vector scalar multiplication.
	* Returns k * a,  where a is a vector and k is a scalar(float)
	*/
	Vector4D operator*(const float& k, const Vector4D& a);

	/**@brief 4D vector scalar division.
	* Returns a / k,  where a is a vector and k is a scalar(float)
	* If k = 0 the returned vector is the zero vector.
	*/
	Vector4D operator/(const Vector4D& a, const float& k);

	/**@brief Returns the dot product between two 4D vectors.
	*/
	float dotProduct(const Vector4D& a, const Vector4D& b);

	/**@brief Returns the length(magnitude) of the 4D vector v.
	*/
	float length(const Vector4D& v);

	/**@brief Normalizes the 4D vector v.
	*
	* If the 4D vector is the zero vector v is returned.
	*/
	Vector4D norm(const Vector4D& v);

	/**@brief Returns a 4D vector that is the projection of a onto b.
	* If b is the zero vector a is returned.
	*/
	Vector4D Projection(const Vector4D& a, const Vector4D& b);


#if defined(_DEBUG)
	void print(const Vector4D& v);
#endif
}