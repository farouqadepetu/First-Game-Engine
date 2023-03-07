#include "FAText.h"
#include "FADirectXException.h"

namespace FARender
{
	//----------------------------------------------------------------------------------------------------------------------
	//TEXT FUNCTION DEFINITIONS

	void Text::initialize(const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& deviceContext,
		const Microsoft::WRL::ComPtr<IDWriteFactory>& writeFactory,
		const D2D1_RECT_F& textLocation, const std::wstring& textString, float textSize, const D2D1_COLOR_F& textColor)
	{
		mTextLocation = textLocation;
		mText = textString;
		mTextSize = textSize;
		mTextColor = textColor;

		ThrowIfFailed(deviceContext->CreateSolidColorBrush(mTextColor,
			mDirect2DBrush.GetAddressOf()));

		ThrowIfFailed(writeFactory->CreateTextFormat(
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

	const D2D1_RECT_F& Text::textLocation()
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

	const D2D1_COLOR_F Text::textColor()
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

	void Text::changeTextSize(const Microsoft::WRL::ComPtr<IDWriteFactory>& writeFactory, float textSize)
	{
		mTextSize = textSize;

		ThrowIfFailed(writeFactory->CreateTextFormat(
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

	void Text::changeTextColor(const D2D1_COLOR_F& textColor)
	{
		mTextColor = textColor;

		mDirect2DBrush->SetColor(mTextColor);
	}

	void Text::changeTextString(const std::wstring& textString)
	{
		mText = textString;
	}

	void Text::changeTextLocation(const D2D1_RECT_F& textLocation)
	{
		mTextLocation = textLocation;
	}

	//----------------------------------------------------------------------------------------------------------------------
}