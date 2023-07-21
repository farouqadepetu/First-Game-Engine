#pragma once

#include "FARenderScene.h"
#include "FARigidBox.h"
#include "FARigidCone.h"
#include "FARigidCylinder.h"
#include "FARigidSphere.h"
#include "FARigidPyramid.h"
#include "FATime.h"
#include "FAForceFunctions.h"
#include "FAShapesUtility.h"
#include "Structures.h"
#include "FAAABB.h"
#include <memory>

namespace MVC
{
	class Model
	{
	public:
		Model();

		void StoreVerticesAndIndices(FARender::RenderScene* scene);

		void Simulate(float renderTime);

		void UpdateModels(FARender::RenderScene* scene, const FAMath::Matrix4x4& viewMatrix, const FAMath::Matrix4x4& projectionMatrix);

		void RenderModels(FARender::RenderScene* scene);

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

		FAPhysicsShapes::RigidBox mPreviousBox;
		FAPhysicsShapes::RigidBox mInterpolatedBox;
		FAPhysicsShapes::RigidBox mCurrentBox;

		FAPhysicsShapes::RigidCone mPreviousCone;
		FAPhysicsShapes::RigidCone mInterpolatedCone;
		FAPhysicsShapes::RigidCone mCurrentCone;

		FAPhysicsShapes::RigidCylinder mPreviousCylinder;
		FAPhysicsShapes::RigidCylinder mInterpolatedCylinder;
		FAPhysicsShapes::RigidCylinder mCurrentCylinder;

		FAPhysicsShapes::RigidSphere mPreviousSphere;
		FAPhysicsShapes::RigidSphere mInterpolatedSphere;
		FAPhysicsShapes::RigidSphere mCurrentSphere;

		FAPhysicsShapes::RigidPyramid mPreviousPyramid;
		FAPhysicsShapes::RigidPyramid mInterpolatedPyramid;
		FAPhysicsShapes::RigidPyramid mCurrentPyramid;

		std::vector<FAShapes::ThreeDimensionalShape*> mShapes;
		std::vector<FAPhysics::RigidBody*> mRigidBodies;

		std::vector<FAShapes::Vertex> mVertexList;
		std::vector<unsigned int> mIndexList;
	};
}
