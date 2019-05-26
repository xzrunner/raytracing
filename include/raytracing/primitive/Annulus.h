#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/maths/AABB.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class Annulus : public GeoPrimitive
{
public:
    Annulus();
    Annulus(Point3D center, Normal normal, double inner_radius, double wall_thickness);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

    virtual AABB GetBoundingBox() const override { return bbox; }

private:
	Point3D center;
	Normal  normal         = Normal(0, 1, 0);
	double  inner_radius   = 1;
	double  wall_thickness = 1;

	double i_squared = 0;
	double w_squared = 4;

    AABB bbox;	// the bounding box

}; // Annulus

}