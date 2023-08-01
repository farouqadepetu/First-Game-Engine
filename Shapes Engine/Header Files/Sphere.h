#pragma once

#include "ThreeDimensionalShape.h"
#include "RenderingEngineUtility.h"

namespace ShapesEngine
{
	/** @class Sphere ""
	*	@brief This class is used to render a sphere.
	*/
	class Sphere : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a Sphere object.
		* Call InitializeSphere to initialize the sphere.
		*/
		Sphere(float radius, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Initializes the properties of the sphere.
		*
		* @param[in] radius The radius of the sphere.
		* @param[in] position The position of the sphere.
		* @param[in] orientation The orientation of the sphere.
		* @param[in] color The color of the sphere.
		*/
		void InitializeSphere(float radius, const vec3& position, const MathEngine::Quaternion& orientation,
			const RenderingEngine::Color& color);

		/**@brief Returns the dimensions of the sphere.
		* The x component is the radius, the y component is the radius and the z component is the radius.
		*/
		vec3 GetDimensions() const override;

		/**@brief Sets the dimensions of the sphere.
		* The x component should be the radius, the y component should be the radius and the z component should be the radius.
		*/
		void SetDimensions(const vec3& dimensions) override;

		/**@brief Updates the spheres model matrix.
		*/
		void UpdateModelMatrix() override;

		/**@brief Returns the volume of the sphere.
		*/
		float Volume() const override;

	private:
		float mRadius;
	};
}