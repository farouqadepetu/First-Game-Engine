#include "FAShapesUtility.h"

namespace FAShapes
{
	void CreateBox(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles)
	{
		//Store the vertices
		vertices.push_back({ FAMath::Vector4D(-0.5f, 0.5f, 0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.0f, 0.0f) }); //0

		vertices.push_back({ FAMath::Vector4D(0.5f, 0.5f, 0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(1.0f, 0.0f) });	//1

		vertices.push_back({ FAMath::Vector4D(0.5f, -0.5f, 0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(1.0f, 1.0f) });   //2

		vertices.push_back({ FAMath::Vector4D(-0.5f, -0.5f, 0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.0f, 1.0f) });  //3

		vertices.push_back({ FAMath::Vector4D(-0.5f, 0.5f, -0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(1.0f, 0.0f) });  //4

		vertices.push_back({ FAMath::Vector4D(0.5f, 0.5f, -0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.0f, 0.0f) });   //5

		vertices.push_back({ FAMath::Vector4D(0.5f, -0.5f, -0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.0f, 1.0f) });  //6

		vertices.push_back({ FAMath::Vector4D(-0.5f, -0.5f, -0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(1.0f, 1.0f) }); //7

		//the indices of the vertices that make up each face of the box
		FAShapes::Quad(1, 0, 3, 2, triangles, vertices.data()); //front face
		FAShapes::Quad(4, 5, 6, 7, triangles, vertices.data()); //back face
		FAShapes::Quad(0, 1, 5, 4, triangles, vertices.data()); //top face
		FAShapes::Quad(7, 6, 2, 3, triangles, vertices.data()); //bottom face
		FAShapes::Quad(0, 4, 7, 3, triangles, vertices.data()); //left face
		FAShapes::Quad(5, 1, 2, 6, triangles, vertices.data()); //right face

		//For each vertex sum all the normals of the triangles they are a part of.
		for (auto& i : triangles)
		{
			FAMath::Vector4D norm(i.GetNormal());
			vertices.at(i.GetP0Index()).normal += norm;
			vertices.at(i.GetP1Index()).normal += norm;
			vertices.at(i.GetP2Index()).normal += norm;
		}

		//Normalize the normals
		for (auto& j : vertices)
		{
			j.normal = Norm(j.normal);
		}
	}

	void CreateCone(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles,
		unsigned int numVerticesPerCircle, unsigned int numCircles)
	{
		if (numCircles < 2)
			numCircles = 2;

		if (numVerticesPerCircle < 3)
			numVerticesPerCircle = 3;

		//Parameteric equations used to the produce the vertices of a cone.
		//x = (v * r) * sin(θ * u)
		//y = 0.5 - (v * r)
		//z = (v * r) * cos(θ * u)
		//radius = 1
		//θ = 360 or 2π
		//u = [0, 1]
		//v = [0, 1]
		//(u, v) are the texture coordinates for each vertex.
		//y is in the range [0.5, -0.5] so the center of the cone is the origin.

		float radius{ 1.0f };
		float u{ 0.0f };
		float v{ 0.0f };
		float uStep{ 1.0f / numVerticesPerCircle };
		float vStep{ 1.0f / numCircles };

		//Generate the vertices of the cone
		for (unsigned int i = 0; i <= numCircles; ++i)
		{
			for (unsigned int j = 0; j <= numVerticesPerCircle; ++j)
			{
				float x{ (v * radius) * sin(u * PI2) };
				float y{ 0.5f - (v * radius) };
				float z{ (v * radius) * cos(u * PI2) };

				//Last circle
				if (i == numCircles)
				{
					vertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f), FAMath::Vector4D(),
						FAMath::Vector2D((x * 0.5f) + 0.5f, (z * 0.5f) + 0.5f) });
				}
				else //all other circles
				{
					vertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f),FAMath::Vector4D(), FAMath::Vector2D(u, v) });
				}

				u += uStep;
			}
			v += vStep;
			u = 0.0f;
		}

		//Center of bottom circle.
		vertices.push_back({ FAMath::Vector4D(0.0f, -0.5f, 0.0f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.5f, 0.5f) });


		/* The vertex locations of each vertex in a quad.
		Top Left : (i * n) + j
		Top Right : (i * n) + j + 1
		Bottom Right : ((i + 1) * n) + j + 1
		Bottom Left : ((i + 1) * n) + j
		i is a circle (slice) of the sphere.
		j is a vertex of the ith circle (slice).
		n is the number of vertices per slice.
		*/

		//Make quads with the vertices.
		for (unsigned int i = 0; i < numCircles; ++i)
		{
			for (unsigned int j = 0; j < numVerticesPerCircle; ++j)
			{
				unsigned int a{ i * (numVerticesPerCircle + 1) + j }; //top left

				unsigned int b{ i * (numVerticesPerCircle + 1) + j + 1 }; //top right

				unsigned int c{ (i + 1) * (numVerticesPerCircle + 1) + j + 1 }; //bottom right

				unsigned int d{ (i + 1) * (numVerticesPerCircle + 1) + j }; //bottom left

				Quad(a, d, c, b, triangles, vertices.data());
			}
		}

		unsigned int indexCenterVertexBottom = (numCircles + 1) * (numVerticesPerCircle + 1);
		unsigned int indexLastVertexLastCircle = numCircles * (numVerticesPerCircle + 1);

		//Create triangles with the center of the bottom circle with the vertices of the bottom circle.
		for (unsigned int i = 0; i < numVerticesPerCircle; ++i)
		{
			triangles.push_back(Triangle(vertices.data(), indexCenterVertexBottom,
				indexLastVertexLastCircle + i + 1, indexLastVertexLastCircle + i));
		}

		//each vertex sums all the normals of the triangles they are a part of.
		for (auto& i : triangles)
		{
			FAMath::Vector4D norm(i.GetNormal());
			vertices[i.GetP0Index()].normal += norm;
			vertices[i.GetP1Index()].normal += norm;
			vertices[i.GetP2Index()].normal += norm;
		}

		//Sum the normals of the first and last vertex of each circle and give the result to both.
		//Sum the normals of the vertices of the first circle and give the result to each vertex.
		//The vertices of the first circle have the same vertex position but have different texture coordinates. They all need to have the 
		//same normal for lighting.
		//The same thing for the first and last vertex for each circle.
		for (unsigned int i = 0; i <= numCircles; ++i)
		{
			if (i == 0)
			{
				FAMath::Vector4D normalSum;
				for (unsigned int j = 0; j <= numVerticesPerCircle; ++j)
				{
					normalSum += vertices[j].normal;
				}

				for (unsigned int j = 0; j <= numVerticesPerCircle; ++j)
				{
					vertices[j].normal = normalSum;
				}
			}
			else
			{
				unsigned int firstV = i * (numVerticesPerCircle + 1);
				unsigned int lastV = i * (numVerticesPerCircle + 1) + numVerticesPerCircle;
				auto sumNormal = vertices[lastV].normal + vertices[firstV].normal;
				vertices[lastV].normal = sumNormal;
				vertices[firstV].normal = sumNormal;
			}
		}

		//Sum the normal of the center vertex of the bottom circle with each vertex and give the sum to the vertex.
		//This will make the normal point more downward, 
		//which means there will be better interpolation from the center to the edges of the circle.
		unsigned int indexFirstVertexLastCircle = numCircles * (numVerticesPerCircle + 1);
		unsigned int indexCenterVertexBottomCircle = (numCircles + 1) * (numVerticesPerCircle + 1);
		for (unsigned int i = 0; i <= numVerticesPerCircle; ++i)
		{
			auto sumNormal = (vertices[indexCenterVertexBottomCircle].normal * 0.25f) + vertices[indexFirstVertexLastCircle + i].normal;
			vertices[indexFirstVertexLastCircle + i].normal = sumNormal;
		}

		//Normalize the normals
		for (auto& j : vertices)
		{
			j.normal = Norm(j.normal);
		}
	}

	void CreateCylinder(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles,
		unsigned int numVerticesPerCircle, unsigned int numCircles )
	{
		if (numCircles < 2)
			numCircles = 2;

		if (numVerticesPerCircle < 3)
			numVerticesPerCircle = 3;

		//Parameteric equations used to produce the vertices of the cylinder
		//x = rsin(θ * u)
		//y = 0.5 - (v * h)
		//z = rcos(θ * u)
		//θ = 360 or 2π
		//r = 1
		//h = 1
		//u = [0, 1]
		//v = [0, 1]
		//y is in the range [0.5, -0.5] so the center of the cylinder is the origin.
		//(u, v) are the texture coordinates for each vertex.

		float u{ 0.0f };
		float v{ 0.0f };
		float uStep{ 1.0f / numVerticesPerCircle };
		float vStep{ 1.0f / numCircles };
		float h{ 1.0f };

		for (unsigned int i = 0; i <= numCircles; ++i)
		{
			for (unsigned int j = 0; j <= numVerticesPerCircle; ++j)
			{
				float x{ sin(u * PI2) };
				float y{ 0.5f - (v * h) };
				float z{ cos(u * PI2) };

				//First or last circle
				if (i == 0 || i == numCircles)
				{
					vertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f), FAMath::Vector4D(),
						FAMath::Vector2D((x * 0.5f) + 0.5f, (z * 0.5f) + 0.5f) });
				}
				else
				{
					vertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f),  FAMath::Vector4D(), FAMath::Vector2D(u, v) });
				}

				u += uStep;
			}
			u = 0.0f;
			v += vStep;
		}

		//Center of top circle.
		vertices.push_back({ FAMath::Vector4D(0.0f, 0.5f, 0.0f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.5f, 0.5f) });

		//Center of bottom circle.
		vertices.push_back({ FAMath::Vector4D(0.0f, -0.5f, 0.0f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.5f, 0.5f) });

		/* The vertex locations of each vertex in a quad.
		Top Left : (i * n) + j
		Top Right : (i * n) + j + 1
		Bottom Right : ((i + 1) * n) + j + 1
		Bottom Left : ((i + 1) * n) + j
		i is a circle (slice) of the sphere.
		j is a vertex of the ith circle (slice).
		n is the number of vertices per slice.
		*/

		//Make quads with the vertices.
		for (unsigned int i = 0; i < numCircles; ++i)
		{
			for (unsigned int j = 0; j < numVerticesPerCircle; ++j)
			{
				unsigned int a{ i * (numVerticesPerCircle + 1) + j }; //top left

				unsigned int b{ i * (numVerticesPerCircle + 1) + j + 1 }; //top right

				unsigned int c{ (i + 1) * (numVerticesPerCircle + 1) + j + 1 }; //bottom right

				unsigned int d{ (i + 1) * (numVerticesPerCircle + 1) + j }; //bottom left

				Quad(a, d, c, b, triangles, vertices.data());
			}
		}

		unsigned int indexCenterVertexTop = (numCircles + 1) * (numVerticesPerCircle + 1);
		unsigned int indexCenterVertexBottom = indexCenterVertexTop + 1;
		unsigned int indexLastVertexLastCircle = numCircles * (numVerticesPerCircle + 1);

		//Create triangles with the center of the top circle with the vertices of the top circle.
		//Create triangles with the center of the bottom circle with the vertices of the bottom circle.
		for (unsigned int i = 0; i < numVerticesPerCircle; ++i)
		{
			triangles.push_back(Triangle(vertices.data(), indexCenterVertexTop,
				i, i + 1));

			triangles.push_back(Triangle(vertices.data(), indexCenterVertexBottom,
				indexLastVertexLastCircle + i + 1, indexLastVertexLastCircle + i));
		}

		//each vertex sums all the normals of the triangles they are a part of.
		for (auto& i : triangles)
		{
			FAMath::Vector4D norm(i.GetNormal());
			vertices[i.GetP0Index()].normal += norm;
			vertices[i.GetP1Index()].normal += norm;
			vertices[i.GetP2Index()].normal += norm;
		}

		//Sum the normals of the first and last vertex of each circle and give the result to both the first and last vertex.
		for (unsigned int i = 0; i <= numCircles; ++i)
		{
			unsigned int firstV = i * (numVerticesPerCircle + 1);
			unsigned int lastV = i * (numVerticesPerCircle + 1) + numVerticesPerCircle;
			auto sumNormal = vertices[lastV].normal + vertices[firstV].normal;
			vertices[lastV].normal = sumNormal;
			vertices[firstV].normal = sumNormal;
		}

		//Normalize the normals
		for (auto& j : vertices)
		{
			j.normal = Norm(j.normal);
		}
	}

	void CreateSphere(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles,
		unsigned int numVerticesPerCircle, unsigned int numCircles)
	{
		/* Parametric equations of a sphere.
		x = rsin(Φ * v)cos(θ * u)
		y = rcos(Φ * v)
		z = rsin(Φ * v)sin(θ * u)
		θ = 360 or 2π
		Φ = 180 or π
		r = 1
		u = [0, 1]
		v = [0, 1]
		(u, v) are the texture coordinates for each vertex.
		*/

		//(u, v) for the texture coordinates for each vertex
		float u{ 0.0f };
		float v{ 0.0f };
		float uStep{ 1.0f / numVerticesPerCircle };
		float vStep{ 1.0f / numCircles };

		//Generate the vertices of the sphere
		for (unsigned int i = 0; i <= numCircles; ++i)
		{
			for (unsigned int j = 0; j <= numVerticesPerCircle; ++j)
			{
				float x{ sin(v * PI) * sin(u * PI2) };
				float y{ cos(v * PI) };
				float z{ sin(v * PI) * cos(u * PI2) };

				vertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f), FAMath::Vector4D(x, y, z, 0.0f), FAMath::Vector2D(u, v) });

				u += uStep;
			}
			v += vStep;
			u = 0.0f;
		}

		/* The vertex locations of each vertex in a quad.
		Top Left : (i * n) + j
		Top Right : (i * n) + j + 1
		Bottom Right : ((i + 1) * n) + j + 1
		Bottom Left : ((i + 1) * n) + j
		i is a circle (slice) of the sphere.
		j is a vertex of the ith circle (slice).
		n is the number of vertices per slice.
		*/

		//Make quads with the vertices.
		for (unsigned int i = 0; i < numCircles; ++i)
		{
			for (unsigned int j = 0; j < numVerticesPerCircle; ++j)
			{
				unsigned int a{ i * (numVerticesPerCircle + 1) + j }; //top left

				unsigned int b{ i * (numVerticesPerCircle + 1) + j + 1 }; //top right

				unsigned int c{ (i + 1) * (numVerticesPerCircle + 1) + j + 1 }; //bottom right

				unsigned int d{ (i + 1) * (numVerticesPerCircle + 1) + j }; //bottom left

				Quad(a, d, c, b, triangles, vertices.data());
			}
		}

		size_t numVertices{ vertices.size() };

		for (size_t i = 0; i < numVertices; ++i)
		{
			//Make each normal a unit vector.
			vertices[i].normal = Norm(vertices[i].normal);
		}
	}

	void CreatePyramid(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles)
	{
		//top vertex
		vertices.push_back({ FAMath::Vector4D(0.0f, 0.5f, 0.0f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.5f, 0.5f) });

		//base vertices
		vertices.push_back({ FAMath::Vector4D(-0.5f, -0.5f, 0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.0f, 0.0f) }); //1
		vertices.push_back({ FAMath::Vector4D(0.5f, -0.5f, 0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(1.0f, 0.0f) }); //2
		vertices.push_back({ FAMath::Vector4D(0.5f, -0.5f, -0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(1.0f, 1.0f) }); //3
		vertices.push_back({ FAMath::Vector4D(-0.5f, -0.5f, -0.5f, 1.0f), FAMath::Vector4D(), FAMath::Vector2D(0.0f, 1.0f) }); //4

		triangles.push_back(Triangle(vertices.data(), 0, 1, 2)); //front triangle
		triangles.push_back(Triangle(vertices.data(), 0, 3, 4)); //back triangle
		triangles.push_back(Triangle(vertices.data(), 0, 2, 3)); //right triangle
		triangles.push_back(Triangle(vertices.data(), 0, 4, 1)); //left triangle

		//Base
		Quad(4, 3, 2, 1, triangles, vertices.data());

		//For each vertex sum all the normals of the triangles they are a part of.
		for (auto& i : triangles)
		{
			FAMath::Vector4D norm(i.GetNormal());
			vertices.at(i.GetP0Index()).normal += norm;
			vertices.at(i.GetP1Index()).normal += norm;
			vertices.at(i.GetP2Index()).normal += norm;
		}

		//Normalize the normals
		for (auto& j : vertices)
		{
			j.normal = Norm(j.normal);
		}
	}
}