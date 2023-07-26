#pragma once

#include "MathEngine.h"

namespace PhysicsEngine
{
	/**brief Returns the force due to gravity based off the specified parameters.
	*
	* Formula used its F = mgU, where m is the mass of the object, g is acceleration due to gravity and U is the gravity direction.
	*/
	vec3 GravitationalForce(float mass, float gravityAcceleration, const vec3& direction);

	/**brief Returns the force due to drag (e.g. air resistance, friction)
	*
	* Formula used is F = -cv, where v is the velocity of the object and c is the drag coefficient.
	*/
	vec3 DragForce(float dragCoefficent, const vec3& velocity);

	/**brief Returns a force that is being applied to an object.
	*
	* Formula used is F = magnitude * direction
	*/
	vec3 ApplyForce(float magnitdue, const vec3& direction);
}