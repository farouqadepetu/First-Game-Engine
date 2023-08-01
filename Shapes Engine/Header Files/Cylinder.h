#pragma once

#include "ThreeDimensionalShape.h"
#include "RenderingEngineUtility.h"

namespace ShapesEngine
{
	/** @class Cylinder ""
	*	@brief This class is used to render a cylinder.
	*/
	class Cylinder : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a Cylinder object.
		* @param[in] radius The radius of the cylinder.
		* @param[in] height The height of the cylinder.
		* @param[in] position The position of the cylinder.
		* @param[in] orientation The orientation of the cylinder.
		* @param[in] color The color of the cylinder.
		*/
		Cylinder(float radius, float height, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Initializes the properties of the cylinder.
		*
		* @param[in] width The radius of the cylinder.
		* @param[in] height The height of the cylinder.
		* @param[in] position The position of the cylinder.
		* @param[in] orientation The orientation of the cylinder.
		* @param[in] color The color of the cylinder.
		*/
		void InitializeCylinder(float radius, float height, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Returns the dimensions of the cylinder.
		* The x component is the radius, the y component is the height and the z component is the radius.
		*/
		vec3 GetDimensions() const override;

		/**@brief Sets the dimensions of the cylinder.
		* The x component should be the radius, the y component should be the height and the z component should be the radius.
		*/
		void SetDimensions(const vec3& dimensions) override;

		/**@brief Updates the cylinders model matrix.
		*/
		void UpdateModelMatrix() override;

		/**@brief Returns the volume of the cylinder.
		*/
		float Volume() const override;

	private:
		float mRadius;
		float mHeight;
	};
}