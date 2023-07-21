#pragma once

#include "FAMathEngine.h"

namespace FAPhysics
{
	/**brief Returns the force due to gravity based off the specified parameters.
	*
	* Formula used its F = mgU, where m is the mass of the object, g is acceleration due to gravity and U is the gravity direction.
	*/
	FAMath::Vector4D GravitationalForce(float mass, float gravityAcceleration, const FAMath::Vector4D& direction);

	/**brief Returns the force due to drag (e.g. air resistance, friction)
	*
	* Formula used is F = -cv, where v is the velocity of the object and c is the drag coefficient.
	*/
	FAMath::Vector4D DragForce(float dragCoefficent, const FAMath::Vector4D& velocity);

	/**brief Returns a force that is being applied to an object.
	*
	* Formula used is F = magnitude * direction
	*/
	FAMath::Vector4D ApplyForce(float magnitdue, const FAMath::Vector4D& direction);
}