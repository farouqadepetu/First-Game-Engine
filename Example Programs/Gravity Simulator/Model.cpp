#include "Model.h"
#include <Windows.h>
#include <string>

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
		std::vector<ShapesEngine::Vertex> vertices;
		std::vector<ShapesEngine::Triangle> triangles;

		ShapesEngine::CreateBox(vertices, triangles);

		mCurrentBox.InitializeRigidBox(5.0f, 1.0f, 1.0f, 1.0f, vec3{ 0.0f, 0.0f, 0.0f }, MathEngine::Quaternion{},
			RenderingEngine::Color(1.0f, 0.0f, 0.0f, 1.0f), vertices, triangles);

		mPreviousBox = mCurrentBox;
		mInterpolatedBox = mCurrentBox;

		SetDrawArguments(*mShapes.at(RIGID_BOX), (unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_BOX, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.p0);
			mIndexList.push_back(i.p1);
			mIndexList.push_back(i.p2);
		}
	}

	void Model::CreateCone()
	{
		std::vector<ShapesEngine::Vertex> vertices;
		std::vector<ShapesEngine::Triangle> triangles;

		ShapesEngine::CreateCone(vertices, triangles);

		mCurrentCone.InitializeRigidCone(2.0f, 2.0f, 1.0f, vec3{ 5.0f, 0.0f, 0.0f }, MathEngine::Quaternion{},
			RenderingEngine::Color(1.0f, 1.0f, 0.0f, 1.0f), vertices, triangles);

		mPreviousCone = mCurrentCone;
		mInterpolatedCone = mCurrentCone;

		SetDrawArguments(*mShapes.at(RIGID_CONE), (unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_CONE, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.p0);
			mIndexList.push_back(i.p1);
			mIndexList.push_back(i.p2);
		}
	}

	void Model::CreateCylinder()
	{
		std::vector<ShapesEngine::Vertex> vertices;
		std::vector<ShapesEngine::Triangle> triangles;

		ShapesEngine::CreateCylinder(vertices, triangles);

		mCurrentCylinder.InitializeRigidCylinder(2.0f, 2.0f, 1.0f, vec3{ 10.0f, 0.0f, 0.0f }, MathEngine::Quaternion{},
			RenderingEngine::Color(1.0f, 0.0f, 1.0f, 1.0f), vertices, triangles);

		mPreviousCylinder = mCurrentCylinder;
		mInterpolatedCylinder = mCurrentCylinder;

		SetDrawArguments(*mShapes.at(RIGID_CYLINDER), (unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_CYLINDER, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.p0);
			mIndexList.push_back(i.p1);
			mIndexList.push_back(i.p2);
		}
	}

	void Model::CreateSphere()
	{
		std::vector<ShapesEngine::Vertex> vertices;
		std::vector<ShapesEngine::Triangle> triangles;

		ShapesEngine::CreateSphere(vertices, triangles);

		mCurrentSphere.InitializeRigidSphere(2.0f, 1.0f, vec3{ 15.0f, 0.0f, 0.0f }, MathEngine::Quaternion{},
			RenderingEngine::Color(0.2f, 0.1f, 1.0f, 1.0f), vertices, triangles);

		mPreviousSphere = mCurrentSphere;
		mInterpolatedSphere = mCurrentSphere;

		SetDrawArguments(*mShapes.at(RIGID_SPHERE), (unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_SPHERE, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.p0);
			mIndexList.push_back(i.p1);
			mIndexList.push_back(i.p2);
		}
	}

	void Model::CreatePyramid()
	{
		std::vector<ShapesEngine::Vertex> vertices;
		std::vector<ShapesEngine::Triangle> triangles;

		ShapesEngine::CreatePyramid(vertices, triangles);

		mCurrentPyramid.InitializeRigidPyramid(5.0f, 1.0f, 1.0f, 1.0f, vec3{ 20.0f, 0.0f, 0.0f }, MathEngine::Quaternion{},
			RenderingEngine::Color(0.0f, 0.4f, 0.3f, 1.0f), vertices, triangles);

		mPreviousPyramid = mCurrentPyramid;
		mInterpolatedPyramid = mCurrentPyramid;

		SetDrawArguments(*mShapes.at(RIGID_PYRAMID), (unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
			RIGID_PYRAMID, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.p0);
			mIndexList.push_back(i.p1);
			mIndexList.push_back(i.p2);
		}
	}

	void Model::StoreVerticesAndIndices(RenderingEngine::RenderScene* scene)
	{
		scene->CreateStaticBuffer(L"Vertex Buffer", mVertexList.data(), (unsigned int)(mVertexList.size() * sizeof(ShapesEngine::Vertex)),
			sizeof(ShapesEngine::Vertex));

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
		PhysicsEngine::Interpolate(mPreviousBox.GetRigidBody(), mCurrentBox.GetRigidBody(), mInterpolatedBox.GetRigidBody(), mAlpha);
		PhysicsEngine::Interpolate(mPreviousCone.GetRigidBody(), mCurrentCone.GetRigidBody(), mInterpolatedCone.GetRigidBody(), mAlpha);
		PhysicsEngine::Interpolate(mPreviousCylinder.GetRigidBody(), mCurrentCylinder.GetRigidBody(), mInterpolatedCylinder.GetRigidBody(), mAlpha);
		PhysicsEngine::Interpolate(mPreviousSphere.GetRigidBody(), mCurrentSphere.GetRigidBody(), mInterpolatedSphere.GetRigidBody(), mAlpha);
		PhysicsEngine::Interpolate(mPreviousPyramid.GetRigidBody(), mCurrentPyramid.GetRigidBody(), mInterpolatedPyramid.GetRigidBody(), mAlpha);
	}

	void Model::UpdateModels(RenderingEngine::RenderScene* scene, const MathEngine::Matrix4x4& viewMatrix, const MathEngine::Matrix4x4& projectionMatrix)
	{
		mInterpolatedBox.UpdateModelMatrix();
		mInterpolatedCone.UpdateModelMatrix();
		mInterpolatedCylinder.UpdateModelMatrix();
		mInterpolatedSphere.UpdateModelMatrix();
		mInterpolatedPyramid.UpdateModelMatrix();

		ObjectConstants data;
		for (const auto& i : mShapes)
		{
			data.MVP = MathEngine::Transpose(i->modelMatrix * viewMatrix * projectionMatrix);
			data.color = i->color;
			ShapesEngine::UpdateShape(*i, scene, &data, sizeof(ObjectConstants));
		}
	}

	void Model::RenderModels(RenderingEngine::RenderScene* scene)
	{
		for (const auto& i : mShapes)
		{
			ShapesEngine::RenderShape(*i, scene);
		}
	}

	void Model::SimulateRigidBodies()
	{
		for (auto& i : mRigidBodies)
		{
			vec3 direction{ 0.0f, -1.0f, 0.0f };
			i->ResetForce();
			i->AddForce(PhysicsEngine::GravitationalForce(i->GetMass(), 9.81f, direction));
			i->AddForce(PhysicsEngine::DragForce(10.0f, i->GetLinearVelocity()));

			i->ResetTorque();
			i->AddTorque(i->GetNetForce(), i->GetCenterOfMass() + vec3{ 0.5f, 0.0f, 0.0f });

			i->Integrate(mSimulationTime);
		}
	}

	void Model::Reset()
	{
		mCurrentBox.SetPosition(vec3{ 0.0f, 0.0f, 0.0f });
		mCurrentCone.SetPosition(vec3{ 5.0f, 0.0f, 0.0f });
		mCurrentCylinder.SetPosition(vec3{ 10.0f, 0.0f, 0.0f });
		mCurrentSphere.SetPosition(vec3{ 15.0f, 0.0f, 0.0f });
		mCurrentPyramid.SetPosition(vec3{ 20.0f, 0.0f, 0.0f });

		for (auto& i : mRigidBodies)
		{
			i->SetOrientation(MathEngine::Quaternion{});
			i->SetLinearMomentum(vec3{ 0.0f, 0.0f, 0.0f });
			i->SetAngularMomentum(vec3{ 0.0f, 0.0f, 0.0f });
		}

		mAccumulator = 0.0f;
		mAlpha = 0.0f;

		mPreviousBox = mCurrentBox;
		PhysicsEngine::Interpolate(mPreviousBox.GetRigidBody(), mCurrentBox.GetRigidBody(), mInterpolatedBox.GetRigidBody(), mAlpha);

		mPreviousCone = mCurrentCone;
		PhysicsEngine::Interpolate(mPreviousCone.GetRigidBody(), mCurrentCone.GetRigidBody(), mInterpolatedCone.GetRigidBody(), mAlpha);

		mPreviousCylinder = mCurrentCylinder;
		PhysicsEngine::Interpolate(mPreviousCylinder.GetRigidBody(), mCurrentCylinder.GetRigidBody(), mInterpolatedCylinder.GetRigidBody(), mAlpha);

		mPreviousSphere = mCurrentSphere;
		PhysicsEngine::Interpolate(mPreviousSphere.GetRigidBody(), mCurrentSphere.GetRigidBody(), mInterpolatedSphere.GetRigidBody(), mAlpha);

		mPreviousPyramid = mCurrentPyramid;
		PhysicsEngine::Interpolate(mPreviousPyramid.GetRigidBody(), mCurrentPyramid.GetRigidBody(), mInterpolatedPyramid.GetRigidBody(), mAlpha);
	}
	
}