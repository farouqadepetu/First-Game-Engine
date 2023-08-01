#include "ForceFunctions.h"

namespace PhysicsEngine
{
	vec3 GravitationalForce(float mass, float gravityAcceleration, const vec3& direction)
	{
		//F = mgU, where m is the mass of the object, g is gravity due to acceleration and U is the gravity directon.
		return mass * gravityAcceleration * MathEngine::Normalize(direction);
	}

	vec3 DragForce(float k1, float k2, const vec3& velocity)
	{
		vec3 vel(MathEngine::Normalize(velocity));

		float speed{ MathEngine::Length(velocity) };

		return -vel * (k1 * speed + k2 * speed * speed);
	}


	vec3 ApplyForce(float magnitdue, const vec3& direction)
	{
		return magnitdue * MathEngine::Normalize(direction);
	}
}