#include "Model.h"

namespace MVC
{
	Model::Model() : mSimulationTime{ 0.1f }, mAccumulator{ 0.0f }, mAlpha{ 0.0f }
	{
		for (unsigned int i = 0; i < 5; ++i)
		{
			mPreviousRigidShapes.emplace_back();
			mInterpolatedRigidShapes.emplace_back();
			mCurrentRigidShapes.emplace_back();
		}

		CreateBox();
		CreateCone();
		CreateCylinder();
		CreateSphere();
		CreatePyramid();
		CreateBoundingVolumes();
	}

	void Model::CreateBox()
	{
		std::vector<ShapesEngine::Vertex> vertices;
		std::vector<ShapesEngine::Triangle> triangles;

		ShapesEngine::CreateBox(vertices, triangles);

		vec3 position{ -2.0f, 0.0f, 0.0f };
		MathEngine::Quaternion orientation{};
		RenderingEngine::Color color(1.0f, 0.0f, 0.0f, 1.0f);
		RenderingEngine::Color boundingVolumeColor(1.0f, 1.0f, 0.0f, 1.0f);
		float massDensity{ 1.0f };
		float width{ 1.0f };
		float height{ 1.0f };
		float depth{ 1.0f };

		/*mPreviousRigidShapes.at(RIGID_BOX).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Box>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_BOX).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Box>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_BOX).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Box>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));*/

		mPreviousRigidShapes.at(RIGID_BOX).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Box>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_BOX).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Box>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_BOX).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Box>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_BOX).SetDrawArguments(
			RenderingEngine::MakeDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
				RIGID_BOX, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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

		vec3 position{ 4.0f, 0.0f, 0.0f };
		MathEngine::Quaternion orientation{};
		RenderingEngine::Color color(1.0f, 0.5f, 0.5f, 1.0f);
		RenderingEngine::Color boundingVolumeColor(1.0f, 1.0f, 0.0f, 1.0f);
		float massDensity{ 1.0f };
		float radius{ 1.0f };
		float height{ 1.0f };

		/*mPreviousRigidShapes.at(RIGID_CONE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cone>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_CONE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cone>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_CONE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cone>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));*/

		mPreviousRigidShapes.at(RIGID_CONE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cone>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_CONE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cone>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_CONE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cone>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_CONE).SetDrawArguments(
			RenderingEngine::MakeDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
				RIGID_CONE, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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

		vec3 position{ 10.0f, 0.0f, 0.0f };
		MathEngine::Quaternion orientation{};
		RenderingEngine::Color color(1.0f, 0.0f, 1.0f, 1.0f);
		RenderingEngine::Color boundingVolumeColor(1.0f, 1.0f, 0.0f, 1.0f);
		float massDensity{ 1.0f };
		float radius{ 1.0f };
		float height{ 1.0f };

		/*mPreviousRigidShapes.at(RIGID_CYLINDER).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cylinder>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_CYLINDER).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cylinder>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_CYLINDER).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cylinder>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));*/

		mPreviousRigidShapes.at(RIGID_CYLINDER).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cylinder>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_CYLINDER).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cylinder>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_CYLINDER).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Cylinder>(radius, height, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_CYLINDER).SetDrawArguments(
			RenderingEngine::MakeDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
				RIGID_CYLINDER, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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

		vec3 position{ 16.0f, 0.0f, 0.0f };
		MathEngine::Quaternion orientation{};
		RenderingEngine::Color color(0.2f, 0.1f, 1.0f, 1.0f);
		RenderingEngine::Color boundingVolumeColor(1.0f, 1.0f, 0.0f, 1.0f);
		float massDensity{ 0.75f };
		float radius{ 1.0f };;

		/*mPreviousRigidShapes.at(RIGID_SPHERE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Sphere>(radius, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_SPHERE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Sphere>(radius, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_SPHERE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Sphere>(radius, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));*/

		mPreviousRigidShapes.at(RIGID_SPHERE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Sphere>(radius, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_SPHERE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Sphere>(radius, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_SPHERE).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Sphere>(radius, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_SPHERE).SetDrawArguments(
			RenderingEngine::MakeDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
				RIGID_SPHERE, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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

		vec3 position{ 22.0f, 0.0f, 0.0f };
		MathEngine::Quaternion orientation{};
		RenderingEngine::Color color(0.0f, 0.4f, 0.3f, 1.0f);
		RenderingEngine::Color boundingVolumeColor(1.0f, 1.0f, 0.0f, 1.0f);
		float massDensity{ 2.0f };
		float width{ 1.0f };
		float height{ 5.0f };
		float depth{ 1.0f };

		/*mPreviousRigidShapes.at(RIGID_PYRAMID).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Pyramid>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_PYRAMID).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Pyramid>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_PYRAMID).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Pyramid>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingBox>(vertices, boundingVolumeColor));*/

		mPreviousRigidShapes.at(RIGID_PYRAMID).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Pyramid>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_PYRAMID).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Pyramid>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mCurrentRigidShapes.at(RIGID_PYRAMID).InitializeRigidShape(massDensity, triangles,
			std::make_unique<ShapesEngine::Pyramid>(width, height, depth, position, orientation, color),
			std::make_unique<PhysicsEngine::BoundingSphere>(vertices, boundingVolumeColor));

		mInterpolatedRigidShapes.at(RIGID_PYRAMID).SetDrawArguments(
			RenderingEngine::MakeDrawArguments((unsigned int)triangles.size() * 3, (unsigned int)mIndexList.size(), (int)mVertexList.size(),
				RIGID_PYRAMID, L"Object Constant Buffer", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (auto& i : triangles)
		{
			mIndexList.push_back(i.p0);
			mIndexList.push_back(i.p1);
			mIndexList.push_back(i.p2);
		}
	}

	void Model::CreateBoundingVolumes()
	{
		RenderingEngine::DrawArguments sphereDrawArgs(mInterpolatedRigidShapes.at(RIGID_SPHERE).GetDrawArguments());
		RenderingEngine::DrawArguments boxDrawArgs(mInterpolatedRigidShapes.at(RIGID_BOX).GetDrawArguments());

		for (unsigned int i = 0; i < 5; ++i)
		{
			/*mInterpolatedRigidShapes.at(i).SetBoundingVolumeDrawArguments(
				RenderingEngine::MakeDrawArguments(boxDrawArgs.indexCount, boxDrawArgs.locationOfFirstIndex, boxDrawArgs.indexOfFirstVertex,
					i + 5, boxDrawArgs.constantBufferKey, boxDrawArgs.rootParameterIndex, boxDrawArgs.primtive));*/

			mInterpolatedRigidShapes.at(i).SetBoundingVolumeDrawArguments(
				RenderingEngine::MakeDrawArguments(sphereDrawArgs.indexCount, sphereDrawArgs.locationOfFirstIndex, sphereDrawArgs.indexOfFirstVertex,
					i + 5, sphereDrawArgs.constantBufferKey, sphereDrawArgs.rootParameterIndex, sphereDrawArgs.primtive));
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
		static float total{ 0.0f };
		//set the max number of simulation updates in one frame to 25
		if (renderTime > 0.25f)
		{
			renderTime = 0.25f;
		}

		mAccumulator += renderTime;

		//simulate until accumulator < simulation time
		while (mAccumulator >= mSimulationTime)
		{
			for (unsigned int i = 0; i < 5; ++i)
			{
				vec3 force(NetForce(mCurrentRigidShapes.at(i).GetMass(), mCurrentRigidShapes.at(i).GetLinearVelocity()));

				vec3 torque(NetTorque(force, mCurrentRigidShapes.at(i).GetAngularVelocity(),
					mCurrentRigidShapes.at(i).GetCenterOfMass(), mCurrentRigidShapes.at(i).GetCenterOfMass() + vec3{ 0.5f, 0.0f, 0.0f }));

				PhysicsEngine::SimulateRigidShape(mPreviousRigidShapes.at(i), mCurrentRigidShapes.at(i), force, torque, mSimulationTime);
			}

			mAccumulator -= mSimulationTime;
		}

		//Interpolation value
		mAlpha = mAccumulator / mSimulationTime;

		//Interpolate to avoid stuttering.
		for (unsigned int i = 0; i < 5; ++i)
		{
			PhysicsEngine::Interpolate(mPreviousRigidShapes.at(i), mCurrentRigidShapes.at(i), mInterpolatedRigidShapes.at(i), mAlpha);
		}
	}

	void Model::UpdateModels(RenderingEngine::RenderScene* scene, const MathEngine::Matrix4x4& viewMatrix, const MathEngine::Matrix4x4& projectionMatrix)
	{
		ObjectConstants data;
		for (auto& i : mInterpolatedRigidShapes)
		{
			i.UpdateModelMatrix();
			data.MVP = MathEngine::Transpose(i.GetModelMatrix() * viewMatrix * projectionMatrix);
			data.color = i.GetColor();
			RenderingEngine::Update(scene, i.GetDrawArguments(), &data, sizeof(ObjectConstants));

			data.MVP = MathEngine::Transpose(i.GetBoundingVolumeModelMatrix() * viewMatrix * projectionMatrix);
			data.color = i.GetBoundingVolumeColor();
			RenderingEngine::Update(scene, i.GetBoundingVolumeDrawArguments(), &data, sizeof(ObjectConstants));
		}
	}

	void Model::RenderModels(RenderingEngine::RenderScene* scene)
	{
		for (const auto& i : mInterpolatedRigidShapes)
		{
			RenderingEngine::Render(scene, i.GetDrawArguments());

			RenderingEngine::Render(scene, i.GetBoundingVolumeDrawArguments());
		}
	}

	vec3 Model::NetForce(float mass, const vec3& linearVelocity)
	{
		return PhysicsEngine::GravitationalForce(mass, 9.81f, vec3{ 0.0f, -1.0f, 0.0f }) + PhysicsEngine::DragForce(10.0f, 0.0f, linearVelocity);
	}

	vec3 Model::NetTorque(const vec3& force, const vec3& angularVelocity, const vec3& centerOfMass, const vec3& point)
	{
		return MathEngine::CrossProduct(point - centerOfMass, force); //PhysicsEngine::DragForce(2.0f, 0.0f, angularVelocity);
	}

	void Model::Reset()
	{
		vec3 position{ -2.0f, 0.0f, 0.0f };
		for (unsigned int i = 0; i < 5; ++i)
		{
			mPreviousRigidShapes.at(i).SetPosition(position);
			mPreviousRigidShapes.at(i).SetOrientation(MathEngine::Quaternion{});
			mPreviousRigidShapes.at(i).SetLinearMomentum(vec3{ 0.0f, 0.0f, 0.0f });
			mPreviousRigidShapes.at(i).SetAngularMomentum(vec3{ 0.0f, 0.0f, 0.0f });

			mInterpolatedRigidShapes.at(i).SetPosition(position);
			mInterpolatedRigidShapes.at(i).SetOrientation(MathEngine::Quaternion{});
			mInterpolatedRigidShapes.at(i).SetLinearMomentum(vec3{ 0.0f, 0.0f, 0.0f });
			mInterpolatedRigidShapes.at(i).SetAngularMomentum(vec3{ 0.0f, 0.0f, 0.0f });

			mCurrentRigidShapes.at(i).SetPosition(position);
			mCurrentRigidShapes.at(i).SetOrientation(MathEngine::Quaternion{});
			mCurrentRigidShapes.at(i).SetLinearMomentum(vec3{ 0.0f, 0.0f, 0.0f });
			mCurrentRigidShapes.at(i).SetAngularMomentum(vec3{ 0.0f, 0.0f, 0.0f });
			position += vec3{ 6.0f, 0.0f, 0.0f };
		}

		mAccumulator = 0.0f;
		mAlpha = 0.0f;
	}

}