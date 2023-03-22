#pragma once

#include "FAMathEngine.h"

/** @file FAColor.h
*	@brief File has class Color under namespace FAColor.
*/

namespace FAColor
{
	/** @class Color""
	*	@brief This class stores a RGBA color in a 4D vector using floats.
	*	The range of each component is [0.0, 1.0]. The first componet is red, second component is green,
	*	third component is blue and the 4th component is alpha.
	*/
	class Color
	{
	public:

		/**@brief Default Constructor.
		* Initializes the color to the specified RGBA values.
		*/
		Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

		/**@brief Overloaded Constructor.
		* Initializes the color to the specified color.
		*/
		Color(const FAMath::Vector4D& color);

		/**@brief Returns the color.
		*/
		const FAMath::Vector4D& GetColor() const;

		/**@brief Returns the value of the red component.
		*/
		float GetRed() const;

		/**@brief Returns the value of the blue component.
		*/
		float GetGreen() const;

		/**@brief Returns the value of the green component.
		*/
		float GetBlue() const;

		/**@brief Returns the value of the alpha component.
		*/
		float GetAlpha() const;

		/**@brief Sets the color to the specified color.
		*/
		void SetColor(const FAMath::Vector4D& color);

		/**@brief Sets the red component to the specified float value.
		*/
		void SetRed(float r);

		/**@brief Sets the green component to the specified float value.
		*/
		void SetGreen(float g);

		/**@brief Sets the blue component to the specified float value.
		*/
		void SetBlue(float b);

		/**@brief Sets the alpha component to the specified float value.
		*/
		void SetAlpha(float a);

		/**@brief Adds this objects color to the specified color and stores the result in this object.
		* Does component-wise addtion. If any of the resultant components are > 1.0f, they are set to 1.0f.
		*/
		Color& operator+=(const Color& c);

		/**@brief Subtracts the specified color from this objects color and stores the result in this object.
		* Does component-wise subtraction. If any of the resultant components are < 0.0f, they are set to 0.0f.
		*/
		Color& operator-=(const Color& c);

		/**@brief Multiplies this objects color by the specified float value k and stores the result in this object.
		*	If k < 0.0f, no multiplication happens and this objects color does not get modified.\n
		*	If any of the resultant components are > 1.0f, they are set to 1.0f.\n
		*/
		Color& operator*=(float k);

		/**@brief Multiplies this objects color by the specified color c and stores the result in this object.
		*  If any of the resultant components are > 1.0f, they are set to 1.0f.\n
		*	Does component-wise multiplication.
		*/
		Color& operator*=(const Color& c);

	private:
		FAMath::Vector4D mColor;
	};

	/**@brief Returns the result of c1 + c2.
	* Does component-wise addtion. If any of the resultant components are > 1.0f, they are set to 1.0f.
	*/
	Color operator+(const Color& c1, const Color& c2);

	/**@brief Returns the result of c1 - c2.
	* Does component-wise subtraction. If any of the resultant components are < 0.0f, they are set to 0.0f.
	*/
	Color operator-(const Color& c1, const Color& c2);

	/**@brief Returns the result of c * k.
	*	If k < 0.0f, no multiplication happens and Color c is returned.\n
	*	If any of the resultant components are > 1.0f, they are set to 1.0f.\n
	*/
	Color operator*(const Color& c, float k);

	/**@brief Returns the result of k * c.
	*	If k < 0.0f, no multiplication happens and Color c is returned.\n
	*	If any of the resultant components are > 1.0f, they are set to 1.0f.\n
	*/
	Color operator*(float k, const Color& c);

	/**@brief Returns the result of c1 * c2.
	*	If any of the resultant components are > 1.0f, they are set to 1.0f.\n
	*/
	Color operator*(const Color& c1, const Color& c2);
}