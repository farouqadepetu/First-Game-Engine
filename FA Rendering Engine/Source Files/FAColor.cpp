#include "FAColor.h"
#include <algorithm>

namespace FAColor
{
	Color::Color(float r, float g, float b, float a)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f

		//red
		if (r < 0.0f)
			mColor.SetX(0.0f);
		if (r > 1.0f)
			mColor.SetX(1.0f);
		else
			mColor.SetX(r);

		//green
		if (g < 0.0f)
			mColor.SetY(0.0f);
		if (g > 1.0f)
			mColor.SetY(1.0f);
		else
			mColor.SetY(g);

		//blue
		if (b < 0.0f)
			mColor.SetZ(0.0f);
		if (b > 1.0f)
			mColor.SetZ(1.0f);
		else
			mColor.SetZ(b);

		//alpha
		if (a < 0.0f)
			mColor.SetW(0.0f);
		if (a > 1.0f)
			mColor.SetW(1.0f);
		else
			mColor.SetW(a);
	}

	Color::Color(const FAMath::Vector4D& color)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f

		//red
		if (color.GetX() < 0.0f)
			mColor.SetX(0.0f);
		else if (color.GetX() > 1.0f)
			mColor.SetX(1.0f);
		else
			mColor.SetX(color.GetX());

		//green
		if (color.GetY() < 0.0f)
			mColor.SetY(0.0f);
		else if (color.GetY() > 1.0f)
			mColor.SetY(1.0f);
		else
			mColor.SetY(color.GetY());

		//blue
		if (color.GetZ() < 0.0f)
			mColor.SetZ(0.0f);
		else if (color.GetZ() > 1.0f)
			mColor.SetZ(1.0f);
		else
			mColor.SetZ(color.GetZ());

		//alpha
		if (color.GetW() < 0.0f)
			mColor.SetW(0.0f);
		else if (color.GetW() > 1.0f)
			mColor.SetW(1.0f);
		else
			mColor.SetW(color.GetW());
	}

	const FAMath::Vector4D& Color::GetColor() const
	{
		return mColor;
	}

	float Color::GetRed() const
	{
		return mColor.GetX();
	}

	float Color::GetGreen() const
	{
		return mColor.GetY();
	}

	float Color::GetBlue() const
	{
		return mColor.GetZ();
	}

	float Color::GetAlpha() const
	{
		return mColor.GetW();
	}

	void Color::SetColor(const FAMath::Vector4D& color)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f

		//red
		if (color.GetX() < 0.0f)
			mColor.SetX(0.0f);
		else if (color.GetX() > 1.0f)
			mColor.SetX(1.0f);
		else
			mColor.SetX(color.GetX());

		//green
		if (color.GetY() < 0.0f)
			mColor.SetY(0.0f);
		else if (color.GetY() > 1.0f)
			mColor.SetY(1.0f);
		else
			mColor.SetY(color.GetY());

		//blue
		if (color.GetZ() < 0.0f)
			mColor.SetZ(0.0f);
		else if (color.GetZ() > 1.0f)
			mColor.SetZ(1.0f);
		else
			mColor.SetZ(color.GetZ());

		//alpha
		if (color.GetW() < 0.0f)
			mColor.SetW(0.0f);
		else if (color.GetW() > 1.0f)
			mColor.SetW(1.0f);
		else
			mColor.SetW(color.GetW());
	}

	void Color::SetRed(float r)
	{
		//red
		if (r < 0.0f)
			mColor.SetX(0.0f);
		if (r > 1.0f)
			mColor.SetX(1.0f);
		else
			mColor.SetX(r);

	}

	void Color::SetGreen(float g)
	{
		//green
		if (g < 0.0f)
			mColor.SetY(0.0f);
		if (g > 1.0f)
			mColor.SetY(1.0f);
		else
			mColor.SetY(g);
	}

	void Color::SetBlue(float b)
	{
		//blue
		if (b < 0.0f)
			mColor.SetZ(0.0f);
		if (b > 1.0f)
			mColor.SetZ(1.0f);
		else
			mColor.SetZ(b);
	}

	void Color::SetAlpha(float a)
	{
		//alpha
		if (a < 0.0f)
			mColor.SetW(0.0f);
		if (a > 1.0f)
			mColor.SetW(1.0f);
		else
			mColor.SetW(a);
	}

	Color& Color::operator+=(const Color& c)
	{
		this->mColor += c.mColor;

		if (mColor.GetX() > 1.0f)
			mColor.SetX(1.0f);

		if (mColor.GetY() > 1.0f)
			mColor.SetY(1.0f);

		if (mColor.GetZ() > 1.0f)
			mColor.SetZ(1.0f);

		if (mColor.GetW() > 1.0f)
			mColor.SetW(1.0f);

		return *this;
	}
	
	Color& Color::operator-=(const Color& c)
	{
		this->mColor -= c.mColor;

		if (mColor.GetX() < 0.0f)
			mColor.SetX(0.0f);

		if (mColor.GetY() < 0.0f)
			mColor.SetY(0.0f);

		if (mColor.GetZ() < 0.0f)
			mColor.SetZ(0.0f);

		if (mColor.GetW() < 0.0f)
			mColor.SetW(0.0f);

		return *this;
	}

	Color& Color::operator*=(float k)
	{
		if (k < 0.0f)
			return *this;

		mColor *= k;

		if (mColor.GetX() > 1.0f)
			mColor.SetX(1.0f);

		if (mColor.GetY() > 1.0f)
			mColor.SetY(1.0f);

		if (mColor.GetZ() > 1.0f)
			mColor.SetZ(1.0f);

		if (mColor.GetW() > 1.0f)
			mColor.SetW(1.0f);

		return *this;
	}

	Color& Color::operator*=(const Color& c)
	{
		this->mColor.SetX(this->mColor.GetX() * c.mColor.GetX());
		this->mColor.SetY(this->mColor.GetY() * c.mColor.GetY());
		this->mColor.SetZ(this->mColor.GetZ() * c.mColor.GetZ());
		this->mColor.SetW(this->mColor.GetW() * c.mColor.GetW());

		return *this;
	}

	Color operator+(const Color& c1, const Color& c2)
	{
		Color result;
		result.SetColor(c1.GetColor() + c2.GetColor());

		if (result.GetRed() > 1.0f)
			result.SetRed(1.0f);

		if (result.GetGreen() > 1.0f)
			result.SetGreen(1.0f);

		if (result.GetBlue() > 1.0f)
			result.SetBlue(1.0f);

		if (result.GetAlpha() > 1.0f)
			result.SetAlpha(1.0f);

		return result;
	}

	Color operator-(const Color& c1, const Color& c2)
	{
		Color result;
		result.SetColor(c1.GetColor() - c2.GetColor());

		if (result.GetRed() < 0.0f)
			result.SetRed(0.0f);

		if (result.GetGreen() < 0.0f)
			result.SetGreen(0.0f);

		if (result.GetBlue() < 0.0f)
			result.SetBlue(0.0f);

		if (result.GetAlpha() < 0.0f)
			result.SetAlpha(0.0f);

		return result;
	}

	Color operator*(const Color& c, float k)
	{
		if (k < 0.0f)
			return c;

		Color result = c.GetColor() * k;

		if (result.GetRed() > 1.0f)
			result.SetRed(1.0f);

		if (result.GetGreen() > 1.0f)
			result.SetGreen(1.0f);

		if (result.GetBlue() > 1.0f)
			result.SetBlue(1.0f);

		if (result.GetAlpha() > 1.0f)
			result.SetAlpha(1.0f);

		return result;
	}

	Color operator*(float k, const Color& c)
	{
		if (k < 0.0f)
			return c;

		Color result = k * c.GetColor();

		if (result.GetRed() > 1.0f)
			result.SetRed(1.0f);

		if (result.GetGreen() > 1.0f)
			result.SetGreen(1.0f);

		if (result.GetBlue() > 1.0f)
			result.SetBlue(1.0f);

		if (result.GetAlpha() > 1.0f)
			result.SetAlpha(1.0f);

		return result;
	}

	Color operator*(const Color& c1, const Color& c2)
	{
		Color result;
		result.SetRed(c1.GetRed()* c2.GetRed());
		result.SetGreen(c1.GetGreen() * c2.GetGreen());
		result.SetBlue(c1.GetBlue() * c2.GetBlue());
		result.SetAlpha(c1.GetAlpha() * c2.GetAlpha());


		return result;
	}
}