#include "Camera.h"

namespace RenderingEngine
{
	void SetProperties(Camera& camera, vec3 position, vec3 x, vec3 y, vec3 z, float linearSpeed, float angularSpeed)
	{
		camera.position = position;
		camera.x = MathEngine::Normalize(x);
		camera.y = MathEngine::Normalize(y);
		camera.z = MathEngine::Normalize(z);
		camera.linearSpeed = linearSpeed;
		camera.angularSpeed = angularSpeed;
	}

	void LookAt(Camera& camera, vec3 position, vec3 target, vec3 up)
	{
		camera.position = position;

		camera.z = MathEngine::Normalize(target - position);

		camera.x = MathEngine::Normalize(MathEngine::CrossProduct(up, camera.z));

		camera.y = MathEngine::CrossProduct(camera.z, camera.x);
	}

	void UpdateViewMatrix(Camera& camera)
	{
		//Orthonormalize the camera space axes

		MathEngine::Orthonormalize(camera.x, camera.y, camera.z);

		//update the camera's view matrix
		camera.viewMatrix.SetRow(0, vec4{ camera.x.x, camera.y.x, camera.z.x, 0.0f });
		camera.viewMatrix.SetRow(1, vec4{ camera.x.y, camera.y.y, camera.z.y, 0.0f });
		camera.viewMatrix.SetRow(2, vec4{ camera.x.z, camera.y.z, camera.z.z, 0.0f });
		camera.viewMatrix.SetRow(3, vec4{ -MathEngine::DotProduct(camera.position, camera.x), -MathEngine::DotProduct(camera.position, camera.y),
			-MathEngine::DotProduct(camera.position, camera.z), 1.0f });
	}

	//compute the linear velocity vector using the linear speed and the x direction.
	//move along the camera's negative x-axis
	void Left(Camera& camera, float dt)
	{
		vec3 linearVelocity = camera.linearSpeed * camera.x;
		camera.position -= linearVelocity * dt;
	}

	//compute the linear velocity vector using the linear speed and the x direction.
	//move along the camera's positive x-axis
	void Right(Camera& camera, float dt)
	{
		vec3 linearVelocity = camera.linearSpeed * camera.x;
		camera.position += linearVelocity * dt;

	}

	//compute the linear velocity vector using the linear speed and the z direction.
	//move along the camera's positive z-axis
	void Foward(Camera& camera, float dt)
	{
		vec3 linearVelocity = camera.linearSpeed * camera.z;
		camera.position += linearVelocity * dt;
	}

	//compute the linear velocity vector using the linear speed and the z direction.
	//move along the camera's negative z-axis
	void Backward(Camera& camera, float dt)
	{
		vec3 linearVelocity = camera.linearSpeed * camera.z;
		camera.position -= linearVelocity * dt;
	}

	//compute the linear velocity vector using the linear speed and the y direction.
	//move along the camera's positive y-axis
	void Up(Camera& camera, float dt)
	{
		vec3 linearVelocity = camera.linearSpeed * camera.y;
		camera.position += linearVelocity * dt;
	}

	//compute the linear velocity vector using the linear speed and the y direction.
	//move along the camera's positive y-axis
	void Down(Camera& camera, float dt)
	{
		vec3 linearVelocity = camera.linearSpeed * camera.y;
		camera.position -= linearVelocity * dt;
	}

	//To look left and right you rotate the camera around worlds up-axis. 
	//You rotate the basis vectors of the camera around the worlds up-axis. 
	//The x difference in mouse positions is how many degrees to rotate.
	void RotateCameraLeftRight(Camera& camera, float xDiff)
	{
		MathEngine::Quaternion rotateY(MathEngine::RotationQuaternion(xDiff, vec3{ 0.0f, 1.0f, 0.0f }));

		camera.x = MathEngine::Rotate(rotateY, camera.x);
		camera.y = MathEngine::Rotate(rotateY, camera.y);
		camera.z = MathEngine::Rotate(rotateY, camera.z);
	}

	//To look up or down you rotate the camera around its x-axis.
	//You do this by rotating the cameras y and z axes around the cameras x-axis. 
	//The y difference in mouse positions is how many degrees to rotate
	void RotateCameraUpDown(Camera& camera, float yDiff)
	{
		MathEngine::Quaternion rotateX(MathEngine::RotationQuaternion(yDiff, camera.x));

		camera.y = MathEngine::Rotate(rotateX, camera.y);
		camera.z = MathEngine::Rotate(rotateX, camera.z);
	}
}