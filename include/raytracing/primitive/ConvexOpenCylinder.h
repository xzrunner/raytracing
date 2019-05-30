#pragma once

#include "raytracing/primitive/GeoPrimitive.h"

namespace rt
{

class ConvexOpenCylinder : public GeoPrimitive
{
public:
    ConvexOpenCylinder() {}
    ConvexOpenCylinder(double bottom, double top, double radius);

	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
	virtual bool ShadowHit(const Ray& ray, float& t) const override;

private:
    double y0         = -1;		// bottom y value
    double y1         = 1;		// top y value
    double radius     = 1;		// radius
    double inv_radius = 1;  	// one over the radius

}; // ConvexOpenCylinder

}