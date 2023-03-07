#pragma once

/** @file FAText.h
*	@brief File has class Text under namespace FARender.
*/

#include <wrl.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <string>

namespace FARender
{
	/** @class Text ""
	*	@brief This class is used to help render text.
	*	Stores the location of the text, the text string, text size and color of the text.
	*/
	class Text
	{
	public:

		Text() = default;

		/**@brief Initializes the format of the text.
		*/
		void initialize(const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& deviceContext,
			const Microsoft::WRL::ComPtr<IDWriteFactory>& writeFactory,
			const D2D1_RECT_F& textLocation, const std::wstring& textString, float textSize, const D2D1_COLOR_F& textColor);

		/**@brief Returns a constant reference to the text location.
		*/
		const D2D1_RECT_F& textLocation();

		/**@brief Returns a constant reference to the text string.
		*/
		const std::wstring& textString();

		/**@brief Returns a constant reference to the text size.
		*/
		const float& textSize();

		/**@brief Returns a constant reference to the color brush.
		*/
		const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& brush();

		/**@brief Returns a constant reference to the format of the text.
		*/
		const Microsoft::WRL::ComPtr<IDWriteTextFormat>& format();

		/**@brief Returns a constant reference to the text color.
		*/
		const D2D1_COLOR_F textColor();

		/**@brief Changes the text size to the specified size.
		*/
		void changeTextSize(const Microsoft::WRL::ComPtr<IDWriteFactory>& mDirectWriteFactory, float textSize);

		/**@brief Changes the text color to the specified color.
		*/
		void changeTextColor(const D2D1_COLOR_F& textColor);

		/**@brief Changes the text string to the specified string.
		*/
		void changeTextString(const std::wstring& textString);

		/**@brief Changes the text location to the specified location.
		*/
		void changeTextLocation(const D2D1_RECT_F& textLocation);

	private:

		D2D1_RECT_F mTextLocation;
		std::wstring mText;
		float mTextSize;
		D2D1_COLOR_F mTextColor;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mDirect2DBrush;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> mDirectWriteFormat;
	};
}