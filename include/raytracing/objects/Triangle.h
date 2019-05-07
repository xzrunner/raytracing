#pragma once

#include "raytracing/objects/Object.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class Triangle : public Object
{
public:
	Triangle();
	Triangle(const Point3D& v0, const Point3D& v1, const Point3D& v2);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

	virtual AABB GetBoundingBox() const;

private:
	Point3D	v0, v1, v2;
	Normal	normal;

}; // Triangle

}
