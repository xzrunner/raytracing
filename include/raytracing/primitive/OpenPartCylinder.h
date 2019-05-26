#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

class OpenPartCylinder : public GeoPrimitive
{
public:
	OpenPartCylinder() {}
	OpenPartCylinder(double bottom, double top, double radius, double polar_min, double polar_max);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

private:
    double m_y0         = -1.0;	    // bottom y value
    double m_y1         = 1.0;	    // top y value
    double m_radius     = 1.0;	    // radius
    double m_inv_radius = 1.0;  	// one over the radius
    double m_polar_min  = 0.0;
    double m_polar_max  = TWO_PI;

}; // OpenPartCylinder

}
