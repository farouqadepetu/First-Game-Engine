#pragma once

#include "ThreeDimensionalShape.h"
#include "RenderingEngineUtility.h"

namespace ShapesEngine
{
	/** @class Cone ""
	*	@brief This class is used to render a cone.
	*/
	class Cone : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a Cone object.
		* @param[in] radius The radius of the cone.
		* @param[in] height The height of the cone.
		* @param[in] position The position of the cone.
		* @param[in] orientation The orientation of the cone.
		* @param[in] color The color of the cone.
		*/
		Cone(float radius, float height, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Initializes the properties of the cone.
		*
		* @param[in] width The radius of the cone.
		* @param[in] height The height of the cone.
		* @param[in] position The position of the cone.
		* @param[in] orientation The orientation of the cone.
		* @param[in] color The color of the cone.
		*/
		void InitializeCone(float radius, float height, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Returns the dimensions of the cone.
		* The x component is the radius, the y component is the height and the z component is the radius.
		*/
		vec3 GetDimensions() const override;

		/**@brief Sets the dimensions of the cone.
		* The x component should be the radius, the y component should be the height and the z component should be the radius.
		*/
		void SetDimensions(const vec3& dimensions) override;

		/**@brief Updates the cones model matrix.
		*/
		void UpdateModelMatrix() override;

		/**@brief Returns the volume of the cone.
		*/
		float Volume() const override;

	private:
		float mRadius;
		float mHeight;
	};
}