#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

class ConvexPartSphere : public GeoPrimitive
{
public:
    ConvexPartSphere() {}
	ConvexPartSphere(const Point3D&	c,
					 double r,
					 double azimuth_min,	// in degrees
					 double azimuth_max,	// in degrees
					 double polar_min,		// in degrees measured from top
					 double polar_max);		// in degrees measured from top

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

private:
    Point3D m_center;   			    // center coordinates
	double 	m_radius    = 1.0;	        // sphere radius
	double 	m_phi_min   = 0.0;	        // minimum azimiuth angle in degrees measured counter clockwise from the +ve z axis
	double 	m_phi_max   = TWO_PI;	    // maximum azimiuth angle in degrees measured counter clockwise from the +ve z axis
	double 	m_theta_min = 0.0;	        // minimum polar angle in degrees measured down from the +ve y axis
	double 	m_theta_max = PI;		    // maximum polar angle in degrees measured down from the +ve y axis

	double	m_cos_theta_min = 1.0;		// stored to avoid repeated calculations
	double	m_cos_theta_max = -1.0;		// stored to avoid repeated calculations

}; // ConvexPartSphere

}