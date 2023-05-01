#pragma once

/** @file FAProjection.h
*	@brief File that has namespace FAProjection. Within the namespace is the interface IProjection and class PerspectiveProjection.
*/

#include "FAMathEngine.h"

/** @namespace FAProjection
*	@brief Within the namespace is the interface IProjection and class PerspectiveProjection.
*/
namespace FAProjection
{
	/** @class IProjection ""
	*	@brief An interface for projections.
	*/
	class IProjection
	{
	public:

		/**@brief Default Constructor.
		*/
		IProjection();

		/**@brief Returns the projection matrix.
		*/
		const FAMath::Matrix4x4& GetProjectionMatrix() const;

		virtual void UpdateProjectionMatrix() = 0;

	protected:
		FAMath::Matrix4x4 mProjectionMatrix;
		bool mUpdateProjectionMatrix;
	};

	/** @class PerspectiveProjection ""
	*	@brief A class for doing perspective projection.
	*/
	class PerspectiveProjection : public IProjection
	{
	public:

		/**@brief Initializes all properties to 0.0f.
		*/
		PerspectiveProjection();

		/**@brief Initializes all the properties to the specified values.
		* 
		* @param[in] znear The z value of where the near plane of the frustrum intersects the z-axis.
		* @param[in] zfar The z value of where the far plane of the frustrum intersects the z-axis.
		* @param[in] vFov The vertical field of view of the frustrum.
		* @param[in] aspectRatio The aspect ratio of the view plane.
		*/
		PerspectiveProjection(float znear, float zfar, float vFov, float aspectRatio);

		/**@brief Sets all the properties to the specified values.
		*
		* @param[in] znear The z value of where the near plane of the frustrum intersects the z-axis.
		* @param[in] zfar The z value of where the far plane of the frustrum intersects the z-axis.
		* @param[in] vFov The vertical field of view of the frustrum.
		* @param[in] aspectRatio The aspect ratio of the view plane.
		*/
		void SetProperties(float znear, float zfar, float vFov, float aspectRatio);

		/**@brief Returns the near value of the frustrum.
		*/
		float GetNear() const;

		/**@brief Returns the far value of the frustrum.
		*/
		float GetFar() const;

		/**@brief Returns the vertical field of view of the frustrum in degrees.
		*/
		float GetVerticalFov() const;

		/**@brief Returns the aspect ratio of the frustrum.
		*/
		float GetAspectRatio() const;

		/**@brief Sets the camera's near plane value to the specified value.
		*/
		void SetNear(float near);

		/**@brief Sets the camera's far plane value to the specified value.
		*/
		void SetFar(float far);

		/**@brief Sets the camera's vertical field of view to the specified vertical field of view .
		*/
		void SetVerticalFov(float fov);

		/**@brief Sets the camera's aspect ratio to the specified aspect ratio.
		*/
		void SetAspectRatio(float ar);

		/**@brief Updates the projection matrix if any of the properties have changed.
		*/
		void UpdateProjectionMatrix() override final;

	private:
		//frustrum properties
		float mNear;
		float mFar;
		float mVerticalFov;
		float mAspectRatio;
	};
}