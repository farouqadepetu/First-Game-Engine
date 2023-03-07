#pragma once

/** @file FAMathUtility.h
*	@brief File that has math utility functions.
*/

#include <cmath>

#if defined(_DEBUG)
#include <iostream>
#endif


#define EPSILON 1e-6
#define PI 3.14159265

/** @namespace FAMath
*	@brief Has utility functions, Vector2D, Vector3D, Vector4D, Matrix4x4, and Quaternion classes.
*/
namespace FAMath
{
	/*@brief Checks if the two specified floats are equal using exact epsilion and adaptive epsilion.
	*/
	bool compareFloats(float x, float y, float epsilon);

	/*@brief Checks if the two specified doubles are equal using exact epsilion and adaptive epsilion.
	*/
	bool compareDoubles(double x, double y, double epsilon);

	class Vector2D;
	class Vector3D;
	class Vector4D;
}