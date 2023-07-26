#pragma once

#include "Color.h"

struct ObjectConstants
{
	mat4 MVP;
	RenderingEngine::Color color;

	MathEngine::Matrix4x4 pad0;
	MathEngine::Matrix4x4 pad1;
	MathEngine::Matrix3x3 pad2;
	MathEngine::Vector3D pad3;
};