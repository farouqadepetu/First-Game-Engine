#pragma once

#include <cmath>

#if defined(_DEBUG)
#include <iostream>
#endif


#define EPSILON 1e-6f
#define PI 3.14159f
#define PI2 6.28319f

/** @namespace FAMath
*	@brief Has utility functions, Vector2D, Vector3D, Vector4D, Matrix4x4, and Quaternion classes.
*/
namespace FAMath
{
	class Vector2D;
	class Vector3D;
	class Vector4D;

	/**@brief Returns true if \a x and \a y are equal.
	* 
	* Uses exact \a epsilion and adaptive \a epsilion to compare.
	*/
	inline bool CompareFloats(float x, float y, float epsilon)
	{
		float diff = fabs(x - y);
		//exact epsilon
		if (diff < epsilon)
		{
			return true;
		}

		//adapative epsilon
		return diff <= epsilon * (((fabs(x)) >(fabs(y))) ? (fabs(x)) : (fabs(y)));
	}

	/**@brief Returns true if \a x and \a y are equal.
	* 
	* Uses exact \a epsilion and adaptive \a epsilion to compare.
	*/
	inline bool CompareDoubles(double x, double y, double epsilon)
	{
		double diff = fabs(x - y);
		//exact epsilon
		if (diff < epsilon)
		{
			return true;
		}

		//adapative epsilon
		return  diff <= epsilon * (((fabs(x)) >(fabs(y))) ? (fabs(x)) : (fabs(y)));
	}

//------------------------------------------------------------------------------------------------------------------------------------
	
	/** @class Vector2D ""
		*	@brief A vector class used for 2D vectors/points and their manipulations.
		*
		*	The datatype for the components is float.
		*/
	class Vector2D
	{
	public:

		/**@brief Creates a new 2D vector/point with the components initialized to the arguments.
		*/
		Vector2D(float x = 0.0f, float y = 0.0f);

		/**@brief Creates a new 2D vector/point with the components initialized to the x and y values of the 3D vector.
		*/
		Vector2D(const Vector3D& v);

		/**@brief Creates a new 2D vector/point with the components initialized to the x and y values of the 4D vector.
		*/
		Vector2D(const Vector4D& v);

		/**@brief Returns the x component.
		*/
		float GetX() const;

		/**@brief Returns the y component.
		*/
		float GetY() const;

		/**@brief Sets the x component of the vector to the specified value.
		*/
		void SetX(float x);

		/**@brief Sets the y component to the specified value.
		*/
		void SetY(float y);

		/**@brief Sets the x and y components of this 2D vector to the x and y values of the 3D vector.
		*/
		Vector2D& operator=(const Vector3D& v);

		/**@brief Sets the x and y components of this 2D vector to the x and y values of the 4D vector.
		*/
		Vector2D& operator=(const Vector4D& v);

		/**@brief Adds this vector to vector \a b and stores the result in this vector.
		*/
		Vector2D& operator+=(const Vector2D& b);

		/**@brief Subtracts the vector \a b from this vector and stores the result in this vector.
		*/
		Vector2D& operator-=(const Vector2D& b);

		/**@brief Multiplies this vector by \a k and stores the result in this vector.
		*/
		Vector2D& operator*=(float k);

		/**@brief Divides this vector by \a k and stores the result in this vector.
		*
		* If \a k is zero, the vector is unchanged.
		*/
		Vector2D& operator/=(float k);

	private:
		float mX;
		float mY;
	};


	//--------------------------------------------------------------------------------------
	//Vector2D Constructor

	inline Vector2D::Vector2D(float x, float y) : mX{ x }, mY{ y }
	{}
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector2D Getters and Setters

	inline float Vector2D::GetX() const
	{
		return mX;
	}

	inline float Vector2D::GetY() const
	{
		return mY;
	}

	inline void Vector2D::SetX(float x)
	{
		mX = x;
	}

	inline void Vector2D::SetY(float y)
	{
		mY = y;
	}

	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector2D Memeber functions

	inline Vector2D& Vector2D::operator+=(const Vector2D& b)
	{
		this->mX += b.mX;
		this->mY += b.mY;

		return *this;
	}

	inline Vector2D& Vector2D::operator-=(const Vector2D& b)
	{
		this->mX -= b.mX;
		this->mY -= b.mY;

		return *this;
	}

	inline Vector2D& Vector2D::operator*=(float k)
	{
		this->mX *= k;
		this->mY *= k;

		return *this;
	}

	inline Vector2D& Vector2D::operator/=(float k)
	{
		if (CompareFloats(k, 0.0f, EPSILON))
		{
			return *this;
		}

		this->mX /= k;
		this->mY /= k;

		return *this;
	}

	//-------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------
	//Vector2D Non-member functions

	/**@brief Returns true if \a a is the zero vector.
	*/
	inline bool ZeroVector(const Vector2D& a)
	{
		if (CompareFloats(a.GetX(), 0.0f, EPSILON) && CompareFloats(a.GetY(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	/**@brief Adds \a a with \a b and returns the result.
	*/
	inline Vector2D operator+(const Vector2D& a, const Vector2D& b)
	{
		return Vector2D(a.GetX() + b.GetX(), a.GetY() + b.GetY());
	}

	/**@brief Negates the vector \a v and returns the result.
	*/
	inline Vector2D operator-(const Vector2D& v)
	{
		return Vector2D(-v.GetX(), -v.GetY());
	}

	/**@brief Subtracts \a b from \a a and returns the result.
	*/
	inline Vector2D operator-(const Vector2D& a, const Vector2D& b)
	{
		return Vector2D(a.GetX() - b.GetX(), a.GetY() - b.GetY());
	}

	/**@brief Returns \a a * \a k.
	*/
	inline Vector2D operator*(const Vector2D& a, float k)
	{
		return Vector2D(a.GetX() * k, a.GetY() * k);
	}

	/**@brief Returns \a k * \a a.
	*/
	inline Vector2D operator*(float k, const Vector2D& a)
	{
		return Vector2D(k * a.GetX(), k * a.GetY());
	}

	/**@brief Returns \a a / \a k.
	* If \a k = 0 the returned vector is the zero vector.
	*/
	inline Vector2D operator/(const Vector2D& a, const float& k)
	{
		if (CompareFloats(k, 0.0f, EPSILON))
		{
			return Vector2D();
		}

		return Vector2D(a.GetX() / k, a.GetY() / k);
	}

	/**@brief Returns the dot product between \a a and \a b.
	*/
	inline float DotProduct(const Vector2D& a, const Vector2D& b)
	{
		return a.GetX() * b.GetX() + a.GetY() * b.GetY();
	}

	/**@brief Returns the length(magnitude) of the 2D vector \a v.
	*/
	inline float Length(const Vector2D& v)
	{
		return sqrt(v.GetX() * v.GetX() + v.GetY() * v.GetY());
	}

	/**@brief Normalizes the 2D vector \a v.
	* If the 2D vector is the zero vector \a v is returned.
	*/
	inline Vector2D Norm(const Vector2D& v)
	{
		//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
		
		//v is the zero vector
		if (ZeroVector(v))
		{
			return v;
		}

		float mag{ Length(v) };

		return Vector2D(v.GetX() / mag, v.GetY() / mag);
	}

	/**@brief Converts the 2D vector \a v from polar coordinates to cartesian coordinates.
	* \a v should = (r, theta(degrees))
	* The returned 2D vector = (x, y)
	*/
	inline Vector2D PolarToCartesian(const Vector2D& v)
	{
		//v = (r, theta)
		//x = rcos((theta)
		//y = rsin(theta)
		float angle{ v.GetY() * PI / 180.0f };

		return Vector2D(v.GetX() * cos(angle), v.GetX() * sin(angle));
	}

	/**@brief Converts the 2D vector \a v from cartesian coordinates to polar coordinates.
	* \a v should = (x, y)
	* If \a vx is zero then no conversion happens and \a v is returned.\n
	* The returned 2D vector = (r, theta(degrees)).
	*/
	inline Vector2D CartesianToPolar(const Vector2D& v)
	{
		//v = (x, y)
		//r = sqrt(vx^2 + vy^2)
		//theta = arctan(y / x)

		if (CompareFloats(v.GetX(), 0.0f, EPSILON))
		{
			return v;
		}

		float theta{ atan2(v.GetY(), v.GetX()) * 180.0f / PI };
		return Vector2D(Length(v), theta);
	}

	/**@brief Returns a 2D vector that is the projection of \a a onto \a b.
	* If \a b is the zero vector \a a is returned.
	*/
	inline Vector2D Projection(const Vector2D& a, const Vector2D& b)
	{
		//Projb(a) = (a dot b)b
		//normalize b before projecting

		Vector2D normB(Norm(b));
		return Vector2D(DotProduct(a, normB) * normB);
	}


#if defined(_DEBUG)
	inline void print(const Vector2D& v)
	{
		std::cout << "(" << v.GetX() << ", " << v.GetY() << ")";
	}
#endif
	//-------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------
	/** @class Vector3D ""
	*	@brief A vector class used for 3D vectors/points and their manipulations.
	*
	*	The datatype for the components is float.
	*/
	class Vector3D
	{
	public:

		/**@brief Creates a new 3D vector/point with the components initialized to the arguments.
		*/
		Vector3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);

		/**@brief Creates a new 3D vector/point with the components initialized to the x and y values of the 2D vector and the specified z value;
		*/
		Vector3D(const Vector2D& v, float z = 0.0f);

		/**@brief Creates a new 3D vector/point with the components initialized to the x, y and z values of the 4D vector.
		*/
		Vector3D(const Vector4D& v);

		/**@brief Returns the x component.
		*/
		float GetX() const;

		/**@brief Returns y component.
		*/
		float GetY() const;

		/**@brief Returns the z component.
		*/
		float GetZ() const;

		/**@brief Sets the x component to the specified value.
		*/
		void SetX(float x);

		/**@brief Sets the y component to the specified value.
		*/
		void SetY(float y);

		/**@brief Sets the z component to the specified value.
		*/
		void SetZ(float z);

		/**@brief Sets the x and y components of this 3D vector to the x and y values of the 2D vector and sets the z component to 0.0f.
		*/
		Vector3D& operator=(const Vector2D& v);

		/**@brief Sets the x, y  and z components of this 3D vector to the x, y and z values of the 4D vector.
		*/
		Vector3D& operator=(const Vector4D& v);

		/**@brief Adds this vector to vector \a b and stores the result in this vector.
		*/
		Vector3D& operator+=(const Vector3D& b);

		/**@brief Subtracts \a b from this vector and stores the result in this vector.
		*/
		Vector3D& operator-=(const Vector3D& b);

		/**@brief Multiplies this vector by \a k and stores the result in this vector.
		*/
		Vector3D& operator*=(float k);

		/**@brief Divides this vector by \a k and stores the result in this vector.
		*
		* If \a k is zero, the vector is unchanged.
		*/
		Vector3D& operator/=(float k);

	private:
		float mX;
		float mY;
		float mZ;
	};

	//--------------------------------------------------------------------------------------
	//Vector3D Constructors

	inline Vector3D::Vector3D(float x, float y, float z) : mX{ x }, mY{ y }, mZ{ z }
	{}

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector3D Getters and Setters

	inline float Vector3D::GetX() const
	{
		return mX;
	}

	inline float Vector3D::GetY() const
	{
		return mY;
	}

	inline float Vector3D::GetZ() const
	{
		return mZ;
	}

	inline void Vector3D::SetX(float x)
	{
		mX = x;
	}

	inline void Vector3D::SetY(float y)
	{
		mY = y;
	}

	inline void Vector3D::SetZ(float z)
	{
		mZ = z;
	}
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector3D Memeber functions

	inline Vector3D& Vector3D::operator+=(const Vector3D& b)
	{
		this->mX += b.mX;
		this->mY += b.mY;
		this->mZ += b.mZ;

		return *this;
	}

	inline Vector3D& Vector3D::operator-=(const Vector3D& b)
	{
		this->mX -= b.mX;
		this->mY -= b.mY;
		this->mZ -= b.mZ;

		return *this;
	}

	inline Vector3D& Vector3D::operator*=(float k)
	{
		this->mX *= k;
		this->mY *= k;
		this->mZ *= k;

		return *this;
	}

	inline Vector3D& Vector3D::operator/=(float k)
	{
		if (CompareFloats(k, 0.0f, EPSILON))
		{
			return *this;
		}

		this->mX /= k;
		this->mY /= k;
		this->mZ /= k;

		return *this;
	}

	//-------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------
	//Vector3D Non-member functions

	/**@brief Returns true if \a a is the zero vector.
	*/
	inline bool ZeroVector(const Vector3D& a)
	{
		if (CompareFloats(a.GetX(), 0.0f, EPSILON) && CompareFloats(a.GetY(), 0.0f, EPSILON) &&
			CompareFloats(a.GetZ(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	/**@brief Adds \a a and \a b and returns the result.
	*/
	inline Vector3D operator+(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.GetX() + b.GetX(), a.GetY() + b.GetY(), a.GetZ() + b.GetZ());
	}

	/**@brief Negates the vector \a v and returns the result.
	*/
	inline Vector3D operator-(const Vector3D& v)
	{
		return Vector3D(-v.GetX(), -v.GetY(), -v.GetZ());
	}

	/**@brief Subtracts \a b from \a a and returns the result.
	*/
	inline Vector3D operator-(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.GetX() - b.GetX(), a.GetY() - b.GetY(), a.GetZ() - b.GetZ());
	}

	/**@brief Returns \a a * \a k.
	*/
	inline Vector3D operator*(const Vector3D& a, float k)
	{
		return Vector3D(a.GetX() * k, a.GetY() * k, a.GetZ() * k);
	}

	/**@brief Returns \a k * \a a.
	*/
	inline Vector3D operator*(float k, const Vector3D& a)
	{
		return Vector3D(k * a.GetX(), k * a.GetY(), k * a.GetZ());
	}

	/**@brief Returns \a a / \a k.
	* 
	* If \a k = 0 the returned vector is the zero vector.
	*/
	inline Vector3D operator/(const Vector3D& a, float k)
	{
		if (CompareFloats(k, 0.0f, EPSILON))
		{
			return Vector3D();
		}

		return Vector3D(a.GetX() / k, a.GetY() / k, a.GetZ() / k);
	}

	/**@brief Returns the dot product between \a a and \a b.
	*/
	inline float DotProduct(const Vector3D& a, const Vector3D& b)
	{
		//a dot b = axbx + ayby + azbz
		return a.GetX() * b.GetX() + a.GetY() * b.GetY() + a.GetZ() * b.GetZ();
	}

	/**@brief Returns the cross product between \a a and \a b.
	*/
	inline Vector3D CrossProduct(const Vector3D& a, const Vector3D& b)
	{
		//a x b = (aybz - azby, azbx - axbz, axby - aybx)

		return Vector3D(a.GetY() * b.GetZ() - a.GetZ() * b.GetY(),
			a.GetZ() * b.GetX() - a.GetX() * b.GetZ(),
			a.GetX() * b.GetY() - a.GetY() * b.GetX());
	}

	/**@brief Returns the length(magnitude) of the 3D vector \a v.
	*/
	inline float Length(const Vector3D& v)
	{
		//length(v) = sqrt(vx^2 + vy^2 + vz^2)

		return sqrt(v.GetX() * v.GetX() + v.GetY() * v.GetY() + v.GetZ() * v.GetZ());
	}

	/**@brief Normalizes the 3D vector \a v.
	* 
	* If the 3D vector is the zero vector \a v is returned.
	*/
	inline Vector3D Norm(const Vector3D& v)
	{
		//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
		//v is the zero vector
		if (ZeroVector(v))
		{
			return v;
		}

		float mag{ Length(v) };

		return Vector3D(v.GetX() / mag, v.GetY() / mag, v.GetZ() / mag);
	}

	/**@brief Converts the 3D vector \a v from cylindrical coordinates to cartesian coordinates.
	* 
	* \a v should = (r, theta(degrees), z).\n
	* The returned 3D vector = (x, y ,z).
	*/
	inline Vector3D CylindricalToCartesian(const Vector3D& v)
	{
		//v = (r, theta, z)
		//x = rcos(theta)
		//y = rsin(theta)
		//z = z
		float angle{ v.GetY() * PI / 180.0f };

		return Vector3D(v.GetX() * cos(angle), v.GetX() * sin(angle), v.GetZ());
	}

	/**@brief Converts the 3D vector \a v from cartesian coordinates to cylindrical coordinates.
	* 
	* \a v should = (x, y, z).\n
	* If \a vx is zero then no conversion happens and \a v is returned.\n
	* The returned 3D vector = (r, theta(degrees), z).
	*/
	inline Vector3D CartesianToCylindrical(const Vector3D& v)
	{
		//v = (x, y, z)
		//r = sqrt(vx^2 + vy^2 + vz^2)
		//theta = arctan(y / x)
		//z = z
		if (CompareFloats(v.GetX(), 0.0f, EPSILON))
		{
			return v;
		}

		float theta{ atan2(v.GetY(), v.GetX()) * 180.0f / PI };
		return Vector3D(Length(v), theta, v.GetZ());
	}

	/**@brief Converts the 3D vector \a v from spherical coordinates to cartesian coordinates.
	* 
	* \a v should = (pho, phi(degrees), theta(degrees)).\n
	* The returned 3D vector = (x, y, z)
	*/
	inline Vector3D SphericalToCartesian(const Vector3D& v)
	{
		// v = (pho, phi, theta)
		//x = pho * sin(phi) * cos(theta)
		//y = pho * sin(phi) * sin(theta)
		//z = pho * cos(theta);

		float phi{ v.GetY() * PI / 180.0f };
		float theta{ v.GetZ() * PI / 180.0f };

		return Vector3D(v.GetX() * sin(phi) * cos(theta), v.GetX() * sin(phi) * sin(theta), v.GetX() * cos(theta));
	}

	/**@brief Converts the 3D vector \a v from cartesian coordinates to spherical coordinates.
	* 
	* If \a v is the zero vector or if \a vx is zero then no conversion happens and \a v is returned.\n
	* The returned 3D vector = (r, phi(degrees), theta(degrees)).
	*/
	inline Vector3D CartesianToSpherical(const Vector3D& v)
	{
		//v = (x, y ,z)
		//pho = sqrt(vx^2 + vy^2 + vz^2)
		//phi = arcos(z / pho)
		//theta = arctan(y / x)

		if (CompareFloats(v.GetX(), 0.0f, EPSILON) || ZeroVector(v))
		{
			return v;
		}

		float pho{ Length(v) };
		float phi{ acos(v.GetZ() / pho) * 180.0f / PI };
		float theta{ atan2(v.GetY(), v.GetX()) * 180.0f / PI };

		return Vector3D(pho, phi, theta);
	}

	/**@brief Returns a 3D vector that is the projection of \a a onto \a b.
	* 
	* If \a b is the zero vector a is returned.
	*/
	inline Vector3D Projection(const Vector3D& a, const Vector3D& b)
	{
		//Projb(a) = (a dot b)b
		//normalize b before projecting

		Vector3D normB(Norm(b));
		return Vector3D(DotProduct(a, normB) * normB);
	}

	/**@brief Orthonormalizes the specified vectors.
	* 
	* Uses Classical Gram-Schmidt.
	*/
	inline void Orthonormalize(Vector3D& x, Vector3D& y, Vector3D& z)
	{
		x = Norm(x);
		y = Norm(CrossProduct(z, x));
		z = Norm(CrossProduct(x, y));
	}


#if defined(_DEBUG)
	inline void print(const Vector3D& v)
	{
		std::cout << "(" << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << ")";
	}
#endif
	//-------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------
	/** @class Vector4D ""
		*	@brief A vector class used for 4D vectors/points and their manipulations.
		*
		*	The datatype for the components is float
		*/
	class Vector4D
	{
	public:
		/**@brief Creates a new 4D vector/point with the components initialized to the arguments.
		*/
		Vector4D(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

		/**@brief Creates a new 4D vector/point with the components initialized to the x and y values of the 2D vector and the specified z and w values.
		*/
		Vector4D(const Vector2D& v, float z = 0.0f, float w = 0.0f);

		/**@brief Creates a new 4D vector/point with the components initialized to x, y and z values of the 3D vector and the specified w value.
		*/
		Vector4D(const Vector3D& v, float w = 0.0f);

		/**@brief Returns the x component.
		*/
		float GetX() const;

		/**@brief Returns the y component.
		*/
		float GetY() const;

		/**@brief Returns the z component.
		*/
		float GetZ() const;

		/**@brief Returns the w component.
		*/
		float GetW() const;

		/**@brief Sets the x component to the specified value.
		*/
		void SetX(float x);

		/**@brief Sets the y component to the specified value.
		*/
		void SetY(float y);

		/**@brief Sets the z component to the specified value.
		*/
		void SetZ(float z);

		/**@brief Sets the w component to the specified value.
		*/
		void SetW(float w);

		/**@brief Sets the x and y components of this 4D vector to the x and y values of the 2D vector and sets the z and w component to 0.0f.
		*/
		Vector4D& operator=(const Vector2D& v);

		/**@brief Sets the x, y and z components of this 4D vector to the x, y and z values of the 3D vector and sets the w component to 0.0f.
		*/
		Vector4D& operator=(const Vector3D& v);

		/**@brief Adds this vector to vector \a b and stores the result in this vector. 
		*/
		Vector4D& operator+=(const Vector4D& b);

		/**@brief Subtracts the vector \a b from this vector and stores the result in this vector. 
		*/
		Vector4D& operator-=(const Vector4D& b);

		/**@brief Multiplies this vector by \a k and stores the result in this vector. 
		*/
		Vector4D& operator*=(float k);

		/**@brief Divides this vector by \a k and stores the result in this vector. 
		*
		* If \a k is zero, the vector is unchanged.
		*/
		Vector4D& operator/=(float k);

	private:
		float mX;
		float mY;
		float mZ;
		float mW;
	};

	//--------------------------------------------------------------------------------------
	//Vector4D Constructors

	inline Vector4D::Vector4D(float x, float y, float z, float w) : mX{ x }, mY{ y }, mZ{ z }, mW{ w }
	{}

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector4D Getters and Setters

	inline float Vector4D::GetX() const
	{
		return mX;
	}

	inline float Vector4D::GetY() const
	{
		return mY;
	}

	inline float Vector4D::GetZ() const
	{
		return mZ;
	}

	inline float Vector4D::GetW() const
	{
		return mW;
	}

	inline void Vector4D::SetX(float x)
	{
		mX = x;
	}

	inline void Vector4D::SetY(float y)
	{
		mY = y;
	}

	inline void Vector4D::SetZ(float z)
	{
		mZ = z;
	}

	inline void Vector4D::SetW(float w)
	{
		mW = w;
	}
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector4D Memeber functions

	inline Vector4D& Vector4D::operator+=(const Vector4D& b)
	{
		this->mX += b.mX;
		this->mY += b.mY;
		this->mZ += b.mZ;
		this->mW += b.mW;

		return *this;
	}

	inline Vector4D& Vector4D::operator-=(const Vector4D& b)
	{
		this->mX -= b.mX;
		this->mY -= b.mY;
		this->mZ -= b.mZ;
		this->mW -= b.mW;

		return *this;
	}

	inline Vector4D& Vector4D::operator*=(float k)
	{
		this->mX *= k;
		this->mY *= k;
		this->mZ *= k;
		this->mW *= k;

		return *this;
	}

	inline Vector4D& Vector4D::operator/=(float k)
	{
		if (CompareFloats(k, 0.0f, EPSILON))
		{
			return *this;
		}

		this->mX /= k;
		this->mY /= k;
		this->mZ /= k;
		this->mW /= k;

		return *this;
	}

	//-------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------
	//Vector4D Non-member functions

	/**@brief Returns true if \a a is the zero vector.
	*/
	inline bool ZeroVector(const Vector4D& a)
	{
		if (CompareFloats(a.GetX(), 0.0f, EPSILON) && CompareFloats(a.GetY(), 0.0f, EPSILON) &&
			CompareFloats(a.GetZ(), 0.0f, EPSILON) && CompareFloats(a.GetW(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	/**@brief Adds \a a with \a b and returns the result.
	*/
	inline Vector4D operator+(const Vector4D& a, const Vector4D& b)
	{
		return Vector4D(a.GetX() + b.GetX(), a.GetY() + b.GetY(), a.GetZ() + b.GetZ(), a.GetW() + b.GetW());
	}

	/**@brief Negatives \a v and returns the result.
	*/
	inline Vector4D operator-(const Vector4D& v)
	{
		return Vector4D(-v.GetX(), -v.GetY(), -v.GetZ(), -v.GetW());
	}

	/**@brief Subtracts \a b from \a a and returns the result.
	*/
	inline Vector4D operator-(const Vector4D& a, const Vector4D& b)
	{
		return Vector4D(a.GetX() - b.GetX(), a.GetY() - b.GetY(), a.GetZ() - b.GetZ(), a.GetW() - b.GetW());
	}

	/**@brief  Returns \a a * \a k.
	*/
	inline Vector4D operator*(const Vector4D& a, float k)
	{
		return Vector4D(a.GetX() * k, a.GetY() * k, a.GetZ() * k, a.GetW() * k);
	}

	/**@brief Returns \a k * \a a.
	*/
	inline Vector4D operator*(float k, const Vector4D& a)
	{
		return Vector4D(k * a.GetX(), k * a.GetY(), k * a.GetZ(), k * a.GetW());
	}

	/**@brief Returns \a a / \a k.
	* 
	* If \a k = 0 the returned vector is the zero vector.
	*/
	inline Vector4D operator/(const Vector4D& a, float k)
	{
		if (CompareFloats(k, 0.0f, EPSILON))
		{
			return Vector4D();
		}

		return Vector4D(a.GetX() / k, a.GetY() / k, a.GetZ() / k, a.GetW() / k);
	}

	/**@brief Returns the dot product between \a a and \a b.
	*/
	inline float DotProduct(const Vector4D& a, const Vector4D& b)
	{
		//a dot b = axbx + ayby + azbz + awbw
		return a.GetX() * b.GetX() + a.GetY() * b.GetY() + a.GetZ() * b.GetZ() + a.GetW() * b.GetW();
	}

	/**@brief Returns the length(magnitude) of the 4D vector \a v.
	*/
	inline float Length(const Vector4D& v)
	{
		//length(v) = sqrt(vx^2 + vy^2 + vz^2 + vw^2)
		return sqrt(v.GetX() * v.GetX() + v.GetY() * v.GetY() + v.GetZ() * v.GetZ() + v.GetW() * v.GetW());
	}

	/**@brief Normalizes the 4D vector \a v.
	* 
	* If the 4D vector is the zero vector \a v is returned.
	*/
	inline Vector4D Norm(const Vector4D& v)
	{
		//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
		//v is the zero vector
		if (ZeroVector(v))
		{
			return v;
		}

		float mag{ Length(v) };

		return Vector4D(v.GetX() / mag, v.GetY() / mag, v.GetZ() / mag, v.GetW() / mag);
	}

	/**@brief Returns a 4D vector that is the projection of \a a onto \a b.
	* 
	* If \a b is the zero vector a is returned.
	*/
	inline Vector4D Projection(const Vector4D& a, const Vector4D& b)
	{
		//Projb(a) = (a dot b)b
		//normalize b before projecting
		Vector4D normB(Norm(b));
		return Vector4D(DotProduct(a, normB) * normB);
	}

	/**@brief Orthonormalizes the specified vectors.
	*
	* Uses Classical Gram-Schmidt.
	*/
	inline void Orthonormalize(Vector4D& x, Vector4D& y, Vector4D& z)
	{
		FAMath::Vector3D tempX(x.GetX(), x.GetY(), x.GetZ());
		FAMath::Vector3D tempY(y.GetX(), y.GetY(), y.GetZ());
		FAMath::Vector3D tempZ(z.GetX(), z.GetY(), z.GetZ());

		tempX = Norm(tempX);
		tempY = Norm(CrossProduct(tempZ, tempX));
		tempZ = Norm(CrossProduct(tempX, tempY));

		x = FAMath::Vector4D(tempX.GetX(), tempX.GetY(), tempX.GetZ(), 0.0f);
		y = FAMath::Vector4D(tempY.GetX(), tempY.GetY(), tempY.GetZ(), 0.0f);
		z = FAMath::Vector4D(tempZ.GetX(), tempZ.GetY(), tempZ.GetZ(), 0.0f);
	}


#if defined(_DEBUG)
	inline void print(const Vector4D& v)
	{
		std::cout << "(" << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << ", " << v.GetW() << ")";
	}
#endif
	//-------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------
	/** @class Matrix4x4 ""
	*	@brief A matrix class used for 4x4 matrices and their manipulations.
	*
	*	The datatype for the components is float.\n
	*	The 4x4 matrix is treated as a row-major matrix.
	*
	*/
	class Matrix4x4
	{
	public:

		/**@brief Creates a new 4x4 identity matrix.
		*/
		Matrix4x4();

		/**@brief Creates a new 4x4 matrix with elements initialized to the given 2D array.
		* 
		*	If \a a isn't a 4x4 matrix, the behavior is undefined.
		*/
		Matrix4x4(float a[][4]);

		/**@brief Creates a new 4x4 matrix with each row being set to the specified rows.
		*/
		Matrix4x4(const Vector4D& r1, const Vector4D& r2, const Vector4D& r3, const Vector4D& r4);

		/**@brief Returns a pointer to the first element in the matrix.
		*/
		float* Data();

		/**@brief Returns a constant pointer to the first element in the matrix.
		*/
		const float* Data() const;

		/**@brief Returns a constant reference to the element at the given (\a row, \a col).
		* 
		* The \a row and \a col values should be between [0,3]. If any of them are out of that range, the first element will be returned.
		*/
		const float& operator()(unsigned int row, unsigned int col) const;

		/**@brief Returns a reference to the element at the given (\a row, \a col).
		* 
		* The \a row and \a col values should be between [0,3]. If any of them are out of that range, the first element will be returned.
		*/
		float& operator()(unsigned int row, unsigned int col);

		/**@brief Returns the specified \a row.
		* 
		*  \a Row should be between [0,3]. If it is out of range the first row will be returned.
		*/
		Vector4D GetRow(unsigned int row) const;

		/**@brief Returns the specified \a col.
		* 
		* \a Col should be between [0,3]. If it is out of range the first col will be returned.
		*/
		Vector4D GetCol(unsigned int col) const;

		/**@brief Sets each element in the given \a row to the components of vector \a v.
		* 
		* \a Row should be between [0,3]. If it is out of range the first row will be set.
		*/
		void SetRow(unsigned int row, Vector4D v);

		/**@brief Sets each element in the given \a col to the components of vector \a v.
		* 
		* \a Col should be between [0,3]. If it is out of range the first col will be set.
		*/
		void SetCol(unsigned int col, Vector4D v);

		/**@brief Adds this 4x4 matrix with given matrix \a m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator+=(const Matrix4x4& m);

		/**@brief Subtracts \a m from this 4x4 matrix stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator-=(const Matrix4x4& m);

		/**@brief Multiplies this 4x4 matrix with \a k and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator*=(float k);

		/**@brief Multiplies this 4x4 matrix with given matrix \a m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator*=(const Matrix4x4& m);

	private:

		float mMat[4][4];
	};

	//-------------------------------------------------------------------------------------
	inline Matrix4x4::Matrix4x4()
	{
		//1st row
		mMat[0][0] = 1.0f;
		mMat[0][1] = 0.0f;
		mMat[0][2] = 0.0f;
		mMat[0][3] = 0.0f;

		//2nd
		mMat[1][0] = 0.0f;
		mMat[1][1] = 1.0f;
		mMat[1][2] = 0.0f;
		mMat[1][3] = 0.0f;

		//3rd row
		mMat[2][0] = 0.0f;
		mMat[2][1] = 0.0f;
		mMat[2][2] = 1.0f;
		mMat[2][3] = 0.0f;

		//4th row
		mMat[3][0] = 0.0f;
		mMat[3][1] = 0.0f;
		mMat[3][2] = 0.0f;
		mMat[3][3] = 1.0f;
	}



	inline Matrix4x4::Matrix4x4(float a[][4])
	{
		//1st row
		mMat[0][0] = a[0][0];
		mMat[0][1] = a[0][1];
		mMat[0][2] = a[0][2];
		mMat[0][3] = a[0][3];

		//2nd
		mMat[1][0] = a[1][0];
		mMat[1][1] = a[1][1];
		mMat[1][2] = a[1][2];
		mMat[1][3] = a[1][3];

		//3rd row
		mMat[2][0] = a[2][0];
		mMat[2][1] = a[2][1];
		mMat[2][2] = a[2][2];
		mMat[2][3] = a[2][3];

		//4th row
		mMat[3][0] = a[3][0];
		mMat[3][1] = a[3][1];
		mMat[3][2] = a[3][2];
		mMat[3][3] = a[3][3];
	}

	inline Matrix4x4::Matrix4x4(const Vector4D& r1, const Vector4D& r2, const Vector4D& r3, const Vector4D& r4)
	{
		SetRow(0, r1);
		SetRow(1, r2);
		SetRow(2, r3);
		SetRow(3, r4);
	}

	inline float* Matrix4x4::Data()
	{
		return mMat[0];
	}

	inline const float* Matrix4x4::Data() const
	{
		return mMat[0];
	}

	inline const float& Matrix4x4::operator()(unsigned int row, unsigned int col) const
	{
		if (row > 3 || col > 3)
		{
			return mMat[0][0];
		}
		else
		{
			return mMat[row][col];
		}
	}

	inline float& Matrix4x4::operator()(unsigned int row, unsigned int col)
	{
		if (row > 3 || col > 3)
		{
			return mMat[0][0];
		}
		else
		{
			return mMat[row][col];
		}
	}

	inline Vector4D Matrix4x4::GetRow(unsigned int row) const
	{
		if (row < 0 || row > 3)
			return Vector4D(mMat[0][0], mMat[0][1], mMat[0][2], mMat[0][3]);
		else
			return Vector4D(mMat[row][0], mMat[row][1], mMat[row][2], mMat[row][3]);
		
	}

	inline Vector4D Matrix4x4::GetCol(unsigned int col) const
	{
		if (col < 0 || col > 3)
			return Vector4D(mMat[0][0], mMat[1][0], mMat[2][0], mMat[3][0]);
		else
			return Vector4D(mMat[0][col], mMat[1][col], mMat[2][col], mMat[3][col]);
	}

	inline void Matrix4x4::SetRow(unsigned int row, Vector4D v)
	{
		if (row > 3)
		{
			mMat[0][0] = v.GetX();
			mMat[0][1] = v.GetY();
			mMat[0][2] = v.GetZ();
			mMat[0][3] = v.GetW();
		}
		else
		{
			mMat[row][0] = v.GetX();
			mMat[row][1] = v.GetY();
			mMat[row][2] = v.GetZ();
			mMat[row][3] = v.GetW();
		}
	}

	inline void Matrix4x4::SetCol(unsigned int col, Vector4D v)
	{
		if (col > 3)
		{
			mMat[0][0] = v.GetX();
			mMat[1][0] = v.GetY();
			mMat[2][0] = v.GetZ();
			mMat[3][0] = v.GetW();
		}
		else
		{
			mMat[0][col] = v.GetX();
			mMat[1][col] = v.GetY();
			mMat[2][col] = v.GetZ();
			mMat[3][col] = v.GetW();
		}
	}

	inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->mMat[i][j] += m.mMat[i][j];
			}
		}

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->mMat[i][j] -= m.mMat[i][j];
			}
		}

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(float k)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->mMat[i][j] *= k;
			}
		}

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
	{
		Matrix4x4 res;

		for (int i = 0; i < 4; ++i)
		{
			res.mMat[i][0] = (mMat[i][0] * m.mMat[0][0]) +
				(mMat[i][1] * m.mMat[1][0]) +
				(mMat[i][2] * m.mMat[2][0]) +
				(mMat[i][3] * m.mMat[3][0]);

			res.mMat[i][1] = (mMat[i][0] * m.mMat[0][1]) +
				(mMat[i][1] * m.mMat[1][1]) +
				(mMat[i][2] * m.mMat[2][1]) +
				(mMat[i][3] * m.mMat[3][1]);

			res.mMat[i][2] = (mMat[i][0] * m.mMat[0][2]) +
				(mMat[i][1] * m.mMat[1][2]) +
				(mMat[i][2] * m.mMat[2][2]) +
				(mMat[i][3] * m.mMat[3][2]);

			res.mMat[i][3] = (mMat[i][0] * m.mMat[0][3]) +
				(mMat[i][1] * m.mMat[1][3]) +
				(mMat[i][2] * m.mMat[2][3]) +
				(mMat[i][3] * m.mMat[3][3]);
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mMat[i][j] = res.mMat[i][j];
			}
		}

		return *this;
	}

	/**@brief Adds \a m1 with \a m2 and returns the result.
	*/
	inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = m1(i, j) + m2(i, j);
			}
		}

		return res;
	}

	/**@brief Negates the 4x4 matrix \a m.
	*/
	inline Matrix4x4 operator-(const Matrix4x4& m)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = -m(i, j);
			}
		}

		return res;
	}

	/**@brief Subtracts \a m2 from \a m1 and returns the result.
	*/
	inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = m1(i, j) - m2(i, j);
			}
		}

		return res;
	}

	/**@brief Multiplies \a m with \a k and returns the result.
	*/
	inline Matrix4x4 operator*(const Matrix4x4& m, const float& k)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = m(i, j) * k;
			}
		}

		return res;
	}

	/**@brief Multiplies \a k with \m and returns the result.
	*/
	inline Matrix4x4 operator*(const float& k, const Matrix4x4& m)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = k * m(i, j);
			}
		}

		return res;
	}

	/**@brief Multiplies \a m1 with \m2 and returns the result.
	* 
	* Does \a m1 * \a m2 in that order.
	*/
	inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;

		for (int i = 0; i < 4; ++i)
		{
			res(i, 0) = (m1(i, 0) * m2(0, 0)) +
				(m1(i, 1) * m2(1, 0)) +
				(m1(i, 2) * m2(2, 0)) +
				(m1(i, 3) * m2(3, 0));

			res(i, 1) = (m1(i, 0) * m2(0, 1)) +
				(m1(i, 1) * m2(1, 1)) +
				(m1(i, 2) * m2(2, 1)) +
				(m1(i, 3) * m2(3, 1));

			res(i, 2) = (m1(i, 0) * m2(0, 2)) +
				(m1(i, 1) * m2(1, 2)) +
				(m1(i, 2) * m2(2, 2)) +
				(m1(i, 3) * m2(3, 2));

			res(i, 3) = (m1(i, 0) * m2(0, 3)) +
				(m1(i, 1) * m2(1, 3)) +
				(m1(i, 2) * m2(2, 3)) +
				(m1(i, 3) * m2(3, 3));
		}

		return res;
	}

	/**@brief Multiplies \a m with \a v and returns the result.
	* 
	* The vector \a v is a column vector.
	*/
	inline Vector4D operator*(const Matrix4x4& m, const Vector4D& v)
	{
		Vector4D res;

		res.SetX(m(0, 0) * v.GetX() + m(0, 1) * v.GetY() + m(0, 2) * v.GetZ() + m(0, 3) * v.GetW());

		res.SetY(m(1, 0) * v.GetX() + m(1, 1) * v.GetY() + m(1, 2) * v.GetZ() + m(1, 3) * v.GetW());

		res.SetZ(m(2, 0) * v.GetX() + m(2, 1) * v.GetY() + m(2, 2) * v.GetZ() + m(2, 3) * v.GetW());

		res.SetW(m(3, 0) * v.GetX() + m(3, 1) * v.GetY() + m(3, 2) * v.GetZ() + m(3, 3) * v.GetW());

		return res;
	}

	/**@brief Multiplies \a v with \a m and returns the result.
	*
	* The vector \a v is a row vector.
	*/
	inline Vector4D operator*(const Vector4D& v, const Matrix4x4& m)
	{
		Vector4D res;

		res.SetX(v.GetX() * m(0, 0) + v.GetY() * m(1, 0) + v.GetZ() * m(2, 0) + v.GetW() * m(3, 0));

		res.SetY(v.GetX() * m(0, 1) + v.GetY() * m(1, 1) + v.GetZ() * m(2, 1) + v.GetW() * m(3, 1));

		res.SetZ(v.GetX() * m(0, 2) + v.GetY() * m(1, 2) + v.GetZ() * m(2, 2) + v.GetW() * m(3, 2));

		res.SetW(v.GetX() * m(0, 3) + v.GetY() * m(1, 3) + v.GetZ() * m(2, 3) + v.GetW() * m(3, 3));

		return res;
	}

	/**@brief Sets \a m to the identity matrix.
	*/
	inline void SetToIdentity(Matrix4x4& m)
	{
		//set to identity matrix by setting the diagonals to 1.0f and all other elements to 0.0f
		
		//1st row
		m(0, 0) = 1.0f;
		m(0, 1) = 0.0f;
		m(0, 2) = 0.0f;
		m(0, 3) = 0.0f;

		//2nd row
		m(1, 0) = 0.0f;
		m(1, 1) = 1.0f;
		m(1, 2) = 0.0f;
		m(1, 3) = 0.0f;

		//3rd row
		m(2, 0) = 0.0f;
		m(2, 1) = 0.0f;
		m(2, 2) = 1.0f;
		m(2, 3) = 0.0f;

		//4th row
		m(3, 0) = 0.0f;
		m(3, 1) = 0.0f;
		m(3, 2) = 0.0f;
		m(3, 3) = 1.0f;
	}

	/**@brief Returns true if \a m is the identity matrix, false otherwise.
	*/
	inline bool IsIdentity(const Matrix4x4& m)
	{
		//Is the identity matrix if the diagonals are equal to 1.0f and all other elements equals to 0.0f

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
				{
					if (!CompareFloats(m(i, j), 1.0f, EPSILON))
						return false;
					
				}
				else
				{
					if (!CompareFloats(m(i, j), 0.0f, EPSILON))
						return false;
					
				}
			}
		}
	}

	/**@brief Returns the tranpose of the given matrix \a m.
	*/
	inline Matrix4x4 Transpose(const Matrix4x4& m)
	{
		//make the rows into cols

		Matrix4x4 res;

		//1st col = 1st row
		res(0, 0) = m(0, 0);
		res(1, 0) = m(0, 1);
		res(2, 0) = m(0, 2);
		res(3, 0) = m(0, 3);

		//2nd col = 2nd row
		res(0, 1) = m(1, 0);
		res(1, 1) = m(1, 1);
		res(2, 1) = m(1, 2);
		res(3, 1) = m(1, 3);

		//3rd col = 3rd row
		res(0, 2) = m(2, 0);
		res(1, 2) = m(2, 1);
		res(2, 2) = m(2, 2);
		res(3, 2) = m(2, 3);

		//4th col = 4th row
		res(0, 3) = m(3, 0);
		res(1, 3) = m(3, 1);
		res(2, 3) = m(3, 2);
		res(3, 3) = m(3, 3);

		return res;
	}

	/**@brief Constructs a 4x4 translation matrix with \a x, \a y, \a z and multiplies it by \a cm.
	* 
	* Returns cm * translate.
	*/
	inline Matrix4x4 Translate(const Matrix4x4& cm, float x, float y, float z)
	{
		//1 0 0 0
		//0 1 0 0
		//0 0 1 0
		//x y z 1

		Matrix4x4 t;
		t(3, 0) = x;
		t(3, 1) = y;
		t(3, 2) = z;

		return cm * t;
	}

	/**@brief Construct a 4x4 scaling matrix with \a x, \a y, \a z and it by the \a cm.
	* 
	*  Returns cm * scale.
	*/
	inline Matrix4x4 Scale(const Matrix4x4& cm, float x, float y, float z)
	{
		//x 0 0 0
		//0 y 0 0
		//0 0 z 0
		//0 0 0 1

		Matrix4x4 s;
		s(0, 0) = x;
		s(1, 1) = y;
		s(2, 2) = z;

		return cm * s;
	}

	/**@brief Construct a 4x4 rotation matrix with \a angle (in degrees) and axis (\a x, \a y, \a z) and post-multiply's it by \a cm.
	* 
	* Returns cm * rotate.
	*/
	inline Matrix4x4 Rotate(const Matrix4x4& cm, float angle, float x, float y, float z)
	{

		//c + (1 - c)x^2	(1 - c)xy + sz	(1 - c)xz - sy	0
		//(1 - c)xy - sz	c + (1 - c)y^2	(1 - c)yz + sx	0
		//(1 - c)xz + sy	(1 - c)yz - sx	c + (1 - c)z^2	0
		//0					0				0				1
		//c = cos(angle)
		//s = sin(angle)

		float c = cos(angle * PI / 180.0f);
		float s = sin(angle * PI / 180.0f);

		Matrix4x4 r;

		//1st row
		r(0, 0) = c + (1.0f - c) * (x * x);
		r(0, 1) = (1.0f - c) * (x * y) + (s * z);
		r(0, 2) = (1.0f - c) * (x * z) - (s * y);

		//2nd row
		r(1, 0) = (1.0f - c) * (x * y) - (s * z);
		r(1, 1) = c + (1.0f - c) * (y * y);
		r(1, 2) = (1.0f - c) * (y * z) + (s * x);

		//3rd row
		r(2, 0) = (1.0f - c) * (x * z) + (s * y);
		r(2, 1) = (1.0f - c) * (y * z) - (s * x);
		r(2, 2) = c + (1.0f - c) * (z * z);

		return cm * r;
	}

	/**@brief Returns the determinant \a m.
	*/
	inline double Det(const Matrix4x4& m)
	{
		//m00m11(m22m33 - m23m32)
		double c1 = (double)m(0, 0) * m(1, 1) * m(2, 2) * m(3, 3) - (double)m(0, 0) * m(1, 1) * m(2, 3) * m(3, 2);

		//m00m12(m23m31 - m21m33)
		double c2 = (double)m(0, 0) * m(1, 2) * m(2, 3) * m(3, 1) - (double)m(0, 0) * m(1, 2) * m(2, 1) * m(3, 3);

		//m00m13(m21m32 - m22m31)
		double c3 = (double)m(0, 0) * m(1, 3) * m(2, 1) * m(3, 2) - (double)m(0, 0) * m(1, 3) * m(2, 2) * m(3, 1);

		//m01m10(m22m33 - m23m32)
		double c4 = (double)m(0, 1) * m(1, 0) * m(2, 2) * m(3, 3) - (double)m(0, 1) * m(1, 0) * m(2, 3) * m(3, 2);

		//m01m12(m23m30 - m20m33)
		double c5 = (double)m(0, 1) * m(1, 2) * m(2, 3) * m(3, 0) - (double)m(0, 1) * m(1, 2) * m(2, 0) * m(3, 3);

		//m01m13(m20m32 - m22m30)
		double c6 = (double)m(0, 1) * m(1, 3) * m(2, 0) * m(3, 2) - (double)m(0, 1) * m(1, 3) * m(2, 2) * m(3, 0);

		//m02m10(m21m33 - m23m31)
		double c7 = (double)m(0, 2) * m(1, 0) * m(2, 1) * m(3, 3) - (double)m(0, 2) * m(1, 0) * m(2, 3) * m(3, 1);

		//m02m11(m23m30 - m20m33)
		double c8 = (double)m(0, 2) * m(1, 1) * m(2, 3) * m(3, 0) - (double)m(0, 2) * m(1, 1) * m(2, 0) * m(3, 3);

		//m02m13(m20m31 - m21m30)
		double c9 = (double)m(0, 2) * m(1, 3) * m(2, 0) * m(3, 1) - (double)m(0, 2) * m(1, 3) * m(2, 1) * m(3, 0);

		//m03m10(m21m32 - m22m21)
		double c10 = (double)m(0, 3) * m(1, 0) * m(2, 1) * m(3, 2) - (double)m(0, 3) * m(1, 0) * m(2, 2) * m(3, 1);

		//m03m11(m22m30 - m20m32)
		double c11 = (double)m(0, 3) * m(1, 1) * m(2, 2) * m(3, 0) - (double)m(0, 3) * m(1, 1) * m(2, 0) * m(3, 2);

		//m03m12(m20m31 - m21m30)
		double c12 = (double)m(0, 3) * m(1, 2) * m(2, 0) * m(3, 1) - (double)m(0, 3) * m(1, 2) * m(2, 1) * m(3, 0);

		return (c1 + c2 + c3) - (c4 + c5 + c6) + (c7 + c8 + c9) - (c10 + c11 + c12);
	}

	/**@brief Returns the cofactor of the \a row and \a col in \a m.
	*/
	inline double Cofactor(const Matrix4x4& m, unsigned int row, unsigned int col)
	{
		//cij = (-1)^i + j * det of minor(i, j);
		double tempMat[3][3]{};
		int tr{ 0 };
		int tc{ 0 };

		//minor(i, j)
		for (int i = 0; i < 4; ++i)
		{
			if (i == row)
				continue;

			for (int j = 0; j < 4; ++j)
			{
				if (j == col)
					continue;

				tempMat[tr][tc] = m(i, j);
				++tc;

			}
			tc = 0;
			++tr;
		}

		//determinant of minor(i, j)
		double det3x3 = (tempMat[0][0] * tempMat[1][1] * tempMat[2][2]) + (tempMat[0][1] * tempMat[1][2] * tempMat[2][0]) +
			(tempMat[0][2] * tempMat[1][0] * tempMat[2][1]) - (tempMat[0][2] * tempMat[1][1] * tempMat[2][0]) -
			(tempMat[0][1] * tempMat[1][0] * tempMat[2][2]) - (tempMat[0][0] * tempMat[1][2] * tempMat[2][1]);

		return pow(-1, row + col) * det3x3;
	}

	/**@brief Returns the adjoint of \a m.
	*/
	inline Matrix4x4 Adjoint(const Matrix4x4& m)
	{
		//Cofactor of each ijth position put into matrix cA.
		//Adjoint is the tranposed matrix of cA.
		Matrix4x4 cA;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				cA(i, j) = static_cast<float>(Cofactor(m, i, j));
			}
		}

		return Transpose(cA);
	}

	/**@brief Returns the inverse of \a m.
	* 
	* If \a m is noninvertible/singular, the identity matrix is returned.
	*/
	inline Matrix4x4 Inverse(const Matrix4x4& m)
	{
		//Inverse of m = adjoint of m / det of m
		double determinant = Det(m);
		if (CompareDoubles(determinant, 0.0, EPSILON))
			return Matrix4x4();

		return Adjoint(m) * (1.0f / static_cast<float>(determinant));
	}


#if defined(_DEBUG)
	inline void print(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				std::cout << m(i, j) << " ";
			}

			std::cout << std::endl;
		}
	}
#endif

//------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------

	/** @class Matrix4x4 ""
	*	@brief A matrix class used for 4x4 matrices and their manipulations.
	*
	*	The datatype for the components is float.\n
	*	The 4x4 matrix is treated as a row-major matrix.
	*
	*/

	/** @class Quaternion ""
	*	@brief A quaternion class used for quaternions and their manipulations.
	* 
	*	The datatype for the components is float.
	*/
	class Quaternion
	{
	public:
		/**@brief Constructs a quaternion with the specified values.
		* 
		* If no values are specified the identity quaternion is constructed.
		*/
		Quaternion(float scalar = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f);

		/**@brief Constructs a quaternion with the specified values.
		*/
		Quaternion(float scalar, const Vector3D& v);

		/**@brief Constructs a quaternion with the given values in the 4D vector \a v.
		* 
		*	The x value in the 4D vector should be the scalar.
		*	The y, z and w value in the 4D vector should be the axis.
		*/
		Quaternion(const Vector4D& v);

		/**@brief Returns the scalar component of the quaternion.
		*/
		float GetScalar() const;

		/**@brief Returns the x value of the vector component in the quaternion.
		*/
		float GetX() const;

		/**@brief Returns the y value of the vector component in the quaternion.
		*/
		float GetY() const;

		/**@brief Returns the z value of the vector component in the quaternion.
		*/
		float GetZ() const;

		/**@brief Returns the vector component of the quaternion.
		*/
		const Vector3D& GetVector() const;

		/**@brief Sets the scalar component to the specified value.
		*/
		void SetScalar(float scalar);

		/**@brief Sets the x component to the specified value.
		*/
		void SetX(float x);

		/**@brief Sets the y component to the specified value.
		*/
		void SetY(float y);

		/**@brief Sets the z component to the specified value.
		*/
		void SetZ(float z);

		/**@brief Sets the vector to the specified vector.
		*/
		void SetVector(const Vector3D& v);

		/**@brief Adds this quaternion to /a q and stores the result in this quaternion.
		*/
		Quaternion& operator+=(const Quaternion& q);

		/**@brief Subtracts the quaternion \a q from this and stores the result in this quaternion.
		*/
		Quaternion& operator-=(const Quaternion& q);

		/**@brief Multiplies this quaternion by \a k and stores the result in this quaternion.
		*/
		Quaternion& operator*=(float k);

		/**@brief Multiplies this quaternion by \a q and stores the result in this quaternion.
		*/
		Quaternion& operator*=(const Quaternion& q);

	private:
		float mScalar;
		float mX;
		float mY;
		float mZ;
	};

	//-------------------------------------------------------------------------------------
	inline Quaternion::Quaternion(float scalar, float x, float y, float z) : 
		mScalar{ scalar }, mX{ x }, mY{ y }, mZ{ z }
	{
	}

	inline Quaternion::Quaternion(float scalar, const Vector3D& v) : 
		mScalar{ scalar }, mX{ v.GetX() }, mY{ v.GetY() }, mZ{ v.GetZ() }
	{
	}

	inline Quaternion::Quaternion(const Vector4D& v) : 
		mScalar{ v.GetX() }, mX{ v.GetY() }, mY{ v.GetZ() }, mZ{ v.GetW() }
	{
	}

	inline float Quaternion::GetScalar() const
	{
		return mScalar;
	}

	inline float Quaternion::GetX() const
	{
		return mX;
	}

	inline float Quaternion::GetY() const
	{
		return mY;
	}

	inline float Quaternion::GetZ() const
	{
		return mZ;
	}

	inline const Vector3D& Quaternion::GetVector() const
	{
		return Vector3D(mX, mY, mZ);
	}

	inline void Quaternion::SetScalar(float scalar)
	{
		mScalar = scalar;
	}

	inline void Quaternion::SetX(float x)
	{
		mX = x;
	}

	inline void Quaternion::SetY(float y)
	{
		mY = y;
	}

	inline void  Quaternion::SetZ(float z)
	{
		mZ = z;
	}

	inline void Quaternion::SetVector(const Vector3D& v)
	{
		mX = v.GetX();
		mY = v.GetY();
		mZ = v.GetZ();
	}

	inline Quaternion& Quaternion::operator+=(const Quaternion& q)
	{
		this->mScalar += q.mScalar;
		this->mX += q.mX;
		this->mY += q.mY;
		this->mZ += q.mZ;

		return *this;
	}

	inline Quaternion& Quaternion::operator-=(const Quaternion& q)
	{
		this->mScalar -= q.mScalar;
		this->mX -= q.mX;
		this->mY -= q.mY;
		this->mZ -= q.mZ;

		return *this;
	}

	inline Quaternion& Quaternion::operator*=(float k)
	{
		this->mScalar *= k;
		this->mX *= k;
		this->mY *= k;
		this->mZ *= k;

		return *this;
	}

	inline Quaternion& Quaternion::operator*=(const Quaternion& q)
	{
		Vector3D thisVector(this->mX, this->mY, this->mZ);
		Vector3D qVector(q.mX, q.mY, q.mZ);

		float s{ this->mScalar * q.mScalar };
		float dP{ DotProduct(thisVector, qVector) };
		float resultScalar{ s - dP };

		Vector3D a(this->mScalar * qVector);
		Vector3D b(q.mScalar * thisVector);
		Vector3D cP(CrossProduct(thisVector, qVector));
		Vector3D resultVector(a + b + cP);

		this->mScalar = resultScalar;
		this->mX = resultVector.GetX();
		this->mY = resultVector.GetY();
		this->mZ = resultVector.GetZ();

		return *this;
	}

	/**@brief Returns a quaternion that has the result of \a q1 + \a q2.
	*/
	inline Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion(q1.GetScalar() + q2.GetScalar(), q1.GetX() + q2.GetX(), q1.GetY() + q2.GetY(), q1.GetZ() + q2.GetZ());
	}

	/**@brief Returns a quaternion that has the result of \a -q.
	*/
	inline Quaternion operator-(const Quaternion& q)
	{
		return Quaternion(-q.GetScalar(), -q.GetX(), -q.GetY(), -q.GetZ());
	}

	/**@brief Returns a quaternion that has the result of \a q1 - \a q2.
	*/
	inline Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion(q1.GetScalar() - q2.GetScalar(), 
			q1.GetX() - q2.GetX(), q1.GetY() - q2.GetY(), q1.GetZ() - q2.GetZ());
	}

	/**@brief Returns a quaternion that has the result of \a k * \a q.
	*/
	inline Quaternion operator*(float k, const Quaternion& q)
	{
		return Quaternion(k * q.GetScalar(), k * q.GetX(), k * q.GetY(), k * q.GetZ());
	}

	/**@brief Returns a quaternion that has the result of \a q * \a k.
	*/
	inline Quaternion operator*(const Quaternion& q, float k)
	{
		return Quaternion(q.GetScalar() * k, q.GetX() * k, q.GetY() * k, q.GetZ() * k);
	}

	/**@brief Returns a quaternion that has the result of \a q1 * \a q2.
	*/
	inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
	{
		//scalar part = q1scalar * q2scalar - q1Vector dot q2Vector
		//vector part = q1Scalar * q2Vector + q2Scalar * q1Vector + q1Vector cross q2Vector

		Vector3D q1Vector(q1.GetX(), q1.GetY(), q1.GetZ());
		Vector3D q2Vector(q2.GetX(), q2.GetY(), q2.GetZ());

		float s{ q1.GetScalar() * q2.GetScalar() };
		float dP{ DotProduct(q1Vector, q2Vector) };
		float resultScalar{ s - dP };

		Vector3D a(q1.GetScalar() * q2Vector);
		Vector3D b(q2.GetScalar() * q1Vector);
		Vector3D cP(CrossProduct(q1Vector, q2Vector));
		Vector3D resultVector(a + b + cP);

		return Quaternion(resultScalar, resultVector);
	}

	/**@brief Returns true if quaternion \a q is a zero quaternion, false otherwise.
	*/
	inline bool IsZeroQuaternion(const Quaternion& q)
	{
		//zero quaternion = (0, 0, 0, 0)
		return CompareFloats(q.GetScalar(), 0.0f, EPSILON) && CompareFloats(q.GetX(), 0.0f, EPSILON) &&
			CompareFloats(q.GetY(), 0.0f, EPSILON) && CompareFloats(q.GetZ(), 0.0f, EPSILON);
	}

	/**@brief Returns true if quaternion \a q is an identity quaternion, false otherwise.
	*/
	inline bool IsIdentity(const Quaternion& q)
	{
		//identity quaternion = (1, 0, 0, 0)
		return CompareFloats(q.GetScalar(), 1.0f, EPSILON) && CompareFloats(q.GetX(), 0.0f, EPSILON) &&
			CompareFloats(q.GetY(), 0.0f, EPSILON) && CompareFloats(q.GetZ(), 0.0f, EPSILON);
	}

	/**@brief Returns the conjugate of quaternion \a q.
	*/
	inline Quaternion Conjugate(const Quaternion& q)
	{
		//conjugate of a quaternion is the quaternion with its vector part negated
		return Quaternion(q.GetScalar(), -q.GetX(), -q.GetY(), -q.GetZ());
	}

	/**@brief Returns the length of quaternion \a q.
	*/
	inline float Length(const Quaternion& q)
	{
		//length of a quaternion = sqrt(scalar^2 + x^2 + y^2 + z^2)
		return sqrt(q.GetScalar() * q.GetScalar() + q.GetX() * q.GetX() + q.GetY() * q.GetY() + q.GetZ() * q.GetZ());
	}

	/**@brief Normalizes \a q and returns the normalized quaternion.
	* 
	* If \a q is the zero quaternion then \a q is returned.
	*/
	inline Quaternion Normalize(const Quaternion& q)
	{
		//to normalize a quaternion you do q / |q|

		if (IsZeroQuaternion(q))
			return q;

		float d{ Length(q) };

		return Quaternion(q.GetScalar() / d, q.GetX() / d, q.GetY() / d, q.GetZ() / d);
	}

	/**@brief Returns the invese of \a q.
	* 
	*  If \a q is the zero quaternion then \a q is returned.
	*/
	inline Quaternion Inverse(const Quaternion& q)
	{
		//inverse = conjugate of q / |q|^2

		if (IsZeroQuaternion(q))
			return q;

		Quaternion conjugateOfQ(Conjugate(q));

		float d{ Length(q) };
		d *= d;

		return Quaternion(conjugateOfQ.GetScalar() / d, conjugateOfQ.GetX() / d, 
			conjugateOfQ.GetY() / d, conjugateOfQ.GetZ() / d);
	}

	/**@brief Returns a rotation quaternion from the axis-angle rotation representation.
	* 
	*  The \a angle should be given in degrees.
	*/
	inline Quaternion RotationQuaternion(float angle, float x, float y, float z)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		float ang{ angle / 2.0f };
		float c{ cos(ang * PI / 180.0f) };
		float s{ sin(ang * PI / 180.0f) };

		Vector3D axis(x, y, z);
		axis = Norm(axis);

		return Quaternion(c, s * axis.GetX(), s * axis.GetY(), s * axis.GetZ());
	}

	/**@brief Returns a quaternion from the axis-angle rotation representation.
	* 
	* The \a angle should be given in degrees.
	*/
	inline Quaternion RotationQuaternion(float angle, const Vector3D& axis)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		float ang{ angle / 2.0f };
		float c{ cos(ang * PI / 180.0f) };
		float s{ sin(ang * PI / 180.0f) };

		Vector3D axisN(Norm(axis));

		return Quaternion(c, s * axisN.GetX(), s * axisN.GetY(), s * axisN.GetZ());
	}

	/**@brief Returns a quaternion from the axis-angle rotation representation.
	* 
	* The x value in the 4D vector \a v should be the angle(in degrees).\n
	* The y, z and w value in the 4D vector \a v should be the axis.
	*/
	inline Quaternion RotationQuaternion(const Vector4D& angAxis)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		float angle{ angAxis.GetX() / 2.0f };
		float c{ cos(angle * PI / 180.0f) };
		float s{ sin(angle * PI / 180.0f) };

		Vector3D axis(angAxis.GetY(), angAxis.GetZ(), angAxis.GetW());
		axis = Norm(axis);

		return Quaternion(c, s * axis.GetX(), s * axis.GetY(), s * axis.GetZ());
	}

	/**@brief Transforms \a q into a column-major matrix.
	* 
	* \a q should be a unit quaternion.
	*/
	inline Matrix4x4 QuaternionToRotationMatrixCol(const Quaternion& q)
	{
		//1 - 2q3^2 - 2q4^2		2q2q3 - 2q1q4		2q2q4 + 2q1q3		0
		//2q2q3 + 2q1q4			1 - 2q2^2 - 2q4^2	2q3q4 - 2q1q2		0
		//2q2q4 - 2q1q3			2q3q4 + 2q1q2		1 - 2q2^2 - 2q3^2	0
		//0						0					0					1
		//q1 = scalar
		//q2 = x
		//q3 = y
		//q4 = z

		float colMat[4][4] = {};

		colMat[0][0] = 1.0f - 2.0f * q.GetY() * q.GetY() - 2.0f * q.GetZ() * q.GetZ();
		colMat[0][1] = 2.0f * q.GetX() * q.GetY() - 2.0f * q.GetScalar() * q.GetZ();
		colMat[0][2] = 2.0f * q.GetX() * q.GetZ() + 2.0f * q.GetScalar() * q.GetY();
		colMat[0][3] = 0.0f;

		colMat[1][0] = 2.0f * q.GetX() * q.GetY() + 2.0f * q.GetScalar() * q.GetZ();
		colMat[1][1] = 1.0f - 2.0f * q.GetX() * q.GetX() - 2.0f * q.GetZ() * q.GetZ();
		colMat[1][2] = 2.0f * q.GetY() * q.GetZ() - 2.0f * q.GetScalar() * q.GetX();
		colMat[1][3] = 0.0f;

		colMat[2][0] = 2.0f * q.GetX() * q.GetZ() - 2.0f * q.GetScalar() * q.GetY();
		colMat[2][1] = 2.0f * q.GetY() * q.GetZ() + 2.0f * q.GetScalar() * q.GetX();
		colMat[2][2] = 1.0f - 2.0f * q.GetX() * q.GetX() - 2.0f * q.GetY() * q.GetY();
		colMat[2][3] = 0.0f;

		colMat[3][0] = 0.0f;
		colMat[3][1] = 0.0f;
		colMat[3][2] = 0.0f;
		colMat[3][3] = 1.0f;

		return Matrix4x4(colMat);
	}

	/**@brief Transforms \a q into a row-major matrix.
	* 
	* \a q should be a unit quaternion.
	*/
	inline Matrix4x4 QuaternionToRotationMatrixRow(const Quaternion& q)
	{
		//1 - 2q3^2 - 2q4^2		2q2q3 + 2q1q4		2q2q4 - 2q1q3		0
		//2q2q3 - 2q1q4			1 - 2q2^2 - 2q4^2	2q3q4 + 2q1q2		0
		//2q2q4 + 2q1q3			2q3q4 - 2q1q2		1 - 2q2^2 - 2q3^2	0
		//0						0					0					1
		//q1 = scalar
		//q2 = x
		//q3 = y
		//q4 = z

		float rowMat[4][4] = {};

		rowMat[0][0] = 1.0f - 2.0f * q.GetY() * q.GetY() - 2.0f * q.GetZ() * q.GetZ();
		rowMat[0][1] = 2.0f * q.GetX() * q.GetY() + 2.0f * q.GetScalar() * q.GetZ();
		rowMat[0][2] = 2.0f * q.GetX() * q.GetZ() - 2.0f * q.GetScalar() * q.GetY();
		rowMat[0][3] = 0.0f;

		rowMat[1][0] = 2.0f * q.GetX() * q.GetY() - 2.0f * q.GetScalar() * q.GetZ();
		rowMat[1][1] = 1.0f - 2.0f * q.GetX() * q.GetX() - 2.0f * q.GetZ() * q.GetZ();
		rowMat[1][2] = 2.0f * q.GetY() * q.GetZ() + 2.0f * q.GetScalar() * q.GetX();
		rowMat[1][3] = 0.0f;

		rowMat[2][0] = 2.0f * q.GetX() * q.GetZ() + 2.0f * q.GetScalar() * q.GetY();
		rowMat[2][1] = 2.0f * q.GetY() * q.GetZ() - 2.0f * q.GetScalar() * q.GetX();
		rowMat[2][2] = 1.0f - 2.0f * q.GetX() * q.GetX() - 2.0f * q.GetY() * q.GetY();
		rowMat[2][3] = 0.0f;

		rowMat[3][0] = 0.0f;
		rowMat[3][1] = 0.0f;
		rowMat[3][2] = 0.0f;
		rowMat[3][3] = 1.0f;

		return Matrix4x4(rowMat);
	}

#if defined(_DEBUG)
	inline void print(const Quaternion& q)
	{
		std::cout << "(" << q.GetScalar() << ", " << q.GetX() << ", " << q.GetY() << ", " << q.GetZ();
	}
#endif
	//-------------------------------------------------------------------------------------

	inline Vector2D::Vector2D(const Vector3D& v) : mX{ v.GetX() }, mY{ v.GetY() }
	{}

	inline Vector2D::Vector2D(const Vector4D& v) : mX{ v.GetX() }, mY{ v.GetY() }
	{}

	inline Vector3D::Vector3D(const Vector2D& v, float z) : mX{ v.GetX() }, mY{ v.GetY() }, mZ{ z }
	{}

	inline Vector3D::Vector3D(const Vector4D& v) : mX{ v.GetX() }, mY{ v.GetY() }, mZ{ v.GetZ() }
	{}

	inline Vector4D::Vector4D(const Vector2D& v, float z, float w) : mX{ v.GetX() }, mY{ v.GetY() }, mZ{ z }, mW{ w }
	{}

	inline Vector4D::Vector4D(const Vector3D& v, float w) : mX{ v.GetX() }, mY{ v.GetY() }, mZ{ v.GetZ() }, mW{ w }
	{}

	inline Vector2D& Vector2D::operator=(const Vector3D& v)
	{
		mX = v.GetX();
		mY = v.GetY();
	}

	inline Vector2D& Vector2D::operator=(const Vector4D& v)
	{
		mX = v.GetX();
		mY = v.GetY();
	}

	inline Vector3D& Vector3D::operator=(const Vector2D& v)
	{
		mX = v.GetX();
		mY = v.GetY();
		mZ = 0.0f;
	}

	inline Vector3D& Vector3D::operator=(const Vector4D& v)
	{
		mX = v.GetX();
		mY = v.GetY();
		mZ = v.GetZ();
	}

	inline Vector4D& Vector4D::operator=(const Vector2D& v)
	{
		mX = v.GetX();
		mY = v.GetY();
		mZ = 0.0f;
		mW = 0.0f;
	}

	inline Vector4D& Vector4D::operator=(const Vector3D& v)
	{
		mX = v.GetX();
		mY = v.GetY();
		mZ = v.GetZ();
		mW = 0.0f;
	}
//------------------------------------------------------------------------------------------------------------------------------------
}