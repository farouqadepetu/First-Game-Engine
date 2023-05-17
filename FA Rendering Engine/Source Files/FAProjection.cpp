#include "FAProjection.h"

namespace FAProjection
{
	//-------------------------------------------------------------------------------------------------------------------------------------
	//IPROJECTION MEMBER FUNCTION DEFINITIONS
	IProjection::IProjection() : mUpdateProjectionMatrix{ true }
	{}

	const FAMath::Matrix4x4& IProjection::GetProjectionMatrix() const
	{
		return mProjectionMatrix;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------------------------------------------
	//PERSPECTIVEPROJECTION MEMBER FUNCTION DEFINITIONS
	PerspectiveProjection::PerspectiveProjection() : IProjection(),
		mNear{ 0.0f }, mFar{ 0.0f }, mVerticalFov{ 0.0f }, mAspectRatio{ 0.0f }
	{}

	PerspectiveProjection::PerspectiveProjection(float znear, float zfar, float vFov, float aspectRatio) : IProjection(),
		mNear{ znear }, mFar{ zfar }, mVerticalFov{ vFov }, mAspectRatio{ aspectRatio }
	{}

	void PerspectiveProjection::SetProperties(float znear, float zfar, float vFov, float aspectRatio)
	{
		mNear = znear;
		mFar = zfar;
		mVerticalFov = vFov;
		mAspectRatio = aspectRatio;
		mUpdateProjectionMatrix = true;
	}

	float PerspectiveProjection::GetNear() const
	{
		return mNear;
	}

	float PerspectiveProjection::GetFar() const
	{
		return mFar;
	}

	float PerspectiveProjection::GetVerticalFov() const
	{
		return mVerticalFov;
	}

	float PerspectiveProjection::GetAspectRatio() const
	{
		return mAspectRatio;
	}

	void PerspectiveProjection::SetNear(float znear)
	{
		mNear = znear;
		mUpdateProjectionMatrix = true;
	}

	void PerspectiveProjection::SetFar(float zfar)
	{
		mFar = zfar;
		mUpdateProjectionMatrix = true;
	}

	void PerspectiveProjection::SetVerticalFov(float fov)
	{
		mVerticalFov = fov;
		mUpdateProjectionMatrix = true;
	}

	void PerspectiveProjection::SetAspectRatio(float ar)
	{
		mAspectRatio = ar;
		mUpdateProjectionMatrix = true;
	}

	void PerspectiveProjection::UpdateProjectionMatrix()
	{
		if (mUpdateProjectionMatrix)
		{
			float angle{ mVerticalFov / 2.0f };
			angle = angle * PI / 180.0f;

			float t{ tan(angle) };

			float fMinusN = mFar - mNear;

			mProjectionMatrix.SetRow(0, FAMath::Vector4D(1.0f / (mAspectRatio * t), 0.0f, 0.0f, 0.0f));
			mProjectionMatrix.SetRow(1, FAMath::Vector4D(0.0f, 1.0f / t, 0.0f, 0.0f));
			mProjectionMatrix.SetRow(2, FAMath::Vector4D(0.0f, 0.0f, mFar / fMinusN, 1.0f));
			mProjectionMatrix.SetRow(3, FAMath::Vector4D(0.0f, 0.0f, -(mNear * mFar) / fMinusN, 0.0f));

			mUpdateProjectionMatrix = false;
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------------
}