#include "FAText.h"

namespace FARender
{
	//----------------------------------------------------------------------------------------------------------------------
	//TEXT FUNCTION DEFINITIONS

	Text::Text(const FAMath::Vector4D& textLocation, const std::wstring& textString, float textSize, const FAColor::Color& textColor)
		: mTextLocation{ textLocation }, mText{ textString }, mTextSize{ textSize }, mTextColor{ textColor }
	{}

	const FAMath::Vector4D& Text::GetTextLocation() const
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

	const FAColor::Color& Text::GetTextColor() const
	{
		return mTextColor;
	}

	void Text::SetTextSize( float textSize)
	{
		mTextSize = textSize;
	}

	void Text::SetTextColor(const FAColor::Color& textColor)
	{
		mTextColor = textColor;
	}

	void Text::SetTextString(const std::wstring& textString)
	{
		mText = textString;
	}

	void Text::SetTextLocation(const FAMath::Vector4D& textLocation)
	{
		mTextLocation = textLocation;
	}

	//----------------------------------------------------------------------------------------------------------------------
}