#pragma once

#include "raytracing/objects/GeometricObject.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class Plane : public GeometricObject
{
public:
	Plane();
	Plane(const Point3D& pos, const Normal& normal);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

private:
	Point3D m_pos;
	Normal m_normal;

}; // Plane

}
