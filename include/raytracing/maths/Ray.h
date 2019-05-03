#pragma once

#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Vector3D.h"

namespace rt
{

class Ray
{
public:
	Point3D ori;
	Vector3D dir;

	// ray depth, for reflections
	int	depth;

public:
	Ray();
	Ray(const Point3D& ori, const Vector3D& dir);

}; // Ray

inline Ray::
Ray() 
	: ori(0, 0, 0), dir(0, 0, 1) {}

inline Ray::
Ray(const Point3D& ori, const Vector3D& dir)
	: ori(ori), dir(dir) {}

}
