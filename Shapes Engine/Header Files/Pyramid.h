#pragma once

#include "ThreeDimensionalShape.h"
#include "RenderingEngineUtility.h"

namespace ShapesEngine
{
	/** @class Pyramid ""
	*	@brief This class is used to render a pyramid.
	*/
	class Pyramid : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a Pyramid object.
		* @param[in] width The width of the pyramid.
		* @param[in] height The height of the pyramid.
		* @param[in] depth The depth of the pyramid.
		* @param[in] position The position of the pyramid.
		* @param[in] orientation The orientation of the pyramid.
		* @param[in] color The color of the pyramid.
		*/
		Pyramid(float width, float height, float depth, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Initializes the properties of the pyramid.
		*
		* @param[in] width The width of the pyramid.
		* @param[in] height The height of the pyramid.
		* @param[in] depth The depth of the pyramid.
		* @param[in] position The position of the pyramid.
		* @param[in] orientation The orientation of the pyramid.
		* @param[in] color The color of the pyramid.
		*/
		void InitializePyramid(float width, float height, float depth, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Returns the dimensions of the pyramid.
		* The x component is the width, the y component is the height and the z component is the depth.
		*/
		vec3 GetDimensions() const override;

		/**@brief Sets the dimensions of the pyramid.
		* The x component should be the width, the y component should be the height and the z component should be the depth.
		*/
		void SetDimensions(const vec3& dimensions) override;

		/**@brief Updates the pyramids model matrix.
		*/
		void UpdateModelMatrix() override;

		/**@brief Returns the volume of the pyramid.
		*/
		float Volume() const override;

	private:
		float mWidth;
		float mHeight;
		float mDepth;
	};
}