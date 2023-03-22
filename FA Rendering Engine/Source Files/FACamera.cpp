#include "FACamera.h"

namespace FACamera
{
	Camera::Camera(vec3 cameraPosition, vec3 x, vec3 y, vec3 z,
		float znear, float zfar, float aspectRatio, float vFov, float cameraVelocity, float rotateVelocity) :
		mCameraPosition{ cameraPosition }, mN{ z }, mU{ x }, mV{ y },
		mNear{ znear }, mFar{ zfar }, mVerticalFov{ vFov }, mAspectRatio{ aspectRatio },
		mCameraVelocity{ cameraVelocity }, mAngularVelocity{ rotateVelocity }
	{}

	const vec3& Camera::GetCameraPosition() const
	{
		return mCameraPosition;
	}

	const vec3& Camera::GetX() const
	{
		return mU;
	}

	const vec3& Camera::GetY() const
	{
		return mV;
	}

	const vec3& Camera::GetZ() const
	{
		return mN;
	}

	const mat4& Camera::GetViewTransformationMatrix() const
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
	void Camera::LookAt(vec3 cameraPosition, vec3 target, vec3 up)
	{
		mN = FAMath::Norm(target - cameraPosition);

		mU = FAMath::Norm(FAMath::CrossProduct(up, mN));

		mV = FAMath::CrossProduct(mN, mU);
	}

	float Camera::GetZNear() const 
	{
		return mNear;
	}

	float Camera::GetZFar() const
	{
		return mFar;
	}

	float Camera::GetVerticalFov() const
	{
		return mVerticalFov;
	}

	float Camera::GetAspectRatio() const
	{
		return mAspectRatio;
	}

	void Camera::SetCameraPosition(const vec3& position)
	{
		mCameraPosition = position;
	}

	void Camera::SetX(const vec3& x)
	{
		mU = x;
	}

	void Camera::SetY(const vec3& y)
	{
		mV = y;
	}

	void Camera::SetZ(const vec3& z)
	{
		mN = z;
	}

	void Camera::SetCameraVelocity(float velocity)
	{
		mCameraVelocity = velocity;
	}

	void Camera::SetAngularVelocity(float velocity)
	{
		mAngularVelocity = velocity;
	}

	void Camera::SetZNear(float znear)
	{
		mNear = znear;
	}

	void Camera::SetZFar(float zfar)
	{
		mFar = zfar;
	}

	void Camera::SetVerticalFov(float fov)
	{
		mVerticalFov = fov;
	}

	void Camera::SetAspectRatio(float ar)
	{
		mAspectRatio = ar;
	}

	const mat4& Camera::GetPerspectiveProjectionMatrix() const
	{
		return mPerspectiveProjectionMatrix;
	}

	const mat4& Camera::GetViewPerspectiveProjectionMatrix() const
	{
		return mViewPerspectiveProjectionMatrix;
	}

	/* View transformation matrix
	*		ux							vx						nx				0
	*		uy							vy						ny				0
	*		uz							vz						nz				0
	* -cameraPosition dot u		-cameraPosition dot v	-cameraPosition dot n	1
	*/
	void Camera::UpdateViewMatrix()
	{
		//Orthonormalize the camera space axes

		Orthonormalize(mU, mV, mN);

		//update the camera's view matrix
		mViewMatrix.SetRow(0, vec4(mU.GetX(), mV.GetX(), mN.GetX(), 0.0f));
		mViewMatrix.SetRow(1, vec4(mU.GetY(), mV.GetY(), mN.GetY(), 0.0f));
		mViewMatrix.SetRow(2, vec4(mU.GetZ(), mV.GetZ(), mN.GetZ(), 0.0f));
		mViewMatrix.SetRow(3, vec4(-FAMath::DotProduct(mCameraPosition, mU), -FAMath::DotProduct(mCameraPosition, mV),
			-FAMath::DotProduct(mCameraPosition, mN), 1.0f));

	}

	/* Perspective Projection Transformation Matrix
	*  1 / rtan(vFov / 2)				0					0			0
	*		0					1 / tan(vFov / 2)			0			0
	*		0							0				f / f - n		1
	*		0							0				-nf/f - n		0
	* r = aspect ratio
	* f = zfar
	* n = znear
	* vFov = vertical field of view
	*/
	void Camera::UpdatePerspectiveProjectionMatrix()
	{
		double angle{ mVerticalFov / 2.0 };
		angle = angle * PI / 180.0;

		double t{ tan(angle) };

		double fMinusN = mFar - mNear;

		mPerspectiveProjectionMatrix.SetRow(0, vec4(1.0 / (mAspectRatio * t), 0.0f, 0.0f, 0.0f));
		mPerspectiveProjectionMatrix.SetRow(1, vec4(0.0f, 1.0 / t, 0.0f, 0.0f));
		mPerspectiveProjectionMatrix.SetRow(2, vec4(0.0f, 0.0f, mFar / fMinusN, 1.0f));
		mPerspectiveProjectionMatrix.SetRow(3, vec4(0.0f, 0.0f, -(mNear * mFar) / fMinusN, 0.0f));
	}

	void Camera::UpdateViewPerspectiveProjectionMatrix()
	{
		mViewPerspectiveProjectionMatrix = mViewMatrix * mPerspectiveProjectionMatrix;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's x-axis
	//move along the camera's x-axis
	void Camera::Left(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition -= distance * mU;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's x-axis
	//move along the camera's x-axis
	void Camera::Right(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition += distance * mU;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's z-axis
	//move along the camera's z-axis
	void Camera::Foward(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition += distance * mN;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's z-axis
	//move along the camera's z-axis
	void Camera::Backward(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition -= distance * mN;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's y-axis
	//move along the camera's y-axis
	void Camera::Up(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition += distance * mV;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's y-axis
	//move along the camera's y-axis
	void Camera::Down(float dt)
	{
		float distance{ mCameraVelocity * dt };
		mCameraPosition -= distance * mV;
	}


	//To look left and right you rotate the camera around worlds up-axis. 
	//You rotate the basis vectors of the camera around the worlds up-axis. 
	//The x difference in mouse positions is how many degrees to rotate.
	void Camera::RotateCameraLeftRight(float xDiff)
	{
		//make rotation matrix
		mat4 rotateY = FAMath::QuaternionToRotationMatrixRow(FAMath::RotationQuaternion(xDiff, vec3(0.0f, 1.0f, 0.0f)));

		vec4 cameraX(mU.GetX(), mU.GetY(), mU.GetZ(), 0.0f);
		vec4 cameraY(mV.GetX(), mV.GetY(), mV.GetZ(), 0.0f);
		vec4 cameraZ(mN.GetX(), mN.GetY(), mN.GetZ(), 0.0f);

		vec4 resX(cameraX * rotateY);
		vec4 resY(cameraY * rotateY);
		vec4 resZ(cameraZ * rotateY);

		//rotate the x, y and z axis around the world's y-axis
		mU = vec3(resX.GetX(), resX.GetY(), resX.GetZ());
		mV = vec3(resY.GetX(), resY.GetY(), resY.GetZ());
		mN = vec3(resZ.GetX(), resZ.GetY(), resZ.GetZ());
	}

	//To look up or down you rotate the camera around its u vector. 
	//You do this by rotating the other two vectors around its u vector. 
	//The y difference in mouse positions is how many degrees to rotate
	void Camera::RotateCameraUpDown(float yDiff)
	{
		//make rotation matrix
		mat4 rotateY = FAMath::QuaternionToRotationMatrixRow(FAMath::RotationQuaternion(yDiff, mU));

		vec4 cameraY(mV.GetX(), mV.GetY(), mV.GetZ(), 0.0f);
		vec4 cameraZ(mN.GetX(), mN.GetY(), mN.GetZ(), 0.0f);

		vec4 resY(cameraY * rotateY);
		vec4 resZ(cameraZ * rotateY);

		//rotate the camera's y and z axis around the cameras x-axis
		mV = vec3(resY.GetX(), resY.GetY(), resY.GetZ());
		mN = vec3(resZ.GetX(), resZ.GetY(), resZ.GetZ());
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


	void Camera::MouseInput()
	{
		POINT currMousePos{};
		GetCursorPos(&currMousePos);

		FAMath::Vector2D currentMousePosition(currMousePos.x, currMousePos.y);

		FAMath::Vector2D mousePositionDiff(currentMousePosition - mLastMousePosition);

		//if the mouse goes outside the window and comes back into the window, the camera won't be rotated.
		if (Length(mousePositionDiff) < 5.0f && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			RotateCameraLeftRight(mAngularVelocity * mousePositionDiff.GetX());
			RotateCameraUpDown(mAngularVelocity * mousePositionDiff.GetY());
		}

		mLastMousePosition = currentMousePosition;
	}
}
