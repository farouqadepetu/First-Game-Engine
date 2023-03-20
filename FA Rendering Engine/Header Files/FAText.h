#pragma once

/** @file FAText.h
*	@brief File has class Text under namespace FARender.
*/

#include <string>
#include "FADeviceResources.h"
#include "FAColor.h"

namespace FARender
{
	/** @class Text ""
	*	@brief This class is used to help render text.
	*	Stores the location of the text, the text string, text size and the color of the text.
	*/
	class Text
	{
	public:

		/**@brief Default Constructor.
		*/
		Text();

		/**@brief Overloaded Constructor.
		* Initializes the format of the text.\n
		* For text location the first two values in the vector is the top-left location of the rectangle and
		* the last two values are the bottom-right location of the rectangle.
		*/
		Text(const DeviceResources& deviceResources,
			const FAMath::Vector4D& textLocation, const std::wstring& textString, float textSize, const FAColor::Color& textColor);

		/**@brief Initializes the format of the text.
		* For text location the  first two values in the vector is the top-left location of the rectangle and
		* the last two values are the bottom-right location of the rectangle.
		*/
		void initialize(const DeviceResources& deviceResources,
			const FAMath::Vector4D& textLocation, const std::wstring& textString, float textSize, const FAColor::Color& textColor);

		/**@brief Returns a constant reference to the text location.
		*/
		const FAMath::Vector4D& textLocation();

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
		const FAColor::Color& textColor();

		/**@brief Changes the text size to the specified size.
		*/
		void setTextSize(const DeviceResources& deviceResources, float textSize);

		/**@brief Changes the text color to the specified color.
		*/
		void setTextColor(const FAColor::Color& textColor);

		/**@brief Changes the text string to the specified string.
		*/
		void setTextString(const std::wstring& textString);

		/**@brief Changes the text location to the specified location.
		*/
		void setTextLocation(const FAMath::Vector4D& textLocation);

	private:

		FAMath::Vector4D mTextLocation;
		std::wstring mText;
		float mTextSize;
		FAColor::Color mTextColor;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mDirect2DBrush;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> mDirectWriteFormat;
	};
}