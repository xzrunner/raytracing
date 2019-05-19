#pragma once

#include "raytracing/objects/Compound.h"

namespace rt
{

class BeveledBox : public Compound
{
public:
	BeveledBox(const Point3D& bottom, const Point3D& top, float bevel_radius);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

private:
    AABB bbox;	// the bounding box

    Point3D p0;
    Point3D p1;
    float br;

}; // BeveledBox

}
