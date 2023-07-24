#include "Model.h"

namespace MVC
{
	Model::Model() : mSimulationTime{ 0.1f }, mAccumulator{ 0.0f }, mAlpha{ 0.0f }
	{
		for (unsigned int i = 0; i < 5; ++i)
		{
			mShapes.push_back(nullptr);
			mRigidBodies.push_back(nullptr);
		}

		mShapes.at(RIGID_BOX) = &mInterpolatedBox.GetShape();
		mRigidBodies.at(RIGID_BOX) = &mCurrentBox.GetRigidBody();
		CreateBox();

		mShapes.at(RIGID_CONE) = &mInterpolatedCone.GetShape();
		mRigidBodies.at(RIGID_CONE) = &mCurrentCone.GetRigidBody();
		CreateCone();

		mShapes.at(RIGID_CYLINDER) = &mInterpolatedCylinder.GetShape();
		mRigidBodies.at(RIGID_CYLINDER) = &mCurrentCylinder.GetRigidBody();
		CreateCylinder();

		mShapes.at(RIGID_SPHERE) = &mInterpolatedSphere.GetShape();
		mRigidBodies.at(RIGID_SPHERE) = &mCurrentSphere.GetRigidBody();
		CreateSphere();

		mShapes.at(RIGID_PYRAMID) = &mInterpolatedPyramid.GetShape();
		mRigidBodies.at(RIGID_PYRAMID) = &mCurrentPyramid.GetRigidBody();
		CreatePyramid();
	}

	void Model::CreateBox()
	{
		std::vector<FAShapes::Vertex> vertices;
		std::vector<FAShapes::Triangle> triangles;

		FAShapes::CreateBox(vertices, triangles);

		mCurrentBox.InitializeRigidBox(5.0f, 1.0f, 1.0f, 1.0f, FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(),
			FAColor::Color(1.0f, 0.0f, 0.0f, 1.0f), vertices, triangles);

		mPreviousBox = mCurrentBox;
		mInterpolatedBox = mCurrentBox;

		mShapes.at(RIGID_BOX)->SetDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_BOX, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
	}

	void Model::CreateCone()
	{
		std::vector<FAShapes::Vertex> vertices;
		std::vector<FAShapes::Triangle> triangles;

		FAShapes::CreateCone(vertices, triangles);

		mCurrentCone.InitializeRigidCone(2.0f, 2.0f, 1.0f, FAMath::Vector4D(5.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(),
			FAColor::Color(1.0f, 1.0f, 0.0f, 1.0f), vertices, triangles);

		mPreviousCone = mCurrentCone;
		mInterpolatedCone = mCurrentCone;

		mShapes.at(RIGID_CONE)->SetDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_CONE, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
	}

	void Model::CreateCylinder()
	{
		std::vector<FAShapes::Vertex> vertices;
		std::vector<FAShapes::Triangle> triangles;

		FAShapes::CreateCylinder(vertices, triangles);

		mCurrentCylinder.InitializeRigidCylinder(2.0f, 2.0f, 1.0f, FAMath::Vector4D(10.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(),
			FAColor::Color(1.0f, 0.0f, 1.0f, 1.0f), vertices, triangles);

		mPreviousCylinder = mCurrentCylinder;
		mInterpolatedCylinder = mCurrentCylinder;

		mShapes.at(RIGID_CYLINDER)->SetDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_CYLINDER, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
	}

	void Model::CreateSphere()
	{
		std::vector<FAShapes::Vertex> vertices;
		std::vector<FAShapes::Triangle> triangles;

		FAShapes::CreateSphere(vertices, triangles);

		mCurrentSphere.InitializeRigidSphere(2.0f, 1.0f, FAMath::Vector4D(15.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(),
			FAColor::Color(0.2f, 0.1f, 1.0f, 1.0f), vertices, triangles);

		mPreviousSphere = mCurrentSphere;
		mInterpolatedSphere = mCurrentSphere;

		mShapes.at(RIGID_SPHERE)->SetDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_SPHERE, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
	}

	void Model::CreatePyramid()
	{
		std::vector<FAShapes::Vertex> vertices;
		std::vector<FAShapes::Triangle> triangles;

		FAShapes::CreatePyramid(vertices, triangles);

		mCurrentPyramid.InitializeRigidPyramid(5.0f, 1.0f, 1.0f, 1.0f, FAMath::Vector4D(20.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(),
			FAColor::Color(0.0f, 0.4f, 0.3f, 1.0f), vertices, triangles);

		mPreviousPyramid = mCurrentPyramid;
		mInterpolatedPyramid = mCurrentPyramid;

		mShapes.at(RIGID_PYRAMID)->SetDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_PYRAMID, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
	}

	void Model::StoreVerticesAndIndices(FARender::RenderScene* scene)
	{
		scene->CreateStaticBuffer(L"Vertex Buffer", mVertexList.data(), (unsigned int)(mVertexList.size() * sizeof(FAShapes::Vertex)),
			sizeof(FAShapes::Vertex));

		scene->CreateStaticBuffer(L"Index Buffer", mIndexList.data(), (unsigned int)(mIndexList.size() * sizeof(unsigned int)), DXGI_FORMAT_R32_UINT);
	}

	void Model::Simulate(float renderTime)
	{
		//set the max number of simulation updates in one frame to 25
		if (renderTime > 0.25f)
		{
			renderTime = 0.25f;
		}

		mAccumulator += renderTime;

		//simulate until accumulator < simulation time
		while (mAccumulator >= mSimulationTime)
		{
			mPreviousBox = mCurrentBox;
			mPreviousCone = mCurrentCone;
			mPreviousCylinder = mCurrentCylinder;
			mPreviousSphere = mCurrentSphere;
			mPreviousPyramid = mCurrentPyramid;

			SimulateRigidBodies();

			mAccumulator -= mSimulationTime;
		}

		//Interpolation value
		mAlpha = mAccumulator / mSimulationTime;

		//Interpolate to avoid stuttering.
		FAPhysics::Interpolate(mPreviousBox.GetRigidBody(), mCurrentBox.GetRigidBody(), mInterpolatedBox.GetRigidBody(), mAlpha);
		FAPhysics::Interpolate(mPreviousCone.GetRigidBody(), mCurrentCone.GetRigidBody(), mInterpolatedCone.GetRigidBody(), mAlpha);
		FAPhysics::Interpolate(mPreviousCylinder.GetRigidBody(), mCurrentCylinder.GetRigidBody(), mInterpolatedCylinder.GetRigidBody(), mAlpha);
		FAPhysics::Interpolate(mPreviousSphere.GetRigidBody(), mCurrentSphere.GetRigidBody(), mInterpolatedSphere.GetRigidBody(), mAlpha);
		FAPhysics::Interpolate(mPreviousPyramid.GetRigidBody(), mCurrentPyramid.GetRigidBody(), mInterpolatedPyramid.GetRigidBody(), mAlpha);
	}

	void Model::UpdateModels(FARender::RenderScene* scene, const FAMath::Matrix4x4& viewMatrix, const FAMath::Matrix4x4& projectionMatrix)
	{
		mInterpolatedBox.UpdateModelMatrix();
		mInterpolatedCone.UpdateModelMatrix();
		mInterpolatedCylinder.UpdateModelMatrix();
		mInterpolatedSphere.UpdateModelMatrix();
		mInterpolatedPyramid.UpdateModelMatrix();

		ObjectConstants data;
		for (const auto& i : mShapes)
		{
			data.MVP = FAMath::Transpose(i->GetModelMatrix() * viewMatrix * projectionMatrix);
			data.color = i->GetColor();
			i->UpdateShape(scene, &data, sizeof(ObjectConstants));
		}

	}

	void Model::RenderModels(FARender::RenderScene* scene)
	{
		for (const auto& i : mShapes)
		{
			i->RenderShape(scene);
		}
	}

	void Model::SimulateRigidBodies()
	{
		for (auto& i : mRigidBodies)
		{
			i->ResetForce();
			i->AddForce(FAPhysics::GravitationalForce(i->GetMass(), 9.81f, FAMath::Vector4D(0.0f, -1.0f, 0.0f, 0.0f)));
			i->AddForce(FAPhysics::DragForce(10.0f, i->GetLinearVelocity()));
			i->ResetTorque();
			i->AddTorque(i->GetNetForce(), i->GetCenterOfMass() + FAMath::Vector4D(0.5f, 0.0f, 0.0f, 0.0f));
			i->Integrate(mSimulationTime);
		}
	}

	void Model::Reset()
	{
		mCurrentBox.SetPosition(FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f));
		mCurrentCone.SetPosition(FAMath::Vector4D(5.0f, 0.0f, 0.0f, 1.0f));
		mCurrentCylinder.SetPosition(FAMath::Vector4D(10.0f, 0.0f, 0.0f, 1.0f));
		mCurrentSphere.SetPosition(FAMath::Vector4D(15.0f, 0.0f, 0.0f, 1.0f));
		mCurrentPyramid.SetPosition(FAMath::Vector4D(20.0f, 0.0f, 0.0f, 1.0f));

		for (auto& i : mRigidBodies)
		{
			i->SetOrientation(FAMath::Quaternion());
			i->SetLinearMomentum(FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f));
			i->SetAngularMomentum(FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f));
		}

		mAccumulator = 0.0f;
		mAlpha = 0.0f;

		mPreviousBox = mCurrentBox;
		FAPhysics::Interpolate(mPreviousBox.GetRigidBody(), mCurrentBox.GetRigidBody(), mInterpolatedBox.GetRigidBody(), mAlpha);

		mPreviousCone = mCurrentCone;
		FAPhysics::Interpolate(mPreviousCone.GetRigidBody(), mCurrentCone.GetRigidBody(), mInterpolatedCone.GetRigidBody(), mAlpha);

		mPreviousCylinder = mCurrentCylinder;
		FAPhysics::Interpolate(mPreviousCylinder.GetRigidBody(), mCurrentCylinder.GetRigidBody(), mInterpolatedCylinder.GetRigidBody(), mAlpha);

		mPreviousSphere = mCurrentSphere;
		FAPhysics::Interpolate(mPreviousSphere.GetRigidBody(), mCurrentSphere.GetRigidBody(), mInterpolatedSphere.GetRigidBody(), mAlpha);

		mPreviousPyramid = mCurrentPyramid;
		FAPhysics::Interpolate(mPreviousPyramid.GetRigidBody(), mCurrentPyramid.GetRigidBody(), mInterpolatedPyramid.GetRigidBody(), mAlpha);
	}
	
}