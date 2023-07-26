#pragma once

#include "RenderScene.h"
#include "RigidBox.h"
#include "RigidCone.h"
#include "RigidCylinder.h"
#include "RigidSphere.h"
#include "RigidPyramid.h"
#include "GameTime.h"
#include "ForceFunctions.h"
#include "CreateShapes.h"
#include "Structures.h"
#include <memory>

namespace MVC
{
	enum Shapes { RIGID_BOX = 0, RIGID_PYRAMID, RIGID_SPHERE, RIGID_CYLINDER, RIGID_CONE };

	class Model
	{
	public:
		Model();

		void StoreVerticesAndIndices(RenderingEngine::RenderScene* scene);

		void Simulate(float renderTime);

		void UpdateModels(RenderingEngine::RenderScene* scene, const mat4& viewMatrix, const mat4& projectionMatrix);

		void RenderModels(RenderingEngine::RenderScene* scene);

		void Reset();

	private:
		void CreateBox();
		void CreateCone();
		void CreateCylinder();
		void CreateSphere();
		void CreatePyramid();

		void SimulateRigidBodies();

	private:
		float mSimulationTime;
		float mAccumulator;
		float mAlpha;

		PhysicsEngine::RigidBox mPreviousBox;
		PhysicsEngine::RigidBox mInterpolatedBox;
		PhysicsEngine::RigidBox mCurrentBox;

		PhysicsEngine::RigidCone mPreviousCone;
		PhysicsEngine::RigidCone mInterpolatedCone;
		PhysicsEngine::RigidCone mCurrentCone;

		PhysicsEngine::RigidCylinder mPreviousCylinder;
		PhysicsEngine::RigidCylinder mInterpolatedCylinder;
		PhysicsEngine::RigidCylinder mCurrentCylinder;

		PhysicsEngine::RigidSphere mPreviousSphere;
		PhysicsEngine::RigidSphere mInterpolatedSphere;
		PhysicsEngine::RigidSphere mCurrentSphere;

		PhysicsEngine::RigidPyramid mPreviousPyramid;
		PhysicsEngine::RigidPyramid mInterpolatedPyramid;
		PhysicsEngine::RigidPyramid mCurrentPyramid;

		std::vector<ShapesEngine::ThreeDimensionalShape*> mShapes;
		std::vector<PhysicsEngine::RigidBody*> mRigidBodies;

		std::vector<ShapesEngine::Vertex> mVertexList;
		std::vector<unsigned int> mIndexList;
	};
}
