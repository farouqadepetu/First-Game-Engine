#pragma once


#include <string>
#include "Color.h"

namespace RenderingEngine
{
	/** @class Text ""
	*	@brief This class is used to help render text.
	*	Stores the location of the text, the text string, text size and the color of the text.
	*/
	class Text
	{
	public:

		Text() = default;

		/**@brief Constructs a Text object.
		*
		* For text location the first two values in the vector is the top-left location of the rectangle and
		* the last two values are the bottom-right location of the rectangle.
		*
		* @param[in] textLocation The location of the text on the window.
		* @param[in] textString The text to render.
		* @param[in] textSize How big the text is.
		* @param[in] textColor The color of the text.
		*/
		Text(const vec4& textLocation, const std::wstring& textString, float textSize, const Color& textColor);

		/**@brief Returns a constant reference to the text location.
		*/
		const vec4& GetTextLocation() const;

		/**@brief Returns a constant reference to the text string.
		*/
		const std::wstring& GetTextString() const;

		/**@brief Returns the text size.
		*/
		float GetTextSize() const;

		/**@brief Returns a constant reference to the text color.
		*/
		const Color& GetTextColor() const;

		/**@brief Changes the text size to the specified \a textSize.
		*/
		void SetTextSize(float textSize);

		/**@brief Changes the text color to the specified \a textColor.
		*/
		void SetTextColor(const Color& textColor);

		/**@brief Changes the text string to the specified \a textString.
		*/
		void SetTextString(const std::wstring& textString);

		/**@brief Changes the text location to the specified \a textLocation.
		*/
		void SetTextLocation(const vec4& textLocation);

	private:

		vec4 mTextLocation;
		std::wstring mText;
		float mTextSize{ 0.0f };
		Color mTextColor;
	};
}