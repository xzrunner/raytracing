#pragma once

#include "raytracing/objects/Object.h"

namespace rt
{

class OpenCylinder : public Object
{
public:
	OpenCylinder() {}
	OpenCylinder(double bottom, double top, double radius);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

private:
	// bottom y value
	double m_y0 = -1.0;
	// top y value
    double m_y1 = 1.0;;

	double m_radius     = 1.0;
	double m_inv_radius = 1.0;

}; // OpenCylinder

}
