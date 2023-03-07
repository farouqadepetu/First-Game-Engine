#include "FACamera.h"

namespace FACamera
{
	Camera::Camera() :
		m_cameraPosition{ vec3(0.0f, 0.0f, 0.0f) }, m_n{ vec3(0.0f, 0.0f, 1.0f) }, m_u{ 1.0f, 0.0f, 0.0f }, m_v{ 0.0f, 1.0f, 0.0f },
		m_near{ 1.0f }, m_far{ 100.f }, m_verticalFov{ 45.0f }, m_aspectRatio{ 1.0f }, m_cameraVelocity{ 10.0f }, m_rotateVelocity{ 0.5f }
	{}

	Camera::Camera(vec3 cameraPosition, vec3 x, vec3 y, vec3 z,
		float znear, float zfar, float aspectRatio, float vFov, float cameraVelocity, float rotateVelocity) :
		m_cameraPosition{ cameraPosition }, m_n{ z }, m_u{ x }, m_v{ y },
		m_near{ znear }, m_far{ zfar }, m_verticalFov{ vFov }, m_aspectRatio{ aspectRatio },
		m_cameraVelocity{ cameraVelocity }, m_rotateVelocity{ rotateVelocity }
	{}

	vec3& Camera::cameraPosition()
	{
		return m_cameraPosition;
	}

	const vec3& Camera::cameraPosition() const
	{
		return m_cameraPosition;
	}

	vec3 Camera::x() const
	{
		return m_u;
	}

	vec3 Camera::y() const
	{
		return m_v;
	}

	vec3 Camera::z() const
	{
		return m_n;
	}

	mat4 Camera::viewTransformationMatrix() const
	{
		return m_viewMatrix;
	}

	float& Camera::cameraVelocity()
	{
		return m_cameraVelocity;
	}

	const float& Camera::cameraVelocity() const
	{
		return m_cameraVelocity;
	}

	float& Camera::rotateVelocity()
	{
		return m_rotateVelocity;
	}

	const float& Camera::rotateVelocity() const
	{
		return m_rotateVelocity;
	}

	//UVN model:
	//N = target - cameraPosition
	//U = up x N
	//V = N x U
	void Camera::lookAt(vec3 cameraPosition, vec3 target, vec3 up)
	{
		m_n = FAMath::norm(target - cameraPosition);

		m_u = FAMath::norm(FAMath::crossProduct(up, m_n));

		m_v = FAMath::crossProduct(m_n, m_u);
	}

	float& Camera::znear()
	{
		return m_near;
	}

	const float& Camera::znear() const
	{
		return m_near;
	}

	float& Camera::zfar()
	{
		return m_far;
	}

	const float& Camera::zfar() const
	{
		return m_far;
	}

	float& Camera::vFov()
	{
		return m_verticalFov;
	}

	const float& Camera::vFov() const
	{
		return m_verticalFov;
	}

	float& Camera::aspect()
	{
		return m_aspectRatio;
	}

	const float& Camera::aspect() const
	{
		return m_aspectRatio;
	}

	mat4 Camera::perspectiveProjectionTransformationMatrix()
	{
		return m_perspectiveProjectionMatrix;
	}

	mat4 Camera::viewPerspectiveProjectionTransformationMatrix()
	{
		return m_viewPerspectiveProjectionTransformationMatrix;
	}

	/* View transformation matrix
	*		ux							vx						nx				0
	*		uy							vy						ny				0
	*		uz							vz						nz				0
	* -cameraPosition dot u		-cameraPosition dot v	-cameraPosition dot n	1
	*/
	void Camera::updateViewTransformationMatrix()
	{
		//Orthonormalize the camera space axes
		
		//z-axis
		m_n = FAMath::norm(m_n);
		
		//y-axis of the camera is n x u
		m_v = FAMath::norm(FAMath::crossProduct(m_n, m_u));

		//x-axis of the camera is v x n
		m_u = FAMath::norm(FAMath::crossProduct(m_v, m_n));

		//update the camera's view matrix
		m_viewMatrix.setRow(0, vec4(m_u.x(), m_v.x(), m_n.x(), 0.0f));
		m_viewMatrix.setRow(1, vec4(m_u.y(), m_v.y(), m_n.y(), 0.0f));
		m_viewMatrix.setRow(2, vec4(m_u.z(), m_v.z(), m_n.z(), 0.0f));
		m_viewMatrix.setRow(3, vec4(-FAMath::dotProduct(m_cameraPosition, m_u), -FAMath::dotProduct(m_cameraPosition, m_v),
			-FAMath::dotProduct(m_cameraPosition, m_n), 1.0f));

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
	void Camera::updatePerspectiveProjectionTransformationMatrix()
	{
		double angle{ m_verticalFov / 2.0 };
		angle = angle * PI / 180.0;

		double t{ tan(angle) };

		double fMinusN = m_far - m_near;

		m_perspectiveProjectionMatrix.setRow(0, vec4(1.0 / (m_aspectRatio * t), 0.0f, 0.0f, 0.0f));
		m_perspectiveProjectionMatrix.setRow(1, vec4(0.0f, 1.0 / t, 0.0f, 0.0f));
		m_perspectiveProjectionMatrix.setRow(2, vec4(0.0f, 0.0f, m_far / fMinusN, 1.0f));
		m_perspectiveProjectionMatrix.setRow(3, vec4(0.0f, 0.0f, -(m_near * m_far) / fMinusN, 0.0f));
	}

	void Camera::updateViewPerspectiveProjectionTransformationMatrix()
	{
		m_viewPerspectiveProjectionTransformationMatrix = m_viewMatrix * m_perspectiveProjectionMatrix;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's x-axis
	//move along the camera's x-axis
	void Camera::left(float dt)
	{
		float distance{ m_cameraVelocity * dt };
		m_cameraPosition -= distance * m_u;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's x-axis
	//move along the camera's x-axis
	void Camera::right(float dt)
	{
		float distance{ m_cameraVelocity * dt };
		m_cameraPosition += distance * m_u;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's z-axis
	//move along the camera's z-axis
	void Camera::foward(float dt)
	{
		float distance{ m_cameraVelocity * dt };
		m_cameraPosition += distance * m_n;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's z-axis
	//move along the camera's z-axis
	void Camera::backward(float dt)
	{
		float distance{ m_cameraVelocity * dt };
		m_cameraPosition -= distance * m_n;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position += distance * camera's y-axis
	//move along the camera's y-axis
	void Camera::up(float dt)
	{
		float distance{ m_cameraVelocity * dt };
		m_cameraPosition += distance * m_v;
	}

	//velocity = distance/time
	//distance = velocity * time.
	//camera position -= distance * camera's y-axis
	//move along the camera's y-axis
	void Camera::down(float dt)
	{
		float distance{ m_cameraVelocity * dt };
		m_cameraPosition -= distance * m_v;
	}


	//To look left and right you rotate the camera around worlds up-axis. 
	//You rotate the basis vectors of the camera around the worlds up-axis. 
	//The x difference in mouse positions is how many degrees to rotate.
	void Camera::rotateCameraLeftRight(float xDiff)
	{
		//make rotation matrix
		mat4 rotateY = FAMath::quaternionRotationMatrixRow(FAMath::rotationQuaternion(xDiff, vec3(0.0f, 1.0f, 0.0f)));

		vec4 cameraX(m_u);
		vec4 cameraY(m_v);
		vec4 cameraZ(m_n);

		//rotate the x, y and z axis around the world's y-axis
		m_u = vec3(cameraX * rotateY);
		m_v = vec3(cameraY * rotateY);
		m_n = vec3(cameraZ * rotateY);
	}

	//To look up or down you rotate the camera around its u vector. 
	//You do this by rotating the other two vectors around its u vector. 
	//The y difference in mouse positions is how many degrees to rotate
	void Camera::rotateCameraUpDown(float yDiff)
	{
		//make rotation matrix
		mat4 rotateY = FAMath::quaternionRotationMatrixRow(FAMath::rotationQuaternion(yDiff, m_u));

		vec4 cameraY(m_v);
		vec4 cameraZ(m_n);

		//rotate the camera's y and z axis around the cameras x-axis
		m_v = vec3(cameraY * rotateY);
		m_n = vec3(cameraZ * rotateY);
	}

	void Camera::keyboardInput(float dt)
	{
		//check if w, a, s, d or up, left, right, down, spacebar or control was pressed

		if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000)
			foward(dt);
		if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
			left(dt);
		if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)
			backward(dt);
		if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
			right(dt);
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			up(dt);
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			down(dt);
	}


	void Camera::mouseInput(FAMath::Vector2D currentMousePosition)
	{
		FAMath::Vector2D mousePositionDiff(currentMousePosition - lastMousePosition);

		//if the mouse goes outside the window and comes back into the window, the camera won't be rotated.
		if (length(mousePositionDiff) < 10.0f)
		{
			rotateCameraLeftRight(m_rotateVelocity * mousePositionDiff.x());
			rotateCameraUpDown(m_rotateVelocity * mousePositionDiff.y());
		}

		lastMousePosition = currentMousePosition;
	}
}
