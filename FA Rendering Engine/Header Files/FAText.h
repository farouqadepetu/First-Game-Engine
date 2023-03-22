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
		void Initialize(const DeviceResources& deviceResources,
			const FAMath::Vector4D& textLocation, const std::wstring& textString, float textSize, const FAColor::Color& textColor);

		/**@brief Returns a constant reference to the text location.
		*/
		const FAMath::Vector4D& GetTextLocation() const;

		/**@brief Returns a constant reference to the text string.
		*/
		const std::wstring& GetTextString() const;

		/**@brief Returns the text size.
		*/
		float GetTextSize() const;

		/**@brief Returns a constant reference to the color brush.
		*/
		const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& GetBrush() const;

		/**@brief Returns a constant reference to the format of the text.
		*/
		const Microsoft::WRL::ComPtr<IDWriteTextFormat>& GetFormat() const;

		/**@brief Returns a constant reference to the text color.
		*/
		const FAColor::Color& GetTextColor() const;

		/**@brief Changes the text size to the specified size.
		*/
		void SetTextSize(const DeviceResources& deviceResources, float textSize);

		/**@brief Changes the text color to the specified color.
		*/
		void SetTextColor(const FAColor::Color& textColor);

		/**@brief Changes the text string to the specified string.
		*/
		void SetTextString(const std::wstring& textString);

		/**@brief Changes the text location to the specified location.
		*/
		void SetTextLocation(const FAMath::Vector4D& textLocation);

	private:

		FAMath::Vector4D mTextLocation;
		std::wstring mText;
		float mTextSize;
		FAColor::Color mTextColor;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mDirect2DBrush;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> mDirectWriteFormat;
	};
}