#pragma once

#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/maths/Point2D.h"

#include <vector>

namespace rt
{

class Mesh
{
public:
	std::vector<Point3D>          vertices;				// mesh vertices
    std::vector<Normal>           normals;				// average normal at each vertex;
    std::vector<Point2D>          texcoords;            // mesh texcoords
    std::vector<std::vector<int>> vertex_faces;			// the triangles shared by each vertex
	int 					      num_vertices; 		// number of vertices
	int 					      num_triangles; 		// number of triangles

}; // Mesh

}
