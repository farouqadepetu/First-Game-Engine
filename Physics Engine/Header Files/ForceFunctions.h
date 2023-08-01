#pragma once

#include "MathEngine.h"

namespace PhysicsEngine
{
	/**brief Returns the force due to gravity based off the specified parameters.
	*
	* Formula used its F = mgU, where m is the mass of the object, g is acceleration due to gravity and U is the gravity direction.
	*/
	vec3 GravitationalForce(float mass, float gravityAcceleration, const vec3& direction);

	/**brief Returns the force due to drag.
	*
	* Formula used is F = -v(k1|v| + k2|v|^2), where v is the velocity of the object and k1 and k2 are the drag coefficients.
	*/
	vec3 DragForce(float k1, float k2, const vec3& velocity);

	/**brief Returns a force that is being applied to an object.
	*
	* Formula used is F = magnitude * direction
	*/
	vec3 ApplyForce(float magnitdue, const vec3& direction);
}