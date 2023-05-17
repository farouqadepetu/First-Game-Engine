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
	*/
	class Camera
	{
	public:

		/**@brief Creates a new camera. Initializes all properties to 0.
		*/
		Camera();

		/**@brief Creates a new camera.
		*
		* @param[in] camerPosition The position of the camera.
		* @param[in] x The x axis of the local coordinate system of the camera.
		* @param[in] y The y axis of the local coordinate system of the camera.
		* @param[in] z The z axis of the local coordinate system of the camera.
		* @param[in] cameraVelocity The translational velocity of the camera.
		* @param[in] angularVelocity The angular velocity of the camera.
		*/
		Camera(vec4 cameraPosition, vec4 x, vec4 y, vec4 z, float cameraVelocity, float angularVelocity);

		/**@brief Sets the properties of the camera.
		* 
		* @param[in] camerPosition The position of the camera.
		* @param[in] x The x axis of the local coordinate system of the camera.
		* @param[in] y The y axis of the local coordinate system of the camera.
		* @param[in] z The z axis of the local coordinate system of the camera.
		* @param[in] cameraVelocity The translational velocity of the camera.
		* @param[in] angularVelocity The angular velocity of the camera.
		*/
		void SetProperties(vec4 cameraPosition, vec4 x, vec4 y, vec4 z, float cameraVelocity, float angularVelocity);

		/**@brief Returns a constant reference to the position of the camera in world coordinates.
		*/
		const vec4& GetCameraPosition() const;

		/**@brief Returns a constant reference to the x-axis of the camera.
		*/
		const vec4& GetX() const;
		
		/**@brief Returnsa constant reference to the y-axis of the camera.
		*/
		const vec4& GetY() const;

		/**@brief Returns a constant reference to the z-axis of the camera.
		*/
		const vec4& GetZ() const;

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
		void LookAt(vec4 cameraPosition, vec4 target, vec4 up);

		/**@brief Sets the camera's position to the specified position.
		*/
		void SetCameraPosition(const vec4& position);

		/**@brief Sets the camera's x-axis to the specified vector \a x.
		*/
		void SetX(const vec4& x);

		/**@brief Sets the camera's y-axis to the specified vector \a y.
		*/
		void SetY(const vec4& y);

		/**@brief Sets the camera's z-axis to the specified vector \a z.
		*/
		void SetZ(const vec4& z);

		/**@brief Sets the camera's velocity to the specified \a velocity.
		*/
		void SetCameraVelocity(float velocity);

		/**@brief Sets the camera's angular velocity to the specified \a angular \a velocity.
		*/
		void SetAngularVelocity(float velcoity);

		/**@brief After modifying the camera position and/or orientation, call this to rebuild the view transformation matrix.
		*/
		void UpdateViewMatrix();

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
		vec4 mCameraPosition;

		//x-axis of the camera coordinate system
		vec4 mX;

		//y-axis of the camera coordinate system
		vec4 mY;

		//z-axis of the camera coordinate system
		vec4 mZ;

		//stores the world to camera transform
		mat4 mViewMatrix;

		float mCameraVelocity;
		float mAngularVelocity;

		vec2 mLastMousePosition;

		bool mUpdateViewMatrix;
	};
}