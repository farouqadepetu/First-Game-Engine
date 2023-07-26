#include "Text.h"

namespace RenderingEngine
{
	//----------------------------------------------------------------------------------------------------------------------
	//TEXT FUNCTION DEFINITIONS

	Text::Text(const vec4& textLocation, const std::wstring& textString, float textSize, const RenderingEngine::Color& textColor)
		: mTextLocation{ textLocation }, mText{ textString }, mTextSize{ textSize }, mTextColor{ textColor }
	{}

	const vec4& Text::GetTextLocation() const
	{
		return mTextLocation;
	}

	const std::wstring& Text::GetTextString() const
	{
		return mText;
	}

	float Text::GetTextSize() const
	{
		return mTextSize;
	}

	const Color& Text::GetTextColor() const
	{
		return mTextColor;
	}

	void Text::SetTextSize(float textSize)
	{
		mTextSize = textSize;
	}

	void Text::SetTextColor(const Color& textColor)
	{
		mTextColor = textColor;
	}

	void Text::SetTextString(const std::wstring& textString)
	{
		mText = textString;
	}

	void Text::SetTextLocation(const vec4& textLocation)
	{
		mTextLocation = textLocation;
	}

	//----------------------------------------------------------------------------------------------------------------------
}