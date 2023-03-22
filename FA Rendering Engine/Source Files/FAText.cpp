#include "FAText.h"
#include "FADirectXException.h"

namespace FARender
{
	//----------------------------------------------------------------------------------------------------------------------
	//TEXT FUNCTION DEFINITIONS

	Text::Text() : mTextSize{ 0.0f }
	{}

	Text::Text(const DeviceResources& deviceResources,
		const FAMath::Vector4D& textLocation, const std::wstring& textString, float textSize, const FAColor::Color& textColor)
	{
		mTextLocation = textLocation;
		mText = textString;
		mTextSize = textSize;
		mTextColor = textColor;

		D2D1_COLOR_F tColor{ textColor.GetRed(), textColor.GetGreen(), textColor.GetBlue(), textColor.GetAlpha() };
		ThrowIfFailed(deviceResources.GetDevice2DContext()->CreateSolidColorBrush(tColor,
			mDirect2DBrush.GetAddressOf()));

		ThrowIfFailed(deviceResources.GetDirectWriteFactory()->CreateTextFormat(
			L"Verdana",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			mTextSize,
			L"en-us",
			&mDirectWriteFormat));

		ThrowIfFailed(mDirectWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
		ThrowIfFailed(mDirectWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	}


	void Text::Initialize(const DeviceResources& deviceResources,
		const FAMath::Vector4D& textLocation, const std::wstring& textString, float textSize, const FAColor::Color& textColor)
	{
		mTextLocation = textLocation;
		mText = textString;
		mTextSize = textSize;
		mTextColor = textColor;

		D2D1_COLOR_F tColor{ textColor.GetRed(), textColor.GetGreen(), textColor.GetBlue(), textColor.GetAlpha() };
		ThrowIfFailed(deviceResources.GetDevice2DContext()->CreateSolidColorBrush(tColor,
			mDirect2DBrush.GetAddressOf()));

		ThrowIfFailed(deviceResources.GetDirectWriteFactory()->CreateTextFormat(
			L"Verdana",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			mTextSize,
			L"en-us",
			&mDirectWriteFormat));

		ThrowIfFailed(mDirectWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
		ThrowIfFailed(mDirectWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	}

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

	const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& Text::GetBrush() const
	{
		return mDirect2DBrush;
	}

	const Microsoft::WRL::ComPtr<IDWriteTextFormat>& Text::GetFormat() const
	{
		return mDirectWriteFormat;
	}

	void Text::SetTextSize(const DeviceResources& deviceResources, float textSize)
	{
		mTextSize = textSize;

		ThrowIfFailed(deviceResources.GetDirectWriteFactory()->CreateTextFormat(
			L"Verdana",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			mTextSize,
			L"en-us",
			&mDirectWriteFormat));

		ThrowIfFailed(mDirectWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
		ThrowIfFailed(mDirectWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	}

	void Text::SetTextColor(const FAColor::Color& textColor)
	{
		mTextColor = textColor;
		D2D1_COLOR_F tColor{ textColor.GetRed(), textColor.GetGreen(), textColor.GetBlue(), textColor.GetAlpha() };
		mDirect2DBrush->SetColor(tColor);
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