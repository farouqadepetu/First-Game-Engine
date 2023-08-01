#pragma once

#include "Box.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Pyramid.h"
#include "RigidShape.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "GameTime.h"
#include "ForceFunctions.h"
#include "CreateShapes.h"
#include "Structures.h"
#include <memory>

namespace MVC
{
	enum Shapes { RIGID_BOX = 0, RIGID_CONE, RIGID_CYLINDER, RIGID_SPHERE, RIGID_PYRAMID };

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
		void CreateBoundingVolumes();

		vec3 NetForce(float mass, const vec3& linearVelocity);
		vec3 NetTorque(const vec3& force, const vec3& angularVelocity, const vec3& centerOfMass, const vec3& point);

	private:
		float mSimulationTime;
		float mAccumulator;
		float mAlpha;


		std::vector<PhysicsEngine::RigidShape> mPreviousRigidShapes;
		std::vector<PhysicsEngine::RigidShape> mInterpolatedRigidShapes;
		std::vector<PhysicsEngine::RigidShape> mCurrentRigidShapes;

		std::vector<ShapesEngine::Vertex> mVertexList;
		std::vector<unsigned int> mIndexList;
	};
}
