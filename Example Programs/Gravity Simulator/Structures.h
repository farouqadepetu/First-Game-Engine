#pragma once

#include "FAColor.h"

struct ObjectConstants
{
	FAMath::Matrix4x4 MVP;
	FAColor::Color color;

	FAMath::Matrix4x4 pad0;
	FAMath::Matrix4x4 pad1;
	FAMath::Matrix3x3 pad2;
	FAMath::Vector3D pad3;
};