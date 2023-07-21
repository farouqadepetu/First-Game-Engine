#pragma once

#include "FATriangle.h"
#include <vector>

namespace FAPhysics
{
	/**brief These are the expressions used in computing the mass properties.
	*/
	void SubExpressions(double w0, double w1, double w2, double& f1, double& f2, double& f3, double& g0, double& g1, double& g2);

	/**brief Computes the mass, center of mass and inertia tensor reltaive to the center of mass in body coordinates for a soild polyhedron.
	* 
	* Uses the triangles that make up the solid polyhedron to compute the values.\n
	* 
	* Assumes the mass density is 1, so if it is not you need to multiple the mass and body intertia by the mass density to get the correct values.
	*/
	void ComputeMassProperties(const std::vector<FAShapes::Triangle>& triangles, double& mass, FAMath::Vector4D& cm,
		FAMath::Matrix3x3& bodyInertia);
}