#include "FAVector2D.h"
#include "FAVector3D.h"
#include "FAVector4D.h"

namespace FAMath
{
	//--------------------------------------------------------------------------------------
		//Vector4D Constructors

	Vector4D::Vector4D() : m_x{ 0.0f }, m_y{ 0.0f }, m_z{ 0.0f }, m_w{ 0.0f }
	{}

	Vector4D::Vector4D(float x, float y, float z, float w) : m_x{ x }, m_y{ y }, m_z{ z }, m_w{ w }
	{}

	Vector4D::Vector4D(Vector2D v, float z, float w) : m_x{ v.x() }, m_y{ v.y() }, m_z{ z }, m_w{ w }
	{}

	Vector4D::Vector4D(Vector3D v, float w) : m_x{ v.x() }, m_y{ v.y() }, m_z{ v.z() }, m_w{ w }
	{}

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector4D Getters and Setters

	float& Vector4D::x()
	{
		return m_x;
	}

	float& Vector4D::y()
	{
		return m_y;
	}

	float& Vector4D::z()
	{
		return m_z;
	}

	float& Vector4D::w()
	{
		return m_w;
	}

	const float& Vector4D::x() const
	{
		return m_x;
	}

	const float& Vector4D::y() const
	{
		return m_y;
	}

	const float& Vector4D::z() const
	{
		return m_z;
	}

	const float& Vector4D::w() const
	{
		return m_w;
	}
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector4D Memeber functions

	Vector4D& Vector4D::operator+=(const Vector4D& b)
	{
		this->m_x += (double)b.m_x;
		this->m_y += (double)b.m_y;
		this->m_z += (double)b.m_z;
		this->m_w += (double)b.m_w;

		return *this;
	}

	Vector4D& Vector4D::operator-=(const Vector4D& b)
	{
		this->m_x -= (double)b.m_x;
		this->m_y -= (double)b.m_y;
		this->m_z -= (double)b.m_z;
		this->m_w -= (double)b.m_w;

		return *this;
	}

	Vector4D& Vector4D::operator*=(const float& k)
	{
		this->m_x *= (double)k;
		this->m_y *= (double)k;
		this->m_z *= (double)k;
		this->m_w *= (double)k;

		return *this;
	}

	Vector4D& Vector4D::operator/=(const float& k)
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

	bool zeroVector(const Vector4D& a)
	{
		if (compareFloats(a.x(), 0.0f, EPSILON) && compareFloats(a.y(), 0.0f, EPSILON) &&
			compareFloats(a.z(), 0.0f, EPSILON) && compareFloats(a.w(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	Vector4D operator+(const Vector4D& a, const Vector4D& b)
	{
		return Vector4D((double)a.x() + b.x(), (double)a.y() + b.y(), (double)a.z() + b.z(), (double)a.w() + b.w());
	}

	Vector4D operator-(const Vector4D& v)
	{
		return Vector4D(-v.x(), -v.y(), -v.z(), -v.w());
	}

	Vector4D operator-(const Vector4D& a, const Vector4D& b)
	{
		return Vector4D((double)a.x() - b.x(), (double)a.y() - b.y(), (double)a.z() - b.z(), (double)a.w() - b.w());
	}

	Vector4D operator*(const Vector4D& a, const float& k)
	{
		return Vector4D(a.x() * (double)k, a.y() * (double)k, a.z() * (double)k, a.w() * (double)k);
	}


	Vector4D operator*(const float& k, const Vector4D& a)
	{
		return Vector4D((double)k * a.x(), (double)k * a.y(), (double)k * a.z(), (double)k * a.w());
	}

	Vector4D operator/(const Vector4D& a, const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return Vector4D();
		}

		return Vector4D(a.x() / (double)k, a.y() / (double)k, a.z() / (double)k, a.w() / (double)k);
	}

	//a dot b = axbx + ayby + azbz + awbw
	float dotProduct(const Vector4D& a, const Vector4D& b)
	{
		return (double)a.x() * b.x() + (double)a.y() * b.y() + (double)a.z() * b.z() + (double)a.w() * b.w();
	}

	//length(v) = sqrt(vx^2 + vy^2 + vz^2 + vw^2)
	float length(const Vector4D& v)
	{
		return sqrt((double)v.x() * v.x() + (double)v.y() * v.y() + (double)v.z() * v.z() + (double)v.w() * v.w());
	}

	//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
	Vector4D norm(const Vector4D& v)
	{
		//v is the zero vector
		if (zeroVector(v))
		{
			return v;
		}

		double mag = length(v);

		return Vector4D(v.x() / mag, v.y() / mag, v.z() / mag, v.w() / mag);
	}

	//Projb(a) = (a dot b)b
	//normalize b before projecting
	Vector4D Projection(const Vector4D& a, const Vector4D& b)
	{
		Vector4D normB(norm(b));
		return Vector4D(dotProduct(a, normB) * normB);
	}


#if defined(_DEBUG)
	void print(const Vector4D& v)
	{
		std::cout << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << ")";
	}
#endif
	//-------------------------------------------------------------------------------------
}