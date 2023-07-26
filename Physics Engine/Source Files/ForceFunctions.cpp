#include "ForceFunctions.h"

namespace PhysicsEngine
{
	vec3 GravitationalForce(float mass, float gravityAcceleration, const vec3& direction)
	{
		//F = mgU, where m is the mass of the object, g is gravity due to acceleration and U is the gravity directon.
		return mass * gravityAcceleration * MathEngine::Normalize(direction);
	}

	vec3 DragForce(float dragCoefficent, const vec3& velocity)
	{
		return -(dragCoefficent * velocity);
	}

	vec3 ApplyForce(float magnitdue, const vec3& direction)
	{
		return magnitdue * MathEngine::Normalize(direction);
	}
}