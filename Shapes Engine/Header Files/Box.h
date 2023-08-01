#pragma once

#include "ThreeDimensionalShape.h"
#include "RenderingEngineUtility.h"

namespace ShapesEngine
{
	/** @class Box ""
	*	@brief This class is used to render a box.
	*/
	class Box : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a Box object.
		* @param[in] width The width of the box.
		* @param[in] height The height of the box.
		* @param[in] depth The depth of the box.
		* @param[in] position The position of the box.
		* @param[in] orientation The orientation of the box.
		* @param[in] color The color of the box.
		*/
		Box(float width, float height, float depth, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Initializes the properties of the box.
		*
		* @param[in] width The width of the box.
		* @param[in] height The height of the box.
		* @param[in] depth The depth of the box.
		* @param[in] position The position of the box.
		* @param[in] orientation The orientation of the box.
		* @param[in] color The color of the box.
		*/
		void InitializeBox(float width, float height, float depth, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Returns the dimensions of the box.
		* The x component is the width, the y component is the height and the z component is the depth.
		*/
		vec3 GetDimensions() const override;

		/**@brief Sets the dimensions of the box.
		* The x component should be the width, the y component should be the height and the z component should be the depth.
		*/
		void SetDimensions(const vec3& dimensions) override;

		/**@brief Updates the boxs model matrix.
		*/
		void UpdateModelMatrix() override;

		/**@brief Returns the volume of the box.
		*/
		float Volume() const override;

	private:
		float mWidth;
		float mHeight;
		float mDepth;
	};
}