#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/maths/AABB.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

class ConvexPartTorus : public GeoPrimitive
{
public:
    ConvexPartTorus();
    ConvexPartTorus(double a, double b, double min_azimuth, double max_azimuth, double min_polar, double max_polar);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

private:
    Normal ComputeNormal(const Point3D& p) const;

private:
	double a           = 2.0;	 	// swept radius
	double b           = 0.5;	 	// tube radius
	double min_azimuth = 0;
	double max_azimuth = TWO_PI;
	double min_polar   = 0;
	double max_polar   = TWO_PI;
    AABB   bbox;	                // the bounding box

}; // ConvexPartTorus

}