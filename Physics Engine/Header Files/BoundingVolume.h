#pragma once

#include "Color.h"
#include "DrawArguments.h"
#include "RenderingEngineUtility.h"

namespace PhysicsEngine
{
	class BoundingVolumeAbstract
	{
	public:

		/**@brief Updates a bounding volumes model matrix.
		*/
		virtual void UpdateModelMatrix() = 0;

		/**@brief Transforms the bounding volume from local space to world space.
		*/
		virtual void TransformBoundingVolume(const mat4& model) = 0;

		/**@brief Returns the color of a bounding volume.
		*/
		virtual const RenderingEngine::Color& GetColor() const;

		/**@brief Returns the draw arguments of a bounding volume.
		*/
		virtual const RenderingEngine::DrawArguments& GetDrawArguments() const;

		/**@brief Returns the model matrix of a bounding volume.
		*/
		virtual const mat4& GetModelMatrix() const;

		/**@brief Returns the position of a bounding volume.
		*/
		virtual const vec3& GetPosition() const;

		/**@brief Returns the orientation of a bounding volume.
		*/
		virtual const MathEngine::Quaternion& GetOrientation() const;

		/**@brief Sets the position of a bounding volume.
		*/
		virtual void SetPosition(const vec3& position);

		/**@brief Sets the orientation of a bounding volume.
		*/
		virtual void SetOrientation(const MathEngine::Quaternion& orientation);

		/**@brief Sets the color of a bounding volume.
		*/
		virtual void SetColor(const RenderingEngine::Color& color);

		/**@brief Sets the draw arguments of a bounding volume.
		*/
		virtual void SetDrawArguments(const RenderingEngine::DrawArguments& drawArgs);

	protected:
		RenderingEngine::RenderObject mRenderObject;
	};
}