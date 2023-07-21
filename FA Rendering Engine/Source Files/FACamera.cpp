#include "FACamera.h"

namespace FACamera
{
	Camera::Camera() : mCameraVelocity{ 0.0f }, mAngularVelocity{ 0.0f }, mUpdateViewMatrix{ true }
	{}

	Camera::Camera(vec4 cameraPosition, vec4 x, vec4 y, vec4 z,
		float cameraVelocity, float rotateVelocity) :
		mCameraPosition{ cameraPosition }, mX{ x }, mY{ y }, mZ{ z },
		mCameraVelocity{ cameraVelocity }, mAngularVelocity{ rotateVelocity }, mUpdateViewMatrix{ true }
	{}

	void Camera::SetProperties(vec4 cameraPosition, vec4 x, vec4 y, vec4 z, float cameraVelocity, float angularVelocity)
	{
		mCameraPosition = cameraPosition;
		mX = x;
		mY = y;
		mZ = z;
		mCameraVelocity = cameraVelocity;
		mAngularVelocity = angularVelocity;

		mUpdateViewMatrix = true;
	}

	const vec4& Camera::GetCameraPosition() const
	{
		return mCameraPosition;
	}

	const vec4& Camera::GetX() const
	{
		return mX;
	}

	const vec4& Camera::GetY() const
	{
		return mY;
	}

	const vec4& Camera::GetZ() const
	{
		return mZ;
	}

	const mat4& Camera::GetViewMatrix() const
	{
		return mViewMatrix;
	}

	float Camera::GetCameraVelocity() const
	{
		return mCameraVelocity;
	}

	float Camera::GetAngularVelocity() const
	{
		return mAngularVelocity;
	}

	//UVN model:
	//N = target - cameraPosition
	//U = up x N
	//V = N x U
	void Camera::LookAt(vec4 cameraPosition, vec4 target, vec4 up)
	{
		mCameraPosition = cameraPosition;

		mZ = FAMath::Norm(target - cameraPosition);

		mX = vec4(FAMath::Norm(FAMath::CrossProduct(up, mZ)), 0.0f);

		mY = vec4(FAMath::CrossProduct(mZ, mX), 0.0f);

		mUpdateViewMatrix = true;
	}

	void Camera::SetCameraPosition(const vec4& position)
	{
		mCameraPosition = position;
		mUpdateViewMatrix = true;
	}

	void Camera::SetX(const vec4& x)
	{
		mX = x;
		mUpdateViewMatrix = true;
	}

	void Camera::SetY(const vec4& y)
	{
		mY = y;
		mUpdateViewMatrix = true;
	}

	void Camera::SetZ(const vec4& z)
	{
		mZ = z;
		mUpdateViewMatrix = true;
	}

	void Camera::SetCameraVelocity(float velocity)
	{
		mCameraVelocity = velocity;
	}

	void Camera::SetAngularVelocity(float velocity)
	{
		mAngularVelocity = velocity;
	}

	/* View transformation matrix
	*		ux							vx						nx				0
	*		uy							vy						ny				0
	*		uz							vz						nz				0
	* -cameraPosition dot u		-cameraPosition dot v	-cameraPosition dot n	1
	*/
	void Camera::UpdateViewMatrix()
	{
		if (mUpdateViewMatrix)
		{
			//Orthonormalize the camera space axes

			Orthonormalize(mX, mY, mZ);

			//update the camera's view matrix
			mViewMatrix.SetRow(0, vec4(mX.GetX(), mY.GetX(), mZ.GetX(), 0.0f));
			mViewMatrix.SetRow(0, vec4(mX.GetX(), mY.GetX(), mZ.GetX(), 0.0f));
			mViewMatrix.SetRow(1, vec4(mX.GetY(), mY.GetY(), mZ.GetY(), 0.0f));
			mViewMatrix.SetRow(2, vec4(mX.GetZ(), mY.GetZ(), mZ.GetZ(), 0.0f));
			mViewMatrix.SetRow(3, vec4(-FAMath::DotProduct(mCameraPosition, mX), -FAMath::DotProduct(mCameraPosition, mY),
				-FAMath::DotProduct(mCameraPosition, mZ), 1.0f));

			mUpdateViewMatrix = false;
		}
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's x-axis
	//move along the camera's x-axis
	void Camera::Left(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition -= distance * mX;

		mUpdateViewMatrix = true;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's x-axis
	//move along the camera's x-axis
	void Camera::Right(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition += distance * mX;

		mUpdateViewMatrix = true;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's z-axis
	//move along the camera's z-axis
	void Camera::Foward(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition += distance * mZ;

		mUpdateViewMatrix = true;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's z-axis
	//move along the camera's z-axis
	void Camera::Backward(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition -= distance * mZ;

		mUpdateViewMatrix = true;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's y-axis
	//move along the camera's y-axis
	void Camera::Up(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition += distance * mY;

		mUpdateViewMatrix = true;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's y-axis
	//move along the camera's y-axis
	void Camera::Down(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition -= distance * mY;

		mUpdateViewMatrix = true;
	}

	//To look left and right you rotate the camera around worlds up-axis. 
	//You rotate the basis vectors of the camera around the worlds up-axis. 
	//The x difference in mouse positions is how many degrees to rotate.
	void Camera::RotateCameraLeftRight(float xDiff)
	{
		FAMath::Quaternion rotateY(FAMath::RotationQuaternion(xDiff, vec3(0.0f, 1.0f, 0.0f)));
		mX = FAMath::Rotate(rotateY, mX);
		mY = FAMath::Rotate(rotateY, mY);
		mZ = FAMath::Rotate(rotateY, mZ);

		mUpdateViewMatrix = true;
	}

	//To look up or down you rotate the camera around its u vector. 
	//You do this by rotating the other two vectors around its u vector. 
	//The y difference in mouse positions is how many degrees to rotate
	void Camera::RotateCameraUpDown(float yDiff)
	{
		FAMath::Quaternion rotateX(FAMath::RotationQuaternion(yDiff, mX));
		mY = FAMath::Rotate(rotateX, mY);
		mZ = FAMath::Rotate(rotateX, mZ);

		mUpdateViewMatrix = true;
	}

	void Camera::KeyboardInput(float dt)
	{
		//check if w, a, s, d or up, left, right, down, spacebar or control was pressed

		if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000)
			Foward(dt);
		if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
			Left(dt);
		if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)
			Backward(dt);
		if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
			Right(dt);
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			Up(dt);
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			Down(dt);
	}

	void Camera::KeyboardInputWASD(float dt)
	{
		//check if w, a, s, d, spacebar or control was pressed

		if (GetAsyncKeyState('W') & 0x8000)
			Foward(dt);
		if (GetAsyncKeyState('A') & 0x8000)
			Left(dt);
		if (GetAsyncKeyState('S') & 0x8000)
			Backward(dt);
		if (GetAsyncKeyState('D') & 0x8000)
			Right(dt);
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			Up(dt);
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			Down(dt);
	}


	void Camera::KeyboardInputArrow(float dt)
	{
		//check if up, left, right, down, spacebar or control was pressed

		if (GetAsyncKeyState(VK_UP) & 0x8000)
			Foward(dt);
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			Left(dt);
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			Backward(dt);
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			Right(dt);
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			Up(dt);
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			Down(dt);
	}

	void Camera::MouseInput()
	{
		POINT currMousePos{};
		GetCursorPos(&currMousePos);

		FAMath::Vector2D currentMousePosition((float)currMousePos.x, (float)currMousePos.y);

		FAMath::Vector2D mousePositionDiff(currentMousePosition - mLastMousePosition);

		//if the mouse goes outside the window and comes back into the window, the camera won't be rotated.
		if (Length(mousePositionDiff) < 10.0f && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			RotateCameraLeftRight(mAngularVelocity * mousePositionDiff.GetX());
			RotateCameraUpDown(mAngularVelocity * mousePositionDiff.GetY());
		}

		mLastMousePosition = currentMousePosition;
	}
}
