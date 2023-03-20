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

		D2D1_COLOR_F tColor{ textColor.getRed(), textColor.getGreen(), textColor.getBlue(), textColor.getAlpha() };
		ThrowIfFailed(deviceResources.device2DContext()->CreateSolidColorBrush(tColor,
			mDirect2DBrush.GetAddressOf()));

		ThrowIfFailed(deviceResources.directWriteFactory()->CreateTextFormat(
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


	void Text::initialize(const DeviceResources& deviceResources,
		const FAMath::Vector4D& textLocation, const std::wstring& textString, float textSize, const FAColor::Color& textColor)
	{
		mTextLocation = textLocation;
		mText = textString;
		mTextSize = textSize;
		mTextColor = textColor;

		D2D1_COLOR_F tColor{ textColor.getRed(), textColor.getGreen(), textColor.getBlue(), textColor.getAlpha() };
		ThrowIfFailed(deviceResources.device2DContext()->CreateSolidColorBrush(tColor,
			mDirect2DBrush.GetAddressOf()));

		ThrowIfFailed(deviceResources.directWriteFactory()->CreateTextFormat(
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

	const FAMath::Vector4D& Text::textLocation()
	{
		return mTextLocation;
	}

	const std::wstring& Text::textString()
	{
		return mText;
	}

	const float& Text::textSize()
	{
		return mTextSize;
	}

	const FAColor::Color& Text::textColor()
	{
		return mTextColor;
	}

	const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& Text::brush()
	{
		return mDirect2DBrush;
	}

	const Microsoft::WRL::ComPtr<IDWriteTextFormat>& Text::format()
	{
		return mDirectWriteFormat;
	}

	void Text::setTextSize(const DeviceResources& deviceResources, float textSize)
	{
		mTextSize = textSize;

		ThrowIfFailed(deviceResources.directWriteFactory()->CreateTextFormat(
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

	void Text::setTextColor(const FAColor::Color& textColor)
	{
		mTextColor = textColor;
		D2D1_COLOR_F tColor{ textColor.getRed(), textColor.getGreen(), textColor.getBlue(), textColor.getAlpha() };
		mDirect2DBrush->SetColor(tColor);
	}

	void Text::setTextString(const std::wstring& textString)
	{
		mText = textString;
	}

	void Text::setTextLocation(const FAMath::Vector4D& textLocation)
	{
		mTextLocation = textLocation;
	}

	//----------------------------------------------------------------------------------------------------------------------
}