#include "PolyhedralMassProperties.h"

namespace PhysicsEngine
{
	//Computes the necessary values necessary to solve the surface integral of q(x(u,v), y(u,v), z(u,v)).
	//where q can equal to x, x^2, y^2, z^2, x^3, y^3, z^3, x^2y, y^2z, z^2x
	void SubExpressions(double w0, double w1, double w2, double& f1, double& f2, double& f3, double& g0, double& g1, double& g2)
	{
		double temp0{ 0.0 };
		double temp1{ 0.0 };
		double temp2{ 0.0 };

		temp0 = w0 + w1;
		f1 = temp0 + w2;
		temp1 = w0 * w0;
		temp2 = temp1 + w1 * temp0;
		f2 = temp2 + w2 * f1;
		f3 = w0 * temp1 + w1 * temp2 + w2 * f2;
		g0 = f2 + w0 * (f1 + w0);
		g1 = f2 + w1 * (f1 + w1);
		g2 = f2 + w2 * (f1 + w2);
	}

	//Computes the mass, center of mass and inertia tensor of a solid polyhedron using the triangles that make up the solid.
	//Computes the volume integrals 1, x, y, z, xˆ2, yˆ2, zˆ2, xy, yz, zx through surface integrals
	void ComputeMassProperties(const std::vector<ShapesEngine::Triangle>& triangles, double& mass, vec3& cm,
		mat3& bodyInertia)
	{
		double f1x{ 0.0 };
		double f2x{ 0.0 };
		double f3x{ 0.0 };

		double f1y{ 0.0 };
		double f2y{ 0.0 };
		double f3y{ 0.0 };

		double f1z{ 0.0 };
		double f2z{ 0.0 };
		double f3z{ 0.0 };

		double g0x{ 0.0 };
		double g0y{ 0.0 };
		double g0z{ 0.0 };

		double g1x{ 0.0 };
		double g1y{ 0.0 };
		double g1z{ 0.0 };

		double g2x{ 0.0 };
		double g2y{ 0.0 };
		double g2z{ 0.0 };

		//order: 1, x, y, z, xˆ2, yˆ2, zˆ2, xy, yz, zx
		double integral[10]{ 0.0 };

		for (auto& i : triangles)
		{
			//the indicies of the points that make up the triangles
			double p0x = i.vertexList[i.p0].position.x;
			double p0y = i.vertexList[i.p0].position.y;
			double p0z = i.vertexList[i.p0].position.z;

			double p1x = i.vertexList[i.p1].position.x;
			double p1y = i.vertexList[i.p1].position.y;
			double p1z = i.vertexList[i.p1].position.z;

			double p2x = i.vertexList[i.p2].position.x;
			double p2y = i.vertexList[i.p2].position.y;
			double p2z = i.vertexList[i.p2].position.z;

			//calculate the normal of the triangle
			vec3 p01{ i.vertexList[i.p1].position - i.vertexList[i.p0].position };
			vec3 p02{ i.vertexList[i.p2].position - i.vertexList[i.p0].position };
			vec3 result{ CrossProduct(p01, p02) };

			double nx = result.x;
			double ny = result.y;
			double nz = result.z;

			//Compute integral terms
			SubExpressions(p0x, p1x, p2x, f1x, f2x, f3x, g0x, g1x, g2x);
			SubExpressions(p0y, p1y, p2y, f1y, f2y, f3y, g0y, g1y, g2y);
			SubExpressions(p0z, p1z, p2z, f1z, f2z, f3z, g0z, g1z, g2z);

			//update integrals
			integral[0] += nx * f1x;
			integral[1] += nx * f2x;
			integral[2] += ny * f2y;
			integral[3] += nz * f2z;
			integral[4] += nx * f3x;
			integral[5] += ny * f3y;
			integral[6] += nz * f3z;
			integral[7] += nx * (p0y * g0x + p1y * g1x + p2y * g2x);
			integral[8] += ny * (p0z * g0y + p1z * g1y + p2z * g2y);
			integral[9] += nz * (p0x * g0z + p1x * g1z + p2x * g2z);
		}

		integral[0] /= 6.0;
		integral[1] /= 24.0;
		integral[2] /= 24.0;
		integral[3] /= 24.0;
		integral[4] /= 60.0;
		integral[5] /= 60.0;
		integral[6] /= 60.0;
		integral[7] /= 120.0;
		integral[8] /= 120.0;
		integral[9] /= 120.0;

		mass = integral[0];

		double inverseMass{ 1.0f / integral[0] };

		//(x / m, y / m, z / m)
		cm = vec3{ (float)(integral[1] * inverseMass), (float)(integral[2] * inverseMass), (float)(integral[3] * inverseMass) };

		//inertia relative to the world origin
		mat3 worldInertia;
		worldInertia(0, 0) = (float)(integral[5] + integral[6]); //Ixx = y^2 + z^2;
		worldInertia(0, 1) = (float)-integral[7]; //Ixy = xy
		worldInertia(0, 2) = (float)-integral[9]; //Ixz = xz
		worldInertia(1, 0) = worldInertia(0, 1); //Iyx = Ixy = yx
		worldInertia(1, 1) = (float)(integral[4] + integral[6]); //Iyy = x^2 + z^2
		worldInertia(1, 2) = (float)-integral[8]; //Iyz = yz
		worldInertia(2, 0) = worldInertia(0, 2); //Ixz = Izx = zx
		worldInertia(2, 1) = worldInertia(1, 2);//Iyz = Izy = zy
		worldInertia(2, 2) = (float)(integral[4] + integral[5]);//Izz = x^2 + y^2

		//inertia relative to the center of mass in body coordinates
		bodyInertia(0, 0) = (float)(worldInertia(0, 0) - mass * (cm.y * cm.y + cm.z * cm.z));//Ixx - m(cm.y^2 + cm.z^2)
		bodyInertia(0, 1) = (float)(worldInertia(0, 1) + mass * cm.x * cm.y); //Ixy + mxy
		bodyInertia(0, 2) = (float)(worldInertia(0, 2) + mass * cm.z * cm.x); //Ixz + mxz
		bodyInertia(1, 0) = bodyInertia(0, 1);
		bodyInertia(1, 1) = (float)(worldInertia(1, 1) - mass * (cm.x * cm.x + cm.z * cm.z));//Iyy - m(cm.x^2 + cm.z^2)
		bodyInertia(1, 2) = (float)(worldInertia(1, 2) + mass * cm.y * cm.z); //Iyz + myz
		bodyInertia(2, 0) = bodyInertia(0, 2);
		bodyInertia(2, 1) = bodyInertia(1, 2);
		bodyInertia(2, 2) = (float)(worldInertia(2, 2) - mass * (cm.x * cm.x + cm.y * cm.y));//Izz - m(cm.x^2 + cm.y^2)
	}
}