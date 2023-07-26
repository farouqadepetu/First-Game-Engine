#pragma once


#include "MathEngine.h"
#include <Windows.h>


namespace RenderingEngine
{
	/** @struct Camera ""
	*	@brief Simple first person style camera class that lets the viewer explore the 3D scene.\n
	*   It keeps track of the camera coordinate system relative to the world space so that the view matrix can be constructed.\n
	*/
	struct Camera
	{
		//camera position in world coordinates
		vec3 position;

		//x-axis of the camera coordinate system
		vec3 x;

		//y-axis of the camera coordinate system
		vec3 y;

		//z-axis of the camera coordinate system
		vec3 z;

		//stores the world to camera transform
		mat4 viewMatrix;

		//the velocities of the camera.
		float linearSpeed = 0.0f;
		float angularSpeed = 0.0f;
	};

	/**@brief Sets the properties of the camera.
	*
	* @param[in] camera The camera object.
	* @param[in] position The position of the camera.
	* @param[in] x The x axis of the local coordinate system of the camera.
	* @param[in] y The y axis of the local coordinate system of the camera.
	* @param[in] z The z axis of the local coordinate system of the camera.
	* @param[in] linearVelocity The translational velocity of the camera.
	* @param[in] angularVelocity The angular velocity of the camera.
	*/
	void SetProperties(Camera& camera, vec3 position, vec3 x, vec3 y, vec3 z, float linearSpeed, float angularSpeed);

	/**@brief Defines the camera space using UVN.
	*
	* @param[in] camera The camera object.
	* @param[in] position The position of the camera.
	* @param[in] target The point the camera is looking at.
	* @param[in] up The up direction of the world.
	*/
	void LookAt(Camera& camera, vec3 position, vec3 target, vec3 up);

	/**@brief After modifying the camera position and/or orientation, call this to rebuild the view transformation matrix.
	*/
	void UpdateViewMatrix(Camera& camera);

	/**@brief Moves the camera left along the camera's x-axis.
	*/
	void Left(Camera& camera, float dt);

	/**@brief Moves the camera right along the camera's x-axis.
	*
	*  @param[in] camera The camera object.
	*  @param[in] dt The time between frames.
	*/
	void Right(Camera& camera, float dt);

	/**@brief Moves the camera foward along the camera's z-axis.
	* 
	* @param[in] camera The camera object.
	* @param[in] dt The time between frames.
	*/
	void Foward(Camera& camera, float dt);

	/**@brief Moves the camera backward along the camera's z-axis.
	*
	* @param[in] camera The camera object.
	* @param[in] dt The time between frames.
	*/
	void Backward(Camera& camera, float dt);

	/**@brief Moves the camera up along the camera's y-axis.
	*
	* @param[in] camera The camera object.
	* @param[in] dt The time between frames.
	*/
	void Up(Camera& camera, float dt);

	/**@brief Moves the camera down along the camera's y-axis.
	*
	* @param[in] camera The camera object.
	* @param[in] dt The time between frames.
	*/
	void Down(Camera& camera, float dt);

	/**@brief Rotates the camera to look left and right.
	*
	* @param[in] camera The camera object.
	* @param[in] xDiff How many degrees to rotate.
	*/
	void RotateCameraLeftRight(Camera& camera, float xDiff);

	/**@brief Rotates the camera to look up and down.
	*
	*  @param[in] camera The camera object.
	*  @param[in] yDiff How many degrees to rotate.
	*/
	void RotateCameraUpDown(Camera& camera, float yDiff);
}