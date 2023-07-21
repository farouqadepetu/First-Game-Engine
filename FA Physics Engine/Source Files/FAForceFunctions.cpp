#include "FAForceFunctions.h"

namespace FAPhysics
{
	FAMath::Vector4D GravitationalForce(float mass, float gravityAcceleration, const FAMath::Vector4D& direction)
	{
		//F = mgU, where m is the mass of the object, g is gravity due to acceleration and U is the gravity directon.
		return mass * gravityAcceleration * FAMath::Norm(direction);
	}

	FAMath::Vector4D DragForce(float dragCoefficent, const FAMath::Vector4D& velocity)
	{
		return -(dragCoefficent * velocity);
	}

	FAMath::Vector4D ApplyForce(float magnitdue, const FAMath::Vector4D& direction)
	{
		return magnitdue * direction;
	}
}