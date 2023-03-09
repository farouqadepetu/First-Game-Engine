#include "FAColor.h"

namespace FAColor
{
	Color::Color() : mColor{ 0.0f, 0.0f, 0.0f, 1.0f }
	{}

	Color::Color(const FAMath::Vector4D& color)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f
		
		//red
		if (color.x() < 0.0f)
			mColor.x() = 0.0f;
		else if (color.x() > 1.0f)
			mColor.x() = 1.0f;
		else
			mColor.x() = color.x();

		//green
		if (color.y() < 0.0f)
			mColor.y() = 0.0f;
		else if (color.y() > 1.0f)
			mColor.y() = 1.0f;
		else
			mColor.y() = color.y();

		//blue
		if (color.z() < 0.0f)
			mColor.z() = 0.0f;
		else if (color.z() > 1.0f)
			mColor.z() = 1.0f;
		else
			mColor.z() = color.z();

		//alpha
		if (color.w() < 0.0f)
			mColor.w() = 0.0f;
		else if (color.w() > 1.0f)
			mColor.w() = 1.0f;
		else
			mColor.w() = color.w();
	}

	Color::Color(float r, float g, float b, float a)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f

		//red
		if (r < 0.0f)
			mColor.x() = 0.0f;
		if (r > 1.0f)
			mColor.x() = 1.0f;
		else
			mColor.x() = r;

		//green
		if (g < 0.0f)
			mColor.y() = 0.0f;
		if (g > 1.0f)
			mColor.y() = 1.0f;
		else
			mColor.y() = g;

		//blue
		if (b < 0.0f)
			mColor.z() = 0.0f;
		if (b > 1.0f)
			mColor.z() = 1.0f;
		else
			mColor.z() = b;

		//alpha
		if (a < 0.0f)
			mColor.w() = 0.0f;
		if (a > 1.0f)
			mColor.w() = 1.0f;
		else
			mColor.w() = a;
	}

	void Color::setColor(const FAMath::Vector4D& color)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f

		//red
		if (color.x() < 0.0f)
			mColor.x() = 0.0f;
		else if (color.x() > 1.0f)
			mColor.x() = 1.0f;
		else
			mColor.x() = color.x();

		//green
		if (color.y() < 0.0f)
			mColor.y() = 0.0f;
		else if (color.y() > 1.0f)
			mColor.y() = 1.0f;
		else
			mColor.y() = color.y();

		//blue
		if (color.z() < 0.0f)
			mColor.z() = 0.0f;
		else if (color.z() > 1.0f)
			mColor.z() = 1.0f;
		else
			mColor.z() = color.z();

		//alpha
		if (color.w() < 0.0f)
			mColor.w() = 0.0f;
		else if (color.w() > 1.0f)
			mColor.w() = 1.0f;
		else
			mColor.w() = color.w();
	}

	void Color::setRed(float r)
	{
		//red
		if (r < 0.0f)
			mColor.x() = 0.0f;
		if (r > 1.0f)
			mColor.x() = 1.0f;
		else
			mColor.x() = r;
	}

	void Color::setGreen(float g)
	{
		//green
		if (g < 0.0f)
			mColor.y() = 0.0f;
		if (g > 1.0f)
			mColor.y() = 1.0f;
		else
			mColor.y() = g;
	}

	void Color::setBlue(float b)
	{
		//blue
		if (b < 0.0f)
			mColor.z() = 0.0f;
		if (b > 1.0f)
			mColor.z() = 1.0f;
		else
			mColor.z() = b;
	}

	void Color::setAlpha(float a)
	{
		//alpha
		if (a < 0.0f)
			mColor.w() = 0.0f;
		if (a > 1.0f)
			mColor.w() = 1.0f;
		else
			mColor.w() = a;
	}

	FAMath::Vector4D Color::getColor() const
	{
		return mColor;
	}

	float Color::getRed() const
	{
		return mColor.x();
	}

	float Color::getGreen() const
	{
		return mColor.y();
	}

	float Color::getBlue() const
	{
		return mColor.z();
	}

	float Color::getAlpha() const
	{
		return mColor.w();
	}

	Color& Color::operator+=(const Color& c)
	{
		this->mColor += c.mColor;

		if (mColor.x() > 1.0f)
			mColor.x() = 1.0f;

		if (mColor.y() > 1.0f)
			mColor.y() = 1.0f;

		if (mColor.z() > 1.0f)
			mColor.z() = 1.0f;

		if (mColor.w() > 1.0f)
			mColor.w() = 1.0f;

		return *this;
	}
	
	Color& Color::operator-=(const Color& c)
	{
		this->mColor -= c.mColor;

		if (mColor.x() < 0.0f)
			mColor.x() = 0.0f;

		if (mColor.y() < 0.0f)
			mColor.y() = 0.0f;

		if (mColor.z() < 0.0f)
			mColor.z() = 0.0f;

		if (mColor.w() < 0.0f)
			mColor.w() = 0.0f;

		return *this;
	}

	Color& Color::operator*=(float k)
	{
		if (k < 0.0f)
			return *this;

		mColor *= k;

		if (mColor.x() > 1.0f)
			mColor.x() = 1.0f;

		if (mColor.y() > 1.0f)
			mColor.y() = 1.0f;

		if (mColor.z() > 1.0f)
			mColor.z() = 1.0f;

		if (mColor.w() > 1.0f)
			mColor.w() = 1.0f;

		return *this;
	}

	Color& Color::operator*=(const Color& c)
	{
		this->mColor.x() = this->mColor.x() * c.mColor.x();
		this->mColor.y() = this->mColor.y() * c.mColor.y();
		this->mColor.z() = this->mColor.z() * c.mColor.z();
		this->mColor.w() = this->mColor.w() * c.mColor.w();

		if (mColor.x() > 1.0f)
			mColor.x() = 1.0f;

		if (mColor.y() > 1.0f)
			mColor.y() = 1.0f;

		if (mColor.z() > 1.0f)
			mColor.z() = 1.0f;

		if (mColor.w() > 1.0f)
			mColor.w() = 1.0f;

		return *this;
	}

	Color operator+(const Color& c1, const Color& c2)
	{
		Color result;
		result.setColor(c1.getColor() + c2.getColor());

		if (result.getRed() > 1.0f)
			result.setRed(1.0f);

		if (result.getGreen() > 1.0f)
			result.setGreen(1.0f);

		if (result.getBlue() > 1.0f)
			result.setBlue(1.0f);

		if (result.getAlpha() > 1.0f)
			result.setAlpha(1.0f);

		return result;
	}

	Color operator-(const Color& c1, const Color& c2)
	{
		Color result;
		result.setColor(c1.getColor() - c2.getColor());

		if (result.getRed() < 0.0f)
			result.setRed(0.0f);

		if (result.getGreen() < 0.0f)
			result.setGreen(0.0f);

		if (result.getBlue() < 0.0f)
			result.setBlue(0.0f);

		if (result.getAlpha() < 0.0f)
			result.setAlpha(0.0f);

		return result;
	}

	Color operator*(const Color& c, float k)
	{
		if (k < 0.0f)
			return c;

		Color result = c.getColor() * k;

		if (result.getRed() > 1.0f)
			result.setRed(1.0f);

		if (result.getGreen() > 1.0f)
			result.setGreen(1.0f);

		if (result.getBlue() > 1.0f)
			result.setBlue(1.0f);

		if (result.getAlpha() > 1.0f)
			result.setAlpha(1.0f);

		return result;
	}

	Color operator*(float k, const Color& c)
	{
		if (k < 0.0f)
			return c;

		Color result = k * c.getColor();

		if (result.getRed() > 1.0f)
			result.setRed(1.0f);

		if (result.getGreen() > 1.0f)
			result.setGreen(1.0f);

		if (result.getBlue() > 1.0f)
			result.setBlue(1.0f);

		if (result.getAlpha() > 1.0f)
			result.setAlpha(1.0f);

		return result;
	}

	Color operator*(const Color& c1, const Color& c2)
	{
		Color result;
		result.setRed(c1.getRed()* c2.getRed());
		result.setGreen(c1.getGreen() * c2.getGreen());
		result.setBlue(c1.getBlue() * c2.getBlue());
		result.setAlpha(c1.getAlpha() * c2.getAlpha());

		if (result.getRed() > 1.0f)
			result.setRed(1.0f);

		if (result.getGreen() > 1.0f)
			result.setGreen(1.0f);

		if (result.getBlue() > 1.0f)
			result.setBlue(1.0f);

		if (result.getAlpha() > 1.0f)
			result.setAlpha(1.0f);

		return result;
	}
}