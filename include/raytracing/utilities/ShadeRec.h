#pragma once

#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/RGBColor.h"

#include <memory>

namespace rt
{

class World;
class Material;

class ShadeRec
{
public:
	bool				      hit_an_object;		// Did the ray hit an object?
	std::shared_ptr<Material> material;			    // Pointer to the nearest object's material
	Point3D 			      hit_point;			// World coordinates of intersection
	Point3D				      local_hit_point;	    // World coordinates of hit point on generic object (used for texture transformations)
	Normal				      normal;				// Normal at hit point
	Ray					      ray;				    // Required for specular highlights and area lights
	int					      depth;				// recursion depth
	Vector3D			      dir;				    // for area lights
	double				      t;					// ray parameter
	const World&		      w;					// World reference
	RGBColor			      m_color;

	float u;
	float v;

public:
	ShadeRec(const World& world)
		: hit_an_object(false), material(0), depth(0), t(0), w(world), u(0), v(0) {}

}; // ShadeRec

}
