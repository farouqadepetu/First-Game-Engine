#include "Color.h"

namespace RenderingEngine
{
	Color::Color(float r, float g, float b, float a)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f

		mColor.x = MathEngine::Clamp(r, 0.0f, 1.0f); //red
		mColor.y = MathEngine::Clamp(g, 0.0f, 1.0f); //green
		mColor.z = MathEngine::Clamp(b, 0.0f, 1.0f); //blue
		mColor.w = MathEngine::Clamp(a, 0.0f, 1.0f); //alpha

	}

	Color::Color(const vec4& color)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f

		mColor.x = MathEngine::Clamp(color.x, 0.0f, 1.0f); //red
		mColor.y = MathEngine::Clamp(color.y, 0.0f, 1.0f); //green
		mColor.z = MathEngine::Clamp(color.z, 0.0f, 1.0f); //blue
		mColor.w = MathEngine::Clamp(color.w, 0.0f, 1.0f); //alpha
	}

	const vec4& Color::GetColor() const
	{
		return mColor;
	}

	float Color::GetRed() const
	{
		return mColor.x;
	}

	float Color::GetGreen() const
	{
		return mColor.y;
	}

	float Color::GetBlue() const
	{
		return mColor.z;
	}

	float Color::GetAlpha() const
	{
		return mColor.w;
	}

	void Color::SetColor(const vec4& color)
	{
		//clamp to 0.0f if a component is < 0.0f
		//clamp to 1.0f if a component is > 1.0f

		mColor.x = MathEngine::Clamp(color.x, 0.0f, 1.0f); //red
		mColor.y = MathEngine::Clamp(color.y, 0.0f, 1.0f); //green
		mColor.z = MathEngine::Clamp(color.z, 0.0f, 1.0f); //blue
		mColor.w = MathEngine::Clamp(color.w, 0.0f, 1.0f); //alpha
	}

	void Color::SetRed(float r)
	{
		mColor.x = MathEngine::Clamp(r, 0.0f, 1.0f); //red
	}

	void Color::SetGreen(float g)
	{
		mColor.y = MathEngine::Clamp(g, 0.0f, 1.0f); //red
	}

	void Color::SetBlue(float b)
	{
		mColor.z = MathEngine::Clamp(b, 0.0f, 1.0f); //red
	}

	void Color::SetAlpha(float a)
	{
		mColor.w = MathEngine::Clamp(a, 0.0f, 1.0f); //red
	}

	Color& Color::operator+=(const Color& c)
	{
		this->mColor += c.mColor;

		mColor.x = MathEngine::Clamp(c.mColor.x, 0.0f, 1.0f); //red
		mColor.y = MathEngine::Clamp(c.mColor.y, 0.0f, 1.0f); //green
		mColor.z = MathEngine::Clamp(c.mColor.z, 0.0f, 1.0f); //blue
		mColor.w = MathEngine::Clamp(c.mColor.w, 0.0f, 1.0f); //alpha

		return *this;
	}

	Color& Color::operator-=(const Color& c)
	{
		this->mColor -= c.mColor;

		mColor.x = MathEngine::Clamp(c.mColor.x, 0.0f, 1.0f); //red
		mColor.y = MathEngine::Clamp(c.mColor.y, 0.0f, 1.0f); //green
		mColor.z = MathEngine::Clamp(c.mColor.z, 0.0f, 1.0f); //blue
		mColor.w = MathEngine::Clamp(c.mColor.w, 0.0f, 1.0f); //alpha

		return *this;
	}

	Color& Color::operator*=(float k)
	{
		if (k < 0.0f)
			return *this;

		mColor *= k;

		mColor.x = MathEngine::Clamp(mColor.x, 0.0f, 1.0f); //red
		mColor.y = MathEngine::Clamp(mColor.y, 0.0f, 1.0f); //green
		mColor.z = MathEngine::Clamp(mColor.z, 0.0f, 1.0f); //blue
		mColor.w = MathEngine::Clamp(mColor.w, 0.0f, 1.0f); //alpha

		return *this;
	}

	Color& Color::operator*=(const Color& c)
	{
		this->mColor.x = this->mColor.x * c.mColor.x;
		this->mColor.y = this->mColor.y * c.mColor.y;
		this->mColor.z = this->mColor.z * c.mColor.z;
		this->mColor.w = this->mColor.w * c.mColor.w;

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
		result.SetRed(c1.GetRed() * c2.GetRed());
		result.SetGreen(c1.GetGreen() * c2.GetGreen());
		result.SetBlue(c1.GetBlue() * c2.GetBlue());
		result.SetAlpha(c1.GetAlpha() * c2.GetAlpha());

		return result;
	}
}