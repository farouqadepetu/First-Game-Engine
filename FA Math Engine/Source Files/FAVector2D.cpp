#include "FAVector2D.h"
#include "FAVector3D.h"
#include "FAVector4D.h"

namespace FAMath
{
	//--------------------------------------------------------------------------------------
		//Vector2D Constructors

	Vector2D::Vector2D() : m_x{ 0.0f }, m_y{ 0.0f }
	{}

	Vector2D::Vector2D(float x, float y) : m_x{ x }, m_y{ y }
	{}

	Vector2D::Vector2D(Vector3D v) : m_x{ v.x() }, m_y{ v.y() }
	{}

	Vector2D::Vector2D(Vector4D v) : m_x{ v.x() }, m_y{ v.y() }
	{}

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector2D Getters and Setters

	float& Vector2D::x()
	{
		return m_x;
	}

	float& Vector2D::y()
	{
		return m_y;
	}

	const float& Vector2D::x() const
	{
		return m_x;
	}

	const float& Vector2D::y() const
	{
		return m_y;
	}

	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector2D Memeber functions

	Vector2D& Vector2D::operator+=(const Vector2D& b)
	{
		this->m_x += (double)b.m_x;
		this->m_y += (double)b.m_y;

		return *this;
	}

	Vector2D& Vector2D::operator-=(const Vector2D& b)
	{
		this->m_x -= (double)b.m_x;
		this->m_y -= (double)b.m_y;

		return *this;
	}

	Vector2D& Vector2D::operator*=(const float& k)
	{
		this->m_x *= (double)k;
		this->m_y *= (double)k;

		return *this;
	}

	Vector2D& Vector2D::operator/=(const float& k)
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

	bool zeroVector(const Vector2D& a)
	{
		if (compareFloats(a.x(), 0.0f, EPSILON) && compareFloats(a.y(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	Vector2D operator+(const Vector2D& a, const Vector2D& b)
	{
		return Vector2D((double)a.x() + b.x(), (double)a.y() + b.y());
	}

	Vector2D operator-(const Vector2D& v)
	{
		return Vector2D(-v.x(), -v.y());
	}

	Vector2D operator-(const Vector2D& a, const Vector2D& b)
	{
		return Vector2D((double)a.x() - b.x(), (double)a.y() - b.y());
	}

	Vector2D operator*(const Vector2D& a, const float& k)
	{
		return Vector2D((double)a.x() * k, (double)a.y() * k);
	}

	Vector2D operator*(const float& k, const Vector2D& a)
	{
		return Vector2D((double)k * a.x(), (double)k * a.y());
	}

	Vector2D operator/(const Vector2D& a, const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return Vector2D();
		}

		return Vector2D(a.x() / (double)k, a.y() / (double)k);
	}

	//a dot b = axbx + ayby
	float dotProduct(const Vector2D& a, const Vector2D& b)
	{
		return (double)a.x() * b.x() + (double)a.y() * b.y();
	}

	//length(v) = sqrt(vx^2 + vy^2)
	float length(const Vector2D& v)
	{
		return sqrt((double)v.x() * v.x() + (double)v.y() * v.y());
	}

	//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
	Vector2D norm(const Vector2D& v)
	{
		//v is the zero vector
		if (zeroVector(v))
		{
			return v;
		}

		double mag = length(v);

		return Vector2D(v.x() / mag, v.y() / mag);
	}

	//v = (r, theta)
	//x = rcos((theta)
	//y = rsin(theta)
	Vector2D PolarToCartesian(const Vector2D& v)
	{
		float angle = v.y() * PI / 180.0f;

		return Vector2D(v.x() * cos(angle), v.x() * sin(angle));
	}

	//v = (x, y)
	//r = sqrt(vx^2 + vy^2)
	//theta = arctan(y / x)
	Vector2D CartesianToPolar(const Vector2D& v)
	{
		if (compareFloats(v.x(), 0.0f, EPSILON))
		{
			return v;
		}

		double theta{ atan2(v.y(), v.x()) * 180.0 / PI };
		return Vector2D(length(v), theta);
	}

	//Projb(a) = (a dot b)b
	//normalize b before projecting
	Vector2D Projection(const Vector2D& a, const Vector2D& b)
	{
		Vector2D normB(norm(b));
		return Vector2D(dotProduct(a, normB) * normB);
	}


#if defined(_DEBUG)
	void print(const Vector2D& v)
	{
		std::cout << "(" << v.x() << ", " << v.y() << ")";
	}
#endif
	//-------------------------------------------------------------------------------------
}