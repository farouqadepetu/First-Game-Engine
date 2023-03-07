#include "FAVector2D.h"
#include "FAVector3D.h"
#include "FAVector4D.h"

namespace FAMath
{
	//--------------------------------------------------------------------------------------
		//Vector3D Constructors

	Vector3D::Vector3D() : m_x{ 0.0f }, m_y{ 0.0f }, m_z{ 0.0f }
	{}

	Vector3D::Vector3D(float x, float y, float z) : m_x{ x }, m_y{ y }, m_z{ z }
	{}

	Vector3D::Vector3D(Vector2D v, float z) : m_x{ v.x() }, m_y{ v.y() }, m_z{ z }
	{}

	Vector3D::Vector3D(Vector4D v) : m_x{ v.x() }, m_y{ v.y() }, m_z{ v.z() }
	{}

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Vector3D Getters and Setters

	float& Vector3D::x()
	{
		return m_x;
	}

	float& Vector3D::y()
	{
		return m_y;
	}

	float& Vector3D::z()
	{
		return m_z;
	}

	const float& Vector3D::x() const
	{
		return m_x;
	}

	const float& Vector3D::y() const
	{
		return m_y;
	}

	const float& Vector3D::z() const
	{
		return m_z;
	}
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//Vector3D Memeber functions

	Vector3D& Vector3D::operator+=(const Vector3D& b)
	{
		this->m_x += (double)b.m_x;
		this->m_y += (double)b.m_y;
		this->m_z += (double)b.m_z;

		return *this;
	}

	Vector3D& Vector3D::operator-=(const Vector3D& b)
	{
		this->m_x -= (double)b.m_x;
		this->m_y -= (double)b.m_y;
		this->m_z -= (double)b.m_z;

		return *this;
	}

	Vector3D& Vector3D::operator*=(const float& k)
	{
		this->m_x *= (double)k;
		this->m_y *= (double)k;
		this->m_z *= (double)k;

		return *this;
	}

	Vector3D& Vector3D::operator/=(const float& k)
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

	bool zeroVector(const Vector3D& a)
	{
		if (compareFloats(a.x(), 0.0f, EPSILON) && compareFloats(a.y(), 0.0f, EPSILON) &&
			compareFloats(a.z(), 0.0f, EPSILON))
		{
			return true;
		}

		return false;
	}

	Vector3D operator+(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D((double)a.x() + b.x(), (double)a.y() + b.y(), (double)a.z() + b.z());
	}

	Vector3D operator-(const Vector3D& v)
	{
		return Vector3D(-v.x(), -v.y(), -v.z());
	}

	Vector3D operator-(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
	}

	Vector3D operator*(const Vector3D& a, const float& k)
	{
		return Vector3D(a.x() * (double)k, a.y() * (double)k, a.z() * (double)k);
	}


	Vector3D operator*(const float& k, const Vector3D& a)
	{
		return Vector3D((double)k * a.x(), (double)k * a.y(), (double)k * a.z());
	}

	Vector3D operator/(const Vector3D& a, const float& k)
	{
		if (compareFloats(k, 0.0f, EPSILON))
		{
			return Vector3D();
		}

		return Vector3D(a.x() / (double)k, a.y() / (double)k, a.z() / (double)k);
	}

	//a dot b = axbx + ayby + azbz
	float dotProduct(const Vector3D& a, const Vector3D& b)
	{
		return (double)a.x() * b.x() + (double)a.y() * b.y() + (double)a.z() * b.z();
	}

	//a x b = (aybz - azby, azbx - axbz, axby - aybx)
	Vector3D crossProduct(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D((double)a.y() * b.z() - (double)a.z() * b.y(),
			(double)a.z() * b.x() - (double)a.x() * b.z(),
			(double)a.x() * b.y() - (double)a.y() * b.x());
	}

	//length(v) = sqrt(vx^2 + vy^2 + vz^2)
	float length(const Vector3D& v)
	{
		return sqrt((double)v.x() * v.x() + (double)v.y() * v.y() + (double)v.z() * v.z());
	}

	//norm(v) = v / length(v) == (vx / length(v), vy / length(v))
	Vector3D norm(const Vector3D& v)
	{
		//v is the zero vector
		if (zeroVector(v))
		{
			return v;
		}

		double mag = length(v);

		return Vector3D(v.x() / mag, v.y() / mag, v.z() / mag);
	}

	//v = (r, theta, z)
	//x = rcos(theta)
	//y = rsin(theta)
	//z = z
	Vector3D CylindricalToCartesian(const Vector3D& v)
	{
		double angle = v.y() * PI / 180.0;

		return Vector3D(v.x() * cos(angle), v.x() * sin(angle), v.z());
	}

	//v = (x, y ,z)
	//r = sqrt(vx^2 + vy^2 + vz^2)
	//theta = arctan(y / x)
	//z = z
	Vector3D CartesianToCylindrical(const Vector3D& v)
	{
		if (compareFloats(v.x(), 0.0f, EPSILON))
		{
			return v;
		}

		double theta{ atan2(v.y(), v.x()) * 180.0 / PI };
		return Vector3D(length(v), theta, v.z());
	}

	// v = (pho, phi, theta)
	//x = pho * sin(phi) * cos(theta)
	//y = pho * sin(phi) * sin(theta)
	//z = pho * cos(theta);
	Vector3D SphericalToCartesian(const Vector3D& v)
	{
		double phi{ v.y() * PI / 180.0 };
		double theta{ v.z() * PI / 180.0 };

		return Vector3D(v.x() * sin(phi) * cos(theta), v.x() * sin(phi) * sin(theta), v.x() * cos(theta));
	}

	//v = (x, y ,z)
	//pho = sqrt(vx^2 + vy^2 + vz^2)
	//phi = arcos(z / pho)
	//theta = arctan(y / x)
	Vector3D CartesianToSpherical(const Vector3D& v)
	{
		if (compareFloats(v.x(), 0.0f, EPSILON) || zeroVector(v))
		{
			return v;
		}

		double pho{ length(v) };
		double phi{ acos(v.z() / pho) * 180.0 / PI };
		double theta{ atan2(v.y(), v.x()) * 180.0 / PI };

		return Vector3D(pho, phi, theta);
	}


	//Projb(a) = (a dot b)b
	//normalize b before projecting
	Vector3D Projection(const Vector3D& a, const Vector3D& b)
	{
		Vector3D normB(norm(b));
		return Vector3D(dotProduct(a, normB) * normB);
	}


#if defined(_DEBUG)
	void print(const Vector3D& v)
	{
		std::cout << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
	}
#endif
	//-------------------------------------------------------------------------------------
}