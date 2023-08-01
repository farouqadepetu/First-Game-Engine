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
			//the points that make up the triangle
			vec3 p0(i.vertexList[i.p0].position);
			vec3 p1(i.vertexList[i.p1].position);
			vec3 p2(i.vertexList[i.p2].position);

			//calculate the normal of the triangle
			vec3 p01{ p1 - p0 };
			vec3 p02{ p2 - p0 };
			vec3 n{ CrossProduct(p01, p02) };

			//Compute integral terms
			SubExpressions(p0.x, p1.x, p2.x, f1x, f2x, f3x, g0x, g1x, g2x);
			SubExpressions(p0.y, p1.y, p2.y, f1y, f2y, f3y, g0y, g1y, g2y);
			SubExpressions(p0.z, p1.z, p2.z, f1z, f2z, f3z, g0z, g1z, g2z);

			//update integrals
			integral[0] += n.x * f1x;
			integral[1] += n.x * f2x;
			integral[2] += n.y * f2y;
			integral[3] += n.z * f2z;
			integral[4] += n.x * f3x;
			integral[5] += n.y * f3y;
			integral[6] += n.z * f3z;
			integral[7] += n.x * (p0.y * g0x + p1.y * g1x + p2.y * g2x);
			integral[8] += n.y * (p0.z * g0y + p1.z * g1y + p2.z * g2y);
			integral[9] += n.z * (p0.x * g0z + p1.x * g1z + p2.x * g2z);
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

	//Computes the mass, center of mass and inertia tensor of a solid polyhedron using the triangles that make up the solid.
	//Computes the volume integrals 1, x, y, z, xˆ2, yˆ2, zˆ2, xy, yz, zx through surface integrals
	void ComputeMassProperties(const std::vector<ShapesEngine::Triangle>& triangles, double& mass, vec3& cm,
		mat3& bodyInertia, const mat3& scale)
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
			//the points that make up the triangle
			vec3 p0(i.vertexList[i.p0].position * scale);
			vec3 p1(i.vertexList[i.p1].position * scale);
			vec3 p2(i.vertexList[i.p2].position * scale);

			//calculate the normal of the triangle
			vec3 p01{ p1 - p0 };
			vec3 p02{ p2 - p0 };
			vec3 n{ CrossProduct(p01, p02) };

			//Compute integral terms
			SubExpressions(p0.x, p1.x, p2.x, f1x, f2x, f3x, g0x, g1x, g2x);
			SubExpressions(p0.y, p1.y, p2.y, f1y, f2y, f3y, g0y, g1y, g2y);
			SubExpressions(p0.z, p1.z, p2.z, f1z, f2z, f3z, g0z, g1z, g2z);

			//update integrals
			integral[0] += n.x * f1x;
			integral[1] += n.x * f2x;
			integral[2] += n.y * f2y;
			integral[3] += n.z * f2z;
			integral[4] += n.x * f3x;
			integral[5] += n.y * f3y;
			integral[6] += n.z * f3z;
			integral[7] += n.x * (p0.y * g0x + p1.y * g1x + p2.y * g2x);
			integral[8] += n.y * (p0.z * g0y + p1.z * g1y + p2.z * g2y);
			integral[9] += n.z * (p0.x * g0z + p1.x * g1z + p2.x * g2z);
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

		double inverseMass{ 1.0f / mass };

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