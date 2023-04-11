#pragma once

/** @file FACamera.h
*	@brief File that has namespace FACamera. Withn the namespace is the class Camera.
*/

/**
*										FACAMERA_H FILE
*
*/

#include "FAMathEngine.h"
#include <Windows.h>

typedef FAMath::Vector2D vec2;
typedef FAMath::Vector3D vec3;
typedef FAMath::Vector4D vec4;
typedef FAMath::Matrix4x4 mat4;

/** @namespace FACamera
*	@brief Has Camera class.
*/
namespace FACamera
{
	/** @class Camera ""
	*	@brief Simple first person style camera class that lets the viewer explore the 3D scene.\n
	*   It keeps track of the camera coordinate system relative to the world space so that the view matrix can be constructed.\n  
	*	It keeps track of the viewing frustum of the camera so that the projection matrix can be obtained.\n
	*/
	class Camera
	{
	public:
		/**@brief Creates a new camera.
		*
		* @param[in] camerPosition The position of the camera.
		* @param[in] x The x axis of the local coordinate system of the camera.
		* @param[in] y The y axis of the local coordinate system of the camera.
		* @param[in] z The z axis of the local coordinate system of the camera.
		* @param[in] znear The distance of the near plane from the camera.
		* @param[in] zfar The distance of the far plane from the camera.
		* @param[in] aspectRatio The aspect ratio of the view plane.
		* @param[in] vFov The vertical field of view of the frustrum.
		* @param[in] cameraVelocity The translational velocity of the camera.
		* @param[in] angularVelocity The angular velocity of the camera.
		*/
		Camera(vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f), 
			vec3 x = vec3(1.0f, 0.0f, 0.0f), vec3 y = vec3(0.0f, 1.0f, 0.0f), vec3 z = vec3(0.0f, 0.0f, 1.0f),
			float znear = 1.0f, float zfar = 100.f, float aspectRatio = 1.0f, float vFov = 45.0f, 
			float cameraVelocity = 10.0f, float angularVelocity = 0.25f);

		/**@brief Returns a constant reference to the position of the camera in world coordinates.
		*/
		const vec3& GetCameraPosition() const;

		/**@brief Returns a constant reference to the x-axis of the camera.
		*/
		const vec3& GetX() const;
		
		/**@brief Returnsa constant reference to the y-axis of the camera.
		*/
		const vec3& GetY() const;

		/**@brief Returns a constant reference to the z-axis of the camera.
		*/
		const vec3& GetZ() const;

		/**@brief Returns a constant reference to the view transformation matrix of this camera.
		*/
		const mat4& GetViewMatrix() const;

		/**@brief Returns the camera's velocity.
		*/
		float GetCameraVelocity() const;

		/**@brief Returns the camera's angular velocity.
		*/
		float GetAngularVelocity() const;
		
		/**@brief Defines the camera space using UVN.
		* 
		* @param[in] camerPosition The position of the camera.
		* @param[in] target The point the camera is looking at.
		* @param[in] up The up direction of the world.
		*/
		void LookAt(vec3 cameraPosition, vec3 target, vec3 up);

		/**@brief Returns the near value of the frustrum.
		*/
		float GetZNear() const;

		/**@brief Returns the far value of the frustrum.
		*/
		float GetZFar() const;

		/**@brief Returns the vertical field of view of the frustrum in degrees.
		*/
		float GetVerticalFov() const;

		/**@brief Returns the aspect ratio of the frustrum.
		*/
		float GetAspectRatio() const;

		/**@brief Sets the camera's position to the specified position.
		*/
		void SetCameraPosition(const vec3& position);

		/**@brief Sets the camera's x-axis to the specified vector \a x.
		*/
		void SetX(const vec3& x);

		/**@brief Sets the camera's y-axis to the specified vector \a y.
		*/
		void SetY(const vec3& y);

		/**@brief Sets the camera's z-axis to the specified vector \a z.
		*/
		void SetZ(const vec3& z);

		/**@brief Sets the camera's velocity to the specified \a velocity.
		*/
		void SetCameraVelocity(float velocity);

		/**@brief Sets the camera's angular velocity to the specified \a angular \a velocity.
		*/
		void SetAngularVelocity(float velcoity);

		/**@brief Sets the camera's near plane value to the specified value.
		*/
		void SetZNear(float znear);

		/**@brief Sets the camera's far plane value to the specified value.
		*/
		void SetZFar(float zfar);

		/**@brief Sets the camera's vertical field of view to the specified vertical field of view .
		*/
		void SetVerticalFov(float fov);

		/**@brief Sets the camera's aspect ratio to the specified aspect ratio.
		*/
		void SetAspectRatio(float ar);

		/**@brief Returns a constant reference to the perspective projection transformation matrix of this camera.
		*/
		const mat4& GetPerspectiveProjectionMatrix() const;

		/**@brief Returns a constant reference to the view perspective projection transformation matrix of this camera.
		*/
		const mat4& GetViewPerspectiveProjectionMatrix() const;

		/**@brief After modifying the camera position and/or orientation, call this to rebuild the view transformation matrix.
		*/
		void UpdateViewMatrix();

		/**@brief After modifying any of the frustrum properties, call this to rebuild the perspective projection transformation matrix.
		*/
		void UpdatePerspectiveProjectionMatrix();

		/**@brief Updates the camera's view perspective projection matrix.
		* 
		* Call this to rebuild the view perspective projection transformation matrix.
		*/
		void UpdateViewPerspectiveProjectionMatrix();

		/**@brief Moves the camera left along the camera's x-axis.
		*/
		void Left(float dt);

		/**@brief Moves the camera right along the camera's x-axis.
		* 
		*  @param[in] dt The time between frames.
		*/
		void Right(float dt);

		/**@brief Moves the camera foward along the camera's z-axis.
		* 
		* @param[in] dt The time between frames.
		*/
		void Foward(float dt);

		/**@brief Moves the camera backward along the camera's z-axis.
		* 
		* @param[in] dt The time between frames.
		*/
		void Backward(float dt);

		/**@brief Moves the camera up along the camera's y-axis.
		* 
		* @param[in] dt The time between frames.
		*/
		void Up(float dt);

		/**@brief Moves the camera down along the camera's y-axis.
		* 
		* @param[in] dt The time between frames.
		*/
		void Down(float dt);

		/**@brief Rotates the camera to look left and right.
		* 
		* @param[in] xDiff How many degrees to rotate.
		*/
		void RotateCameraLeftRight(float xDiff);

		/**@brief Rotates the camera to look up and down.
		* 
		*  @param[in] yDiff How many degrees to rotate.
		*/
		void RotateCameraUpDown(float yDiff);

		/**@brief Polls keyboard input and moves the camera.
		* 
		* Moves the camera foward/backward if w/s or up/down arrow was pressed.\n
		* Moves the camera left/right if a/d or left/right arrow was pressed.\n
		* Moves the camera up/down if space/crtl was pressed.\n
		* 
		* @param[in] dt The time between frames.
		*/
		void KeyboardInput(float dt);

		/**@brief Polls keyboard input and moves the camera.
		*
		* Moves the camera foward/backward if w/s was pressed.\n
		* Moves the camera left/right if a/d was pressed.\n
		* Moves the camera up/down if space/crtl was pressed.\n
		*
		* @param[in] dt The time between frames.
		*/
		void KeyboardInputWASD(float dt);

		/**@brief Polls keyboard input and moves the camera.
		*
		* Moves the camera foward/backward if up/down arrow was pressed.\n
		* Moves the camera left/right if left/right arrow was pressed.\n
		* Moves the camera up/down if space/crtl was pressed.\n
		*
		* @param[in] dt The time between frames.
		*/
		void KeyboardInputArrow(float dt);

		/**@brief Polls mouse input and rotates the camera.
		*/
		void MouseInput();

	private:
		//camera position in world coordinates
		vec3 mCameraPosition;

		//z-axis of the camera coordinate system
		vec3 mN;

		//y-axis of the camera coordinate system
		vec3 mV;

		//x-axis of the camera coordinate system
		vec3 mU;

		//stores the world to camera transform
		mat4 mViewMatrix;

		//frustrum properties
		float mNear;
		float mFar;
		float mVerticalFov;
		float mAspectRatio;
		mat4 mPerspectiveProjectionMatrix;

		mat4 mViewPerspectiveProjectionMatrix;

		float mCameraVelocity;
		float mAngularVelocity;

		vec2 mLastMousePosition;
	};
}