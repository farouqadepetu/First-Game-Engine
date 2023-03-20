#pragma once

#include <cmath>

#if defined(_DEBUG)
#include <iostream>
#endif


#define EPSILON 1e-6f
#define PI 3.14159265

/** @namespace FAMath
*	@brief Has utility functions, Vector2D, Vector3D, Vector4D, Matrix4x4, and Quaternion classes.
*/
namespace FAMath
{
	class Vector2D;
	class Vector3D;
	class Vector4D;

	/*@brief Checks if the two specified floats are equal using exact epsilion and adaptive epsilion.
	*/
	inline bool compareFloats(float x, float y, float epsilon)
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

	/*@brief Checks if the two specified doubles are equal using exact epsilion and adaptive epsilion.
	*/
	inline bool compareDoubles(double x, double y, double epsilon)
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


	//--------------------------------------------------------------------------------------
	//Vector2D Constructors

	inline Vector2D::Vector2D() : m_x{ 0.0f }, m_y{ 0.0f }
	{}

	inline Vector2D::Vector2D(float x, float y) : m_x{ x }, m_y{ y }
	{}

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector2D Getters and Setters

	inline float& Vector2D::x()
	{
		return m_x;
	}

	inline float& Vector2D::y()
	{
		return m_y;
	}

	inline const float& Vector2D::x() const
	{
		return m_x;
	}

	inline const float& Vector2D::y() const
	{
		return m_y;
	}

	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector2D Memeber functions

	inline Vector2D& Vector2D::operator+=(const Vector2D& b)
	{
		this->m_x += (double)b.m_x;
		this->m_y += (double)b.m_y;

		return *this;
	}

	inline Vector2D& Vector2D::operator-=(const Vector2D& b)
	{
		this->m_x -= (double)b.m_x;
		this->m_y -= (double)b.m_y;

		return *this;
	}

	inline Vector2D& Vector2D::operator*=(const float& k)
	{
		this->m_x *= (double)k;
		this->m_y *= (double)k;

		return *this;
	}

	inline Vector2D& Vector2D::operator/=(const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return *this;
		}

		this->m_x /= (double)k;
		this->m_y /= (double)k;

		return *this;
	}

	//-------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------
	//Vector2D Non-member functions

	/**@brief Returns true if a is the zero vector.
	*/
	inline bool zeroVector(const Vector2D& a)
	{
		if (compareFloats(a.x(), 0.0f, EPSILON) && compareFloats(a.y(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	/**@brief 2D vector addition.
	*/
	inline Vector2D operator+(const Vector2D& a, const Vector2D& b)
	{
		return Vector2D((double)a.x() + b.x(), (double)a.y() + b.y());
	}

	/**@brief 2D vector negation.
	*/
	inline Vector2D operator-(const Vector2D& v)
	{
		return Vector2D(-v.x(), -v.y());
	}

	/**@brief 2D vector subtraction.
	*/
	inline Vector2D operator-(const Vector2D& a, const Vector2D& b)
	{
		return Vector2D((double)a.x() - b.x(), (double)a.y() - b.y());
	}

	/**@brief 2D vector scalar multiplication.
	* Returns a * k, where a is a vector and k is a scalar(float)
	*/
	inline Vector2D operator*(const Vector2D& a, const float& k)
	{
		return Vector2D((double)a.x() * k, (double)a.y() * k);
	}

	/**@brief 2D vector scalar multiplication.
	* Returns k * a,  where a is a vector and k is a scalar(float)
	*/
	inline Vector2D operator*(const float& k, const Vector2D& a)
	{
		return Vector2D((double)k * a.x(), (double)k * a.y());
	}

	/**@brief 2D vector scalar division.
	* Returns a / k,  where a is a vector and k is a scalar(float)
	* If k = 0 the returned vector is the zero vector.
	*/
	inline Vector2D operator/(const Vector2D& a, const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return Vector2D();
		}

		return Vector2D(a.x() / (double)k, a.y() / (double)k);
	}

	/**@brief Returns the dot product between two 2D vectors.
	*/
	inline float dotProduct(const Vector2D& a, const Vector2D& b)
	{
		return (double)a.x() * b.x() + (double)a.y() * b.y();
	}

	/**@brief Returns the length(magnitude) of the 2D vector v.
	*/
	inline float length(const Vector2D& v)
	{
		return sqrt((double)v.x() * v.x() + (double)v.y() * v.y());
	}

	/**@brief Normalizes the 2D vector v.
	* If the 2D vector is the zero vector v is returned.
	*/
	inline Vector2D norm(const Vector2D& v)
	{
		//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
		
		//v is the zero vector
		if (zeroVector(v))
		{
			return v;
		}

		double mag = length(v);

		return Vector2D(v.x() / mag, v.y() / mag);
	}

	/**@brief Converts the 2D vector v from polar coordinates to cartesian coordinates.
	* v should = (r, theta(degrees))
	* The returned 2D vector = (x, y)
	*/
	inline Vector2D PolarToCartesian(const Vector2D& v)
	{
		//v = (r, theta)
		//x = rcos((theta)
		//y = rsin(theta)
		float angle = v.y() * PI / 180.0f;

		return Vector2D(v.x() * cos(angle), v.x() * sin(angle));
	}

	/**@brief Converts the 2D vector v from cartesian coordinates to polar coordinates.
	* v should = (x, y, z)
	* If vx is zero then no conversion happens and v is returned.\n
	* The returned 2D vector = (r, theta(degrees)).
	*/
	inline Vector2D CartesianToPolar(const Vector2D& v)
	{
		//v = (x, y)
		//r = sqrt(vx^2 + vy^2)
		//theta = arctan(y / x)

		if (compareFloats(v.x(), 0.0f, EPSILON))
		{
			return v;
		}

		double theta{ atan2(v.y(), v.x()) * 180.0 / PI };
		return Vector2D(length(v), theta);
	}

	/**@brief Returns a 2D vector that is the projection of a onto b.
	* If b is the zero vector a is returned.
	*/
	inline Vector2D Projection(const Vector2D& a, const Vector2D& b)
	{
		//Projb(a) = (a dot b)b
		//normalize b before projecting

		Vector2D normB(norm(b));
		return Vector2D(dotProduct(a, normB) * normB);
	}


#if defined(_DEBUG)
	inline void print(const Vector2D& v)
	{
		std::cout << "(" << v.x() << ", " << v.y() << ")";
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

	//--------------------------------------------------------------------------------------
	//Vector3D Constructors

	inline Vector3D::Vector3D() : m_x{ 0.0f }, m_y{ 0.0f }, m_z{ 0.0f }
	{}

	inline Vector3D::Vector3D(float x, float y, float z) : m_x{ x }, m_y{ y }, m_z{ z }
	{}

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector3D Getters and Setters

	inline float& Vector3D::x()
	{
		return m_x;
	}

	inline float& Vector3D::y()
	{
		return m_y;
	}

	inline float& Vector3D::z()
	{
		return m_z;
	}

	inline const float& Vector3D::x() const
	{
		return m_x;
	}

	inline const float& Vector3D::y() const
	{
		return m_y;
	}

	inline const float& Vector3D::z() const
	{
		return m_z;
	}
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector3D Memeber functions

	inline Vector3D& Vector3D::operator+=(const Vector3D& b)
	{
		this->m_x += (double)b.m_x;
		this->m_y += (double)b.m_y;
		this->m_z += (double)b.m_z;

		return *this;
	}

	inline Vector3D& Vector3D::operator-=(const Vector3D& b)
	{
		this->m_x -= (double)b.m_x;
		this->m_y -= (double)b.m_y;
		this->m_z -= (double)b.m_z;

		return *this;
	}

	inline Vector3D& Vector3D::operator*=(const float& k)
	{
		this->m_x *= (double)k;
		this->m_y *= (double)k;
		this->m_z *= (double)k;

		return *this;
	}

	inline Vector3D& Vector3D::operator/=(const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return *this;
		}

		this->m_x /= (double)k;
		this->m_y /= (double)k;
		this->m_z /= (double)k;

		return *this;
	}

	//-------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------
	//Vector3D Non-member functions

	/**@brief Returns true if a is the zero vector.
	*/
	inline bool zeroVector(const Vector3D& a)
	{
		if (compareFloats(a.x(), 0.0f, EPSILON) && compareFloats(a.y(), 0.0f, EPSILON) &&
			compareFloats(a.z(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	/**@brief 3D vector addition.
	*/
	inline Vector3D operator+(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D((double)a.x() + b.x(), (double)a.y() + b.y(), (double)a.z() + b.z());
	}

	/**@brief 3D vector negeation.
	*/
	inline Vector3D operator-(const Vector3D& v)
	{
		return Vector3D(-v.x(), -v.y(), -v.z());
	}

	/**@brief 3D vector subtraction.
	*/
	inline Vector3D operator-(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
	}

	/**@brief 3D vector scalar multiplication.
	* Returns a * k, where a is a vector and k is a scalar(float)
	*/
	inline Vector3D operator*(const Vector3D& a, const float& k)
	{
		return Vector3D(a.x() * (double)k, a.y() * (double)k, a.z() * (double)k);
	}

	/**@brief 3D vector scalar multiplication.
	* Returns k * a,  where a is a vector and k is a scalar(float)
	*/
	inline Vector3D operator*(const float& k, const Vector3D& a)
	{
		return Vector3D((double)k * a.x(), (double)k * a.y(), (double)k * a.z());
	}

	/**@brief 3D vector scalar division.
	* Returns a / k,  where a is a vector and k is a scalar(float)
	* If k = 0 the returned vector is the zero vector.
	*/
	inline Vector3D operator/(const Vector3D& a, const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return Vector3D();
		}

		return Vector3D(a.x() / (double)k, a.y() / (double)k, a.z() / (double)k);
	}

	/**@brief Returns the dot product between two 3D vectors.
	*/
	inline float dotProduct(const Vector3D& a, const Vector3D& b)
	{
		//a dot b = axbx + ayby + azbz
		return (double)a.x() * b.x() + (double)a.y() * b.y() + (double)a.z() * b.z();
	}

	/**@brief Returns the cross product between two 3D vectors.
	*/
	inline Vector3D crossProduct(const Vector3D& a, const Vector3D& b)
	{
		//a x b = (aybz - azby, azbx - axbz, axby - aybx)

		return Vector3D((double)a.y() * b.z() - (double)a.z() * b.y(),
			(double)a.z() * b.x() - (double)a.x() * b.z(),
			(double)a.x() * b.y() - (double)a.y() * b.x());
	}

	/**@brief Returns the length(magnitude) of the 3D vector v.
	*/
	inline float length(const Vector3D& v)
	{
		//length(v) = sqrt(vx^2 + vy^2 + vz^2)

		return sqrt((double)v.x() * v.x() + (double)v.y() * v.y() + (double)v.z() * v.z());
	}

	/**@brief Normalizes the 3D vector v.
	* If the 3D vector is the zero vector v is returned.
	*/
	inline Vector3D norm(const Vector3D& v)
	{
		//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
		//v is the zero vector
		if (zeroVector(v))
		{
			return v;
		}

		double mag = length(v);

		return Vector3D(v.x() / mag, v.y() / mag, v.z() / mag);
	}

	/**@brief Converts the 3D vector v from cylindrical coordinates to cartesian coordinates.
	* v should = (r, theta(degrees), z).\n
	* The returned 3D vector = (x, y ,z).
	*/
	inline Vector3D CylindricalToCartesian(const Vector3D& v)
	{
		//v = (r, theta, z)
		//x = rcos(theta)
		//y = rsin(theta)
		//z = z
		double angle = v.y() * PI / 180.0;

		return Vector3D(v.x() * cos(angle), v.x() * sin(angle), v.z());
	}

	/**@brief Converts the 3D vector v from cartesian coordinates to cylindrical coordinates.
	* v should = (x, y, z).\n
	* If vx is zero then no conversion happens and v is returned.\n
	* The returned 3D vector = (r, theta(degrees), z).
	*/
	inline Vector3D CartesianToCylindrical(const Vector3D& v)
	{
		//v = (x, y, z)
		//r = sqrt(vx^2 + vy^2 + vz^2)
		//theta = arctan(y / x)
		//z = z
		if (compareFloats(v.x(), 0.0f, EPSILON))
		{
			return v;
		}

		double theta{ atan2(v.y(), v.x()) * 180.0 / PI };
		return Vector3D(length(v), theta, v.z());
	}

	/**@brief Converts the 3D vector v from spherical coordinates to cartesian coordinates.
	* v should = (pho, phi(degrees), theta(degrees)).\n
	* The returned 3D vector = (x, y, z)
	*/
	inline Vector3D SphericalToCartesian(const Vector3D& v)
	{
		// v = (pho, phi, theta)
		//x = pho * sin(phi) * cos(theta)
		//y = pho * sin(phi) * sin(theta)
		//z = pho * cos(theta);

		double phi{ v.y() * PI / 180.0 };
		double theta{ v.z() * PI / 180.0 };

		return Vector3D(v.x() * sin(phi) * cos(theta), v.x() * sin(phi) * sin(theta), v.x() * cos(theta));
	}

	/**@brief Converts the 3D vector v from cartesian coordinates to spherical coordinates.
	* If v is the zero vector or if vx is zero then no conversion happens and v is returned.\n
	* The returned 3D vector = (r, phi(degrees), theta(degrees)).
	*/
	inline Vector3D CartesianToSpherical(const Vector3D& v)
	{
		//v = (x, y ,z)
		//pho = sqrt(vx^2 + vy^2 + vz^2)
		//phi = arcos(z / pho)
		//theta = arctan(y / x)

		if (compareFloats(v.x(), 0.0f, EPSILON) || zeroVector(v))
		{
			return v;
		}

		double pho{ length(v) };
		double phi{ acos(v.z() / pho) * 180.0 / PI };
		double theta{ atan2(v.y(), v.x()) * 180.0 / PI };

		return Vector3D(pho, phi, theta);
	}

	/**@brief Returns a 3D vector that is the projection of a onto b.
	* If b is the zero vector a is returned.
	*/
	inline Vector3D Projection(const Vector3D& a, const Vector3D& b)
	{
		//Projb(a) = (a dot b)b
		//normalize b before projecting

		Vector3D normB(norm(b));
		return Vector3D(dotProduct(a, normB) * normB);
	}


#if defined(_DEBUG)
	inline void print(const Vector3D& v)
	{
		std::cout << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
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

	//--------------------------------------------------------------------------------------
	//Vector4D Constructors

	inline Vector4D::Vector4D() : m_x{ 0.0f }, m_y{ 0.0f }, m_z{ 0.0f }, m_w{ 0.0f }
	{}

	inline Vector4D::Vector4D(float x, float y, float z, float w) : m_x{ x }, m_y{ y }, m_z{ z }, m_w{ w }
	{}

	inline Vector4D::Vector4D(Vector2D v, float z, float w) : m_x{ v.x() }, m_y{ v.y() }, m_z{ z }, m_w{ w }
	{}

	inline Vector4D::Vector4D(Vector3D v, float w) : m_x{ v.x() }, m_y{ v.y() }, m_z{ v.z() }, m_w{ w }
	{}

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector4D Getters and Setters

	inline float& Vector4D::x()
	{
		return m_x;
	}

	inline float& Vector4D::y()
	{
		return m_y;
	}

	inline float& Vector4D::z()
	{
		return m_z;
	}

	inline float& Vector4D::w()
	{
		return m_w;
	}

	inline const float& Vector4D::x() const
	{
		return m_x;
	}

	inline const float& Vector4D::y() const
	{
		return m_y;
	}

	inline const float& Vector4D::z() const
	{
		return m_z;
	}

	inline const float& Vector4D::w() const
	{
		return m_w;
	}
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector4D Memeber functions

	inline Vector4D& Vector4D::operator+=(const Vector4D& b)
	{
		this->m_x += (double)b.m_x;
		this->m_y += (double)b.m_y;
		this->m_z += (double)b.m_z;
		this->m_w += (double)b.m_w;

		return *this;
	}

	inline Vector4D& Vector4D::operator-=(const Vector4D& b)
	{
		this->m_x -= (double)b.m_x;
		this->m_y -= (double)b.m_y;
		this->m_z -= (double)b.m_z;
		this->m_w -= (double)b.m_w;

		return *this;
	}

	inline Vector4D& Vector4D::operator*=(const float& k)
	{
		this->m_x *= (double)k;
		this->m_y *= (double)k;
		this->m_z *= (double)k;
		this->m_w *= (double)k;

		return *this;
	}

	inline Vector4D& Vector4D::operator/=(const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return *this;
		}

		this->m_x /= (double)k;
		this->m_y /= (double)k;
		this->m_z /= (double)k;
		this->m_w /= (double)k;

		return *this;
	}

	//-------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------
	//Vector4D Non-member functions

	/**@brief Returns true if a is the zero vector.
	*/
	inline bool zeroVector(const Vector4D& a)
	{
		if (compareFloats(a.x(), 0.0f, EPSILON) && compareFloats(a.y(), 0.0f, EPSILON) &&
			compareFloats(a.z(), 0.0f, EPSILON) && compareFloats(a.w(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	/**@brief 4D vector addition.
	*/
	inline Vector4D operator+(const Vector4D& a, const Vector4D& b)
	{
		return Vector4D((double)a.x() + b.x(), (double)a.y() + b.y(), (double)a.z() + b.z(), (double)a.w() + b.w());
	}

	/**@brief 4D vector negation.
	*/
	inline Vector4D operator-(const Vector4D& v)
	{
		return Vector4D(-v.x(), -v.y(), -v.z(), -v.w());
	}

	/**@brief 4D vector subtraction.
	*/
	inline Vector4D operator-(const Vector4D& a, const Vector4D& b)
	{
		return Vector4D((double)a.x() - b.x(), (double)a.y() - b.y(), (double)a.z() - b.z(), (double)a.w() - b.w());
	}

	/**@brief 4D vector scalar multiplication.
	* Returns a * k, where a is a vector and k is a scalar(float)
	*/
	inline Vector4D operator*(const Vector4D& a, const float& k)
	{
		return Vector4D(a.x() * (double)k, a.y() * (double)k, a.z() * (double)k, a.w() * (double)k);
	}

	/**@brief 4D vector scalar multiplication.
	* Returns k * a,  where a is a vector and k is a scalar(float)
	*/
	inline Vector4D operator*(const float& k, const Vector4D& a)
	{
		return Vector4D((double)k * a.x(), (double)k * a.y(), (double)k * a.z(), (double)k * a.w());
	}

	/**@brief 4D vector scalar division.
	* Returns a / k,  where a is a vector and k is a scalar(float)
	* If k = 0 the returned vector is the zero vector.
	*/
	inline Vector4D operator/(const Vector4D& a, const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return Vector4D();
		}

		return Vector4D(a.x() / (double)k, a.y() / (double)k, a.z() / (double)k, a.w() / (double)k);
	}

	/**@brief Returns the dot product between two 4D vectors.
	*/
	inline float dotProduct(const Vector4D& a, const Vector4D& b)
	{
		//a dot b = axbx + ayby + azbz + awbw
		return (double)a.x() * b.x() + (double)a.y() * b.y() + (double)a.z() * b.z() + (double)a.w() * b.w();
	}

	/**@brief Returns the length(magnitude) of the 4D vector v.
	*/
	inline float length(const Vector4D& v)
	{
		//length(v) = sqrt(vx^2 + vy^2 + vz^2 + vw^2)
		return sqrt((double)v.x() * v.x() + (double)v.y() * v.y() + (double)v.z() * v.z() + (double)v.w() * v.w());
	}

	/**@brief Normalizes the 4D vector v.
	* If the 4D vector is the zero vector v is returned.
	*/
	inline Vector4D norm(const Vector4D& v)
	{
		//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
		//v is the zero vector
		if (zeroVector(v))
		{
			return v;
		}

		double mag = length(v);

		return Vector4D(v.x() / mag, v.y() / mag, v.z() / mag, v.w() / mag);
	}

	/**@brief Returns a 4D vector that is the projection of a onto b.
	* If b is the zero vector a is returned.
	*/
	inline Vector4D Projection(const Vector4D& a, const Vector4D& b)
	{
		//Projb(a) = (a dot b)b
		//normalize b before projecting
		Vector4D normB(norm(b));
		return Vector4D(dotProduct(a, normB) * normB);
	}


#if defined(_DEBUG)
	inline void print(const Vector4D& v)
	{
		std::cout << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << ")";
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

		/**@brief Default Constructor.
		*
		*	Creates a new 4x4 identity matrix.
		*/
		Matrix4x4();

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 4x4 matrix with elements initialized to the given 2D array.\n
		*	If the passed in 2D array isn't a 4x4 matrix, the behavior is undefined.
		*/
		Matrix4x4(float a[][4]);

		/**@brief Returns a pointer to the first element in the matrix.
		*/
		float* data();

		/**@brief Returns a constant pointer to the first element in the matrix.
		*/
		const float* data() const;

		/**@brief Returns a constant reference to the element at the given (row, col).
		* The row and col values should be between [0,3]. If any of them are out of that range, the first element will be returned.
		*/
		const float& operator()(unsigned int row, unsigned int col) const;

		/**@brief Returns a reference to the element at the given (row, col).
		* The row and col values should be between [0,3]. If any of them are out of that range, the first element will be returned.
		*/
		float& operator()(unsigned int row, unsigned int col);

		/**@brief Sets each element in the given row to the components of vector v.
		* Row should be between [0,3]. If it out of range the first row will be set.
		*/
		void setRow(unsigned int row, Vector4D v);

		/**@brief Sets each element in the given col to the components of vector v.
		* Col should be between [0,3]. If it out of range the first col will be set.
		*/
		void setCol(unsigned int col, Vector4D v);

		/**@brief Adds this 4x4 matrix with given matrix m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator+=(const Matrix4x4& m);

		/**@brief Subtracts this 4x4 matrix with given matrix m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator-=(const Matrix4x4& m);

		/**@brief Multiplies this 4x4 matrix with given scalar k and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator*=(const float& k);


		/**@brief Multiplies this 4x4 matrix with given matrix m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator*=(const Matrix4x4& m);

	private:

		float m_mat[4][4];
	};

	//-------------------------------------------------------------------------------------
	inline Matrix4x4::Matrix4x4()
	{
		//1st row
		m_mat[0][0] = 1.0f;
		m_mat[0][1] = 0.0f;
		m_mat[0][2] = 0.0f;
		m_mat[0][3] = 0.0f;

		//2nd
		m_mat[1][0] = 0.0f;
		m_mat[1][1] = 1.0f;
		m_mat[1][2] = 0.0f;
		m_mat[1][3] = 0.0f;

		//3rd row
		m_mat[2][0] = 0.0f;
		m_mat[2][1] = 0.0f;
		m_mat[2][2] = 1.0f;
		m_mat[2][3] = 0.0f;

		//4th row
		m_mat[3][0] = 0.0f;
		m_mat[3][1] = 0.0f;
		m_mat[3][2] = 0.0f;
		m_mat[3][3] = 1.0f;
	}



	inline Matrix4x4::Matrix4x4(float a[][4])
	{
		//1st row
		m_mat[0][0] = a[0][0];
		m_mat[0][1] = a[0][1];
		m_mat[0][2] = a[0][2];
		m_mat[0][3] = a[0][3];

		//2nd
		m_mat[1][0] = a[1][0];
		m_mat[1][1] = a[1][1];
		m_mat[1][2] = a[1][2];
		m_mat[1][3] = a[1][3];

		//3rd row
		m_mat[2][0] = a[2][0];
		m_mat[2][1] = a[2][1];
		m_mat[2][2] = a[2][2];
		m_mat[2][3] = a[2][3];

		//4th row
		m_mat[3][0] = a[3][0];
		m_mat[3][1] = a[3][1];
		m_mat[3][2] = a[3][2];
		m_mat[3][3] = a[3][3];
	}

	inline float* Matrix4x4::data()
	{
		return m_mat[0];
	}

	inline const float* Matrix4x4::data() const
	{
		return m_mat[0];
	}

	inline const float& Matrix4x4::operator()(unsigned int row, unsigned int col) const
	{
		if (row > 3 || col > 3)
		{
			return m_mat[0][0];
		}
		else
		{
			return m_mat[row][col];
		}
	}

	inline float& Matrix4x4::operator()(unsigned int row, unsigned int col)
	{
		if (row > 3 || col > 3)
		{
			return m_mat[0][0];
		}
		else
		{
			return m_mat[row][col];
		}
	}

	inline void Matrix4x4::setRow(unsigned int row, Vector4D v)
	{
		if (row < 0 || row > 3)
		{
			m_mat[0][0] = v.x();
			m_mat[0][1] = v.y();
			m_mat[0][2] = v.z();
			m_mat[0][3] = v.w();
		}
		else
		{
			m_mat[row][0] = v.x();
			m_mat[row][1] = v.y();
			m_mat[row][2] = v.z();
			m_mat[row][3] = v.w();
		}
	}

	inline void Matrix4x4::setCol(unsigned int col, Vector4D v)
	{
		if (col < 0 || col > 3)
		{
			m_mat[0][0] = v.x();
			m_mat[1][0] = v.y();
			m_mat[2][0] = v.z();
			m_mat[3][0] = v.w();
		}
		else
		{
			m_mat[0][col] = v.x();
			m_mat[1][col] = v.y();
			m_mat[2][col] = v.z();
			m_mat[3][col] = v.w();
		}
	}

	inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->m_mat[i][j] += (double)m.m_mat[i][j];
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
				this->m_mat[i][j] -= (double)m.m_mat[i][j];
			}
		}

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(const float& k)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->m_mat[i][j] *= (double)k;
			}
		}

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
	{
		Matrix4x4 res;

		for (int i = 0; i < 4; ++i)
		{
			res.m_mat[i][0] = ((double)m_mat[i][0] * m.m_mat[0][0]) +
				((double)m_mat[i][1] * m.m_mat[1][0]) +
				((double)m_mat[i][2] * m.m_mat[2][0]) +
				((double)m_mat[i][3] * m.m_mat[3][0]);

			res.m_mat[i][1] = ((double)m_mat[i][0] * m.m_mat[0][1]) +
				((double)m_mat[i][1] * m.m_mat[1][1]) +
				((double)m_mat[i][2] * m.m_mat[2][1]) +
				((double)m_mat[i][3] * m.m_mat[3][1]);

			res.m_mat[i][2] = ((double)m_mat[i][0] * m.m_mat[0][2]) +
				((double)m_mat[i][1] * m.m_mat[1][2]) +
				((double)m_mat[i][2] * m.m_mat[2][2]) +
				((double)m_mat[i][3] * m.m_mat[3][2]);

			res.m_mat[i][3] = ((double)m_mat[i][0] * m.m_mat[0][3]) +
				((double)m_mat[i][1] * m.m_mat[1][3]) +
				((double)m_mat[i][2] * m.m_mat[2][3]) +
				((double)m_mat[i][3] * m.m_mat[3][3]);
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_mat[i][j] = res.m_mat[i][j];
			}
		}

		return *this;
	}

	/**@brief Adds the two given 4x4 matrices and returns a Matrix4x4 object with the result.
	*/
	inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = (double)m1(i, j) + m2(i, j);
			}
		}

		return res;
	}

	/**@brief Negates the 4x4 matrix m.
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

	/**@brief Subtracts the two given 4x4 matrices and returns a Matrix4x4 object with the result.
	*/
	inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = (double)m1(i, j) - m2(i, j);
			}
		}

		return res;
	}

	/**@brief Multiplies the given 4x4 matrix with the given scalar and returns a Matrix4x4 object with the result.
	*/
	inline Matrix4x4 operator*(const Matrix4x4& m, const float& k)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = (double)m(i, j) * k;
			}
		}

		return res;
	}

	/**@brief Multiplies the the given scalar with the given 4x4 matrix and returns a Matrix4x4 object with the result.
	*/
	inline Matrix4x4 operator*(const float& k, const Matrix4x4& m)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = k * (double)m(i, j);
			}
		}

		return res;
	}

	/**@brief Multiplies the two given 4x4 matrices and returns a Matrix4x4 object with the result.
	*/
	inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;

		for (int i = 0; i < 4; ++i)
		{
			res(i, 0) = ((double)m1(i, 0) * m2(0, 0)) +
				((double)m1(i, 1) * m2(1, 0)) +
				((double)m1(i, 2) * m2(2, 0)) +
				((double)m1(i, 3) * m2(3, 0));

			res(i, 1) = ((double)m1(i, 0) * m2(0, 1)) +
				((double)m1(i, 1) * m2(1, 1)) +
				((double)m1(i, 2) * m2(2, 1)) +
				((double)m1(i, 3) * m2(3, 1));

			res(i, 2) = ((double)m1(i, 0) * m2(0, 2)) +
				((double)m1(i, 1) * m2(1, 2)) +
				((double)m1(i, 2) * m2(2, 2)) +
				((double)m1(i, 3) * m2(3, 2));

			res(i, 3) = ((double)m1(i, 0) * m2(0, 3)) +
				((double)m1(i, 1) * m2(1, 3)) +
				((double)m1(i, 2) * m2(2, 3)) +
				((double)m1(i, 3) * m2(3, 3));
		}

		return res;
	}

	/**@brief Multiplies the given 4x4 matrix with the given 4D vector and returns a Vector4D object with the result.
	* The vector is a column vector.
	*/
	inline Vector4D operator*(const Matrix4x4& m, const Vector4D& v)
	{
		Vector4D res;

		res.x() = ((double)m(0, 0) * v.x() + (double)m(0, 1) * v.y() + (double)m(0, 2) * v.z() + (double)m(0, 3) * v.w());
		res.y() = ((double)m(1, 0) * v.x() + (double)m(1, 1) * v.y() + (double)m(1, 2) * v.z() + (double)m(1, 3) * v.w());
		res.z() = ((double)m(2, 0) * v.x() + (double)m(2, 1) * v.y() + (double)m(2, 2) * v.z() + (double)m(2, 3) * v.w());
		res.w() = ((double)m(3, 0) * v.x() + (double)m(3, 1) * v.y() + (double)m(3, 2) * v.z() + (double)m(3, 3) * v.w());

		return res;
	}

	/**@brief Multiplies the given 4D vector with the given 4x4 matrix and returns a Vector4D object with the result.
	* The vector is a row vector.
	*/
	inline Vector4D operator*(const Vector4D& v, const Matrix4x4& m)
	{
		Vector4D res;

		res.x() = ((double)v.x() * m(0, 0) + (double)v.y() * m(1, 0) + (double)v.z() * m(2, 0) + (double)v.w() * m(3, 0));
		res.y() = ((double)v.x() * m(0, 1) + (double)v.y() * m(1, 1) + (double)v.z() * m(2, 1) + (double)v.w() * m(3, 1));
		res.z() = ((double)v.x() * m(0, 2) + (double)v.y() * m(1, 2) + (double)v.z() * m(2, 2) + (double)v.w() * m(3, 2));
		res.w() = ((double)v.x() * m(0, 3) + (double)v.y() * m(1, 3) + (double)v.z() * m(2, 3) + (double)v.w() * m(3, 3));

		return res;
	}

	/**@brief Sets the given matrix to the identity matrix.
	*/
	inline void setToIdentity(Matrix4x4& m)
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

	/**@brief Returns true if the given matrix is the identity matrix, false otherwise.
	*/
	inline bool isIdentity(const Matrix4x4& m)
	{
		//Is the identity matrix if the diagonals are equal to 1.0f and all other elements equals to 0.0f

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
				{
					if (!compareFloats(m(i, j), 1.0f, EPSILON))
					{
						return false;
					}
				}
				else
				{
					if (!compareFloats(m(i, j), 0.0f, EPSILON))
					{
						return false;
					}
				}
			}
		}
	}

	/**@brief Returns the tranpose of the given matrix m.
	*/
	inline Matrix4x4 transpose(const Matrix4x4& m)
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

	/**@brief Construct a 4x4 translation matrix with the given floats and post-multiply's it by the given matrix.
	* cm = cm * translate
	*/
	inline Matrix4x4 translate(const Matrix4x4& cm, float x, float y, float z)
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

	/**@brief Construct a 4x4 scaling matrix with the given floats and post-multiply's it by the given matrix.
	* cm = cm * scale
	*/
	inline Matrix4x4 scale(const Matrix4x4& cm, float x, float y, float z)
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

	/**@brief Construct a 4x4 rotation matrix with the given angle (in degrees) and axis (x, y, z) and post-multiply's it by the given matrix.
	* cm = cm * rotate.\n
	*/
	inline Matrix4x4 rotate(const Matrix4x4& cm, float angle, float x, float y, float z)
	{

		//c + (1 - c)x^2	(1 - c)xy + sz	(1 - c)xz - sy	0
		//(1 - c)xy - sz	c + (1 - c)y^2	(1 - c)yz + sx	0
		//(1 - c)xz + sy	(1 - c)yz - sx	c + (1 - c)z^2	0
		//0					0				0				1
		//c = cos(angle)
		//s = sin(angle)

		double c = cos(angle * PI / 180.0);
		double s = sin(angle * PI / 180.0);

		Matrix4x4 r;

		//1st row
		r(0, 0) = c + (1.0 - c) * ((double)x * x);
		r(0, 1) = (1.0 - c) * ((double)x * y) + (s * z);
		r(0, 2) = (1.0 - c) * ((double)x * z) - (s * y);

		//2nd row
		r(1, 0) = (1.0 - c) * ((double)x * y) - (s * z);
		r(1, 1) = c + (1.0 - c) * ((double)y * y);
		r(1, 2) = (1.0 - c) * ((double)y * z) + (s * x);

		//3rd row
		r(2, 0) = (1.0 - c) * ((double)x * z) + (s * y);
		r(2, 1) = (1.0 - c) * ((double)y * z) - (s * x);
		r(2, 2) = c + (1.0 - c) * ((double)z * z);

		return cm * r;
	}

	/**@brief Returns the determinant of the given matrix.
	*/
	inline double det(const Matrix4x4& m)
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

	/**@brief Returns the cofactor of the given row and col using the given matrix.
	*/
	inline double cofactor(const Matrix4x4& m, unsigned int row, unsigned int col)
	{
		//cij = (-1)^i + j * det of minor(i, j);
		double tempMat[3][3];
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

	/**@brief Returns the adjoint of the given matrix.
	*/
	inline Matrix4x4 adjoint(const Matrix4x4& m)
	{
		//Cofactor of each ijth position put into matrix cA.
		//Adjoint is the tranposed matrix of cA.
		Matrix4x4 cA;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				cA(i, j) = cofactor(m, i, j);
			}
		}

		return transpose(cA);
	}

	/**@brief Returns the inverse of the given matrix.
	* If the matrix is noninvertible/singular, the identity matrix is returned.
	*/
	inline Matrix4x4 inverse(const Matrix4x4& m)
	{
		//Inverse of m = adjoint of m / det of m
		double determinant = det(m);
		if (compareDoubles(determinant, 0.0, EPSILON))
			return Matrix4x4();

		return adjoint(m) * (1.0 / determinant);
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

	/** @class Quaternion ""
	*	The datatype for the components is float.\n
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

	//-------------------------------------------------------------------------------------
	inline Quaternion::Quaternion() : m_scalar{ 1.0f }, m_x{ 0.0f }, m_y{ 0.0f }, m_z{ 0.0f }
	{
	}

	inline Quaternion::Quaternion(float scalar, float x, float y, float z) : m_scalar{ scalar }, m_x{ x }, m_y{ y }, m_z{ z }
	{
	}

	inline Quaternion::Quaternion(float scalar, const Vector3D& v) : m_scalar{ scalar }, m_x{ v.x() }, m_y{ v.y() }, m_z{ v.z() }
	{
	}

	inline Quaternion::Quaternion(const Vector4D& v) : m_scalar{ v.x() }, m_x{ v.y() }, m_y{ v.z() }, m_z{ v.w() }
	{
	}

	inline float& Quaternion::scalar()
	{
		return m_scalar;
	}

	inline const float& Quaternion::scalar() const
	{
		return m_scalar;
	}

	inline float& Quaternion::x()
	{
		return m_x;
	}

	inline const float& Quaternion::x() const
	{
		return m_x;
	}

	inline float& Quaternion::y()
	{
		return m_y;
	}

	inline const float& Quaternion::y() const
	{
		return m_y;
	}

	inline float& Quaternion::z()
	{
		return m_z;
	}

	inline const float& Quaternion::z() const
	{
		return m_z;
	}

	inline Vector3D Quaternion::vector()
	{
		return Vector3D(m_x, m_y, m_z);
	}

	inline Quaternion& Quaternion::operator+=(const Quaternion& q)
	{
		this->m_scalar += (double)q.m_scalar;
		this->m_x += (double)q.m_x;
		this->m_y += (double)q.m_y;
		this->m_z += (double)q.m_z;

		return *this;
	}

	inline Quaternion& Quaternion::operator-=(const Quaternion& q)
	{
		this->m_scalar -= (double)q.m_scalar;
		this->m_x -= (double)q.m_x;
		this->m_y -= (double)q.m_y;
		this->m_z -= (double)q.m_z;

		return *this;
	}

	inline Quaternion& Quaternion::operator*=(float k)
	{
		this->m_scalar *= (double)k;
		this->m_x *= (double)k;
		this->m_y *= (double)k;
		this->m_z *= (double)k;

		return *this;
	}

	inline Quaternion& Quaternion::operator*=(const Quaternion& q)
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

	/**@brief Returns a quaternion that has the result of q1 + q2.
	*/
	inline Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion((double)q1.scalar() + q2.scalar(), (double)q1.x() + q2.x(), (double)q1.y() + q2.y(), (double)q1.z() + q2.z());
	}

	/**@brief Returns a quaternion that has the result of -q.
	*/
	inline Quaternion operator-(const Quaternion& q)
	{
		return Quaternion(-q.scalar(), -q.x(), -q.y(), -q.z());
	}

	/**@brief Returns a quaternion that has the result of q1 - q2.
	*/
	inline Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion((double)q1.scalar() - q2.scalar(), (double)q1.x() - q2.x(), (double)q1.y() - q2.y(), (double)q1.z() - q2.z());
	}

	/**@brief Returns a quaternion that has the result of k * q.
	*/
	inline Quaternion operator*(float k, const Quaternion& q)
	{
		return Quaternion((double)k * q.scalar(), (double)k * q.x(), (double)k * q.y(), (double)k * q.z());
	}

	/**@brief Returns a quaternion that has the result of q * k.
	*/
	inline Quaternion operator*(const Quaternion& q, float k)
	{
		return Quaternion(q.scalar() * (double)k, q.x() * (double)k, q.y() * (double)k, q.z() * (double)k);
	}

	/**@brief Returns a quaternion that has the result of q1 * q2.
	*/
	inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
	{
		//scalar part = q1scalar * q2scalar - q1Vector dot q2Vector
		//vector part = q1Scalar * q2Vector + q2Scalar * q1Vector + q1Vector cross q2Vector

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

	/**@brief Returns true if quaternion q is a zero quaternion, false otherwise.
	*/
	inline bool isZeroQuaternion(const Quaternion& q)
	{
		//zero quaternion = (0, 0, 0, 0)
		return compareFloats(q.scalar(), 0.0f, EPSILON) && compareFloats(q.x(), 0.0f, EPSILON) &&
			compareFloats(q.y(), 0.0f, EPSILON) && compareFloats(q.z(), 0.0f, EPSILON);
	}

	/**@brief Returns true if quaternion q is an identity quaternion, false otherwise.
	*/
	inline bool isIdentity(const Quaternion& q)
	{
		//identity quaternion = (1, 0, 0, 0)
		return compareFloats(q.scalar(), 1.0f, EPSILON) && compareFloats(q.x(), 0.0f, EPSILON) &&
			compareFloats(q.y(), 0.0f, EPSILON) && compareFloats(q.z(), 0.0f, EPSILON);
	}

	/**@brief Returns the conjugate of quaternion q.
	*/
	inline Quaternion conjugate(const Quaternion& q)
	{
		//conjugate of a quaternion is the quaternion with its vector part negated
		return Quaternion(q.scalar(), -q.x(), -q.y(), -q.z());
	}

	/**@brief Returns the length of quaternion q.
	*/
	inline float length(const Quaternion& q)
	{
		//length of a quaternion = sqrt(scalar^2 + x^2 + y^2 + z^2)
		return sqrt((double)q.scalar() * q.scalar() + (double)q.x() * q.x() + (double)q.y() * q.y() + (double)q.z() * q.z());
	}

	/**@brief Normalizes quaternion q and returns the normalized quaternion.
	* If q is the zero quaternion then q is returned.
	*/
	inline Quaternion normalize(const Quaternion& q)
	{
		//to normalize a quaternion you do q / |q|

		if (isZeroQuaternion(q))
			return q;

		double d{ length(q) };

		return Quaternion(q.scalar() / d, q.x() / d, q.y() / d, q.z() / d);
	}

	/**@brief Returns the invese of quaternion q.
	*  If q is the zero quaternion then q is returned.
	*/
	inline Quaternion inverse(const Quaternion& q)
	{
		//inverse = conjugate of q / |q|^2

		if (isZeroQuaternion(q))
			return q;

		Quaternion conjugateOfQ(conjugate(q));

		double d{ length(q) };
		d *= d;

		return Quaternion(conjugateOfQ.scalar() / d, conjugateOfQ.x() / d, conjugateOfQ.y() / d, conjugateOfQ.z() / d);
	}

	/**@brief Returns a quaternion from the axis-angle rotation representation.
	*  The angle should be given in degrees.
	*/
	inline Quaternion rotationQuaternion(float angle, float x, float y, float z)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		double ang{ angle / 2.0 };
		double c{ cos(ang * PI / 180.0) };
		double s{ sin(ang * PI / 180.0) };

		Vector3D axis(x, y, z);
		axis = norm(axis);

		return Quaternion(c, s * axis.x(), s * axis.y(), s * axis.z());
	}

	/**@brief Returns a quaternion from the axis-angle rotation representation.
	* The angle should be given in degrees.
	*/
	inline Quaternion rotationQuaternion(float angle, const Vector3D& axis)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		double ang{ angle / 2.0 };
		double c{ cos(ang * PI / 180.0) };
		double s{ sin(ang * PI / 180.0) };

		Vector3D axisN(norm(axis));

		return Quaternion(c, s * axisN.x(), s * axisN.y(), s * axisN.z());
	}

	/**@brief Returns a quaternion from the axis-angle rotation representation.
	* The x value in the 4D vector should be the angle(in degrees).\n
	* The y, z and w value in the 4D vector should be the axis.
	*/
	inline Quaternion rotationQuaternion(const Vector4D& angAxis)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		double angle{ angAxis.x() / 2.0 };
		double c{ cos(angle * PI / 180.0) };
		double s{ sin(angle * PI / 180.0) };

		Vector3D axis(angAxis.y(), angAxis.z(), angAxis.w());
		axis = norm(axis);

		return Quaternion(c, s * axis.x(), s * axis.y(), s * axis.z());
	}

	/**@brief Returns a matrix from the given quaterion for column vector-matrix multiplication.
	* Quaternion q should be a unit quaternion.
	*/
	inline Matrix4x4 quaternionRotationMatrixCol(const Quaternion& q)
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

	/**@brief Returns a matrix from the given quaterion for row vector-matrix multiplication.
	* Quaternion q should be a unit quaternion.
	*/
	inline Matrix4x4 quaternionRotationMatrixRow(const Quaternion& q)
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
	inline void print(const Quaternion& q)
	{
		std::cout << "(" << q.scalar() << ", " << q.x() << ", " << q.y() << ", " << q.z();
	}
#endif
	//-------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------
}