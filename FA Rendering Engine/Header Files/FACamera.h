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

		/**@brief Default Constructor.
		*
		*	Creates a new camera.\n
		*	The origin of the camera space is (0.0f, 0.0f, 0.0f).\n
		*	The x, y and z axes of the camera space is the same as the x, y and z axes as world space.\n
		*	Sets the frustrum properties for perspective projection \n
		*	to the values:\n
		*	znear = 1.0f\n
		*	zfar = 1000.0f\n
		*	aspect ratio = 1.0f\n
		*	fov = 45 degrees\n
		*	The constant velocity of the camera when moved is 10.0f.
		*	The rotate velocity is 0.5f.
		*/
		Camera();

		/**@brief Overloaded Constructor.
		*
		*	Creates a new camera.\n
		*	Sets the origin of the camera space to the given cameraPosition.\n
		*	Sets the axis of the camera space to the given x, y and z vectors.\n
		*	The origin and basis vectors of the camera space should be relative to world space.\n
		*	Sets the frustum properties for perspective projection to the given znear, zar, aspectRatio and fov values.\n
		*	vFov should be in degrees.\n
		*	The constant velocity of the camera when moved is set to the given cameraVelocity;
		*	The rotate velocity of the camera is set the to specified rotateVelocity.
		*/
		Camera(vec3 cameraPosition, vec3 x, vec3 y, vec3 z,
			float znear, float zfar, float aspectRatio, float vFov, float cameraVelocity, float rotateVelocity);

		/**@brief Returns a reference to the position of the camera in world coordinates.
		*/
		vec3& cameraPosition();

		/**@brief Returns a constant reference to the position of the camera in world coordinates.
		*/
		const vec3& cameraPosition() const;

		/**@brief Returns the x-axis of the camera.
		*/
		vec3 x() const;
		
		/**@brief Returns the y-axis of the camera.
		*/
		vec3 y() const;

		/**@brief Returns the z-axis of the camera.
		*/
		vec3 z() const;

		/**@brief Returns the view transformation matrix of this camera.
		*/
		mat4 viewTransformationMatrix() const;

		/**@brief Returns a reference to the camera's velocity.
		*/
		float& cameraVelocity();

		/**@brief Returns a constant reference to the camera's velocity.
		*/
		const float& cameraVelocity() const;

		/**@brief Returns a reference to the camera's rotate velocity.
		*/
		float& rotateVelocity();

		/**@brief Returns a constant reference to the camera's rotate velocity.
		*/
		const float& rotateVelocity() const;
		
		/**@brief Defines the camera space using UVN.
		*/
		void lookAt(vec3 cameraPosition, vec3 target, vec3 up);

		/**@brief Returns a reference to the near value of the frustrum.
		*/
		float& znear();

		/**@brief Returns a constant reference to the near value of the frustrum.
		*/
		const float& znear() const;

		/**@brief Returns a reference to the far value of the frustrum.
		*/
		float& zfar();

		/**@brief Returns a constant reference to the far value of the frustrum.
		*/
		const float& zfar() const;

		/**@brief Returns a reference to the vertical field of view of the frustrum in degrees.
		*/
		float& vFov();

		/**@brief Returns a constant reference to the vertical field of view of the frustrum in degrees.
		*/
		const float& vFov() const;

		/**@brief Returns a reference to the aspect ratio of the frustrum.
		*/
		float& aspect();

		/**@brief Returns a constant reference to the aspect ratio of the frustrum.
		*/
		const float& aspect() const;

		/**@brief Returns the perspective projection transformation matrix of this camera.
		*/
		mat4 perspectiveProjectionMatrix();

		/**@brief Returns the view perspective projection transformation matrix of this camera.
		*/
		mat4 viewPerspectiveProjectionMatrix();

		/**@brief After modifying the camera position and/or orientation, call this to rebuild the view transformation matrix.
		*/
		void updateViewMatrix();

		/**@brief After modifying any of the frustrum properties, call this to rebuild the perspective projection transformation matrix.
		*/
		void updatePerspectiveProjectionMatrix();

		/**@brief After modifying view and/or perspective projection transformation matrix, 
		*	call this to rebuild the view perspective projection transformation matrix.
		*/
		void updateViewPerspectiveProjectionMatrix();

		/**@brief Moves the camera left along the camera's x-axis
		*/
		void left(float dt);

		/**@brief Moves the camera right along the camera's x-axis
		*/
		void right(float dt);

		/**@brief Moves the camera foward along the camera's z-axis
		*/
		void foward(float dt);

		/**@brief Moves the camera backward along the camera's z-axis
		*/
		void backward(float dt);

		/**@brief Moves the camera up along the camera's y-axis
		*/
		void up(float dt);

		/**@brief Moves the camera down along the camera's y-axis
		*/
		void down(float dt);

		/**@brief Rotates the camera to look left and right.
		*/
		void rotateCameraLeftRight(float xDiff);

		/**@brief Rotates the camera to look up and down.
		*/
		void rotateCameraUpDown(float yDiff);

		/**@brief Polls keyboard input and moves the camera.
		* Moves the camera foward/backward if w/s or up/down arrow was pressed.
		* Moves the camera left/right if a/d or left/right arrow was pressed.
		* Moves the camera up/down if space/crtl was pressed.
		*/
		void keyboardInput(float dt);

		/**@brief Rotates camera on mouse movement.
		*/
		void mouseInput();

	private:
		//camera position in world coordinates
		vec3 m_cameraPosition;

		//z-axis of the camera coordinate system
		vec3 m_n;

		//y-axis of the camera coordinate system
		vec3 m_v;

		//x-axis of the camera coordinate system
		vec3 m_u;

		//stores the world to camera transform
		mat4 m_viewMatrix;

		//frustrum properties
		float m_near;
		float m_far;
		float m_verticalFov;
		float m_aspectRatio;
		mat4 m_perspectiveProjectionMatrix;

		mat4 m_viewPerspectiveProjectionMatrix;

		float m_cameraVelocity;
		float m_rotateVelocity;

		vec2 lastMousePosition;
	};
}