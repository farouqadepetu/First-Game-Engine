#pragma once

#include "Color.h"
#include "DrawArguments.h"
#include "RenderingEngineUtility.h"
#include "Vertex.h"
#include <vector>

/** @namespace ShapesEngine
* @brief An engine for rendering 3D shapes.
*/
namespace ShapesEngine
{
	/** @class ThreeDimensionalShapeAbstract ""
	*	@brief An abstract class for 3D shapes.
	*/
	class ThreeDimensionalShapeAbstract
	{
	public:
		/**@brief Updates a 3D shapes model matrix.
		*/
		virtual void UpdateModelMatrix() = 0;

		/**@brief Returns a 3D shapes volume.
		*/
		virtual float Volume() const = 0;

		/**@brief Returns the dimensions of a 3D shape.
		*/
		virtual vec3 GetDimensions() const = 0;

		/**@brief Sets the dimensions of a 3D shape.
		*/
		virtual void SetDimensions(const vec3& dimensions) = 0;

		/**@brief Returns the color of a 3D shape.
		*/
		virtual const RenderingEngine::Color& GetColor() const;

		/**@brief Returns the draw arguments of a 3D shape.
		*/
		virtual const RenderingEngine::DrawArguments& GetDrawArguments() const;

		/**@brief Returns the model matrix of a 3D shape.
		*/
		virtual const mat4& GetModelMatrix() const;

		/**@brief Returns the position of a 3D shape.
		*/
		virtual const vec3& GetPosition() const;

		/**@brief Returns the orientation of a 3D shape.
		*/
		virtual const MathEngine::Quaternion& GetOrientation() const;

		/**@brief Sets the position of a 3D shape.
		*/
		virtual void SetPosition(const vec3& position);

		/**@brief Sets the orientation of a 3D shape.
		*/
		virtual void SetOrientation(const MathEngine::Quaternion& orientation);

		/**@brief Sets the color of a 3D shape.
		*/
		virtual void SetColor(const RenderingEngine::Color& color);

		/**@brief Sets the draw arguments of a 3D shape.
		*/
		virtual void SetDrawArguments(const RenderingEngine::DrawArguments& drawArgs);

	protected:
		RenderingEngine::RenderObject mRenderObject;
	};

}